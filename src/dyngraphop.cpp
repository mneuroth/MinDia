/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002-2003 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/src/dyngraphop.cpp,v $
 *
 *  $Revision: 1.6 $
 *
 *	$Log: not supported by cvs2svn $
 *	Revision 1.5  2004/04/09 15:20:07  min
 *	Added new methods for movie support
 *	
 *	Revision 1.4  2004/03/19 13:24:48  min
 *	Disable dynamic graphic operations for the Zaurus.
 *	
 *	Revision 1.3  2004/01/18 23:50:15  min
 *	Bugfixes and new class for relative position of text implemented.
 *	
 *	Revision 1.2  2003/09/24 22:15:51  min
 *	portability fixes
 *	
 *	Revision 1.1.1.1  2003/08/15 16:38:21  min
 *	Initial checkin of MinDia Ver. 0.97.1
 *	
 *
 ***************************************************************************/
/***************************************************************************
 *																		   *
 * This file is part of the MinDia package (program to make slide shows),  *
 *																		   *
 * Copyright (C) 2002-2003 by Michael Neuroth.							   *
 *                                                                         *
 * This program is free software; you can redistribute it and/or modify    *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation; either version 2 of the License, or       *
 * (at your option) any later version.                                     *
 *                                                                         *
 ***************************************************************************/

#include "iscript.h"

#include "dyngraphop.h"

#include "minutils.h"

#include <QGraphicsScene>

// *******************************************************************
// *******************************************************************
// *******************************************************************

const char * c_sSetRel = "SetRel";
const char * c_sMove = "Move";
const char * c_sMoveRel = "MoveRel";
const char * c_sFade = "Fade";
const char * c_sColor = "ChangeColor";
const char * c_sFontsize = "ChangeFontsize";
const char * c_sHide = "Hide";
const char * c_sShow = "Show";
const char * c_sDelay = "Delay";

//********************************************************************

OpItem_ChangeColor::OpItem_ChangeColor( DynText * pItem, QColor aStartColor, QColor aStopColor, int iTimeInMS, const string & sClassName )
: OpItem_Base( pItem, sClassName )
{
    m_iTimeInMS = iTimeInMS;
    m_aStartColor = aStartColor;
    m_aStopColor = aStopColor;
	m_iCount = 0;
	m_dSteps = 0;
    Update();
}

void OpItem_ChangeColor::Update()
{
	OpItem_Base::Update();
    m_dSteps = (double)(m_iTimeInMS / m_iTimeout);
    m_aRedDelta = Delta(m_aStartColor.red(),m_aStopColor.red(),m_dSteps);
    m_aGreenDelta = Delta(m_aStartColor.green(),m_aStopColor.green(),m_dSteps);
    m_aBlueDelta = Delta(m_aStartColor.blue(),m_aStopColor.blue(),m_dSteps);
    m_iCount = 0;
}

void OpItem_ChangeColor::Reset()
{
	OpItem_Base::Reset();
    m_pItem->setBrush( QColor(m_aStartColor) );
}

bool OpItem_ChangeColor::Write( ostream & aStream ) const
{
	OpItem_Base::Write( aStream );

	FileUtilityObj aFU;

	aFU.WriteStructBegin( aStream );
	aStream << m_aStartColor.red();
	aFU.WriteSeparator( aStream );
	aStream << m_aStartColor.green();
	aFU.WriteSeparator( aStream );
	aStream << m_aStartColor.blue();
	aFU.WriteSeparator( aStream );
	aStream << m_aStopColor.red();
	aFU.WriteSeparator( aStream );
	aStream << m_aStopColor.green();
	aFU.WriteSeparator( aStream );
	aStream << m_aStopColor.blue();
	aFU.WriteSeparator( aStream );
	aStream << m_iTimeInMS;
	aFU.WriteSeparator( aStream );
	aStream << m_iDelayInMS;
	aFU.WriteStructEnd( aStream );

	return aStream.good();
}

bool OpItem_ChangeColor::Read( istream & aStream )
{
	Update();
	
	FileUtilityObj aFU;
	int iRed,iGreen,iBlue;

	aFU.ReadStructBegin( aStream );
	aStream >> iRed;
	aFU.ReadSeparator( aStream );
	aStream >> iGreen;
	aFU.ReadSeparator( aStream );
	aStream >> iBlue;
	aFU.ReadSeparator( aStream );
	m_aStartColor = QColor( iRed, iGreen, iBlue );
	aStream >> iRed;
	aFU.ReadSeparator( aStream );
	aStream >> iGreen;
	aFU.ReadSeparator( aStream );
	aStream >> iBlue;
	aFU.ReadSeparator( aStream );
	m_aStopColor = QColor( iRed, iGreen, iBlue );
	aStream >> m_iTimeInMS;
	aFU.ReadSeparator( aStream );
	aStream >> m_iDelayInMS;
	aFU.ReadStructEnd( aStream );

	Update();

	return aStream.good();
}

void OpItem_ChangeColor::sltOnTimer()
{
    m_aTimer->stop();

    if( m_iCount == 0 )
	{
        //m_pItem->show();
	}
    if( m_iCount < m_dSteps )
	{
        QColor aColor = QColor( m_aRedDelta.GetActValue(m_iCount), m_aGreenDelta.GetActValue(m_iCount), m_aBlueDelta.GetActValue(m_iCount));
        m_pItem->setBrush( QColor(aColor) );
        m_iCount++;
        if( m_pItem->scene() )
		{
            m_pItem->scene()->update();
		}
        m_aTimer->start( m_iTimeout );
	}
	else
	{
		m_bDone = true;
	}
}

