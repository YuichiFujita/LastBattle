//============================================================
//
//	テクスチャ処理 [texture.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "texture.h"
#include "manager.h"
#include "renderer.h"

//************************************************************
//	静的メンバ変数宣言
//************************************************************
int CTexture::m_nNumAll = 0;	// テクスチャの総数

//************************************************************
//	親クラス [CTexture] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CTexture::CTexture()
{
	// テクスチャ連想配列をクリア
	m_mapTexture.clear();
}

//============================================================
//	デストラクタ
//============================================================
CTexture::~CTexture()
{

}

//============================================================
//	テクスチャ生成処理
//============================================================
HRESULT CTexture::Load(void)
{
	// テクスチャ連想配列を初期化
	m_mapTexture.clear();

	// 成功を返す
	return S_OK;
}

//============================================================
//	テクスチャ破棄処理
//============================================================
void CTexture::Unload(void)
{
	for (auto& rMap : m_mapTexture)
	{ // テクスチャの要素数分繰り返す

		// テクスチャの破棄
		SAFE_RELEASE(rMap.second.textureData.pTexture);
	}

	// テクスチャ連想配列をクリア
	m_mapTexture.clear();
}

//============================================================
//	テクスチャ登録処理 (生成)
//============================================================
int CTexture::Regist(const SInfo info)
{
	// 変数を宣言
	HRESULT  hr;			// 異常終了の確認用
	SMapInfo tempMapInfo;	// マップ情報
	int nID = m_nNumAll;	// テクスチャ読込番号

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// デバイスのポインタ

	// 空のテクスチャを生成
	hr = D3DXCreateTexture
	( // 引数
		pDevice,		// Direct3Dデバイス
		info.Width,		// テクスチャ横幅
		info.Height,	// テクスチャ縦幅
		info.MipLevels,	// ミップマップレベル
		info.Usage,		// 性質・確保オプション
		info.Format,	// ピクセルフォーマット
		info.Pool,		// 格納メモリ
		&tempMapInfo.textureData.pTexture	// テクスチャへのポインタ
	);
	if (FAILED(hr))
	{ // テクスチャの生成に失敗した場合

		// 失敗を返す
		assert(false);
		return NONE_IDX;
	}

	// ファイルパス名を保存
	tempMapInfo.sFilePassName = NONE_STRING;	// 読込ではないのでパス無し

	// テクスチャ情報を保存
	m_mapTexture.insert(std::make_pair(m_nNumAll, tempMapInfo));

	// テクスチャの総数を加算
	m_nNumAll++;

	// 読み込んだテクスチャの配列番号を返す
	return nID;
}

//============================================================
//	テクスチャ登録処理 (パス)
//============================================================
int CTexture::Regist(const char *pFileName)
{
	// 変数を宣言
	SMapInfo tempMapInfo;	// マップ情報
	int nID = m_nNumAll;	// テクスチャ読込番号

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// デバイスのポインタ

	if (pFileName != nullptr)
	{ // ポインタが使用されている場合

		int nCntTexture = 0;	// テクスチャ番号
		for (const auto& rMap : m_mapTexture)
		{ // テクスチャの要素数分繰り返す

			if (rMap.second.sFilePassName.compare(pFileName) == 0)
			{ // 文字列が一致した場合

				// すでに読み込んでいるテクスチャの配列番号を返す
				return nCntTexture;
			}

			// 次のテクスチャ番号にする
			nCntTexture++;
		}

		// テクスチャの読み込み
		if (FAILED(D3DXCreateTextureFromFile(pDevice, pFileName, &tempMapInfo.textureData.pTexture)))
		{ // テクスチャの読み込みに失敗した場合

			// 失敗を返す
			assert(false);
			return NONE_IDX;
		}

		// ファイルパス名を保存
		tempMapInfo.sFilePassName = pFileName;

		// テクスチャ情報を保存
		m_mapTexture.insert(std::make_pair(m_nNumAll, tempMapInfo));

		// テクスチャの総数を加算
		m_nNumAll++;

		// 読み込んだテクスチャの配列番号を返す
		return nID;
	}
	else
	{ // ポインタが使用されていない場合

		// テクスチャ非使用を返す
		return NONE_IDX;
	}
}

//============================================================
//	テクスチャ情報の取得処理
//============================================================
CTexture::STexture CTexture::GetInfo(const int nID)
{
	int nArray = (int)m_mapTexture.size();	// 配列要素数
	if (nID > NONE_IDX && nID < nArray)
	{ // テクスチャがある場合

		// 引数のテクスチャ情報を返す
		return m_mapTexture.find(nID)->second.textureData;
	}
	else
	{ // テクスチャがない場合

		// インデックスエラー
		assert(false);

		if (nArray > 0)
		{ // テクスチャ生成がされている場合

			// 先頭テクスチャを返す
			return m_mapTexture.find(0)->second.textureData;
		}
		else
		{ // テクスチャが一つもない場合

			// 空のテクスチャ情報を返す
			STexture tempTexture;
			memset(&tempTexture, 0, sizeof(tempTexture));
			return tempTexture;
		}
	}
}

//============================================================
//	テクスチャポインタの取得処理
//============================================================
LPDIRECT3DTEXTURE9 CTexture::GetPtr(const int nID)
{
	if (nID >= 0 && nID < m_nNumAll)
	{ // 引数のインデックスが範囲内の場合

		if (nID > NONE_IDX && nID < (int)m_mapTexture.size())
		{ // テクスチャがある場合

			// 引数のテクスチャポインタを返す
			return m_mapTexture.find(nID)->second.textureData.pTexture;
		}
		else
		{ // テクスチャがない場合

			// nullptrを返す
			assert(false);
			return nullptr;
		}
	}
	else if (nID == NONE_IDX)
	{ // 引数のインデックスが -1の場合

		// nullptrを返す
		return nullptr;
	}
	else
	{ // 引数のインデックスが使用不可の場合

		// nullptrを返す
		assert(false);
		return nullptr;
	}
}

//============================================================
//	生成処理
//============================================================
CTexture *CTexture::Create(void)
{
	// テクスチャの生成
	CTexture *pTexture = new CTexture;
	if (pTexture == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// テクスチャの読込
		if (FAILED(pTexture->Load()))
		{ // テクスチャ読込に失敗した場合

			// テクスチャの破棄
			SAFE_DELETE(pTexture);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pTexture;
	}
}

//============================================================
//	破棄処理
//============================================================
void CTexture::Release(CTexture *&prTexture)
{
	// テクスチャの破棄
	assert(prTexture != nullptr);
	prTexture->Unload();

	// メモリ開放
	SAFE_DELETE(prTexture);
}
