#ifndef MONITORSERVICE_H
#define MONITORSERVICE_H

#include <QObject>
#include <QDateTime>
#include <QTimer>

class QNetworkAccessManager;
class QNetworkReply;
class QJsonObject;

struct MonitorData {
    QString status;
    double uptimeSeconds = 0.0;
    double loadAvg = 0.0;
    double memoryUsedPercent = 0.0;
    double diskUsedPercent = 0.0;
    QDateTime timestamp;
    bool hasError = false;
    QString errorMessage;
};

class MonitorService : public QObject {
    Q_OBJECT
public:
    explicit MonitorService(QObject *parent = nullptr);

    void setEndpoint(const QString &url);
    void setIntervalSeconds(int seconds);
    void setThresholds(int memoryPercent, int diskPercent);

    QString endpoint() const;
    int intervalSeconds() const;
    int memoryThreshold() const;
    int diskThreshold() const;

public slots:
    void refreshNow();
    void startAutoRefresh();
    void stopAutoRefresh();

signals:
    void metricsUpdated(const MonitorData &data);
    void eventOccurred(const QString &eventText);

private slots:
    void onNetworkReply(QNetworkReply *reply);

private:
    MonitorData parseResponse(const QByteArray &payload) const;
    QString deriveStatus(const MonitorData &data) const;
    static double readNumber(const QJsonObject &obj, const QString &key, double fallback);

    QNetworkAccessManager *m_network = nullptr;
    QTimer m_timer;
    QString m_endpoint;
    int m_intervalSeconds = 10;
    int m_memoryThreshold = 85;
    int m_diskThreshold = 90;
};

Q_DECLARE_METATYPE(MonitorData)

#endif // MONITORSERVICE_H
