//============================================================
//
//	敵攻撃処理 [enemyAttack.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "enemyAttack.h"
#include "manager.h"
#include "enemyBossDragon.h"
#include "wave.h"

//************************************************************
//	親クラス [CEnemyAttack] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEnemyAttack::CEnemyAttack() : m_pBoss(nullptr)
{

}

//============================================================
//	デストラクタ
//============================================================
CEnemyAttack::~CEnemyAttack()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CEnemyAttack::Init(void)
{
	// メンバ変数を初期化
	m_pBoss = nullptr;	// ボスへのポインタ

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CEnemyAttack::Uninit(void)
{

}

//============================================================
//	生成処理
//============================================================
CEnemyAttack *CEnemyAttack::Create(const EAttack attackID, CEnemyBossDragon *pBoss)
{
	// ポインタを宣言
	CEnemyAttack *pEnemyAttack = nullptr;	// 敵攻撃情報

	// 敵攻撃の生成
	switch (attackID)
	{ // 種類ごとの処理
	case ATTACK_00:
		pEnemyAttack = new CEnemyAttack00;
		break;

	//case ATTACK_01:
		//pEnemyAttack = new CEnemyAttack01;
		//break;

	default:	// 例外処理
		assert(false);
		break;
	}

	if (pEnemyAttack == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// 敵攻撃の初期化
		if (FAILED(pEnemyAttack->Init()))
		{ // 初期化に失敗した場合

			// 敵攻撃の破棄
			SAFE_DELETE(pEnemyAttack);
			return nullptr;
		}

		// ボスへのポインタを設定
		pEnemyAttack->m_pBoss = pBoss;

		// 確保したアドレスを返す
		return pEnemyAttack;
	}
}

//============================================================
//	破棄処理
//============================================================
void CEnemyAttack::Release(CEnemyAttack *&prEnemyAttack)
{
	// 敵攻撃の終了
	assert(prEnemyAttack != nullptr);
	prEnemyAttack->Uninit();

	// メモリ開放
	SAFE_DELETE(prEnemyAttack);
}

//============================================================
//	ボス取得処理
//============================================================
CEnemyBossDragon *CEnemyAttack::GetBossDragon(void)
{
	// ボスの情報を返す
	return m_pBoss;
}

//************************************************************
//	子クラス [CEnemyAttack00] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEnemyAttack00::CEnemyAttack00() :
	m_pWave			(nullptr),		// 波動の情報
	m_state			(STATE_WAVE),	// 状態
	m_nCounterState (0),			// 状態管理カウンター
	m_nCreateWave	(0)				// 波動の生成数
{

}

//============================================================
//	デストラクタ
//============================================================
CEnemyAttack00::~CEnemyAttack00()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CEnemyAttack00::Init(void)
{
	// メンバ変数を初期化
	m_pWave = nullptr;		// 波動の情報
	m_state = STATE_WAVE;	// 状態
	m_nCounterState	= 0;	// 状態管理カウンター
	m_nCreateWave	= 0;	// 波動の生成数

	// 敵攻撃の初期化
	if (FAILED(CEnemyAttack::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CEnemyAttack00::Uninit(void)
{
	// 敵攻撃の終了
	CEnemyAttack::Uninit();
}

//============================================================
//	更新処理
//============================================================
bool CEnemyAttack00::Update(void)
{
	// TODO：攻撃処理しっかりと
	switch (m_state)
	{ // 状態ごとの処理
	case STATE_WAVE:

		if (m_pWave != nullptr)
		{ // 波動がある場合

			// 波動の速度を上げる
			CWave::SGrow grow = m_pWave->GetGrow();
			m_pWave->SetGrow(CWave::SGrow(grow.fAddHoleRadius + 0.1f, grow.fAddThickness, grow.fSubAlpha));
		}

		// カウンターを加算
		m_nCounterState++;
		if (m_nCounterState > 120)
		{ // 攻撃の余韻が終了した場合

			// カウンターを初期化
			m_nCounterState = 0;

			if (m_pWave != nullptr)
			{ // 波動がある場合

				// 透明にしていくようにする
				m_pWave->SetGrow(CWave::SGrow(6.0f, 0.0f, 0.05f));
			}

			// 波動の生成数を加算
			m_nCreateWave++;
			if (m_nCreateWave > 3)
			{ // 生成仕切った場合

				// 生成数を初期化
				m_nCreateWave = 0;

				// 待機状態にする
				m_state = STATE_WAIT;

				return false;
			}
			
			// 波動の生成
			m_pWave = CWave::Create
			( // 引数
				CWave::TEXTURE_NONE,
				GetBossDragon()->GetVec3Position(),
				VEC3_ZERO,
				XCOL_BLUE,
				CWave::SGrow(2.0f * (m_nCreateWave + 1), 0.0f, 0.0f),
				POSGRID2(64, 1),
				POSGRID2(8, 1),
				100.0f,
				0.0f, 
				25.0f
			);
			if (m_pWave == nullptr)
			{ // 生成に失敗した場合

				assert(false);
				return false;
			}
		}

		break;

	case STATE_WAIT:

		// カウンターを加算
		m_nCounterState++;
		if (m_nCounterState > 120)
		{ // 攻撃の余韻が終了した場合

			// 終了状態にする
			m_state = STATE_END;
		}

	case STATE_END:

		// 攻撃終了を返す
		return true;

	default:
		assert(false);
		break;
	}

	return false;
}
