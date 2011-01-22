/* ----------------------------------------------------------

   original C++ code by:
                        Gustav "Grim Reaper" Munkby
                        http://floach.pimpin.net/grd/
                        grimreaperdesigns@gmx.net
   
   modified and converted to C# by:
                        Robert A. Wlodarczyk
                        http://rob.wincereview.com:8080
                        rwlodarc@hotmail.com
   ---------------------------------------------------------- */

#include <string>
#include <fstream>
#include <iostream>

#include <stdio.h>

#include "minutils.h"

using namespace std;

typedef unsigned long   ulong;
typedef /*unsigned*/ char	_byte;

class MP3Header
{
    // Public variables for storing the information about the MP3
public:
    int intBitRate;
    string strFileName;
    long lngFileSize;
    int intFrequency;
    string strMode;
    int intLength;
    string strLengthFormatted;

    // Private variables used in the process of reading in the MP3 files
private:
    ulong bithdr;
    bool boolVBitRate;
    int intVFrames;

public:
    bool ReadMP3Information(const string & FileName)
    {
        fstream/*FileStream*/ fs /*= new FileStream*/(FileName.c_str(), ios::in/*FileMode.Open, FileAccess.Read*/);
        // Set the filename not including the path information
        strFileName = FileName;		// @fs.Name;
/*
        char[] chrSeparators = new char[]{'\\','/'};
        string[] strSeparator = strFileName.Split(chrSeparators);
        
		int intUpper = strSeparator.GetUpperBound(0);
        strFileName = strSeparator[intUpper];
        
        // Replace ' with '' for the SQL INSERT statement
        strFileName = strFileName.Replace("'", "''");
   
        // Set the file size
*/
        lngFileSize = FileUtilityObj::GetFileSize(FileName.c_str()); //Length;

        _byte * bytHeader = new _byte[4];
        _byte * bytVBitRate = new _byte[12];
        int intPos = 0;
        
        // Keep reading 4 bytes from the header until we know for sure that in 
        // fact it's an MP3
        do
        {
//            fs.Position = intPos;
//            fs.Read(bytHeader,0,4);
			fs.read(bytHeader,4);
            intPos++;
            LoadMP3Header(bytHeader);
        }
        while(!IsValidHeader() && !fs.eof()/*&& (fs.Position!=fs.Length)*/);
        
		delete [] bytHeader;

        // If the current file stream position is equal to the length, 
        // that means that we've read the entire file and it's not a valid MP3 file
//        if(fs.Position != fs.Length)
		if( !fs.eof() )
        {
            intPos += 3;

            if(getVersionIndex() == 3)    // MPEG Version 1
            {
                if(getModeIndex() == 3)    // Single Channel
                {
                    intPos += 17;
                }
                else
                {
                    intPos += 32;
                }
            }
            else                        // MPEG Version 2.0 or 2.5
            {
                if(getModeIndex() == 3)    // Single Channel
                {
                    intPos += 9;
                }
                else
                {
                    intPos += 17;
                }
            }
            
            // Check to see if the MP3 has a variable bitrate
//            fs.Position = intPos;
//            fs.Read(bytVBitRate,0,12);
			fs.read(bytVBitRate,12);
            boolVBitRate = LoadVBRHeader(bytVBitRate);

			delete [] bytVBitRate;

            // Once the file's read in, then assign the properties of the file to the public variables
            intBitRate = getBitrate();
            intFrequency = getFrequency();
            strMode = getMode();
            intLength = getLengthInSeconds();
            strLengthFormatted = getFormattedLength();
            fs.close();
            return true;
        }
		delete [] bytVBitRate;
        return false;
    }

//private:  // patch to start inbetween the song with mpg123
    void LoadMP3Header(_byte * c)
    {
        // this thing is quite interesting, it works like the following
        // c[0] = 00000011
        // c[1] = 00001100
        // c[2] = 00110000
        // c[3] = 11000000
        // the operator << means that we'll move the bits in that direction
        // 00000011 << 24 = 00000011000000000000000000000000
        // 00001100 << 16 =         000011000000000000000000
        // 00110000 << 24 =                 0011000000000000
        // 11000000       =                         11000000
        //                +_________________________________
        //                  00000011000011000011000011000000
        bithdr = (ulong)(((c[0] & 255) << 24) | ((c[1] & 255) << 16) | ((c[2] & 255) <<  8) | ((c[3] & 255))); 
    }

    bool LoadVBRHeader(_byte * inputheader)
    {
        // If it's a variable bitrate MP3, the first 4 bytes will read 'Xing'
        // since they're the ones who added variable bitrate-edness to MP3s
        if(inputheader[0] == 88 && inputheader[1] == 105 && 
            inputheader[2] == 110 && inputheader[3] == 103)
        {
            int flags = (int)(((inputheader[4] & 255) << 24) | ((inputheader[5] & 255) << 16) | ((inputheader[6] & 255) <<  8) | ((inputheader[7] & 255)));
            if((flags & 0x0001) == 1)
            {
                intVFrames = (int)(((inputheader[8] & 255) << 24) | ((inputheader[9] & 255) << 16) | ((inputheader[10] & 255) <<  8) | ((inputheader[11] & 255)));
                return true;
            }
            else
            {
                intVFrames = -1;
                return true;
            }
        }
        return false;
    }

    bool IsValidHeader() 
    {
        return (((getFrameSync()      & 2047)==2047) &&
                ((getVersionIndex()   &    3)!=   1) &&
                ((getLayerIndex()     &    3)!=   0) && 
                ((getBitrateIndex()   &   15)!=   0) &&
                ((getBitrateIndex()   &   15)!=  15) &&
                ((getFrequencyIndex() &    3)!=   3) &&
                ((getEmphasisIndex()  &    3)!=   2)    );
    }

