# Modul with functions for text operations (fading, centering)
# on the play-info dialog.
# Some test functions.

import mindiapy

aPres = mindiapy.GetDiaPresentation()
aWin = aPres.GetOutputWindow()

#****************************************
def CenterText( iId, bCenterX = 1 ):
    "center a given text in x or y direction"
    if bCenterX == 1:
        aWin.MoveText( iId, aWin.GetDrawWidth() / 2 - (aWin.GetTextWidth(iId) / 2), aWin.GetTextY(iId) )
    else:
        aWin.MoveText( iId, aWin.GetTextX(iId), aWin.GetDrawHeight() / 2 - (aWin.GetTextHeight(iId) / 2) )

#****************************************
def FadeTextOrg( iId, iFadeTime, bIn, aColor ):
    "fade in or out a given text"
    if iFadeTime<>0:
        iStep = 10
	aHsv = mindiapy.RgbToHsv( aColor );
	s = "%d %d %d" % (aHsv.iHue, aHsv.iSaturation, aHsv.iValue)
        mindiapy.PrintLn( s )
        for i in range(0,iFadeTime,iStep):
            dDelta = 255.0 / (iFadeTime / iStep)
            iGray = ((i/iStep)*dDelta)/255.0
            if bIn == 1:
                iGray = (1.0 - iGray)
                aWin.SetTextColor( iId, iGray*aColor.iRed, iGray*aColor.iGreen, iGray*aColor.iBlue )
                #aWin.SetTextColor( iId, iGray*aColor.iRed, iGray*aColor.iGreen, iGray*aColor.iBlue )
            else:
                aWin.SetTextColor( iId, 255-iGray*aColor.iRed, 255-iGray*aColor.iGreen, 255-iGray*aColor.iBlue )
            #s = "%d %f %f %f" % (i, iFadeTime, dDelta, iGray)
            #s = "%f %d %d %d" % (iGray, iGray*aColor.iRed, iGray*aColor.iGreen, iGray*aColor.iBlue)
            #mindiapy.PrintLn( s )
            mindiapy.SleepMS( iStep )
    return iId

#****************************************
def FadeText( iId, iFadeTime, bIn, aColor ):
    "fade in or out a given text"
    if iFadeTime<>0:
        iStep = 10
	aHsv = mindiapy.RgbToHsv( aColor );
	s = "%d %d %d" % (aHsv.iHue, aHsv.iSaturation, aHsv.iValue)
        mindiapy.PrintLn( s )
        for i in range(0,iFadeTime,iStep):
            dDelta = 255.0 / (iFadeTime / iStep)
            iGray = ((i/iStep)*dDelta)/255.0
            if bIn == 0:
                iGray = (1.0 - iGray)
                aHsv.iSaturation = iGray*255
		aColor = mindiapy.HsvToRgb( aHsv )
                aWin.SetTextColor( iId, iGray*aColor.iRed, iGray*aColor.iGreen, iGray*aColor.iBlue )
                #s = "%f %d %d %d" % (iGray, aHsv.iHue, aHsv.iSaturation, aHsv.iValue)
                #mindiapy.PrintLn( s )
            else:
                aHsv.iSaturation = iGray*255
		aColor = mindiapy.HsvToRgb( aHsv )
                aWin.SetTextColor( iId, iGray*aColor.iRed, iGray*aColor.iGreen, iGray*aColor.iBlue )
            #s = "%d %f %f %f" % (i, iFadeTime, dDelta, iGray)
            #s = "%f %d %d %d" % (iGray, iGray*aColor.iRed, iGray*aColor.iGreen, iGray*aColor.iBlue)
            #mindiapy.PrintLn( s )
            mindiapy.SleepMS( iStep )
    return iId

#****************************************
def FadeInOutText( iId, iFadeInTime, iShowTime, iFadeOutTime ):
    aColor = aWin.GetTextColor( iId )
    FadeText( iId, iFadeInTime, 1, aColor )
    mindiapy.SleepMS( iShowTime )
    FadeText( iId, iFadeOutTime, 0, aColor )
    return iId

#****************************************
def testx():
    aWin.Clear()

def test():
    aWin.Clear()
    aWin.Show()
    aWin.SetFontSize( 30 )
    aWin.SetColor( 0, 255, 0 )
    iId = aWin.SetTextXY( 10, 10, "Neuseeland" )
    CenterText( iId, 1 )
    CenterText( iId, 0 )
    FadeInOutText( iId, 2000, 2000, 2000 )
    #s = "%d" % aWin.GetTextCount()
    #mindiapy.PrintLn( s )

#****************************************
#****************************************
#****************************************
if __name__ == '__main__':
   test()

