/****************************************************************************
** Meta object code from reading C++ file 'clima.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/clima.h"
#include <QtNetwork/QSslError>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'clima.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.10.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN5ClimaE_t {};
} // unnamed namespace

template <> constexpr inline auto Clima::qt_create_metaobjectdata<qt_meta_tag_ZN5ClimaE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "Clima",
        "climaListo",
        "",
        "WeatherInfo",
        "info",
        "mensaje",
        "texto",
        "esError",
        "mostrar",
        "ocultar",
        "actualizarClima",
        "onConnectivityFinished",
        "QNetworkReply*",
        "reply",
        "onWeatherFinished"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'climaListo'
        QtMocHelpers::SignalData<void(const WeatherInfo &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Signal 'mensaje'
        QtMocHelpers::SignalData<void(const QString &, bool)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 6 }, { QMetaType::Bool, 7 },
        }}),
        // Slot 'mostrar'
        QtMocHelpers::SlotData<void()>(8, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'ocultar'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'actualizarClima'
        QtMocHelpers::SlotData<void()>(10, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onConnectivityFinished'
        QtMocHelpers::SlotData<void(QNetworkReply *)>(11, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 12, 13 },
        }}),
        // Slot 'onWeatherFinished'
        QtMocHelpers::SlotData<void(QNetworkReply *)>(14, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 12, 13 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Clima, qt_meta_tag_ZN5ClimaE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject Clima::staticMetaObject = { {
    QMetaObject::SuperData::link<Pantalla::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN5ClimaE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN5ClimaE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN5ClimaE_t>.metaTypes,
    nullptr
} };

void Clima::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Clima *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->climaListo((*reinterpret_cast<std::add_pointer_t<WeatherInfo>>(_a[1]))); break;
        case 1: _t->mensaje((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[2]))); break;
        case 2: _t->mostrar(); break;
        case 3: _t->ocultar(); break;
        case 4: _t->actualizarClima(); break;
        case 5: _t->onConnectivityFinished((*reinterpret_cast<std::add_pointer_t<QNetworkReply*>>(_a[1]))); break;
        case 6: _t->onWeatherFinished((*reinterpret_cast<std::add_pointer_t<QNetworkReply*>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (Clima::*)(const WeatherInfo & )>(_a, &Clima::climaListo, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (Clima::*)(const QString & , bool )>(_a, &Clima::mensaje, 1))
            return;
    }
}

const QMetaObject *Clima::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Clima::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN5ClimaE_t>.strings))
        return static_cast<void*>(this);
    return Pantalla::qt_metacast(_clname);
}

int Clima::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Pantalla::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void Clima::climaListo(const WeatherInfo & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void Clima::mensaje(const QString & _t1, bool _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1, _t2);
}
QT_WARNING_POP
