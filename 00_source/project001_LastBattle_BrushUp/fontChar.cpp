//============================================================
//
//	フォント文字処理 [fontChar.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "fontChar.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const MAT2 INIT_MATRIX = { { 0, 1 }, { 0, 0 }, { 0, 0 }, { 0, 1 } };	// 初期化マトリックス
	const CFontChar::SChar ZERO_CHAR;				// フォント文字初期値
	const UINT FORMAT_BITMAP = GGO_GRAY4_BITMAP;	// ビットマップのフォーマット
	const int MAX_GRAD = 16;						// 上のビットマップの最大階調
}

//************************************************************
//	親クラス [CFontChar] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CFontChar::CFontChar(const HFONT pFont) : m_pFont(pFont)
{
	// フォント文字連想配列をクリア
	m_mapChar.clear();
}

//============================================================
//	デストラクタ
//============================================================
CFontChar::~CFontChar()
{

}

//============================================================
//	フォント文字初期化処理
//============================================================
HRESULT CFontChar::Init(void)
{
	// フォント文字連想配列を初期化
	m_mapChar.clear();

	// 成功を返す
	return S_OK;
}

//============================================================
//	フォント文字終了処理
//============================================================
void CFontChar::Uninit(void)
{
	// フォント文字連想配列をクリア
	m_mapChar.clear();
}

//============================================================
//	フォント文字登録処理 (名前)
//============================================================
CFontChar::SChar CFontChar::Regist(const UINT uChar)
{
	// 既に生成済みかを検索
	auto itr = m_mapChar.find(uChar);	// 引数の文字を検索
	if (itr != m_mapChar.end())
	{ // 生成済みの場合

		// 読込済みのフォント文字情報を返す
		return itr->second;
	}

	// フォント文字情報を初期化
	SChar tempChar = ZERO_CHAR;

	// デバイスコンテキストを取得
	HDC pDC = GetDC(nullptr);
	if (pDC == nullptr)
	{ // 取得に失敗した場合

		// 初期値を返す
		assert(false);
		return ZERO_CHAR;
	}

	// 使用するフォントを選択し、過去のフォントを保存
	HFONT pOldFont = (HFONT)SelectObject(pDC, m_pFont);

	// ビットマップの生成・文字情報の保存
	BYTE *pBitMap = CreateBitMap(&tempChar, pDC, uChar);
	if (pBitMap == nullptr)
	{ // 生成に失敗した場合

		// 初期値を返す
		assert(false);
		return ZERO_CHAR;
	}

	// テクスチャの生成・設定
	if (FAILED(CreateTexture(&tempChar, pBitMap)))
	{ // 生成に失敗した場合

		// 初期値を返す
		assert(false);
		return ZERO_CHAR;
	}

	// 保存したフォントに戻す
	SelectObject(pDC, pOldFont);

	// ビットマップを破棄
	SAFE_DEL_ARRAY(pBitMap);

	if (pDC != nullptr)
	{
		// デバイスコンテキストを破棄
		ReleaseDC(nullptr, pDC);
	}

	// フォント文字情報を保存
	m_mapChar.insert(std::make_pair(uChar, tempChar));

	// 生成したフォント文字情報を返す
	return tempChar;
}

