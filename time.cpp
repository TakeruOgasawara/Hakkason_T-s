//==================================================================================================
//
//Time.cpp
//Author:���R����
//
//==================================================================================================

#include "main.h"
#include "time.h"
#include "game.h"
#include "input.h"

//�}�N����`
#define NUM_PLACE				(4)											//�X�R�A�̌���
#define NUM_TEX					(3)											//�e�N�X�`���̐�

#define TIME_WIDTH				(50)										//����
#define TIME_HEIGHT				(50)										//����

#define BACK_WIDTH				(300)										//��
#define BACK_HEIGHT				(100)										//����

#define CLOCK_WIDTH				(5)										//��
#define CLOCK_HEIGHT			(70)										//����

#define TIME_TEX_FILE			"data\\TEXTURE\\UI\\Number000.png"			//�e�N�X�`����
#define BACK_TEX_FILE			"data\\TEXTURE\\UI\\time_back.png"			//�e�N�X�`����
#define CLOCK_TEX_FILE			"data\\TEXTURE\\UI\\clock.png"				//�e�N�X�`����

//���v�̍\����
typedef struct
{
	D3DXVECTOR3 pos;						//�ʒu
	D3DXVECTOR3 rot;						//����
	float fAngle;							//�Ίp���̊p�x
	float fLength;							//�Ίp���̒���
}Clock;

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_apTextureTime[NUM_TEX] = {};		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTime = NULL;			//���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_posTime;									//�X�R�A�̈ʒu
Clock g_clock;											//���v�̍\���̏��
int g_nTime;											//�X�R�A�̒l
int g_nLastTime;										//�O��̃X�R�A�̒l
int g_nCntSecond;										//��b���͂���J�E���^
int g_nMinute;											//���̐�

//�v���g�^�C�v�錾
void ControlDebug(void);

//==================================================================================================
//����������
//==================================================================================================
void InitTime(void)
{
	//�f�o�C�X�|�C���^�ݒ�
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		TIME_TEX_FILE,
		&g_apTextureTime[0]);

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		BACK_TEX_FILE,
		&g_apTextureTime[1]);

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		CLOCK_TEX_FILE,
		&g_apTextureTime[2]);

	//�J�E���^�̏�����
	g_nCntSecond = 0;
	g_nMinute = INITIAL_MINITE;

	//�X�R�A�l�̏�����
	g_nTime = 0;
	g_nLastTime = INITIAL_SECOND;

	//�X�R�A�ʒu�̏�����
	g_posTime = D3DXVECTOR3(SCREEN_WIDTH * 0.5f,50.0f,0.0f);
	D3DXVECTOR3 posBack = { SCREEN_WIDTH * 0.5f,50.0f,0.0f };

	//���v�̏�����
	g_clock.pos = D3DXVECTOR3(posBack.x - 88, posBack.y - 3, 0.0f);
	g_clock.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_clock.fAngle = atan2f(CLOCK_WIDTH,CLOCK_HEIGHT);
	g_clock.fLength = sqrtf(CLOCK_WIDTH * CLOCK_WIDTH + CLOCK_HEIGHT * CLOCK_HEIGHT) * 0.5f;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * (NUM_PLACE + NUM_TEX - 1),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTime,
		NULL);

	//���_���̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTime = 0; nCntTime < NUM_PLACE; nCntTime++)
	{//�S�Ă̏�����

		if (nCntTime >= NUM_PLACE / 2)
		{
			g_posTime.x += TIME_WIDTH / 2;
		}

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_posTime.x - TIME_WIDTH * 0.5f + (nCntTime * TIME_WIDTH),
								  g_posTime.y - TIME_HEIGHT * 0.5f, 0.0f);

		pVtx[1].pos = D3DXVECTOR3(g_posTime.x + TIME_WIDTH * 0.5f + (nCntTime * TIME_WIDTH),
								  g_posTime.y - TIME_HEIGHT * 0.5f, 0.0f);

		pVtx[2].pos = D3DXVECTOR3(g_posTime.x - TIME_WIDTH * 0.5f + (nCntTime * TIME_WIDTH),
								  g_posTime.y + TIME_HEIGHT * 0.5f, 0.0f);

		pVtx[3].pos = D3DXVECTOR3(g_posTime.x + TIME_WIDTH * 0.5f + (nCntTime * TIME_WIDTH),
								  g_posTime.y + TIME_HEIGHT * 0.5f, 0.0f);

		if (nCntTime >= NUM_PLACE / 2)
		{
			g_posTime.x -= TIME_WIDTH / 2;
		}

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 125);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 125);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 125);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 125);

		//�e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		//���_�f�[�^�̃|�C���^��4���i�߂�
		pVtx += 4;
	}

	//UI�w�i�ݒ�========================================================================
	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3
	(
		posBack.x - BACK_WIDTH * 0.5f,
		posBack.y - BACK_HEIGHT * 0.5f,
		0.0f
	);
	pVtx[1].pos = D3DXVECTOR3
	(
		posBack.x + BACK_WIDTH * 0.5f,
		posBack.y - BACK_HEIGHT * 0.5f,
		0.0f
	);
	pVtx[2].pos = D3DXVECTOR3
	(
		posBack.x - BACK_WIDTH * 0.5f,
		posBack.y + BACK_HEIGHT * 0.5f,
		0.0f
	);
	pVtx[3].pos = D3DXVECTOR3
	(
		posBack.x + BACK_WIDTH * 0.5f,
		posBack.y + BACK_HEIGHT * 0.5f,
		0.0f
	);

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

	//�e�N�X�`���ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx += 4;

	//�j�ݒ�========================================================================
	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
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

	//�e�N�X�`���ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffTime->Unlock();
}

