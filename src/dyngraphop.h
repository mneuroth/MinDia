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
 * Copyright (C) 2013 by Michael Neuroth.	   						       *
 *                                                                         *
 * This program is free software; you can redistribute it and/or modify    *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation; either version 2 of the License, or       *
 * (at your option) any later version.                                     *
 *                                                                         *
 ***************************************************************************/

#ifndef _DYNGRAPHOP_H
#define _DYNGRAPHOP_H

#include <qobject.h>
#include <qcolor.h>
#include <qtimer.h>
#include <qpainter.h>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsRectItem>

class QTimer;

class DynGraphicOpContainer;
class IDiaOutputWindowInternal;

#include "minhandle.h"
#include "minutils.h"
#include "iocontainer.h"
#include "timerjobproc.h"
#include "writexml.h"

/*
// *******************************************************************
const int ID_SHOW_TEXT = 1;

// *******************************************************************
// to implement a 'dynamic' constructor
class DynamicTyp
{
public:
	DynamicTyp( int iTypeId )
		: m_iTypeId( iTypeId )
	{}

	int GetTypeId() const
	{
		return m_iTypeId;
	}

	bool Read( istream & aStream );
	bool Write( ostream & aStream ) const;

	static Job * CreateJob( int iTypeId, DynGraphicOpContainer * pOwner );

private:
	int		m_iTypeId;
};
	
// *******************************************************************
class dgoShowText : public QObject, public Job, public DynamicTyp
{
	Q_OBJECT

public:
	dgoShowText( DynGraphicOpContainer * pOwner, const string & sText = "", int x = 0, int y = 0, int iStartTimeInMS = 0, int iShowTimeInMS = 0, const QColor aColor = QColor(), int iFontSize = 12, const string & sFontName = "Helvetica" );
	virtual ~dgoShowText();

	// implements the JobInterface
	virtual bool Run();
	virtual void Cancel();

	virtual bool Read( istream & aStream );
	virtual bool Write( ostream & aStream ) const;

protected slots:
	void sltTimerEvent();

private:
	DynGraphicOpContainer *		m_pOwner;	// not an owner !
	QCanvasText *				m_pText;	// temp
	QTimer *					m_pTimer;
	string						m_sText;
	int							m_x;
	int							m_y;
	int							m_iStartTimeInMS;
	int							m_iShowTimeInMS;
	int							m_iFontSize;
	string						m_sFontName;
	QColor						m_aColor;
};

// *******************************************************************
class DynGraphicOpContainer : public JobContainerT, public ObjectChanged
{
public:
	DynGraphicOpContainer( IDiaOutputWindowInternal * pOutputWindowProxy );

	QCanvas *	GetCanvas();

	void AddJob( JobHandle hJob );
	void AddOperation( double dTimeInMS, const string & sText );

	XmlTree	GetXMLTree() const;

	bool	Read( istream & aStream );
	bool	Write( ostream & aStream ) const;

private:
	IDiaOutputWindowInternal *	m_pOutputWindowProxy;	// not an owner !
};
*/
// *******************************************************************
// *******************************************************************
// *******************************************************************

extern const char * c_sSetRel;
extern const char * c_sMove;
extern const char * c_sMoveRel;
extern const char * c_sFade;
extern const char * c_sColor;
extern const char * c_sFontsize;
extern const char * c_sHide;
extern const char * c_sDelay;

class DynText;
/*
// *******************************************************************
class DynOperation
{
public:
	virtual ~DynOperation() {}

	virtual bool Write( ostream & aStream ) const = 0;
	virtual bool Read( istream & aStream ) = 0;

    virtual void Run() = 0;
	virtual void Pause() = 0;
	virtual void Continue() = 0;
	virtual void Stop() = 0;
	virtual void Reset() = 0;

	virtual bool IsDone() const = 0;

//	virtual XmlTree	GetXMLTree() const = 0;
};
*/

