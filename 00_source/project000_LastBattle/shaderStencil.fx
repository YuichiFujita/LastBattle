//============================================================
//
//	ステンシルシェーダーのエフェクトファイル [shaderStencil.fx]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	グローバル変数宣言
//************************************************************
float4x4 g_mtxWorld	: WORLD;		// ワールドマトリックス
float4x4 g_mtxView	: VIEW;			// ビューマトリックス
float4x4 g_mtxProj	: PROJECTION;	// プロジェクションマトリックス
texture  g_textureStencil;			// ステンシルテクスチャ

int g_nRefValue;	// 参照値
int g_nComparison;	// 比較演算子
int g_nOperation;	// 数値操作

//************************************************************
//	サンプラー宣言
//************************************************************
sampler texObject : register(s0);	// オブジェクトテクスチャ
sampler texStencil =	// ステンシルテクスチャ
sampler_state			// サンプラーステート
{
	// 対象テクスチャ
	Texture = <g_textureStencil>;	// ステンシルテクスチャ

	// 拡縮・ミップマップ
	Filter = MIN_MAG_MIP_LINEAR;	// 線形補完

	// テクスチャアドレッシングモード
	AddressU = Clamp;	// U成分の端を引き延ばす
	AddressV = Clamp;	// V成分の端を引き延ばす
};

//************************************************************
//	構造体定義
//************************************************************
// 頂点シェーダー出力情報
struct VS_OUTPUT
{
	float4 pos : POSITION;	// 頂点座標
	float2 tex : TEXCOORD0;	// テクセル座標
};

//************************************************************
//	マクロ定義
//************************************************************
#define MIN_REF (0)		// 最小参照値
#define MAX_REF (9)		// 最大参照値
#define NUM_REF (10)	// 合計参照値

//************************************************************
//	グローバル関数
//************************************************************
//============================================================
//	頂点座標の射影変換
//============================================================
float4 TransVertex
(
	in float4	inPos		: POSITION,		// 頂点座標
	in float4x4	inMtxWorld	: WORLD,		// ワールドマトリックス
	in float4x4	inMtxView	: VIEW,			// ビューマトリックス
	in float4x4	inMtxProj	: PROJECTION	// プロジェクションマトリックス
)
{
	// 頂点座標を射影変換
	inPos = mul(inPos, inMtxWorld);
	inPos = mul(inPos, inMtxView);
	inPos = mul(inPos, inMtxProj);

	// 変換後の頂点座標を返す
	return inPos;
}

//============================================================
//	頂点シェーダー
//============================================================
void VS
(
	in	float4		inPos : POSITION,	// 頂点座標
	in	float2		inTex : TEXCOORD0,	// テクセル座標
	out	VS_OUTPUT	outVertex			// 頂点情報
)
{
	// ローカル頂点座標を射影変換し設定
	outVertex.pos = TransVertex(inPos, g_mtxWorld, g_mtxView, g_mtxProj);

	// テクセル座標を設定
	outVertex.tex = inTex;
}

//============================================================
//	ピクセルシェーダー
//============================================================
void PS
(
	in	VS_OUTPUT	inVertex,		// 頂点情報
	out	float4		outCol : COLOR0	// ピクセル色
)
{
	bool bVsSuccess = false;	// 比較結果フラグ
	int nPixcelValue = 0;		// ピクセルの設定済み参照値

	// ピクセルの色情報を初期化
	outCol = float4(1.0f, 0.0f, 0.0f, 1.0f);

	// ピクセルに設定されている参照値を取得
	nPixcelValue = (int)(((float)NUM_REF / 1.0f) * tex2D(texStencil, inVertex.tex).r);

	// 比較演算子ごとの処理
	if (g_nComparison == 0)
	{ // 必ず失敗する場合

		// 失敗させる
		bVsSuccess = false;
	}
	else if (g_nComparison == 1)
	{ //  <で成功する場合

		if (nPixcelValue < g_nRefValue)
		{
			// 成功させる
			bVsSuccess = true;
		}
	}
	else if (g_nComparison == 2)
	{ // <=で成功する場合

		if (nPixcelValue <= g_nRefValue)
		{
			// 成功させる
			bVsSuccess = true;
		}
	}
	else if (g_nComparison == 3)
	{ // ==で成功する場合

		if (nPixcelValue == g_nRefValue)
		{
			// 成功させる
			bVsSuccess = true;
		}
	}
	else if (g_nComparison == 4)
	{ // !=で成功する場合

		if (nPixcelValue != g_nRefValue)
		{
			// 成功させる
			bVsSuccess = true;
		}
	}
	else if (g_nComparison == 5)
	{ //  >で成功する場合

		if (nPixcelValue > g_nRefValue)
		{
			// 成功させる
			bVsSuccess = true;
		}
	}
	else if (g_nComparison == 6)
	{ // >=で成功する場合

		if (nPixcelValue >= g_nRefValue)
		{
			// 成功させる
			bVsSuccess = true;
		}
	}
	else if (g_nComparison == 7)
	{ // 必ず成功する場合

		// 成功させる
		bVsSuccess = true;
	}

	if (!bVsSuccess)
	{ // 比較結果が失敗の場合

		// 描画を中止する
		//discard;
	}
	else
	{ // 比較結果が成功の場合

		// 数値操作ごとの処理
		float fSetValue = 0.0f;	// 設定するピクセル参照値
		if (g_nOperation == 0)
		{ // 参照値に置換する場合
		
			fSetValue = ((1.0f / (float)NUM_REF) * g_nRefValue);
		}
		else if (g_nOperation == 1)
		{ // インクリメントする場合

			// ピクセルの設定済み参照値を加算
			fSetValue = nPixcelValue;
			fSetValue += (1.0f / (float)NUM_REF);
			saturate(fSetValue);
		}
		else if (g_nOperation == 2)
		{ // デクリメントする場合

			// ピクセルの設定済み参照値を減算
			fSetValue  = nPixcelValue;
			fSetValue -= (1.0f / (float)NUM_REF);
			saturate(fSetValue);
		}

		// ピクセルの色情報を設定
		outCol = float4(fSetValue, fSetValue, fSetValue, 1.0f);
	}
}

//============================================================
//	テクニック関数
//============================================================
technique TShader
{
	pass P0
	{
		VertexShader = compile vs_1_1 VS();
		PixelShader  = compile ps_2_0 PS();
	}
}
