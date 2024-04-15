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
	const float REV_SCALE = 1.0f;	// 大きさの補正係数
}

//************************************************************
//	子クラス [CFont2D] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CFont2D::CFont2D(const CObject::ELabel label, const EDim dimension, const int nPriority) : CObject2D(label, dimension, nPriority),
	m_fScale	(0.0f)	// 拡大率
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
	m_fScale = 1.0f;	// 拡大率

	// オブジェクト2Dの初期化
	if (FAILED(CObject2D::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// TODO
	SetPriority(7);
	SetLabel(CObject::LABEL_UI);

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
	CFontChar *pFontChar,		// フォント文字情報
	const UINT uChar,			// 指定文字
	const D3DXVECTOR3& rPos,	// 位置
	const float fScale,			// 拡大率
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

		// 拡大率を設定
		pFont2D->SetScale(fScale);

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
	if (uChar == L'o')
	{
		int a = 0;
	}

	// フォント文字の取得
	m_infoChar = pFontChar->Regist(uChar);

	// テクスチャを作成・割当
	BindTexture(m_infoChar.nTexID);
}

//============================================================
//	位置の設定処理
//============================================================
void CFont2D::SetVec3Position(const D3DXVECTOR3& rPos)
{
	D3DXVECTOR3 pos = rPos;
	//float fBaseLine = (float)m_infoChar.text.tmHeight - (float)m_infoChar.text.tmAscent;
	//float fLineDown = (float)m_infoChar.glyph.gmptGlyphOrigin.y - (float)m_infoChar.glyph.gmBlackBoxY;
	//float fDown = /*fBaseLine*/ - fLineDown;

	//static int n = 0;
	//if (n < 16)
	{

		// TODO：位置動かすのどうにか
		//pos.x -= ((float)m_infoChar.glyph.gmptGlyphOrigin.x * REV_SCALE);
		//pos.y += ((float)m_infoChar.text.tmDescent - (float)m_infoChar.glyph.gmptGlyphOrigin.y) * REV_SCALE * 0.5f;

		// 位置の設定
		CObject2D::SetVec3Position(pos);

		//n++;
	}
	//else
	//{
	//	CObject2D::SetVec3Position(pos);

	//}
}

//============================================================
//	向きの設定処理
//============================================================
void CFont2D::SetVec3Rotation(const D3DXVECTOR3& rRot)
{
	// 向きの設定
	CObject2D::SetVec3Rotation(rRot);
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
}

//============================================================
//	拡大率の設定処理
//============================================================
void CFont2D::SetScale(const float fScale)
{
	// テクスチャステータス情報
	D3DXIMAGE_INFO status = GET_MANAGER->GetTexture()->GetInfo(GetTextureIndex()).status;

	// 引数の拡大率を設定
	m_fScale = fScale;

	// 大きさを設定
	CObject2D::SetVec3Sizing(D3DXVECTOR3((float)status.Width, (float)status.Height, 0.0f) * fScale * REV_SCALE);
}