//********************************************************************

OpItem_FadeInOut::OpItem_FadeInOut( bool bFadeIn, DynText * pItem, int iTimeInMS )
: OpItem_ChangeColor( pItem, QColor(0,0,0), QColor(0,0,0), iTimeInMS, c_sFade )
{
    m_bFadeIn = bFadeIn;
    Update();
}

void OpItem_FadeInOut::Update()
{
	if( m_pItem )
	{
		if( m_bFadeIn )
		{
            if( m_pItem->scene() )
			{
                m_aStartColor = m_pItem->scene()->backgroundBrush().color();
			}
            m_aStopColor = m_pItem->brush().color();
		}
		else
		{
            m_aStartColor = m_pItem->brush().color();
            if( m_pItem->scene() )
			{
                m_aStopColor = m_pItem->scene()->backgroundBrush().color();
			}
		}
	}
	OpItem_ChangeColor::Update();
}

void OpItem_FadeInOut::Reset()
{
    // FadeIn/Out aendern die Farbe nicht mehr !
    // Nur die Counter zuruecksetzen !
	OpItem_Base::Reset();
}

bool OpItem_FadeInOut::Write( ostream & aStream ) const
{
	OpItem_Base::Write( aStream );

	FileUtilityObj aFU;

	aFU.WriteStructBegin( aStream );
	aStream << m_bFadeIn;
	aFU.WriteSeparator( aStream );
	aStream << m_iTimeInMS;
	aFU.WriteSeparator( aStream );
	aStream << m_iDelayInMS;
	aFU.WriteStructEnd( aStream );

	return aStream.good();
}

bool OpItem_FadeInOut::Read( istream & aStream )
{
	FileUtilityObj aFU;

	aFU.ReadStructBegin( aStream );
	aStream >> m_bFadeIn;
	aFU.ReadSeparator( aStream );
	aStream >> m_iTimeInMS;
	aFU.ReadSeparator( aStream );
	aStream >> m_iDelayInMS;
	aFU.ReadStructEnd( aStream );

	Update();

	return aStream.good();
}

//********************************************************************

OpItem_Hide::OpItem_Hide( DynText * pItem, int iTimeInMS, const string & sClassName )
: OpItem_Base( pItem, sClassName )
{
    m_iDelayInMS = iTimeInMS;
    Update();
}

void OpItem_Hide::Reset()
{
	OpItem_Base::Reset();
    //m_pItem->show();
}

bool OpItem_Hide::Write( ostream & aStream ) const
{
	OpItem_Base::Write( aStream );

	FileUtilityObj aFU;

	aFU.WriteStructBegin( aStream );
	aStream << m_iDelayInMS;
	aFU.WriteStructEnd( aStream );

	return aStream.good();
}

bool OpItem_Hide::Read( istream & aStream )
{
	FileUtilityObj aFU;

	aFU.ReadStructBegin( aStream );
	aStream >> m_iDelayInMS;
	aFU.ReadStructEnd( aStream );

	Update();

	return aStream.good();
}

void OpItem_Hide::sltOnTimer()
{
    m_aTimer->stop();
    m_pItem->hide();
    if( m_pItem->scene() )
	{
        m_pItem->scene()->update();
	}
    m_bDone = true;
}

//********************************************************************

OpItem_Show::OpItem_Show( DynText * pItem, int iTimeInMS )
: OpItem_Hide( pItem, iTimeInMS, c_sShow )
{
}

void OpItem_Show::sltOnTimer()
{
    m_aTimer->stop();
    m_pItem->show();
    if( m_pItem->scene() )
	{
        m_pItem->scene()->update();
	}
    m_bDone = true;
}

int OpItem_Show::GetShowAtTimeInMS() const
{
	return GetDelayInMS();
}

void OpItem_Show::SetShowAtTimeInMS( int iTimeInMS )
{
	SetDelayInMS( iTimeInMS );
}

//********************************************************************

OpItem_ChangeFontSize::OpItem_ChangeFontSize( DynText * pItem, int iStartSize, int iStopSize, int iTimeInMS )
: OpItem_Base( pItem, c_sFontsize )
{
    m_iTimeInMS = iTimeInMS;
    m_iStartSize = iStartSize;
    m_iStopSize = iStopSize;
    Update();
}

void OpItem_ChangeFontSize::Update()
{
	OpItem_Base::Update();
    m_dSteps = (double)(m_iTimeInMS / m_iTimeout);
    m_aDelta = Delta( m_iStartSize, m_iStopSize, m_dSteps );
    m_iCount = 0;
}

void OpItem_ChangeFontSize::Reset()
{
	OpItem_Base::Reset();
    QFont aFont = m_pItem->font();
    aFont.setPointSize( m_iStartSize );
    m_pItem->setFont( aFont );
}

