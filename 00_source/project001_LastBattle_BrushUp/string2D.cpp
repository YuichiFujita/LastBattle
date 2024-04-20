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
	m_ppChar	(nullptr),		// 文字ポリゴンの情報
	m_pos		(VEC3_ZERO),	// 位置
	m_origin	(ORIGIN_CENTER)	// 原点

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
	m_ppChar	= nullptr;			// 文字ポリゴンの情報
	m_pos		= VEC3_ZERO;		// 位置
	m_origin	= ORIGIN_CENTER;	// 原点

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
	D3DXVECTOR3 rot = GetVec3Rotation();
	rot.z += 0.01f;
	SetVec3Rotation(rot);

	// 相対位置の設定
	SetPositionRelative();
}

//============================================================
//	描画処理
//============================================================
void CString2D::Draw(CShader * /*pShader*/)
{

}

//============================================================
//	位置の設定処理
//============================================================
void CString2D::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// 引数の位置を設定
	m_pos = rPos;

	// 相対位置の設定
	SetPositionRelative();
}

//============================================================
//	位置の取得処理
//============================================================
D3DXVECTOR3 CString2D::GetVec3Position(void) const
{
	// 位置を返す
	return m_pos;
}

//============================================================
//	向きの設定処理
//============================================================
void CString2D::SetVec3Rotation(const D3DXVECTOR3& rRot)
{
	for (int i = 0; i < (int)m_wsStr.size(); i++)
	{ // 文字数分繰り返す

		// 文字向きの設定
		assert(m_ppChar[i] != nullptr);
		m_ppChar[i]->SetVec3Rotation(rRot);
	}

	// 相対位置の設定
	SetPositionRelative();
}

