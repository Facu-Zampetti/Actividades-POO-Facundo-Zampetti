#include "MainWindow.h"

#include "MonitorService.h"

#include <QDateTime>
#include <QFormLayout>
#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QProgressBar>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_service(new MonitorService(this)) {
    buildUi();
    applyStyle();

    m_endpointEdit->setText(m_service->endpoint());
    m_intervalSpin->setValue(m_service->intervalSeconds());
    m_memoryThresholdSpin->setValue(m_service->memoryThreshold());
    m_diskThresholdSpin->setValue(m_service->diskThreshold());

    connect(m_applyButton, &QPushButton::clicked, this, &MainWindow::applySettings);
    connect(m_refreshButton, &QPushButton::clicked, m_service, &MonitorService::refreshNow);
    connect(m_service, &MonitorService::metricsUpdated, this, &MainWindow::onMetricsUpdated);
    connect(m_service, &MonitorService::eventOccurred, this, &MainWindow::onEvent);

    applySettings();
    m_service->refreshNow();
}

void MainWindow::applySettings() {
    m_service->setEndpoint(m_endpointEdit->text());
    m_service->setIntervalSeconds(m_intervalSpin->value());
    m_service->setThresholds(m_memoryThresholdSpin->value(), m_diskThresholdSpin->value());
    m_service->startAutoRefresh();

    pushHistory(QString("%1 - Configuracion aplicada: intervalo %2s, umbrales mem=%3%% disk=%4%%")
                    .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"))
                    .arg(m_intervalSpin->value())
                    .arg(m_memoryThresholdSpin->value())
                    .arg(m_diskThresholdSpin->value()));
}

void MainWindow::onMetricsUpdated(const MonitorData &data) {
    m_uptimeValue->setText(formatUptime(data.uptimeSeconds));
    m_loadValue->setText(QString::number(data.loadAvg, 'f', 2));
    m_memoryValue->setText(QString("%1%")
                               .arg(QString::number(data.memoryUsedPercent, 'f', 1)));
    m_diskValue->setText(QString("%1%")
                             .arg(QString::number(data.diskUsedPercent, 'f', 1)));

    m_memoryBar->setValue(static_cast<int>(data.memoryUsedPercent));
    m_diskBar->setValue(static_cast<int>(data.diskUsedPercent));

    m_lastCheckValue->setText(data.timestamp.toString("yyyy-MM-dd HH:mm:ss"));

    QString badgeColor = "#22c55e";
    if (data.status == "ALERT") {
        badgeColor = "#f59e0b";
    } else if (data.status == "DOWN") {
        badgeColor = "#ef4444";
    }

    m_statusValue->setText(data.status);
    m_statusValue->setStyleSheet(QString("font-weight: 800; color: %1; font-size: 22px;").arg(badgeColor));
}

void MainWindow::onEvent(const QString &text) {
    pushHistory(text);
}