//==================================================================================================
//�I������
//==================================================================================================
void UninitTime(void)
{
	for (int nCnt = 0; nCnt < NUM_TEX; nCnt++)
	{
		if (g_apTextureTime[nCnt] != NULL)
		{//�e�N�X�`���ւ̃|�C���^�j��
			g_apTextureTime[nCnt]->Release();
			g_apTextureTime[nCnt] = NULL;
		}
	}

	if (g_pVtxBuffTime != NULL)
	{//���_�o�b�t�@�ւ̃|�C���^�j��
		g_pVtxBuffTime->Release();
		g_pVtxBuffTime = NULL;
	}
}

//==================================================================================================
//�X�V����
//==================================================================================================
void UpdateTime(void)
{
#ifdef  _DEBUG
	//�f�o�b�O����
	ControlDebug();
#endif //  _DEBUG

	//�ϐ��錾
	int aTexU[NUM_PLACE], nCount;
	int aData1, aData2;
	D3DXCOLOR UICol = { 1.0f,1.0f,1.0f,1.0f };

	//�|�C���^�錾
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);
	
	if (g_nMinute <= 0 && g_nLastTime <= 0)
	{//���Ԓ��߂̕␳
		g_nMinute = 0;
		g_nLastTime = 0;
	}

	if ((g_nMinute != 0 || g_nLastTime != 0) && GetGameState() == GAMESTATE_NORMAL)
	{//���Ԑ؂�łȂ��ꍇ�̂݃J�E���g�_�E��
		g_nCntSecond++;

		if (g_nCntSecond >= 60)
		{//�P�b�̌v��
			g_nLastTime--;

			g_nCntSecond = 0;
		}

		if (g_nLastTime < 0)
		{//�ꕪ�̌v��
			g_nMinute--;

			g_nLastTime = 59;
		}

		//�j����
		g_clock.rot.z -= (D3DX_PI * 2) / ((INITIAL_MINITE * 60 + INITIAL_SECOND) * 60);
	}

	if (g_nMinute == 0 && g_nLastTime < 30)
	{//�Ԍx��
		UICol = {1.0f,0.0f,0.0f,1.0f};
	}
	else if (g_nMinute == 0)
	{//���F�x��
		UICol = { 1.0f,0.86f,0.6f,1.0f };
	}

	for (nCount = 0; nCount < NUM_PLACE / 2; nCount++)
	{//�X�R�A�\��
	 //�e������o��
		aData1 = pow(10.0f, NUM_PLACE / 2 - nCount);
		aData2 = pow(10.0f, NUM_PLACE / 2 - nCount - 1);

		aTexU[nCount] = g_nMinute % aData1 / aData2;

		//�e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f + aTexU[nCount] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + aTexU[nCount] * 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + aTexU[nCount] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + aTexU[nCount] * 0.1f, 1.0f);
		
		//���_�J���[�̐ݒ�
		pVtx[0].col = UICol;
		pVtx[1].col = UICol;
		pVtx[2].col = UICol;
		pVtx[3].col = UICol;

		//���_���̃|�C���^���S���i�߂�
		pVtx += 4;
	}

	for (; nCount < NUM_PLACE; nCount++)
	{//�X�R�A�\��
	 //�e������o��
		aData1 = pow(10.0, NUM_PLACE - nCount);
		aData2 = pow(10.0, NUM_PLACE - nCount - 1);

		aTexU[nCount] = g_nLastTime % aData1 / aData2;

		//�e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f + aTexU[nCount] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + aTexU[nCount] * 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + aTexU[nCount] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + aTexU[nCount] * 0.1f, 1.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = UICol;
		pVtx[1].col = UICol;
		pVtx[2].col = UICol;
		pVtx[3].col = UICol;

		//���_���̃|�C���^���S���i�߂�
		pVtx += 4;
	}

	//���_�J���[�̐ݒ�
	pVtx[0].col = UICol;
	pVtx[1].col = UICol;
	pVtx[2].col = UICol;
	pVtx[3].col = UICol;

	//�j�܂Ń|�C���^��i�߂�
	pVtx += 4;

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3
	(
		g_clock.pos.x + sinf(g_clock.rot.z + g_clock.fAngle) * g_clock.fLength,
		g_clock.pos.y + cosf(g_clock.rot.z + g_clock.fAngle) * g_clock.fLength,
		0.0f
	);
	pVtx[1].pos = D3DXVECTOR3
	(
		g_clock.pos.x + sinf(g_clock.rot.z - g_clock.fAngle) * g_clock.fLength,
		g_clock.pos.y + cosf(g_clock.rot.z - g_clock.fAngle) * g_clock.fLength,
		0.0f
	);
	pVtx[2].pos = D3DXVECTOR3
	(
		g_clock.pos.x + sinf(g_clock.rot.z - g_clock.fAngle + D3DX_PI) * g_clock.fLength,
		g_clock.pos.y + cosf(g_clock.rot.z - g_clock.fAngle + D3DX_PI) * g_clock.fLength,
		0.0f
	);
	pVtx[3].pos = D3DXVECTOR3
	(
		g_clock.pos.x + sinf(g_clock.rot.z + g_clock.fAngle - D3DX_PI) * g_clock.fLength,
		g_clock.pos.y + cosf(g_clock.rot.z + g_clock.fAngle - D3DX_PI) * g_clock.fLength,
		0.0f
	);

	//���_�J���[�̐ݒ�
	/*pVtx[0].col = UICol;
	pVtx[1].col = UICol;
	pVtx[2].col = UICol;
	pVtx[3].col = UICol;*/

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffTime->Unlock();
}