bool OpItem_ChangeFontSize::Write( ostream & aStream ) const
{
	OpItem_Base::Write( aStream );

	FileUtilityObj aFU;

	aFU.WriteStructBegin( aStream );
	aStream << m_iStartSize;
	aFU.WriteSeparator( aStream );
	aStream << m_iStopSize;
	aFU.WriteSeparator( aStream );
	aStream << m_iTimeInMS;
	aFU.WriteSeparator( aStream );
	aStream << m_iDelayInMS;
	aFU.WriteStructEnd( aStream );

	return aStream.good();
}

bool OpItem_ChangeFontSize::Read( istream & aStream )
{
	FileUtilityObj aFU;

	aFU.ReadStructBegin( aStream );
	aStream >> m_iStartSize;
	aFU.ReadSeparator( aStream );
	aStream >> m_iStopSize;
	aFU.ReadSeparator( aStream );
	aStream >> m_iTimeInMS;
	aFU.ReadSeparator( aStream );
	aStream >> m_iDelayInMS;
	aFU.ReadStructEnd( aStream );

	Update();

	return aStream.good();
}

void OpItem_ChangeFontSize::sltOnTimer()
{
    m_aTimer->stop();
    if( m_iCount == 0 )
	{
        //m_pItem->show();
	}
    if( m_iCount < m_dSteps )
	{
        int iSize = m_aDelta.GetActValue(m_iCount);
        QFont aFont = m_pItem->font();
        aFont.setPointSize( iSize );
        m_pItem->setFont( aFont );
        m_iCount++;
        if( m_pItem->scene() )
		{
            m_pItem->scene()->update();
		}
        m_aTimer->start( m_iTimeout );
	}
	else
	{
		m_bDone = true;
	}
}

//********************************************************************

OpItem_SetRelPos::OpItem_SetRelPos( DynText * pItem, double dRelX, double dRelY, const string & sClassName )
: OpItem_Base( pItem, sClassName )
{
	m_dRelX = dRelX;
	m_dRelY = dRelY;
}

void OpItem_SetRelPos::Update()
{
    QGraphicsScene * pCanvas = m_pItem->scene();

	if( pCanvas && m_dRelX>=0 && m_dRelY>=0 )
	{
		int x = (int)pCanvas->width()*m_dRelX;
		int y = (int)pCanvas->height()*m_dRelY;

        m_pItem->setPos(x,y);
	}
}

void OpItem_SetRelPos::Reset()
{
	Update();
}

bool OpItem_SetRelPos::Write( ostream & aStream ) const
{
	OpItem_Base::Write( aStream );

	FileUtilityObj aFU;

	aFU.WriteStructBegin( aStream );
	aStream << m_dRelX;
	aFU.WriteSeparator( aStream );
	aStream << m_dRelY;
	aFU.WriteStructEnd( aStream );

	return aStream.good();
}

bool OpItem_SetRelPos::Read( istream & aStream )
{
	FileUtilityObj aFU;

	aFU.ReadStructBegin( aStream );
	aStream >> m_dRelX;
	aFU.ReadSeparator( aStream );
	aStream >> m_dRelY;
	aFU.ReadStructEnd( aStream );

	return aStream.good();
}

bool OpItem_SetRelPos::SetRelPos( double xRel, double yRel )
{
	m_dRelX = xRel;
	m_dRelY = yRel;
	return true;
}

bool OpItem_SetRelPos::GetRelPos( double & xRel, double & yRel )
{
	xRel = m_dRelX;
	yRel = m_dRelY;
	return true;
}


//********************************************************************

OpItem_MoveTo::OpItem_MoveTo( DynText * pItem, int iFromPosX, int iFromPosY, int iToPosX, int iToPosY, int iTimeInMS, const string & sClassName )
: OpItem_Base( pItem, sClassName )
{
    m_iTimeInMS = iTimeInMS;
    m_iToPosX = iToPosX;
    m_iToPosY = iToPosY;
    m_iStartPosX = iFromPosX;
    m_iStartPosY = iFromPosY;
	m_dDeltaX = 0;
	m_dDeltaY = 0;
	m_iCount = 0;
	m_dSteps = 0;
	m_bIsDone = true;
    Update();
}

void OpItem_MoveTo::Update()
{
	OpItem_Base::Update();
    m_dSteps = (double)( m_iTimeInMS / m_iTimeout);
    m_dDeltaX = (m_iToPosX-m_iStartPosX)/m_dSteps;
    m_dDeltaY = (m_iToPosY-m_iStartPosY)/m_dSteps;
    m_iCount = 0;
}
        
void OpItem_MoveTo::Reset()
{
	OpItem_Base::Reset();
    MoveMe( m_iStartPosX, m_iStartPosY );
    //m_pItem->show();
}

void OpItem_MoveTo::sltOnTimer()
{
    m_aTimer->stop();
    if( m_iCount == 0 )
	{
        //m_pItem->show();
	}
    if( m_iCount < m_dSteps )
	{
        MoveMe( m_iStartPosX+m_iCount*m_dDeltaX, m_iStartPosY+m_iCount*m_dDeltaY );
        m_iCount++;
        if( m_pItem->scene() )
		{
            m_pItem->scene()->update();
		}
        m_aTimer->start( m_iTimeout );
	}
    else
	{
        m_bIsDone = true;
	}
}

