# make a cd distribution of the actual slide show 

import mindiapy
import os
import shutil

aPres = mindiapy.GetDiaPresentation()
aWin = aPres.GetOutputWindow()

def CopyAllImages( sNewImagePath ):
    sNewImagePath = os.path.normpath( sNewImagePath )
    if not os.access( sNewImagePath, os.F_OK ):
        os.mkdir( sNewImagePath )
    for i in range( 0, aPres.GetDiaCount() ):
        aDia = aPres.GetDia( i )
        sName = os.path.normpath( aDia.GetImageFile() )
        (h,t) = os.path.split( sName )
        sNewName = sNewImagePath + os.sep + t
        s = "%d %s %s %s %s %s" % ( i, sName, h, t, sNewName, os.getcwd() )
        shutil.copyfile( sName, sNewName )
        aDia.SetImageFile( sNewName )
        mindiapy.PrintLn( s )

def CopyAllSounds( sNewSoundPath ):
    sNewSoundPath = os.path.normpath( sNewSoundPath )
    if not os.access( sNewSoundPath, os.F_OK ):
        os.mkdir( sNewSoundPath )
    for i in range( 0, aPres.GetSoundFileCount() ):
        sSoundFile = os.path.normpath( aPres.GetSoundFileNameAt( i ) )
        (h,t) = os.path.split( sSoundFile )
        sNewSoundFile = sNewSoundPath + os.sep + t
        s = "%d %s %s" % (i, sSoundFile, sNewSoundFile)
        shutil.copyfile( sSoundFile, sNewSoundFile )
        aPres.SetSoundFileNameAt( i, sNewSoundFile )
        mindiapy.PrintLn( s )

#****************************************
def test():
    aWin.Clear()

#****************************************
#****************************************
#****************************************
if __name__ == '__main__':
   test()

CopyAllImages( os.curdir + os.sep + "tmp" )
CopyAllSounds( os.curdir + os.sep + "tmp" )
