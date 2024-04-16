//============================================================
//
//	フォント2Dヘッダー [font2D.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _FONT2D_H_
#define _FONT2D_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object2D.h"
#include "fontChar.h"

//************************************************************
//	クラス定義
//************************************************************
// フォント2Dクラス
class CFont2D : public CObject2D
{
public:
	// コンストラクタ
	explicit CFont2D(const CObject::ELabel label = LABEL_NONE, const EDim dimension = DIM_2D, const int nPriority = object::DEFAULT_PRIO);

	// デストラクタ
	~CFont2D() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	void Draw(CShader *pShader = nullptr) override;			// 描画
	void SetVec3Position(const D3DXVECTOR3& rPos) override;	// 位置設定
	void SetVec3Rotation(const D3DXVECTOR3& rRot) override;	// 向き設定
	void SetVec3Sizing(const D3DXVECTOR3& rSize) override;	// 大きさ設定
	void SetColor(const D3DXCOLOR& rCol) override;			// 色設定
	void SetScale(const float fScale) override;				// 拡大率設定

	// 静的メンバ関数
	static CFont2D *Create	// 生成
	( // 引数
		CFontChar *pFontChar,		// フォント文字情報
		const UINT uChar,			// 指定文字
		const D3DXVECTOR3& rPos,	// 位置
		const float fScale = 1.0f,				// 拡大率
		const D3DXVECTOR3& rRot = VEC3_ZERO,	// 向き
		const D3DXCOLOR& rCol = XCOL_WHITE		// 色
	);

	// メンバ関数
	void SetFontChar	// フォント・文字の設定
	( // 引数
		CFontChar *pFontChar,	// フォント文字情報
		const UINT uChar		// 指定文字
	);

	// 原点分のずらす距離取得
	float GetOffset(void)
	{
		float fOrigin = (float)m_infoChar.glyph.gmptGlyphOrigin.x;
		if (fOrigin < 0.0f) { fOrigin *= -1.0f; }
		float fBlack = (float)m_infoChar.glyph.gmBlackBoxX;
		return (fOrigin + fBlack * 0.5f);
	}

	// 次の文字までの距離取得
	float GetNext(void)
	{
		float fNext = (float)m_infoChar.glyph.gmCellIncX;
		return fNext;
	}

private:
	// メンバ変数
	CFontChar::SChar m_infoChar;	// 文字情報
	float m_fScale;	// 拡大率
};

#endif	// _FONT2D_H_
