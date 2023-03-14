//===========================================================================================
//
//[game.cpp]
//Author:���}���@�V
//
//===========================================================================================
#include "input.h"
#include "effect.h"
#include "particle.h"
#include "particle.h"
#include "time.h"
#include "game.h"
#include "fade.h"
#include "pause.h"
#include "sound.h"

//*****************************
// �v���g�^�C�v�錾
//*****************************
//void SlowUpdate(void);

//*****************************
//�O���[�o���錾
//*****************************
LPDIRECT3D9 g_pDGame = NULL;						//Directx3D�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECT3DDEVICE9 g_pD3DDeviceGame = NULL;			//Directx3D�ւ̃f�o�C�X�ւ̃|�C���^
LPD3DXFONT g_pFontGame = NULL;						//�t�H���g�ւ̃|�C���^
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

	//�J�����̏���������
	//InitCamera();

	//�G�t�F�N�g�̏���������
	InitEffect();

	//�p�[�e�B�N���̏���������
	InitParticle();

	//�|�[�Y�̏���������
	InitPause();

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

	//�f�o�b�N�\���p�t�H���g�̐���
	D3DXCreateFont(pDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &g_pFontGame);
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

	//�f�o�b�N�\���p�t�H���g�̔j��
	if (g_pFontGame != NULL)
	{
		g_pFontGame->Release();
		g_pFontGame = NULL;
	}
}

//========================================================================
//�Q�[����ʂ̍X�V����
//========================================================================
void UpdateGame(void)
{
	//�|�[�Y���̎擾
	Pause *pPause = GetPause();
	Fade pFade = GetFade();

	if (pFade == FADE_NONE)
	{
		if (GetKeyboardTrigger(DIK_P) == true || GetJoyPadTrigger(BUTTON_START, 0) == true)
		{//�|�[�Y����
			pPause->bPause = pPause->bPause ? false : true;
		}

		if (pPause->bPause == true)
		{
			UpdatePause();
		}


		if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoyPadTrigger(BUTTON_A, 0) == true)
		{//����L�[(ENTER�L�[)�������ꂽ
		 //���[�h�̐ݒ�(�Q�[����ʂɈڍs)
			SetFade(MODE_RESULT);
		}
	}

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
