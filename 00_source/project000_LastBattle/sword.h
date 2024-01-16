//============================================================
//
//	剣ヘッダー [sword.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _SWORD_H_
#define _SWORD_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "multiModel.h"

//************************************************************
//	前方宣言
//************************************************************
class COrbit;	// 軌跡クラス

//************************************************************
//	クラス定義
//************************************************************
// 剣クラス
class CSword : public CMultiModel
{
public:
	// モデル列挙
	enum EModel
	{
		MODEL_SWORD = 0,	// 剣
		MODEL_MAX			// この列挙型の総数
	};

	// 状態列挙
	enum EState
	{
		STATE_NONE = 0,	// 何もしない状態
		STATE_NORMAL,	// 通常状態
		STATE_VANISH,	// 消失状態
		STATE_MAX		// この列挙型の総数
	};

	// コンストラクタ
	CSword();

	// デストラクタ
	~CSword() override;

	// オーバーライド関数
	HRESULT Init(void) override;		// 初期化
	void Uninit(void) override;			// 終了
	void Update(void) override;			// 更新
	void Draw(void) override;			// 描画
	int GetState(void) const override;	// 状態取得

	// 静的メンバ関数
	static CSword *Create(CObject *pObject);	// 生成

	// メンバ関数
	void SetState(const EState state);	// 状態設定

private:
	// メンバ変数
	COrbit	*m_pOrbit;			// 軌跡の情報
	EState	m_state;			// 状態
	int		m_nCounterState;	// 状態管理カウンター
	bool	m_bDisp;			// 表示状況
};

#endif	// _SWORD_H_
