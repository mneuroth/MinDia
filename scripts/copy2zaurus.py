# Prepare presentation for Zaurus

import mindiapy
import os.path
import shutil

# where to copy on the PC
sTargetOffset = "e:\\test\\"    # "g:\\"
#sTargetOffset = "g:\\"
# location of the files on the Zaurus
sLocalOffset  = "/mnt/cf/" 

aPres = mindiapy.GetDiaPresentation()

# update the path to the sound files
# copy the sound files
iCount = aPres.GetSoundFileCount()
for i in range(iCount):
    sSource = aPres.GetSoundFileNameAt(i)
    (sPath,sName) = os.path.split(sSource)
    sTarget = sTargetOffset+sName
    sLocalTarget = sLocalOffset+sName
    shutil.copy(sSource,sTarget)
    aPres.SetSoundFileNameAt(i,sLocalTarget)
    mindiapy.PrintLn( "copied "+sSource+" --> "+sTarget+" as "+sLocalTarget )

# and than save the presenation 
sFullName = aPres.GetDocName()
(sPath,sName) = os.path.split(aPres.GetDocName())
aPres.SavePresentationAs(sTargetOffset+sName)
mindiapy.PrintLn( "saved presentation "+sPath+sName+" as "+aPres.GetDocName() )

aPres.LoadPresentation(sFullName)
mindiapy.PrintLn( "loading original presentation "+sFullName+" again." )


