//============================================================
//
//	敵攻撃ヘッダー [enemyAttack.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _ENEMY_ATTACK_H_
#define _ENEMY_ATTACK_H_

//************************************************************
//	前方宣言
//************************************************************
class CEnemyBossDragon;	// ボスドラゴンクラス
class CWave;			// 波動クラス

//************************************************************
//	クラス定義
//************************************************************
// 敵攻撃クラス
class CEnemyAttack
{
public:
	// 攻撃列挙
	enum EAttack
	{
		ATTACK_00 = 0,	// 攻撃00
		//ATTACK_01,		// 攻撃01
		ATTACK_MAX		// この列挙型の総数
	};

	// コンストラクタ
	CEnemyAttack();

	// デストラクタ
	virtual ~CEnemyAttack();

	// 純粋仮想関数
	virtual bool Update(void) = 0;	// 更新

	// 仮想関数
	virtual HRESULT Init(void);	// 初期化
	virtual void Uninit(void);	// 終了

	// 静的メンバ関数
	static CEnemyAttack *Create	// 生成
	( // 引数
		const EAttack attackID,	// 攻撃インデックス
		CEnemyBossDragon *pBoss	// ボスへのポインタ
	);
	static void Release(CEnemyAttack *&prEnemyAttack);	// 破棄

	// メンバ関数
	CEnemyBossDragon *GetBossDragon(void);	// ボス取得

private:
	// メンバ変数
	CEnemyBossDragon *m_pBoss;	// ボスへのポインタ
};

// 敵攻撃00クラス
class CEnemyAttack00 : public CEnemyAttack
{
public:
	// 状態列挙
	enum EState
	{
		STATE_SET = 0,	// 初期設定
		STATE_WAVE,		// 波動発射
		STATE_WAIT,		// 待機
		STATE_END,		// 終了
		STATE_MAX		// この列挙型の総数
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
	CWave *m_pWave;			// 波動の情報
	EState m_state;			// 状態
	int m_nCounterState;	// 状態管理カウンター
	int m_nCreateWave;		// 波動の生成数
};

#endif	// _ENEMY_ATTACK_H_