bool OpItem_MoveTo::Write( ostream & aStream ) const
{
	OpItem_Base::Write( aStream );

	FileUtilityObj aFU;

	aFU.WriteStructBegin( aStream );
	aStream << m_iStartPosX;
	aFU.WriteSeparator( aStream );
	aStream << m_iStartPosY;
	aFU.WriteSeparator( aStream );
	aStream << m_iToPosX;
	aFU.WriteSeparator( aStream );
	aStream << m_iToPosY;
	aFU.WriteSeparator( aStream );
	aStream << m_iTimeInMS;
	aFU.WriteSeparator( aStream );
	aStream << m_iDelayInMS;
	aFU.WriteStructEnd( aStream );

	return aStream.good();
}

bool OpItem_MoveTo::Read( istream & aStream )
{
	FileUtilityObj aFU;

	aFU.ReadStructBegin( aStream );
	aStream >> m_iStartPosX;
	aFU.ReadSeparator( aStream );
	aStream >> m_iStartPosY;
	aFU.ReadSeparator( aStream );
	aStream >> m_iToPosX;
	aFU.ReadSeparator( aStream );
	aStream >> m_iToPosY;
	aFU.ReadSeparator( aStream );
	aStream >> m_iTimeInMS;
	aFU.ReadSeparator( aStream );
	aStream >> m_iDelayInMS;
	aFU.ReadStructEnd( aStream );

	Update();

	return aStream.good();
}

void OpItem_MoveTo::MoveMe( double x, double y )
{
    m_pItem->move(x,y);
}

//********************************************************************

OpItem_MoveToRel::OpItem_MoveToRel( DynText * pItem, int iFromPosX, int iFromPosY, int iToPosX, int iToPosY, int iTimeInMS, const string & sClassName )
: OpItem_MoveTo( pItem, iFromPosX, iFromPosY, iToPosX, iToPosY, iTimeInMS, sClassName )
{
}

void OpItem_MoveToRel::MoveMe( double x, double y )
{
	int iMaxX = 0, iMaxY = 0;
    if( m_pItem->scene() )
	{
        iMaxX = m_pItem->scene()->width();
        iMaxY = m_pItem->scene()->height();
	}
    double relX = x*iMaxX/100;
    double relY = y*iMaxY/100;
    m_pItem->move(relX,relY);
}

//********************************************************************

OpItem_Base::OpItem_Base( DynText * pItem, const string & sTypeName, int iDelayInMS )
: m_sTypeName( sTypeName ),
  m_bDone( true ),
  m_iDelayInMS( iDelayInMS ),
  m_iElapsedTimeInMS( 0 ),
  m_iTimeout( 10 ),
  m_aTimer( 0 ),
  m_pItem( pItem )
{
	m_aTimer = new QTimer();
    connect( m_aTimer, SIGNAL(timeout()), this, SLOT( sltOnTimer() ) );
}

OpItem_Base::~OpItem_Base()
{
	delete m_aTimer;
}

const string & OpItem_Base::GetTypeName() const
{
    return m_sTypeName; 
}
    
void OpItem_Base::SetItem( DynText * pItem )
{
	m_pItem = pItem;
}
    
DynText * OpItem_Base::GetItem() const
{
    return m_pItem;
}

bool OpItem_Base::IsDone() const
{
	return m_bDone;
}

int OpItem_Base::GetDelayInMS() const
{
	return m_iDelayInMS;
}

void OpItem_Base::SetDelayInMS( int iDelayInMS )
{
	m_iDelayInMS = iDelayInMS;
}

int OpItem_Base::GetShowAtTimeInMS() const
{
	// everything is imediately visible !
	return 0;
}

void OpItem_Base::SetShowAtTimeInMS( int /*iTimeInMS*/ )
{
	// dummy
}

bool OpItem_Base::SetRelPos( double /*xRel*/, double /*yRel*/ )
{
	// dummy
	return false;
}

bool OpItem_Base::GetRelPos( double & /*xRel*/, double & /*yRel*/ )
{
	// dummy
	return false;
}

void OpItem_Base::Update()
{
    m_bDone = true;
}

void OpItem_Base::Run( int iStartFromPosInMS )
{
    m_bDone = false;

	//int iRet = 0;
	if( (iStartFromPosInMS==0) || (m_iDelayInMS-iStartFromPosInMS>0) )
	{
		if( m_iDelayInMS>0 )
		{
			/*iRet =*/ m_aTimer->start( m_iDelayInMS - iStartFromPosInMS );		// bugfix 21.6.2003 subtract the start pos !
		}
		else
		{
			/*iRet =*/ m_aTimer->start( /*m_iTimeout*/0 );
		}
	}
    m_aTime.start();
    m_iElapsedTimeInMS = 0;		// min todo --> hier noch m_iDelayInMS beachten !
}

void OpItem_Base::Pause()
{
    m_aTimer->stop();
    m_iElapsedTimeInMS = m_iElapsedTimeInMS + m_aTime.elapsed();
}

void OpItem_Base::Continue()
{
    if( m_iElapsedTimeInMS<m_iDelayInMS )
	{
        m_aTimer->start( m_iDelayInMS-m_iElapsedTimeInMS );
	}
    else
	{
        m_aTimer->start( m_iTimeout );
	}
    m_aTime.restart();
}

void OpItem_Base::Stop()
{
    m_aTimer->stop();
    m_bDone = true;
}

void OpItem_Base::Reset()
{
    m_aTimer->stop();
    Update();
}