// *******************************************************************
class Delta
{
public:
	Delta( double iStartVal = 0, double iStopVal = 1, double iSteps = 1 )
	{
        m_iSteps = (double)(iSteps);
        m_iStartVal = (double)(iStartVal);
        m_iStopVal = (double)(iStopVal);
        m_iDelta = (double)((m_iStopVal-m_iStartVal)/m_iSteps);
	}

    int GetActValue( int iActStep ) const
	{
        return (int)(m_iStartVal + m_iDelta * iActStep);
	}

private:
	double		m_iSteps;
	double		m_iStartVal;
	double		m_iStopVal;
	double		m_iDelta;
};

// *******************************************************************
class OpItem_Base : public QObject /*, public DynOperation*/
{
	Q_OBJECT

public:
	OpItem_Base( DynText * pItem, const string & sTypeName="Base", int iDelayInMS=0 );
	virtual ~OpItem_Base();

    const string & GetTypeName() const;
        
    void SetItem( DynText * pItem );     
    DynText * GetItem() const;

    bool IsDone() const;

	int GetDelayInMS() const;
	void SetDelayInMS( int iDelayInMS );

	// methods to send data to objects from subclasses
	virtual int GetShowAtTimeInMS() const;
	virtual void SetShowAtTimeInMS( int iTimeInMS );
	virtual bool SetRelPos( double xRel, double yRel );
	virtual bool GetRelPos( double & xRel, double & yRel );

    virtual void Update();

	virtual void Run( int iStartFromPosInMS = 0 );
	virtual void Pause();
	virtual void Continue();
	virtual void Stop();
	virtual void Reset();

	virtual bool Write( ostream & aStream ) const;
	virtual bool Read( istream & aStream );

	static OpItem_Base * CreateFromType( const string & sTypeName );

public slots:
	virtual void sltOnTimer();

protected:
	string			m_sTypeName;
	bool			m_bDone;
	int				m_iDelayInMS;
	int				m_iElapsedTimeInMS;

	int				m_iTimeout;

	QTimer *		m_aTimer;
	QTime			m_aTime;

	DynText *		m_pItem;		// NOT an owner !
};

// *******************************************************************
class OpItem_Hide : public OpItem_Base
{
	Q_OBJECT

public:
	OpItem_Hide( DynText * pItem, int iTimeInMS = 1000, const string & sClassName = c_sHide );

	virtual void Reset();

	virtual bool Write( ostream & aStream ) const;
	virtual bool Read( istream & aStream );

public slots:
	virtual void sltOnTimer();
};

// *******************************************************************
class OpItem_Show : public OpItem_Hide
{
	Q_OBJECT

public:
	OpItem_Show( DynText * pItem, int iTimeInMS = 1000 );

	virtual int GetShowAtTimeInMS() const;
	virtual void SetShowAtTimeInMS( int iTimeInMS );

public slots:
	virtual void sltOnTimer();
};

// *******************************************************************
class OpItem_ChangeColor : public OpItem_Base
{
	Q_OBJECT

public:
	OpItem_ChangeColor( DynText * pItem, QColor aStartColor = QColor(0,0,0), QColor aStopColor = QColor(255,255,255), int iTimeInMS = 1000, const string & sClassName = c_sColor );

	virtual void Update();

	virtual void Reset();

	virtual bool Write( ostream & aStream ) const;
	virtual bool Read( istream & aStream );

public slots:
	virtual void sltOnTimer();

protected:
    int				m_iTimeInMS;
	QColor			m_aStartColor;
	QColor			m_aStopColor;

	Delta			m_aRedDelta;	// temp
	Delta			m_aGreenDelta;	// temp
	Delta			m_aBlueDelta;	// temp
	int				m_iCount;		// temp
	double			m_dSteps;		// temp
};

// *******************************************************************
class OpItem_FadeInOut : public OpItem_ChangeColor
{
	Q_OBJECT

public:
	OpItem_FadeInOut( bool bFadeIn, DynText * pItem, int iTimeInMS = 1000 );

