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

#ifndef _TIMELINEITEM_H
#define _TIMELINEITEM_H

class QGraphicsScene;
class QGraphicsPolygonItem;
class QGraphicsSimpleTextItem;

class DiaPresentation;

// *******************************************************************
/** Handels a timeline-item, represents a single dia of 
  * the dia presentation.
  */
class TimeLineItem 
{
public:
    TimeLineItem( QGraphicsScene *	pCanvas, DiaPresentation * pDiaPres, int iSlideNo, double dFactor, int iRampSize, bool bIsSelected );
	virtual ~TimeLineItem();

	bool IsSelected() const;
	void SetSelected( bool bValue );

	bool IsSelected( int x, int y ) const;
	bool IsStopBorderSelected( int x, int y ) const;
	bool IsDissolveBorderSelected( int x, int y ) const;

	void ChangeShowTime( double dDeltaTime );
	void ChangeDissolveTime( double dDeltaTime );

	int  GetPositionX() const;
	int  GetPositionY() const;
    int	 GetLength() const;
    int	 GetHeight() const;

private:
	// ** help methods **
	void UpdateItem();
	void CleanUp();

	// ** gui data **
    QGraphicsScene *            m_pCanvas;				// !!! NO OWNER !!!
    QGraphicsPolygonItem *	    m_pDissolveRamp;
    QGraphicsSimpleTextItem *	m_pSlideLabel;

	// ** document data **
    DiaPresentation *	m_pDiaPres;     				// !!! NO OWNER !!!
	int					m_iSlideNo;
	double				m_dFactor;
	int					m_iRampSize;
	int					m_iRampDelta;

	// ** temp data **
	bool				m_bIsSelected;
};

#endif
