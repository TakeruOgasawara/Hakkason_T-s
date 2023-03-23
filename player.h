//===========================
//
//�_�C���N�gX.player�w�b�_�[
//Author:���R����
//
//===========================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "model.h"
#include <stdio.h>

//*********************
// �񋓌^��`
//*********************
typedef enum
{// �v���C���[�̏��
	PLAYERSTATE_APPEAR = 0,			//�o�����
	PLAYERSTATE_NORMAL,				//�ʏ���
	PLAYERSTATE_MOVE,				//�ړ����
	PLAYERSTATE_DAMAGE,				//�_���[�W���
	PLAYERSTATE_DEATH,				//���S���
	PLAYERSTATE_MAX
}PLAYERSTATE;

//*********************
// �\���̒�`
//*********************
// �p�[�c�̍\����
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld;		//���[���h�}�g���b�N�X
	int nIdx;	//�ԍ�
	int nIdxParent;	//�e�̔ԍ�
}PlayerPart;

// �v���C���[�̍\����
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 posOld;	//�O��̈ʒu
	D3DXVECTOR3 move;	//�ړ���
	D3DXVECTOR3 rot;	//����
	D3DXVECTOR3 rotDest;		//�ڕW�̌���
	D3DXMATRIX mtxWorld;		//���[���h�}�g���b�N�X
	PlayerPart part[MAX_PART];	//�p�[�c
}Player;

//*********************
// �v���g�^�C�v�錾
//*********************
//��{����
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

//���擾
Player *GetPlayer(void);

//�ǂݍ��ݏ���
void LoadMotion(FILE *pFile);

//���쏈��
void ControlPlayerKeyboard(void);
void ControlPlayerPad(void);

//���̑�
void FactingRot(float *pfRot,float fRotDest);

#endif // !_PLAYER_H_