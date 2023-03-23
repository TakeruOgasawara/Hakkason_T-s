//====================================================================
//
// ���f���̏���[object.h]
// Author: ���}���V
//
//====================================================================
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "main.h"

//�}�N����`
#define MAX_OBJTEX			(20)			//�e�N�X�`���̂���
#define MAX_OBJ				(128)				//���f���̐�

#define MAX_WAVE	(2)

//�I�u�W�F�N�g�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;					//�ʒu
	D3DXVECTOR3 rot;					//����
	D3DXVECTOR3 move;					//�ړ���
	D3DXVECTOR3 vtxMin, vtxMax;	//�I�u�W�F�N�g�̍ő�l�E�ŏ��l
	D3DXMATRIX mtxWorld;				//���[���h�}�g���b�N�X
	int nIdxShadow;						//�Ώۂ̉e�̃C���f�b�N�X(�ԍ�)
	int nType;							//���
	bool bUse;							//�g���Ă��邩
} Object;

typedef struct
{
	LPDIRECT3DTEXTURE9 pTexture[MAX_OBJTEX];
} Obj;

//�v���g�^�C�v�錾
void InitObject(void);
void UninitObject(void);
void UpdateObject(void);
void DrawObject(void);
void SetObject(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);
Object *GetObj(void);
int GetNumObj(void);
void LoadObj(int nStage);
bool ClsObj(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pVtxMax, D3DXVECTOR3 *pVtxMin, D3DXVECTOR3 *pMove);
#endif