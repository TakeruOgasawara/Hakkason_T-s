//==========================================
//
//  �擾�X�R�A�\������
//  Author : ���R����
//
//==========================================
#include "point.h"
#include "input.h"
#include "point_log.h"

//==========================================
//  �}�N����`
//==========================================
#define MAX_NUM_PLACE (2) //�X�R�A�̌���
#define POINT_WIDTH (40.0f) //�X�R�A�̉���
#define POINT_HEIGHT (60.0f) //�X�R�A�̍���
#define WAIT_RANGE	(0.1f)	//�ړ������Ƃ݂Ȃ������Ƃ̒�������
#define OUT_TIME	(90)	//�ҋ@��Ԃ���J�ڂ���܂ł̎���
#define ALPHA_DEC_TIME	(0.04f)	//�����ɂȂ鑬�x
#define MOVE_SPEED	(5.0f)	//�ړ����x

//==========================================
//  �O���[�o���ϐ��錾
//==========================================
LPDIRECT3DTEXTURE9 g_pTexturePoint = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPoint = NULL;
Point g_aPoint[MAX_POINT_NUM];

//==========================================
//  ����������
//==========================================
void InitPoint()
{
	//�ϐ��̏�����
	ZeroMemory(&g_aPoint[0],sizeof(Point) * MAX_POINT_NUM);

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_NUM_PLACE * MAX_POINT_NUM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPoint,
		NULL
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data/TEXTURE/UI/Number000.png",
		&g_pTexturePoint
	);

	//���_�o�b�t�@�̌Ăяo��
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffPoint->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_NUM_PLACE * MAX_POINT_NUM; nCnt++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			//rhw�̐ݒ�
			pVtx[nCnt].rhw = 1.0f;

			//���_�J���[�̐ݒ�
			pVtx[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		//���_�f�[�^��i�߂�
		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffPoint->Unlock();
}

//==========================================
//  �I������
//==========================================
void UninitPoint()
{
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffPoint != NULL)
	{
		g_pVtxBuffPoint->Release();
		g_pVtxBuffPoint = NULL;
	}

	//�e�N�X�`���̔j��
	if (g_pTexturePoint != NULL)
	{
		g_pTexturePoint->Release();
		g_pTexturePoint = NULL;
	}
}

//==========================================
//  �X�V����
//==========================================
void UpdatePoint()
{
	//���擾
	Point *pPoint = GetPoint();

	for (int nCntPoint = 0; nCntPoint < MAX_POINT_NUM; nCntPoint++, pPoint++)
	{
		if (pPoint->bUse)
		{//�g�p���Ă����ԂȂ�

			switch (pPoint->type)
			{//��ނ��Ƃ̍X�V����
			case POINTTYPE_SLIDE:
				UpdatePointSlide(pPoint);
				break;
			case POINTTYPE_SECOND:
			case POINTTYPE_MINITE:
				UpdatePointStay(pPoint);
				break;
			default:
				break;
			}

			//���_���X�V=========================================================
			//���_�o�b�t�@�̌Ăяo��
			VERTEX_2D *pVtx;

			//���_�o�b�t�@�����b�N
			g_pVtxBuffPoint->Lock(0, 0, (void**)&pVtx, 0);

			//�ڕW�ʒu�܂Ń|�C���^��i�߂�
			pVtx += 4 * nCntPoint * MAX_NUM_PLACE;

			//���_���ݒ�
			for (int nCntNum = 0; nCntNum < MAX_NUM_PLACE; nCntNum++)
			{
				//���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(pPoint->pos.x + POINT_HEIGHT * pPoint->fScale * nCntNum, pPoint->pos.y, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(pPoint->pos.x + POINT_HEIGHT * pPoint->fScale * (nCntNum + 1), pPoint->pos.y, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(pPoint->pos.x + POINT_HEIGHT * pPoint->fScale * nCntNum, pPoint->pos.y + POINT_HEIGHT * pPoint->fScale * 2, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pPoint->pos.x + POINT_HEIGHT * pPoint->fScale * (nCntNum + 1), pPoint->pos.y + POINT_HEIGHT * pPoint->fScale * 2, 0.0f);

				pVtx[0].col = pPoint->col;
				pVtx[1].col = pPoint->col;
				pVtx[2].col = pPoint->col;
				pVtx[3].col = pPoint->col;

				pVtx += 4; //���_�f�[�^�̃|�C���^��4�i�߂�
			}

			//���_�o�b�t�@���A�����b�N
			g_pVtxBuffPoint->Unlock();
			//���_���X�V=========================================================
		}
	}
}

