# wxWindows test script

import sys

from wxPython.wx import *

class wxTest( wxApp ):

    def __init__( self ):
        wxApp.__init__( self, 0 )

    def OnInit( self ):
        self.mainWindow = wxFrame( None, -1, "wxTest" )
        self.mainWindow.Show( 1 )
        self.SetTopWindow( self.mainWindow )
        return 1

    def run( self ):
        self.MainLoop()


if __name__ == "__main__":
    applikation = wxTest()
    applikation.run()
