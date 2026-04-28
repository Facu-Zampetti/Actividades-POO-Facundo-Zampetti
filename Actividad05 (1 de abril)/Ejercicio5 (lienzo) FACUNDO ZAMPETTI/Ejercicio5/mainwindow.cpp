#include "mainwindow.h"

#include "canvaswidget.h"
#include "syncservice.h"
#include "realtimesyncservice.h"

#include <QAction>
#include <QMessageBox>
#include <QStatusBar>
#include <QToolBar>
#include <QUuid>
#include <QUrl>

static int findStrokeIndexById(const DrawingModel& model, const QString& id) {
    for (int i = 0; i < model.strokes().size(); ++i) {
        if (model.strokes()[i].id == id) return i;
    }
    return -1;
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle("Lienzo colaborativo en tiempo real");
    resize(1000, 700);

    const QString user = qEnvironmentVariable("USERNAME", "user");
    const QString shortId = QUuid::createUuid().toString(QUuid::WithoutBraces).left(8);
    m_localAuthor = user + "-" + shortId;

    m_canvas = new CanvasWidget(this);
    m_canvas->setLocalAuthor(m_localAuthor);
    setCentralWidget(m_canvas);

    m_sync = new SyncService(QUrl("http://173.212.234.190:8082"), this);
    m_rt = new RealtimeSyncService(QUrl("http://173.212.234.190:8082"), this);
    m_rt->setLocalAuthor(m_localAuthor);

    auto* tb = addToolBar("Toolbar");
    tb->setMovable(false);
    QAction* actSave = tb->addAction("Guardar");

    tb->setStyleSheet(R"(
        QToolBar { background: #f2f2f2; spacing: 8px; padding: 6px; }
        QToolButton {
            background: #0078d4; color: white;
            border: none; border-radius: 3px;
            font: 600 13px "Segoe UI";
            padding: 8px 14px;
        }
        QToolButton:hover { background: #106ebe; }
        QToolButton:pressed { background: #005a9e; }
    )");

    connect(m_rt, &RealtimeSyncService::connectionStateChanged, this, [this](const QString& s) {
        statusBar()->showMessage(s, 3000);
    });

    connect(m_sync, &SyncService::loadFinished, this, [this](bool ok, const QString& msg) {
        statusBar()->showMessage(msg, 4000);
        if (!ok) QMessageBox::warning(this, "Carga desde VPS", msg);

        m_canvas->update();
        m_rt->connectToServer();
    });

    connect(m_sync, &SyncService::saveFinished, this, [this](bool ok, const QString& msg) {
        statusBar()->showMessage(msg, 4000);
        if (!ok) QMessageBox::warning(this, "Guardar en VPS", msg);
    });

    connect(actSave, &QAction::triggered, this, [this]() {
        statusBar()->showMessage("Guardando en VPS (HTTP)...");
        m_sync->saveCanvas(&m_canvas->model());
    });

    connect(m_canvas, &CanvasWidget::strokeStarted, this, [this](const Stroke& s) {
        m_rt->sendStrokeBegin(s);
    });

    connect(m_canvas, &CanvasWidget::strokePointsChunk, this, [this](const QString& id, const QVector<QPointF>& pts) {
        m_rt->sendStrokePoints(id, pts);
    });

    connect(m_canvas, &CanvasWidget::strokeEnded, this, [this](const QString& id) {
        m_rt->sendStrokeEnd(id);
    });

    connect(m_rt, &RealtimeSyncService::remoteStrokeBegin, this, [this](const Stroke& s) {
        if (s.author == m_localAuthor) return;
        if (findStrokeIndexById(m_canvas->model(), s.id) >= 0) return;

        const int idx = m_canvas->model().strokes().size();
        m_canvas->model().strokes().push_back(s);
        m_remoteInProgressIndex.insert(s.id, idx);
        m_canvas->update();
    });

    connect(m_rt, &RealtimeSyncService::remoteStrokePoints, this, [this](const QString& id, const QVector<QPointF>& pts) {
        if (pts.isEmpty()) return;

        int idx = m_remoteInProgressIndex.value(id, -1);
        if (idx < 0) {
            idx = findStrokeIndexById(m_canvas->model(), id);
        }
        if (idx < 0 || idx >= m_canvas->model().strokes().size()) return;

        Stroke& st = m_canvas->model().strokes()[idx];
        if (st.author == m_localAuthor) return;

        st.points.reserve(st.points.size() + pts.size());
        for (const QPointF& p : pts) {
            st.points.push_back(StrokePoint{p});
        }
        m_canvas->update();
    });

    connect(m_rt, &RealtimeSyncService::remoteStrokeEnd, this, [this](const QString& id) {
        m_remoteInProgressIndex.remove(id);
    });

    statusBar()->showMessage("Cargando canvas (HTTP)...");
    m_sync->loadCanvas(&m_canvas->model());
}
