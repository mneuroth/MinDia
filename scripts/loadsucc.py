# Load a presentation successor file
import mindiapy
aObj = mindiapy.GetDiaPresentation()

iRet = mindiapy.DoMessageBox( "Really load a successor file ?", "Question" )
if iRet==0:
	iRet = aObj.LoadPresentation( "successor.dia", 1 )
else:
	mindiapy.DoMessageBox( "Successor file NOT loaded !", "Warning" )

s = "ret=%d" % iRet
mindiapy.PrintLn( s )