	virtual void Update();

	virtual void Reset();

	virtual bool Write( ostream & aStream ) const;
	virtual bool Read( istream & aStream );

private:
	bool			m_bFadeIn;
};

// *******************************************************************
class OpItem_ChangeFontSize : public OpItem_Base
{
	Q_OBJECT

public:
	OpItem_ChangeFontSize( DynText * pItem, int iStartSize = 12, int iStopSize = 24, int iTimeInMS = 1000 );

	virtual void Update();

	virtual void Reset();

	virtual bool Write( ostream & aStream ) const;
	virtual bool Read( istream & aStream );

public slots:
	virtual void sltOnTimer();

private:
    int				m_iTimeInMS;
	int				m_iStartSize;
	int				m_iStopSize;

	int				m_iCount;		// temp
	double			m_dSteps;		// temp
	Delta			m_aDelta;		// temp
};

// *******************************************************************
class OpItem_SetRelPos : public OpItem_Base
{
	Q_OBJECT

public:
	OpItem_SetRelPos( DynText * pItem, double dRelX, double dRelY, const string & sClassName = c_sSetRel );

	virtual void Update();

	virtual void Reset();

	virtual bool Write( ostream & aStream ) const;
	virtual bool Read( istream & aStream );

	virtual bool SetRelPos( double xRel, double yRel );
	virtual bool GetRelPos( double & xRel, double & yRel );

private:
	double			m_dRelX;
	double			m_dRelY;
};

// *******************************************************************
class OpItem_MoveTo : public OpItem_Base
{
	Q_OBJECT

public:
	OpItem_MoveTo( DynText * pItem, int iFromPosX = 0, int iFromPosY = 0, int iToPosX = 0, int iToPosY = 0, int iTimeInMS = 1000, const string & sClassName = c_sMove );

	virtual void Update();

	virtual void Reset();

	virtual bool Write( ostream & aStream ) const;
	virtual bool Read( istream & aStream );

public slots:
	virtual void sltOnTimer();

protected:
	virtual void MoveMe( double x, double y );

    int				m_iTimeInMS;
    int				m_iToPosX;
    int				m_iToPosY;
    int				m_iStartPosX;
    int				m_iStartPosY;

	double			m_dDeltaX;		// temp
	double			m_dDeltaY;		// temp
	int				m_iCount;		// temp
	double			m_dSteps;		// temp
	bool			m_bIsDone;		// temp
};

// *******************************************************************
class OpItem_MoveToRel : public OpItem_MoveTo
{
	Q_OBJECT

public:
	OpItem_MoveToRel( DynText * pItem, int iFromPosX = 0, int iFromPosY = 0, int iToPosX = 0, int iToPosY = 0, int iTimeInMS = 1000, const string & sClassName = c_sMoveRel );

protected:
	virtual void MoveMe( double x, double y );
};

// *******************************************************************
class DynText : public QGraphicsSimpleTextItem, public GenericDataInterface
{
public:
	typedef minHandle<OpItem_Base>		OperationT;
	typedef vector< OperationT >		OperationContainer;

    DynText( const string & sText = "", QGraphicsScene * pOwner = 0 );
	virtual ~DynText();

	XmlTree	GetXMLTree() const;

    void SetCanvas( QGraphicsScene * pCanvas );

	// to update internal state of this object
	void Sync();

// only for edit modus needed...
    void Deselect();
	void SetSelected( bool bSelected );
    bool IsSelected() const;
    bool IsPosInElement( int x, int y ) const;
	void InitMouseMove( int x, int y );
	void DoneMouseMove();
	bool IsInMouseMove() const;
	void DoMouseMove( int x, int y );
// done 
	void move( double x, double y );

    bool IsAttachedToSlide() const;
    string GetAttachedSlideUUID() const;
    void SetAttachedSlideUUID( const string & sUUID, double dRelativeTimeInMS );
    void UpdateData( double dStartTimeOfAttachedDiaInMS );      // needs to be called after moving of image

