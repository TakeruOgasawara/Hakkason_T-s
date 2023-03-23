//========================================
//
//fÇ(model.h)
//Author:ûüRç
//
//========================================
#ifndef _MODEL_H_							//±Ì}Nè`ª³êÄ¢È©Á½ç
#define _MODEL_H_							//2dCN[hh~Ì}Nðè`·év

#include"main.h"

//*****************************************
// }Nè`
//*****************************************
#define NUM_TEX	(10)	//eNX`Ì

//*****************************************
// \¢Ìè`
//*****************************************
typedef struct
{
	LPD3DXMESH pMesh;	//bV(¸_îñ)ÖÌ|C^
	LPD3DXBUFFER pBuffMat;	//}eAÖÌ|C^
	DWORD dwNumMat;	//}eAÌ
	LPDIRECT3DTEXTURE9 apTexture[NUM_TEX];	//eNX`ÖÌ|C^
	D3DXVECTOR3 vtxMin;	//Å¬¸_ÀW
	D3DXVECTOR3 vtxMax;	//Åå¸_ÀW
}Model;

//*****************************************
// vg^Cvé¾
//*****************************************
//î{
void InitModel(void);
void UninitModel(void);
void UninitXFile(Model *pModel);

//ÇÝÝ
void LoadModel(Model *pModel, const char *pFilePass);	//ÄpÇÝÝ
void LoadAllModel(void);	//ÇÝÝÜÆß

//îñæ¾

#endif