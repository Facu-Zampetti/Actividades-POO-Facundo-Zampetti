#include "realtimesyncservice.h"

#include <QJsonDocument>
#include <QNetworkReply>
#include <QNetworkRequest>

static int findStrokeIndexById(const QVector<Stroke>& strokes, const QString& id) {
    for (int i = 0; i < strokes.size(); ++i) {
        if (strokes[i].id == id) return i;
    }
    return -1;
}

RealtimeSyncService::RealtimeSyncService(const QUrl& baseUrl, QObject* parent)
    : QObject(parent), m_baseUrl(baseUrl)
{
    m_pollTimer.setInterval(900);
    connect(&m_pollTimer, &QTimer::timeout, this, &RealtimeSyncService::pollCanvas);

    m_pushTimer.setInterval(220);
    m_pushTimer.setSingleShot(true);
    connect(&m_pushTimer, &QTimer::timeout, this, &RealtimeSyncService::pushLocalDelta);
}

bool RealtimeSyncService::isConnected() const {
    return m_connected;
}

void RealtimeSyncService::connectToServer() {
    if (m_connected) return;
    m_connected = true;
    emit connectionStateChanged("Realtime HTTP conectado");
    m_firstSync = true;
    m_pollTimer.start();
    pollCanvas();
}

void RealtimeSyncService::disconnectFromServer() {
    if (!m_connected) return;
    m_connected = false;
    m_pollTimer.stop();
    m_pushTimer.stop();
    m_pushInFlight = false;
    m_pushQueued = false;
    emit connectionStateChanged("Realtime HTTP desconectado");
}

void RealtimeSyncService::sendStrokeBegin(const Stroke& stroke) {
    m_localStrokes.insert(stroke.id, stroke);
    queuePush();
}

void RealtimeSyncService::sendStrokePoints(const QString& strokeId, const QVector<QPointF>& points) {
    if (points.isEmpty()) return;
    auto it = m_localStrokes.find(strokeId);
    if (it == m_localStrokes.end()) return;

    Stroke& s = it.value();
    s.points.reserve(s.points.size() + points.size());
    for (const QPointF& p : points) {
        s.points.push_back(StrokePoint{p});
    }
    queuePush();
}

void RealtimeSyncService::sendStrokeEnd(const QString& strokeId) {
    Q_UNUSED(strokeId);
    queuePush();
}

void RealtimeSyncService::queuePush() {
    if (!m_connected) return;
    m_pushTimer.start();
}

void RealtimeSyncService::mergeLocalIntoRemote(DrawingModel& remote) const {
    QVector<Stroke>& remoteStrokes = remote.strokes();

    for (auto it = m_localStrokes.constBegin(); it != m_localStrokes.constEnd(); ++it) {
        const Stroke& local = it.value();
        const int idx = findStrokeIndexById(remoteStrokes, local.id);

        if (idx < 0) {
            remoteStrokes.push_back(local);
            continue;
        }

        Stroke& merged = remoteStrokes[idx];
        merged.author = local.author;
        merged.color = local.color;
        merged.width = local.width;
        merged.eraser = local.eraser;

        if (merged.points.size() < local.points.size()) {
            for (int i = merged.points.size(); i < local.points.size(); ++i) {
                merged.points.push_back(local.points[i]);
            }
        }
    }
}

