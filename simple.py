# Simple python test program (print message in output-window)
import mindiapy
mindiapy.PrintLn( "hallo Pyhton !" )
aObj = mindiapy.GetDiaPresentation()
s = "The count of slides in presentation is %d" % aObj.GetDiaCount()
mindiapy.PrintLn( s )
