#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream.h>

using namespace std;

FILE *Input;

int XOffset,YOffset;
long double WaterLevel;

/*
MCNK
0x14 MCVT Offset
0x18 MCNR Offset
0x1C MCLY Offset
0x20 MCRF Offset
0x24 MCAL Offset
0x2C MCSH Offset
0x58 MCSE Offset
0x60 MCLQ Offset
0x68 Z' Base Coordinate
0x6C X' Base Coordinate
0x70 Y Base Coordinate
*/

struct MCIN{
	unsigned int Offset;
	unsigned int Size;
	unsigned int Temp1;
	unsigned int Temp2;
};

unsigned int MDDF_Offset;
unsigned int MODF_Offset;

MCIN *Positions;
unsigned int MCLQ_Positions[256];
unsigned int MCSE_Positions[256];

char *File;
unsigned int FileSize;

char *NewFile;
unsigned int NewFileSize;
unsigned int Change;


void FindMCNKs()
{
	Positions=(MCIN *)(File+92);
}



void FixHoles()
{
	unsigned int *TFloat;
	unsigned int *TInt;
	for (int i=0;i<256;i++)
	{
		TInt=(unsigned int *)(File+Positions[i].Offset+0x8+0x3c);
		if(*TInt!=0)
		{
			printf("Found Holes in Chunk %d and patching. ",i,*TInt);
			*TInt=0;
			fseek(Input,Positions[i].Offset+0x8+0x3c,SEEK_SET);
			fwrite(TInt,sizeof(unsigned int),1,Input);
		}
	}
}



void MakeHoles(int a, int b)
{
    unsigned int *TFloat;
	unsigned int *TInt;
    TInt=(unsigned int *)(File+Positions[a].Offset+0x8+0x3c);

        printf("Creating Hole in Chunk %d",a,*TInt);
        *TInt=b;
        fseek(Input,Positions[a].Offset+0x8+0x3c,SEEK_SET);
        fwrite(TInt,sizeof(unsigned int),1,Input);


}



int main(int argc, char **argv)
{
	char *replace;
	int i,len;

    if(argc<2)
	{
		printf("Usage: CreateHoles <adt> <optional chunk>");
		return 0;
	}

	Input=fopen(argv[1],"rb+");
	fseek(Input,0,SEEK_END);
	FileSize=ftell(Input);
	NewFileSize=FileSize;
	File=new char[FileSize];
	fseek(Input,0,SEEK_SET);
	fread(File,1,FileSize,Input);


	Change=0;


	FindMCNKs();
	if (argc>2)
	{
	    if (atof(argv[2])>255)
	    cout<<"An ADT has only 256 chunks,starting with 0";
	    else
	    {
	    if (atof(argv[2])<0)
	    cout<<"An ADT has of course no negative chunks.";
	    else
        MakeHoles(atof(argv[2]),atof(argv[3]));
	    }
	}
	else
	{
	printf("Patchingg Holes from File %s\n",argv[1]);
	FixHoles();
	}

	fclose(Input);


	delete File;
}
