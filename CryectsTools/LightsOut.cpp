#include <stdio.h>

struct SMOLight // 04-29-2005 By ObscuR
{
			int Flags;
/*004h*/  unsigned int color;  
/*008h*/  float position[3];
/*014h*/  float intensity;
/*018h*/  float attenStart;
/*01Ch*/  float attenEnd;
/*020h*/  float unk1;
/*024h*/  float unk2;
/*028h*/  float unk3;
/*02Ch*/  float unk4;
/*030h*/  
};

struct MODD
{
	int	Fake[9];
	unsigned int color;
};



char *nextChunk(char *buffer)
{
	unsigned int offset;
	offset=*(unsigned int *)(buffer+4);
	return buffer+offset+8;
}

int main(int argc, char *argv[])
{
	FILE *f;
	char *buffer;
	char *pos;
	int	Length;	

	if(argc<2)
		printf("LightsOut <WMO File>\n");

	f=fopen(argv[1],"rb+");
	if(f==0)
	{
		printf("ERROR: Couldn't open %s\n",argv[1]);
		return 0;
	}
	fseek(f,0,SEEK_END);
	Length=ftell(f);
	fseek(f,0,SEEK_SET);
	buffer=new char[Length];
	fread(buffer,Length,1,f);
	fclose(f);

	
	unsigned int HeaderType;
	char *Temp;
	Temp=(char *)&HeaderType;

	HeaderType=*(unsigned int *)(buffer+12);

	printf("Checking File Type %c%c%c%c\n",Temp[3],Temp[2],Temp[1],Temp[0]);

	if(HeaderType==0x4d4f4750)
	{
		printf("Processing a Group WMO\n");
		pos=buffer+88;
	}
	else if(HeaderType==0x4d4f4844)
	{
		printf("Processing a Root WMO\n");
		pos=buffer+84;
	}

	while(((pos-buffer)<Length)&&(*(unsigned int *)pos!=0x4d4f4356)&&(*(unsigned int *)pos!=0x4d4f4C54))
	{
		printf("Skipping chunk %c%c%c%c\n",pos[3],pos[2],pos[1],pos[0]);
		pos=nextChunk(pos);
	}

	printf("Found chunk %c%c%c%c\n",pos[3],pos[2],pos[1],pos[0]);

	if(((pos-buffer)<Length)&&(*(unsigned int *)pos==0x4d4f4C54))
	{
		int count=(*(int *)(pos+4))/sizeof(SMOLight);
		SMOLight *Lights;
		Lights=(SMOLight *)(pos+8);

		printf("Setting %d light colors to (0,0,0,1)\n",count);
		
		for(int i=0;i<count;i++)
			Lights[i].color=0xff000000;

		printf("Looking for MODD\n");

		pos=buffer+84;

		while(((pos-buffer)<Length)&&(*(unsigned int *)pos!=0x4d4f4444))
		{
			printf("Skipping chunk %c%c%c%c\n",pos[3],pos[2],pos[1],pos[0]);
			pos=nextChunk(pos);
		}

		if(((pos-buffer)<Length)&&(*(unsigned int *)pos==0x4d4f4444))
		{
			int count=(*(int *)(pos+4))/sizeof(MODD);
			MODD *Models;
			Models=(MODD *)(pos+8);

			printf("Setting %d model colors to (0,0,0,1)\n",count);
		
			for(int i=0;i<count;i++)
				Models[i].color=0xff000000;
		}

	}
	else if(((pos-buffer)<Length)&&(*(unsigned int *)pos==0x4d4f4356))
	{
		int count=(*(int *)(pos+4))/4;
		unsigned int *light=(unsigned int *)(pos+8);

		printf("Setting %d vertex colors to (0,0,0,1)\n",count);
		
		for(int i=0;i<count;i++)
			light[i]=0xff000000;
	}

	f=fopen(argv[1],"wb");
	fwrite(buffer,Length,1,f);
	fclose(f);
	delete buffer;

	return 0;
}
