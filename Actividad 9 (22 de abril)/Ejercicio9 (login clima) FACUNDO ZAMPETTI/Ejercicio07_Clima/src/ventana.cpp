#include "ventana.h"

#include "logger.h"

#include <QApplication>
#include <QBitmap>
#include <QEvent>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QPainterPath>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

Ventana::Ventana(QObject *parent)
    : Pantalla(parent)
    , m_widget(new QWidget)
    , m_backgroundLabel(new QLabel(m_widget))
    , m_card(new QFrame(m_widget))
{
    m_widget->setWindowTitle("Ventana principal");
    m_widget->setMinimumSize(960, 540);
    m_widget->installEventFilter(this);

    m_backgroundLabel->setScaledContents(false);
    m_backgroundLabel->setAlignment(Qt::AlignCenter);

    m_card->setStyleSheet(
        "QFrame {"
        "background-color: rgba(6, 22, 16, 0.90);"
        "border: 1px solid rgba(84, 240, 160, 0.35);"
        "border-radius: 16px;"
        "}");

    // Botón cerrar (esquina superior derecha, fuera de la card)
    auto *btnCerrar = new QPushButton("✕", m_widget);
    btnCerrar->setFixedSize(42, 42);
    btnCerrar->setCursor(Qt::PointingHandCursor);
    btnCerrar->setStyleSheet(
        "QPushButton {"
        "  background-color: rgba(255,255,255,0.85);"
        "  color: #0f172a;"
        "  border: 1px solid rgba(11,58,83,0.25);"
        "  border-radius: 21px;"
        "  font-size: 18px;"
        "  font-weight: bold;"
        "}"
        "QPushButton:hover { background-color: #e11d48; color: white; }"
    );
    connect(btnCerrar, &QPushButton::clicked, qApp, &QApplication::quit);

    // Layout absoluto para posicionar el botón en esquina
    btnCerrar->raise();

    auto *mainLayout = new QHBoxLayout(m_widget);
    mainLayout->setContentsMargins(48, 48, 48, 48);
    mainLayout->addWidget(m_card, 0, Qt::AlignLeft | Qt::AlignTop);

    // Posicionar el botón cuando el widget cambia de tamaño: lo hacemos en eventFilter
    m_btnCerrar = btnCerrar;

    auto *cardLayout = new QVBoxLayout(m_card);
    cardLayout->setContentsMargins(28, 24, 28, 24);
    cardLayout->setSpacing(12);

    auto *photo = new QLabel(m_card);
    photo->setFixedSize(140, 140);
    photo->setPixmap(crearFotoPerfil());
    photo->setAlignment(Qt::AlignCenter);

    auto *name = new QLabel("Facundo Zampetti", m_card);
    name->setStyleSheet("font-size: 30px; font-weight: 800; color: #f0fff6;");

    auto *headline = new QLabel("Desarrollador Qt/C++ | Sistemas Embebidos | Automatizacion", m_card);
    headline->setStyleSheet("font-size: 15px; font-weight: 700; color: #6ef7b2;");
    headline->setWordWrap(true);

    auto *summary = new QLabel(
        "Perfil orientado a desarrollo de software con C++/Qt, consumo de APIs y diseno de interfaces robustas. "
        "Enfoque en buenas practicas, validacion de datos y experiencia de usuario clara.",
        m_card);
    summary->setWordWrap(true);
    summary->setStyleSheet("font-size: 14px; color: #d8ffe9; line-height: 1.35em;");

    auto *items = new QLabel(
        "Experiencia\n"
        "- Aplicaciones de escritorio con Qt Widgets\n"
        "- Integracion de servicios REST y JSON\n"
        "- Arquitectura basada en clases y signals/slots\n\n"
        "Contacto\n"
        "- zfacundo18@gmail.com\n"
        "- linkedin.com/in/facundo-zampetti",
        m_card);
    items->setWordWrap(true);
    items->setStyleSheet("font-size: 14px; color: #c7f9de;");

    cardLayout->addWidget(photo, 0, Qt::AlignLeft);
    cardLayout->addWidget(name);
    cardLayout->addWidget(headline);
    cardLayout->addWidget(summary);
    cardLayout->addWidget(items);

    Logger::instancia().registrar("Ventana: componente principal inicializado");
}

Ventana::~Ventana()
{
    delete m_widget;
}

QWidget *Ventana::widget()
{
    return m_widget;
}

void Ventana::mostrar()
{
    m_widget->showFullScreen();
}

void Ventana::ocultar()
{
    m_widget->hide();
}

void Ventana::setImagenFondo(const QString &path)
{
    QPixmap loaded(path);
    if (loaded.isNull()) {
        Logger::instancia().registrar("Ventana: imagen de fondo invalida en " + path);
        return;
    }

    m_background = loaded;
    actualizarFondo();
    Logger::instancia().registrar("Ventana: fondo cargado y escalado con relacion de aspecto");
}

bool Ventana::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == m_widget && event->type() == QEvent::Resize) {
        m_backgroundLabel->setGeometry(m_widget->rect());
        actualizarFondo();
        // Reposicionar botón cerrar en esquina superior derecha
        if (m_btnCerrar) {
            const QSize ws = m_widget->size();
            m_btnCerrar->move(ws.width() - 58, 16);
            m_btnCerrar->raise();
        }
    }

    return Pantalla::eventFilter(obj, event);
}

void Ventana::actualizarFondo()
{
    if (m_background.isNull()) {
        return;
    }

    const QSize size = m_widget->size();
    const QPixmap scaled = m_background.scaled(size, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    m_backgroundLabel->setPixmap(scaled);
    m_backgroundLabel->lower();
}

QPixmap Ventana::crearFotoPerfil() const
{
    // Busca foto.jpg o foto.png junto al ejecutable
    const QString base = QApplication::applicationDirPath();
    QPixmap source;
    for (const QString &ext : {QString("jpg"), QString("jpeg"), QString("png")}) {
        QPixmap tmp(base + "/foto." + ext);
        if (!tmp.isNull()) { source = tmp; break; }
    }

    QPixmap pix(140, 140);
    pix.fill(Qt::transparent);
    QPainter painter(&pix);
    painter.setRenderHint(QPainter::Antialiasing, true);

    if (!source.isNull()) {
        // Recortar en circulo
        QPixmap scaled = source.scaled(140, 140, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
        // centrar
        int ox = (scaled.width()  - 140) / 2;
        int oy = (scaled.height() - 140) / 2;
        QPainterPath path;
        path.addEllipse(0, 0, 140, 140);
        painter.setClipPath(path);
        painter.drawPixmap(-ox, -oy, scaled);
    } else {
        // Fallback: circulo degradado con iniciales
        QLinearGradient gradient(0, 0, 140, 140);
        gradient.setColorAt(0.0, QColor("#0b3a53"));
        gradient.setColorAt(1.0, QColor("#2b6c8e"));
        painter.setBrush(gradient);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(0, 0, 140, 140);
        painter.setPen(Qt::white);
        painter.setFont(QFont("Segoe UI", 42, QFont::Bold));
        painter.drawText(pix.rect(), Qt::AlignCenter, "FZ");
    }

    return pix;
}
