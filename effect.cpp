//========================================================================================
//
// エフェクトの処理[Effect.cpp]
// Author: 丹野 竜之介
//
//========================================================================================

#include "Effect.h"


//プロトタイプ宣言
#define MAX_Effect (40480)
#define NUM_EFFECT (2)

//構造体を宣言
typedef struct
{
	D3DXVECTOR3 pos; //位置
	D3DXCOLOR col; //色
	D3DXVECTOR3 move;
	D3DXMATRIX  mtx;
	float fRot;
	float fRadius; //半径
	int nLife; //寿命
	int nType;
	bool bUse; //使用しているか
}Effect;

//グローバル変数
D3DXVECTOR3 g_rotEffect;
float g_fLengthEffect;
float g_fAngleEffect;


LPDIRECT3DTEXTURE9 g_pTextureEffect[NUM_EFFECT] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;
Effect g_aEffect[MAX_Effect];

//======================================
//初期化処理
//======================================
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntEffect;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Shadow000.jpg",
		&g_pTextureEffect[0]);
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\effect002.tga",
		&g_pTextureEffect[1]);

	
	//弾の情報の初期化
	for (nCntEffect = 0; nCntEffect < MAX_Effect; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aEffect[nCntEffect].bUse = false;
	}
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_Effect,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL);

	VERTEX_3D *pVtx;
	//頂点バッファをロックし、頂点データへのポインタ
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_Effect; nCntEffect++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3((- 10), (- 10), 0.0f);
		pVtx[1].pos = D3DXVECTOR3((+ 10), (- 10), 0.0f);
		pVtx[2].pos = D3DXVECTOR3((- 10), (+ 10), 0.0f);
		pVtx[3].pos = D3DXVECTOR3((+ 10), (+ 10), 0.0f);

		//法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	g_pVtxBuffEffect->Unlock();
}

//======================================
//終了処理
//======================================
void UninitEffect(void)
{
	for (int nCnt = 0; nCnt < NUM_EFFECT; nCnt++)
	{
		if (g_pTextureEffect[nCnt] != NULL)
		{
			g_pTextureEffect[nCnt]->Release();
			g_pTextureEffect[nCnt] = NULL;
		}
	}

	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}
//======================================
//更新処理
//======================================
void UpdateEffect(void)
{
	int nCntEffect;

	VERTEX_3D *pVtx;
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_Effect; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{//エフェクトが使用されている
			
			g_aEffect[nCntEffect].nLife -= 1;
			g_aEffect[nCntEffect].fRadius -= g_aEffect[nCntEffect].fRadius / g_aEffect[nCntEffect].nLife;
			g_aEffect[nCntEffect].col.a *= (g_aEffect[nCntEffect].nLife * 0.015f);
			g_aEffect[nCntEffect].pos += g_aEffect[nCntEffect].move;
			

			pVtx[0].pos = D3DXVECTOR3(( - g_aEffect[nCntEffect].fRadius), ( + g_aEffect[nCntEffect].fRadius), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(( + g_aEffect[nCntEffect].fRadius), ( + g_aEffect[nCntEffect].fRadius), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(( - g_aEffect[nCntEffect].fRadius), ( - g_aEffect[nCntEffect].fRadius), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(( + g_aEffect[nCntEffect].fRadius), ( - g_aEffect[nCntEffect].fRadius), 0.0f);

			pVtx[0].col = g_aEffect[nCntEffect].col;
			pVtx[1].col = g_aEffect[nCntEffect].col;
			pVtx[2].col = g_aEffect[nCntEffect].col;
			pVtx[3].col = g_aEffect[nCntEffect].col;

			if (g_aEffect[nCntEffect].nLife <= 0 || g_aEffect[nCntEffect].fRadius <= 0.0f)
			{
				g_aEffect[nCntEffect].bUse = false;
			}

		}pVtx += 4;
	}

	g_pVtxBuffEffect->Unlock();
}

//======================================
//描画処理
//======================================
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = GetDevice();

	D3DXMATRIX  mtxTrans; //計算用マトリクス
	D3DXMATRIX mtxView; //ビューマトリクス取得用

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
						
	//アルファテストを有効化
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 80);

	////Zテストを無効化
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	for (int nCnt = 0; nCnt < MAX_Effect; nCnt++)
	{
		if (g_aEffect[nCnt].bUse == true)
		{
			if (g_aEffect[nCnt].nType == 0)
			{
				//aブレンディングを加算合成に設定
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			}
			
			//ワールドマトリクスの初期化
			D3DXMatrixIdentity(&g_aEffect[nCnt].mtx);

			//ビューマトリクス取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_aEffect[nCnt].mtx, NULL, &mtxView);

			g_aEffect[nCnt].mtx._41 = 0.0f;
			g_aEffect[nCnt].mtx._42 = 0.0f;
			g_aEffect[nCnt].mtx._43 = 0.0f;

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aEffect[nCnt].pos.x, g_aEffect[nCnt].pos.y, g_aEffect[nCnt].pos.z);

			D3DXMatrixMultiply(&g_aEffect[nCnt].mtx, &g_aEffect[nCnt].mtx, &mtxTrans);

			//ワールドマトリクスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aEffect[nCnt].mtx);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureEffect[g_aEffect[nCnt].nType]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
		}
	}
	////Zテストを有効化
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//アルファテストを無効化
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 80);

	//aブレンディングを通常に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}
//======================================
//設定処理
//======================================
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col, D3DXVECTOR3 move,float fRadius, int nLife,int nType)
{
	int nCntEffect;
	VERTEX_3D *pVtx;
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_Effect; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == false)
		{
			g_aEffect[nCntEffect].pos = pos;
			g_aEffect[nCntEffect].col = col;
			g_aEffect[nCntEffect].fRadius = fRadius;
			g_aEffect[nCntEffect].nLife = nLife;
			g_aEffect[nCntEffect].move = move;
			g_aEffect[nCntEffect].nType = nType;
			g_aEffect[nCntEffect].bUse = true;
			break;
		}

		pVtx += 4;
	}
	g_pVtxBuffEffect->Unlock();
}

