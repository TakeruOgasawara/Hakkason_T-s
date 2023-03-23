//======================================================================================
//
//�L�[�{�[�h���͏���[input.cpp]
//Author;���}���@�V
//
//======================================================================================
#include "input.h"

//*****************************
//�}�N����`
//*****************************
#define NUM_KEY_MAX			(256)			//�L�[�̍ő吔

#define NUM_PLAYER			(4)				//�v���C���[��

//*****************************
//�O���[�o����`
//*****************************
LPDIRECTINPUT8 g_pInput = NULL;						//Directinput�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;			//���̓f�o�C�X(�L�[�{�[�h)�ւ̃|�C���^

BYTE g_aKeyState[NUM_KEY_MAX];						//�L�[�{�[�h�v���X�̏��
BYTE g_aKeyStateTrigger[NUM_KEY_MAX];				//�L�[�{�[�h�g���K�[�̏��
BYTE g_aKeyStateRipeat[NUM_KEY_MAX];				//�L�[�{�[�h���s�[�g�̏��
BYTE g_aKeyStateRelease[NUM_KEY_MAX];				//�L�[�{�[�h�����[�X�̏��

//�Q�[���p�b�h
XINPUT_STATE g_aGamePadState[NUM_PLAYER];						//�Q�[���p�b�h�v���X�̏��
XINPUT_STATE g_aGamePadStateTrigger[NUM_PLAYER];				//�Q�[���p�b�h�g���K�[�̏��
XINPUT_STATE g_aGamePadStateRelease[NUM_PLAYER];				//�Q�[���p�b�h�����[�X�̏��
XINPUT_STATE g_aGamePadStateRepeat[NUM_PLAYER];					//�Q�[���p�b�h


//========================================================================
//�f�o�C�X�̏I������
//========================================================================
void UninitDevice(void)
{
	//�L�[�{�[�h�̏���������
	UninitKeyboard();

	//�W���C�p�b�h�̏I������
	UninitJoyPad();
}

//========================================================================
//�f�o�C�X�̍X�V����
//========================================================================
void UpdateDevice(void)
{
	//�L�[�{�[�h�̍X�V����
	UpdateKeyboard();

	//�W���C�p�b�h�̍X�V����
	UpdateJoyPad();
}

//========================================================================
//�L�[�{�[�h�̏�����
//========================================================================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInput�I�u�W�F�N�g�̏�����
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pInput, NULL)))
	{
		return E_FAIL;
	}
	//���̓f�o�C�X(�L�[�{�[�h)�̐���
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}
	//�f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}
	//�������[�h��ݒ�
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}
	//�L�[�{�[�h�ւ̃A�N�Z�X�����l��
	g_pDevKeyboard->Acquire();

	return S_OK;
}

//========================================================================
//�L�[�{�[�h�I������
//========================================================================
void UninitKeyboard(void)
{
	//���̓f�o�C�X(�L�[�{�[�h)�̔j��
	if (g_pDevKeyboard != NULL)
	{
		g_pDevKeyboard->Unacquire();			//�L�[�{�[�h�ւ̃A�N�Z�X�������
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	//Directinput�I�u�W�F�N�g�̔j��
	if (g_pInput != NULL)
	{
		g_pInput->Release();
		g_pInput = NULL;
	}
}

//========================================================================
//�L�[�{�[�h�X�V����
//========================================================================
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];			//�L�[�{�[�h�̓��͏��
	int nCntKey;

	//���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			g_aKeyStateRipeat[nCntKey] = (g_aKeyState[nCntKey] & aKeyState[nCntKey]);		//�L�[�{�[�h�̃��s�[�g����ۑ�

			g_aKeyStateRelease[nCntKey] = (g_aKeyState[nCntKey] | aKeyState[nCntKey]) ^ aKeyState[nCntKey];		//�L�[�{�[�h�̃����[�X����ۑ�

			g_aKeyStateTrigger[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];		//�L�[�{�[�h�̃g���K�[����ۑ�

			g_aKeyState[nCntKey] = aKeyState[nCntKey];			//�L�[�{�[�h�̃v���X����ۑ�
		}
	}
	else
	{
		g_pDevKeyboard->Acquire();				//�L�[�{�[�h�ւ̃A�N�Z�X�����l��
	}
}

//========================================================================
//�L�[�{�[�h�̃v���X�����擾
//========================================================================
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true : false;
}

//========================================================================
//�L�[�{�[�h�̃g���K�[�����擾
//========================================================================
bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//========================================================================
//�L�[�{�[�h�̃��s�[�g�����擾
//========================================================================
bool GetKeyboardRipeat(int nKey)
{
	return (g_aKeyStateRipeat[nKey] & 0x80) ? true : false;
}

