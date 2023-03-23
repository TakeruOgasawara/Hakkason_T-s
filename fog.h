//========================================================================================
//
// �t�H�O�w�b�_�[[fog.h]
// Author: �O�� ���V��
//
//========================================================================================
#ifndef _FOG_H_
#define _FOG_H_
#include "main.h"

//�\���̂̒�`
typedef struct
{
	D3DFOGMODE FogMode;	//�t�H�O�̃��[�h
	D3DXCOLOR FogColor;	//�t�H�O�̐F
	float FogStart;		//�t�H�O�̊J�n�n�_
	float FogEnd;		//�t�H�O�̏I���n�_
	float FogDensity;	//�t�H�O�̖��x
	
}FOG;

//�v���g�^�C�v�錾
void InitFog();
void UninitFog();
void UpdateFog();
void DrawFog();
void SetFog(D3DFOGMODE FogMode, D3DXCOLOR FogColor, float FogStart, float FogEnd, float FogDensity);

#endif // !_FOG_H_
