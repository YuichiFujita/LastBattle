//============================================================
//
//	敵攻撃00ヘッダー [enemyAttack00.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _ENEMY_ATTACK00_H_
#define _ENEMY_ATTACK00_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "enemyAttack.h"

//************************************************************
//	クラス定義
//************************************************************
// 敵攻撃00クラス
/*
	@brief ボスの攻撃管理クラス 地面を殴って波動を出す
*/
class CEnemyAttack00 : public CEnemyAttack
{
public:
	// 状態列挙
	enum EState
	{
		STATE_INIT_TELEPORT = 0,	// テレポートの初期化
		STATE_TELEPORT,				// テレポート
		STATE_INIT_WAVE,			// 波動発射の初期化
		STATE_WAVE,					// 波動発射
		STATE_WAIT,					// 待機
		STATE_END,					// 終了
		STATE_MAX					// この列挙型の総数
	};

	// コンストラクタ
	CEnemyAttack00();

	// デストラクタ
	~CEnemyAttack00() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	bool Update(void) override;		// 更新

private:
	// メンバ変数
	EState m_state;			// 状態
	int m_nCounterState;	// 状態管理カウンター
	int m_nCounterNumAtk;	// 攻撃回数カウンター
};

#endif	// _ENEMY_ATTACK00_H_
