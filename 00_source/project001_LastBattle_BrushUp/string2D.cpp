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
	CTexture *pTexture = GET_MANAGER->GetTexture();	// テクスチャ情報

	{
#if 0
		int nTexID = GetTextureIndex();	// フォントのテクスチャインデックス
		float fTexWidth = useful::GetTexWidthFromAspect(fHeight, nTexID);	// テクスチャ横幅
		D3DXIMAGE_INFO status = pTexture->GetInfo(nTexID).status;			// テクスチャステータス

		// 縦幅の増減割合を保存
		m_fSizeRate = fHeight / (float)status.Height;

		// 大きさを設定
		SetVec3Sizing(D3DXVECTOR3(fTexWidth, fHeight, 0.0f));
#endif
	}
}

//============================================================
//	文字縦幅の取得処理
//============================================================
float CString2D::GetHeight(void) const
{
	return 0.0f;
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

		// 原点位置を設定
		pString2D->SetVec3Position(rPos);

		// 文字縦幅を設定
		pString2D->SetHeight(fHeight);

		// フォント・文字列を設定
		if (FAILED(pString2D->SetFontString(pFontChar, wsStr, fHeight)))
		{ // 設定に失敗した場合

			// 文字列2Dの破棄
			SAFE_DELETE(pString2D);
			return nullptr;
		}

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
	std::wstring wsStr,		// 指定文字列
	const float fHeight		// 文字縦幅
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

	// 最初の文字のX座標
	float fPosX = m_pos.x;
	for (int i = 0; i < nCurStrLen; i++)
	{ // 生成する文字数分繰り返す

		// 文字を生成
		D3DXVECTOR3 pos = D3DXVECTOR3(fPosX, m_pos.y, 0.0f);
		m_ppChar[i] = CChar2D::Create
		( // 引数
			pFontChar,
			m_wsStr[i],
			pos,
			fHeight
		);
		if (m_ppChar[i] == nullptr)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// オフセット分ずらす
		D3DXVECTOR3 old = m_ppChar[i]->GetVec3Position();
		old.x += m_ppChar[i]->GetOffset();
		m_ppChar[i]->SetVec3Position(old);

		// 次の位置設定用に原点を保存
		fPosX = m_ppChar[i]->GetVec3Position().x - m_ppChar[i]->GetOffset() + m_ppChar[i]->GetNext();
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
