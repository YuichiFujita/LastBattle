//============================================================
//
//	ボスドラゴンヘッダー [enemyBossDragon.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _ENEMY_BOSS_DRAGON_H_
#define _ENEMY_BOSS_DRAGON_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "enemy.h"

//************************************************************
//	前方宣言
//************************************************************
class CEnemyAttack;	// 敵攻撃クラス

//************************************************************
//	クラス定義
//************************************************************
// ボスドラゴンクラス
class CEnemyBossDragon : public CEnemy
{
public:
	// 種類列挙
	enum EModel
	{
		MODEL_NORMAL = 0,	// 通常
		MODEL_MAX			// この列挙型の総数
	};

	// 通常状態列挙
	enum ENormalState
	{
		NSTATE_NONE = 0,	// 何もしない状態
		NSTATE_WAIT,		// 待機状態
		NSTATE_ATTACK,		// 攻撃状態
		NSTATE_MAX			// この列挙型の総数
	};

	// コンストラクタ
	explicit CEnemyBossDragon(const EType type);

	// デストラクタ
	~CEnemyBossDragon() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画
	void Hit(const int nDamage) override;	// ヒット
	void HitKnockBack(const int nDamage, const D3DXVECTOR3 &vecKnock) override;	// ノックバックヒット

private:
	// オーバーライド関数
	void UpdateNormal(void) override;	// 通常状態時の更新

	// メンバ変数
	CEnemyAttack *m_pAttack;	// 攻撃の情報
	ENormalState m_stateNormal;	// 通常状態時の状態
	int m_nCounterNormal;		// 通常状態管理カウンター
};

#endif	// _ENEMY_BOSS_DRAGON_H_
