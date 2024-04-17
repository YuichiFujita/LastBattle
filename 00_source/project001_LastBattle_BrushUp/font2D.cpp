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
#include "manager.h"
#include "texture.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int PRIORITY = 6;	// フォント2Dの優先順位
}

//************************************************************
//	子クラス [CFont2D] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CFont2D::CFont2D() : CObject2D(CObject::LABEL_UI, CObject::DIM_2D, PRIORITY),
	m_uChar			(0),	// 指定文字
	m_fSizeRate		(0.0f),	// 縦幅の割合
	m_fAbsOriginX	(0.0f)	// X原点オフセットの絶対値
{
	// メンバ変数をクリア
	memset(&m_infoChar, 0, sizeof(m_infoChar));	// 文字情報
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
	memset(&m_infoChar, 0, sizeof(m_infoChar));	// 文字情報
	m_uChar			= 0;	// 指定文字
	m_fSizeRate		= 1.0f;	// 縦幅の割合
	m_fAbsOriginX	= 0.0f;	// X原点オフセットの絶対値

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
//	大きさの設定処理
//============================================================
void CFont2D::SetVec3Sizing(const D3DXVECTOR3& /*rSize*/)
{
	/*
		こっちで大きさを自由に変えられると
		文字の比率がおかしくなるのと、
		縦幅の割合なども変更できないため
		大きさ変更は SetHeight を利用してね。
	*/

	assert(false);
}

//============================================================
//	縦幅の設定処理
//============================================================
void CFont2D::SetHeight(const float fHeight)
{
	int nTexID = GetTextureIndex();	// フォントのテクスチャインデックス
	float fTexWidth = useful::GetTexWidthFromAspect(fHeight, nTexID);			// テクスチャ横幅
	D3DXIMAGE_INFO status = GET_MANAGER->GetTexture()->GetInfo(nTexID).status;	// テクスチャステータス

	// 縦幅の増減割合を保存
	m_fSizeRate = fHeight / (float)status.Height;

	// 大きさを設定
	CObject2D::SetVec3Sizing(D3DXVECTOR3(fTexWidth, fHeight, 0.0f));
}

//============================================================
//	生成処理
//============================================================
CFont2D *CFont2D::Create
(
	CFontChar *pFontChar,		// フォント文字情報
	const UINT uChar,			// 指定文字
	const D3DXVECTOR3& rPos,	// 位置
	const float fHeight,		// 縦幅
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

		// フォント・文字を設定
		pFont2D->SetFontChar(pFontChar, uChar);

		// 位置を設定
		pFont2D->SetVec3Position(rPos);

		// 向きを設定
		pFont2D->SetVec3Rotation(rRot);

		// 縦幅を設定
		pFont2D->SetHeight(fHeight);

		// 色を設定
		pFont2D->SetColor(rCol);

		// 確保したアドレスを返す
		return pFont2D;
	}
}

//============================================================
//	フォント・文字の設定処理
//============================================================
void CFont2D::SetFontChar
(
	CFontChar *pFontChar,	// フォント文字情報
	const UINT uChar		// 指定文字
)
{
	// フォント文字の取得
	m_infoChar = pFontChar->Regist(uChar);

	// 指定文字を保存
	m_uChar = uChar;

	// X原点オフセットの絶対値を保存
	m_fAbsOriginX = fabsf((float)m_infoChar.glyph.gmptGlyphOrigin.x);

	// テクスチャを作成・割当
	BindTexture(m_infoChar.nTexID);
}

//============================================================
//	原点のオフセット取得処理
//============================================================
float CFont2D::GetOffset(void)
{
	// 文字ブラックボックスの横幅
	float fBlackBoxX = (float)m_infoChar.glyph.gmBlackBoxX;

	// 文字原点のオフセットを返す
	return (m_fAbsOriginX + fBlackBoxX * 0.5f) * m_fSizeRate;
}

//============================================================
//	次の文字までの距離取得処理
//============================================================
float CFont2D::GetNext(void)
{
	// 次の文字の距離を返す
	return (float)m_infoChar.glyph.gmCellIncX * m_fSizeRate;
}