void OpItem_Base::sltOnTimer()
{
    m_aTimer->stop();
    m_bDone = true;
}        

bool OpItem_Base::Write( ostream & aStream ) const
{
	FileUtilityObj aFU;

	aFU.WriteTypeBegin( aStream );
	WriteString( aStream, m_sTypeName );
	aFU.WriteTypeEnd( aStream );

	return aStream.good();
}

bool OpItem_Base::Read( istream & aStream )
{
	FileUtilityObj aFU;

	aFU.ReadTypeBegin( aStream );
	ReadString( aStream, m_sTypeName );
	aFU.ReadTypeEnd( aStream );

	return aStream.good();
}

OpItem_Base * OpItem_Base::CreateFromType( const string & sTypeName )
{
	if( sTypeName == c_sSetRel )
	{
		return new OpItem_SetRelPos( 0, -1.0, -1.0 );
	}
	else if( sTypeName == c_sMove )
	{
		return new OpItem_MoveTo( 0 );
	}
	else if( sTypeName == c_sMoveRel )
	{
		return new OpItem_MoveToRel( 0 );
	}
	else if( sTypeName == c_sFade )
	{
		return new OpItem_FadeInOut( true, 0 );
	}
	else if( sTypeName == c_sColor )
	{
		return new OpItem_ChangeColor( 0 );
	}
	else if( sTypeName == c_sFontsize )
	{
		return new OpItem_ChangeFontSize( 0 );
	}
	else if( sTypeName == c_sHide )
	{
		return new OpItem_Hide( 0 );
	}
	else if( sTypeName == c_sShow )
	{
		return new OpItem_Show( 0 );
	}
/*	else if( sTypeName == c_sDelay )
	{
		return 0;
	}
*/	return 0;
}

//********************************************************************

DynContainer::DynContainer( IDiaOutputWindowInternal * pOutputWindowProxy )
: IOContainer< DynText >( "DynContainer" ),
  m_bIsPause( false ),
  m_pOutputWindowProxy( pOutputWindowProxy )
  //m_pCanvas( pCanvas )
{
}

void DynContainer::Update()
{
    if( m_pOutputWindowProxy && m_pOutputWindowProxy->GetCanvas() )
	{
        m_pOutputWindowProxy->GetCanvas()->update();
	}
}

void DynContainer::UpdateInfos()
{
	iterator aIter = begin();

	while( aIter != end() )
	{
        (*aIter)->SetCanvas( m_pOutputWindowProxy->GetCanvas() );
		++aIter;
	}
}

bool DynContainer::ImportFile( const string & sFileName )
{
	fstream aFile( sFileName.c_str(), ios::in );

	if( aFile.good() )
	{
		return Read( aFile );
	}

	return aFile.good();
}

bool DynContainer::Read( istream & aStream )
{
	bool bOk = IOContainer< DynText >::Read( aStream );

	return bOk;
}

bool DynContainer::Write( ostream & aStream ) const
{
	bool bOk = IOContainer< DynText >::Write( aStream );

	//Update();
    if( m_pOutputWindowProxy && m_pOutputWindowProxy->GetCanvas() )
	{
        m_pOutputWindowProxy->GetCanvas()->update();
	}

	return bOk;
}

void DynContainer::RunSelected( int iStartFromPosInMS )
{
	UpdateInfos();

    Reset();

	const_iterator aIter = begin();
	while( aIter != end() )
	{
		/*IOContainer<DynText>::*/HandleType hItem = *aIter;

		if( hItem->IsSelected() )
		{
			//hItem->Show();
			hItem->Run( iStartFromPosInMS );
		}
		else
		{
			hItem->Hide();
		}

		++aIter;
	}

	Update();
}

void DynContainer::Run( int iStartFromPosInMS )
{
	UpdateInfos();

    if( m_bIsPause )
	{
        Continue();
	}
    else
	{
        Reset();
		const_iterator aIter = begin();
		while( aIter != end() )
		{
			/*IOContainer<DynText>::*/HandleType hItem = *aIter;

			//hItem->Show();
			hItem->Run( iStartFromPosInMS );

			++aIter;
		}
	}

	Update();
}

void DynContainer::Pause()
{
    m_bIsPause = true;

	const_iterator aIter = begin();
	while( aIter != end() )
	{
		/*IOContainer<DynText>::*/HandleType hItem = *aIter;

		hItem->Pause();

		++aIter;
	}

    Update();
}

void DynContainer::Continue()
{
	const_iterator aIter = begin();
	while( aIter != end() )
	{
		/*IOContainer<DynText>::*/HandleType hItem = *aIter;

		hItem->Continue();

		++aIter;
	}

    m_bIsPause = false;
    Update();
}

void DynContainer::Stop()
{
	const_iterator aIter = begin();
	while( aIter != end() )
	{
		/*IOContainer<DynText>::*/HandleType hItem = *aIter;

		hItem->Hide();
		hItem->Stop();

		++aIter;
	}

    m_bIsPause = false;
    Update();
}

void DynContainer::Reset()
{
	const_iterator aIter = begin();
	while( aIter != end() )
	{
		/*IOContainer<DynText>::*/HandleType hItem = *aIter;

		hItem->Reset();
		//hItem->Show();

		++aIter;
	}

    m_bIsPause = false;
    Update();
}

