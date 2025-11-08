/****************************************************************************
** Meta object code from reading C++ file 'qsherpaonnxthreader.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.11)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "qsherpaonnxthreader.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qsherpaonnxthreader.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.11. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QSherpaOnnxThreader_t {
    QByteArrayData data[8];
    char stringdata0[58];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QSherpaOnnxThreader_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QSherpaOnnxThreader_t qt_meta_stringdata_QSherpaOnnxThreader = {
    {
QT_MOC_LITERAL(0, 0, 19), // "QSherpaOnnxThreader"
QT_MOC_LITERAL(1, 20, 5), // "start"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 8), // "Priority"
QT_MOC_LITERAL(4, 36, 3), // "pri"
QT_MOC_LITERAL(5, 40, 4), // "stop"
QT_MOC_LITERAL(6, 45, 5), // "pause"
QT_MOC_LITERAL(7, 51, 6) // "resume"

    },
    "QSherpaOnnxThreader\0start\0\0Priority\0"
    "pri\0stop\0pause\0resume"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QSherpaOnnxThreader[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x0a /* Public */,
       1,    0,   42,    2, 0x2a /* Public | MethodCloned */,
       5,    0,   43,    2, 0x0a /* Public */,
       6,    0,   44,    2, 0x0a /* Public */,
       7,    0,   45,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void QSherpaOnnxThreader::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<QSherpaOnnxThreader *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->start((*reinterpret_cast< Priority(*)>(_a[1]))); break;
        case 1: _t->start(); break;
        case 2: _t->stop(); break;
        case 3: _t->pause(); break;
        case 4: _t->resume(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject QSherpaOnnxThreader::staticMetaObject = { {
    QMetaObject::SuperData::link<QThread::staticMetaObject>(),
    qt_meta_stringdata_QSherpaOnnxThreader.data,
    qt_meta_data_QSherpaOnnxThreader,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QSherpaOnnxThreader::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QSherpaOnnxThreader::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QSherpaOnnxThreader.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int QSherpaOnnxThreader::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
