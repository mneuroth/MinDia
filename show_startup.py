#!/usr/bin/env python

# A simple debugger application.

import sys, string, os
from qt import *
from qtcanvas import *

# *************************************************************************
class MoveText(QObject):
    
    def __init__(self,aCanvasText,iToPosX,iToPosY,iSteps):
        self.iTimeout = 50
        self.iSteps = iSteps
        self.iToPosX = iToPosX
        self.iToPosY = iToPosY
        self.aCanvasText = aCanvasText
        self.iStartPosX = aCanvasText.x()
        self.iStartPosY = aCanvasText.y()
        self.iDeltaX = (self.iToPosX-self.iStartPosX)/self.iSteps
        self.iDeltaY = (self.iToPosY-self.iStartPosY)/self.iSteps
        self.iCount = 0
        #self.aTimer = QTimer()
        #self.connect(self.aTimer,SIGNAL("timeout()"),self.OnTimer)
        #self.connect(self.aTimer,SIGNAL("timeout()"),self.OnTimer)
        #self.connect(self.aTimer,SIGNAL("clicked()"),qApp,SLOT("quit()"))
        #self.aTimer.start( self.iTimeout )

    def SetTimer(self,aTimer):
        self.aTimer = aTimer

    def OnTimer(self):
        self.aTimer.stop()
        print "timer ! %d %d" % (self.iCount,self.iSteps)
        if self.iCount < self.iSteps:            
            self.aCanvasText.move( self.iStartPosX+self.iCount*self.iDeltaX, self.iStartPosY+self.iCount*self.iDeltaY )
            self.aCanvasText.show()
            self.iCount = self.iCount + 1
            self.aCanvasText.canvas().update()
            self.aTimer.start( self.iTimeout )


# *************************************************************************
class StartupWindow(QDialog):

    def __init__(self):
        QDialog.__init__( self,None,'Simple Dbg',0,QDialog.WStyle_NoBorder )

        self.iOffsetY = 20;
        self.iOffsetX = 20;

        self.setCaption( 'Show Startup !' )

        aForm1Layout = QVBoxLayout(self)
        aForm1Layout.setSpacing(0)
        aForm1Layout.setMargin(0)

        self.aCanvas = QCanvas(100,100)
        self.aCanvas.setBackgroundColor( QColor( 0, 0, 255 ) )
        self.aCanvasView = QCanvasView(self.aCanvas,self)

        self.ShowInitScreen()

        aForm1Layout.addWidget(self.aCanvasView)

        self.resize( 500, 400 )

        self.aTextShifter = MoveText( self.aText1,500, 500, 50 )
        self.aTimer = QTimer()
        self.connect(self.aTimer,SIGNAL("timeout()"),self.aTextShifter.OnTimer)
        self.aTextShifter.SetTimer( self.aTimer )
        self.aTimer.start( 100 )


    def ShowInitScreen(self):
        aColor = QColor( 255,255,255 )
        aFont = QFont( "Helvetica", 26 );
        self.aText1 = QCanvasText( "Hallo", self.aCanvas )
        self.aText1.setVisible( 1 )
        self.aText1.setColor( aColor )
        self.aText1.setFont( aFont )
        self.aText1.show()
        #self.aLine = QCanvasLine( self.aCanvas )
        #self.aLine.setPoints( 0, 0, 500, 500 )
        #self.aLine.show()
        self.aButton = QPushButton( self.aCanvasView )
        self.aButton.setText( "Abbrechen" )
        self.aButton.show()
        self.connect(self.aButton,SIGNAL("clicked()"),qApp,SLOT("quit()"))
        self.aStartButton = QPushButton( self.aCanvasView )
        self.aStartButton.setText( "Start" )
        self.aStartButton.show()
        self.connect(self.aStartButton,SIGNAL("clicked()"),self.StartShow)

    def resizeEvent( self, aEvent ):
        self.aButton.move( 30, self.height()-self.aButton.height()-self.iOffsetX )
        self.aStartButton.move( 30+self.aButton.width()+10, self.height()-self.aButton.height()-self.iOffsetX )
        self.aCanvas.resize( self.width()-self.iOffsetX, self.height()-self.iOffsetY )
        #print "***-->width=%d height=%d" % (self.aCanvas.width(),self.aCanvas.height())        

#    def showMaximized( self ):
#        QDialog.showMaximized( self )
#        self.aCanvas.resize( self.width()-self.iOffsetX, self.height()-self.iOffsetY )
#        print "###-->width=%d height=%d" % (self.width(),self.height())

#    def show( self ):
#        QDialog.show( self )
#        print "+++-->width=%d height=%d" % (self.width(),self.height())

    def done(self,iResult):
        print "done 1"
        qApp.quit()

    def accept(self):
        print "done 2"
        qApp.quit()

    def reject(self):
        print "done 3"
        qApp.quit()

    def StartShow(self):
        print "start show now"
        os.system( "notepad" )

# *************************************************************************

a = QApplication(sys.argv)

mw = StartupWindow()
mw.showMaximized()
#mw.ShowInitScreen()

#print "width=%d" % (mw.aCanvas.width())
print "test"

iRet = a.exec_loop()

print "width=%d" % (mw.aCanvas.width())

print "iRet=%d" % (iRet)
