/****************************************************************************
** Meta object code from reading C++ file 'realtimesyncservice.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../realtimesyncservice.h"
#include <QtNetwork/QSslError>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'realtimesyncservice.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN19RealtimeSyncServiceE_t {};
} // unnamed namespace

template <> constexpr inline auto RealtimeSyncService::qt_create_metaobjectdata<qt_meta_tag_ZN19RealtimeSyncServiceE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "RealtimeSyncService",
        "connectionStateChanged",
        "",
        "state",
        "remoteStrokeBegin",
        "Stroke",
        "stroke",
        "remoteStrokePoints",
        "strokeId",
        "QList<QPointF>",
        "points",
        "remoteStrokeEnd",
        "pollCanvas",
        "pushLocalDelta"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'connectionStateChanged'
        QtMocHelpers::SignalData<void(QString)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 },
        }}),
        // Signal 'remoteStrokeBegin'
        QtMocHelpers::SignalData<void(const Stroke &)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 5, 6 },
        }}),
        // Signal 'remoteStrokePoints'
        QtMocHelpers::SignalData<void(const QString &, const QVector<QPointF> &)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 8 }, { 0x80000000 | 9, 10 },
        }}),
        // Signal 'remoteStrokeEnd'
        QtMocHelpers::SignalData<void(const QString &)>(11, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 8 },
        }}),
        // Slot 'pollCanvas'
        QtMocHelpers::SlotData<void()>(12, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'pushLocalDelta'
        QtMocHelpers::SlotData<void()>(13, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<RealtimeSyncService, qt_meta_tag_ZN19RealtimeSyncServiceE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject RealtimeSyncService::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN19RealtimeSyncServiceE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN19RealtimeSyncServiceE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN19RealtimeSyncServiceE_t>.metaTypes,
    nullptr
} };

void RealtimeSyncService::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<RealtimeSyncService *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->connectionStateChanged((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->remoteStrokeBegin((*reinterpret_cast<std::add_pointer_t<Stroke>>(_a[1]))); break;
        case 2: _t->remoteStrokePoints((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QList<QPointF>>>(_a[2]))); break;
        case 3: _t->remoteStrokeEnd((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 4: _t->pollCanvas(); break;
        case 5: _t->pushLocalDelta(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (RealtimeSyncService::*)(QString )>(_a, &RealtimeSyncService::connectionStateChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (RealtimeSyncService::*)(const Stroke & )>(_a, &RealtimeSyncService::remoteStrokeBegin, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (RealtimeSyncService::*)(const QString & , const QVector<QPointF> & )>(_a, &RealtimeSyncService::remoteStrokePoints, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (RealtimeSyncService::*)(const QString & )>(_a, &RealtimeSyncService::remoteStrokeEnd, 3))
            return;
    }
}

const QMetaObject *RealtimeSyncService::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RealtimeSyncService::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN19RealtimeSyncServiceE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int RealtimeSyncService::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void RealtimeSyncService::connectionStateChanged(QString _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void RealtimeSyncService::remoteStrokeBegin(const Stroke & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void RealtimeSyncService::remoteStrokePoints(const QString & _t1, const QVector<QPointF> & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1, _t2);
}

// SIGNAL 3
void RealtimeSyncService::remoteStrokeEnd(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}
QT_WARNING_POP
