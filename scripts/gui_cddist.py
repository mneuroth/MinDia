# Graphical Frontend for the Create CD Distribution script
# WARNING: do not create a application object in scripts !
#          the application object will be created in mindia !

import sys
import os
from qt import *

import CreateCDDist

class CreateCDDistImpl( CreateCDDist.CreateCDDist ):
    
    def __init__(self):
        CreateCDDist.CreateCDDist.__init__( self,None,'SimpleDlg',True,QDialog.WStyle_NoBorder|QDialog.WStyle_Minimize|QDialog.WStyle_Maximize )
        self.sTargetDir = "cd_dist"
        self.sDefImageDir = os.sep + "images"
        self.sDefSoundDir = os.sep + "sounds"

    def accept(self):
        self.setResult( 1 )
        self.hide()

    def reject(self):
        self.setResult( 0 )
        self.hide()

    def sltSelectTargetDir(self):
        sTargetDir = QFileDialog.getExistingDirectory(None,None,"Target directory").__str__()
        if sTargetDir is not None:
            self.sTargetDir = os.path.normpath( sTargetDir )
            self.pTargetDir.setText( self.sTargetDir )		 
            self.pImageDir.setText( os.path.normpath( self.sTargetDir + self.sDefImageDir ) )		 
            self.pSoundDir.setText( os.path.normpath( self.sTargetDir + self.sDefSoundDir ) )		 
        
    def sltSelectImageDir(self):
        sDir = QFileDialog.getExistingDirectory(None,None,"Image directory").__str__()
        if sDir is not None:
            self.pImageDir.setText( os.path.normpath( sDir ) )

    def sltSelectSoundDir(self):
        sDir = QFileDialog.getExistingDirectory(None,None,"Sound directory").__str__()
        if sDir is not None:
            self.pSoundDir.setText( os.path.normpath( sDir ) )


#a = QApplication(sys.argv)

w = CreateCDDistImpl()
ret = w.exec_loop()
s = "Return=%d %s" % (ret,w.pTargetDir.text())
print( s )

#a.exec_loop()

