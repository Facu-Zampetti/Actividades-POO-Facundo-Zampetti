#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QLabel;
class QLineEdit;
class QListWidget;
class QProgressBar;
class QPushButton;
class QSpinBox;

struct MonitorData;
class MonitorService;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void applySettings();
    void onMetricsUpdated(const MonitorData &data);
    void onEvent(const QString &text);

private:
    void buildUi();
    void applyStyle();
    void pushHistory(const QString &line);
    static QString formatUptime(double seconds);

    MonitorService *m_service = nullptr;

    QLineEdit *m_endpointEdit = nullptr;
    QSpinBox *m_intervalSpin = nullptr;
    QSpinBox *m_memoryThresholdSpin = nullptr;
    QSpinBox *m_diskThresholdSpin = nullptr;
    QPushButton *m_applyButton = nullptr;
    QPushButton *m_refreshButton = nullptr;

    QLabel *m_statusValue = nullptr;
    QLabel *m_uptimeValue = nullptr;
    QLabel *m_loadValue = nullptr;
    QLabel *m_memoryValue = nullptr;
    QLabel *m_diskValue = nullptr;
    QLabel *m_lastCheckValue = nullptr;

    QProgressBar *m_memoryBar = nullptr;
    QProgressBar *m_diskBar = nullptr;
    QListWidget *m_historyList = nullptr;
};

#endif // MAINWINDOW_H
