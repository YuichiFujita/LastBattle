//============================================================
//
//	チュートリアル画面処理 [sceneTutorial.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "sceneTutorial.h"
#include "manager.h"
#include "sound.h"
#include "camera.h"
#include "tutorialManager.h"

#include "player.h"
#include "stage.h"
#include "wall.h"
#include "scenery.h"
#include "sky.h"

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CTutorialManager *CSceneTutorial::m_pTutorialManager = nullptr;	// チュートリアルマネージャー

//************************************************************
//	子クラス [CSceneTutorial] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CSceneTutorial::CSceneTutorial(const EMode mode) : CScene(mode)
{

}

//============================================================
//	デストラクタ
//============================================================
CSceneTutorial::~CSceneTutorial()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CSceneTutorial::Init(void)
{
	//--------------------------------------------------------
	//	チュートリアルの初期化
	//--------------------------------------------------------
	// チュートリアルマネージャーの生成
	m_pTutorialManager = CTutorialManager::Create();
	if (m_pTutorialManager == nullptr)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// シーンの初期化
	CScene::Init();

	//--------------------------------------------------------
	//	初期設定
	//--------------------------------------------------------
	CListManager<CPlayer> *pList = CPlayer::GetList();				// プレイヤーリスト
	if (pList == nullptr)		 { assert(false); return E_FAIL; }	// リスト未使用
	if (pList->GetNumAll() != 1) { assert(false); return E_FAIL; }	// プレイヤーが1人じゃない
	auto player = pList->GetList().front();							// プレイヤー情報

	// カメラを設定
	GET_MANAGER->GetCamera()->SetState(CCamera::STATE_FOLLOW);	// カメラを追従状態に設定

	// プレイヤーを出現させる
	player->SetSpawn();

	// BGMの再生
	GET_MANAGER->GetSound()->Play(CSound::LABEL_BGM_TUTORIAL);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CSceneTutorial::Uninit(void)
{
	// チュートリアルマネージャーの破棄
	SAFE_REF_RELEASE(m_pTutorialManager);

	// シーンの終了
	CScene::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CSceneTutorial::Update(void)
{
	// チュートリアルマネージャーの更新
	assert(m_pTutorialManager != nullptr);
	m_pTutorialManager->Update();

	// シーンの更新
	CScene::Update();
}

//============================================================
//	描画処理
//============================================================
void CSceneTutorial::Draw(void)
{

}

//============================================================
//	チュートリアルマネージャー取得処理
//============================================================
CTutorialManager *CSceneTutorial::GetTutorialManager(void)
{
	// インスタンス未使用
	assert(m_pTutorialManager != nullptr);

	// チュートリアルマネージャーのポインタを返す
	return m_pTutorialManager;
}