void DynContainer::AddDefaultDynText( const string & sText, double dStartTimeInMS, double dShowTimeInMS )
{
	DynText * pText = new DynText( sText );
	pText->CreateDefaultOperations( dStartTimeInMS, dShowTimeInMS );
	push_back( minHandle<DynText>( pText ) );
	SetChanged();
}

void DynContainer::SetAttributesForAllItems( minHandle<DynText> hOtherItem )
{
	iterator aIter = begin();
	while( aIter != end() )
	{
		/*IOContainer<DynText>::*/HandleType hItem = *aIter;

		hItem->SetAttributesFrom( hOtherItem.GetPtr() );

		++aIter;
	}
	Update();
}

void DynContainer::PaintElementsForTime( QPainter & aPainter, double dTimeMS ) const
{
	const_iterator aIter = begin();
	while( aIter != end() )
	{
		/*IOContainer<DynText>::*/HandleType hItem = *aIter;

		hItem->PaintForTime( aPainter, dTimeMS );

		++aIter;
	}
}

bool DynContainer::IsNextElementChanging( double dTimeMS, double dDeltaMS ) const
{
	const_iterator aIter = begin();
	while( aIter != end() )
	{
		/*IOContainer<DynText>::*/HandleType hItem = *aIter;

		// if only one element is changing --> return true !
		if( hItem->IsNextChanging( dTimeMS, dDeltaMS ) )
		{
			return true;
		}

		++aIter;
	}
	return false;
}

//********************************************************************

DynText::DynText( const string & sText, QGraphicsScene * pOwner )
: QGraphicsSimpleTextItem( sText.c_str(), 0 ),
  m_bIsSelected( false ),
  m_xOld( -1 ),
  m_yOld( -1 ),
  m_pSelectedHelper( 0 )
{
    if( pOwner )
    {
        pOwner->addItem(this);
    }
	Sync();
}

DynText::~DynText()
{
	Deselect();
}

XmlTree	DynText::GetXMLTree() const
{
// min todo gulp
	return XmlTree( "DynText" );
}

void DynText::SetCanvas( QGraphicsScene * pCanvas )
{
    if( pCanvas )
    {
        pCanvas->addItem(this);
    }

	setFont( m_aInitFont );
    setBrush( QColor(m_aInitColor) );
}

void DynText::Sync()
{
	m_aInitFont = font();
    m_aInitColor = brush().color();
}

void DynText::Deselect()
{
	if( m_pSelectedHelper )
	{	
		delete m_pSelectedHelper;
		m_pSelectedHelper = 0;
	}
}

void DynText::SetSelected( bool bSelected )
{
	m_bIsSelected = bSelected;

	if( m_bIsSelected )
	{
		if( !m_pSelectedHelper )
		{
            m_pSelectedHelper = new QGraphicsRectItem( boundingRect() );
            scene()->addItem(m_pSelectedHelper);
			m_pSelectedHelper->setPen( QPen( QColor( 255,0,0 ) ) );
		}
        m_pSelectedHelper->setRect( 0, 0, boundingRect().width(), boundingRect().height() );
		m_pSelectedHelper->show();
	}
	else
	{
		if( m_pSelectedHelper )
		{
			m_pSelectedHelper->hide();
		}
	}
    scene()->update();
}

bool DynText::IsPosInElement( int x, int y ) const
{
    QRectF aRect = boundingRect();

	if( aRect.left()<=x && aRect.right()>=x && aRect.top()<=y && aRect.bottom()>=y )
	{
		return true;
	}

	return false;
}

void DynText::InitMouseMove( int x, int y )
{
	m_xOld = x;
	m_yOld = y;
}

void DynText::DoneMouseMove()
{
	m_xOld = -1;
	m_yOld = -1;
}

bool DynText::IsInMouseMove() const
{
	if( m_xOld>=0 && m_yOld>=0 )
	{
		return true;
	}
	return false;
}

void DynText::DoMouseMove( int x, int y )
{
	if( IsInMouseMove() )
	{
		double dx = x-m_xOld;
		double dy = y-m_yOld;
        move( (double)pos().x()+dx, (double)pos().y()+dy );
		m_xOld = x;
		m_yOld = y;
        scene()->update();
	}
}

void DynText::move( double x, double y )
{
    move( x, y );
	if( m_pSelectedHelper )
	{
        m_pSelectedHelper->setPos( x, y );
	}
}

bool DynText::IsSelected() const
{
	return m_bIsSelected;
}

bool DynText::Write( ostream & aStream ) const
{
	FileUtilityObj aFU;

	aFU.WriteStructBegin( aStream );
    WriteString( aStream, text().toStdString() );
	aFU.WriteSeparator( aStream );
	aStream << x();
	aFU.WriteSeparator( aStream );
	aStream << y();
	aFU.WriteSeparator( aStream );
    WriteString( aStream, font().family().toStdString() );
	aFU.WriteSeparator( aStream );
	aStream << font().pointSize();
	aFU.WriteSeparator( aStream );
    aStream << brush().color().red();
	aFU.WriteSeparator( aStream );
    aStream << brush().color().green();
	aFU.WriteSeparator( aStream );
    aStream << brush().color().blue();
	aFU.WriteSeparator( aStream );
	WriteOpContainer( aStream );
	aFU.WriteStructEnd( aStream );

	return aStream.good();
}