//==========================================
//  �X���C�h�^�C�v�̍X�V
//==========================================
void UpdatePointSlide(Point *pPoint)
{
	//�ϐ��錾
	D3DXVECTOR3 posDiff;
	float fLengthDiff;

	//�ڕW�Ƃ̍����Z�o
	posDiff = pPoint->posDest - pPoint->pos;

	//�����̒����Z�o
	fLengthDiff = D3DXVec3Length(&posDiff);

	//���K��
	D3DXVec3Normalize(&posDiff, &posDiff);

	//�ړ��ʉ��Z
	for (int nCntPos = 0; nCntPos < 3; nCntPos++)
	{//XYZ�̎O����for��
		if (posDiff[nCntPos] >= 0.01f || posDiff[nCntPos] <= -0.01f)
		{//���������Z����
			pPoint->pos[nCntPos] += posDiff[nCntPos] * MOVE_SPEED;
		}
	}

	switch (pPoint->state)
	{//��Ԃ��Ƃ̍X�V
	case POINTSTATE_IN:

		if (pPoint->col.a < 1.0f)
		{//�s�����ɂ���
			pPoint->col.a += ALPHA_DEC_TIME;
		}
		else
		{//1.0f�ɌŒ�
			pPoint->col.a = 1.0f;
		}

		if (fLengthDiff <= WAIT_RANGE)
		{//�ҋ@��Ԃֈڍs
			pPoint->state = POINTSTATE_WAIT;
		}

		break;
	case POINTSTATE_WAIT:

		if (pPoint->nCntState <= OUT_TIME)
		{
			//�J�ڃJ�E���^�[�㏸
			pPoint->nCntState++;
		}
		else
		{//�X���C�h�A�E�g��Ԃ֑J��
			pPoint->nCntState = 0;
			pPoint->state = POINTSTATE_OUT;
		}

		break;
	case POINTSTATE_OUT:

		if (pPoint->col.a > 0.0f)
		{//�����ɂ���
			pPoint->col.a -= ALPHA_DEC_TIME;
		}
		else
		{//����
			pPoint->col.a = 0.0f;
			pPoint->bUse = false;
		}

		break;
	default:
		break;
	}
}

//==========================================
//  �X�e�C�^�C�v�̍X�V
//==========================================
void UpdatePointStay(Point *pPoint)
{

}

//==========================================
//  �`�揈��
//==========================================
void DrawPoint()
{
	//���擾
	Point *pPoint = GetPoint();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPoint, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntPoint = 0;nCntPoint < MAX_POINT_NUM;nCntPoint++, pPoint++)
	{
		if (pPoint->bUse)
		{//�g�p���Ă����ԂȂ�
			for (int nCnt = 0; nCnt < MAX_NUM_PLACE; nCnt++)
			{//�������`��
				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_pTexturePoint);

				//�`��
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCnt + 4 * nCntPoint * MAX_NUM_PLACE, 2);
			}
		}
	}
}

