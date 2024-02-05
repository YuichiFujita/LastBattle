//============================================================
//
//	敵攻撃03処理 [enemyAttack03.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "enemyAttack03.h"
#include "enemyBossDragon.h"
#include "manager.h"
#include "camera.h"
#include "player.h"
#include "stage.h"
#include "attackThunder.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const float	TELEPORT_POS_DIS	= 3600.0f;	// テレポート時のステージ中心位置から遠ざける距離
	const float	TELEPORT_POSY		= 800.0f;	// テレポート時のY座標

	const int	MAX_SUB_WARN_WAIT	= 65;		// 警告表示フレームの最大減算量
	const int	INIT_WAIT_FRAME		= 22;		// 初期攻撃待機フレーム
	const int	NUM_ATTACK			= 20;		// 攻撃の生成数
	const int	SUB_WAIT_FRAME		= 1;		// 一攻撃ごとの攻撃待機フレーム加算量
}

//************************************************************
//	子クラス [CEnemyAttack03] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEnemyAttack03::CEnemyAttack03() :
	m_state				(STATE_INIT_TELEPORT),	// 状態
	m_nCounterWait		(0),					// 余韻管理カウンター
	m_nCounterNumAtk	(0),					// 攻撃回数カウンター
	m_nWaitFrame		(0)						// 余韻フレーム数
{

}

//============================================================
//	デストラクタ
//============================================================
CEnemyAttack03::~CEnemyAttack03()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CEnemyAttack03::Init(void)
{
	// メンバ変数を初期化
	m_state				= STATE_INIT_TELEPORT;	// 状態
	m_nCounterWait		= 0;					// 余韻管理カウンター
	m_nCounterNumAtk	= 0;					// 攻撃回数カウンター
	m_nWaitFrame		= INIT_WAIT_FRAME;		// 余韻フレーム数

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
void CEnemyAttack03::Uninit(void)
{
	// 敵攻撃の終了
	CEnemyAttack::Uninit();
}

//============================================================
//	更新処理
//============================================================
bool CEnemyAttack03::Update(void)
{
	// ポインタを宣言
	CEnemyBossDragon *pBoss = GetBoss();	// ボスの情報
	CStage *pStage = CScene::GetStage();	// ステージの情報

	switch (m_state)
	{ // 状態ごとの処理
	case STATE_INIT_TELEPORT:	// テレポートの初期化
	{
		D3DXVECTOR3 posEnemy = VEC3_ZERO;	// 敵の設定位置
		D3DXVECTOR3 rotEnemy = VEC3_ZERO;	// 敵の設定向き

		// ランダム方向上空に敵の位置を設定
		float fRandRot = useful::RandomRot();
		posEnemy.x = sinf(fRandRot) * TELEPORT_POS_DIS;
		posEnemy.y = TELEPORT_POSY;
		posEnemy.z = cosf(fRandRot) * TELEPORT_POS_DIS;

		// ステージ中心方向に敵の向きを設定
		D3DXVECTOR3 vec = posEnemy - pStage->GetStageLimit().center;
		rotEnemy.y = atan2f(vec.x, vec.z);

		// ボスをテレポートさせる
		pBoss->SetTeleport(posEnemy, rotEnemy, CEnemyBossDragon::MOTION_FLY_IDOL);

		// テレポート状態にする
		m_state = STATE_TELEPORT;

		// 処理は抜けずテレポートの状態更新に移行
	}
	case STATE_TELEPORT:	// テレポート
	{
		if (pBoss->GetAction() == CEnemyBossDragon::ACT_NONE)
		{ // 何も行動していない場合

			// 雷発射の初期化状態にする
			m_state = STATE_INIT_THUNDER;
		}

		break;
	}
	case STATE_INIT_THUNDER:	// 雷発射の初期化
	{
		// 空中攻撃の行動を設定
		pBoss->SetActFlyAttack();

		// 雷発射状態にする
		m_state = STATE_THUNDER;

		// 処理は抜けず雷発射の状態更新に移行
	}
	case STATE_THUNDER:	// 雷発射
	{
		// カウンターを加算
		m_nCounterWait++;
		if (m_nCounterWait > m_nWaitFrame)
		{ // 余韻フレームが終了した場合

			// カウンターを初期化
			m_nCounterWait = 0;

			// 余韻の長さを短縮
			m_nWaitFrame -= SUB_WAIT_FRAME;

			CListManager<CPlayer> *pList = CPlayer::GetList();				// プレイヤーリスト
			if (pList == nullptr)		 { assert(false); return false; }	// リスト未使用
			if (pList->GetNumAll() != 1) { assert(false); return false; }	// プレイヤーが1人じゃない
			auto player = pList->GetList().front();							// プレイヤー情報

			// 雷攻撃をプレイヤー位置に生成
			int nWarnFrame = attackThunder::WARN_FRAME - (MAX_SUB_WARN_WAIT / NUM_ATTACK) * m_nCounterNumAtk;	// 警告表示フレーム
			CAttackThunder::Create(player->GetVec3Position(), nWarnFrame);

			// 攻撃回数を加算
			m_nCounterNumAtk++;
			if (m_nCounterNumAtk >= NUM_ATTACK)
			{ // ステージ端に到達した場合

				// 中央テレポートの初期化状態にする
				m_state = STATE_CENTER_TELEPORT_INIT;
			}
		}

		break;
	}
	case STATE_CENTER_TELEPORT_INIT:	// 中央テレポートの初期化
	{
		CListManager<CPlayer> *pList = CPlayer::GetList();				// プレイヤーリスト
		if (pList == nullptr)		 { assert(false); return false; }	// リスト未使用
		if (pList->GetNumAll() != 1) { assert(false); return false; }	// プレイヤーが1人じゃない
		auto player = pList->GetList().front();							// プレイヤー情報

		D3DXVECTOR3 posPlayer = player->GetVec3Position();		// プレイヤーの位置
		D3DXVECTOR3 posEnemy  = pStage->GetStageLimit().center;	// 敵の位置
		D3DXVECTOR3 rotEnemy  = VEC3_ZERO;	// 敵の設定向き

		// プレイヤー方向を設定
		D3DXVECTOR3 vec = posEnemy - posPlayer;
		rotEnemy.y = atan2f(vec.x, vec.z);

		// ボスをテレポートさせる
		pBoss->SetTeleport(posEnemy, rotEnemy, CEnemyBossDragon::MOTION_IDOL);

		// 中央テレポート状態にする
		m_state = STATE_CENTER_TELEPORT;

		// 処理は抜けず中央テレポートの状態更新に移行

		break;
	}
	case STATE_CENTER_TELEPORT:	// 中央テレポート
	{
		if (pBoss->GetAction() == CEnemyBossDragon::ACT_NONE)
		{ // 何も行動していない場合

			// 終了状態にする
			m_state = STATE_END;
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
