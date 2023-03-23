//======================================================================================
//
// �J�����̏���[camera.cpp]
// Author;���}���@�V
//
//======================================================================================
#include <stdio.h>
#include "camera.h"
#include "input.h"
#include "debugproc.h"
#include "player.h"

//*****************************
// �}�N����`
//*****************************
#define CAMERA_SPEED		(2.5f)			//�J�����̑��x
#define CAMERA_DISTANCE		(100.0f)		//���_�ƒ����_�̋���
#define CAMELA_AUTO			(0.0001f)		//�J���������ɉ�鎞�̊���
#define CAMERA_ROTATE		(0.04f)			//�J�����̉�]���x

#define TITLE_CAMERA_SISTANCE		(1200.0f)

//*****************************
// �v���g�^�C�v�錾
//*****************************
void CameraOps(void);			//�J��������̏���
void CameraMove(int nCnt);			//�J�����̈ړ�
void CameraRot(int nCnt);
void CameraV(int nCnt);				//���_����̏���
void CameraR(int nCnt);				//�����_����̏���
void CameraRoll(int nCnt);			//�J�����̃��[������

//*****************************
// �O���[�o���錾
//*****************************
Camera g_aCamera[MAX_CAMERA];			//�J�������

//====================================================================
// �J�����̏���������
//====================================================================
void InitCamera(void)
{
	//�ϐ��̏�����
	for (int nCntCamera = 0; nCntCamera < MAX_CAMERA; nCntCamera++)
	{
		g_aCamera[nCntCamera].posV	= D3DXVECTOR3(0.0f, 100.0f, 0.0f);
		g_aCamera[nCntCamera].posR	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aCamera[nCntCamera].vecU	= D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		
		g_aCamera[nCntCamera].rot	= D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
		g_aCamera[nCntCamera].fRoll = 0.0f;
		g_aCamera[nCntCamera].fLengthCamera	= CAMERA_DISTANCE;						//�����_���王�_�Ԃ̋���
		g_aCamera[nCntCamera].bUse	= false;
	}

	g_aCamera[0].viewport.X = 0;				// �`�悷���ʂ̍���X���W
	g_aCamera[0].viewport.Y = 0;				// �`�悷���ʂ̍���Y���W
	g_aCamera[0].viewport.Width = 1280;			// ��ʂ̕�
	g_aCamera[0].viewport.Height = 720;			// ��ʂ̍���
	g_aCamera[0].viewport.MinZ = 0.0f;			// 
	g_aCamera[0].viewport.MaxZ = 1.0f;			// 
}

//====================================================================
//�J�����̏I����������
//====================================================================
void UninitCamera(void)
{

}

//====================================================================
//�J�����̍X�V����
//====================================================================
void UpdateCamera(void)
{
	//�J�����̑��삪�����Ă�֐�
	CameraOps();

	//�v���C���[�Ǐ]����
	//FollowPlayer();
}

//====================================================================
//�J�����̑���
//====================================================================
void CameraOps(void)
{
	for (int nCnt = 0; nCnt < MAX_CAMERA; nCnt++)
	{
		//�J�����̈ړ�
		CameraMove(nCnt);

		//���_����
		CameraV(nCnt);

		//�����_�ړ�
		CameraR(nCnt);

		//�J�����̌����C��
		CameraRot(nCnt);

		//�J�����̃��[��
		CameraRoll(nCnt);
	}
}

//====================================================================
// �v���C���[�̒Ǐ]
//====================================================================
void FollowPlayer(void)
{
	//���擾
	Player *pPlayer = GetPlayer();

	for (int nCnt = 0; nCnt < MAX_CAMERA; nCnt++)
	{
		//�v���C���[�̈ʒu�𒍎��_�ɐݒ�
		g_aCamera[nCnt].posR = pPlayer->pos;

		//���_�̑��Έʒu�ݒ�
		g_aCamera[nCnt].posV.x = g_aCamera[nCnt].posR.x;
		g_aCamera[nCnt].posV.y = g_aCamera[nCnt].posR.y + 50.0f;			//���_Y
		g_aCamera[nCnt].posV.z = g_aCamera[nCnt].posR.z + g_aCamera[nCnt].fLengthCamera;
	}
}

