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
		MODEL_WAIST = 0,	// 腰
		MODEL_BODY,			// 体
		MODEL_JAW,			// 顎
		MODEL_HEAD,			// 頭
		MODEL_ARM_UL,		// 左上腕
		MODEL_ARM_UR,		// 右上腕
		MODEL_ARM_DL,		// 左下腕
		MODEL_ARM_DR,		// 右下腕
		MODEL_HAND_L,		// 左手
		MODEL_HAND_R,		// 右手
		MODEL_LEG_UL,		// 左太もも
		MODEL_LEG_UR,		// 右太もも
		MODEL_LEG_DL,		// 左脛
		MODEL_LEG_DR,		// 右脛
		MODEL_FOOT_L,		// 左足
		MODEL_FOOT_R,		// 右足
		MODEL_WING_L_00,	// 左翼00
		MODEL_WING_R_00,	// 右翼00
		MODEL_WING_L_01,	// 左翼01
		MODEL_WING_R_01,	// 右翼01
		MODEL_WING_L_02,	// 左翼02
		MODEL_WING_R_02,	// 右翼02
		MODEL_WING_L_03,	// 左翼03
		MODEL_WING_R_03,	// 右翼03
		MODEL_TAIL_00,		// 尻尾00
		MODEL_TAIL_01,		// 尻尾01
		MODEL_TAIL_02,		// 尻尾02
		MODEL_TAIL_03,		// 尻尾03
		MODEL_TAIL_04,		// 尻尾04
		MODEL_TAIL_05,		// 尻尾05
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
	const char *GetModelFileName(const int nModel) const;	// モデルファイル取得
	void UpdateNormal(void) override;	// 通常状態時の更新

	// メンバ変数
	CEnemyAttack *m_pAttack;	// 攻撃の情報
	ENormalState m_stateNormal;	// 通常状態時の状態
	int m_nCounterNormal;		// 通常状態管理カウンター
};

#endif	// _ENEMY_BOSS_DRAGON_H_
