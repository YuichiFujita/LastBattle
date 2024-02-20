//============================================================
//
//	敵攻撃05ヘッダー [enemyAttack05.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _ENEMY_ATTACK05_H_
#define _ENEMY_ATTACK05_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "enemyAttack.h"

//************************************************************
//	クラス定義
//************************************************************
// 敵攻撃05クラス
/*
	@brief ボスの攻撃管理クラス 爪でひっかく攻撃
*/
class CEnemyAttack05 : public CEnemyAttack
{
public:
	// 状態列挙
	enum EState
	{
		STATE_INIT_TELEPORT = 0,	// テレポートの初期化
		STATE_TELEPORT,				// テレポート
		STATE_INIT_ATTACK,			// 攻撃の初期化
		STATE_ATTACK,				// 攻撃
		STATE_END,					// 終了
		STATE_MAX					// この列挙型の総数
	};

	// コンストラクタ
	explicit CEnemyAttack05(CEnemyBossDragon *pBoss);

	// デストラクタ
	~CEnemyAttack05() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	bool Update(void) override;		// 更新

private:
	// メンバ関数
	void SetEnableTeleport(void);	// テレポートするかの設定
	void InitTeleport(void);		// テレポートの初期化
	void UpdateAttack(void);		// 攻撃の更新

	// メンバ変数
	EState m_state;	// 状態
};

#endif	// _ENEMY_ATTACK05_H_