void MainWindow::buildUi() {
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout *root = new QVBoxLayout(central);
    root->setContentsMargins(16, 16, 16, 16);
    root->setSpacing(14);

    QLabel *title = new QLabel("Panel de Monitoreo VPS", this);
    title->setObjectName("title");

    QLabel *subtitle = new QLabel("Inspirado en tableros profesionales (Grafana/Datadog)", this);
    subtitle->setObjectName("subtitle");

    root->addWidget(title);
    root->addWidget(subtitle);

    QFrame *controlCard = new QFrame(this);
    controlCard->setObjectName("card");

    QGridLayout *controls = new QGridLayout(controlCard);
    controls->setContentsMargins(16, 14, 16, 14);
    controls->setHorizontalSpacing(12);
    controls->setVerticalSpacing(10);

    m_endpointEdit = new QLineEdit(this);
    m_endpointEdit->setPlaceholderText("http://tu-vps:8080/health");

    m_intervalSpin = new QSpinBox(this);
    m_intervalSpin->setRange(5, 3600);
    m_intervalSpin->setSuffix(" s");

    m_memoryThresholdSpin = new QSpinBox(this);
    m_memoryThresholdSpin->setRange(40, 99);
    m_memoryThresholdSpin->setSuffix(" %");

    m_diskThresholdSpin = new QSpinBox(this);
    m_diskThresholdSpin->setRange(40, 99);
    m_diskThresholdSpin->setSuffix(" %");

    m_applyButton = new QPushButton("Aplicar configuracion", this);
    m_refreshButton = new QPushButton("Refrescar ahora", this);

    controls->addWidget(new QLabel("Endpoint:"), 0, 0);
    controls->addWidget(m_endpointEdit, 0, 1, 1, 5);
    controls->addWidget(new QLabel("Intervalo de chequeo:"), 1, 0);
    controls->addWidget(m_intervalSpin, 1, 1);
    controls->addWidget(new QLabel("Umbral Memoria:"), 1, 2);
    controls->addWidget(m_memoryThresholdSpin, 1, 3);
    controls->addWidget(new QLabel("Umbral Disco:"), 1, 4);
    controls->addWidget(m_diskThresholdSpin, 1, 5);
    controls->addWidget(m_applyButton, 2, 4);
    controls->addWidget(m_refreshButton, 2, 5);

    root->addWidget(controlCard);

    QFrame *metricsCard = new QFrame(this);
    metricsCard->setObjectName("card");

    QGridLayout *metrics = new QGridLayout(metricsCard);
    metrics->setContentsMargins(16, 16, 16, 16);
    metrics->setHorizontalSpacing(24);
    metrics->setVerticalSpacing(12);

    QLabel *statusLabel = new QLabel("Estado general", this);
    m_statusValue = new QLabel("-", this);

    QLabel *uptimeLabel = new QLabel("Uptime", this);
    m_uptimeValue = new QLabel("-", this);

    QLabel *loadLabel = new QLabel("Carga promedio", this);
    m_loadValue = new QLabel("-", this);

    QLabel *memLabel = new QLabel("Uso de memoria", this);
    m_memoryValue = new QLabel("-", this);
    m_memoryBar = new QProgressBar(this);
    m_memoryBar->setRange(0, 100);

    QLabel *diskLabel = new QLabel("Uso de disco", this);
    m_diskValue = new QLabel("-", this);
    m_diskBar = new QProgressBar(this);
    m_diskBar->setRange(0, 100);

    QLabel *lastCheckLabel = new QLabel("Ultimo chequeo", this);
    m_lastCheckValue = new QLabel("-", this);

    metrics->addWidget(statusLabel, 0, 0);
    metrics->addWidget(m_statusValue, 0, 1);
    metrics->addWidget(uptimeLabel, 1, 0);
    metrics->addWidget(m_uptimeValue, 1, 1);
    metrics->addWidget(loadLabel, 2, 0);
    metrics->addWidget(m_loadValue, 2, 1);

    metrics->addWidget(memLabel, 0, 2);
    metrics->addWidget(m_memoryValue, 0, 3);
    metrics->addWidget(m_memoryBar, 1, 2, 1, 2);

    metrics->addWidget(diskLabel, 2, 2);
    metrics->addWidget(m_diskValue, 2, 3);
    metrics->addWidget(m_diskBar, 3, 2, 1, 2);

    metrics->addWidget(lastCheckLabel, 4, 0);
    metrics->addWidget(m_lastCheckValue, 4, 1, 1, 3);

    root->addWidget(metricsCard);

    QFrame *historyCard = new QFrame(this);
    historyCard->setObjectName("card");
    QVBoxLayout *historyLayout = new QVBoxLayout(historyCard);
    historyLayout->setContentsMargins(16, 14, 16, 14);
    historyLayout->addWidget(new QLabel("Historial de eventos (ultimos 25)", this));

    m_historyList = new QListWidget(this);
    historyLayout->addWidget(m_historyList);

    root->addWidget(historyCard, 1);
}

void MainWindow::applyStyle() {
    setStyleSheet(
        "QMainWindow, QWidget {"
        " background: #f3f5f8;"
        " color: #1f2937;"
        " font-family: 'Segoe UI';"
        " font-size: 13px;"
        "}"
        "QFrame#card {"
        " background: #ffffff;"
        " border: 1px solid #dde3ea;"
        " border-radius: 12px;"
        "}"
        "QLabel#title {"
        " font-size: 28px;"
        " font-weight: 800;"
        " color: #0f172a;"
        "}"
        "QLabel#subtitle {"
        " color: #475569;"
        " margin-bottom: 4px;"
        "}"
        "QLineEdit, QSpinBox, QListWidget, QProgressBar {"
        " border: 1px solid #cbd5e1;"
        " border-radius: 8px;"
        " background: #ffffff;"
        " min-height: 30px;"
        " padding: 2px 8px;"
        "}"
        "QPushButton {"
        " border: 0;"
        " border-radius: 8px;"
        " background: #0ea5e9;"
        " color: #ffffff;"
        " font-weight: 700;"
        " min-height: 32px;"
        " padding: 0 12px;"
        "}"
        "QPushButton:hover {"
        " background: #0284c7;"
        "}"
        "QProgressBar {"
        " text-align: center;"
        "}"
        "QProgressBar::chunk {"
        " border-radius: 7px;"
        " background: #22c55e;"
        "}"
    );
}

void MainWindow::pushHistory(const QString &line) {
    m_historyList->insertItem(0, line);
    while (m_historyList->count() > 25) {
        delete m_historyList->takeItem(m_historyList->count() - 1);
    }
}

QString MainWindow::formatUptime(double seconds) {
    const int total = qMax(0, static_cast<int>(seconds));
    const int days = total / 86400;
    const int hours = (total % 86400) / 3600;
    const int minutes = (total % 3600) / 60;

    return QString("%1d %2h %3m").arg(days).arg(hours).arg(minutes);
}