//============================================================
//	生成処理
//============================================================
CFontChar *CFontChar::Create(const HFONT pFont)
{
	// フォント文字の生成
	CFontChar *pFontChar = new CFontChar(pFont);
	if (pFontChar == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// フォント文字の初期化
		if (FAILED(pFontChar->Init()))
		{ // フォント文字初期化に失敗した場合

			// フォント文字の破棄
			SAFE_DELETE(pFontChar);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pFontChar;
	}
}

//============================================================
//	破棄処理
//============================================================
void CFontChar::Release(CFontChar *&prFontChar)
{
	// フォント文字の終了
	assert(prFontChar != nullptr);
	prFontChar->Uninit();

	// メモリ開放
	SAFE_DELETE(prFontChar);
}

//============================================================
//	ビットマップの生成・文字情報の保存
//============================================================
BYTE *CFontChar::CreateBitMap(SChar *pChar, HDC pDC, const UINT uChar)
{
	// ビットマップのサイズを取得
	DWORD dwGlyphSize = GetGlyphOutlineW(pDC, uChar, FORMAT_BITMAP, &pChar->glyph, 0, nullptr, &INIT_MATRIX);
	if (dwGlyphSize == GDI_ERROR)
	{ // 取得に失敗した場合

		// 失敗を返す
		assert(false);
		return nullptr;
	}

	// ビットマップのサイズ分メモリ確保
	BYTE *pBitMap = new BYTE[dwGlyphSize];
	if (pBitMap == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return nullptr;
	}

	// ビットマップ内の情報を取得
	dwGlyphSize = GetGlyphOutlineW(pDC, uChar, FORMAT_BITMAP, &pChar->glyph, dwGlyphSize, pBitMap, &INIT_MATRIX);
	if (dwGlyphSize == GDI_ERROR)
	{ // 取得に失敗した場合

		// 失敗を返す
		assert(false);
		return nullptr;
	}

	// フォントアウトラインの情報を取得
	DWORD dwOutLineSize = GetOutlineTextMetrics(pDC, sizeof(OUTLINETEXTMETRIC), &pChar->outline);
	if (dwOutLineSize == GDI_ERROR)
	{ // 取得に失敗した場合

		// 失敗を返す
		assert(false);
		return nullptr;
	}

	// フォントテキストの情報を取得
	if (!GetTextMetrics(pDC, &pChar->text))
	{ // 取得に失敗した場合

		// 失敗を返す
		assert(false);
		return nullptr;
	}

	// 確保したビットマップを返す
	return pBitMap;
}

//============================================================
//	テクスチャの生成・作成
//============================================================
HRESULT CFontChar::CreateTexture(SChar *pChar, BYTE *pBitMap)
{
	CTexture *pTexture = GET_MANAGER->GetTexture();	// テクスチャ情報
	LPDIRECT3DTEXTURE9 pTexChar;	// 文字テクスチャ情報
	D3DLOCKED_RECT lockRect;		// テクスチャピクセル情報

	// 空のテクスチャを生成・テクスチャインデックスを保存
	int nWidth = (pChar->glyph.gmBlackBoxX + 3) / 4 * 4;	// テクスチャ横幅
	int nHeight = pChar->glyph.gmBlackBoxY;					// テクスチャ縦幅
	pChar->nTexID = pTexture->Regist(CTexture::SInfo
	( // 引数
		nWidth,				// テクスチャ横幅
		nHeight,			// テクスチャ縦幅
		1,					// ミップマップレベル
		0,					// 性質・確保オプション
		D3DFMT_A8R8G8B8,	// ピクセルフォーマット
		D3DPOOL_MANAGED		// 格納メモリ
	));

	// 生成したテクスチャのポインタを取得
	pTexChar = pTexture->GetPtr(pChar->nTexID);

	// テクスチャをロックし、ピクセル情報を取得
	pTexChar->LockRect(0, &lockRect, nullptr, 0);

	// テクスチャにフォントの見た目を書き込み
	DWORD *pTexBuf = (DWORD*)lockRect.pBits;	// テクスチャメモリへのポインタ
	for (int nCntHeight = 0; nCntHeight < nHeight; nCntHeight++)
	{ // テクスチャ縦幅分繰り返す

		for (int nCntWidth = 0; nCntWidth < nWidth; nCntWidth++)
		{ // テクスチャ横幅分繰り返す

			// 現在のビットインデックスを計算
			int nBitID = nWidth * nCntHeight + nCntWidth;

			// ビットマップからアルファ値を計算
			DWORD dwAlpha = pBitMap[nBitID] * 255 / MAX_GRAD;

			// テクスチャの透明度を設定
			pTexBuf[nBitID] = (dwAlpha << 24) | 0x00ffffff;
		}
	}

	// テクスチャをアンロックする
	pTexChar->UnlockRect(0);

	// 成功を返す
	return S_OK;
}
