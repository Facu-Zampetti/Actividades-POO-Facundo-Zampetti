#include "taskdialog.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>

TaskDialog::TaskDialog(Task task, QWidget *parent)
    : QWidget(parent), currentTask(task)
{
    setWindowTitle("Editar Trabajo");
    resize(400, 400);

    QVBoxLayout *layout = new QVBoxLayout(this);

    titleEdit = new QLineEdit(task.title);

    statusBox = new QComboBox();
    statusBox->addItems({"Pendiente", "En progreso", "Finalizado"});
    statusBox->setCurrentText(task.status);

    priorityBox = new QComboBox();
    priorityBox->addItems({"Baja", "Media", "Alta"});
    priorityBox->setCurrentText(task.priority);

    notesEdit = new QTextEdit(task.notes);

    saveButton = new QPushButton("Guardar");

    layout->addWidget(new QLabel("Título"));
    layout->addWidget(titleEdit);

    layout->addWidget(new QLabel("Estado"));
    layout->addWidget(statusBox);

    layout->addWidget(new QLabel("Prioridad"));
    layout->addWidget(priorityBox);

    layout->addWidget(new QLabel("Notas"));
    layout->addWidget(notesEdit);

    layout->addWidget(saveButton);

    connect(saveButton, &QPushButton::clicked, this, &TaskDialog::saveTask);
}

void TaskDialog::saveTask()
{
    QString title = titleEdit->text();

    // 🔴 validar título vacío
    if (title.trimmed().isEmpty()) {
        QMessageBox::warning(this, "Error", "El título no puede estar vacío");
        return;
    }

    currentTask.title = title;
    currentTask.status = statusBox->currentText();
    currentTask.priority = priorityBox->currentText();
    currentTask.notes = notesEdit->toPlainText();

    emit taskSaved(currentTask);
    close();
}