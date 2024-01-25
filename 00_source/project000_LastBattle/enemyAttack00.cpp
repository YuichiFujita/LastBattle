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
#include "player.h"
#include "multiModel.h"
#include "wave.h"

//************************************************************
//	子クラス [CEnemyAttack00] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEnemyAttack00::CEnemyAttack00() :
	m_state				(STATE_INIT_TELEPORT),	// 状態
	m_nCounterState		(0),					// 状態管理カウンター
	m_nCounterNumAtk	(0)						// 攻撃回数カウンター
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
	m_state = STATE_INIT_TELEPORT;	// 状態
	m_nCounterState  = 0;			// 状態管理カウンター
	m_nCounterNumAtk = 0;			// 攻撃回数カウンター

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
	case STATE_INIT_TELEPORT:	// テレポートの初期化
	{
		CListManager<CPlayer> *pList = CPlayer::GetList();				// プレイヤーリスト
		if (pList == nullptr)		 { assert(false); return false; }	// リスト未使用
		if (pList->GetNumAll() != 1) { assert(false); return false; }	// プレイヤーが1人じゃない
		auto player = pList->GetList().front();							// プレイヤー情報

		float fRandRot = (float)(rand() % 629 - 314) / 100.0f;	// ランダム向き
		D3DXVECTOR3 posPlayer = player->GetVec3Position();		// プレイヤーの位置

		D3DXVECTOR3 posEnemy = VEC3_ZERO;	// 敵の設定位置
		D3DXVECTOR3 rotEnemy = VEC3_ZERO;	// 敵の設定向き

		// プレイヤーからランダム方向に遠ざけた位置を設定
		posEnemy.x += posPlayer.x + sinf(fRandRot) * 1200.0f;
		posEnemy.z += posPlayer.z + cosf(fRandRot) * 1200.0f;

		// プレイヤー方向を設定
		D3DXVECTOR3 vec = posEnemy - posPlayer;
		rotEnemy.y = atan2f(vec.x, vec.z);

		// ボスをテレポートさせる
		pBoss->SetTeleport(posEnemy, rotEnemy);

		// テレポート状態にする
		m_state = STATE_TELEPORT;

		// 処理は抜けずテレポートの状態更新に移行
	}
	case STATE_TELEPORT:	// テレポート
	{
		if (pBoss->GetAction() == CEnemyBossDragon::ACT_NONE)
		{ // 何も行動していない場合

			// 波動発射の初期化状態にする
			m_state = STATE_INIT_WAVE;
		}

		break;
	}
	case STATE_INIT_WAVE:	// 波動発射の初期化
	{
		// 地面殴りの行動を設定
		pBoss->SetActPunchGround();

		// 波動発射状態にする
		m_state = STATE_WAVE;

		// 処理は抜けず波動発射の状態更新に移行
	}
	case STATE_WAVE:	// 波動発射
	{
		if (pBoss->GetMotionType() != CEnemyBossDragon::MOTION_PUNCH_GROUND)
		{ // 地面殴りモーションではない場合

			// 待機状態にする
			m_state = STATE_WAIT;
		}

		if (pBoss->GetMotionKey() == pBoss->GetMotionNumKey() - 1 && pBoss->GetMotionKeyCounter() == 0)
		{ // モーションが地面を殴ったタイミングの場合

			// 波動の生成
			D3DXMATRIX  mtx = pBoss->GetMultiModel(CEnemyBossDragon::MODEL_HAND_L)->GetMtxWorld();
			D3DXVECTOR3 pos = D3DXVECTOR3(mtx._41, pBoss->GetVec3Position().y, mtx._43);
			CWave::Create
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

			// 攻撃回数を加算
			m_nCounterNumAtk++;
		}

		break;
	}
	case STATE_WAIT:	// 待機
	{
		// カウンターを加算
		m_nCounterState++;
		if (m_nCounterState >= 10)
		{ // 待機が完了した場合

			// カウンターを初期化
			m_nCounterState = 0;

			if (m_nCounterNumAtk < 3)
			{
				// テレポート初期化状態にする
				m_state = STATE_INIT_TELEPORT;
			}
			else
			{
				// 攻撃回数を初期化
				m_nCounterNumAtk = 0;

				// 終了状態にする
				m_state = STATE_END;
			}
		}

		break;
	}
	case STATE_END:	// 終了
	{
		// 攻撃終了を返す
		return true;
	}
	default:	// 例外処理
		assert(false);
		break;
	}

	// 攻撃非終了を返す
	return false;
}
