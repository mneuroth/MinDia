# Show the play-info dialog in different modi

import mindiapy

aObj = mindiapy.GetDiaPresentation()
aWin = aObj.GetOutputWindow()

aWin.Show()
mindiapy.SleepMS( 2000 )
aWin.Maximize()
mindiapy.SleepMS( 2000 )
aWin.Restore()
mindiapy.SleepMS( 2000 )
aWin.Hide()


