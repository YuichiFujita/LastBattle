//============================================================
//
//	ボスドラゴン処理 [enemyBossDragon.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "enemyBossDragon.h"
#include "manager.h"
#include "renderer.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char *MODEL_FILE[] =	// モデルファイル
	{
		"data\\MODEL\\ENEMY\\boss_dragon000.x",	// ボスドラゴン
	};
}

//************************************************************
//	スタティックアサート
//************************************************************
static_assert(NUM_ARRAY(MODEL_FILE) == CEnemyBossDragon::MODEL_MAX, "ERROR : Model Count Mismatch");

//************************************************************
//	子クラス [CEnemyBossDragon] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEnemyBossDragon::CEnemyBossDragon(const EType type) : CEnemy(type)
{

}

//============================================================
//	デストラクタ
//============================================================
CEnemyBossDragon::~CEnemyBossDragon()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CEnemyBossDragon::Init(void)
{
	// 敵の初期化
	if (FAILED(CEnemy::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// モデルの登録・割当
	BindModel(MODEL_FILE[MODEL_NORMAL]);

	// スポーン状態にする
	SetState(STATE_SPAWN);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CEnemyBossDragon::Uninit(void)
{
	// 敵の終了
	CEnemy::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CEnemyBossDragon::Update(void)
{
	// 敵の更新
	CEnemy::Update();
}

//============================================================
//	描画処理
//============================================================
void CEnemyBossDragon::Draw(void)
{
	// 敵の描画
	CEnemy::Draw();
}
