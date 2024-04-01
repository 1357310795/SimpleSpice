/****************************************************************************
** Meta object code from reading C++ file 'QtAdvancedStylesheet.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../src/qss/QtAdvancedStylesheet.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtAdvancedStylesheet.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_acss__QtAdvancedStylesheet_t {
    QByteArrayData data[14];
    char stringdata0[233];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_acss__QtAdvancedStylesheet_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_acss__QtAdvancedStylesheet_t qt_meta_stringdata_acss__QtAdvancedStylesheet = {
    {
QT_MOC_LITERAL(0, 0, 26), // "acss::QtAdvancedStylesheet"
QT_MOC_LITERAL(1, 27, 19), // "currentStyleChanged"
QT_MOC_LITERAL(2, 47, 0), // ""
QT_MOC_LITERAL(3, 48, 5), // "Style"
QT_MOC_LITERAL(4, 54, 19), // "currentThemeChanged"
QT_MOC_LITERAL(5, 74, 5), // "Theme"
QT_MOC_LITERAL(6, 80, 17), // "stylesheetChanged"
QT_MOC_LITERAL(7, 98, 15), // "setCurrentTheme"
QT_MOC_LITERAL(8, 114, 15), // "setDefaultTheme"
QT_MOC_LITERAL(9, 130, 15), // "setCurrentStyle"
QT_MOC_LITERAL(10, 146, 16), // "updateStylesheet"
QT_MOC_LITERAL(11, 163, 20), // "processStyleTemplate"
QT_MOC_LITERAL(12, 184, 17), // "generateResources"
QT_MOC_LITERAL(13, 202, 30) // "updateApplicationPaletteColors"

    },
    "acss::QtAdvancedStylesheet\0"
    "currentStyleChanged\0\0Style\0"
    "currentThemeChanged\0Theme\0stylesheetChanged\0"
    "setCurrentTheme\0setDefaultTheme\0"
    "setCurrentStyle\0updateStylesheet\0"
    "processStyleTemplate\0generateResources\0"
    "updateApplicationPaletteColors"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_acss__QtAdvancedStylesheet[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   64,    2, 0x06 /* Public */,
       4,    1,   67,    2, 0x06 /* Public */,
       6,    0,   70,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    1,   71,    2, 0x0a /* Public */,
       8,    0,   74,    2, 0x0a /* Public */,
       9,    1,   75,    2, 0x0a /* Public */,
      10,    0,   78,    2, 0x0a /* Public */,
      11,    0,   79,    2, 0x0a /* Public */,
      12,    0,   80,    2, 0x0a /* Public */,
      13,    0,   81,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Bool, QMetaType::QString,    5,
    QMetaType::Void,
    QMetaType::Bool, QMetaType::QString,    3,
    QMetaType::Bool,
    QMetaType::Bool,
    QMetaType::Bool,
    QMetaType::Void,

       0        // eod
};

void acss::QtAdvancedStylesheet::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<QtAdvancedStylesheet *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->currentStyleChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->currentThemeChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->stylesheetChanged(); break;
        case 3: { bool _r = _t->setCurrentTheme((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 4: _t->setDefaultTheme(); break;
        case 5: { bool _r = _t->setCurrentStyle((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 6: { bool _r = _t->updateStylesheet();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 7: { bool _r = _t->processStyleTemplate();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 8: { bool _r = _t->generateResources();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 9: _t->updateApplicationPaletteColors(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (QtAdvancedStylesheet::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&QtAdvancedStylesheet::currentStyleChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (QtAdvancedStylesheet::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&QtAdvancedStylesheet::currentThemeChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (QtAdvancedStylesheet::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&QtAdvancedStylesheet::stylesheetChanged)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject acss::QtAdvancedStylesheet::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_acss__QtAdvancedStylesheet.data,
    qt_meta_data_acss__QtAdvancedStylesheet,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *acss::QtAdvancedStylesheet::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *acss::QtAdvancedStylesheet::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_acss__QtAdvancedStylesheet.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int acss::QtAdvancedStylesheet::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void acss::QtAdvancedStylesheet::currentStyleChanged(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void acss::QtAdvancedStylesheet::currentThemeChanged(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void acss::QtAdvancedStylesheet::stylesheetChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
