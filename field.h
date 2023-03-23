//====================================================================
//
// �t�B�[���h�̏���[field.h]
// Author: ���}���V
//
//====================================================================
#ifndef _FIELD_H_
#define _FIELD_H_

#include "main.h"

//�t�B�[���h�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;					//�ʒu
	D3DXVECTOR3 rot;					//����
	D3DXMATRIX mtxWorldField;			//���[���h�}�g���b�N�X
	bool bUse;							//�g���Ă��邩
} Field;

//�v���g�^�C�v�錾
void InitField(void);
void UninitField(void);
void UpdateField(void);
void DrawField(void);
void SetField(D3DXVECTOR3 pos);

#endif