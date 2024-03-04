/****************************************************************************
** Meta object code from reading C++ file 'qt_openglwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../QtVision/Include/qt_openglwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qt_openglwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QtVision__QtOpenGLWidget_t {
    QByteArrayData data[3];
    char stringdata0[45];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QtVision__QtOpenGLWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QtVision__QtOpenGLWidget_t qt_meta_stringdata_QtVision__QtOpenGLWidget = {
    {
QT_MOC_LITERAL(0, 0, 24), // "QtVision::QtOpenGLWidget"
QT_MOC_LITERAL(1, 25, 18), // "showCompanyWebSite"
QT_MOC_LITERAL(2, 44, 0) // ""

    },
    "QtVision::QtOpenGLWidget\0showCompanyWebSite\0"
    ""
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QtVision__QtOpenGLWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void QtVision::QtOpenGLWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<QtOpenGLWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->showCompanyWebSite(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject QtVision::QtOpenGLWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QOpenGLWidget::staticMetaObject>(),
    qt_meta_stringdata_QtVision__QtOpenGLWidget.data,
    qt_meta_data_QtVision__QtOpenGLWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QtVision::QtOpenGLWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QtVision::QtOpenGLWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QtVision__QtOpenGLWidget.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "Object"))
        return static_cast< Object*>(this);
    return QOpenGLWidget::qt_metacast(_clname);
}

int QtVision::QtOpenGLWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QOpenGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
struct qt_meta_stringdata_QtVision__QtOpenGLSceneWidget_t {
    QByteArrayData data[3];
    char stringdata0[48];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QtVision__QtOpenGLSceneWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QtVision__QtOpenGLSceneWidget_t qt_meta_stringdata_QtVision__QtOpenGLSceneWidget = {
    {
QT_MOC_LITERAL(0, 0, 29), // "QtVision::QtOpenGLSceneWidget"
QT_MOC_LITERAL(1, 30, 16), // "viewPortCommands"
QT_MOC_LITERAL(2, 47, 0) // ""

    },
    "QtVision::QtOpenGLSceneWidget\0"
    "viewPortCommands\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QtVision__QtOpenGLSceneWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void QtVision::QtOpenGLSceneWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<QtOpenGLSceneWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->viewPortCommands(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject QtVision::QtOpenGLSceneWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QtOpenGLWidget::staticMetaObject>(),
    qt_meta_stringdata_QtVision__QtOpenGLSceneWidget.data,
    qt_meta_data_QtVision__QtOpenGLSceneWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QtVision::QtOpenGLSceneWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QtVision::QtOpenGLSceneWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QtVision__QtOpenGLSceneWidget.stringdata0))
        return static_cast<void*>(this);
    return QtOpenGLWidget::qt_metacast(_clname);
}

int QtVision::QtOpenGLSceneWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QtOpenGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
