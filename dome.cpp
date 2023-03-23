#include "Dome.h"
#include "input.h"
#include "title.h"
#include "player.h"

//�}�N����`
#define MOVE_SPEED (5.0f)

//�O���[�o���ϐ�

LPD3DXMESH g_pMeshDome = NULL; //���b�V��(���_���)�ւ̃|�C���^
LPD3DXBUFFER g_pBuffMatDome = NULL; //�}�e���A���ւ̃|�C���^
DWORD g_dwNumMatDome = 0; //�}�e���A���̐�
D3DXVECTOR3 g_posDome; //�ʒu
D3DXVECTOR3 g_rotDome; //����
D3DXVECTOR3 g_vtxMinDome, g_vtxMaxDome;
D3DXMATRIX g_mtxWorldDome; //���[���h�}�g���N�X
LPDIRECT3DTEXTURE9 g_pTextureDome[8] = {};


void InitDome()
{
	int nNumVtx; //���_��
	DWORD dwSizeFVF; //���_�t�H�[�}�b�g�̃T�C�Y
	BYTE*pVtxBuff; //���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice; //�f�o�C�X�̃|�C���^
	pDevice = GetDevice();

	g_posDome = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotDome = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_vtxMinDome = D3DXVECTOR3(1000.0f, 1000.0f, 1000.0f);
	g_vtxMaxDome = D3DXVECTOR3(-1000.0f, -1000.0f, -1000.0f);

	//x�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX("data\\MODEL\\sky.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatDome,
		NULL,
		&g_dwNumMatDome,
		&g_pMeshDome);

	//���_�����擾
	nNumVtx = g_pMeshDome->GetNumVertices();

	//���_�t�H�[�}�b�g�̃T�C�Y���擾
	dwSizeFVF = D3DXGetFVFVertexSize(g_pMeshDome->GetFVF());

	//���_�o�b�t�@�����b�N
	g_pMeshDome->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;
		if (g_vtxMinDome.x > vtx.x)
		{
			g_vtxMinDome.x = vtx.x;
		}
		if (g_vtxMinDome.y > vtx.y)
		{
			g_vtxMinDome.y = vtx.y;
		}
		if (g_vtxMinDome.z > vtx.z)
		{
			g_vtxMinDome.z = vtx.z;
		}
		if (g_vtxMaxDome.x < vtx.x)
		{
			g_vtxMaxDome.x = vtx.x;
		}
		if (g_vtxMaxDome.y < vtx.y)
		{
			g_vtxMaxDome.y = vtx.y;
		}
		if (g_vtxMaxDome.z < vtx.z)
		{
			g_vtxMaxDome.z = vtx.z;
		}

		pVtxBuff += dwSizeFVF;
	}
	//���_�o�b�t�@���A�����b�N
	g_pMeshDome->UnlockVertexBuffer();

	//�e�N�X�`���̓ǂݍ���
	D3DXMATERIAL *pMat; //�}�e���A���f�[�^�ւ̃|�C���^
	pMat = (D3DXMATERIAL*)g_pBuffMatDome->GetBufferPointer();
	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatDome; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{//�e�N�X�`���t�@�C�������݂��鎞
			D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,
				&g_pTextureDome[nCntMat]);
		}
	}
}
void UninitDome()
{
	for (int nCntTex = 0; nCntTex < 8; nCntTex++)
	{
		if (g_pTextureDome[nCntTex] != NULL)
		{
			g_pTextureDome[nCntTex]->Release();
			g_pTextureDome[nCntTex] = NULL;
		}
	}
	//���b�V���̔j��
	if (g_pMeshDome != NULL)
	{
		g_pMeshDome->Release();
		g_pMeshDome = NULL;
	}

	//�}�e���A���̔j��
	if (g_pBuffMatDome != NULL)
	{
		g_pBuffMatDome->Release();
		g_pBuffMatDome = NULL;
	}
}
void UpdateDome()
{
	Player*pPlayer = GetPlayer();
	g_posDome = pPlayer->pos;
}
void DrawDome()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); //�f�o�C�X�̃|�C���^
	D3DXMATRIX mtxRot, mtxTrans; //�v�Z�p�}�g���N�X
	D3DMATERIAL9 matDef; //���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat; //�}�e���A���f�[�^�ւ̃|�C���^

						//���[���h�}�g���N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldDome);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotDome.y, g_rotDome.x, g_rotDome.z);
	D3DXMatrixMultiply(&g_mtxWorldDome, &g_mtxWorldDome, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_posDome.x, g_posDome.y, g_posDome.z);
	D3DXMatrixMultiply(&g_mtxWorldDome, &g_mtxWorldDome, &mtxTrans);

	//���[���h�}�g���N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldDome);

	//���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	//�}�e���A���f�[�^�̃|�C���^���擾
	pMat = (D3DXMATERIAL*)g_pBuffMatDome->GetBufferPointer();
	pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //�t�H�O�̖�����
	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatDome; nCntMat++)
	{
		//�}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureDome[nCntMat]);

		//���f��(�p�[�c)�̕`��
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		g_pMeshDome->DrawSubset(nCntMat);
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}
	//�ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
	pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE); //�t�H�O�̗L����
}
