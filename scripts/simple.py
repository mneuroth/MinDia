# Simple python test program (print message in output-window)
import mindiapy
mindiapy.PrintLn( "hallo Pyhton !" )
aObj = mindiapy.GetDiaPresentation()
s = "The count of slides in presentation is %d" % aObj.GetDiaCount()
mindiapy.PrintLn( s )

#
# $Source: /Users/min/Documents/home/cvsroot/mindia/scripts/simple.py,v $
#
# $Revision: 1.2 $
#
# $Log: not supported by cvs2svn $
#