//============================================================
//
//	テキスト2D処理 [text2D.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "text2D.h"
#include "manager.h"
#include "texture.h"
#include "fontChar.h"
#include "char2D.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int PRIORITY = 6;	// テキスト2Dの優先順位
}

//************************************************************
//	子クラス [CText2D] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CText2D::CText2D() : CObject(CObject::LABEL_UI, CObject::DIM_2D, PRIORITY),
	m_pFontChar		(nullptr),					// フォント文字
	m_pos			(VEC3_ZERO),				// 位置
	m_rot			(VEC3_ZERO),				// 向き
	m_col			(XCOL_WHITE),				// 色
	m_alignX		(CString2D::XALIGN_CENTER),	// 横配置
	m_alignY		(YALIGN_CENTER),			// 縦配置
	m_fCharHeight	(0.0f),						// 文字の縦幅
	m_fLineHeight	(0.0f)						// 文字の行間
{
	// 文字列リストをクリア
	m_listString.clear();
}

//============================================================
//	デストラクタ
//============================================================
CText2D::~CText2D()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CText2D::Init(void)
{
	// メンバ変数を初期化
	m_pFontChar		= nullptr;					// フォント文字
	m_pos			= VEC3_ZERO;				// 位置
	m_rot			= VEC3_ZERO;				// 向き
	m_col			= XCOL_WHITE;				// 色
	m_alignX		= CString2D::XALIGN_CENTER;	// 横配置
	m_alignY		= YALIGN_CENTER;			// 縦配置
	m_fCharHeight	= 0.0f;						// 文字の縦幅
	m_fLineHeight	= 0.0f;						// 文字の行間

	// 文字列リストを初期化
	m_listString.clear();

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CText2D::Uninit(void)
{
	for (auto& rList : m_listString)
	{ // 文字列の格納数分繰り返す

		// 文字列の終了
		SAFE_UNINIT(rList);
	}

	// 文字列リストをクリア
	m_listString.clear();

	// オブジェクトを破棄
	Release();
}

//============================================================
//	更新処理
//============================================================
void CText2D::Update(void)
{
	// 相対位置の設定
	SetPositionRelative();
}

//============================================================
//	描画処理
//============================================================
void CText2D::Draw(CShader * /*pShader*/)
{

}

//============================================================
//	位置の設定処理
//============================================================
void CText2D::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// 引数の位置を設定
	m_pos = rPos;

	// 相対位置の設定
	SetPositionRelative();
}

//============================================================
//	位置の取得処理
//============================================================
D3DXVECTOR3 CText2D::GetVec3Position(void) const
{
	// 位置を返す
	return m_pos;
}

//============================================================
//	向きの設定処理
//============================================================
void CText2D::SetVec3Rotation(const D3DXVECTOR3& rRot)
{
	// 設定する向きを保存
	m_rot = rRot;

	for (auto& rList : m_listString)
	{ // 文字列の格納数分繰り返す

		// 文字列向きの設定
		assert(rList != nullptr);
		rList->SetVec3Rotation(rRot);
	}

	// 相対位置の設定
	SetPositionRelative();
}

//============================================================
//	向き取得処理
//============================================================
D3DXVECTOR3 CText2D::GetVec3Rotation(void) const
{
	// 保存された向きを返す
	return m_rot;
}

//============================================================
//	色の設定処理
//============================================================
void CText2D::SetColor(const D3DXCOLOR& rCol)
{
	// 設定する色を保存
	m_col = rCol;

	for (auto& rList : m_listString)
	{ // 文字列の格納数分繰り返す

		// 文字列色の設定
		assert(rList != nullptr);
		rList->SetColor(rCol);
	}
}

//============================================================
//	色取得処理
//============================================================
D3DXCOLOR CText2D::GetColor(void) const
{
	// 保存された色を返す
	return m_col;
}

//============================================================
//	生成処理
//============================================================
CText2D *CText2D::Create
(
	CFontChar *pFontChar,				// フォント文字情報
	const D3DXVECTOR3& rPos,			// 原点位置
	const float fCharHeight,			// 文字縦幅
	const float fLineHeight,			// 行間縦幅
	const CString2D::EAlignX alignX,	// 横配置
	const EAlignY alignY,				// 縦配置
	const D3DXVECTOR3& rRot,			// 原点向き
	const D3DXCOLOR& rCol				// 色
)
{
	// テキスト2Dの生成
	CText2D *pText2D = new CText2D;
	if (pText2D == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// テキスト2Dの初期化
		if (FAILED(pText2D->Init()))
		{ // 初期化に失敗した場合

			// テキスト2Dの破棄
			SAFE_DELETE(pText2D);
			return nullptr;
		}

		// フォントを設定
		pText2D->SetFont(pFontChar);

		// 原点位置を設定
		pText2D->SetVec3Position(rPos);

		// 原点向きを設定
		pText2D->SetVec3Rotation(rRot);

		// 色を設定
		pText2D->SetColor(rCol);

		// 文字縦幅を設定
		pText2D->SetCharHeight(fCharHeight);

		// 行間縦幅を設定
		pText2D->SetLineHeight(fLineHeight);

		// 横配置を設定
		pText2D->SetAlignX(alignX);

		// 縦配置を設定
		pText2D->SetAlignY(alignY);

		// 確保したアドレスを返す
		return pText2D;
	}
}

//============================================================
//	フォントの設定処理
//============================================================
void CText2D::SetFont(CFontChar *pFontChar)
{
	// 引数のフォントを保存
	assert(pFontChar != nullptr);
	m_pFontChar = pFontChar;

	for (auto& rList : m_listString)
	{ // 文字列の格納数分繰り返す

		// 文字列フォントの設定
		assert(rList != nullptr);
		rList->SetFont(pFontChar);
	}

	// 相対位置の設定
	SetPositionRelative();
}

