# Form implementation generated from reading ui file 'CreateCDDist.ui'
#
# Created: Sat Jun 21 18:17:19 2003
#      by: The Python User Interface Compiler (pyuic)
#
# WARNING! All changes made in this file will be lost!


from qt import *


class CreateCDDist(QDialog):
    def __init__(self,parent = None,name = None,modal = 0,fl = 0):
        QDialog.__init__(self,parent,name,modal,fl)

        if name == None:
            self.setName('CreateCDDist')

        self.resize(449,175)
        self.setProperty('caption',QVariant(self.tr("Create CD distribution")))
        self.setProperty('sizeGripEnabled',QVariant(1,0))
        CreateCDDistLayout = QGridLayout(self)
        CreateCDDistLayout.setSpacing(6)
        CreateCDDistLayout.setMargin(11)

        self.buttonOk = QPushButton(self,'buttonOk')
        self.buttonOk.setProperty('text',QVariant(self.tr("&OK")))
        self.buttonOk.setProperty('autoDefault',QVariant(1,0))
        self.buttonOk.setProperty('default',QVariant(1,0))

        CreateCDDistLayout.addWidget(self.buttonOk,3,0)

        self.buttonCancel = QPushButton(self,'buttonCancel')
        self.buttonCancel.setProperty('text',QVariant(self.tr("&Cancel")))
        self.buttonCancel.setProperty('autoDefault',QVariant(1,0))

        CreateCDDistLayout.addWidget(self.buttonCancel,3,1)

        self.buttonHelp = QPushButton(self,'buttonHelp')
        self.buttonHelp.setProperty('text',QVariant(self.tr("&Help")))
        self.buttonHelp.setProperty('autoDefault',QVariant(1,0))

        CreateCDDistLayout.addMultiCellWidget(self.buttonHelp,3,3,3,4)
        spacer = QSpacerItem(20,20,QSizePolicy.Expanding,QSizePolicy.Minimum)
        CreateCDDistLayout.addItem(spacer,3,2)

        self.pTargetDir = QLineEdit(self,'pTargetDir')

        CreateCDDistLayout.addMultiCellWidget(self.pTargetDir,0,0,1,3)

        self.pSelectTargetDir = QPushButton(self,'pSelectTargetDir')
        self.pSelectTargetDir.setProperty('text',QVariant(self.tr("&...")))

        CreateCDDistLayout.addWidget(self.pSelectTargetDir,0,4)

        self.pLabelTargetDir = QLabel(self,'pLabelTargetDir')
        self.pLabelTargetDir.setProperty('text',QVariant(self.tr("Target directory:")))

        CreateCDDistLayout.addWidget(self.pLabelTargetDir,0,0)

        self.pLabelImageDir = QLabel(self,'pLabelImageDir')
        self.pLabelImageDir.setProperty('text',QVariant(self.tr("Image directory:")))

        CreateCDDistLayout.addWidget(self.pLabelImageDir,1,0)

        self.pImageDir = QLineEdit(self,'pImageDir')

        CreateCDDistLayout.addMultiCellWidget(self.pImageDir,1,1,1,3)

        self.pSelectImageDir = QPushButton(self,'pSelectImageDir')
        self.pSelectImageDir.setProperty('text',QVariant(self.tr("...")))

        CreateCDDistLayout.addWidget(self.pSelectImageDir,1,4)

        self.pLabelSoundDir = QLabel(self,'pLabelSoundDir')
        self.pLabelSoundDir.setProperty('text',QVariant(self.tr("Sound directory:")))

        CreateCDDistLayout.addWidget(self.pLabelSoundDir,2,0)

        self.pSoundDir = QLineEdit(self,'pSoundDir')

        CreateCDDistLayout.addMultiCellWidget(self.pSoundDir,2,2,1,3)

        self.pSelectSoundDir = QPushButton(self,'pSelectSoundDir')
        self.pSelectSoundDir.setProperty('text',QVariant(self.tr("...")))

        CreateCDDistLayout.addWidget(self.pSelectSoundDir,2,4)

        self.connect(self.buttonOk,SIGNAL('clicked()'),self,SLOT('accept()'))
        self.connect(self.buttonCancel,SIGNAL('clicked()'),self,SLOT('reject()'))
        self.connect(self.pSelectTargetDir,SIGNAL('clicked()'),self.sltSelectTargetDir)
        self.connect(self.pSelectImageDir,SIGNAL('clicked()'),self.sltSelectImageDir)
        self.connect(self.pSelectSoundDir,SIGNAL('clicked()'),self.sltSelectSoundDir)

        self.setTabOrder(self.pTargetDir,self.pSelectTargetDir)
        self.setTabOrder(self.pSelectTargetDir,self.pImageDir)
        self.setTabOrder(self.pImageDir,self.pSelectImageDir)
        self.setTabOrder(self.pSelectImageDir,self.pSoundDir)
        self.setTabOrder(self.pSoundDir,self.pSelectSoundDir)
        self.setTabOrder(self.pSelectSoundDir,self.buttonOk)
        self.setTabOrder(self.buttonOk,self.buttonCancel)
        self.setTabOrder(self.buttonCancel,self.buttonHelp)

    def sltSelectSoundDir(self):
        print 'CreateCDDist.sltSelectSoundDir(): not implemented yet'

    def sltSelectImageDir(self):
        print 'CreateCDDist.sltSelectImageDir(): not implemented yet'

    def sltSelectTargetDir(self):
        print 'CreateCDDist.sltSelectTargetDir(): not implemented yet'
