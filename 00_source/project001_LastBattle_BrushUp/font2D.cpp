//============================================================
//
//	フォント2D処理 [font2D.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "font2D.h"

//************************************************************
//	子クラス [CFont2D] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CFont2D::CFont2D(const CObject::ELabel label, const EDim dimension, const int nPriority) : CObject2D(label, dimension, nPriority),
	m_fHeight	(0.0f)	// 縦幅
{

}

//============================================================
//	デストラクタ
//============================================================
CFont2D::~CFont2D()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CFont2D::Init(void)
{
	// メンバ変数を初期化
	m_fHeight = 0.0f;	// 縦幅

	// オブジェクト2Dの初期化
	if (FAILED(CObject2D::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CFont2D::Uninit(void)
{
	// オブジェクト2Dの終了
	CObject2D::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CFont2D::Update(void)
{
	// オブジェクト2Dの更新
	CObject2D::Update();
}

//============================================================
//	描画処理
//============================================================
void CFont2D::Draw(CShader *pShader)
{
	// オブジェクト2Dの描画
	CObject2D::Draw(pShader);
}

//============================================================
//	生成処理
//============================================================
CFont2D *CFont2D::Create
(
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rSize,	// 大きさ
	const D3DXVECTOR3& rRot,	// 向き
	const D3DXCOLOR& rCol		// 色
)
{
	// フォント2Dの生成
	CFont2D *pFont2D = new CFont2D;
	if (pFont2D == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// フォント2Dの初期化
		if (FAILED(pFont2D->Init()))
		{ // 初期化に失敗した場合

			// フォント2Dの破棄
			SAFE_DELETE(pFont2D);
			return nullptr;
		}

		// 位置を設定
		pFont2D->SetVec3Position(rPos);

		// 向きを設定
		pFont2D->SetVec3Rotation(rRot);

		// 大きさを設定
		pFont2D->SetVec3Sizing(rSize);

		// 色を設定
		pFont2D->SetColor(rCol);

		// 確保したアドレスを返す
		return pFont2D;
	}
}

//============================================================
//	位置の設定処理
//============================================================
void CFont2D::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// 位置の設定
	CObject2D::SetVec3Position(rPos);

	// フォントのテクスチャ座標の設定
	//CObject2D::SetFontTex(m_nPattern, m_nWidthPtrn, m_nHeightPtrn);
}

//============================================================
//	向きの設定処理
//============================================================
void CFont2D::SetVec3Rotation(const D3DXVECTOR3& rRot)
{
	// 向きの設定
	CObject2D::SetVec3Rotation(rRot);

	// フォントのテクスチャ座標の設定
	//CObject2D::SetFontTex(m_nPattern, m_nWidthPtrn, m_nHeightPtrn);
}

//============================================================
//	大きさの設定処理
//============================================================
void CFont2D::SetVec3Sizing(const D3DXVECTOR3& /*rSize*/)
{
	// 大きさの設定関数は比率が変わってしまうので使用できない
	// 大きさ変更の際は同クラスの SetHegiht を利用してください
	assert(false);
}

//============================================================
//	色の設定処理
//============================================================
void CFont2D::SetColor(const D3DXCOLOR& rCol)
{
	// 色の設定
	CObject2D::SetColor(rCol);

	// フォントのテクスチャ座標の設定
	//CObject2D::SetFontTex(m_nPattern, m_nWidthPtrn, m_nHeightPtrn);
}

//============================================================
//	縦幅の設定処理
//============================================================
void CFont2D::SetHeight(const float fHeight)
{
	// 引数の縦幅を設定
	m_fHeight = fHeight;

	// 縦幅を基準に横幅を求める
	float fWidth = useful::GetTexWidthFromAspect(fHeight, GetTexID());

	// 大きさを設定
	SetVec3Sizing(D3DXVECTOR3(fWidth, fHeight, 0.0f));
}
