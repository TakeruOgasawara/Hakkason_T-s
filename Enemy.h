
//======================================================================================
//
//�G�̏���[Enemy.h]
//Author;�O�엳�V��
//
//======================================================================================
#ifndef _Enemy_H_
#define _Enemy_H_

#include "main.h"
#include "model.h"

typedef enum
{
	ENEMY_TYPE_NONE = 0,
	ENEMY_TYPE_MAX
}ENEMY_TYPE;

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posOld;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 move;
	D3DXMATRIX mtxWorld;
	ENEMY_TYPE Type;
	bool bUse;
}ENEMY;

//�v���g�^�C�v�錾
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move, ENEMY_TYPE Type);
bool CollisionOuterProductEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove);

#endif