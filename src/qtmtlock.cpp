
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
		pAppl->lock();
	}
}

QtMTLock::~QtMTLock()
{
	QApplication * pAppl = GetApplication();

	if( pAppl )
	{
		pAppl->unlock();
	}
}

void QtMTLock::Set()
{
}
