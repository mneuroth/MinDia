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
 * Copyright (C) 2013 by Michael Neuroth.								   *
 *                                                                         *
 * This program is free software; you can redistribute it and/or modify    *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation; either version 2 of the License, or       *
 * (at your option) any later version.                                     *
 *                                                                         *
 ***************************************************************************/

#ifndef _ISCRIPT_H
#define _ISCRIPT_H

#include <string>

using namespace std;

// *************************************************************************
// *************************************************************************
// *************************************************************************

// *************************************************************************
// ** Interfaces for the Script-Interpreter (Python) **
// *************************************************************************

/** represents a rgb-color value */
struct IColor
{
	IColor()
	{
		iRed = 0;
		iGreen = 0;
		iBlue = 0;
	}

	int iRed;
	int iGreen;
	int iBlue;
};

struct IHsv
{
	IHsv()
	{
		iHue = 0;
		iSaturation = 0;
		iValue = 0;
	}

	int iHue;
	int iSaturation;
	int iValue;
};

/** represents the output window
  */
class IDiaOutputWindow
{
public:
	virtual ~IDiaOutputWindow() {}

	virtual bool			IsValid() const = 0;

	virtual bool			Show() = 0;
	virtual bool			Hide() = 0;
	virtual bool			Maximize() = 0;
	virtual bool			Restore() = 0;

	virtual int				GetWidth() const = 0;
	virtual int				GetHeight() const = 0;
	virtual bool			SetSize( int iWidth, int iHeight ) = 0;
	virtual bool			SetPos( int iXPos, int iYPos ) = 0;

	virtual int				GetDrawWidth() const = 0;
	virtual int				GetDrawHeight() const = 0;

	virtual bool			Clear() = 0;

	virtual bool			SetColor( int iRed, int iGreen, int iBlue ) = 0;
    virtual bool			SetFont( const string & sFontName ) = 0;
	virtual bool			SetFontSize( int iSizeInPixel, bool bBold = false, bool bItalic = false ) = 0;
    virtual int 			SetTextXY( int x, int y, const string & sText ) = 0;
	virtual int				GetTextCount() const = 0;
	virtual bool			MoveText( int iTextID, int x, int y ) = 0;
	virtual int				GetTextX( int iTextID ) const = 0;
	virtual int				GetTextY( int iTextID ) const = 0;
	virtual int				GetTextWidth( int iTextID ) const = 0;
	virtual int				GetTextHeight( int iTextID ) const = 0;
	virtual bool			SetTextColor( int iTextID, int iRed, int iGreen, int iBlue ) = 0;
	virtual IColor			GetTextColor( int iTextID ) const = 0;
	virtual bool			DeleteText( int iTextID ) = 0;
};

/** represents data for a single dia.
  */
class IDia
{
public:
	virtual ~IDia() {}

	virtual double			GetTotalTime() const = 0;

	virtual double			GetShowTime() const = 0;
	virtual bool 			SetShowTime( double dVal ) = 0;
	virtual double			GetDissolveTime() const = 0;
	virtual bool			SetDissolveTime( double dVal ) = 0;

    virtual string          GetUUID() const = 0;

    virtual string      	GetId() const = 0;
    virtual bool			SetId( const string & sId ) = 0;
    virtual string      	GetComment() const = 0;
    virtual bool			SetComment( const string & sComment ) = 0;
    virtual string      	GetImageFile() const = 0;
    virtual bool			SetImageFile( const string & sFileName ) = 0;
    virtual string      	GetScript() const = 0;
    virtual bool			SetScript( const string & sScript ) = 0;

	virtual bool			GetHorizontalFormat() const = 0;
	virtual bool			SetHorizontalFormat( bool bValue ) = 0;

    virtual double          GetRelX() const = 0;
    virtual bool            SetRelX( double dVal ) = 0;
    virtual double          GetRelY() const = 0;
    virtual bool            SetRelY( double dVal ) = 0;
    virtual double          GetRelDX() const = 0;
    virtual bool            SetRelDX( double dVal ) = 0;
    virtual double          GetRelDY() const = 0;
    virtual bool            SetRelDY( double dVal ) = 0;

    // for Ken Burns processing (new since 28.8.2015)
    virtual bool            IsKenBurns() const = 0;
    virtual void            SetKenBurns( bool enable ) = 0;

    virtual double          GetRelXEnd() const = 0;
    virtual bool            SetRelXEnd( double dVal ) = 0;
    virtual double          GetRelYEnd() const = 0;
    virtual bool            SetRelYEnd( double dVal ) = 0;
    virtual double          GetRelDXEnd() const = 0;
    virtual bool            SetRelDXEnd( double dVal ) = 0;
    virtual double          GetRelDYEnd() const = 0;
    virtual bool            SetRelDYEnd( double dVal ) = 0;
};

/** represents data for the dia-presentation (document). 
  */
class IDiaPresentation
{
public:
    virtual ~IDiaPresentation() {}

	virtual bool			IsChanged() const = 0;

    virtual string      	GetDocName() const = 0;
    virtual string  		GetDescription() const = 0;
	virtual int  			GetDiaCount() const = 0;
	virtual IDia * 			GetDia( int iIndex ) = 0;
	virtual IDia *			AddNewDia() = 0;

	// new since 19.6.2003
	virtual int				GetSoundFileCount() const = 0;
    virtual string          GetSoundFileNameAt( int iIndex ) const = 0;
    virtual bool			SetSoundFileNameAt( int iIndex, const string & sFileName ) = 0;

	virtual IDiaOutputWindow * GetOutputWindow() = 0;