//--------------------------------------------------------------------
// �J�����̌����C��
//--------------------------------------------------------------------
void CameraRot(int nCnt)
{
	//������3.14�ȏ�-3.14�ȉ��ɍs���Ȃ��悤�ɂ���
	//y
	if (g_aCamera[nCnt].rot.y > D3DX_PI)
	{
		g_aCamera[nCnt].rot.y = -D3DX_PI;
	}
	if (g_aCamera[nCnt].rot.y < -D3DX_PI)
	{
		g_aCamera[nCnt].rot.y = D3DX_PI;
	}
	//z
	if (g_aCamera[nCnt].rot.z > D3DX_PI)
	{
		g_aCamera[nCnt].rot.z = -D3DX_PI;
	}
	if (g_aCamera[nCnt].rot.z < -D3DX_PI)
	{
		g_aCamera[nCnt].rot.z = D3DX_PI;
	}
	//x
	if (g_aCamera[nCnt].rot.x > D3DX_PI)
	{
		g_aCamera[nCnt].rot.x = -D3DX_PI;
	}
	if (g_aCamera[nCnt].rot.x < -D3DX_PI)
	{
		g_aCamera[nCnt].rot.x = D3DX_PI;
	}
}

//--------------------------------------------------------------------
//�J�����̈ړ�
//--------------------------------------------------------------------
void CameraMove(int nCnt)
{
	if (g_aCamera[nCnt].bUse == true)
	{
		//�������ړ�
		if (GetKeyboardPress(DIK_A) == true)
		{//A�L�[�������ꂽ
			if (GetKeyboardPress(DIK_W) == true)
			{//����ړ�
				g_aCamera[nCnt].posV.x += sinf(g_aCamera[nCnt].rot.y + -D3DX_PI * 0.25f) * CAMERA_SPEED;
				g_aCamera[nCnt].posR.x += sinf(g_aCamera[nCnt].rot.y + -D3DX_PI * 0.25f) * CAMERA_SPEED;
				g_aCamera[nCnt].posV.z += cosf(g_aCamera[nCnt].rot.y + -D3DX_PI * 0.25f) * CAMERA_SPEED;
				g_aCamera[nCnt].posR.z += cosf(g_aCamera[nCnt].rot.y + -D3DX_PI * 0.25f) * CAMERA_SPEED;
			}
			else if (GetKeyboardPress(DIK_S) == true)
			{//�����ړ�
				g_aCamera[nCnt].posV.x += sinf(g_aCamera[nCnt].rot.y + -D3DX_PI * 0.75f) * CAMERA_SPEED;
				g_aCamera[nCnt].posR.x += sinf(g_aCamera[nCnt].rot.y + -D3DX_PI * 0.75f) * CAMERA_SPEED;
				g_aCamera[nCnt].posV.z += cosf(g_aCamera[nCnt].rot.y + -D3DX_PI * 0.75f) * CAMERA_SPEED;
				g_aCamera[nCnt].posR.z += cosf(g_aCamera[nCnt].rot.y + -D3DX_PI * 0.75f) * CAMERA_SPEED;
			}
			else
			{//���ړ�
				//�J�����ʒu���̍X�V
				g_aCamera[nCnt].posV.x += sinf(g_aCamera[nCnt].rot.y + -D3DX_PI * 0.5f) * CAMERA_SPEED;
				g_aCamera[nCnt].posR.x += sinf(g_aCamera[nCnt].rot.y + -D3DX_PI * 0.5f) * CAMERA_SPEED;
				g_aCamera[nCnt].posV.z += cosf(g_aCamera[nCnt].rot.y + -D3DX_PI * 0.5f) * CAMERA_SPEED;
				g_aCamera[nCnt].posR.z += cosf(g_aCamera[nCnt].rot.y + -D3DX_PI * 0.5f) * CAMERA_SPEED;
			}
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{//D�L�[�������ꂽ
			if (GetKeyboardPress(DIK_W) == true)
			{//�E��ړ�
				g_aCamera[nCnt].posV.x += sinf(g_aCamera[nCnt].rot.y + D3DX_PI * 0.25f) * CAMERA_SPEED;
				g_aCamera[nCnt].posR.x += sinf(g_aCamera[nCnt].rot.y + D3DX_PI * 0.25f) * CAMERA_SPEED;
				g_aCamera[nCnt].posV.z += cosf(g_aCamera[nCnt].rot.y + D3DX_PI * 0.25f) * CAMERA_SPEED;
				g_aCamera[nCnt].posR.z += cosf(g_aCamera[nCnt].rot.y + D3DX_PI * 0.25f) * CAMERA_SPEED;
			}
			else if (GetKeyboardPress(DIK_S) == true)
			{//�E���ړ�
				g_aCamera[nCnt].posV.x += sinf(g_aCamera[nCnt].rot.y + D3DX_PI * 0.75f) * CAMERA_SPEED;
				g_aCamera[nCnt].posR.x += sinf(g_aCamera[nCnt].rot.y + D3DX_PI * 0.75f) * CAMERA_SPEED;
				g_aCamera[nCnt].posV.z += cosf(g_aCamera[nCnt].rot.y + D3DX_PI * 0.75f) * CAMERA_SPEED;
				g_aCamera[nCnt].posR.z += cosf(g_aCamera[nCnt].rot.y + D3DX_PI * 0.75f) * CAMERA_SPEED;
			}
			else
			{//�E�ړ�
				//�J�����ʒu���̍X�V
				g_aCamera[nCnt].posV.x += sinf(g_aCamera[nCnt].rot.y + D3DX_PI * 0.5f) * CAMERA_SPEED;
				g_aCamera[nCnt].posR.x += sinf(g_aCamera[nCnt].rot.y + D3DX_PI * 0.5f) * CAMERA_SPEED;
				g_aCamera[nCnt].posV.z += cosf(g_aCamera[nCnt].rot.y + D3DX_PI * 0.5f) * CAMERA_SPEED;
				g_aCamera[nCnt].posR.z += cosf(g_aCamera[nCnt].rot.y + D3DX_PI * 0.5f) * CAMERA_SPEED;
			}
		}
		else if (GetKeyboardPress(DIK_W) == true)
		{//W�L�[�������ꂽ
			//��ړ�
			g_aCamera[nCnt].posV.x += sinf(g_aCamera[nCnt].rot.y) * CAMERA_SPEED;
			g_aCamera[nCnt].posR.x += sinf(g_aCamera[nCnt].rot.y) * CAMERA_SPEED;
			g_aCamera[nCnt].posV.z += cosf(g_aCamera[nCnt].rot.y) * CAMERA_SPEED;
			g_aCamera[nCnt].posR.z += cosf(g_aCamera[nCnt].rot.y) * CAMERA_SPEED;
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{//S�L�[�������ꂽ
			//���ړ�
			g_aCamera[nCnt].posV.x += sinf(g_aCamera[nCnt].rot.y + D3DX_PI) * CAMERA_SPEED;
			g_aCamera[nCnt].posR.x += sinf(g_aCamera[nCnt].rot.y + D3DX_PI) * CAMERA_SPEED;
			g_aCamera[nCnt].posV.z += cosf(g_aCamera[nCnt].rot.y + D3DX_PI) * CAMERA_SPEED;
			g_aCamera[nCnt].posR.z += cosf(g_aCamera[nCnt].rot.y + D3DX_PI) * CAMERA_SPEED;
		}

		if (GetKeyboardPress(DIK_LSHIFT) == true)
		{
			g_aCamera[nCnt].posR.y += CAMERA_SPEED;
		}
		if (GetKeyboardPress(DIK_LCONTROL) == true)
		{
			g_aCamera[nCnt].posR.y -= CAMERA_SPEED;
		}
	}
	
}

//--------------------------------------------------------------------
// �J�������_�̈ړ�����
//--------------------------------------------------------------------
void CameraV(int nCnt)
{
	//�c��]
	if (GetKeyboardPress(DIK_Y) == true)
	{//Y�L�[�������ꂽ
		g_aCamera[nCnt].rot.z -= CAMERA_ROTATE;
	}
	else if (GetKeyboardPress(DIK_N) == true)
	{//N�L�[�������ꂽ
		g_aCamera[nCnt].rot.z += CAMERA_ROTATE;
	}

	//����]
	if (GetKeyboardPress(DIK_Z) == true)
	{//Z�L�[�������ꂽ
		g_aCamera[nCnt].rot.y += CAMERA_ROTATE;
	}
	else if (GetKeyboardPress(DIK_C) == true)
	{//C�L�[�������ꂽ
		g_aCamera[nCnt].rot.y -= CAMERA_ROTATE;
	}

	g_aCamera[nCnt].posV.x = g_aCamera[nCnt].posR.x + (cosf(g_aCamera[nCnt].rot.z) * sinf(g_aCamera[nCnt].rot.y)) * -g_aCamera[nCnt].fLengthCamera;
	g_aCamera[nCnt].posV.y = g_aCamera[nCnt].posR.y + sinf(g_aCamera[nCnt].rot.z) * -g_aCamera[nCnt].fLengthCamera;			//���_Y
	g_aCamera[nCnt].posV.z = g_aCamera[nCnt].posR.z + (cosf(g_aCamera[nCnt].rot.z) * cosf(g_aCamera[nCnt].rot.y)) * -g_aCamera[nCnt].fLengthCamera;
}

//--------------------------------------------------------------------
// �J���������_�̈ړ�����
//--------------------------------------------------------------------
void CameraR(int nCnt)
{
	//�c��]
	if (GetKeyboardPress(DIK_T) == true)
	{//T�L�[�������ꂽ
		g_aCamera[nCnt].rot.z += CAMERA_ROTATE;
	}
	else if (GetKeyboardPress(DIK_B) == true)
	{//B�L�[�������ꂽ
		g_aCamera[nCnt].rot.z -= CAMERA_ROTATE;
	}

	//����]
	if (GetKeyboardPress(DIK_Q) == true || GetJoyPadPress(BUTTON_LB, 0) == true)
	{//Q�L�[�������ꂽ
		g_aCamera[nCnt].rot.y -= CAMERA_ROTATE;
	}
	else if (GetKeyboardPress(DIK_E) == true || GetJoyPadPress(BUTTON_RB, 0) == true)
	{//E�L�[�������ꂽ
		g_aCamera[nCnt].rot.y += CAMERA_ROTATE;
	}

	g_aCamera[nCnt].posR.x = g_aCamera[nCnt].posV.x + (cosf(g_aCamera[nCnt].rot.z) * sinf(g_aCamera[nCnt].rot.y)) * g_aCamera[nCnt].fLengthCamera;
	g_aCamera[nCnt].posR.y = g_aCamera[nCnt].posV.y + sinf(g_aCamera[nCnt].rot.z) * g_aCamera[nCnt].fLengthCamera;			//���_Y
	g_aCamera[nCnt].posR.z = g_aCamera[nCnt].posV.z + (cosf(g_aCamera[nCnt].rot.z) * cosf(g_aCamera[nCnt].rot.y)) * g_aCamera[nCnt].fLengthCamera;
}
//====================================================================
//�J�����̃��[������
//====================================================================
void CameraRoll(int nCnt)
{
	if (GetKeyboardPress(DIK_RIGHT) == true)
	{
		g_aCamera[nCnt].fRoll += 0.1f;
	}
	else if (GetKeyboardPress(DIK_LEFT) == true)
	{
		g_aCamera[nCnt].fRoll -= 0.1f;
	}

	if (g_aCamera[nCnt].fRoll > D3DX_PI)
	{
		g_aCamera[nCnt].fRoll = -D3DX_PI;
	}
	if (g_aCamera[nCnt].fRoll < -D3DX_PI)
	{
		g_aCamera[nCnt].fRoll = D3DX_PI;
	}
}

//====================================================================
//�J�����̐ݒ菈������
//====================================================================
void SetCamera(int nIdx)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_aCamera[nIdx].mtxProjection);

	//�v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH(&g_aCamera[nIdx].mtxProjection,
		D3DXToRadian(45.0f),										//����p
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,					//��ʂ̃A�X�y�N�g��
		10.0f,														//z�l�̍ŏ��l
		10000.0f);													//z�l�̍ő�l

														//�v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &g_aCamera[nIdx].mtxProjection);

	//�r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_aCamera[nIdx].mtxView);

	//�r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&g_aCamera[nIdx].mtxView,
		&g_aCamera[nIdx].posV,						//���_
		&g_aCamera[nIdx].posR,						//�����_
		&g_aCamera[nIdx].vecU);						//���x�N�g��

	//�J�����̃��[���p
	D3DXMATRIX rotationZMatrix;
	D3DXMatrixRotationZ(&rotationZMatrix, g_aCamera[nIdx].fRoll); // Z����]�s��̌v�Z
	D3DXMATRIX viewMatrixWithRotation = rotationZMatrix * g_aCamera[nIdx].mtxView; // Z����]���������r���[�s��

	//�r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &viewMatrixWithRotation);

	//�g���Ă����Ԃ�
	g_aCamera[nIdx].bUse = true;

#ifdef _DEBUG		//�f�o�b�O���̂�
	PrintDebugProc("�y�J�������z\n");
	PrintDebugProc("���_     [%f  %f  %f]\n", g_aCamera[nIdx].posV.x, g_aCamera[nIdx].posV.y, g_aCamera[nIdx].posV.z);
	PrintDebugProc("�����_   [%f  %f  %f]\n", g_aCamera[nIdx].posR.x, g_aCamera[nIdx].posR.y, g_aCamera[nIdx].posR.z);
	PrintDebugProc("����     [%f  %f  %f]\n", g_aCamera[nIdx].rot.x, g_aCamera[nIdx].rot.y, g_aCamera[nIdx].rot.z);
	PrintDebugProc("���[��   [%f]\n", g_aCamera[nIdx].fRoll);
#endif
}

//====================================================================
//�J�����̏��̃|�C���^
//====================================================================
Camera *GetCamera(void)
{
	return &g_aCamera[0];
}