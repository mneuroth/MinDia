# Clear all texts visible in the play info dialog

import mindiapy

aPres = mindiapy.GetDiaPresentation()
aWin = aPres.GetOutputWindow()

aWin.Clear()
