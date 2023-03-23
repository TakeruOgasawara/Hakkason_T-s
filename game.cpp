//===========================================================================================
//
// �Q�[�����̓���[game.cpp]
// Author:���}���@�V
//
//===========================================================================================
#include "input.h"
#include "effect.h"
#include "particle.h"
#include "time.h"
#include "game.h"
#include "fade.h"
#include "pause.h"
#include "sound.h"
#include "meshfield.h"
#include "field.h"
#include "camera.h"
#include "light.h"
#include "player.h"
#include "Enemy.h"
#include "fog.h"
#include "object.h"
#include "dome.h"

//*****************************
// �}�N����`
//*****************************
#define GOAL_DIST	(150000.0f)	//�S�[���܂ł̋���

//*****************************
// �v���g�^�C�v�錾
//*****************************

//*****************************
//�O���[�o���錾
//*****************************
GAMESTATE g_gameState = GAMESTATE_NONE;				//�Q�[���̏��
int g_nCounterGameState = 0;						//��ԊǗ��̃J�E���^�[
bool g_bPause = false;								//�|�[�Y��Ԃ�ON/OFF

//========================================================================
//�Q�[����ʂ̏���������
//========================================================================
void InitGame(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���C�g�̏���������
	InitLight();

	//�J�����̏���������
	InitCamera();

	//�G�t�F�N�g�̏���������
	InitEffect();

	//�p�[�e�B�N���̏���������
	InitParticle();

	//�|�[�Y�̏���������
	InitPause();

	//���b�V���t�B�[���h�̏���������
	InitMeshField();
	InitField();

	//�^�C���̏�����
	InitTime();

	//�v���C���[�̏�����
	InitPlayer();

	InitEnemy();
	//�t�H�O�̏�����
	InitFog();

	//�t�H�O�̐ݒ�
	SetFog(D3DFOG_LINEAR, D3DXCOLOR(0.7f, 0.7f, 0.8f, 1.0f), 100.0f, 7000.0f, 0.1f);

	InitObject();

	InitDome();

	//�ʏ��Ԃ�
	g_gameState = GAMESTATE_NORMAL;

	//��ԊǗ��̃J�E���^�[��������
	g_nCounterGameState = 0;

	//�|�[�Y���̎擾
	Pause *pPause = GetPause();

	//�|�[�Y��Ԃ�ON/OFF
	pPause->bPause = false;

	//�T�E���h�̍Đ�
	PlaySound(SOUND_LABEL_BGM001);
}
//========================================================================
//�Q�[����ʂ̏I������
//========================================================================
void UninitGame(void)
{
	//�T�E���h�̒�~
	StopSound();

	//�v���C���[�I������
	UninitPlayer();

	UninitEnemy();
	//�G�t�F�N�g�̏I������
	UninitEffect();

	//�^�C���̏I��
	UninitTime();

	//�p�[�e�B�N���̏I������
	UninitParticle();

	//�|�[�Y�̏I������
	UninitPause();

	UninitDome();
	//�t�H�O�̏I������
	UninitFog();

	//�I�u�W�F�N�g�̏I������
	UninitObject();

	//���b�V���t�B�[���h�̏I������
	UninitMeshField();
	UninitField();
}

//========================================================================
//�Q�[����ʂ̍X�V����
//========================================================================
void UpdateGame(void)
{
	//���̎擾
	Pause *pPause = GetPause();			//�|�[�Y
	Fade pFade = GetFade();				//�t�F�[�h
	Player *pPlayer = GetPlayer();

	if (pFade == FADE_NONE)
	{//�t�F�[�h��Ԃ���Ȃ��ꍇ

		UpdateDome();

		if (GetKeyboardTrigger(DIK_P) == true || GetJoyPadTrigger(BUTTON_START, 0) == true)
		{//�L�[�������ꂽ�ꍇ
			//�|�[�Y��ON/OFF
			pPause->bPause = pPause->bPause ? false : true;
		}
		if (GetKeyboardTrigger(DIK_P) == true || GetJoyPadTrigger(BUTTON_START, 0) == true && pPause->bPause == false)
		{//�|�[�Y����
			//�|�[�Y�̏�����
			InitPause();
		}

		//�|�[�Y���g���Ă���Ƃ�
		if (pPause->bPause == true)
		{
			//�|�[�Y�̍X�V����
			UpdatePause();
		}
	}

	if (pPause->bPause == false)
	{
		//�v���C���[�̍X�V
		UpdatePlayer();

		//�^�C���̍X�V
		UpdateTime();

		UpdateEnemy();

		UpdateEffect();

		UpdateParticle();

		UpdateCamera();

		UpdateFog();
	}

	

	//if (g_gameState == GAMESTATE_END)
	//{//���������낤���ɍs����
		switch (g_gameState)
		{
		case GAMESTATE_NORMAL:			//�ʏ���

			if (pPlayer->pos.z > GOAL_DIST)
			{//�S�[������
				g_gameState = GAMESTATE_END;
			}

			break;

		case GAMESTATE_END:				//�I�����

			//��ԊǗ��̃J�E���^�[������
			g_nCounterGameState--;

			if (g_nCounterGameState <= 0)
			{//0�ɂȂ����ꍇ
				g_gameState = GAMESTATE_NONE;				//�������Ă��Ȃ���Ԃɐݒ�

				//�t�F�[�h�ݒ�(�X�e�[�W�Z���N�g��ʂɈڍs)
				SetFade(MODE_RESULT);

				////�����L���O�̐ݒ�
				//SetRanking(GetScore());
			}
			break;
		}
	//}
}

//========================================================================
//�Q�[����ʂ̕`�揈��
//========================================================================
void DrawGame(void)
{
	//�|�[�Y���̎擾
	Pause *pPause = GetPause();

	//�J�����̃Z�b�g
	SetCamera(0);

	DrawDome();
	//���b�V���t�B�[���h�̕`�揈��
	DrawMeshField();
	DrawField();

	//�G�t�F�N�g�̕`�揈��
	DrawEffect();

	//�p�[�e�B�N���̕`�揈��
	DrawParticle();

	//�v���C���[�̕`��
	DrawPlayer();

	DrawEnemy();

	//�I�u�W�F�N�g�̕`��
	DrawObject();

	//�^�C���̕`��
	DrawTime();

	//�t�H�O�̕`��
	DrawFog();
	
	if (pPause->bPause == true)
	{//�|�[�Y���������ꍇ
		//�|�[�Y�̕`�揈��
		DrawPause();
	}
}

//========================================================================
//�Q�[����Ԃ̐ݒ�
//========================================================================
void SetGameState(GAMESTATE state, int nCounter)
{
	g_gameState = state;

	g_nCounterGameState = nCounter;
}

//========================================================================
//�Q�[���̏�Ԃ̎擾
//========================================================================
GAMESTATE GetGameState(void)
{
	return g_gameState;
}
