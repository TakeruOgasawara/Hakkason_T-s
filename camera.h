//====================================================================
//
// �J�����̏���[camera.h]
// Author: ���}���V
//
//====================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//***********************************************************
//�}�N����`
//***********************************************************
#define MAX_CAMERA			(1)			//�J�����̍ő吔

//************************************************************
//�J�����\���̂̒�`
//************************************************************
typedef struct
{
	D3DXVECTOR3 posV;			//���_
	D3DXVECTOR3 posR;			//�����_
	D3DXVECTOR3 posVDest;		//�ړI�̎��_
	D3DXVECTOR3 posRDest;		//�ړI�̒����_
	D3DXVECTOR3 vecU;			//������x�N�g��
	D3DXMATRIX mtxProjection;	//�v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX mtxView;			//�r���[�}�g���b�N�X
	D3DXVECTOR3 rot;			//����
	D3DVIEWPORT9 viewport;		//�r���[�|�[�g
	bool bUse;					//�g���Ă��邩�ǂ���

	float fRoll;				 //�J�����̃��[���p�x
	float fLengthCamera;		//����
	bool bFollow;				//�Ǐ]������ON/OFF
} Camera;

//************************************************************
//�v���g�^�C�v�錾
//************************************************************
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);

void SetCamera(int nIdx);

void FollowPlayer(void);

Camera *GetCamera(void);


#endif
