/**************************************************************************
 *
 *	project				 : VisiScript
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/zaurus/zmain.cpp,v $
 *
 *  $Revision: 1.1.1.1 $
 *
 *	$Log: not supported by cvs2svn $
 *
 ***************************************************************************/
/*********************************************************************************
 *																				 *
 * This file is part of the VisiScript package (a frontend for script languages) *
 *																				 *
 * Copyright (C) 2002 by Michael Neuroth.										 *
 *                                                                               *
 * This program is free software; you can redistribute it and/or modify			 *
 * it under the terms of the GNU General Public License as published by    		 *
 * the Free Software Foundation; either version 2 of the License, or             *
 * (at your option) any later version.                                           *
 *                                                                               *
 ********************************************************************************/

#include "zmindia.h"

#include <qpe/qpeapplication.h>

int main( int argc, char ** argv )
{
    QPEApplication aAppl( argc, argv );

    ZMinDia aMainWin;
    aAppl.showMainDocumentWidget( &aMainWin );
//    if ( argc == 3 && argv[1] == QCString("-f") )
//	aMainWin.openFile(argv[2]);

    return aAppl.exec();
}
