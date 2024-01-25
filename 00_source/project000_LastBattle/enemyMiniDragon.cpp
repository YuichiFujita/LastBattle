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

	// スポーン状態にする
	SetState(STATE_SPAWN);

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

//============================================================
//	モデルファイル取得処理
//============================================================
const char *CEnemyMiniDragon::GetModelFileName(const int nModel) const
{
	if (nModel > NONE_IDX && nModel < MODEL_MAX)
	{ // 使用できるインデックスの場合

		// 引数のインデックスのモデルを返す
		return MODEL_FILE[nModel];
	}
	else { assert(false); return NONE_STRING; }	// 範囲外
}

//============================================================
//	モーション・オブジェクトキャラクターの更新処理
//============================================================
void CEnemyMiniDragon::UpdateMotion(const int nMotion)
{
	if (nMotion <= NONE_IDX || nMotion >= MOTION_MAX) { assert(false); return; }	// モーションが未設定
	if (IsDeath()) { return; }	// 死亡している

	if (IsMotionLoop())
	{ // ループするモーションの場合

		if (GetMotionType() != nMotion)
		{ // 現在のモーションが再生中のモーションと一致しない場合

			// 現在のモーションの設定
			SetMotion(nMotion);
		}
	}
	else
	{ // ループしないモーションだった場合

#if 0
		switch (GetMotionType())
		{ // モーションごとの処理
		case MOTION_ATTACK:	// 攻撃モーション：ループOFF
			break;

		case MOTION_LAND:	// 着地モーション：ループOFF

			if (nMotion != MOTION_IDOL)
			{ // 待機モーションではない場合

				// 現在のモーションの設定
				SetMotion(nMotion);
			}

			break;

		default:	// 例外処理
			assert(false);
			break;
		}
#endif
	}

	// オブジェクトキャラクターの更新
	CObjectChara::Update();

#if 0
	switch (GetMotionType())
	{ // モーションごとの処理
	case MOTION_IDOL:	// 待機モーション：ループON
		break;

	case MOTION_DASH:	// ダッシュモーション：ループOFF
	case MOTION_LAND:	// 着地モーション：ループOFF

		if (IsMotionFinish())
		{ // モーションが終了していた場合

			// 現在のモーションの設定
			SetMotion(nMotion);
		}

		break;

	case MOTION_ATTACK:	// 攻撃モーション：ループOFF
		break;

	default:	// 例外処理
		assert(false);
		break;
	}
#endif
}
