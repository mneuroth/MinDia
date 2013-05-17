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

#ifndef _TIMELINEAXIS_H
#define _TIMELINEAXIS_H

#include "minhandle.h"

#include <QGraphicsLineItem>

#include <vector>

using namespace std;

// *******************************************************************
/** Handels a timeline-axis.
  */
class TimeLineAxis
{
    typedef pair< minHandle<QGraphicsLineItem>, minHandle<QGraphicsSimpleTextItem> > AxisTick;
	typedef vector< AxisTick >	AxisTickContainer;

public:
    TimeLineAxis( QGraphicsScene * pCanvas, double dFactor, int iStartPosY );
	virtual ~TimeLineAxis();

	void	SetAxisLength( double dTotalTime );
	double	GetAxisLength() const;

private:
	// ** helper methods **
    void    MakeSecondTicks();

	// ** gui data **
    QGraphicsScene *                m_pCanvas;				// !!! NO OWNER !!!
    minHandle<QGraphicsLineItem>	m_hTimeAxis;
    AxisTickContainer		        m_aTickContainer;

	// ** document data **
    double                          m_dTotalTime;
    double                          m_dFactor;
    int                             m_iStartPosY;
};

#endif

