

#ifndef _PAUSE_H_
#define _PAUSE_H_

#include "main.h"

//�|�[�Y���j���[�̍\����
typedef struct
{
	D3DXVECTOR3 pos;				//�ʒu
	D3DXCOLOR col;					//�F
	bool bPause;					//�|�[�Y���g���Ă��邩
}Pause;

//�v���g�^�C�v�錾
void InitPause();
void UninitPause();
void UpdatePause();
void DrawPause();

Pause *GetPause(void);

#endif