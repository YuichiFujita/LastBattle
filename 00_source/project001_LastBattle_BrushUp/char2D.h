//============================================================
//
//	文字2Dヘッダー [char2D.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _CHAR2D_H_
#define _CHAR2D_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object2D.h"
#include "fontChar.h"

//************************************************************
//	クラス定義
//************************************************************
// 文字2Dクラス
class CChar2D : public CObject2D
{
public:
	// コンストラクタ
	CChar2D();

	// デストラクタ
	~CChar2D() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	void Draw(CShader *pShader = nullptr) override;			// 描画
	void SetVec3Sizing(const D3DXVECTOR3& rSize) override;	// 大きさ設定
	void SetHeight(const float fHeight) override;			// 縦幅設定
	float GetHeight(void) const override;					// 縦幅取得

	// 静的メンバ関数
	static CChar2D *Create	// 生成
	( // 引数
		CFontChar *pFontChar,	// フォント文字情報
		const wchar_t wcChar,	// 指定文字
		const D3DXVECTOR3& rPos = VEC3_ZERO,	// 位置
		const float fHeight = 100.0f,			// 縦幅
		const D3DXVECTOR3& rRot = VEC3_ZERO,	// 向き
		const D3DXCOLOR& rCol = XCOL_WHITE		// 色
	);

	// メンバ関数
	void SetFontChar	// フォント・文字の設定
	( // 引数
		CFontChar *pFontChar,	// フォント文字情報
		const wchar_t wcChar	// 指定文字
	);
	float GetOffset(void);	// 原点のオフセット取得
	float GetNext(void);	// 次の文字までの距離取得

private:
	// メンバ変数
	CFontChar::SChar m_infoChar;	// 文字情報
	wchar_t m_wcChar;		// 指定文字
	float m_fSizeRate;		// 縦幅の割合
	float m_fAbsOriginX;	// X原点オフセットの絶対値
};

#endif	// _CHAR2D_H_
