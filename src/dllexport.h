/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/src/dllexport.h,v $
 *
 *  $Revision: 1.1.1.1 $
 *
 *	$Log: not supported by cvs2svn $
 *
 ***************************************************************************/
/***************************************************************************
 *																		   *
 * This file is part of the MinDia package (program to make slide shows),  *
 *																		   *
 * Copyright (C) 2002 by Michael Neuroth.								   *
 *                                                                         *
 * This program is free software; you can redistribute it and/or modify    *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation; either version 2 of the License, or       *
 * (at your option) any later version.                                     *
 *                                                                         *
 ***************************************************************************/

// Compiler abhaengige Defines zur Verwendung von DLLs

//#ifndef _DLLEXPORT_H
//#define _DLLEXPORT_H

//*************************************************************************

#ifdef _MSC_VER
#ifdef _USE_DLL
#define UMDLLEXPORT			__declspec( dllimport )
#define UMPREDLLEXPORT		__declspec( dllimport )
#else
#ifdef _NO_DLL_EXPORT
#define UMDLLEXPORT	
#define UMPREDLLEXPORT 
#else
#define UMDLLEXPORT			__declspec( dllexport )
#define UMPREDLLEXPORT		__declspec( dllexport )
#endif
#endif
#endif

#ifdef __IBMCPP__
#ifdef _USE_DLL
#define UMDLLEXPORT 
#define UMPREDLLEXPORT 
#else
#ifdef _NO_DLL_EXPORT
#define UMDLLEXPORT	
#define UMPREDLLEXPORT 
#else
#define UMDLLEXPORT			_Export
#define UMPREDLLEXPORT 
#endif
#endif
#endif

#ifdef __EMX__
#ifdef _USE_DLL
#define UMDLLEXPORT 
#define UMPREDLLEXPORT 
#else
#define UMDLLEXPORT
#define UMPREDLLEXPORT 
#endif
#endif

#ifdef __linux__
// nicht implementiert ?
#define UMDLLEXPORT 
#define UMPREDLLEXPORT 
#endif

//#endif

