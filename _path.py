# show all path for python
import sys
import mindiapy
aList = sys.path
for s in aList:
    mindiapy.PrintLn( "s=%s" % s )
