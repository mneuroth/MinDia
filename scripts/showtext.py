# Show a text on the play info dialog

import mindiapy

aPres = mindiapy.GetDiaPresentation()
aWin = aPres.GetOutputWindow()

aWin.Show()
aWin.Clear()
#aWin.SetPos( 20, 20 )
#aWin.SetSize( 600, 300 )
aWin.SetFontSize( 40 )
aWin.SetColor( 255, 0, 0 )
aWin.SetTextXY( 90, 10, "New Zealand !" )

