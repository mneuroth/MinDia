# PyQt test script
# WARNING: do not create a application object in scripts !
#          the application object will be created in mindia !

import sys
from qt import *


class MyWidget(QWidget):
	def __init__(self,parent=None,name=None):
		QWidget.__init__(self,parent,name)

		self.setMinimumSize(200,120)
		self.setMaximumSize(200,120)

		quit = QPushButton("Quit",self,"quit")
		quit.setGeometry(62,40,75,30)
		quit.setFont(QFont("Times",18,QFont.Bold))

		self.connect(quit,SIGNAL("clicked()"),qApp,SLOT("quit()"))


#a = QApplication(sys.argv)

w = MyWidget()
w.setGeometry(100,100,200,120)
#a.setMainWidget(w)
w.show()
#a.exec_loop()
