//===============================================================================
//
//[main.h]
//Author
//
//===============================================================================
#ifndef _MAIN_H_				//このマクロ定義がされていなかったら
#define _MAIN_H_				//2重インクルード帽子のマクロを定義
#include <windows.h>
#include <time.h>			//randの値を毎回変えるために必要
#include "d3dx9.h"			//描画処理に必要
#define DIRECTINPUT_VERSION (0x0800)		//ビルド時の警告対処用マクロ
#include "dinput.h"						//入力処理に必要
#include "xaudio2.h"		//サウンドを使用するために必要

//画面モードの種類
typedef enum
{
	MODE_TITLE = 0,				//タイトル画面
	MODE_TUTORIAL,				//チュートリアル画面
	MODE_GAME,					//ゲーム画面
	MODE_RESULT,				//リザルト画面
	MODE_MAX
} MODE;

//プロトタイプ宣言
LPDIRECT3DDEVICE9 GetDevice(void);
void SetMode(MODE mode);
MODE GetMode(void);

//ライブラリのリンク
#pragma comment(lib, "d3d9.lib")			//描画処理に必要
#pragma comment(lib, "d3dx9.lib")			//[d3d9]の拡張ライブラリ
#pragma comment(lib, "dxguid.lib")			//Directxコンポーネント使用に必要
#pragma comment(lib, "winmm.lib")			//システム時刻の取得に必要
#pragma comment(lib, "dinput8.lib")			//入力処理に必要

//マクロ定義
#define SCREEN_WIDTH	(1280)					//ウィンドウの幅
#define SCREEN_HEIGHT	(720)					//ウィンドウの高さ

#define FVF_VERTEX_2D		(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)			//頂点フォーマット[2D]

#define FVF_VERTEX_3D		(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)			//頂点フォーマット[3D]

//頂点情報[3D]の構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;			//頂点座標
	D3DXVECTOR3 nor;			//法線ベクトル
	D3DCOLOR col;				//頂点カラー
	D3DXVECTOR2 tex;			//テクスチャ座標
} VERTEX_3D;

//頂点情報[2D]の構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;			//頂点座標
	float rhw;					//座標変換用係数(1.0fで固定)
	D3DCOLOR col;				//頂点カラー
	D3DXVECTOR2 tex;

} VERTEX_2D;


#endif