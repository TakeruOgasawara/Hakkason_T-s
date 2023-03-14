//======================================================================================
//
//エフェクトの処理[Player.cpp]
//Author;小笠原　彪
//
//======================================================================================
#include <stdio.h>
#include "main.h"
#include "effect.h"

//*****************************
//マクロ定義
//*****************************
#define EFFECT_TEX_FILE		"data\\TEXTURE\\effect000.jpg"			//テクスチャ名

//*****************************
//グローバル宣言
//*****************************
LPDIRECT3DTEXTURE9 g_pTextureEffect = NULL;					//テクスチャ―へのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;			//頂点バッファへのポインタ
Effect g_aEffect[MAX_EFFECT];								//エフェクトの情報
int g_EffectCnt;

//========================================================================
//弾の初期化処理
//========================================================================
void InitEffect(void)
{
	//変数宣言
	int nCntEffect;

	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, EFFECT_TEX_FILE, &g_pTextureEffect);

	//変数の初期化
	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		//構造体の初期化
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].fRadius = 0.0f;
		g_aEffect[nCntEffect].nLife = 0;
		g_aEffect[nCntEffect].nType = 0;
		g_aEffect[nCntEffect].bUse = false;				//使用していない状態にする
	}

	//グローバル宣言の初期化
	g_EffectCnt = 0;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_EFFECT,
		D3DUSAGE_WRITEONLY,
		pDevice->SetFVF(FVF_VERTEX_2D),
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL);

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
		//法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		//頂点カラー(0.0f～1.0f内で設定)
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		//テクスチャの座標設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;			//頂点データのポインタを4つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffEffect->Unlock();
}

//========================================================================
//エフェクトの終了処理
//========================================================================
void UninitEffect(void)
{
	//テクスチャの破棄
	if (g_pTextureEffect != NULL)
	{
		g_pTextureEffect->Release();
		g_pTextureEffect = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}

//========================================================================
//エフェクトの更新処理
//========================================================================
void UpdateEffect(void)
{
	//変数宣言
	int nCntEffect;

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{//エフェクトが使用されている場合
			//重力処理
			g_aEffect[nCntEffect].move.y -= 0.3f;

			if (g_aEffect[nCntEffect].pos.y <= 0.0f)
			{
				g_aEffect[nCntEffect].move.y += 0.5f;
			}

			//位置情報を更新
			g_aEffect[nCntEffect].pos.y += g_aEffect[nCntEffect].move.y;
			g_aEffect[nCntEffect].pos.x += g_aEffect[nCntEffect].move.x;
			g_aEffect[nCntEffect].pos.z += g_aEffect[nCntEffect].move.z;

			//寿命の減算
			g_aEffect[nCntEffect].nLife--;

			//移動量の更新
			g_aEffect[nCntEffect].pos += g_aEffect[nCntEffect].move;

			//カラーの減算//今の寿命わるマックスの寿命
			g_aEffect[nCntEffect].col.a = ((float)g_aEffect[nCntEffect].nLife) / ((float)g_aEffect[nCntEffect].nMaxLife);

			//半径の減算//今の寿命わるマックスの寿命
			g_aEffect[nCntEffect].fRadius -= ((float)g_aEffect[nCntEffect].nLife) / ((float)g_aEffect[nCntEffect].nMaxLife);

			if (g_aEffect[nCntEffect].nLife <= 0 || g_aEffect[nCntEffect].col.a <= 0 || g_aEffect[nCntEffect].fRadius <= 0)
			{//寿命が0の場合
				g_aEffect[nCntEffect].bUse = false;
			}

			//頂点カラー(0.0f～1.0f内で設定)
			pVtx[0].col = D3DXCOLOR(g_aEffect[nCntEffect].col);
			pVtx[1].col = D3DXCOLOR(g_aEffect[nCntEffect].col);
			pVtx[2].col = D3DXCOLOR(g_aEffect[nCntEffect].col);
			pVtx[3].col = D3DXCOLOR(g_aEffect[nCntEffect].col);

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
		}
		pVtx += 4;
	}
		//頂点バッファをアンロックする
		g_pVtxBuffEffect->Unlock();
}

//========================================================================
//エフェクトの描画処理
//========================================================================
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	D3DXMATRIX mtxTrans;						//計算用マトリックス
	D3DXMATRIX mtxView;							//ビューマトリックスの取得用

	//zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);		//比較方法
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);			//zバッファ更新の有効無効設定

	//アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);			//アルファテストの有効/無効の設定
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);		//比較方法
	pDevice->SetRenderState(D3DRS_ALPHAREF, 10);					//基準値

	//aブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);			//ライティングの有効/無効の設定

	g_EffectCnt = 0;

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aEffect[nCntEffect].mtxWorldBullet);

			//ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_aEffect[nCntEffect].mtxWorldBullet, NULL, &mtxView);

			g_aEffect[nCntEffect].mtxWorldBullet._41 = 0.0f;
			g_aEffect[nCntEffect].mtxWorldBullet._42 = 0.0f;
			g_aEffect[nCntEffect].mtxWorldBullet._43 = 0.0f;

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aEffect[nCntEffect].pos.x, g_aEffect[nCntEffect].pos.y, g_aEffect[nCntEffect].pos.z);
			D3DXMatrixMultiply(&g_aEffect[nCntEffect].mtxWorldBullet, &g_aEffect[nCntEffect].mtxWorldBullet, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aEffect[nCntEffect].mtxWorldBullet);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffEffect,
				0, sizeof(VERTEX_3D));			//頂点情報構造体のサイズ

												//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureEffect);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,				//プリミティブの種類
				nCntEffect * 4,						//描画する最初のインデックス
				2);						//プリミティブ(ポリゴン)数

			g_EffectCnt++;
		}
	}

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);			//ライティングの有効/無効の設定

	//aブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);			//アルファテストの有効/無効の設定
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);		//比較方法
	pDevice->SetRenderState(D3DRS_ALPHAREF, 10);					//基準値

	//zバッファを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);			//zバッファ更新の有効無効設定
}

//========================================================================
//エフェクトの設定処理
//========================================================================
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife)
{
	//変数宣言
	int nCntEffect;

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == false)
		{//使われていない場合
			g_aEffect[nCntEffect].pos = pos;
			g_aEffect[nCntEffect].col = col;
			g_aEffect[nCntEffect].move = move;
			g_aEffect[nCntEffect].fRadius = fRadius;
			g_aEffect[nCntEffect].nLife = nLife;
			g_aEffect[nCntEffect].nMaxLife = nLife;
			g_aEffect[nCntEffect].bUse = true;				//使用している状態にする
			break;
		}
	}
}

void SetEffectLIfe(int nLife)
{

}


//========================================================================
//エフェクトの取得
//========================================================================
Effect *GetEffect(void)
{
	return &g_aEffect[0];
}

//========================================================================
//エフェクトの取得
//========================================================================
int GetEffectNum(void)
{
	return g_EffectCnt;
}