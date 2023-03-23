#include "Dome.h"
#include "input.h"
#include "title.h"
#include "player.h"

//マクロ定義
#define MOVE_SPEED (5.0f)

//グローバル変数

LPD3DXMESH g_pMeshDome = NULL; //メッシュ(頂点情報)へのポインタ
LPD3DXBUFFER g_pBuffMatDome = NULL; //マテリアルへのポインタ
DWORD g_dwNumMatDome = 0; //マテリアルの数
D3DXVECTOR3 g_posDome; //位置
D3DXVECTOR3 g_rotDome; //向き
D3DXVECTOR3 g_vtxMinDome, g_vtxMaxDome;
D3DXMATRIX g_mtxWorldDome; //ワールドマトリクス
LPDIRECT3DTEXTURE9 g_pTextureDome[8] = {};


void InitDome()
{
	int nNumVtx; //頂点数
	DWORD dwSizeFVF; //頂点フォーマットのサイズ
	BYTE*pVtxBuff; //頂点バッファへのポインタ
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = GetDevice();

	g_posDome = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotDome = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_vtxMinDome = D3DXVECTOR3(1000.0f, 1000.0f, 1000.0f);
	g_vtxMaxDome = D3DXVECTOR3(-1000.0f, -1000.0f, -1000.0f);

	//xファイルの読み込み
	D3DXLoadMeshFromX("data\\MODEL\\sky.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatDome,
		NULL,
		&g_dwNumMatDome,
		&g_pMeshDome);

	//頂点数を取得
	nNumVtx = g_pMeshDome->GetNumVertices();

	//頂点フォーマットのサイズを取得
	dwSizeFVF = D3DXGetFVFVertexSize(g_pMeshDome->GetFVF());

	//頂点バッファをロック
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
	//頂点バッファをアンロック
	g_pMeshDome->UnlockVertexBuffer();

	//テクスチャの読み込み
	D3DXMATERIAL *pMat; //マテリアルデータへのポインタ
	pMat = (D3DXMATERIAL*)g_pBuffMatDome->GetBufferPointer();
	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatDome; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{//テクスチャファイルが存在する時
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
	//メッシュの破棄
	if (g_pMeshDome != NULL)
	{
		g_pMeshDome->Release();
		g_pMeshDome = NULL;
	}

	//マテリアルの破棄
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
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); //デバイスのポインタ
	D3DXMATRIX mtxRot, mtxTrans; //計算用マトリクス
	D3DMATERIAL9 matDef; //現在のマテリアル保存用
	D3DXMATERIAL *pMat; //マテリアルデータへのポインタ

						//ワールドマトリクスの初期化
	D3DXMatrixIdentity(&g_mtxWorldDome);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotDome.y, g_rotDome.x, g_rotDome.z);
	D3DXMatrixMultiply(&g_mtxWorldDome, &g_mtxWorldDome, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_posDome.x, g_posDome.y, g_posDome.z);
	D3DXMatrixMultiply(&g_mtxWorldDome, &g_mtxWorldDome, &mtxTrans);

	//ワールドマトリクスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldDome);

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	//マテリアルデータのポインタを取得
	pMat = (D3DXMATERIAL*)g_pBuffMatDome->GetBufferPointer();
	pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //フォグの無効化
	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatDome; nCntMat++)
	{
		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureDome[nCntMat]);

		//モデル(パーツ)の描写
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		g_pMeshDome->DrawSubset(nCntMat);
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}
	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
	pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE); //フォグの有効化
}