	// ** WARNING: load should be the last command in the script, 
	// **          because this command replaces the document !
	// **		   Use Load-Event to start the successor presentation.
    virtual bool			LoadPresentation( const string & sFileName, bool bExecuteScript = true ) = 0;
	virtual bool			SavePresentation() = 0;
    virtual bool			SavePresentationAs( const string & sFileName ) = 0;

	virtual bool			GotoPosition( int iPosition ) = 0;
	virtual bool			PlayFrom( int iFromDiaNo ) = 0;
	virtual bool			Play() = 0;
	virtual bool			Stop() = 0;
	virtual bool			Pause() = 0;
	virtual bool			Continue() = 0;

	virtual bool			IsPlayModus() const = 0;
	virtual bool			IsPauseModus() const = 0;
	virtual bool			IsEditModus() const = 0;
    virtual string      	GetPlayModusStrg() const = 0;

	virtual bool			IsSimulation() const = 0;
	virtual void			SetSimulation( bool bSimulation ) = 0;
};

// *************************************************************************
// ** some global functions for the Script-Interpreter (Python) **
// *************************************************************************

// ** access to the data of the dia presentation
IDiaPresentation * GetDiaPresentation();

// ** output of strings
void PrintLn( const string & sText );

// ** show a (modal) message box 
// ** return: -1 == Undefined, 0 == Ok, 1 == Cancel
int	DoMessageBox( const string & sMsg, const string & sTitle );

// ** sleep a few milli seconds
void SleepMS( int iMS );

// ** convert color values between RGB and HSV color model
IHsv RgbToHsv( const IColor & aColor );
IColor HsvToRgb( const IHsv & aHsv );

// *************************************************************************
// *************************************************************************
// *************************************************************************

#ifndef SWIG

#include "minbase.h"

const long g_IGeneralScriptEnvironmentID	= 500;
const long g_IGeneralScriptVMID				= 510;

const long g_IGeneralScriptFcnID			= 600;
const long g_IMinDiaScriptFcnID				= 610;

class QWidget;
class QMenu;
class QGraphicsScene;

// *************************************************************************
// internal helper Interface, needed for the implementation of 
// the Dynamic Graphics Opreations.
class IDiaOutputWindowInternal : public IDiaOutputWindow
{
public:
    virtual QGraphicsScene *	 GetCanvas() = 0;
};

// *************************************************************************
// general support-functions for a script-interpreter
class IGeneralScriptFcn : public minService
{
public:
	IGeneralScriptFcn( const string & sModuleName )
	: minService( g_IGeneralScriptFcnID, sModuleName )
	{}

	virtual bool IsOk() const = 0;

	// ** get the main-window pointer to register the script-menus
	virtual QWidget *			GetMainWindowPtr() = 0;
	// ** get the menu pointer to register the plugin-submenus
	virtual QMenu *		        GetPluginsMenuPtr() = 0;
	// ** get the actual language, example: "en" or "de"
    virtual string      		GetLanguage() const = 0;
	// ** get the help/shared directory of MinDia 
	// ** (where language- and help-files are stored)
    virtual string      		GetHelpDirecotry() const = 0;
	// ** get the (default-)directory where the scripts are installed
    virtual string      		GetScriptDirecotry() const = 0;
	// ** get the (default-)directory where images can be found
    virtual string          	GetImageDirecotry() const = 0;
	// ** get the (default-)directory where music files can be found
    virtual string      		GetMusicDirecotry() const = 0;
	// ** get the (default-)directory where presentation files can be found
    virtual string      		GetDataDirecotry() const = 0;
};


// *************************************************************************
// script-interpreter to mindia (mindia-support for script-interpreter)
class IMinDiaScriptFcn : public minService
{
public:
	IMinDiaScriptFcn( const string & sModuleName )
	: minService( g_IMinDiaScriptFcnID, sModuleName )
	{}

	virtual bool IsOk() const = 0;

	virtual bool				IsEditMode() const = 0;
	virtual bool				IsPlayMode() const = 0;
	virtual bool				IsPauseMode() const = 0;

	// ** get initial reference for script-interpreter to the document
	virtual IDiaPresentation * 	GetDiaPresentation() = 0;

	virtual int 				GetArgCount() const = 0;
	virtual char * * 			GetArgVec() const = 0;
};


// *************************************************************************
// abstract script environment for applications (mindia using scripts)
class IGeneralScriptEnvironment : public minService
{
public:
	IGeneralScriptEnvironment( const string & sModuleName, minDll * pDll )
	: minService( g_IGeneralScriptEnvironmentID, sModuleName, pDll )
	{}

	enum ScriptLanguage { UNKNOWN, PYTHON, PERL, TCL, SCHEME, CPP };

	virtual bool	IsOk() const = 0;

	virtual bool			HasLanguage( ScriptLanguage aLanguage ) const = 0;
	virtual string			GetLanguageStrg( ScriptLanguage aLanguage ) const = 0;
	virtual ScriptLanguage	GetLanguageFromStrg( const string & sLanguage ) const = 0;

	virtual bool			ModifyScript( const string & sUserComment, string & sScriptInOut, ScriptLanguage aLanguage ) = 0;
	virtual int				ExecuteScript( const string & sScriptIn, ScriptLanguage aLanguage ) = 0;
};


// *************************************************************************
// NOW INTERNAL: helper-class for IGeneralScriptEnvironment
// interface to a script-interpreter (mindia using scripts)
class IGeneralScriptVM : public minService
{
public:
	IGeneralScriptVM( const string & sModuleName, minDll * pDll )
	: minService( g_IGeneralScriptVMID, sModuleName, pDll )
	{}

	virtual bool	IsOk() const = 0;

	virtual int		ExecuteScript( const string & sScript ) = 0;
};


#endif // !SWIG

#endif
