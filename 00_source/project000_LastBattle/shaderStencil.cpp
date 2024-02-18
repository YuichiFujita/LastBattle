//============================================================
//
//	ステンシルシェーダーヘッダー [shaderStencil.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "shaderStencil.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char* EFFECT_FX = "shaderStencil.fx";	// ステンシルシェーダーのエフェクトファイル
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CStencilShader *CStencilShader::m_pShader = nullptr;	// シェーダー情報

//************************************************************
//	親クラス [CStencilShader] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CStencilShader::CStencilShader() :
	m_pRefValue		(nullptr),	// 参照値
	m_pComparison	(nullptr),	// 比較演算子
	m_pOperation	(nullptr)	// 数値操作
{

}

//============================================================
//	デストラクタ
//============================================================
CStencilShader::~CStencilShader()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CStencilShader::Init(void)
{
	// 変数を宣言
	D3DCAPS9 caps;	// ハードウェア機能
	HRESULT hr;		// 異常終了の確認用

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイス情報
	LPD3DXBUFFER pError  = nullptr;	// コンパイルエラー情報
	LPD3DXEFFECT pEffect = nullptr;	// エフェクト設定用

	// メンバ変数を初期化
	m_pRefValue		= nullptr;	// 参照値
	m_pComparison	= nullptr;	// 比較演算子
	m_pOperation	= nullptr;	// 数値操作

	// シェーダーの初期化
	if (FAILED(CShader::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// ハードウェア機能を取得
	pDevice->GetDeviceCaps(&caps);

	if (caps.VertexShaderVersion >= D3DVS_VERSION(1, 1)
	&&  caps.PixelShaderVersion  >= D3DPS_VERSION(2, 0))
	{ // 頂点・ピクセルシェーダのバージョンが使用可能な場合

		// エフェクトファイルの読込
		hr = D3DXCreateEffectFromFile
		( // 引数
			pDevice,	// デバイスへのポインタ
			EFFECT_FX,	// エフェクトファイル
			nullptr,	// プリプロセッサ定義
			nullptr,	// インクルード操作
			0,			// 読込オプションフラグ
			nullptr,	// グローバル変数インターフェース
			&pEffect,	// エフェクトインターフェース
			&pError		// コンパイルエラー情報
		);
		if (SUCCEEDED(hr))
		{ // 読込に成功した場合

			// エフェクト情報を設定
			SetEffect(pEffect);

			// テクニック関数を取得・設定
			SetTechnique(pEffect->GetTechniqueByName("TShader"));

			// 各マトリックスのグローバル変数を取得・設定
			SetMatrixWorld(pEffect->GetParameterByName(nullptr, "g_mtxWorld"));		// ワールドマトリックス
			SetMatrixView(pEffect->GetParameterByName(nullptr, "g_mtxView"));		// ビューマトリックス
			SetMatrixProjection(pEffect->GetParameterByName(nullptr, "g_mtxProj"));	// プロジェクションマトリックス

			// グローバル変数を取得
			m_pTextureStencil	= pEffect->GetParameterByName(nullptr, "g_textureStencil");	// ステンシルテクスチャ
			m_pRefValue			= pEffect->GetParameterByName(nullptr, "g_nRefValue");		// 参照値
			m_pComparison		= pEffect->GetParameterByName(nullptr, "g_nComparison");	// 比較演算子
			m_pOperation		= pEffect->GetParameterByName(nullptr, "g_nOperation");		// 数値操作

			// ステンシルテクスチャを設定
			SetTexture(GET_RENDERER->m_nStencilTexID);
		}
		else
		{ // 読込に失敗した場合

			// 読み込み失敗を返す
			assert(false);
			return E_FAIL;
		}
	}
	else
	{ // バージョンが使用不可な場合

		// 古いバージョンによる失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CStencilShader::Uninit(void)
{
	// シェーダーの終了
	CShader::Uninit();
}

//============================================================
//	参照値の設定処理
//============================================================
void CStencilShader::SetRefValue(const int nRefValue)
{
	if (!IsEffectOK()) { assert(false); return; }	// エフェクト未使用

	// エフェクトに参照値を設定
	GetEffect()->SetInt(m_pRefValue, nRefValue);
}

//============================================================
//	比較演算子の設定処理
//============================================================
void CStencilShader::SetComparison(const EComparison comparison)
{
	if (!IsEffectOK()) { assert(false); return; }	// エフェクト未使用

	// エフェクトに比較演算子を設定
	GetEffect()->SetInt(m_pComparison, comparison);
}

//============================================================
//	数値操作の設定処理
//============================================================
void CStencilShader::SetOperation(const EOperation operation)
{
	if (!IsEffectOK()) { assert(false); return; }	// エフェクト未使用

	// エフェクトに数値操作を設定
	GetEffect()->SetInt(m_pOperation, operation);
}

//============================================================
//	生成処理
//============================================================
CStencilShader *CStencilShader::Create(void)
{
	// インスタンス使用中
	assert(m_pShader == nullptr);

	// ステンシルシェーダーの生成
	m_pShader = new CStencilShader;
	if (m_pShader == nullptr)
	{ // 生成に失敗した場合

		assert(false);
		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// ステンシルシェーダーの初期化
		if (FAILED(m_pShader->Init()))
		{ // 初期化に失敗した場合

			// ステンシルシェーダーの破棄
			SAFE_DELETE(m_pShader);
			return nullptr;
		}

		// 確保したアドレスを返す
		return m_pShader;
	}
}

//============================================================
//	取得処理
//============================================================
CStencilShader *CStencilShader::GetInstance(void)
{
	// インスタンス未使用
	assert(m_pShader != nullptr);

	// ステンシルシェーダーのポインタを返す
	return m_pShader;
}

//============================================================
//	破棄処理
//============================================================
void CStencilShader::Release(void)
{
	// ステンシルシェーダーの終了
	assert(m_pShader != nullptr);
	m_pShader->Uninit();

	// メモリ開放
	SAFE_DELETE(m_pShader);
}

//============================================================
//	テクスチャの設定処理 (ポインタ)
//============================================================
void CStencilShader::SetTexture(const LPDIRECT3DTEXTURE9 pTexture)
{
	if (!IsEffectOK()) { assert(false); return; }	// エフェクト未使用

	// エフェクトにテクスチャを設定
	if (pTexture != nullptr)
	{ // テクスチャが使用されている場合

		if (!IsEffectOK()) { assert(false); return; }	// エフェクト未使用

		// エフェクトにステンシルテクスチャを設定
		GetEffect()->SetTexture(m_pTextureStencil, pTexture);
	}
	else { assert(false); }	// 指定なし
}

//============================================================
//	テクスチャの設定処理 (インデックス)
//============================================================
void CStencilShader::SetTexture(const int nTextureID)
{
	if (!IsEffectOK()) { assert(false); return; }	// エフェクト未使用

	// エフェクトにテクスチャを設定
	if (nTextureID != NONE_IDX)
	{ // テクスチャが使用されている場合

		if (!IsEffectOK()) { assert(false); return; }	// エフェクト未使用
		CTexture *pTexture = CManager::GetInstance()->GetTexture();	// テクスチャ情報

		// エフェクトにステンシルテクスチャを設定
		GetEffect()->SetTexture(m_pTextureStencil, pTexture->GetTexture(nTextureID));
	}
	else { assert(false); }	// 指定なし
}
