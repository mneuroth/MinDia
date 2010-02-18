/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/src/timelineaxis.h,v $
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

#ifndef _TIMELINEAXIS_H
#define _TIMELINEAXIS_H

#include "minhandle.h"

#include <q3canvas.h>

#include <vector>

using namespace std;

// *******************************************************************
/** Handels a timeline-axis.
  */
class TimeLineAxis
{
	typedef pair< minHandle<Q3CanvasLine>, minHandle<Q3CanvasText> > AxisTick;
	typedef vector< AxisTick >	AxisTickContainer;

public:
	TimeLineAxis( Q3Canvas * pCanvas, double dFactor, int iStartPosY );
	virtual ~TimeLineAxis();

	void	SetAxisLength( double dTotalTime );
	double	GetAxisLength() const;

private:
	// ** helper methods **
	void MakeSecondTicks();

	// ** gui data **
	Q3Canvas *				m_pCanvas;				// !!! NO OWNER !!!
	minHandle<Q3CanvasLine>	m_hTimeAxis;
	AxisTickContainer		m_aTickContainer;

	// ** document data **
	double					m_dTotalTime;
	double					m_dFactor;
	int						m_iStartPosY;
};

#endif