//==========================================
//  �ݒ菈��
//==========================================
void SetPointScore(int nPoint, D3DXVECTOR3 pos, D3DXVECTOR3 posDest, POINTTYPE type, float fScale, LOGTYPE logType)
{
	//���[�J���ϐ��錾
	int aTex[MAX_NUM_PLACE];
	int nCntNum;

	//�v�Z�p�ϐ�
	int nCalc = nPoint;

	//���擾
	Point *pPoint = GetPoint();

	for (int nCntPoint = 0; nCntPoint < MAX_POINT_NUM; nCntPoint++, pPoint++)
	{
		if (pPoint->bUse == false)
		{//�g�p����Ă��Ȃ���ԂȂ�
			//�����󂯎��
			pPoint->pos = pos;
			pPoint->posDest = posDest;
			pPoint->type = type;
			pPoint->fScale = fScale;

			if (nPoint >= 0)
			{//���Z�Ȃ�
				pPoint->col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
			}
			else
			{//���Z�Ȃ�
				pPoint->col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

				//�}�␳
				nCalc *= -1;
			}

			if (pPoint->type == POINTTYPE_SLIDE)
			{//�X���C�h�C������Ȃ�
				//�����𓧖��ɂ���
				pPoint->col.a = 0.0f;
			}
			
			if (pPoint->type == POINTTYPE_MINITE)
			{//����������
				//���b�Z�[�W��\��
				SetPointLog(pPoint, logType, fScale);
			}

			//�e�N�X�`�����W�̌v��
			for (int nCntPoint = MAX_NUM_PLACE - 1; nCntPoint >= 0; nCntPoint--)
			{
				aTex[nCntPoint] = nCalc % 10;
				nCalc /= 10;

				if (aTex[nCntPoint] != 0)
				{//�����ۑ�
					pPoint->nNumPlace = MAX_NUM_PLACE - nCntPoint;
				}
			}

			//���_�o�b�t�@�̌Ăяo��
			VERTEX_2D *pVtx;

			//���_�o�b�t�@�����b�N
			g_pVtxBuffPoint->Lock(0, 0, (void**)&pVtx, 0);

			//�ڕW�ʒu�܂Ń|�C���^��i�߂�
			pVtx += 4 * nCntPoint * MAX_NUM_PLACE;

			//�ݒ菈��
			for (nCntNum = 0; nCntNum < MAX_NUM_PLACE; nCntNum++)
			{
				//���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(pPoint->pos.x + POINT_HEIGHT * pPoint->fScale * nCntNum, pPoint->pos.y, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(pPoint->pos.x + POINT_HEIGHT * pPoint->fScale * (nCntNum + 1), pPoint->pos.y, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(pPoint->pos.x + POINT_HEIGHT * pPoint->fScale * nCntNum, pPoint->pos.y + POINT_HEIGHT * pPoint->fScale * 2, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pPoint->pos.x + POINT_HEIGHT * pPoint->fScale * (nCntNum + 1), pPoint->pos.y + POINT_HEIGHT * pPoint->fScale * 2, 0.0f);

				//�e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2(aTex[nCntNum] * 0.1f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2((aTex[nCntNum] * 0.1f) + 0.1f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(aTex[nCntNum] * 0.1f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2((aTex[nCntNum] * 0.1f) + 0.1f, 1.0f);

				pVtx += 4; //���_�f�[�^�̃|�C���^��4�i�߂�
			}

			//���_�o�b�t�@���A�����b�N
			g_pVtxBuffPoint->Unlock();

			//�g�p��Ԃɂ���
			pPoint->bUse = true;

			//�X���C�h�C����Ԃɂ���
			pPoint->state = POINTSTATE_IN;

			//���̃X�R�A�����グ����
			PushUpScore(nCntPoint);

			break;
		}
	}
}

//==========================================
//  ���ɂ���X�R�A�������グ�鏈��
//==========================================
void PushUpScore(int nCntpoint)
{
	//���擾
	Point *pPoint = GetPoint();

	for (int nCntScore = 0; nCntScore < MAX_POINT_NUM; nCntScore++,pPoint++)
	{
		if (pPoint->bUse || nCntpoint != nCntScore)
		{//�V�����X�R�A�ȊO�������グ��
			pPoint->posDest.y -= POINT_HEIGHT;
		}
	}
}

//==========================================
//  ���擾����
//==========================================
Point *GetPoint()
{
	return &g_aPoint[0];
}