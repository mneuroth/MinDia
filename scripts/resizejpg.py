# Convert jpeg images to bmp images
# usage: python jpg2bmp.py jpg bmp 400 300

# set PYTHONPATH=c:\neuroth\python\Imaging-1.1.3\PIL

import Image
import ImageDraw
import os
import sys


#sName = "C:\neuroth\python\Python-2.2.2\PCbuild>dir c:\neuroth\python\Imaging-1.1.3\images\lena.ppm"

def print_image_infos( im ):
    print "image infos:"
    print im.format 
    print im.size 
    print im.mode

def test_image():
    im = Image.new( "RGB", (200, 100) )
    draw = ImageDraw.Draw( im )
    #draw.setfont()
    draw.line( (0,0)+im.size,fill=128 )
    #draw.text( (20,50), "Michael & Madeleen", 64 )
    del draw
    im.save( "x.bmp" )
    return im

#print ">",os.path.abspath( "jpg2bmp.py" ),"<"
#print ">",os.path.basename( "jpg2bmp.py" ),"<"
#print ">",os.path.dirname( ".\jpg2bmp.py" ),"<"
#print ">",os.path.split( ".\jpg2bmp.py" ),"<"
#print ">",os.path.splitext( "jpg2bmp.py" ),"<"

#**************************************************

c_sSeparator = "\\"

# default Werte 
sInputDir = "."
sOutputDir = "."
iNewWidth = 0
iNewHeight = 0

argc = len( sys.argv )

# Argumente verarbeiten
if argc > 1 :
    sInputDir = sys.argv[1]

if argc > 2 :
    sOutputDir = sys.argv[2]

if argc > 3 :
    iNewWidth = int( sys.argv[3] )

if argc > 4 :
    iNewHeight = int( sys.argv[4] )

print "%d %d" % (iNewWidth, iNewHeight)

def filter_jpeg_files( li ):
    liout = []
    for sFile in li:
        sName,sExt = os.path.splitext( sFile )
        if sExt.lower() == ".jpg":
            liout.append( sFile )
    return liout
		
def process_all_files( sInputDir, sOutputDir ):
    li = os.listdir( sInputDir )
    liout = filter_jpeg_files( li )
    for sName in liout:
        process_file( sName, sInputDir, sOutputDir )

def process_file( sFileName, sInputDir, sOutputDir ):
    sFullFileName = sInputDir + c_sSeparator + sFileName
    sName, sExtension = os.path.splitext( sFileName )
    sFullBmpFileName = sOutputDir + c_sSeparator + sName + ".jpg"
    print "converting " + sFullFileName + " to " + sFullBmpFileName
    convert_jpeg2bmp( sFullFileName, sFullBmpFileName, iNewWidth, iNewHeight )
			    
def convert_jpeg2bmp( sNameIn, sNameOut, iWidth = 800, iHeight = 600 ):
    # convert a jpeg image into a bmp image
    im = Image.open( sNameIn )
    x,y = im.size
    if x < y:
        iHeight,iWidth = iWidth,iHeight	  
    if iWidth!=0 and iHeight!=0 :
        im = im.resize( (iWidth, iHeight) )
    if os.path.exists( sNameOut ):
        print "Warning: output file " + sNameOut + " allready exists !"
    else:
        im.save( sNameOut, "JPG" )
    return im


process_all_files( sInputDir, sOutputDir )

#print "input:"+sInputDir		
