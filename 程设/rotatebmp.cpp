#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cstring>

typedef unsigned short WORD;
typedef unsigned long DWORD;

typedef struct targetBITMAPFILEHEADER{
    DWORD bSize;
    WORD bReserved1;
    WORD bReserved2;
    DWORD bOffset;
}BITMAPFILEHEADER;

typedef struct targetBITMAPINFOHEADER{
    DWORD bInfoSize;
    DWORD bWidth;
    DWORD bHeight;
    WORD bPlanes;
    WORD bBitCount;
    DWORD bCompression;
    DWORD bmpImagesize;
    DWORD bXPelsPerMeter;
    DWORD bYPelsPerMeter;
    DWORD bClrUsed;
    DWORD bClrImportant;
}BITMAPINFOHEADER;

typedef struct targetRGBQUAD{
    char rgbBlue;
    char rgbGreen;
    char rgbRed;
    char rgbReserved;
}RGBQUAD;

typedef struct tagIMAGEDATA{
    char blue;
    char green;
    char red;
}IMAGEDATA;

using namespace std;

int main(int argc,char * argv[]){

    FILE *file,*targetFile;
    BITMAPFILEHEADER bmpFile,writeBmpFile;
    BITMAPINFOHEADER bmpInfo,writeBmpInfo;

    WORD bfType;
    file=fopen(argv[1],"rb");
    targetFile=fopen(argv[2],"wb");

    fread(&bfType,1,sizeof(WORD),file);
    fwrite(&bfType,1,sizeof(WORD),targetFile);
    if(0x4d42!=bfType){
        cout<<"wrong format!"<<endl;
        return 0;
    }

    fread(&bmpFile,1,sizeof(BITMAPFILEHEADER),file);    
    fread(&bmpInfo,1,sizeof(BITMAPINFOHEADER),file);
    writeBmpInfo=bmpInfo;
    writeBmpFile=bmpFile;

    int width=bmpInfo.bWidth;
    int height=bmpInfo.bHeight;

    int newheight=width;
    int newwidth=height;

    writeBmpInfo.bHeight=newheight;
    writeBmpInfo.bWidth=newwidth;
    fwrite(&writeBmpFile,1,sizeof(BITMAPFILEHEADER),targetFile);
    fwrite(&writeBmpInfo,1,sizeof(BITMAPINFOHEADER),targetFile);

    int stridewidth=(width%4==0?width*3:((width*3/4)+1)*4);
    int strideNewWidth=(newwidth%4==0?newwidth*3:((newwidth*3/4)+1)*4);
    char * preData = new char [stridewidth*height];
    char * laterData = new char [strideNewWidth*newheight];

    fread(preData,1,height*stridewidth,file);

	for(int j=0; j<newwidth; j++){
		for(int i=0; i<newheight*3; i+=3){				
			memcpy(laterData+(3*j+(newheight-1-i/3)*strideNewWidth), preData+(i+j*stridewidth), 3);
		}
	}

    fwrite(laterData,1,newheight*strideNewWidth,targetFile);

    fclose(file);
    fclose(targetFile);

    delete []preData;
    delete []laterData;

    return 0;
}
