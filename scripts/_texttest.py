# test for text output on the play info dialog
import mindiapy
import time
aPres = mindiapy.GetDiaPresentation()
aWin = aPres.GetOutputWindow()
x = aWin.GetWidth()
y = aWin.GetHeight()
aWin.SetFont( "Times" )
aWin.SetColor( 250, 255, 0 )
aWin.SetFontSize( 28, 0, 0 )
i = aWin.SetTextXY( 200, 120, "Hallo Welt" )
s = "%d %d i=%d" % (x,y,i)
mindiapy.PrintLn( "ok s="+s )

for ii in range( 0, 25 ):
    aWin.MoveText( i, ii, ii )
    time.sleep( 0.1 )
