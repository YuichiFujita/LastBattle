//============================================================
//
//	敵攻撃00処理 [enemyAttack00.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "enemyAttack00.h"
#include "enemyBossDragon.h"
#include "manager.h"
#include "multiModel.h"
#include "wave.h"

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
	m_state = STATE_SET;	// 状態
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
	// ポインタを宣言
	CEnemyBossDragon *pBoss = GetBoss();	// ボスの情報

	switch (m_state)
	{ // 状態ごとの処理
	case STATE_SET:

		// 地面殴りの行動を設定
		pBoss->SetActPunchGround();

		// 波動発射状態にする
		m_state = STATE_WAVE;

		// このまま処理を抜けずに次へ

	case STATE_WAVE:

		if (m_pWave != nullptr)
		{ // 波動がある場合

			// 波動の速度を上げる
			CWave::SGrow grow = m_pWave->GetGrow();
			m_pWave->SetGrow(CWave::SGrow(grow.fAddHoleRadius + 0.25f, grow.fAddThickness, grow.fSubAlpha));
		}

		if (pBoss->GetMotionKey() == pBoss->GetMotionNumKey() - 1
		&&  pBoss->GetMotionKeyCounter() == 0)
		{ // 攻撃の余韻が終了した場合

			if (m_pWave != nullptr)
			{ // 波動がある場合

				// 透明にしていくようにする
				m_pWave->SetGrow(CWave::SGrow(6.0f, 0.0f, 0.05f));
			}

			//// 波動の生成数を加算
			//m_nCreateWave++;
			//if (m_nCreateWave > 3)
			//{ // 生成仕切った場合

			//	// 生成数を初期化
			//	m_nCreateWave = 0;

				// 待機状態にする
				m_state = STATE_WAIT;

			//	return false;
			//}
			
			// 波動の生成
			D3DXMATRIX  mtx = pBoss->GetMultiModel(CEnemyBossDragon::MODEL_HAND_L)->GetMtxWorld();
			D3DXVECTOR3 pos = D3DXVECTOR3(mtx._41, pBoss->GetVec3Position().y, mtx._43);
			m_pWave = CWave::Create
			( // 引数
				CWave::TEXTURE_NONE,
				pos,
				VEC3_ZERO,
				XCOL_BLUE,
				CWave::SGrow(6.0f, 0.0f, 0.0f),
				POSGRID2(64, 1),
				POSGRID2(8, 1),
				10.0f,
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

		break;

	case STATE_END:

		// 攻撃終了を返す
		return true;

	default:
		assert(false);
		break;
	}

	return false;
}
