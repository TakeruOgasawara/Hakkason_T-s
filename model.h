//========================================
//
//���f���Ǘ�(model.h)
//Author:���R����
//
//========================================
#ifndef _MODEL_H_							//���̃}�N����`������Ă��Ȃ�������
#define _MODEL_H_							//2�d�C���N���[�h�h�~�̃}�N�����`����v

#include"main.h"

//*****************************************
// �}�N����`
//*****************************************
#define NUM_TEX	(10)	//�e�N�X�`���̐�
#define MAX_PART	(24)	//�p�[�c�̍ő吔

//*****************************************
// �\���̒�`
//*****************************************
typedef struct
{
	LPD3DXMESH pMesh;	//���b�V��(���_���)�ւ̃|�C���^
	LPD3DXBUFFER pBuffMat;	//�}�e���A���ւ̃|�C���^
	DWORD dwNumMat;	//�}�e���A���̐�
	LPDIRECT3DTEXTURE9 apTexture[NUM_TEX];	//�e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3 vtxMin;	//�ŏ����_���W
	D3DXVECTOR3 vtxMax;	//�ő咸�_���W
}Model;

//*****************************************
// �v���g�^�C�v�錾
//*****************************************
//��{����
void InitModel(void);
void UninitModel(void);
void UninitXFile(Model *pModel);

//�ǂݍ��ݏ���
void LoadModel(Model *pModel, const char *pFilePass);	//�ėp�ǂݍ��ݏ���
void LoadAllModel(void);	//�ǂݍ��ݏ����܂Ƃ�
void LoadPlayermodel(void);

//���擾
Model *GetplayerModel(void);

int GetNumPlayerPart(void);

#endif