	bool Write( ostream & aStream ) const;
	bool Read( istream & aStream );

    void AddOperation( OperationT hOp );
    const OperationContainer & GetOperations() const;
        
    void Show();
    void Hide();

    void Run( int iStartFromPosInMS = 0 );
	void Pause();
	void Continue();
	void Stop();
	void Reset();

	bool IsDone() const;

    double GetStartTimeInMS() const;        // see GetDefaultData()
	string GetString() const;

	void CreateDefaultOperations( double dStartTimeInMS, double dShowTimeInMS );
	void ChangeDefaultData( double dStartTimeInMS, double dShowTimeInMS );
	void GetDefaultData( double & dStartTimeInMS, double & dShowTimeInMS ) const;
	void Delta( double dDeltaTimeInMS );
	bool SetRelativePos( double xRel, double yRel );
	bool GetRelativePos( double & xRel, double & yRel ) const;

	// duplicate all attributes (color, font, size) from the other element
	void SetAttributesFrom( DynText * pOtherItem );

	void PaintForTime( QPainter & aPainter, double dTimeMS ) const;
	bool IsNextChanging( double dTimeMS, double dDeltaMS ) const;

    // *** implement GenericDataInterface ***
    virtual string		GetName() const;

    virtual int			GetDataCount() const;
    virtual string		GetDataName( int iIndex ) const;
    virtual DataType	GetDataType( int iIndex ) const;
    virtual string		GetDataValue( int iIndex ) const;
    virtual bool		SetDataValue( int iIndex, const string & sValue );
    virtual bool        IsDataEditable( int /*iIndex*/ ) const;

    bool operator<( const DynText & right );

private:
	bool WriteOpContainer( ostream & aStream ) const;
	bool ReadOpContainer( istream & aStream );

    string              m_sUUIDOfAttachedImage;
    double              m_dRelativeTimeInMS;        // relative time from start of dissolve of attached image
                                                // needed to update real time after move of attached image
	bool				m_bIsSelected;
	int					m_xOld;
	int					m_yOld;
	QFont				m_aInitFont;
	QColor				m_aInitColor;
    QGraphicsRectItem *	m_pSelectedHelper;
	OperationContainer	m_aOpContainer;

    static int ACT_FILE_VERSION;
};

//********************************************************************

class DynTextContainer : public IOContainer< DynText >, public GenericCommentContainer
{
public:
    DynTextContainer( IDiaOutputWindowInternal * pOutputWindowProxy );

	bool ImportFile( const string & sFileName );

	virtual bool Read( istream & aStream );
	virtual bool Write( ostream & aStream ) const;

	void RunSelected( int iStartFromPosInMS = 0 );
	void Run( int iStartFromPosInMS = 0 );
	void Pause();
	void Continue();
	void Stop();
	void Reset();

    void AddDefaultDynText( const string & sText, double dStartTimeInMS, double dShowTimeInMS = 5000, const string & sUUID = "", double dDiaStartTimeInMS = 0.0 );

	void SetAttributesForAllItems( minHandle<DynText> hItem );

	void PaintElementsForTime( QPainter & aPainter, double dTimeMS ) const;
	bool IsNextElementChanging( double dTimeMS, double dDeltaMS ) const;

    void UpdateShowTimeForDia( const string & sUUID, double dDeltaMS );

    bool HasTextFor( const string & sUUID, const string & sText ) const;

    // *** implement GenericCommentContainer **
    virtual int						size() const;
    virtual void					clear();
    virtual GenericDataInterface *	at( int iIndex );
    virtual GenericDataInterface *	push_back_new_item();
    virtual void                    SortData();

private:
	void Update();
	void UpdateInfos();

    bool                        m_bIsPause;
	IDiaOutputWindowInternal *	m_pOutputWindowProxy;	// not an owner !
	//QCanvas	*	m_pCanvas;		// not an owner !
};

#endif