//==================================================================================================
//�f�o�b�O����
//==================================================================================================
void ControlDebug(void)
{
	if (GetKeyboardTrigger(DIK_1))
	{
		g_nLastTime -= 10;
	}
	else if (GetKeyboardTrigger(DIK_2))
	{
		g_nLastTime += 10;
	}
}

//==================================================================================================
//�`�揈��
//==================================================================================================
void DrawTime(void)
{
	//�f�o�C�X�|�C���^�ݒ�
	LPDIRECT3DDEVICE9 pDevice;

	//�ϐ��錾
	int nCntTime;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTime, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���ݒ�
	pDevice->SetTexture(0, g_apTextureTime[0]);
	
	for (nCntTime = 0; nCntTime < NUM_PLACE; nCntTime++)
	{//�S�Ă̒e���`�F�b�N����
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTime * 4, 2);
	}

	//�e�N�X�`���ݒ�
	pDevice->SetTexture(0, g_apTextureTime[1]);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTime * 4, 2);

	//�e�N�X�`���ݒ�
	pDevice->SetTexture(0, g_apTextureTime[2]);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntTime + 1) * 4, 2);
}

//==================================================================================================
//�ݒ菈��
//==================================================================================================
void SetTime(int nTime)
{
	//�ϐ��錾
	int aTexU[NUM_PLACE],nCount;
	int aData1, aData2;

	//�����X�R�A�����
	g_nTime = nTime;

	g_nMinute = nTime / 60;

	g_nLastTime = nTime % 60;

	//���_���̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (nCount = 0; nCount < NUM_PLACE / 2; nCount++)
	{//�X�R�A�\��
		//�e������o��
		aData1 = pow(10.0, NUM_PLACE / 2 - nCount);
		aData2 = pow(10.0, NUM_PLACE / 2 - nCount - 1);

		aTexU[nCount] = g_nMinute % aData1 / aData2;

		//�e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f + aTexU[nCount] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + aTexU[nCount] * 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + aTexU[nCount] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + aTexU[nCount] * 0.1f, 1.0f);

		//���_���̃|�C���^���S���i�߂�
		pVtx += 4;
	}

	for (; nCount < NUM_PLACE; nCount++)
	{//�X�R�A�\��
	 //�e������o��
		aData1 = pow(10.0, NUM_PLACE - nCount);
		aData2 = pow(10.0, NUM_PLACE - nCount - 1);

		aTexU[nCount] = g_nLastTime % aData1 / aData2;

		//�e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f + aTexU[nCount] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + aTexU[nCount] * 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + aTexU[nCount] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + aTexU[nCount] * 0.1f, 1.0f);

		//���_���̃|�C���^���S���i�߂�
		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffTime->Unlock();
}

//==================================================================================================
//�X�R�A���Z����
//==================================================================================================
void AddTime(int nValue)
{
	//���Z�X�R�A�����
	g_nTime += nValue;

	if (g_nTime >= 100000000)
	{//����ɒB������
		g_nTime = 99999999;
	}
}

//==================================================================================================
//�^�C���擾
//==================================================================================================
int GetTime(void)
{
	//�ϐ��錾
	int nTime = g_nMinute * 60 + g_nLastTime;

	return nTime;
}

//==================================================================================================
//�^�C���A�b�v�擾
//==================================================================================================
bool GetTimeUp(void)
{
	if (g_nMinute == 0 && g_nLastTime == 0)
	{//���Ԑ؂�
		return true;
	}
	else
	{
		return false;
	}
}