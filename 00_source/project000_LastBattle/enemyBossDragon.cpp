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
#include "enemyAttack.h"
#include "manager.h"
#include "renderer.h"
#include "sceneGame.h"
#include "gameManager.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char *MODEL_FILE[] =	// モデルファイル
	{
		"data\\MODEL\\ENEMY\\boss_dragon000.x",	// ボスドラゴン
	};

	const int ATK_WAIT_FRAME = 60;	// 攻撃の余韻フレーム
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
CEnemyBossDragon::CEnemyBossDragon(const EType type) : CEnemy(type),
	m_pAttack			(nullptr),		// 攻撃の情報
	m_stateNormal		(NSTATE_NONE),	// 通常状態時の状態
	m_nCounterNormal	(0)				// 通常状態管理カウンター
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
	// メンバ変数を初期化
	m_pAttack			= nullptr;		// 攻撃の情報
	m_stateNormal		= NSTATE_WAIT;	// 通常状態時の状態
	m_nCounterNormal	= 0;			// 通常状態管理カウンター

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
	// 攻撃の破棄
	SAFE_REF_RELEASE(m_pAttack);

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

//============================================================
//	ヒット処理
//============================================================
void CEnemyBossDragon::Hit(const int nDamage)
{
	// ヒット処理
	CEnemy::Hit(nDamage);

	if (GetLife() <= 0)
	{ // 体力がなくなった場合

		if (GET_MANAGER->GetMode() == CScene::MODE_GAME)
		{ // ゲーム画面の場合

			// ゲーム画面を終了状態にする
			CSceneGame::GetGameManager()->SetState(CGameManager::STATE_END);
		}
	}
}

//============================================================
//	ノックバックヒット処理
//============================================================
void CEnemyBossDragon::HitKnockBack(const int nDamage, const D3DXVECTOR3& vecKnock)
{
	// ノックバックヒット処理
	CEnemy::HitKnockBack(nDamage, vecKnock);
	
	if (GetLife() <= 0)
	{ // 体力がなくなった場合

		if (GET_MANAGER->GetMode() == CScene::MODE_GAME)
		{ // ゲーム画面の場合

			// ゲーム画面を終了状態にする
			CSceneGame::GetGameManager()->SetState(CGameManager::STATE_END);
		}
	}
}

//============================================================
//	通常状態時の更新処理
//============================================================
void CEnemyBossDragon::UpdateNormal(void)
{
	// TODO：攻撃処理しっかりと
	switch (m_stateNormal)
	{ // 通常時の状態ごとの処理
	case NSTATE_WAIT:

		// カウンターを加算
		m_nCounterNormal++;
		if (m_nCounterNormal > ATK_WAIT_FRAME)
		{ // 攻撃の余韻が終了した場合

			// カウンターを初期化
			m_nCounterNormal = 0;

			// 攻撃の生成
			m_pAttack = CEnemyAttack::Create((CEnemyAttack::EAttack)(rand() % 1), this);
			if (m_pAttack == nullptr) { assert(false); return; }	// 生成失敗

			// 攻撃状態にする
			m_stateNormal = NSTATE_ATTACK;
		}

		break;

	case NSTATE_ATTACK:

		if (m_pAttack != nullptr)
		{ // 攻撃が存在する場合

			// 攻撃の更新
			if (m_pAttack->Update())
			{ // 攻撃が終了している場合

				// 攻撃の破棄
				SAFE_REF_RELEASE(m_pAttack);

				// 待機状態にする
				m_stateNormal = NSTATE_WAIT;
			}
		}

		break;

	default:
		assert(false);
		break;
	}
}
