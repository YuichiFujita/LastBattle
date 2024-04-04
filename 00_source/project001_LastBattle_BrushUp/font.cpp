//============================================================
//
//	フォント処理 [font.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "font.h"
#include "manager.h"
#include "renderer.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char *LOAD_FOLDER = "data\\FONT";	// フォントフォルダ相対パス
	const CFont::SFont ZERO_FONT;	// フォント初期値
}

//************************************************************
//	親クラス [CFont] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CFont::CFont()
{
	// フォント連想配列をクリア
	m_mapFont.clear();
}

//============================================================
//	デストラクタ
//============================================================
CFont::~CFont()
{

}

//============================================================
//	フォント初期化処理
//============================================================
HRESULT CFont::Init(void)
{
	// フォント連想配列を初期化
	m_mapFont.clear();

	// 成功を返す
	return S_OK;
}

//============================================================
//	フォント終了処理
//============================================================
void CFont::Uninit(void)
{
	for (auto& rMap : m_mapFont)
	{ // フォントの要素数分繰り返す

		// フォント文字の破棄
		SAFE_REF_RELEASE(rMap.second.pFontChar);

		// フォントの破棄
		SAFE_DEL_OBJECT(rMap.second.pFont);
	}

	// フォント連想配列をクリア
	m_mapFont.clear();
}

//============================================================
//	フォント全読込処理
//============================================================
HRESULT CFont::LoadAll(void)
{
	// フォントの全読込
	if (FAILED(SearchFolderAll(LOAD_FOLDER)))
	{ // 読込に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	フォント登録処理 (名前)
//============================================================
CFont::SFont CFont::RegistName(std::string sFontName)
{
	// 既に生成済みかを検索
	auto itr = m_mapFont.find(sFontName);	// 引数のフォントを検索
	if (itr != m_mapFont.end())
	{ // 生成済みの場合

		// 読込済みのフォント情報を返す
		return itr->second;
	}

	// フォント情報を初期化
	SFont tempFont = ZERO_FONT;

#if 1
	// テストハンドルの生成
	int testHeight = 360;

	// フォント属性の設定
	LOGFONT lf;
	lf.lfHeight			= testHeight;					// フォントの縦幅
	lf.lfWidth			= 0;							// 平均文字幅
	lf.lfEscapement		= 0;							// 描画角度
	lf.lfOrientation	= 0;							// テキスト方向
	lf.lfWeight			= FW_NORMAL;					// フォントの太さ
	lf.lfItalic			= FALSE;						// イタリックス
	lf.lfUnderline		= FALSE;						// アンダーライン
	lf.lfStrikeOut		= FALSE;						// 打ち消し線
	lf.lfCharSet		= SHIFTJIS_CHARSET;				// 文字セット
	lf.lfOutPrecision	= OUT_TT_ONLY_PRECIS;			// 出力精度
	lf.lfClipPrecision	= CLIP_DEFAULT_PRECIS;			// クリッピング精度
	lf.lfQuality		= PROOF_QUALITY;				// 出力品質
	lf.lfPitchAndFamily	= (DEFAULT_PITCH | FF_MODERN);	// ピッチとファミリー
	strcpy(&lf.lfFaceName[0], sFontName.c_str());		// フォント名

	// フォントの生成
	tempFont.pFont = CreateFontIndirectA(&lf);
	if (tempFont.pFont == nullptr)
	{ // 生成に失敗した場合

		// 初期値を返す
		assert(false);
		return ZERO_FONT;
	}
#endif

	// フォント文字の生成
	tempFont.pFontChar = CFontChar::Create(tempFont.pFont);
	if (tempFont.pFontChar == nullptr)
	{ // 生成に失敗した場合

		// 初期値を返す
		assert(false);
		return ZERO_FONT;
	}

	// フォント情報を保存
	m_mapFont.insert(std::make_pair(sFontName, tempFont));

	// 生成したフォント情報を返す
	return tempFont;
}

//============================================================
//	フォント文字登録 (名前)
//============================================================
CFontChar::SChar CFont::RegistChar(std::string sFontName, const UINT uChar)
{
	// 生成したフォントの文字テクスチャインデックスを返す
	return RegistName(sFontName).pFontChar->Regist(uChar);
}

//============================================================
//	生成処理
//============================================================
CFont *CFont::Create(void)
{
	// フォントの生成
	CFont *pFont = new CFont;
	if (pFont == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// フォントの初期化
		if (FAILED(pFont->Init()))
		{ // フォント初期化に失敗した場合

			// フォントの破棄
			SAFE_DELETE(pFont);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pFont;
	}
}

//============================================================
//	破棄処理
//============================================================
void CFont::Release(CFont *&prFont)
{
	// フォントの終了
	assert(prFont != nullptr);
	prFont->Uninit();

	// メモリ開放
	SAFE_DELETE(prFont);
}

//============================================================
//	フォルダ全検索処理
//============================================================
HRESULT CFont::SearchFolderAll(std::string sFolderPath)
{
#if 0
	// 変数を宣言
	HANDLE hFile;	// 検索ハンドル
	WIN32_FIND_DATA findFileData;	// ファイル情報

	// 引数パスのディレクトリを取得
	std::string sAllLoadPath = sFolderPath + "\\*.*";	// 全読込パス
	hFile = FindFirstFile(sAllLoadPath.c_str(), &findFileData);
	if (INVALID_HANDLE_VALUE == hFile)
	{ // ハンドルが無効の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	do
	{ // ファイル内の情報全てを読み込む

		// 現在のディレクトリ、親ディレクトリの場合次のループに移行
		if (strcmp(findFileData.cFileName, ".") == 0)	{ continue; }
		if (strcmp(findFileData.cFileName, "..") == 0)	{ continue; }

		// ファイル名を相対パスに変換
		std::string sFullPath = sFolderPath;	// 現在の相対パスを設定
		sFullPath += "\\";						// パス区切り文字を追加
		sFullPath += findFileData.cFileName;	// ファイル名を追加

		if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{ // ディレクトリだった場合

			// 新たなディレクトリを全検索
			SearchFolderAll(sFullPath);
		}
		else
		{ // ファイルだった場合

			// フォントを登録
			Regist(sFullPath.c_str());
		}

	} while (FindNextFile(hFile, &findFileData));	// 次のファイルを検索

	// 検索ハンドルを閉じる
	FindClose(hFile);
#endif

	// 成功を返す
	return S_OK;
}
