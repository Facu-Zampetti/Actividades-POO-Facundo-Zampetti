#include "mainwindow.h"
#include "taskdialog.h"
#include "taskmanager.h"

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QComboBox>
#include <QTextEdit>
#include <QFile>
#include <QTextStream>
#include <QFrame>

// globales
static QComboBox *g_statusFilter;
static QComboBox *g_priorityFilter;
static QTextEdit *g_historyView;

MainWindow::MainWindow(QString username, QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("TP Planner - " + username);
    resize(900, 600);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel *title = new QLabel("📋 Tablero de Trabajos");
    title->setAlignment(Qt::AlignCenter);

    // 🎨 estilos generales
    setStyleSheet(R"(
        QWidget {
            background-color: #f5f7fa;
            font-family: Segoe UI;
        }
        QLabel {
            font-size: 14px;
        }
        QPushButton {
            padding: 6px 10px;
            border-radius: 8px;
            background-color: #4facfe;
            color: white;
        }
        QPushButton:hover {
            background-color: #00c6ff;
        }
        QComboBox {
            padding: 5px;
            border-radius: 6px;
        }
        QTextEdit {
            border-radius: 10px;
            background: white;
        }
    )");

    grid = new QGridLayout();

    // filtros
    g_statusFilter = new QComboBox();
    g_statusFilter->addItems({"Todos", "Pendiente", "En progreso", "Finalizado"});

    g_priorityFilter = new QComboBox();
    g_priorityFilter->addItems({"Todas", "Baja", "Media", "Alta"});

    // historial
    g_historyView = new QTextEdit();
    g_historyView->setReadOnly(true);

    QPushButton *addBtn = new QPushButton("➕ Agregar Trabajo");

    connect(addBtn, &QPushButton::clicked, [=]() {
        Task t;
        t.title = "Nuevo TP";
        t.status = "Pendiente";
        t.priority = "Media";
        t.notes = "";

        manager.addTask(t);
        refreshUI();
    });

    mainLayout->addWidget(title);
    mainLayout->addWidget(g_statusFilter);
    mainLayout->addWidget(g_priorityFilter);
    mainLayout->addWidget(addBtn);
    mainLayout->addLayout(grid);
    mainLayout->addWidget(new QLabel("📜 Historial"));
    mainLayout->addWidget(g_historyView);

    manager.load();
    refreshUI();

    connect(g_statusFilter, &QComboBox::currentTextChanged, this, [=]() {
        refreshUI();
    });

    connect(g_priorityFilter, &QComboBox::currentTextChanged, this, [=]() {
        refreshUI();
    });
}

void loadHistory()
{
    QFile file("history.txt");
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        g_historyView->setText(in.readAll());
    }
}

void MainWindow::refreshUI()
{
    QLayoutItem *item;
    while ((item = grid->takeAt(0)) != nullptr) {
        if (item->widget()) delete item->widget();
        delete item;
    }

    QString statusFilter = g_statusFilter->currentText();
    QString priorityFilter = g_priorityFilter->currentText();

    for (int i = 0; i < manager.tasks.size(); i++) {

        Task t = manager.tasks[i];

        if (statusFilter != "Todos" && t.status != statusFilter) continue;
        if (priorityFilter != "Todas" && t.priority != priorityFilter) continue;

        // 🎨 tarjeta visual
        QFrame *card = new QFrame();
        card->setStyleSheet("background:white; border-radius:10px; padding:10px;");
        QHBoxLayout *cardLayout = new QHBoxLayout(card);

        QLabel *taskLabel = new QLabel(
            "📌 " + t.title + " | " + t.status + " | " + t.priority
            );

        QPushButton *editBtn = new QPushButton("✏️");
        QPushButton *deleteBtn = new QPushButton("🗑");

        connect(editBtn, &QPushButton::clicked, [=]() {
            TaskDialog *dialog = new TaskDialog(manager.tasks[i]);

            connect(dialog, &TaskDialog::taskSaved, this, [=](Task updatedTask) {
                manager.updateTask(i, updatedTask);
                refreshUI();
            });

            dialog->show();
        });

        connect(deleteBtn, &QPushButton::clicked, [=]() {
            manager.removeTask(i);
            refreshUI();
        });

        cardLayout->addWidget(taskLabel);
        cardLayout->addStretch();
        cardLayout->addWidget(editBtn);
        cardLayout->addWidget(deleteBtn);

        int row = grid->rowCount();
        grid->addWidget(card, row, 0);
    }

    loadHistory();
}