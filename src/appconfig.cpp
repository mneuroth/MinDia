/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 ***************************************************************************/
/***************************************************************************
 *																		   *
 * This file is part of the MinDia package (program to make slide shows),  *
 *																		   *
 * Copyright (C) 2013 by Michael Neuroth.							       *
 *                                                                         *
 * This program is free software; you can redistribute it and/or modify    *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation; either version 2 of the License, or       *
 * (at your option) any later version.                                     *
 *                                                                         *
 ***************************************************************************/

#include "appconfig.h"
#include "minbase.h"
#include "iscript.h"
#include "misctools.h"

#include <QDir>

string GetImagePath()
{
	string sPath;
	minClientHandle<IGeneralScriptFcn> hScriptFcn( g_IGeneralScriptFcnID );

	if( hScriptFcn.IsValid() && hScriptFcn->IsOk() )
	{
        sPath = hScriptFcn->GetImageDirecotry() + string(ToStdString(QDir::separator()));
	}

	return sPath;
}

string GetMusicPath()
{
	string sPath;
	minClientHandle<IGeneralScriptFcn> hScriptFcn( g_IGeneralScriptFcnID );

	if( hScriptFcn.IsValid() && hScriptFcn->IsOk() )
	{
        sPath = hScriptFcn->GetMusicDirecotry()+ string(ToStdString(QDir::separator()));
	}

	return sPath;
}

string GetDataPath()
{
	string sPath;
	minClientHandle<IGeneralScriptFcn> hScriptFcn( g_IGeneralScriptFcnID );

	if( hScriptFcn.IsValid() && hScriptFcn->IsOk() )
	{
        sPath = hScriptFcn->GetDataDirecotry()+ string(ToStdString(QDir::separator()));
	}

	return sPath;
}

