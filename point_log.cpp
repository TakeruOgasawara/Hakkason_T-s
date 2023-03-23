//==================================================================================================
//
//point_log.cpp
//Author:���R����
//
//==================================================================================================

#include "main.h"
#include "point_log.h"
#include "point.h"

//==============================
// �O���[�o���ϐ��錾
//==============================
#define LOG_WIDTH	(500)	//����
#define LOG_HEIGHT	(100)	//����

//==============================
// �O���[�o���ϐ��錾
//==============================
LPDIRECT3DTEXTURE9 g_apTexturePointLog[LOGTYPE_MAX] = {};			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPointLog = NULL;	//���_�o�b�t�@�ւ̃|�C���^
PointLog g_aPointLog[MAX_POINT_NUM];	//�\���̏��
const char *c_apFilenameMessage[LOGTYPE_MAX] =
{//X�t�@�C���p�X
	"data\\TEXTURE\\UI\\HighTime_000.png",
	"data\\TEXTURE\\UI\\nowTime_000.png",
};

//==================================================================================================
// ����������
//==================================================================================================
void InitPointLog(void)
{
	//�ϐ�������
	ZeroMemory(&g_aPointLog[0],sizeof(PointLog) * MAX_POINT_NUM);

	//�f�o�C�X�|�C���^�ݒ�
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	for (int nCntTex = 0; nCntTex < LOGTYPE_MAX;nCntTex++)
	{
		//�e�N�X�`���ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameMessage[nCntTex],
			&g_apTexturePointLog[nCntTex]);
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_POINT_NUM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPointLog,
		NULL);

	//���_���̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPointLog->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPoint = 0;nCntPoint < MAX_POINT_NUM;nCntPoint++)
	{
		g_aPointLog[nCntPoint].col = D3DXCOLOR{1.0f,1.0f,1.0f,1.0f};

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//�e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//�|�C���^��i�߂�
		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffPointLog->Unlock();
}

//==================================================================================================
//�I������
//==================================================================================================
void UninitPointLog(void)
{
	for (int nCntTex = 0; nCntTex < LOGTYPE_MAX; nCntTex++)
	{
		if (g_apTexturePointLog[nCntTex] != NULL)
		{//�e�N�X�`���ւ̃|�C���^�̔j��
			g_apTexturePointLog[nCntTex]->Release();
			g_apTexturePointLog[nCntTex] = NULL;
		}
	}
	
	if (g_pVtxBuffPointLog != NULL)
	{//���_�o�b�t�@�ւ̃|�C���^�̔j��
		g_pVtxBuffPointLog->Release();
		g_pVtxBuffPointLog = NULL;
	}
}

//==================================================================================================
// �X�V����
//==================================================================================================
void UpdatePointLog(void)
{
	//���擾
	PointLog *pPointLog = GetPointLog();

	//���_���̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPointLog->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPointLog = 0; nCntPointLog < MAX_POINT_NUM; nCntPointLog++, pPointLog++)
	{
		if (pPointLog->bUse)
		{//�g�p����Ă�����

			if (pPointLog->type == LOGTYPE_TOTAL)
			{
				//���Έʒu�̐ݒ�
				pPointLog->pos =
				{
					pPointLog->pPoint->pos.x - LOG_WIDTH * pPointLog->fScale,
					pPointLog->pPoint->pos.y,
					0.0f
				};
			}
			else
			{
				//���Έʒu�̐ݒ�
				pPointLog->pos =
				{
					pPointLog->pPoint->pos.x - LOG_WIDTH * pPointLog->fScale,
					pPointLog->pPoint->pos.y,
					0.0f
				};
			}

			//�F�̐ݒ�
			pPointLog->col.a = pPointLog->pPoint->col.a;

			//���ʂƂ��͈ꏏ���揈��
			pPointLog->bUse = pPointLog->pPoint->bUse;

			//���_���̍X�V===========================================

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(pPointLog->pos.x, pPointLog->pos.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pPointLog->pos.x + LOG_WIDTH * pPointLog->fScale, pPointLog->pos.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pPointLog->pos.x, pPointLog->pos.y + LOG_HEIGHT * pPointLog->fScale, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pPointLog->pos.x + LOG_WIDTH * pPointLog->fScale, pPointLog->pos.y + LOG_HEIGHT * pPointLog->fScale, 0.0f);

			//���_�J���[�̐ݒ�
			pVtx[0].col = pPointLog->col;
			pVtx[1].col = pPointLog->col;
			pVtx[2].col = pPointLog->col;
			pVtx[3].col = pPointLog->col;

			//���_���̍X�V===========================================
		}

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffPointLog->Unlock();
}

//==================================================================================================
// �`�揈��
//==================================================================================================
void DrawPointLog(void)
{
	//���擾
	PointLog *pPointLog = GetPointLog();

	//�f�o�C�X�|�C���^�ݒ�
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPointLog, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntPointLog = 0;nCntPointLog < MAX_POINT_NUM;nCntPointLog++, pPointLog++)
	{
		if (pPointLog->bUse)
		{//�g�p����Ă�����
			//�e�N�X�`���ݒ�
			pDevice->SetTexture(0, g_apTexturePointLog[pPointLog->type]);

			//�w�i�̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntPointLog, 2);
		}
	}
}

//==================================================================================================
// �ݒ菈��
//==================================================================================================
void SetPointLog(Point *pPoint, LOGTYPE type, float fScale)
{
	//���擾
	PointLog *pPointLog = GetPointLog();

	for (int nCntPointLog = 0;nCntPointLog < MAX_POINT_NUM;nCntPointLog++, pPointLog++)
	{
		if (pPointLog->bUse == false)
		{//�g�p����Ă��Ȃ��Ȃ�

			//�����󂯎��
			pPointLog->pPoint = pPoint;
			pPointLog->type = type;
			pPointLog->fScale = fScale;

			//�g�p���Ă����Ԃɂ���
			pPointLog->bUse = true;

			break;
		}
	}
}

//==================================================================================================
// ���擾
//==================================================================================================
PointLog *GetPointLog(void)
{
	return &g_aPointLog[0];
}