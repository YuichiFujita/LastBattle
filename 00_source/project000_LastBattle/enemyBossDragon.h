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

	// コンストラクタ
	explicit CEnemyBossDragon(const EType type);

	// デストラクタ
	~CEnemyBossDragon() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画
};

#endif	// _ENEMY_BOSS_DRAGON_H_