bool DynText::WriteOpContainer( ostream & aStream ) const
{
	FileUtilityObj aFU;

	OperationContainer::const_iterator aIter = m_aOpContainer.begin();

	aFU.WriteStructBegin( aStream );
	while( aIter != m_aOpContainer.end() )
	{
		aFU.WriteStructBegin( aStream );
		(*aIter)->Write( aStream );
		aFU.WriteStructEnd( aStream );
		
		aFU.WriteSeparator( aStream );

		++aIter;
	}
	aFU.WriteStructEnd( aStream );

	return aStream.good();
}

bool DynText::Read( istream & aStream )
{
	FileUtilityObj aFU;

	string sTemp;
	double x,y;
	int iTemp;
	int iRed,iGreen,iBlue;

	aFU.ReadStructBegin( aStream );
	ReadString( aStream, sTemp );
	setText( sTemp.c_str() );
	aFU.ReadSeparator( aStream );
	aStream >> x;
	aFU.ReadSeparator( aStream );
	aStream >> y;
    setPos(x,y);
	aFU.ReadSeparator( aStream );
	ReadString( aStream, sTemp );
	aFU.ReadSeparator( aStream );
	aStream >> iTemp;
	QFont aFont( sTemp.c_str(), iTemp );
	m_aInitFont = aFont;
	setFont( aFont );
	aFU.ReadSeparator( aStream );
	aStream >> iRed;
	aFU.ReadSeparator( aStream );
	aStream >> iGreen;
	aFU.ReadSeparator( aStream );
	aStream >> iBlue;
	QColor aColor( iRed, iGreen, iBlue );
    setBrush( aColor );
	m_aInitColor = aColor;
	aFU.ReadSeparator( aStream );
	ReadOpContainer( aStream );
	aFU.ReadStructEnd( aStream );

	return aStream.good();
}

bool DynText::ReadOpContainer( istream & aStream )
{
	// clear container first
	m_aOpContainer.erase( m_aOpContainer.begin(), m_aOpContainer.end() );

	// and than read the container contents from the stream
	FileUtilityObj aFU;

	aFU.ReadStructBegin( aStream );

	while( !aStream.eof() && !aFU.PeekStructEnd( aStream ) && aStream.good() )
	{
		aFU.ReadStructBegin( aStream );

		OpItem_Base aElement( this );
		aElement.Read( aStream );

		OpItem_Base * pNewItem = OpItem_Base::CreateFromType( aElement.GetTypeName() );
		if( pNewItem )
		{
			pNewItem->SetItem( this );

			pNewItem->Read( aStream );

			m_aOpContainer.push_back( OperationT( pNewItem ) );
		}

		aFU.ReadStructEnd( aStream );

		if( aFU.PeekSeparator( aStream ) )
		{
			aFU.ReadSeparator( aStream );
		}
	}

	aFU.ReadStructEnd( aStream );
	
	return aStream.good();
}

void DynText::AddOperation( OperationT hOp )
{
	m_aOpContainer.push_back( hOp );
}

const DynText::OperationContainer & DynText::GetOperations() const
{
	return m_aOpContainer;
}
    
void DynText::Show()
{
	show();
}

void DynText::Hide()
{
	hide();
	if( m_pSelectedHelper )
	{
		m_pSelectedHelper->hide();
	}
}

void DynText::Run( int iStartFromPosInMS )
{
	OperationContainer::iterator aIter = m_aOpContainer.begin();

	hide();

	while( aIter != m_aOpContainer.end() )
	{
		(*aIter)->Run( iStartFromPosInMS );
		++aIter;
	}
}

void DynText::Pause()
{
	OperationContainer::iterator aIter = m_aOpContainer.begin();

	while( aIter != m_aOpContainer.end() )
	{
		(*aIter)->Pause();
		++aIter;
	}
}

void DynText::Continue()
{
	OperationContainer::iterator aIter = m_aOpContainer.begin();

	while( aIter != m_aOpContainer.end() )
	{
		(*aIter)->Continue();
		++aIter;
	}
}

void DynText::Stop()
{
	OperationContainer::iterator aIter = m_aOpContainer.begin();

	while( aIter != m_aOpContainer.end() )
	{
		(*aIter)->Stop();
		++aIter;
	}
	//Show();		// only in edit modus needed !
}

void DynText::Reset()
{
	setFont( m_aInitFont );
    setBrush( m_aInitColor );

	OperationContainer::iterator aIter = m_aOpContainer.begin();
	
	while( aIter != m_aOpContainer.end() )
	{
		(*aIter)->Reset();
		++aIter;
	}
	//Show();		// only in edit modus needed !
}

bool DynText::IsDone() const
{
	bool bIsDone = true;

	OperationContainer::const_iterator aIter = m_aOpContainer.begin();

	while( aIter != m_aOpContainer.end() )
	{
		if( !(*aIter)->IsDone() )
		{
			return false;
		}
		++aIter;
	}

	return bIsDone;
}

double DynText::GetStartTime() const
{
	double dRet = 0;

	OperationContainer::const_iterator aIter = m_aOpContainer.begin();

	while( aIter != m_aOpContainer.end() )
	{
		if( (*aIter)->GetShowAtTimeInMS()>dRet )
		{
			dRet = (double)(*aIter)->GetShowAtTimeInMS();
		}
		++aIter;
	}

	return dRet;
}

