# show all loaded modules (in output window)
import sys
import mindiapy
aList = sys.path
aList = sys.modules
for s in aList:
    mindiapy.PrintLn( "s=%s" % s )
