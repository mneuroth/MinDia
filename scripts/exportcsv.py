# Export the presentation as a csv-file
# use act. file name and add .csv

import mindiapy

aObj = mindiapy.GetDiaPresentation()

# create the file name for the csv-output
sFileName = aObj.GetDocName()
sFileName = sFileName + ".csv"
#mindiapy.PrintLn( sFileName )

aFile = open( sFileName, "w" )

# iterate over all elements of the presentation 
for i in range( 0, aObj.GetDiaCount() ):
    aItem = aObj.GetDia( i )
    s = "%d,\"%s\",%f,%f,\"%s\",\"%s\"\n" % (i, aItem.GetId(), aItem.GetDissolveTime(), aItem.GetShowTime(), aItem.GetComment(), aItem.GetImageFile())
    #mindiapy.PrintLn( s )
    aFile.write( s )

aFile.close()
