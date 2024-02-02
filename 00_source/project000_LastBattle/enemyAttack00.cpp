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
#include "camera.h"
#include "player.h"
#include "multiModel.h"
#include "impact.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const CWave::SGrow		IMPACT_GROW		= CWave::SGrow(6.0f, 0.0f, 0.0f);		// 成長量
	const CWave::SGrow		IMPACT_ADDGROW	= CWave::SGrow(0.2f, 0.002f, 0.0f);		// 成長加速量
	const CCamera::SSwing	PUNCH_SWING		= CCamera::SSwing(12.0f, 1.5f, 0.25f);	// 地面殴り時のカメラ揺れ

	const float	IMPACT_HOLE_RADIUS	= 10.0f;	// 穴の半径
	const float	IMPACT_THICKNESS	= 0.0f;		// 太さ
	const float	IMPACT_OUTER_PLUSY	= 25.0f;	// 外周のY座標加算量
	const float	IMPACT_MAX_RADIUS	= 3000.0f;	// 半径の最大成長量
	const float	TELEPORT_POS_DIS	= 800.0f;	// テレポート時のプレイヤー位置から遠ざける距離
	const int	ATTACK_WAIT_FRAME	= 10;		// 攻撃後の硬直フレーム
	const int	MAX_ATTACK			= 2;		// 攻撃回数
}

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

		float fRandRot = useful::RandomRot();				// ランダム向き
		D3DXVECTOR3 posPlayer = player->GetVec3Position();	// プレイヤーの位置

		D3DXVECTOR3 posEnemy = VEC3_ZERO;	// 敵の設定位置
		D3DXVECTOR3 rotEnemy = VEC3_ZERO;	// 敵の設定向き

		// プレイヤーからランダム方向に遠ざけた位置を設定
		posEnemy.x += posPlayer.x + sinf(fRandRot) * TELEPORT_POS_DIS;
		posEnemy.z += posPlayer.z + cosf(fRandRot) * TELEPORT_POS_DIS;

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

			// カメラ揺れを設定
			GET_MANAGER->GetCamera()->SetSwing(CCamera::TYPE_MAIN, PUNCH_SWING);

			// 衝撃波の生成
			D3DXMATRIX  mtxHandL = pBoss->GetMultiModel(CEnemyBossDragon::MODEL_HAND_L)->GetMtxWorld();	// 左手のマトリックス
			D3DXVECTOR3 posHandL = D3DXVECTOR3(mtxHandL._41, pBoss->GetVec3Position().y, mtxHandL._43);	// 左手のワールド座標
			CImpact::Create
			( // 引数
				CWave::TEXTURE_NONE,	// 種類
				posHandL,				// 位置
				IMPACT_GROW,			// 成長量
				IMPACT_ADDGROW,			// 成長加速量
				IMPACT_HOLE_RADIUS,		// 穴の半径
				IMPACT_THICKNESS,		// 太さ
				IMPACT_OUTER_PLUSY,		// 外周のY座標加算量
				IMPACT_MAX_RADIUS		// 半径の最大成長量
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
		if (m_nCounterState >= ATTACK_WAIT_FRAME)
		{ // 待機が完了した場合

			// カウンターを初期化
			m_nCounterState = 0;

			if (m_nCounterNumAtk < MAX_ATTACK)
			{ // 攻撃回数が総数に到達していない場合

				// テレポート初期化状態にする
				m_state = STATE_INIT_TELEPORT;
			}
			else
			{ // 攻撃回数が総数に到達した場合

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
