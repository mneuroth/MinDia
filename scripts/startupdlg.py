# startup dialog with PyQt

import sys

from qt import *

class StartupDlg(QDialog):

    def __init__(self,parent = None,name = None,modal = 0,fl = 0):
        QDialog.__init__(self,parent,name,modal,fl)

        if name == None:
            self.setName('AboutExtDlg')

        self.resize(476,372)
        #self.showMaximized()


#a = QApplication( sys.argv )

w = StartupDlg(None,"test",1)
#a.setMainWidget( w )
#w.show()
w.exec_loop()
#a.exec_loop()
del w
