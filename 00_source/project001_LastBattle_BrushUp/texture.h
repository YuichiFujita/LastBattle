//============================================================
//
//	テクスチャヘッダー [texture.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include <map>
#include <string>

//************************************************************
//	クラス定義
//************************************************************
// テクスチャクラス
class CTexture
{
public:
	// コンストラクタ
	CTexture();

	// デストラクタ
	~CTexture();

	// テクスチャ生成情報構造体
	struct SInfo
	{
	public:
		// コンストラクタ
		SInfo() {}
		SInfo(const UINT nWidth, const UINT nHeight, const UINT nMip, const DWORD dwUsage, const D3DFORMAT format, const D3DPOOL pool)
			: Width(nWidth), Height(nHeight), MipLevels(nMip), Usage(dwUsage), Format(format), Pool(pool) {}

		// デストラクタ
		~SInfo() {}

		// メンバ変数
		UINT		Width;		// テクスチャ横幅
		UINT		Height;		// テクスチャ縦幅
		UINT		MipLevels;	// ミップマップレベル
		DWORD		Usage;		// 性質・確保オプション
		D3DFORMAT	Format;		// ピクセルフォーマット
		D3DPOOL		Pool;		// 格納メモリ
	};

	// テクスチャ構造体
	struct STexture
	{
		LPDIRECT3DTEXTURE9 pTexture;	// テクスチャへのポインタ
	};

	// マップ情報構造体
	struct SMapInfo
	{
		STexture textureData;		// テクスチャ情報
		std::string sFilePassName;	// ファイルパス名
	};

	// メンバ関数
	int Regist(const SInfo info);				// テクスチャ登録 (生成)
	int Regist(const char *pFileName);			// テクスチャ登録 (パス)
	STexture GetInfo(const int nID);			// テクスチャ情報取得
	LPDIRECT3DTEXTURE9 GetPtr(const int nID);	// テクスチャポインタ取得

	// 静的メンバ関数
	static CTexture *Create(void);				// 生成
	static void Release(CTexture *&pTexture);	// 破棄

private:
	// メンバ関数
	HRESULT Load(void);	// テクスチャ生成
	void Unload(void);	// テクスチャ破棄

	// メンバ変数
	std::map<int, SMapInfo> m_mapTexture;	// テクスチャ連想配列

	// 静的メンバ変数
	static int m_nNumAll;	// テクスチャの総数
};

#endif	// _TEXTURE_H_
