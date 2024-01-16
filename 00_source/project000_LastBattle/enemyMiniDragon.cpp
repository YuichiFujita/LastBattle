//============================================================
//
//	ミニドラゴン処理 [enemyMiniDragon.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "enemyMiniDragon.h"
#include "manager.h"
#include "renderer.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char *MODEL_FILE[] =	// モデルファイル
	{
		"data\\MODEL\\ENEMY\\mini_dragon000.x",	// ミニドラゴン
	};
}

//************************************************************
//	スタティックアサート
//************************************************************
static_assert(NUM_ARRAY(MODEL_FILE) == CEnemyMiniDragon::MODEL_MAX, "ERROR : Model Count Mismatch");

//************************************************************
//	子クラス [CEnemyMiniDragon] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEnemyMiniDragon::CEnemyMiniDragon(const EType type) : CEnemy(type)
{

}

//============================================================
//	デストラクタ
//============================================================
CEnemyMiniDragon::~CEnemyMiniDragon()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CEnemyMiniDragon::Init(void)
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

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CEnemyMiniDragon::Uninit(void)
{
	// 敵の終了
	CEnemy::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CEnemyMiniDragon::Update(void)
{
	switch (GetState())
	{ // 状態ごとの処理
	case STATE_SPAWN:

		// スポーン動作の更新
		UpdateSpawn();

		break;

	case STATE_NORMAL:

		// 通常動作の更新
		UpdateNormal();

		break;

	default:	// 例外処理
		assert(false);
		break;
	}

	// 敵の更新
	CEnemy::Update();
}

//============================================================
//	描画処理
//============================================================
void CEnemyMiniDragon::Draw(void)
{
	// 敵の描画
	CEnemy::Draw();
}
