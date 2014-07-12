/****************************************************************************
** Meta object code from reading C++ file 'Manager.h'
**
** Created: Sat Jul 12 17:36:20 2014
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Manager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Manager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ImgGetter[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ImgGetter[] = {
    "ImgGetter\0\0replyFinished(QNetworkReply*)\0"
};

const QMetaObject ImgGetter::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ImgGetter,
      qt_meta_data_ImgGetter, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ImgGetter::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ImgGetter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ImgGetter::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ImgGetter))
        return static_cast<void*>(const_cast< ImgGetter*>(this));
    return QObject::qt_metacast(_clname);
}

int ImgGetter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: replyFinished((*reinterpret_cast< QNetworkReply*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
