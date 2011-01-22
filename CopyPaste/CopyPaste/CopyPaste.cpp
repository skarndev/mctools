// CopyPasteVb0.1.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include <Windows.h>

/*
Vorstellung: Per Menü kann man die datei dann öffnen und die anzahl und die koordinaten eingeben, sodass das ganze schnell verarbeitet wird!
Mit hilfe von Fehler codes kann ich mir platz in der Hauptprogrammierung sparen^^

ok Lets get started
*/
bool paster()
{
	system("cls");
	char adt0[256];
	printf("ADT-modelname: ");
	scanf("%s", adt0);
	FILE* input = fopen(adt0, "rb");
	if(!input)
	{
		printf("\n\n Cannot open Data. \n");
		return 0;
	}

	printf("\n Writing ADT into temporary Memory \n");
	fseek(input, 0, SEEK_END);
    long size = ftell(input);
    fseek(input, 0, SEEK_SET);

    char* buffer = new char[size];
    fread(buffer, 1, size ,input);
    fclose(input);
	printf("\n Writing ADT into temporary Memory was successfull. \n");

	system("cls");
	printf("Please enter the x and y coordinats. \n Also please enter the second x coordinat for creating an rectangle/square and the name of the new adt's. \n Enter it like this: abc 20 12 14 \n");
	int x, y, z;
	char adt1[256];
	char dBuff[256];
	scanf("%s %d %d %d", &adt1, &x, &y, &z);
	printf("\n Starting to write form temporary Memory too ADT. \n\n");
	for( ; x <= z; ++x)
    {      
		for(int y2 = z; y2 >= y; --y2)
		{  
			if(x <= 9 && y2 <= 9)
            {
				sprintf_s(dBuff,"%s_0%d_0%d.adt", adt1, x, y2);
            }
            else
			{
				if(x <= 9 && y2 >= 10)
				{
					sprintf_s(dBuff,"%s_0%d_%d.adt", adt1, x, y2);
				}
				else
				{
					if(x >= 10 && y2 <= 9)
					{
						sprintf_s(dBuff,"%s_%d_0%d.adt", adt1, x, y2);
					}
					else
					{
						sprintf_s(dBuff,"%s_%d_%d.adt", adt1, x, y2);
					}
				}
			}
			FILE* output = fopen(dBuff,"wb");
            if(!output)
            {
				printf("\n\n Cannot create Data. \n");
                return 0;
			}
			fwrite(buffer, 1, size, output);
            fclose(output);
		}
	}
	printf("Writing Finished, now deleting temporary Memory \n");
    delete[] buffer;
	printf("Temporary Memory has been fully deleted. \n");
	
	return 1;
};

bool menu()
{
	int a = 0;
	while(a != 2)
	{
		printf("****************************************** \n");
		printf("** 1 - paster                           ** \n");
		printf("** 2 - exit                             ** \n");
		printf("****************************************** \n");
		printf("Please enter the number for the command \n");
		scanf("%d", &a);
		switch(a)
		{
			case 1: paster();
					break;
			default: printf("Please enter an available number for a command!");
					 break;
		}
	}
	return 0;
};

int _tmain()
{
	menu();
	return 0;
}

