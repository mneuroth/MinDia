/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/src/timelineaxis.cpp,v $
 *
 *  $Revision: 1.2 $
 *
 *	$Log: not supported by cvs2svn $
 *	Revision 1.1.1.1  2003/08/15 16:38:22  min
 *	Initial checkin of MinDia Ver. 0.97.1
 *	
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

#include "timelineaxis.h"
#include "soundinfo.h"
#include "misctools.h"

#include <QGraphicsScene>
#include <QGraphicsLineItem>

// *******************************************************************
// *******************************************************************
// *******************************************************************

TimeLineAxis::TimeLineAxis( QGraphicsScene * pCanvas, double dFactor, int iStartPosY )
: m_pCanvas( pCanvas ),
  m_dFactor( dFactor ),
  m_iStartPosY( iStartPosY )
{
    m_hTimeAxis	= minHandle<QGraphicsLineItem>( new QGraphicsLineItem() );
    m_pCanvas->addItem(m_hTimeAxis.GetPtr());
	m_hTimeAxis->show();
}

TimeLineAxis::~TimeLineAxis()
{
}

void TimeLineAxis::SetAxisLength( double dTotalTime )
{
	m_dTotalTime = dTotalTime;	
	
	int iTotalLength = (int)(m_dTotalTime*m_dFactor);

    m_hTimeAxis->setLine( 0, m_iStartPosY, iTotalLength, m_iStartPosY );

	MakeSecondTicks();
}

double TimeLineAxis::GetAxisLength() const
{
	return m_dTotalTime;
}

void TimeLineAxis::MakeSecondTicks()
{
	m_aTickContainer.erase( m_aTickContainer.begin(), m_aTickContainer.end() );

	int iMaxSeconds = (int)m_dTotalTime;
	// for a nicer display of ticks...
	if( iMaxSeconds>0 )
	{
		iMaxSeconds++;
	}
	for( int i=0; i<iMaxSeconds; i = i+10 )
	{
		int iPos = (int)(((double)i)*m_dFactor);

        QGraphicsLineItem * pLine = new QGraphicsLineItem();
        m_pCanvas->addItem(pLine);
        pLine->setLine( 0, 0, 0, 6 );
        pLine->setPos( iPos, m_iStartPosY );
		pLine->show();
		
        QGraphicsSimpleTextItem * pText = new QGraphicsSimpleTextItem();
        m_pCanvas->addItem(pText);
		QString sText;
		if( i>60 )
		{
			string s = SecondsInMinSec( i );
            sText = ToQString( s );
			sText += "min";
		}
		else
		{
			sText.setNum( i );
			sText += "s";
		}
		pText->setText( sText );
        pText->setPos( iPos, m_iStartPosY+10 );
		pText->show();

        m_aTickContainer.push_back( AxisTick( minHandle<QGraphicsLineItem>( pLine ), minHandle<QGraphicsSimpleTextItem>( pText ) ) );
	}
}
