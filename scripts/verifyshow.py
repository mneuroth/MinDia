# Verify the show-time for all dias to fit the minimum show-time
import mindiapy

aObj = mindiapy.GetDiaPresentation()

dMinShowTime = 4.5
bAllOk = 1

for i in range( 0, aObj.GetDiaCount() ):
    aItem = aObj.GetDia( i )
    if aItem.GetShowTime() < dMinShowTime:
        bAllOk = 0
        s = "dia %d id=%s show-time=%f s < %f s" % (i, aItem.GetId(), aItem.GetShowTime(), dMinShowTime)
        sMsg = "Verify Error: " + s + "\n\nSet to the minimum show-time ?"
        iRet = mindiapy.DoMessageBox( sMsg, "Question" )
        if iRet == 0:
            aItem.SetShowTime( dMinShowTime )

if bAllOk:
    iRet = mindiapy.DoMessageBox( "All dia show-times are ok !", "Question" )
