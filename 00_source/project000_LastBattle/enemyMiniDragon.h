//============================================================
//
//	ミニドラゴンヘッダー [enemyMiniDragon.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _ENEMY_MINI_DRAGON_H_
#define _ENEMY_MINI_DRAGON_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "enemy.h"

//************************************************************
//	クラス定義
//************************************************************
// ミニドラゴンクラス
class CEnemyMiniDragon : public CEnemy
{
public:
	// モデル列挙
	enum EModel
	{
		MODEL_NORMAL = 0,	// 通常
		MODEL_MAX			// この列挙型の総数
	};

	// コンストラクタ
	explicit CEnemyMiniDragon(const EType type);

	// デストラクタ
	~CEnemyMiniDragon() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画

private:
	// オーバーライド関数
	const char *GetModelFileName(const int nModel) const;	// モデルファイル取得
};

#endif	// _ENEMY_MINI_DRAGON_H_
