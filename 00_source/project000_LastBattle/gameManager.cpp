//============================================================
//
//	ゲームマネージャー処理 [gameManager.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "gameManager.h"
#include "manager.h"
#include "fade.h"
#include "scene.h"
#include "sceneGame.h"
#include "cinemaScope.h"
#include "timerManager.h"
#include "retentionManager.h"
#include "modelFont.h"
#include "camera.h"
#include "player.h"
#include "enemy.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const D3DXVECTOR3 POS_NAME = D3DXVECTOR3(0.0f, 100.0f, 400.0f);	// 名前の表示位置

	const int GAMEEND_WAIT_FRAME = 120;	// リザルト画面への遷移余韻フレーム
}

//************************************************************
//	親クラス [CGameManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CGameManager::CGameManager() :
	m_pBossName		(nullptr),		// ボスの名前モデル情報
	m_state			(STATE_NONE),	// 状態
	m_startState	(START_PLAYER)	// 開始状態
{

}

//============================================================
//	デストラクタ
//============================================================
CGameManager::~CGameManager()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CGameManager::Init(void)
{
	// ポインタを宣言
	CCinemaScope *pScope = CSceneGame::GetCinemaScope();	// シネマスコープの情報
	CPlayer *pPlayer = CScene::GetPlayer();	// プレイヤーの情報
	CEnemy *pBoss = CScene::GetBoss();		// ボスの情報

	// メンバ変数を初期化
	m_pBossName	 = nullptr;			// ボスの名前モデル情報
	m_state		 = STATE_START;		// 状態
	m_startState = START_PLAYER;	// 開始状態

	// ボスの名前モデルの生成
	m_pBossName = CModelFont::Create
	( // 引数
		CModelFont::TYPE_BOSS_NAME,	// 種類
		POS_NAME					// 位置
	);
	if (m_pBossName == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 自動描画をOFFにする
	m_pBossName->SetEnableDraw(false);

	// スコープインさせる
	pScope->SetScopeIn();

	// プレイヤーを出現させる
	pPlayer->SetSpawn();

	// UIの自動描画をOFFにする
	pPlayer->SetEnableDrawUI(false);	// プレイヤー関連UI
	pBoss->SetEnableDrawUI(false);		// ボス関連UI

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CGameManager::Uninit(void)
{
	// ボスの名前モデルの終了
	SAFE_UNINIT(m_pBossName);
}

//============================================================
//	更新処理
//============================================================
void CGameManager::Update(void)
{
	switch (m_state)
	{ // 状態ごとの処理
	case STATE_NONE:
	case STATE_END:
		break;

	case STATE_START:

		// 開始状態の更新
		UpdateStart();

		// ボスの名前モデルの更新
		m_pBossName->Update();

		break;

	case STATE_NORMAL:

		if (CSceneGame::GetTimerManager()->GetState() == CTimerManager::STATE_END)
		{ // 計測が終了していた場合

			// リザルト画面に遷移させる
			TransitionResult(CRetentionManager::WIN_FAILED);
		}

		break;

	default:	// 例外処理
		assert(false);
		break;
	}
}

//============================================================
//	状態取得処理
//============================================================
CGameManager::EState CGameManager::GetState(void) const
{
	// 状態を返す
	return m_state;
}

//============================================================
//	リザルト画面遷移処理
//============================================================
void CGameManager::TransitionResult(const CRetentionManager::EWin win)
{
	// 終了状態にする
	m_state = STATE_END;

	// リザルト情報を保存
	GET_RETENTION->SetResult(win, CSceneGame::GetTimerManager()->Get());

	// タイマーの計測終了
	CSceneGame::GetTimerManager()->End();

	// リザルト画面に遷移
	GET_MANAGER->SetScene(CScene::MODE_RESULT, GAMEEND_WAIT_FRAME);
}

//============================================================
//	生成処理
//============================================================
CGameManager *CGameManager::Create(void)
{
	// ゲームマネージャーの生成
	CGameManager *pGameManager = new CGameManager;
	if (pGameManager == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// ゲームマネージャーの初期化
		if (FAILED(pGameManager->Init()))
		{ // 初期化に失敗した場合

			// ゲームマネージャーの破棄
			SAFE_DELETE(pGameManager);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pGameManager;
	}
}

//============================================================
//	破棄処理
//============================================================
void CGameManager::Release(CGameManager *&prGameManager)
{
	// ゲームマネージャーの終了
	assert(prGameManager != nullptr);
	prGameManager->Uninit();

	// メモリ開放
	SAFE_DELETE(prGameManager);
}

//============================================================
//	開始状態の更新処理
//============================================================
void CGameManager::UpdateStart(void)
{
	if (GET_MANAGER->GetFade()->GetState() != CFade::FADE_NONE) { return; }	// フェード中の場合抜ける

	CPlayer *pPlayer = CScene::GetPlayer();	// プレイヤー情報
	CEnemy  *pBoss   = CScene::GetBoss();	// ボス情報

	switch (m_startState)
	{ // 開始状態ごとの処理
	case START_PLAYER:	// プレイヤースポーン状態
	{
		if (pPlayer->GetState() == CPlayer::STATE_NONE)
		{ // スポーン演出が終了した場合

			// ボスを出現させる
			pBoss->SetState(CEnemy::STATE_SPAWN);

			// ボスの名前モデルの自動描画をONにする
			m_pBossName->SetEnableDraw(true);

			// ボススポーン状態にする
			m_startState = START_BOSS;
		}

		break;
	}
	case START_BOSS:	// ボススポーン状態
	{
		if (pBoss->GetState() == CEnemy::STATE_NONE)
		{ // スポーン演出が終了した場合

			// 終了状態にする
			m_startState = START_END;
		}

		break;
	}
	case START_END:	// 終了状態
	{
		// 通常状態にする
		pPlayer->SetState(CPlayer::STATE_NORMAL);	// プレイヤー
		pBoss->SetState(CEnemy::STATE_NORMAL);		// ボス

		// ボスの名前モデルの自動描画をOFFにする
		m_pBossName->SetEnableDraw(false);

		// UIの自動描画をOFFにする
		pPlayer->SetEnableDrawUI(true);	// プレイヤー関連UI
		pBoss->SetEnableDrawUI(true);	// ボス関連UI

		// スコープアウトさせる
		CSceneGame::GetCinemaScope()->SetScopeOut();

		// カメラを追従状態に設定
		GET_MANAGER->GetCamera()->SetState(CCamera::STATE_FOLLOW);
		GET_MANAGER->GetCamera()->SetDestFollow();	// カメラ目標位置の初期化

		// タイマーの計測開始
		CSceneGame::GetTimerManager()->Start();

		// 通常状態にする
		m_state = STATE_NORMAL;

		break;
	}
	default:
		assert(false);
		break;
	}
}
