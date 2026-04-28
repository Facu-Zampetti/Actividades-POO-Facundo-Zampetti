#include "MonitorService.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>

MonitorService::MonitorService(QObject *parent)
    : QObject(parent),
      m_network(new QNetworkAccessManager(this)),
      m_endpoint("http://173.212.234.190:8080/health") {
    qRegisterMetaType<MonitorData>("MonitorData");

    connect(&m_timer, &QTimer::timeout, this, &MonitorService::refreshNow);
    connect(m_network, &QNetworkAccessManager::finished, this, &MonitorService::onNetworkReply);
}

void MonitorService::setEndpoint(const QString &url) {
    m_endpoint = url.trimmed();
}

void MonitorService::setIntervalSeconds(int seconds) {
    m_intervalSeconds = qMax(1, seconds);
    if (m_timer.isActive()) {
        m_timer.start(m_intervalSeconds * 1000);
    }
}

void MonitorService::setThresholds(int memoryPercent, int diskPercent) {
    m_memoryThreshold = qBound(1, memoryPercent, 100);
    m_diskThreshold = qBound(1, diskPercent, 100);
}

QString MonitorService::endpoint() const {
    return m_endpoint;
}

int MonitorService::intervalSeconds() const {
    return m_intervalSeconds;
}

int MonitorService::memoryThreshold() const {
    return m_memoryThreshold;
}

int MonitorService::diskThreshold() const {
    return m_diskThreshold;
}

void MonitorService::refreshNow() {
    const QUrl url(m_endpoint);
    if (!url.isValid() || m_endpoint.isEmpty()) {
        MonitorData data;
        data.status = "DOWN";
        data.hasError = true;
        data.timestamp = QDateTime::currentDateTime();
        data.errorMessage = "Endpoint invalido";
        emit metricsUpdated(data);
        emit eventOccurred(QString("%1 - Error: endpoint invalido").arg(data.timestamp.toString("yyyy-MM-dd HH:mm:ss")));
        return;
    }

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    m_network->get(request);
}

void MonitorService::startAutoRefresh() {
    m_timer.start(m_intervalSeconds * 1000);
    emit eventOccurred(QString("%1 - Auto chequeo iniciado (%2 s)")
                           .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"))
                           .arg(m_intervalSeconds));
}

void MonitorService::stopAutoRefresh() {
    m_timer.stop();
    emit eventOccurred(QString("%1 - Auto chequeo detenido")
                           .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")));
}

void MonitorService::onNetworkReply(QNetworkReply *reply) {
    MonitorData data;
    data.timestamp = QDateTime::currentDateTime();

    if (reply->error() != QNetworkReply::NoError) {
        data.status = "DOWN";
        data.hasError = true;
        data.errorMessage = reply->errorString();
        emit metricsUpdated(data);
        emit eventOccurred(QString("%1 - Falla de conexion: %2")
                               .arg(data.timestamp.toString("yyyy-MM-dd HH:mm:ss"))
                               .arg(data.errorMessage));
        reply->deleteLater();
        return;
    }

    data = parseResponse(reply->readAll());
    if (!data.timestamp.isValid()) {
        data.timestamp = QDateTime::currentDateTime();
    }

    data.status = deriveStatus(data);
    emit metricsUpdated(data);

    emit eventOccurred(QString("%1 - Estado %2 | load %.2f | mem %.1f%% | disk %.1f%%")
                           .arg(data.timestamp.toString("yyyy-MM-dd HH:mm:ss"))
                           .arg(data.status)
                           .arg(data.loadAvg)
                           .arg(data.memoryUsedPercent)
                           .arg(data.diskUsedPercent));

    reply->deleteLater();
}

MonitorData MonitorService::parseResponse(const QByteArray &payload) const {
    MonitorData data;
    data.timestamp = QDateTime::currentDateTime();

    QJsonParseError parseError;
    const QJsonDocument doc = QJsonDocument::fromJson(payload, &parseError);
    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        data.status = "DOWN";
        data.hasError = true;
        data.errorMessage = "JSON invalido";
        return data;
    }

    const QJsonObject root = doc.object();
    const QJsonObject metrics = root.value("metrics").toObject();

    const QString rawStatus = root.value("status").toString("ok").toLower();
    data.status = rawStatus;

    data.uptimeSeconds = readNumber(root, "uptime_seconds", readNumber(metrics, "uptime_seconds", 0.0));
    data.loadAvg = readNumber(root, "load_avg", readNumber(metrics, "load_avg", 0.0));
    data.memoryUsedPercent = readNumber(root, "memory_used_percent", readNumber(metrics, "memory_used_percent", 0.0));
    data.diskUsedPercent = readNumber(root, "disk_used_percent", readNumber(metrics, "disk_used_percent", 0.0));

    const QString ts = root.value("timestamp").toString();
    if (!ts.isEmpty()) {
        QDateTime parsed = QDateTime::fromString(ts, Qt::ISODate);
        if (!parsed.isValid()) {
            parsed = QDateTime::fromString(ts, "yyyy-MM-dd HH:mm:ss");
        }
        if (parsed.isValid()) {
            data.timestamp = parsed;
        }
    }

    return data;
}

QString MonitorService::deriveStatus(const MonitorData &data) const {
    if (data.hasError) {
        return "DOWN";
    }

    const bool metricAlert = data.memoryUsedPercent >= m_memoryThreshold || data.diskUsedPercent >= m_diskThreshold;

    const QString endpointStatus = data.status.toLower();
    if (endpointStatus == "down") {
        return "DOWN";
    }
    if (endpointStatus == "alert" || metricAlert) {
        return "ALERT";
    }
    return "OK";
}

double MonitorService::readNumber(const QJsonObject &obj, const QString &key, double fallback) {
    if (!obj.contains(key)) {
        return fallback;
    }

    const QJsonValue value = obj.value(key);
    if (value.isDouble()) {
        return value.toDouble();
    }
    if (value.isString()) {
        bool ok = false;
        const double number = value.toString().toDouble(&ok);
        if (ok) {
            return number;
        }
    }

    return fallback;
}
