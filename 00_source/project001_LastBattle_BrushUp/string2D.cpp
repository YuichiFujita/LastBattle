//============================================================
//
//	文字列2D処理 [string2D.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "string2D.h"
#include "manager.h"
#include "texture.h"
#include "fontChar.h"
#include "char2D.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int PRIORITY = 5;	// 文字列2Dの優先順位
}

//************************************************************
//	子クラス [CString2D] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CString2D::CString2D() : CObject(CObject::LABEL_UI, CObject::DIM_2D, PRIORITY),
	m_ppChar	(nullptr),	// 文字ポリゴンの情報
	m_pos		(VEC3_ZERO)	// 文字列の原点位置
{

}

//============================================================
//	デストラクタ
//============================================================
CString2D::~CString2D()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CString2D::Init(void)
{
	// メンバ変数を初期化
	m_ppChar	= nullptr;		// 文字ポリゴンの情報
	m_pos		= VEC3_ZERO;	// 文字列の原点位置

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CString2D::Uninit(void)
{
	for (int i = 0; i < (int)m_wsStr.size(); i++)
	{ // 文字数分繰り返す

		// 文字を破棄
		SAFE_UNINIT(m_ppChar[i]);
	}

	// 文字列を破棄
	SAFE_DEL_ARRAY(m_ppChar);

	// オブジェクトを破棄
	Release();
}

//============================================================
//	更新処理
//============================================================
void CString2D::Update(void)
{
	// TODO
	if (GET_INPUTKEY->IsPress(DIK_W))
	{
		m_pos.y -= 1.0f;
	}
	if (GET_INPUTKEY->IsPress(DIK_S))
	{
		m_pos.y += 1.0f;
	}
	if (GET_INPUTKEY->IsPress(DIK_A))
	{
		m_pos.x -= 1.0f;
	}
	if (GET_INPUTKEY->IsPress(DIK_D))
	{
		m_pos.x += 1.0f;
	}

	SetPositionRelative();
}

//============================================================
//	描画処理
//============================================================
void CString2D::Draw(CShader * /*pShader*/)
{

}

//============================================================
//	原点位置の設定処理
//============================================================
void CString2D::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// 引数の位置を設定
	m_pos = rPos;

	// 相対位置の設定
	SetPositionRelative();
}

//============================================================
//	原点位置の取得処理
//============================================================
D3DXVECTOR3 CString2D::GetVec3Position(void) const
{
	// 位置を返す
	return m_pos;
}

//============================================================
//	文字縦幅の設定処理
//============================================================
void CString2D::SetHeight(const float fHeight)
{
	for (int i = 0; i < (int)m_wsStr.size(); i++)
	{ // 文字数分繰り返す

		// 文字縦幅の設定
		assert(m_ppChar[i] != nullptr);
		m_ppChar[i]->SetHeight(fHeight);

		// TODO
		m_ppChar[i]->BindTexture(-1);
	}

	// 相対位置の設定
	SetPositionRelative();
}

//============================================================
//	文字縦幅の取得処理
//============================================================
float CString2D::GetHeight(void) const
{
	// 文字列がない場合抜ける
	if ((int)m_wsStr.size() <= 0) { assert(false); return 0.0f; }

	// 先頭の文字縦幅の返す
	assert(m_ppChar[0] != nullptr);
	return m_ppChar[0]->GetHeight();
}

//============================================================
//	生成処理
//============================================================
CString2D *CString2D::Create
(
	CFontChar *pFontChar,		// フォント文字情報
	std::wstring wsStr,			// 指定文字列
	const D3DXVECTOR3 &rPos,	// 原点位置
	const float fHeight			// 文字縦幅
)
{
	// 文字列2Dの生成
	CString2D *pString2D = new CString2D;
	if (pString2D == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// 文字列2Dの初期化
		if (FAILED(pString2D->Init()))
		{ // 初期化に失敗した場合

			// 文字列2Dの破棄
			SAFE_DELETE(pString2D);
			return nullptr;
		}

		// フォント・文字列を設定
		if (FAILED(pString2D->SetFontString(pFontChar, wsStr)))
		{ // 設定に失敗した場合

			// 文字列2Dの破棄
			SAFE_DELETE(pString2D);
			return nullptr;
		}

		// 原点位置を設定
		pString2D->SetVec3Position(rPos);

		// 文字縦幅を設定
		pString2D->SetHeight(fHeight);

		// 確保したアドレスを返す
		return pString2D;
	}
}

