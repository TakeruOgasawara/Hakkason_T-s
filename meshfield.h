//====================================================================
//
// �t�B�[���h�̏���[field.h]
// Author: ���}���V
//
//====================================================================
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

#include "main.h"

//*****************************************
// �}�N����`
//*****************************************
#define MAX_MESHFIELD			(12)

//******************************************
//�t�B�[���h�\���̂̒�`
//******************************************
typedef struct
{
	D3DXVECTOR3 pos;					//�ʒu
	D3DXVECTOR3 rot;					//����
	D3DXMATRIX mtxWorldField;			//���[���h�}�g���b�N�X
	int nTexType;						//�e�N�X�`���\�̃^�C�v
	bool bUse;							//�g���Ă��邩
} MeshField;

//******************************************
//�v���g�^�C�v�錾
//******************************************
void InitMeshField(void);
void UninitMeshField(void);
void UpdateMeshField(void);
void DrawMeshField(void);

void SetMeshField(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nTexType);


#endif