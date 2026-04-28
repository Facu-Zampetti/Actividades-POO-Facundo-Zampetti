#pragma once

#include <QObject>
#include <QUrl>
#include <QTimer>
#include <QHash>
#include <QNetworkAccessManager>

#include "drawingmodel.h"

class RealtimeSyncService : public QObject {
    Q_OBJECT
public:
    explicit RealtimeSyncService(const QUrl& baseUrl, QObject* parent = nullptr);

    void connectToServer();
    void disconnectFromServer();
    bool isConnected() const;
    void setLocalAuthor(const QString& author) { m_localAuthor = author; }

    // Streaming API
    void sendStrokeBegin(const Stroke& stroke);
    void sendStrokePoints(const QString& strokeId, const QVector<QPointF>& points);
    void sendStrokeEnd(const QString& strokeId);

signals:
    void connectionStateChanged(QString state);

    // Remoto -> UI
    void remoteStrokeBegin(const Stroke& stroke);
    void remoteStrokePoints(const QString& strokeId, const QVector<QPointF>& points);
    void remoteStrokeEnd(const QString& strokeId);

private slots:
    void pollCanvas();
    void pushLocalDelta();

private:
    void queuePush();
    void mergeLocalIntoRemote(DrawingModel& remote) const;
    void processRemoteSnapshot(const DrawingModel& remote);

private:
    QUrl m_baseUrl;
    QNetworkAccessManager m_nam;
    QTimer m_pollTimer;
    QTimer m_pushTimer;
    bool m_connected = false;
    QHash<QString, int> m_knownPointCount;
    QHash<QString, Stroke> m_localStrokes;
    QString m_localAuthor;
    bool m_pushInFlight = false;
    bool m_pushQueued = false;
    bool m_firstSync = true;
};