//============================================================
//	文字列の追加処理
//============================================================
HRESULT CText2D::AddString(const std::wstring& rStr)
{
	// 文字列オブジェクトを生成
	CString2D *pStr = CString2D::Create
	( // 引数
		m_pFontChar,	// フォント文字情報
		rStr,			// 指定文字列
		m_pos,			// 原点位置
		m_fCharHeight,	// 文字縦幅
		m_alignX,		// 横配置
		m_rot,			// 原点向き
		m_col			// 色
	);
	if (pStr == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 最後尾に生成した文字列を追加
	m_listString.push_back(pStr);

	// 相対位置の設定
	SetPositionRelative();

	// 成功を返す
	return S_OK;
}

//============================================================
//	文字列の取得処理
//============================================================
CString2D *CText2D::GetString2D(const int nStrID) const
{
	// 文字列がない場合抜ける
	int nStrSize = (int)m_listString.size();
	if (nStrSize <= 0) { assert(false); return nullptr; }

	// インデックスが範囲外の場合抜ける
	if (nStrID <= NONE_IDX || nStrID >= nStrSize) { assert(false); return nullptr; }

	// 引数インデックスの文字列を返す
	auto itr = m_listString.begin();	// 先頭イテレーター
	std::advance(itr, nStrID);			// イテレーターを進める
	return *itr;						// イテレーターの中身を返す
}

//============================================================
//	テキストの縦幅取得処理
//============================================================
float CText2D::GetTextHeight(void) const
{
	// 文字列がない場合抜ける
	if ((int)m_listString.size() <= 0) { assert(false); return 0.0f; }

	float fTextHeight = 0.0f;	// テキストの縦幅
	for (auto& rList : m_listString)
	{ // 文字列の格納数分繰り返す

		// 文字列縦幅を加算
		assert(rList != nullptr);
		fTextHeight += rList->GetHeight();
	}

	// テキスト内の全行間分を加算
	fTextHeight += m_fLineHeight * ((int)m_listString.size() - 1);

	// テキストの縦幅を返す
	return fTextHeight;
}

//============================================================
//	文字縦幅の設定処理
//============================================================
void CText2D::SetCharHeight(const float fHeight)
{
	// 設定する文字の縦幅を保存
	m_fCharHeight = fHeight;

	for (auto& rList : m_listString)
	{ // 文字列の格納数分繰り返す

		// 文字列縦幅の設定
		assert(rList != nullptr);
		rList->SetHeight(fHeight);
	}

	// 相対位置の設定
	SetPositionRelative();
}

//============================================================
//	文字縦幅の取得処理
//============================================================
float CText2D::GetCharHeight(void) const
{
	// 保存された文字の縦幅を返す
	return m_fCharHeight;
}

//============================================================
//	行間縦幅の設定処理
//============================================================
void CText2D::SetLineHeight(const float fHeight)
{
	// 引数の行間を設定
	m_fLineHeight = fHeight;

	// 相対位置の設定
	SetPositionRelative();
}

//============================================================
//	行間縦幅の取得処理
//============================================================
float CText2D::GetLineHeight(void) const
{
	// 行間を返す
	return m_fLineHeight;
}

//============================================================
//	横配置の設定処理
//============================================================
void CText2D::SetAlignX(const CString2D::EAlignX alignX)
{
	// 引数の横配置を設定
	m_alignX = alignX;

	for (auto& rList : m_listString)
	{ // 文字列の格納数分繰り返す

		// 文字列横幅の設定
		assert(rList != nullptr);
		rList->SetAlignX(alignX);
	}

	// 相対位置の設定
	SetPositionRelative();
}

//============================================================
//	横配置の取得処理
//============================================================
CString2D::EAlignX CText2D::GetAlignX(void) const
{
	// 横配置を返す
	return m_alignX;
}

//============================================================
//	縦配置の設定処理
//============================================================
void CText2D::SetAlignY(const EAlignY alignY)
{
	// 引数の縦配置を設定
	m_alignY = alignY;

	// 相対位置の設定
	SetPositionRelative();
}

//============================================================
//	縦配置の取得処理
//============================================================
CText2D::EAlignY CText2D::GetAlignY(void) const
{
	// 縦配置を返す
	return m_alignY;
}

//============================================================
//	破棄処理
//============================================================
void CText2D::Release(void)
{
	// オブジェクトの破棄
	CObject::Release();
}

//============================================================
//	相対位置の設定処理
//============================================================
void CText2D::SetPositionRelative(void)
{
	// 文字列がない場合抜ける
	if ((int)m_listString.size() <= 0) { return; }

	float fTextHeight	= GetTextHeight() * 0.5f;	// テキスト全体の縦幅
	float fFrontHeight	= m_listString.front()->GetHeight() * 0.5f;	// 先頭文字列の縦幅
	float fStartOffset	= -fTextHeight + fFrontHeight - (fTextHeight * (m_alignY - 1));	// 文字列の開始位置オフセット

	D3DXVECTOR3 posStart;	// 文字の開始位置
	posStart.x = m_pos.x + sinf(m_rot.z) * fStartOffset;	// 開始位置X
	posStart.y = m_pos.y + cosf(m_rot.z) * fStartOffset;	// 開始位置Y

	for (auto& rList : m_listString)
	{ // 文字列の格納数分繰り返す

		// 位置を反映
		assert(rList != nullptr);
		rList->SetVec3Position(posStart);

		// 設定座標にを与える
		posStart.x += sinf(m_rot.z) * (m_fLineHeight + (rList->GetHeight()));
		posStart.y += cosf(m_rot.z) * (m_fLineHeight + (rList->GetHeight()));
	}
}
