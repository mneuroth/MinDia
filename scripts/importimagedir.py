# Import image directory as a slide show

# Bem.: anscheinend gibt es irgend welche Probleme mit Funktionen !

import mindiapy
import os

aObj = mindiapy.GetDiaPresentation()

c_sSeparator = "\\"
#sInputDir = "bindist\\bmp"
sInputDir = "bmp"
sFilterExt = ".bmp"

#li = filter_files( os.listdir( sInputDir ), ".bmp" )
li = os.listdir( sInputDir )
liout = []
for sFile in li:
    sName,sExt = os.path.splitext( sFile )
    if sExt.lower() == sFilterExt.lower():
        liout.append( sFile )
li = liout

iCount = 1
for sFileName in li:
    iCount = iCount + 1
    mindiapy.PrintLn( sFileName )
    aItem = aObj.AddNewDia()
    sName, sExt = os.path.splitext( sFileName )
    sId = "%d" % ( iCount )
    aItem.SetId( sId )
    aItem.SetComment( sName )
    aItem.SetImageFile( sInputDir + c_sSeparator + sFileName )
    aItem.SetDissolveTime( float( 4 ) )
    aItem.SetShowTime( float( 5 ) )
    #add_dia_for_image( sName, iCount )

# **************************************************

def filter_files( li, sFilterExt = ".bmp" ):
    liout = []
    for sFile in li:
        sName,sExt = os.path.splitext( sFile )
        if sExt.lower() == sFilterExt.lower():
            liout.append( sFile )
    return liout


def add_dia_for_image( sFileName, iCount = 0 ):
    aItem = aObj.AddNewDia()
    sName, sExt = os.path.splitext( sFileName )
    sId = "%d" % ( iCount )
    aItem.SetId( sId )
    aItem.SetComment( sName )
    aItem.SetImageFile( sInputDir + c_sSeparator + sFileName )
    aItem.SetDissolveTime( float( 3.21 ) )
    mindiapy.PrintLn( sFileName )

