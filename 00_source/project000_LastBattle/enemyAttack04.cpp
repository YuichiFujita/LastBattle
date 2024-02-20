//============================================================
//
//	敵攻撃04処理 [enemyAttack04.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "enemyAttack04.h"
#include "enemyBossDragon.h"
#include "player.h"
#include "stage.h"
#include "multiModel.h"
#include "attackMoveFire.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int	ATK_MOTION_KEY	= 2;	// 攻撃生成キー
	const int	WAIT_FRAME		= 150;	// 攻撃待機フレーム
	const int	NUM_FIRE		= 8;	// 炎の生成数
	const int	NUM_ATTACK		= 3;	// 攻撃の生成数
}

//************************************************************
//	子クラス [CEnemyAttack04] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEnemyAttack04::CEnemyAttack04(CEnemyBossDragon *pBoss) : CEnemyAttack(pBoss),
	m_state				(STATE_INIT_TELEPORT),	// 状態
	m_nCounterWait		(0),					// 余韻管理カウンター
	m_nCounterNumAtk	(0)						// 攻撃回数カウンター
{

}

//============================================================
//	デストラクタ
//============================================================
CEnemyAttack04::~CEnemyAttack04()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CEnemyAttack04::Init(void)
{
	// メンバ変数を初期化
	m_state				= STATE_INIT_TELEPORT;	// 状態
	m_nCounterWait		= 0;					// 余韻管理カウンター
	m_nCounterNumAtk	= 0;					// 攻撃回数カウンター

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
void CEnemyAttack04::Uninit(void)
{
	// 敵攻撃の終了
	CEnemyAttack::Uninit();
}

//============================================================
//	更新処理
//============================================================
bool CEnemyAttack04::Update(void)
{
	// ポインタを宣言
	CEnemyBossDragon *pBoss = GetBoss();	// ボスの情報

	switch (m_state)
	{ // 状態ごとの処理
	case STATE_INIT_TELEPORT:	// テレポートの初期化
	{
		// テレポートの初期化
		InitTeleport();

		// 処理は抜けずテレポートの状態更新に移行
	}
	case STATE_TELEPORT:	// テレポート
	{
		if (pBoss->GetAction() == CEnemyBossDragon::ACT_NONE)
		{ // 何も行動していない場合

			// 攻撃待機の初期化状態にする
			m_state = STATE_INIT_WAIT;
		}

		break;
	}
	case STATE_INIT_WAIT:	// 攻撃待機の初期化
	{
		// 炎攻撃の行動を設定
		//pBoss->SetActFire();

		// 攻撃待機状態にする
		m_state = STATE_WAIT;

		// 処理は抜けず攻撃待機の状態更新に移行
	}
	case STATE_WAIT:	// 攻撃待機
	{
		if (pBoss->GetMotionKey() == 2
		&&  pBoss->GetMotionKeyCounter() == 1)
		{ // 手を振り上げたタイミングの場合

			// 炎発射状態にする
			m_state = STATE_FIRE;
		}

		break;
	}
	case STATE_FIRE:	// 炎発射
	{
		// 炎発射の更新
		UpdateFire();

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

//============================================================
//	テレポートの初期化処理
//============================================================
void CEnemyAttack04::InitTeleport(void)
{
	// ポインタを宣言
	CStage  *pStage  = CScene::GetStage();	// ステージの情報
	CPlayer *pPlayer = CScene::GetPlayer();	// プレイヤーの情報
	CEnemyBossDragon *pBoss = GetBoss();	// ボスの情報

	CStage::SStageLimit stageLimit = pStage->GetStageLimit();	// ステージ範囲情報
	D3DXVECTOR3 posPlayer = pPlayer->GetVec3Position();			// プレイヤーの位置
	D3DXVECTOR3 posEnemy = VEC3_ZERO;	// 敵の設定位置
	D3DXVECTOR3 rotEnemy = VEC3_ZERO;	// 敵の設定向き

	// 中心からプレイヤーへの向きを求める
	D3DXVECTOR3 vec = stageLimit.center - posPlayer;
	float fRot = atan2f(vec.x, vec.z);

	// 最大限遠ざけた位置を設定
	posEnemy.x += stageLimit.center.x + sinf(fRot) * stageLimit.fRadius * 0.5f;
	posEnemy.z += stageLimit.center.z + cosf(fRot) * stageLimit.fRadius * 0.5f;

	// プレイヤー方向を向かせる
	D3DXVECTOR3 vecPlayer = posEnemy - posPlayer;
	rotEnemy.y = atan2f(vecPlayer.x, vecPlayer.z);

	// ボスをテレポートさせる
	pBoss->SetTeleport(posEnemy, rotEnemy);

	// テレポート状態にする
	m_state = STATE_TELEPORT;
}

//============================================================
//	炎発射の更新処理
//============================================================
void CEnemyAttack04::UpdateFire(void)
{
	// カウンターを加算
	m_nCounterWait++;
	if (m_nCounterWait > WAIT_FRAME)
	{ // 余韻フレームが終了した場合

		// カウンターを初期化
		m_nCounterWait = 0;

		// 炎の生成
		CreateFire();

		// 攻撃回数を加算
		m_nCounterNumAtk++;
		if (m_nCounterNumAtk >= NUM_ATTACK)
		{ // 攻撃がステージ端に到達した場合

			// 終了状態にする
			m_state = STATE_END;
		}
	}
}

//============================================================
//	炎の生成処理
//============================================================
void CEnemyAttack04::CreateFire(void)
{
	CEnemyBossDragon *pBoss	= GetBoss();	// ボスの情報
	CMultiModel	*pObjJaw	= pBoss->GetMultiModel(CEnemyBossDragon::MODEL_JAW);	// 顎モデル
	D3DXMATRIX	mtxJaw		= pObjJaw->GetMtxWorld();				// 顎マトリックス
	D3DXVECTOR3	posJaw		= useful::GetMatrixPosition(mtxJaw);	// 顎ワールド座標

	// TODO：モーション変えたら消す
	posJaw.y = 100.0f;

	int nNumFire = NUM_FIRE + m_nCounterNumAtk * 3;	// 攻撃生成数

	float fFf = D3DX_PI * 0.25f;
	float fRotRate = (D3DX_PI - fFf * 2.0f) / (nNumFire - 1);	// 攻撃方向の割合

	float fStartRot	= pBoss->GetVec3Rotation().y + (D3DX_PI * 0.5f) + fFf;	// 攻撃生成開始向き

	for (int nCntAttack = 0; nCntAttack < nNumFire; nCntAttack++)
	{ // 攻撃の生成数分繰り返す

		float fMoveRot = fStartRot + fRotRate * nCntAttack;	// 炎の生成方向
		D3DXVECTOR3 vecMove = D3DXVECTOR3(sinf(fMoveRot), 0.0f, cosf(fMoveRot));	// 炎の移動方向

		// 炎攻撃の生成
		CAttackMoveFire::Create
		( // 引数
			posJaw,		// 生成位置
			vecMove,	// 移動方向
			16.5f,		// 移動量
			0.09f,		// 加速量
			100			// 寿命
		);
	}
}
