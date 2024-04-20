//============================================================
//
//	文字列2Dヘッダー [string2D.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _STRING2D_H_
#define _STRING2D_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object.h"

//************************************************************
//	インクルードファイル
//************************************************************
class CFontChar;	// フォント文字クラス
class CChar2D;		// 文字2Dクラス

//************************************************************
//	クラス定義
//************************************************************
// 文字列2Dクラス
class CString2D : public CObject
{
public:
	// 原点列挙
	enum EOrigin
	{
		ORIGIN_LEFT = 0,	// 左原点
		ORIGIN_CENTER,		// 中心原点
		ORIGIN_RIGHT,		// 右原点
		ORIGIN_MAX,			// この列挙型の総数
	};

	// コンストラクタ
	CString2D();

	// デストラクタ
	~CString2D() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	void Draw(CShader *pShader = nullptr) override;			// 描画
	void SetVec3Position(const D3DXVECTOR3& rPos) override;	// 位置設定
	D3DXVECTOR3 GetVec3Position(void) const override;		// 位置取得
	void SetHeight(const float fHeight) override;			// 縦幅設定
	float GetHeight(void) const override;					// 縦幅取得

	// 静的メンバ関数
	static CString2D *Create	// 生成
	( // 引数
		CFontChar *pFontChar,		// フォント文字情報
		std::wstring wsStr,			// 指定文字列
		const D3DXVECTOR3 &rPos,	// 原点位置
		const float fHeight,		// 文字縦幅
		const EOrigin origin = ORIGIN_CENTER	// 原点
	);

	// メンバ関数
	HRESULT SetFontString	// フォント・文字列の設定
	( // 引数
		CFontChar *pFontChar,	// フォント文字情報
		std::wstring wsStr		// 指定文字列
	);
	float GetStrWidth(void);	// 文字列の横幅取得
	void SetOrigin(const EOrigin origin);	// 原点設定
	EOrigin GetOrigin(void) const;			// 原点取得

private:
	// オーバーライド関数
	void Release(void) override;	// 破棄

	// メンバ関数
	void SetPositionRelative(void);	// 相対位置設定

	// メンバ変数
	CChar2D **m_ppChar;		// 文字ポリゴンの情報
	D3DXVECTOR3 m_pos;		// 位置
	EOrigin m_origin;		// 原点
	std::wstring m_wsStr;	// 指定文字列
};

#endif	// _STRING2D_H_
