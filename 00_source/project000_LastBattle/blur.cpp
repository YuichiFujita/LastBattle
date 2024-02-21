//============================================================
//
//	ブラー処理 [blur.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "blur.h"
#include "objectChara.h"
#include "objectModel.h"
#include "multiModel.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int PRIORITY = 6;	// ブラーの優先順位
}

//************************************************************
//	親クラス [CBlur] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CBlur::CBlur() : CObject(LABEL_EFFECT, DIM_3D, PRIORITY),
	m_pParent		(nullptr),	// 親オブジェクト
	m_fStartAlpha	(0.0f),		// ブラーの開始透明度
	m_nMaxLength	(0)			// 保持する親オブジェクトの最大数
{
	// メンバ変数をクリア
	m_oldObject.clear();		// 過去オブジェクト情報
	m_mat = material::White();	// ブラー反映マテリアル
}

//============================================================
//	デストラクタ
//============================================================
CBlur::~CBlur()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CBlur::Init(void)
{
	// メンバ変数を初期化
	m_oldObject.clear();		// 過去オブジェクト情報
	m_mat = material::White();	// ブラー反映マテリアル
	m_pParent		= nullptr;	// 親オブジェクト
	m_fStartAlpha	= 0.0f;		// ブラーの開始透明度
	m_nMaxLength	= 0;		// 保持する親オブジェクトの最大数

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CBlur::Uninit(void)
{
	for (SInfo *pBlur : m_oldObject)
	{ // 要素数分繰り返す

		for (int nCntParts = 0; nCntParts < pBlur->nNumParts; nCntParts++)
		{ // パーツ数分繰り返す

			// パーツの終了
			SAFE_UNINIT(pBlur->apCharaParts[nCntParts]);
		}

		// パーツ格納情報の破棄
		SAFE_DELETE(pBlur);
	}

	// 要素をクリア
	m_oldObject.clear();

	// オブジェクトを破棄
	Release();
}

//============================================================
//	更新処理
//============================================================
void CBlur::Update(void)
{
	// 残像の生成
	{
		SInfo *pTempBlur = new SInfo;	// ブラー情報
		assert(pTempBlur != nullptr);	// 生成失敗エラー

		// ブラー情報のメモリクリア
		memset(pTempBlur, 0, sizeof(*pTempBlur));

		// パーツ数を設定
		pTempBlur->nNumParts = m_pParent->GetNumModel();

		for (int nCntParts = 0; nCntParts < pTempBlur->nNumParts; nCntParts++)
		{ // パーツ数分繰り返す

			// パーツの生成
			pTempBlur->apCharaParts[nCntParts] = CObjectModel::Create(VEC3_ZERO, VEC3_ZERO);
			if (pTempBlur->apCharaParts[nCntParts] != nullptr)
			{ // パーツの生成に成功した場合

				CObjectModel *pBlurParts = pTempBlur->apCharaParts[nCntParts];		// 残像パーツ
				CMultiModel *pOriginParts = m_pParent->GetMultiModel(nCntParts);	// 原点パーツ
				D3DXMATRIX mtxParts = pOriginParts->GetMtxWorld();					// 残像生成元のマトリックス

				// 情報を生成元と一致させる
				pBlurParts->BindModel(pOriginParts->GetModelID());	// 同一モデルを割当
				pBlurParts->SetMtxWorld(mtxParts);					// 同一マトリックスを設定

				// 位置・向き・大きさをマトリックスから取得し設定
				pBlurParts->SetVec3Position(useful::GetMatrixPosition(mtxParts));
				pBlurParts->SetVec3Rotation(useful::GetMatrixRotation(mtxParts));
				pBlurParts->SetVec3Scaling(useful::GetMatrixScaling(mtxParts));
			}
		}

		// 配列の最後尾に残像を追加
		m_oldObject.push_back(pTempBlur);
	}

	// 残像の色反映
	{
		int nCntBlur = 0;	// ブラーインデックス
		for (SInfo *pBlur : m_oldObject)
		{ // 要素数分繰り返す

			D3DXMATERIAL matSet = m_mat;	// 設定マテリアル

			// マテリアルの透明度を後ろになるにつれて下げていく
			matSet.MatD3D.Diffuse.a = (m_fStartAlpha / m_nMaxLength) * nCntBlur;

			for (int nCntParts = 0; nCntParts < pBlur->nNumParts; nCntParts++)
			{ // パーツ数分繰り返す

				// パーツの全マテリアルを変更
				pBlur->apCharaParts[nCntParts]->SetAllMaterial(matSet);
			}

			// ブラーインデックスを進める
			nCntBlur++;
		}
	}

	// 残像の削除
	{
		if ((int)m_oldObject.size() > m_nMaxLength)
		{ // 残像が最大数を超えた場合

			SInfo *pDelete = m_oldObject.front();	// 残像配列の先頭
			for (int nCntParts = 0; nCntParts < pDelete->nNumParts; nCntParts++)
			{ // パーツ数分繰り返す

				// パーツの終了
				SAFE_UNINIT(pDelete->apCharaParts[nCntParts]);
			}

			// パーツ格納情報の破棄
			SAFE_DELETE(pDelete);

			// 残像の最後尾を配列から削除
			m_oldObject.erase(m_oldObject.begin());
		}
	}
}

//============================================================
//	描画処理
//============================================================
void CBlur::Draw(CShader *pShader)
{
	for (SInfo *pBlur : m_oldObject)
	{ // 要素数分繰り返す

		for (int nCntParts = 0; nCntParts < pBlur->nNumParts; nCntParts++)
		{ // パーツ数分繰り返す

			// パーツの描画
			pBlur->apCharaParts[nCntParts]->Draw(pShader);
		}
	}
}

//============================================================
//	生成処理
//============================================================
CBlur *CBlur::Create
(
	CObjectChara *pParent,		// 親オブジェクト
	D3DXMATERIAL& rMat,			// ブラーマテリアル
	const float fStartAlpha,	// ブラー開始透明度
	const int nMaxLength		// 保持オブジェクト最大数
)
{
	// ブラーの生成
	CBlur *pBlur = new CBlur;
	if (pBlur == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// ブラーの初期化
		if (FAILED(pBlur->Init()))
		{ // 初期化に失敗した場合

			// ブラーの破棄
			SAFE_DELETE(pBlur);
			return nullptr;
		}

		// 親オブジェクトを設定
		pBlur->m_pParent = pParent;

		// ブラーマテリアルを設定
		pBlur->SetMaterial(rMat);

		// ブラーの開始透明度を設定
		pBlur->SetStartAlpha(fStartAlpha);

		// 保持オブジェクトの最大数を設定
		pBlur->SetMaxLength(nMaxLength);

		// 確保したアドレスを返す
		return pBlur;
	}
}

//============================================================
//	破棄処理
//============================================================
void CBlur::Release(void)
{
	// オブジェクトの破棄
	CObject::Release();
}
