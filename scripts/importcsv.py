# Import presentation data from a csv-file (import.csv)

import mindiapy

aObj = mindiapy.GetDiaPresentation()

sFileName = "import.csv"
#mindiapy.PrintLn( sFileName )

try:
    aFile = open( sFileName, "r" )
except IOError:
    mindiapy.PrintLn( "Error: File %s not found !" % sFileName )
else:
    # read all lines of the file
    aLines = aFile.readlines()
    
    # fill data in presentation
    for s in aLines:
        aItem = aObj.AddNewDia()
        #mindiapy.PrintLn( s )
        
        # process the single line 
        aSingleLine = s.split( "," )

        if len( aSingleLine ) > 1:
            aItem.SetId( aSingleLine[1] )
        if len( aSingleLine ) > 2:
            aItem.SetDissolveTime( float( aSingleLine[2] ) )
        if len( aSingleLine ) > 3:
            aItem.SetShowTime( float( aSingleLine[3] ) )
        if len( aSingleLine ) > 4:
            aItem.SetComment( aSingleLine[4] )
        if len( aSingleLine ) > 5:
            aItem.SetImageFile( aSingleLine[5] )
      
    aFile.close()
