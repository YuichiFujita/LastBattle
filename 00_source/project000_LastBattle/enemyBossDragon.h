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
class CMagicCircle;	// 魔法陣クラス

//************************************************************
//	クラス定義
//************************************************************
// ボスドラゴンクラス
class CEnemyBossDragon : public CEnemy
{
public:
	// モデル列挙
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

	// モーション列挙
	enum EMotion
	{
		MOTION_IDOL = 0,		// 待機モーション
		MOTION_PUNCH_GROUND,	// 地面殴りモーション
		MOTION_MAX				// この列挙型の総数
	};

	// 行動列挙
	enum EAction
	{
		ACT_NONE = 0,		// 何もしない
		ACT_MAGIC_FADEIN,	// 魔法陣フェードイン
		ACT_MAGIC_FADEOUT,	// 魔法陣フェードアウト
		ACT_PUNCH_GROUND,	// 地面殴り
		ACT_MAX				// この列挙型の総数
	};

	// テレポート列挙
	enum ETeleport
	{
		TELEPORT_SET_APPER = 0,	// 魔法陣の出現設定
		TELEPORT_APPEAR,		// 魔法陣の出現
		TELEPORT_MOVE,			// 魔法陣の移動
		TELEPORT_DISAPPEAR,		// 魔法陣の消失
		TELEPORT_MAX			// この列挙型の総数
	};

	// コンストラクタ
	explicit CEnemyBossDragon(const EType type);

	// デストラクタ
	~CEnemyBossDragon() override;

	// テレポート構造体
	struct STeleport
	{
		D3DXVECTOR3 pos;	// テレポート目標位置
		D3DXVECTOR3 rot;	// テレポート目標向き
		ETeleport state;	// テレポート状態
	};

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画
	void Hit(const int nDamage) override;	// ヒット
	void HitKnockBack(const int nDamage, const D3DXVECTOR3 &vecKnock) override;	// ノックバックヒット

	// メンバ関数
	void SetTeleport(const D3DXVECTOR3& rPos, const D3DXVECTOR3 &rRot);	// テレポート設定
	void SetActPunchGround(void);	// 地面殴り行動設定
	EAction GetAction(void);		// 行動取得

private:
	// オーバーライド関数
	const char *GetModelFileName(const int nModel) const override;	// モデルファイル取得
	void UpdateMotion(void) override;	// モーション・オブジェクトキャラクターの更新
	void UpdateNormal(void) override;	// 通常状態時の更新

	// メンバ関数
	void UpdateAttack(void);	// 攻撃更新
	void UpdateAction(void);	// 行動更新

	void UpdateMagicFadeIn(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pRot);	// 魔法陣フェードイン行動時の更新
	void UpdateMagicFadeOut(const D3DXVECTOR3& rPos);	// 魔法陣フェードアウト行動時の更新
	void UpdatePunchGround(void);	// 地面殴り行動時の更新

	// メンバ変数
	CEnemyAttack *m_pAttack;		// 攻撃の情報
	CMagicCircle *m_pMagicCircle;	// 魔法陣の情報
	STeleport m_teleport;	// テレポートの情報
	EAction m_action;		// 行動
	int m_nCounterAttack;	// 攻撃管理カウンター
};

#endif	// _ENEMY_BOSS_DRAGON_H_