void RealtimeSyncService::pushLocalDelta() {
    if (!m_connected) return;
    if (m_pushInFlight) {
        m_pushQueued = true;
        return;
    }

    m_pushInFlight = true;

    QUrl getUrl = m_baseUrl;
    getUrl.setPath("/canvas");

    QNetworkRequest getReq(getUrl);
    getReq.setHeader(QNetworkRequest::UserAgentHeader, "QtCanvas/1.0");

    QNetworkReply* getReply = m_nam.get(getReq);
    connect(getReply, &QNetworkReply::finished, this, [this, getReply, getUrl]() {
        getReply->deleteLater();

        if (getReply->error() != QNetworkReply::NoError) {
            emit connectionStateChanged("Realtime push read error: " + getReply->errorString());
            m_pushInFlight = false;
            if (m_pushQueued) {
                m_pushQueued = false;
                queuePush();
            }
            return;
        }

        DrawingModel merged;
        QJsonParseError pe;
        const QJsonDocument doc = QJsonDocument::fromJson(getReply->readAll(), &pe);
        if (pe.error != QJsonParseError::NoError || !doc.isObject()) {
            emit connectionStateChanged("Realtime push read error: JSON invalido");
            m_pushInFlight = false;
            if (m_pushQueued) {
                m_pushQueued = false;
                queuePush();
            }
            return;
        }

        QString err;
        if (!merged.fromJson(doc.object(), &err)) {
            emit connectionStateChanged("Realtime push read error: modelo invalido");
            m_pushInFlight = false;
            if (m_pushQueued) {
                m_pushQueued = false;
                queuePush();
            }
            return;
        }

        mergeLocalIntoRemote(merged);

        QNetworkRequest postReq(getUrl);
        postReq.setHeader(QNetworkRequest::UserAgentHeader, "QtCanvas/1.0");
        postReq.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        const QByteArray payload = QJsonDocument(merged.toJson()).toJson(QJsonDocument::Compact);
        QNetworkReply* postReply = m_nam.post(postReq, payload);

        connect(postReply, &QNetworkReply::finished, this, [this, postReply]() {
            postReply->deleteLater();

            if (postReply->error() != QNetworkReply::NoError) {
                emit connectionStateChanged("Realtime push error: " + postReply->errorString());
            }

            m_pushInFlight = false;
            if (m_pushQueued) {
                m_pushQueued = false;
                queuePush();
            }
        });
    });
}

void RealtimeSyncService::pollCanvas() {
    if (!m_connected) return;

    QUrl url = m_baseUrl;
    url.setPath("/canvas");

    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::UserAgentHeader, "QtCanvas/1.0");

    QNetworkReply* reply = m_nam.get(req);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();

        if (reply->error() != QNetworkReply::NoError) {
            emit connectionStateChanged("Realtime HTTP error: " + reply->errorString());
            return;
        }

        QJsonParseError pe;
        const QByteArray body = reply->readAll();
        const QJsonDocument doc = QJsonDocument::fromJson(body, &pe);
        if (pe.error != QJsonParseError::NoError || !doc.isObject()) {
            emit connectionStateChanged("Realtime HTTP: JSON invalido");
            return;
        }

        DrawingModel remote;
        QString err;
        if (!remote.fromJson(doc.object(), &err)) {
            emit connectionStateChanged("Realtime HTTP: modelo invalido");
            return;
        }

        processRemoteSnapshot(remote);
        if (m_firstSync) {
            m_firstSync = false;
            emit connectionStateChanged("Realtime HTTP sincronizado");
        }
    });
}

void RealtimeSyncService::processRemoteSnapshot(const DrawingModel& remote) {
    if (m_firstSync) {
        for (const Stroke& s : remote.strokes()) {
            m_knownPointCount.insert(s.id, s.points.size());
        }
        return;
    }

    for (const Stroke& s : remote.strokes()) {
        const int known = m_knownPointCount.value(s.id, -1);
        if (known < 0) {
            if (s.author == m_localAuthor) {
                m_knownPointCount.insert(s.id, s.points.size());
                continue;
            }

            Stroke meta = s;
            meta.points.clear();
            emit remoteStrokeBegin(meta);

            if (!s.points.isEmpty()) {
                QVector<QPointF> pts;
                pts.reserve(s.points.size());
                for (const StrokePoint& sp : s.points) {
                    pts.push_back(sp.p);
                }
                emit remoteStrokePoints(s.id, pts);
            }

            emit remoteStrokeEnd(s.id);
            m_knownPointCount.insert(s.id, s.points.size());
            continue;
        }

        if (s.points.size() > known) {
            if (s.author == m_localAuthor) {
                m_knownPointCount[s.id] = s.points.size();
                continue;
            }

            QVector<QPointF> delta;
            delta.reserve(s.points.size() - known);
            for (int i = known; i < s.points.size(); ++i) {
                delta.push_back(s.points[i].p);
            }
            if (!delta.isEmpty()) {
                emit remoteStrokePoints(s.id, delta);
                m_knownPointCount[s.id] = s.points.size();
            }
        }
    }
}