string DynText::GetString() const
{
    string s = text().toStdString();
	return s;
}

void DynText::CreateDefaultOperations( double dStartTimeInMS, double dShowTimeInMS )
{
    setPos( 10, 10 );
    setBrush( QColor( 255, 0, 0 ) );
	setFont( QFont( "Arial", 20 ) );
	AddOperation( minHandle<OpItem_Base>( new OpItem_Hide( this, 0 ) ) );
	//AddOperation( minHandle<OpItem_Base>( new OpItem_MoveTo( this, 0, 0, 100, 100, 10 ) ) );
	//AddOperation( minHandle<OpItem_Base>( new OpItem_ChangeColor( this, QColor( 0, 0, 0 ), QColor( 255, 0, 0 ), 10 ) ) );
	AddOperation( minHandle<OpItem_Base>( new OpItem_Show( this, (int)dStartTimeInMS ) ) );
	AddOperation( minHandle<OpItem_Base>( new OpItem_Hide( this, (int)(dStartTimeInMS+dShowTimeInMS) ) ) );
	// new since 17.1.2004:
	AddOperation( minHandle<OpItem_Base>( new OpItem_SetRelPos( this, -1.0, -1.0 ) ) );
	Sync();
}

const int c_iHideStartIndex = 0;
const int c_iShowIndex = 1;
const int c_iHideStopIndex = 2;
const int c_iSetRelIndex = 3;

void DynText::ChangeDefaultData( double dStartTimeInMS, double dShowTimeInMS )
{
	minHandle<OpItem_Base> hItem = m_aOpContainer[ c_iShowIndex ];
	hItem->SetShowAtTimeInMS( (int)dStartTimeInMS );
	hItem = m_aOpContainer[ c_iHideStopIndex ];
	hItem->SetDelayInMS( (int)(dStartTimeInMS + dShowTimeInMS) );
	Sync();
}

void DynText::GetDefaultData( double & dStartTimeInMS, double & dShowTimeInMS ) const
{
	minHandle<OpItem_Base> hItem = m_aOpContainer[ c_iShowIndex ];
    dStartTimeInMS = (double)hItem->GetShowAtTimeInMS();
	hItem = m_aOpContainer[ c_iHideStopIndex ];
    dShowTimeInMS = (double)(hItem->GetDelayInMS() - dStartTimeInMS);
}

void DynText::Delta( double dDeltaTimeInMS )
{
	minHandle<OpItem_Base> hItem = m_aOpContainer[ c_iShowIndex ];
	hItem->SetShowAtTimeInMS( (int)(hItem->GetShowAtTimeInMS() + dDeltaTimeInMS) );

	hItem = m_aOpContainer[ c_iHideStopIndex ];
	hItem->SetDelayInMS( (int)(hItem->GetDelayInMS() + dDeltaTimeInMS) );
}

bool DynText::SetRelativePos( double xRel, double yRel )
{
	if( (int)m_aOpContainer.size()>c_iSetRelIndex )
	{
		minHandle<OpItem_Base> hItem = m_aOpContainer[ c_iSetRelIndex ];
		return hItem->SetRelPos( xRel, yRel );
	}
	return false;
}

bool DynText::GetRelativePos( double & xRel, double & yRel ) const
{
	if( (int)m_aOpContainer.size()>c_iSetRelIndex )
	{
		minHandle<OpItem_Base> hItem = m_aOpContainer[ c_iSetRelIndex ];
		return hItem->GetRelPos( xRel, yRel );
	}
	return false;
}

void DynText::SetAttributesFrom( DynText * pOtherItem )
{
	if( pOtherItem )
	{
		setFont( pOtherItem->font() );
        setBrush( pOtherItem->brush() );
		Sync();
	}
}

void DynText::PaintForTime( QPainter & aPainter, double dTimeMS ) const
{
	double dStartTimeInMS; 
	double dShowTimeInMS;
	
	GetDefaultData( dStartTimeInMS, dShowTimeInMS );

	if( dTimeMS>=dStartTimeInMS && dTimeMS<=dStartTimeInMS+dShowTimeInMS )
	{
		double xRel, yRel;

        aPainter.setPen( QPen( brush().color() ) );
		aPainter.setFont( font() );
		if( GetRelativePos( xRel, yRel ) )
		{
			int x = xRel*aPainter.viewport().width();
			int y = yRel*aPainter.viewport().height();
			aPainter.drawText( x, y, GetString().c_str() );
		}
		else
		{
			aPainter.drawText( x(), y(), GetString().c_str() );
		}
	}
}

bool DynText::IsNextChanging( double dTimeMS, double dDeltaMS ) const
{
	double dStartTimeInMS; 
	double dShowTimeInMS;
	
	GetDefaultData( dStartTimeInMS, dShowTimeInMS );

	if( ( dTimeMS>=dStartTimeInMS && dTimeMS<=dStartTimeInMS+dShowTimeInMS ) &&
		!( dTimeMS-dDeltaMS>=dStartTimeInMS && dTimeMS-dDeltaMS<=dStartTimeInMS+dShowTimeInMS ) )
	{
		return true;
	}
	return false;
}
