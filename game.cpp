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
#include "camera.h"
#include "light.h"

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

	//�ʏ��Ԃ�
	g_gameState = GAMESTATE_NORMAL;

	//��ԊǗ��̃J�E���^�[��������
	g_nCounterGameState = 0;

	//�|�[�Y���̎擾
	Pause *pPause = GetPause();

	//�|�[�Y��Ԃ�ON/OFF
	pPause->bPause = false;

	//�T�E���h�̍Đ�
	//PlaySound(SOUND_LABEL_BGM000);
}
//========================================================================
//�Q�[����ʂ̏I������
//========================================================================
void UninitGame(void)
{
	//�T�E���h�̒�~
	//StopSound();

	//�G�t�F�N�g�̏I������
	UninitEffect();

	//�p�[�e�B�N���̏I������
	UninitParticle();

	//�|�[�Y�̏I������
	UninitPause();

	//���b�V���t�B�[���h�̏I������
	UninitMeshField();
}

//========================================================================
//�Q�[����ʂ̍X�V����
//========================================================================
void UpdateGame(void)
{
	//���̎擾
	Pause *pPause = GetPause();			//�|�[�Y
	Fade pFade = GetFade();				//�t�F�[�h

	if (pFade == FADE_NONE)
	{//�t�F�[�h��Ԃ���Ȃ��ꍇ

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

		//�Q�[�����񂷂���(��ŏ���)
		if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoyPadTrigger(BUTTON_A, 0) == true)
		{//����L�[(ENTER�L�[)�������ꂽ
			//���[�h�̐ݒ�(�Q�[����ʂɈڍs)
			SetFade(MODE_RESULT);
		}
	}

	UpdateCamera();

	//if (g_gameState == GAMESTATE_END)
	//{//���������낤���ɍs����
		switch (g_gameState)
		{
		case GAMESTATE_NORMAL:			//�ʏ���
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

	//���b�V���t�B�[���h�̕`�揈��
	DrawMeshField();

	//�G�t�F�N�g�̕`�揈��
	DrawEffect();

	//�p�[�e�B�N���̕`�揈��
	DrawParticle();
	
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
