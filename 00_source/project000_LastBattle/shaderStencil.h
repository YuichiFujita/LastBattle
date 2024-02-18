//============================================================
//
//	ステンシルシェーダーヘッダー [shaderStencil.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _SHADER_STENCIL_H_
#define _SHADER_STENCIL_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "shader.h"

//************************************************************
//	クラス定義
//************************************************************
// ステンシルシェーダークラス
class CStencilShader : public CShader
{
public:
	// 比較演算子列挙
	enum EComparison
	{
		COMPARISON_NEVER = 0,		// 必ず失敗
		COMPARISON_LESS,			//  <で成功
		COMPARISON_LESSEQUAL,		// <=で成功
		COMPARISON_EQUAL,			// ==で成功
		COMPARISON_NOTEQUAL,		// !=で成功
		COMPARISON_GREATER,			//  >で成功
		COMPARISON_GREATEREQUAL,	// >=で成功
		COMPARISON_ALWAYS,			// 必ず成功
		COMPARISON_MAX				// この列挙型の総数
	};

	// 数値操作列挙
	enum EOperation
	{
		OPERATION_REPLACE = 0,	// 参照値に置換
		OPERATION_INCRSAT,		// インクリメント
		OPERATION_DECRSAT,		// デクリメント
	};

	// コンストラクタ
	CStencilShader();

	// デストラクタ
	~CStencilShader() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了

	// メンバ関数
	void SetRefValue(const int nRefValue);				// 参照値の設定
	void SetComparison(const EComparison comparison);	// 比較演算子の設定
	void SetOperation(const EOperation operation);		// 数値操作の設定

	// 静的メンバ関数
	static CStencilShader *Create(void);		// 生成
	static CStencilShader *GetInstance(void);	// 取得
	static void Release(void);					// 破棄

private:
	// メンバ関数
	void SetTexture(const LPDIRECT3DTEXTURE9 pTexture);	// テクスチャ設定 (ポインタ)
	void SetTexture(const int nTextureID);				// テクスチャ設定 (インデックス)

	// 静的メンバ変数
	static CStencilShader *m_pShader;	// シェーダー情報

	// メンバ変数
	D3DXHANDLE m_pTextureStencil;	// ステンシルテクスチャ
	D3DXHANDLE m_pRefValue;			// 参照値
	D3DXHANDLE m_pComparison;		// 比較演算子
	D3DXHANDLE m_pOperation;		// 数値操作
};

#endif	// _SHADER_H_
