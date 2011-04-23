
#include "qtmtlock.h"

#include <qapplication.h>

// *******************************************************************

// ** this function is defined in main.cpp */
QApplication * GetApplication();

// *******************************************************************
// *******************************************************************
// *******************************************************************

QtMTLock::QtMTLock()
{
	QApplication * pAppl = GetApplication();

	if( pAppl )
	{
// TODO porting		pAppl->lock();
	}
}

QtMTLock::~QtMTLock()
{
	QApplication * pAppl = GetApplication();

	if( pAppl )
	{
// TODO porting		pAppl->unlock();
	}
}

void QtMTLock::Set()
{
}