//============================================================
//	向き取得処理
//============================================================
D3DXVECTOR3 CString2D::GetVec3Rotation(void) const
{
	// 文字列がない場合抜ける
	if ((int)m_wsStr.size() <= 0) { assert(false); return VEC3_ZERO; }

	// 先頭の向きを返す
	assert(m_ppChar[0] != nullptr);
	return m_ppChar[0]->GetVec3Rotation();
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
		//m_ppChar[i]->BindTexture(-1);
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

	// 先頭の文字縦幅を返す
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
	const float fHeight,		// 文字縦幅
	const EOrigin origin,		// 原点
	const D3DXVECTOR3& rRot		// 原点向き
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

		// 原点向きを設定
		pString2D->SetVec3Rotation(rRot);

		// 文字縦幅を設定
		pString2D->SetHeight(fHeight);

		// 原点を設定
		pString2D->SetOrigin(origin);

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
//	文字列の横幅取得処理
//============================================================
float CString2D::GetStrWidth(void)
{
	// 文字列がない場合抜ける
	if ((int)m_wsStr.size() <= 0) { assert(false); return 0.0f; }

	float fStrWidth = 0.0f;	// 文字列の横幅
	int nEndCharID = (int)m_wsStr.size() - 1;	// 終端文字のインデックス

	for (int i = 0; i < nEndCharID; i++)
	{ // 終端文字を抜いた文字数分繰り返す

		// 次の文字までの距離を加算
		assert(m_ppChar[i] != nullptr);
		fStrWidth += m_ppChar[i]->GetNext();
	}

	// 先頭文字の無視した大きさを加算
	assert(m_ppChar[0] != nullptr);
	fStrWidth += (m_ppChar[0]->GetVec3Sizing().x * 0.5f) - m_ppChar[0]->GetOffset();

	// 終端文字の大きさを加算
	assert(m_ppChar[nEndCharID] != nullptr);
	float fEndCharWidth = m_ppChar[nEndCharID]->GetVec3Sizing().x;	// 終端文字の横幅
	fStrWidth += fEndCharWidth - ((fEndCharWidth * 0.5f) - m_ppChar[nEndCharID]->GetOffset());

	// 文字列の横幅を返す
	return fStrWidth;
}

//============================================================
//	原点の設定処理
//============================================================
void CString2D::SetOrigin(const EOrigin origin)
{
	// 引数の原点を設定
	m_origin = origin;

	// 相対位置の設定
	SetPositionRelative();
}

//============================================================
//	原点取得処理
//============================================================
CString2D::EOrigin CString2D::GetOrigin(void) const
{
	// 原点を返す
	return m_origin;
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
	// 文字列がない場合抜ける
	if ((int)m_wsStr.size() <= 0) { assert(false); return; }

#if 0
	assert(m_ppChar[0] != nullptr);
	float fStrWidth = GetStrWidth() * 0.5f;	// 文字列全体の横幅
	float fHeadWidth = m_ppChar[0]->GetVec3Sizing().x * 0.5f;	// 先頭文字の横幅
	float fStartPosX = m_pos.x - fStrWidth + (fHeadWidth - m_ppChar[0]->GetOffset());	// 開始X座標

	// 開始X座標を原点に応じてずらす
	fStartPosX -= fStrWidth * (m_origin - 1);

	for (int i = 0; i < (int)m_wsStr.size(); i++)
	{ // 文字数分繰り返す

		// 文字の設定座標
		D3DXVECTOR3 posChar = D3DXVECTOR3(fStartPosX, m_pos.y, 0.0f);

		// 設定座標に原点オフセットを与える
		assert(m_ppChar[i] != nullptr);
		posChar.x += m_ppChar[i]->GetOffset();

		// 位置を反映
		m_ppChar[i]->SetVec3Position(posChar);

		// 次の設定座標の開始点を保存
		fStartPosX = m_ppChar[i]->GetVec3Position().x - m_ppChar[i]->GetOffset() + m_ppChar[i]->GetNext();
	}
#else
#if 0
	assert(m_ppChar[0] != nullptr);
	float fStrWidth = GetStrWidth() * 0.5f;	// 文字列全体の横幅
	float fHeadRot = m_ppChar[0]->GetVec3Rotation().z - HALF_PI;	// 先頭文字の向き
	float fHeadWidth = m_ppChar[0]->GetVec3Sizing().x * 0.5f;		// 先頭文字の横幅

	D3DXVECTOR3 posStart = VEC3_ZERO;
	posStart.x = m_pos.x - sinf(fHeadRot) * ((fStrWidth + fHeadWidth - m_ppChar[0]->GetOffset()) * (fStrWidth * (m_origin - 1)));
	posStart.y = m_pos.y - cosf(fHeadRot) * ((fStrWidth + fHeadWidth - m_ppChar[0]->GetOffset()) * (fStrWidth * (m_origin - 1)));

	for (int i = 0; i < (int)m_wsStr.size(); i++)
	{ // 文字数分繰り返す

#if 0
		// 設定座標に原点オフセットを与える
		assert(m_ppChar[i] != nullptr);
		D3DXVECTOR3 posChar = VEC3_ZERO;
		posChar.x = posStart.x + sinf(fHeadRot) * m_ppChar[i]->GetOffset();
		posChar.y = posStart.y + cosf(fHeadRot) * m_ppChar[i]->GetOffset();

		// 位置を反映
		m_ppChar[i]->SetVec3Position(posChar);

		// 次の設定座標の開始点を保存
		posStart.x = posChar.x - sinf(fHeadRot) * (m_ppChar[i]->GetOffset() + m_ppChar[i]->GetNext());
		posStart.y = posChar.y - cosf(fHeadRot) * (m_ppChar[i]->GetOffset() + m_ppChar[i]->GetNext());
#else
		// 設定座標に原点オフセットを与える
		assert(m_ppChar[i] != nullptr);
		D3DXVECTOR3 posChar;
		posChar.x = posStart.x + sinf(fHeadRot) * m_ppChar[i]->GetOffset();
		posChar.y = posStart.y + cosf(fHeadRot) * m_ppChar[i]->GetOffset();

		// 位置を反映
		m_ppChar[i]->SetVec3Position(posChar);

		// 次の設定座標の開始点を保存
		posStart.x = m_ppChar[i]->GetVec3Position().x - sinf(fHeadRot) * (m_ppChar[i]->GetOffset() + m_ppChar[i]->GetNext());
		posStart.y = m_ppChar[i]->GetVec3Position().y - cosf(fHeadRot) * (m_ppChar[i]->GetOffset() + m_ppChar[i]->GetNext());
#endif
	}
#else
	assert(m_ppChar[0] != nullptr);
	float fStrWidth = GetStrWidth() * 0.5f;	// 文字列全体の横幅
	float fHeadRot = m_ppChar[0]->GetVec3Rotation().z - HALF_PI;	// 先頭文字の向き
	float fHeadWidth = m_ppChar[0]->GetVec3Sizing().x * 0.5f;		// 先頭文字の横幅

#if 1
	D3DXVECTOR3 posStart;
	posStart.x = m_pos.x + sinf(fHeadRot) * (fStrWidth - (fHeadWidth - m_ppChar[0]->GetOffset()) + (fStrWidth * (m_origin - 1)));
	posStart.y = m_pos.y + cosf(fHeadRot) * (fStrWidth - (fHeadWidth - m_ppChar[0]->GetOffset()) + (fStrWidth * (m_origin - 1)));
#else
	float fStartPosX = fStrWidth + (fHeadWidth - m_ppChar[0]->GetOffset());	// 開始X座標

	// 開始X座標を原点に応じてずらす
	fStartPosX -= fStrWidth * (m_origin - 1);

	D3DXVECTOR3 posStart = VEC3_ZERO;
	posStart.x = m_pos.x + sinf(fHeadRot) * fStartPosX;
	posStart.y = m_pos.y + cosf(fHeadRot) * fStartPosX;
#endif

	//for (int i = 0; i < 1; i++)
	for (int i = 0; i < (int)m_wsStr.size(); i++)
	{ // 文字数分繰り返す

#if 1
		// 設定座標に原点オフセットを与える
		assert(m_ppChar[i] != nullptr);
		posStart.x -= sinf(fHeadRot) * m_ppChar[i]->GetOffset();
		posStart.y -= cosf(fHeadRot) * m_ppChar[i]->GetOffset();

		// 位置を反映
		m_ppChar[i]->SetVec3Position(posStart);

		// 次の設定座標の開始点を保存
		posStart.x += sinf(fHeadRot) * (m_ppChar[i]->GetOffset() - m_ppChar[i]->GetNext());
		posStart.y += cosf(fHeadRot) * (m_ppChar[i]->GetOffset() - m_ppChar[i]->GetNext());
#else

#endif
	}
#endif
#endif
}
