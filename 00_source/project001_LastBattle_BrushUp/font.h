//============================================================
//
//	フォントヘッダー [font.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _FONT_H_
#define _FONT_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "fontChar.h"

//************************************************************
//	クラス定義
//************************************************************
// フォントクラス
class CFont
{
public:
	// コンストラクタ
	CFont();

	// デストラクタ
	~CFont();

	// キー構造体
	struct SKey
	{
		// コンストラクタ
		SKey(std::string FontName, bool Italic) :
		sFontName(FontName), bItalic(Italic) {}

		// 比較演算子オーバーロード
		bool operator<(const SKey& key) const
		{
			// 情報をまとめて比較
			return std::tie(sFontName, bItalic) < std::tie(key.sFontName, key.bItalic);
		}

		std::string sFontName;	// フォント名
		bool bItalic;			// イタリック
	};

	// フォント構造体
	struct SFont
	{
		// コンストラクタ
		SFont() { memset(this, 0, sizeof(*this)); }

		CFontChar *pFontChar;	// フォント文字インスタンス
		HFONT pFont;			// フォントへのポインタ
	};

	// メンバ関数
	HRESULT Init(void);		// フォント初期化
	void Uninit(void);		// フォント終了
	HRESULT LoadAll(void);	// フォント全読込
	SFont RegistName		// フォント登録 (名前)
	( // 引数
		std::string sFontName,	// フォント名
		bool bItalic = false	// イタリック
	);
	CFontChar::SChar RegistChar	// フォント文字登録 (名前)
	( // 引数
		const UINT uChar,		// 指定文字
		std::string sFontName,	// フォント名
		bool bItalic = false	// イタリック
	);

	// 静的メンバ関数
	static CFont *Create(void);			// 生成
	static void Release(CFont *&pFont);	// 破棄

private:
	// メンバ関数
	HRESULT SearchFolderAll(std::string sFolderPath);	// フォルダ全検索

	// メンバ変数
	std::map<SKey, SFont> m_mapFont;	// フォント連想配列
};

#endif	// _FONT_H_
