//========================================================================================
//
// �p�[�e�B�N���̏���[particle.cpp]
// Author: �O�� ���V��
//
//========================================================================================
#include "particle.h"
#include "effect.h"

#define MAX_PARTICLE (128)

typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 move; //�ړ���
	D3DXVECTOR3 movePos; //�o���ʒu�̈ړ���
	D3DXVECTOR3 rot; //�p�x
	D3DXCOLOR col; //�F
	float fMove; //�ړ���
	float fRadius; //���a
	int nLife; //����
	int nEffect; //�G�t�F�N�g�̓����Ăяo����
	int nEffectLife; //�G�t�F�N�g�̎���
	int nRand; //�g�U�x����
	int nMaxSpeed; //�ō����x
	bool bUse; //�g�p���Ă��邩
	PARTICLE_TYPE  type;  //�p�[�e�B�N���̎��
}Particle;

Particle g_aParticle[MAX_PARTICLE];

void InitParticle()
{
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		g_aParticle[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		g_aParticle[nCntParticle].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aParticle[nCntParticle].bUse = false;

		g_aParticle[nCntParticle].type = PARTICLE_TYPE_KEY;
	}
}
void UninitParticle()
{

}
void UpdateParticle()
{
	float fRot;//�p�x
	float fRot2;//�p�x
	float fMove;//�ړ���
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == true)
		{
			for (int nCntAppear = 0; nCntAppear < g_aParticle[nCntParticle].nEffect; nCntAppear++)
			{
				g_aParticle[nCntParticle].nLife--;
				fRot = (float)(rand() % g_aParticle[nCntParticle].nRand - g_aParticle[nCntParticle].nRand / 2) / 100.0f;
				fRot2 = (float)(rand() % g_aParticle[nCntParticle].nRand - g_aParticle[nCntParticle].nRand / 2) / 100.0f;
				fMove = (float)(rand() % g_aParticle[nCntParticle].nMaxSpeed) / 2;
				g_aParticle[nCntParticle].move.x = cosf(fRot + g_aParticle[nCntParticle].rot.x) * sinf(fRot2 + g_aParticle[nCntParticle].rot.y) * fMove;
				g_aParticle[nCntParticle].move.y = cosf(fRot + g_aParticle[nCntParticle].rot.x) * cosf(fRot2 + g_aParticle[nCntParticle].rot.y) *fMove;
				g_aParticle[nCntParticle].move.z = sinf(fRot + g_aParticle[nCntParticle].rot.x) *fMove;

				g_aParticle[nCntParticle].move += g_aParticle[nCntParticle].movePos / 2;

				SetEffect(g_aParticle[nCntParticle].pos, g_aParticle[nCntParticle].col, g_aParticle[nCntParticle].move, g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].nEffectLife, g_aParticle[nCntParticle].type);

				if (g_aParticle[nCntParticle].nLife <= 0)
				{
					g_aParticle[nCntParticle].bUse = false;
				}

				else
				{
					g_aParticle[nCntParticle].col.a -= 0.1f;
					g_aParticle[nCntParticle].fRadius -= 0.1f;
				}
			}
			switch (g_aParticle[nCntParticle].type)
			{
			case PARTICLE_TYPE_KEY:

				KeyParticle();

				break;
			}
		}
	}
}
void DrawParticle()
{

}
void SetParticle(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, D3DXVECTOR3 rot, float fRad, float fMove, int nLife, PARTICLE_TYPE type, int nEffect, int nEffectLife, int nRand, int nMaxSpeed)
{
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == false)
		{
			g_aParticle[nCntParticle].pos = pos;
			g_aParticle[nCntParticle].movePos = move;
			g_aParticle[nCntParticle].rot = rot;
			g_aParticle[nCntParticle].col = col;
			g_aParticle[nCntParticle].fRadius = fRad;
			g_aParticle[nCntParticle].type = type;
			g_aParticle[nCntParticle].fMove = fMove;
			g_aParticle[nCntParticle].nLife = nLife;
			g_aParticle[nCntParticle].bUse = true;
			g_aParticle[nCntParticle].nEffect = nEffect;
			g_aParticle[nCntParticle].nEffectLife = nEffectLife;
			g_aParticle[nCntParticle].nRand = nRand;
			g_aParticle[nCntParticle].nMaxSpeed = nMaxSpeed;
			break;
		}
	}
}

//==============================
//���̃p�[�e�B�N��
//==============================
void KeyParticle(void)
{
	//���̃p�[�e�B�N���̐F
	D3DXCOLOR(0.5f, 0.0f, 0.5f, 1.0f);
}
