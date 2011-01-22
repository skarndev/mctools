/*
 * main.cpp
 *
 *  Created on: 22.10.2009
 *      Author: Bastian
 */
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
using namespace std;
typedef float Vec3D[3];

typedef unsigned short uint16;
typedef short int16;
typedef unsigned int uint32;
typedef int int32;

struct ModelHeader {
	char id[4];
	char version[4];
	uint32 nameLength;
	uint32 nameOfs;
	uint32 Flags;

	uint32 nGlobalSequences;
	uint32 ofsGlobalSequences;
	uint32 nAnimations;
	uint32 ofsAnimations;
	uint32 nAnimationLookup;
	uint32 ofsAnimationLookup;
	uint32 nBones;
	uint32 ofsBones;
	uint32 nKeyBoneLookup;
	uint32 ofsKeyBoneLookup;

	uint32 nVertices;
	uint32 ofsVertices;
	uint32 nViews;

	uint32 nColors;
	uint32 ofsColors;

	uint32 nTextures;
	uint32 ofsTextures;

	uint32 nTransparency; // H
	uint32 ofsTransparency;
	uint32 nTexAnims; // J
	uint32 ofsTexAnims;
	uint32 nTexReplace;
	uint32 ofsTexReplace;

	uint32 nRenderFlags;
	uint32 ofsRenderFlags;
	uint32 nBoneLookupTable;
	uint32 ofsBoneLookupTable;

	uint32 nTexLookup;
	uint32 ofsTexLookup;

	uint32 nTexUnitLookup; // L
	uint32 ofsTexUnitLookup;
	uint32 nTransparencyLookup; // M
	uint32 ofsTransparencyLookup;
	uint32 nTexAnimLookup;
	uint32 ofsTexAnimLookup;

	//not sure about these :/
	Vec3D VertexBoxMin;//?
	Vec3D VertexBoxMax;//?
	float VertexBoxRadius;
	Vec3D BoundingBoxMin;//?
	Vec3D BoundingBoxMax;//?
	float BoundingBoxRadius;

	uint32 nBoundingTriangles;
	uint32 ofsBoundingTriangles;
	uint32 nBoundingVertices;
	uint32 ofsBoundingVertices;
	uint32 nBoundingNormals;
	uint32 ofsBoundingNormals;

	uint32 nAttachments; // O
	uint32 ofsAttachments;
	uint32 nAttachLookup; // P
	uint32 ofsAttachLookup;
	uint32 nEvents; // SoundEvents
	uint32 ofsEvents;
	uint32 nLights; // R
	uint32 ofsLights;
	uint32 nCameras; // S
	uint32 ofsCameras;
	uint32 nCameraLookup;
	uint32 ofsCameraLookup;
	uint32 nRibbonEmitters; // U
	uint32 ofsRibbonEmitters;
	uint32 nParticleEmitters; // V
	uint32 ofsParticleEmitters;

};

struct AnimationBlock {
	int16 type; // interpolation type (0=none, 1=linear, 2=hermite, 3=Bezier)
	int16 seq; // global sequence id or -1
	uint32 nTimes;
	uint32 ofsTimes;
	uint32 nKeys;
	uint32 ofsKeys;
};

struct AnimSub{
	uint32 nOfs;
	uint32 ofsOfs;
};

struct ModelBoneDef {
	int32 KeyBoneID;
	uint32 flags;
	int16 parent; // parent bone index
	uint16 unk[3];
	AnimationBlock translation;
	AnimationBlock rotation;
	AnimationBlock scaling;
	Vec3D pivot;
};

int main(int argc, char **argv) {
	FILE* Input;
	int FileSize;
	char*f;
	Input=fopen(argv[1],"rb+");
	if(Input==NULL)
	{
		printf(" ERROR: Could not load file\n");
		return 0;
	}
	fseek(Input,0,SEEK_END);
	FileSize=ftell(Input);
	fseek(Input,0,SEEK_SET);

	f=new char[FileSize];

	fread(f,FileSize,1,Input);
	fclose(Input);
	string fullname=argv[1];
	fullname = fullname.substr(0, fullname.length() - 3 );
	fullname.append(".txt");
	filebuf fb;
	fb.open(fullname.c_str(),ios::out);
	ostream stream(&fb);
	int ani=atoi(argv[2]);
	ModelHeader *h=(ModelHeader *)f;
	ModelBoneDef *b=(ModelBoneDef *)(f+h->ofsBones);
	for(int i=0;i<h->nBones;i++){
		AnimSub*tr=(AnimSub*)(f+b[i].translation.ofsTimes);
		if(tr[ani].nOfs!=0){
			stream<<"Bone: "<<i<<" is used within Anim "<<ani<<"\n";
			continue;
		}
		AnimSub*sc=(AnimSub*)(f+b[i].scaling.ofsTimes);
		if(sc[ani].nOfs!=0){
			stream<<"Bone: "<<i<<" is used within Anim "<<ani<<"\n";
			continue;
		}
		AnimSub*ro=(AnimSub*)(f+b[i].rotation.ofsTimes);
		if(ro[ani].nOfs!=0){
			stream<<"Bone: "<<i<<" is used within Anim "<<ani<<"\n";
			continue;
		}
	}
	fb.close();
	return 0;
}