//========================================================================
//�L�[�{�[�h�̃����[�X�����擾
//========================================================================
bool GetKeyboardRelease(int nKey)
{
	return (g_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//========================================================================
//�Q�[���p�b�h����������
//========================================================================
HRESULT InitGamePad(void)
{
	//�ϐ��錾
	int nCntPad;

	//XInput�̃X�e�[�g(�g�����)
	XInputEnable(true);

	//�������̏�����(�v���C���[��)
	for (nCntPad = 0; nCntPad < NUM_PLAYER; nCntPad++)
	{
		memset(&g_aGamePadState[nCntPad], 0, sizeof(XINPUT_STATE));
		memset(&g_aGamePadStateTrigger[nCntPad], 0, sizeof(XINPUT_STATE));
		memset(&g_aGamePadStateRelease[nCntPad], 0, sizeof(XINPUT_STATE));
	}

	//�Ȃ��Ƃł��Ȃ�
	return S_OK;
}

//========================================================================
//�Q�[���p�b�h�̏I������
//========================================================================
void UninitJoyPad(void)
{
	//XInput�̃X�e�[�g(�g��Ȃ����)
	XInputEnable(false);
}

//========================================================================
//�Q�[���p�b�h�̍X�V����
//========================================================================
void UpdateJoyPad(void)
{
	//�ϐ��錾
	int nCntPad;

	XINPUT_STATE aGamePadState;			//�Q�[���p�b�h�̓��͏��

	for (nCntPad = 0; nCntPad < NUM_PLAYER; nCntPad++)
	{
		//���̓f�o�C�X����f�[�^���擾
		if (XInputGetState(nCntPad, &aGamePadState) == ERROR_SUCCESS)
		{
			//�Q�[���p�b�h�̃g���K�[����ۑ�
			g_aGamePadStateTrigger[nCntPad].Gamepad.wButtons = 
				(g_aGamePadState[nCntPad].Gamepad.wButtons ^ aGamePadState.Gamepad.wButtons) & aGamePadState.Gamepad.wButtons;

			//���s�[�g���
			g_aGamePadStateRepeat[nCntPad].Gamepad.wButtons =
				(g_aGamePadState[nCntPad].Gamepad.wButtons & aGamePadState.Gamepad.wButtons);

			//�����[�X���
			g_aGamePadStateRelease[nCntPad].Gamepad.wButtons =
				(g_aGamePadState[nCntPad].Gamepad.wButtons | aGamePadState.Gamepad.wButtons) ^ aGamePadState.Gamepad.wButtons;

			//�Q�[���p�b�h�̃v���X����ۑ�
			g_aGamePadState[nCntPad] = aGamePadState;
		}
	}
}

//===================================================================
//�W���C�X�e�B�b�N���擾
//===================================================================
float GetJoyStickLX(int nPlayer)
{
	//����Ԃ�
	return (float)g_aGamePadState[nPlayer].Gamepad.sThumbLX / SHRT_MAX;
}

//===================================================================
//�W���C�X�e�B�b�N���擾
//===================================================================
float GetJoyStickLY(int nPlayer)
{
	//����Ԃ�
	return (float)g_aGamePadState[nPlayer].Gamepad.sThumbLY / SHRT_MAX;
}

//===================================================================
//�W���C�X�e�B�b�N���擾
//===================================================================
float GetJoyStickRX(int nPlayer)
{
	//����Ԃ�
	return (float)g_aGamePadState[nPlayer].Gamepad.sThumbRX / SHRT_MAX;
}

//===================================================================
//�W���C�X�e�B�b�N���擾
//===================================================================
float GetJoyStickRY(int nPlayer)
{
	//����Ԃ�
	return (float)g_aGamePadState[nPlayer].Gamepad.sThumbRY / SHRT_MAX;
}

//========================================================================
//�Q�[���p�b�h�̃v���X�����擾
//========================================================================
bool GetJoyPadPress(JOYKEY nKey, int nPlayer)
{
	return (g_aGamePadState[nPlayer].Gamepad.wButtons & 0x01 << nKey) ? true : false;
}

//========================================================================
//�Q�[���p�b�h�̃g���K�[�����擾
//========================================================================
bool GetJoyPadTrigger(JOYKEY nKey, int nPlayer)
{
	return (g_aGamePadStateTrigger[nPlayer].Gamepad.wButtons & 0x01 << nKey) ? true : false;
}

//========================================================================
//�Q�[���p�b�h�̃����[�X�����擾
//========================================================================
bool GetGamePadRelease(JOYKEY nKey, int nPlayer)
{
	return (g_aGamePadStateRelease[nPlayer].Gamepad.wButtons & 0x01 << nKey) ? true : false;
}

//===================================================================
//���s�[�g���擾
//===================================================================
bool GetJoyPadRepeat(JOYKEY nKey, int nPlayer)
{
	//�O�����Z�q
	return(g_aGamePadStateRepeat[nPlayer].Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}
