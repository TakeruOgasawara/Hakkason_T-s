//==========================================
//
//  �擾�X�R�A�\������
//  Author : ���R����
//
//==========================================

#ifndef _POINT_H_
#define _POINT_H_

#include "main.h"

//==========================================
//  �}�N����`
//==========================================
#define MAX_POINT_NUM (12) //�擾�X�R�A�\���̍ő吔

//==========================================
//  �񋓌^��`
//==========================================
typedef enum
{//�擾�X�R�A�̏��
	POINTSTATE_IN = 0,	//�X���C�h�C�����
	POINTSTATE_WAIT,	//�ҋ@���
	POINTSTATE_OUT,	//�X���C�h�A�E�g���
	POINTSTATE_MAX
}POINTSTATE;

typedef enum
{//�擾�X�R�A�̎��
	POINTTYPE_SLIDE = 0,	//�X���C�h�C�����Ă���X�R�A�\��
	POINTTYPE_MINITE,	//�\����������X�R�A�\��
	POINTTYPE_SECOND,	//�\����������X�R�A�\��
	POINTTYPE_MAX
}POINTTYPE;

typedef enum
{//���b�Z�[�W�̎��
	LOGTYPE_TOTAL = 0,	//���v
	LOGTYPE_NOW,	//���Ȃ��̃X�R�A
	LOGTYPE_MAX
}LOGTYPE;

//==========================================
//  �\���̒�`
//==========================================
//�擾�X�R�A�����̍\����
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 posDest;	//�ڕW�ʒu
	D3DXCOLOR col;	//�F
	POINTSTATE state;	//���
	POINTTYPE type;	//���
	int nScore;	//�X�R�A�l
	int nCntState;	//��ԑJ�ڃJ�E���^�[
	int nNumPlace;	//����
	float fScale;	//�X�P�[��
	bool bUse;	//�g�p���Ă��邩�ǂ���
}Point;

typedef struct
{//���b�Z�[�W
	D3DXVECTOR3 pos;	//�ʒu
	D3DXCOLOR col;	//�F
	LOGTYPE type;	//���
	Point *pPoint;	//�X�R�A�\���ւ̃|�C���^
	float fScale;	//�X�P�[��
	bool bUse;	//�g�p����Ă��邩�ǂ���
}PointLog;

//==========================================
//  �v���g�^�C�v�錾
//==========================================
void InitPoint(void);
void UninitPoint(void);
void UpdatePoint(void);
void DrawPoint(void);
void SetPointScore(int nPoint,D3DXVECTOR3 pos, D3DXVECTOR3 posDest, POINTTYPE type, float fScale, LOGTYPE logType);
void PushUpScore(int nCntpoint);
Point *GetPoint(void);

//��ނ��Ƃ̍X�V����
void UpdatePointSlide(Point *pPoint);
void UpdatePointStay(Point *pPoint);

#endif