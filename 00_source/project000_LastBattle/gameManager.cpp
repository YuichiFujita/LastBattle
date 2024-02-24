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
#include "enemyBossDragon.h"

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

	// ゲームUIの自動描画をOFFにする
	SetDrawGameUI(false);

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
//	ゲーム終了処理
//============================================================
void CGameManager::GameEnd(void)
{
	// 終了状態にする
	m_state = STATE_END;
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
//	ゲーム画面のUI描画設定処理
//============================================================
void CGameManager::SetDrawGameUI(const bool bDraw)
{
	// ポインタを宣言
	CTimerManager *pTimer = CSceneGame::GetTimerManager();	// タイマーマネージャーの情報
	CPlayer *pPlayer = CScene::GetPlayer();	// プレイヤーの情報
	CEnemy *pBoss = CScene::GetBoss();		// ボスの情報

	// 自動描画を設定
	pTimer->SetEnableLogoDraw(bDraw);	// タイマーロゴ
	pTimer->SetEnableDraw(bDraw);		// タイマー
	pPlayer->SetEnableDrawUI(bDraw);	// プレイヤー関連UI
	pBoss->SetEnableDrawUI(bDraw);		// ボス関連UI
}

//============================================================
//	ゲームキャラの色情報リセット処理
//============================================================
void CGameManager::ResetColorGameChara(void)
{
	// ポインタを宣言
	CPlayer *pPlayer = CScene::GetPlayer();	// プレイヤーの情報
	CEnemy *pBoss = CScene::GetBoss();		// ボスの情報

	// プレイヤーの色情報を初期化
	pPlayer->ResetMaterial();		// マテリアル再設定
	pPlayer->SetAlpha(1.0f);		// 透明度初期化
	pPlayer->SetEnableDraw(true);	// 自動描画ON

	// ボスの色情報を初期化
	pBoss->ResetMaterial();			// マテリアル再設定
	pBoss->SetAlpha(1.0f);			// 透明度初期化
	pBoss->SetEnableDraw(true);		// 自動描画ON
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
		// 開始演出の終了
		EndStart();

		break;
	}
	default:
		assert(false);
		break;
	}

	// 開始演出のスキップ
	SkipStart();

	// ボスの名前モデルの更新
	m_pBossName->Update();
}

//============================================================
//	開始演出の終了処理
//============================================================
void CGameManager::EndStart(void)
{
	CPlayer *pPlayer = CScene::GetPlayer();	// プレイヤー情報
	CEnemy  *pBoss   = CScene::GetBoss();	// ボス情報
	CTimerManager *pTimer = CSceneGame::GetTimerManager();	// タイマーマネージャーの情報
	CCinemaScope *pScope  = CSceneGame::GetCinemaScope();	// シネマスコープの情報

	// 通常状態の設定・初期化
	pPlayer->InitNormal();	// プレイヤー
	pBoss->InitNormal();	// ボス

	// ボスの名前モデルの自動描画をOFFにする
	m_pBossName->SetEnableDraw(false);

	// ゲームUIの自動描画をONにする
	SetDrawGameUI(true);

	// スコープアウトさせる
	pScope->SetScopeOut();

	// カメラを追従状態に設定
	GET_MANAGER->GetCamera()->SetState(CCamera::STATE_FOLLOW);
	GET_MANAGER->GetCamera()->SetDestFollow();	// カメラ目標位置の初期化

	// タイマーの計測開始
	pTimer->Start();

	// 通常状態にする
	m_state = STATE_NORMAL;
}

//============================================================
//	開始演出のスキップ処理
//============================================================
void CGameManager::SkipStart(void)
{
	// 開始演出が終了済みの場合抜ける
	if (m_startState == START_END) { return; }

	CInputKeyboard	*pKeyboard	= GET_INPUTKEY;	// キーボード
	CInputPad		*pPad		= GET_INPUTPAD;	// パッド

	if (pKeyboard->IsTrigger(DIK_RETURN)  || pKeyboard->IsTrigger(DIK_SPACE)
	||  pPad->IsTrigger(CInputPad::KEY_A) || pPad->IsTrigger(CInputPad::KEY_B)
	||  pPad->IsTrigger(CInputPad::KEY_X) || pPad->IsTrigger(CInputPad::KEY_Y))
	{ // 決定の操作が行われた場合

		// 開始演出を終了状態にする
		m_startState = START_END;

		// 開始演出の終了
		EndStart();
	}
}
