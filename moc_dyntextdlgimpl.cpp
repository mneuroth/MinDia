/****************************************************************************
** DynamicTextDlgImpl meta object code from reading C++ file 'dyntextdlgimpl.h'
**
** Created: Sat Jun 21 23:53:36 2003
**      by: The Qt MOC ($Id: moc_dyntextdlgimpl.cpp,v 1.1.1.1 2003-08-15 16:38:21 min Exp $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#define Q_MOC_DynamicTextDlgImpl
#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 8
#elif Q_MOC_OUTPUT_REVISION != 8
#error "Moc format conflict - please regenerate all moc files"
#endif

#include "dyntextdlgimpl.h"
#include <qmetaobject.h>
#include <qapplication.h>

#if defined(Q_SPARCWORKS_FUNCP_BUG)
#define Q_AMPERSAND
#else
#define Q_AMPERSAND &
#endif


const char *DynamicTextDlgImpl::className() const
{
    return "DynamicTextDlgImpl";
}

QMetaObject *DynamicTextDlgImpl::metaObj = 0;

void DynamicTextDlgImpl::initMetaObject()
{
    if ( metaObj )
	return;
    if ( strcmp(DynamicTextDlg::className(), "DynamicTextDlg") != 0 )
	badSuperclassWarning("DynamicTextDlgImpl","DynamicTextDlg");
    (void) staticMetaObject();
}

#ifndef QT_NO_TRANSLATION
QString DynamicTextDlgImpl::tr(const char* s)
{
    return ((QNonBaseApplication*)qApp)->translate("DynamicTextDlgImpl",s);
}

#endif // QT_NO_TRANSLATION
QMetaObject* DynamicTextDlgImpl::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    (void) DynamicTextDlg::staticMetaObject();
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    typedef void(DynamicTextDlgImpl::*m1_t0)();
    typedef void(DynamicTextDlgImpl::*m1_t1)();
    typedef void(DynamicTextDlgImpl::*m1_t2)();
    m1_t0 v1_0 = Q_AMPERSAND DynamicTextDlgImpl::sltDeleteText;
    m1_t1 v1_1 = Q_AMPERSAND DynamicTextDlgImpl::sltSelectFont;
    m1_t2 v1_2 = Q_AMPERSAND DynamicTextDlgImpl::sltSelectFontcolor;
    QMetaData *slot_tbl = QMetaObject::new_metadata(3);
    QMetaData::Access *slot_tbl_access = QMetaObject::new_metaaccess(3);
    slot_tbl[0].name = "sltDeleteText()";
    slot_tbl[0].ptr = *((QMember*)&v1_0);
    slot_tbl_access[0] = QMetaData::Public;
    slot_tbl[1].name = "sltSelectFont()";
    slot_tbl[1].ptr = *((QMember*)&v1_1);
    slot_tbl_access[1] = QMetaData::Public;
    slot_tbl[2].name = "sltSelectFontcolor()";
    slot_tbl[2].ptr = *((QMember*)&v1_2);
    slot_tbl_access[2] = QMetaData::Public;
    metaObj = QMetaObject::new_metaobject(
	"DynamicTextDlgImpl", "DynamicTextDlg",
	slot_tbl, 3,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    metaObj->set_slot_access( slot_tbl_access );
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    return metaObj;
}
