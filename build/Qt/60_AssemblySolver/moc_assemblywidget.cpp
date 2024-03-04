/****************************************************************************
** Meta object code from reading C++ file 'assemblywidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../Qt/60_AssemblySolver/assemblywidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'assemblywidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_AssmSolverSceneWidget_t {
    QByteArrayData data[6];
    char stringdata0[65];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AssmSolverSceneWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AssmSolverSceneWidget_t qt_meta_stringdata_AssmSolverSceneWidget = {
    {
QT_MOC_LITERAL(0, 0, 21), // "AssmSolverSceneWidget"
QT_MOC_LITERAL(1, 22, 15), // "openSelectModel"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 12), // "viewCommands"
QT_MOC_LITERAL(4, 52, 8), // "Commands"
QT_MOC_LITERAL(5, 61, 3) // "cmd"

    },
    "AssmSolverSceneWidget\0openSelectModel\0"
    "\0viewCommands\0Commands\0cmd"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AssmSolverSceneWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x0a /* Public */,
       3,    1,   30,    2, 0x0a /* Public */,
       3,    0,   33,    2, 0x2a /* Public | MethodCloned */,

 // slots: parameters
    QMetaType::QString,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void,

       0        // eod
};

void AssmSolverSceneWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<AssmSolverSceneWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: { QString _r = _t->openSelectModel();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 1: _t->viewCommands((*reinterpret_cast< Commands(*)>(_a[1]))); break;
        case 2: _t->viewCommands(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject AssmSolverSceneWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QtVision::QtOpenGLSceneWidget::staticMetaObject>(),
    qt_meta_stringdata_AssmSolverSceneWidget.data,
    qt_meta_data_AssmSolverSceneWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *AssmSolverSceneWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AssmSolverSceneWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_AssmSolverSceneWidget.stringdata0))
        return static_cast<void*>(this);
    return QtVision::QtOpenGLSceneWidget::qt_metacast(_clname);
}

int AssmSolverSceneWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QtVision::QtOpenGLSceneWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
