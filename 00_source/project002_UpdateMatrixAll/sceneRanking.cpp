//============================================================
//
//	ランキング画面処理 [sceneRanking.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "sceneRanking.h"
#include "manager.h"
#include "texture.h"
#include "sound.h"
#include "camera.h"
#include "rankingManager.h"

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CRankingManager *CSceneRanking::m_pRankingManager = nullptr;	// ランキングマネージャー

//************************************************************
//	子クラス [CSceneRanking] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CSceneRanking::CSceneRanking(const EMode mode) : CScene(mode)
{

}

//============================================================
//	デストラクタ
//============================================================
CSceneRanking::~CSceneRanking()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CSceneRanking::Init(void)
{
	//--------------------------------------------------------
	//	ランキングの初期化
	//--------------------------------------------------------
	// ランキングマネージャーの生成
	m_pRankingManager = CRankingManager::Create();
	if (m_pRankingManager == nullptr)
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
	// カメラを設定
	GET_MANAGER->GetCamera()->SetState(CCamera::STATE_ROTATE);	// カメラを回転状態に設定
	GET_MANAGER->GetCamera()->SetDestRotate();	// 目標位置を設定

	// BGMの再生
	PLAY_SOUND(CSound::LABEL_BGM_GENERAL);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CSceneRanking::Uninit(void)
{
	// ランキングマネージャーの破棄
	SAFE_REF_RELEASE(m_pRankingManager);

	// シーンの終了
	CScene::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CSceneRanking::Update(void)
{
	// ランキングマネージャーの更新
	assert(m_pRankingManager != nullptr);
	m_pRankingManager->Update();

	// シーンの更新
	CScene::Update();
}

//============================================================
//	描画処理
//============================================================
void CSceneRanking::Draw(void)
{

}

//============================================================
//	ランキングマネージャー取得処理
//============================================================
CRankingManager *CSceneRanking::GetRankingManager(void)
{
	// インスタンス未使用
	assert(m_pRankingManager != nullptr);

	// ランキングマネージャーを返す
	return m_pRankingManager;
}
