/****************************************************************************
** Meta object code from reading C++ file 'monitorclient.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "monitorclient.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'monitorclient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_monitorclient[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x08,
      25,   14,   14,   14, 0x08,
      40,   14,   14,   14, 0x08,
      83,   14,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_monitorclient[] = {
    "monitorclient\0\0landing()\0new_register()\0"
    "displayError(QAbstractSocket::SocketError)\0"
    "readMessage()\0"
};

void monitorclient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        monitorclient *_t = static_cast<monitorclient *>(_o);
        switch (_id) {
        case 0: _t->landing(); break;
        case 1: _t->new_register(); break;
        case 2: _t->displayError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 3: _t->readMessage(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData monitorclient::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject monitorclient::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_monitorclient,
      qt_meta_data_monitorclient, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &monitorclient::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *monitorclient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *monitorclient::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_monitorclient))
        return static_cast<void*>(const_cast< monitorclient*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int monitorclient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
