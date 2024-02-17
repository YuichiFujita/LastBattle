//============================================================
//
//	敵攻撃04ヘッダー [enemyAttack04.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _ENEMY_ATTACK04_H_
#define _ENEMY_ATTACK04_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "enemyAttack.h"

//************************************************************
//	クラス定義
//************************************************************
// 敵攻撃04クラス
/*
	@brief ボスの攻撃管理クラス 炎を外周から吐き出す
*/
class CEnemyAttack04 : public CEnemyAttack
{
public:
	// 状態列挙
	enum EState
	{
		STATE_INIT_TELEPORT = 0,	// テレポートの初期化
		STATE_TELEPORT,				// テレポート
		STATE_INIT_WAIT,			// 攻撃待機の初期化
		STATE_WAIT,					// 攻撃待機
		STATE_FIRE,					// 炎発射
		STATE_END,					// 終了
		STATE_MAX					// この列挙型の総数
	};

	// コンストラクタ
	CEnemyAttack04();

	// デストラクタ
	~CEnemyAttack04() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	bool Update(void) override;		// 更新

private:
	// メンバ関数
	void InitTeleport(void);	// テレポートの初期化
	void UpdateFire(void);		// 炎発射の更新
	void CreateFire(void);		// 炎の生成

	// メンバ変数
	EState m_state;			// 状態
	int m_nCounterWait;		// 余韻管理カウンター
	int m_nCounterNumAtk;	// 攻撃回数カウンター
};

#endif	// _ENEMY_ATTACK04_H_