    int getFrameSync()     
    {
        return (int)((bithdr>>21) & 2047); 
    }

    int getVersionIndex()  
    { 
        return (int)((bithdr>>19) & 3);  
    }

    int getLayerIndex()    
    { 
        return (int)((bithdr>>17) & 3);  
    }

    int getProtectionBit() 
    { 
        return (int)((bithdr>>16) & 1);  
    }

    int getBitrateIndex()  
    { 
        return (int)((bithdr>>12) & 15); 
    }

    int getFrequencyIndex()
    { 
        return (int)((bithdr>>10) & 3);  
    }

    int getPaddingBit()    
    { 
        return (int)((bithdr>>9) & 1);  
    }

    int getPrivateBit()    
    { 
        return (int)((bithdr>>8) & 1);  
    }

    int getModeIndex()     
    { 
        return (int)((bithdr>>6) & 3);  
    }

    int getModeExtIndex()  
    { 
        return (int)((bithdr>>4) & 3);  
    }

    int getCoprightBit()   
    { 
        return (int)((bithdr>>3) & 1);  
    }

    int getOrginalBit()    
    { 
        return (int)((bithdr>>2) & 1);  
    }
    
    int getEmphasisIndex() 
    { 
        return (int)(bithdr & 3);  
    }

    double getVersion() 
    {
        double table[4] = {2.5, 0.0, 2.0, 1.0};
        return table[getVersionIndex()];
    }

    int getLayer() 
    {
        return (int)(4 - getLayerIndex());
    }

    int getBitrate() 
    {
        // If the file has a variable bitrate, then we return an integer average bitrate,
        // otherwise, we use a lookup table to return the bitrate
        if(boolVBitRate)
        {
            double medFrameSize = (double)lngFileSize / (double)getNumberOfFrames();
            return (int)((medFrameSize * (double)getFrequency()) / (1000.0 * ((getLayerIndex()==3) ? 12.0 : 144.0)));
        }
        else
        {
            int table[2][3][16]
				       =        {
                                { // MPEG 2 & 2.5
                                    {0,  8, 16, 24, 32, 40, 48, 56, 64, 80, 96,112,128,144,160,0}, // Layer III
                                    {0,  8, 16, 24, 32, 40, 48, 56, 64, 80, 96,112,128,144,160,0}, // Layer II
                                    {0, 32, 48, 56, 64, 80, 96,112,128,144,160,176,192,224,256,0}  // Layer I
                                },
                                { // MPEG 1
                                    {0, 32, 40, 48, 56, 64, 80, 96,112,128,160,192,224,256,320,0}, // Layer III
                                    {0, 32, 48, 56, 64, 80, 96,112,128,160,192,224,256,320,384,0}, // Layer II
                                    {0, 32, 64, 96,128,160,192,224,256,288,320,352,384,416,448,0}  // Layer I
                                }
                                };

            return table[getVersionIndex() & 1][getLayerIndex()-1][getBitrateIndex()];
        }
    }

    int getFrequency() 
    {
        int table[4][3] 
				  =    {    
                            {32000, 16000,  8000}, // MPEG 2.5
                            {    0,     0,     0}, // reserved
                            {22050, 24000, 16000}, // MPEG 2
                            {44100, 48000, 32000}  // MPEG 1
                        };

        return table[getVersionIndex()][getFrequencyIndex()];
    }

    string getMode() 
    {
        switch(getModeIndex()) 
        {
            default:
                return "Stereo";
            case 1:
                return "Joint Stereo";
            case 2:
                return "Dual Channel";
            case 3:
                return "Single Channel";
        }
    }

    int getLengthInSeconds() 
    {
        // "intKilBitFileSize" made by dividing by 1000 in order to match the "Kilobits/second"
        int intKiloBitFileSize = (int)((8 * lngFileSize) / 1000);
        return (int)(intKiloBitFileSize/getBitrate());
    }

    string getFormattedLength() 
    {
        // Complete number of seconds
        int s  = getLengthInSeconds();

        // Seconds to display
        int ss = s%60;

        // Complete number of minutes
        int m  = (s-ss)/60;

        // Minutes to display
        int mm = m%60;

        // Complete number of hours
        int h = (m-mm)/60;

        // Make "hh:mm:ss"
		char sBuffer[255];
		sprintf(sBuffer,"%02d:%02d:%02d",h,mm,ss);
		return sBuffer;
        //return h.ToString("D2") + ":" + mm.ToString("D2") + ":" + ss.ToString("D2");
    }

    int getNumberOfFrames() 
    {
        // Again, the number of MPEG frames is dependant on whether it's a variable bitrate MP3 or not
        if (!boolVBitRate) 
        {
            double medFrameSize = (double)(((getLayerIndex()==3) ? 12 : 144) *((1000.0 * (float)getBitrate())/(float)getFrequency()));
            return (int)(lngFileSize/medFrameSize);
        }
        else 
            return intVFrames;
    }
};

#ifdef _with_test_main

  unsigned long GetFileSize( const char *sFileName )
  {
	fstream aFile( sFileName, ios::in );

	if( aFile.good() )
	{
		// fuer die Progress-Anzeige die Dateigroesse merken
		aFile.seekg( 0, ios::end );
		return (unsigned long)aFile.tellg();
	}
 	return (unsigned long)-1;		// Datei nicht gefunden !
  }


int main(int argc, char * * argv)
{
	MP3Header aMP3Header;

	if( argc>1 )
	{
		bool boolIsMP3 = aMP3Header.ReadMP3Information(argv[1]);
		if(boolIsMP3)
		{
			cout << "laenge: " << aMP3Header.intLength << endl;
			cout << "laenge str: " << aMP3Header.strLengthFormatted.c_str() << endl;
		}
	}
	return 0;
}

#endif