//============================================================
//	フォント・文字列の設定処理
//============================================================
HRESULT CString2D::SetFontString
(
	CFontChar *pFontChar,	// フォント文字情報
	std::wstring wsStr		// 指定文字列
)
{
	// 文字数を保存する
	int nOldStrLen = (int)m_wsStr.size();	// 破棄する文字数
	int nCurStrLen = (int)wsStr.size();		// 生成する文字数

	// 指定文字列を変更
	m_wsStr = wsStr;

	//--------------------------------------------------------
	//	既に使用している文字を破棄
	//--------------------------------------------------------
	for (int i = 0; i < nOldStrLen; i++)
	{ // 破棄する文字数分繰り返す

		// 文字を破棄
		SAFE_UNINIT(m_ppChar[i]);
	}

	// 文字列を破棄
	SAFE_DEL_ARRAY(m_ppChar);

	//--------------------------------------------------------
	//	新しい文字を生成
	//--------------------------------------------------------
	// 文字を格納する配列を生成
	m_ppChar = new CChar2D*[nCurStrLen];
	for (int i = 0; i < nCurStrLen; i++)
	{ // 生成する文字数分繰り返す

		// 文字を生成
		m_ppChar[i] = CChar2D::Create
		( // 引数
			pFontChar,	// フォント文字情報
			m_wsStr[i]	// 指定文字
		);
		if (m_ppChar[i] == nullptr)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	破棄処理
//============================================================
void CString2D::Release(void)
{
	// オブジェクトの破棄
	CObject::Release();
}

//============================================================
//	相対位置の設定処理
//============================================================
void CString2D::SetPositionRelative(void)
{
#if 0
	float fPosX = m_pos.x;	// 最初の文字のX座標
	for (int i = 0; i < (int)m_wsStr.size(); i++)
	{ // 文字数分繰り返す

		// オフセット分ずらす
		D3DXVECTOR3 pos = D3DXVECTOR3(fPosX, m_pos.y, 0.0f);
		pos.x += m_ppChar[i]->GetOffset();
		m_ppChar[i]->SetVec3Position(pos);

		// 次の位置設定用に原点を保存
		fPosX = m_ppChar[i]->GetVec3Position().x - m_ppChar[i]->GetOffset() + m_ppChar[i]->GetNext();
	}
#else
	float fPosX = m_pos.x - (GetStrWidth() * 0.5f) + ((m_ppChar[0]->GetVec3Sizing().x * 0.5f) - m_ppChar[0]->GetOffset());	// 最初の文字のX座標
	for (int i = 0; i < (int)m_wsStr.size(); i++)
	{ // 文字数分繰り返す

		// オフセット分ずらす
		D3DXVECTOR3 pos = D3DXVECTOR3(fPosX, m_pos.y, 0.0f);
		pos.x += m_ppChar[i]->GetOffset();
		m_ppChar[i]->SetVec3Position(pos);

		// 次の位置設定用に原点を保存
		fPosX = m_ppChar[i]->GetVec3Position().x - m_ppChar[i]->GetOffset() + m_ppChar[i]->GetNext();
	}
#endif
}

//============================================================
//	文字列の横幅取得処理
//============================================================
float CString2D::GetStrWidth(void)
{
	float fWidth = 0.0f;

#if 1
#if 0
	for (int i = 0; i < (int)m_wsStr.size(); i++)
	{ // 文字数分繰り返す

		assert(m_ppChar[i] != nullptr);
		fWidth += m_ppChar[i]->GetNext();
	}

	// 先頭文字の無視した大きさを加算
	fWidth -= m_ppChar[0]->GetVec3Sizing().x * 0.5f - m_ppChar[0]->GetOffset();

	// 終端文字の減算
	int n = (int)m_wsStr.size() - 1;
	float f = m_ppChar[n]->GetVec3Sizing().x * 0.5f - m_ppChar[n]->GetOffset();
	fWidth -= m_ppChar[n]->GetNext() - m_ppChar[n]->GetVec3Sizing().x - f - 1.0f;
#else
	for (int i = 0; i < (int)m_wsStr.size() - 1; i++)
	{ // 文字数分繰り返す

		assert(m_ppChar[i] != nullptr);
		fWidth += m_ppChar[i]->GetNext();
	}

	// 先頭文字の無視した大きさを加算
	fWidth += m_ppChar[0]->GetVec3Sizing().x * 0.5f - m_ppChar[0]->GetOffset();

	// 
	int n = (int)m_wsStr.size() - 1;
	float f = m_ppChar[n]->GetVec3Sizing().x * 0.5f - m_ppChar[n]->GetOffset();
	fWidth += m_ppChar[n]->GetVec3Sizing().x - f;

	//// 終端文字の減算
	//int n = (int)m_wsStr.size() - 1;
	//float f = m_ppChar[n]->GetVec3Sizing().x * 0.5f - m_ppChar[n]->GetOffset();
	//fWidth -= m_ppChar[n]->GetNext() - m_ppChar[n]->GetVec3Sizing().x - f - 1.0f;
#endif
#else
	float fMinPosX = 0.0f;
	float fMaxPosX = 0.0f;
	for (int i = 0; i < (int)m_wsStr.size(); i++)
	{ // 文字数分繰り返す

		assert(m_ppChar[i] != nullptr);
		fWidth += m_ppChar[i]->GetNext();
	}
#endif

	return fWidth;
}
