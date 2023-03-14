//======================================================================================
//
//�p�[�e�B�N���̏���[Player.cpp]
//Author;���}���@�V
//
//======================================================================================
#include "main.h"
#include "particle.h"
#include "effect.h"

//*****************************
//�}�N����`
//*****************************
#define MAX_PARTICLE			(MAX_EFFECT)			//�p�[�e�B�N���̍ő吔

//*****************************
//�v���g�^�C�v�錾
//*****************************
void ParticleType(int nCntParticle);

//*****************************
//�O���[�o���錾
//*****************************
Particle g_aParticle[MAX_PARTICLE];			//�p�[�e�B�N���̏��

//========================================================================
// �p�[�e�B�N���̏���������
//========================================================================
void InitParticle(void)
{
	//�ϐ��錾
	int nCntParticle;

	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		//�\���̂̏�����
		g_aParticle[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//�ړ��ʂ�����������
		g_aParticle[nCntParticle].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//����������������(Z�l���g�p)
		g_aParticle[nCntParticle].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].type = PARTICLE_TYPE_NONE;
		g_aParticle[nCntParticle].fRadius = 0.0f;
		g_aParticle[nCntParticle].nLife = 0;
		g_aParticle[nCntParticle].bUse = false;				//�g�p���Ă��Ȃ���Ԃɂ���
	}
}

//========================================================================
//�p�[�e�B�N���̏I������
//========================================================================
void UninitParticle(void)
{

}

//========================================================================
// �p�[�e�B�N���̍X�V����
//========================================================================
void UpdateParticle(void)
{
	
}

//========================================================================
// �p�[�e�B�N���^�C�v0(�Ή�)
//========================================================================
void ParticleType(int nCntParticle)
{
	//�ϐ��錾
	float fRot;
	float fMove;

	for (int nCntApper = 0; nCntApper < 20; nCntApper++)
	{
		//�ʒu�̐ݒ�
		//g_aParticle[nCntParticle].pos;

		//�p�x�̐ݒ�
		fRot = (float)(rand() % 629 - 314) / 100.0f;

		//�ړ��ʂ̐ݒ�
		fMove = (float)(rand() % 30) / 10;

		g_aParticle[nCntParticle].move.x = sinf(fRot) * fMove;

		//�p�x�̐ݒ�
		fRot = (float)(rand() % 629 - 314) / 100.0f;

		//�ړ��ʂ̐ݒ�
		fMove = (float)(rand() % 30) / 10;

		g_aParticle[nCntParticle].move.y = cosf(fRot) * fMove;

		//�p�x�̐ݒ�
		fRot = (float)(rand() % 629 - 314) / 100.0f;

		//�ړ��ʂ̐ݒ�
		fMove = (float)(rand() % 30) / 10;

		g_aParticle[nCntParticle].move.z = sinf(fRot) * fMove;

		//�F�̐ݒ�
		g_aParticle[nCntParticle].col = D3DXCOLOR{ 1.0f, 0.6f, 0.3f, 1.0f };

		//���a�̐ݒ�
		g_aParticle[nCntParticle].fRadius = 10.0f;
	
		//�G�t�F�N�g�ݒ�
		SetEffect(g_aParticle[nCntParticle].pos, g_aParticle[nCntParticle].move,
			g_aParticle[nCntParticle].col, g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].nLife);
	}
}

//========================================================================
//�p�[�e�B�N���̕`�揈��
//========================================================================
void DrawParticle(void)
{

}

//========================================================================
//�p�[�e�B�N���̐ݒ菈��
//========================================================================
void SetParticle(D3DXVECTOR3 pos, D3DXVECTOR3 rot, PARTICLE_TYPE type)
{
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == false)
		{//�g�p���Ă��Ȃ���ԂȂ�
			g_aParticle[nCntParticle].pos = pos;
			g_aParticle[nCntParticle].rot = rot;
			g_aParticle[nCntParticle].type = type;
			g_aParticle[nCntParticle].bUse = true;

			switch (g_aParticle[nCntParticle].type)
			{//�^�C�v�ɂ�镪��
			case PARTICLE_TYPE_HIT:
				g_aParticle[nCntParticle].nLife = 100;
				ParticleType(nCntParticle);
				break;
			}

			break;
		}
	}
}
