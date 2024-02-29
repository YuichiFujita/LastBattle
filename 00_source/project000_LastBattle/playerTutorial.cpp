//============================================================
//
//	プレイヤーチュートリアル処理 [playerTutorial.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "playerTutorial.h"
#include "manager.h"
#include "camera.h"
#include "sceneTutorial.h"
#include "tutorialManager.h"
#include "renderer.h"
#include "sound.h"
#include "stage.h"
#include "sword.h"
#include "shadow.h"
#include "anim2D.h"
#include "effect3D.h"
#include "particle3D.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const D3DXVECTOR3 POS = D3DXVECTOR3(0.0f, 800.0f, 0.0f);	// プレイヤー生成位置
}

//************************************************************
//	子クラス [CPlayerTutorial] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CPlayerTutorial::CPlayerTutorial()
{

}

//============================================================
//	デストラクタ
//============================================================
CPlayerTutorial::~CPlayerTutorial()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CPlayerTutorial::Init(void)
{
	// プレイヤーの初期化
	if (FAILED(CPlayer::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 通常状態の初期化
	InitNormal();

	// 待機モーションを設定
	//SetMotion(BODY_LOWER, L_MOTION_IDOL);
	//SetMotion(BODY_UPPER, U_MOTION_IDOL);

	// 自動更新・自動描画をONにする
	SetEnableUpdate(true);
	SetEnableDraw(true);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CPlayerTutorial::Uninit(void)
{
	// プレイヤーの終了
	CPlayer::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CPlayerTutorial::Update(void)
{
#if 1
	if (GET_MANAGER->GetMode() == CScene::MODE_TUTORIAL)
	{ // チュートリアル画面の場合

		CCamera *pCamera = GET_MANAGER->GetCamera();								// カメラ情報
		CTutorialManager *pTutorialManager = CSceneTutorial::GetTutorialManager();	// チュートリアルマネージャー情報
		CTutorialManager::EState state = pTutorialManager->GetState();				// チュートリアル状態
		if (state != CTutorialManager::STATE_PROGRESSION
		&&  state != CTutorialManager::STATE_NEXTWAIT)
		{ // レッスン確認状態の場合

			// カメラの更新をOFFにする
			pCamera->SetEnableUpdate(false);
			return;
		}
		else
		{ // レッスン確認状態ではない場合

			// カメラの更新をONにする
			pCamera->SetEnableUpdate(true);
		}
	}

	// プレイヤーの更新
	CPlayer::Update();
#else
	// オブジェクト分割キャラクターの更新
	CObjectDivChara::Update();

	// 過去位置の更新
	UpdateOldPosition();



	bool bColl[] =	// 判定ON/OFF状況
	{
		IsLeftWeaponCollision(BODY_UPPER),	// 左手の武器の攻撃判定
		IsRightWeaponCollision(BODY_UPPER),	// 右手の武器の攻撃判定
	};
	for (int nCntSword = 0; nCntSword < player::NUM_SWORD; nCntSword++)
	{ // 剣の数分繰り返す

		CSword *pSword = GetSword(nCntSword);	// 剣の情報

		// 剣の攻撃判定を設定
		pSword->SetEnableAttack(bColl[nCntSword]);

		// 剣の更新
		pSword->Update();
	}

	// 影の更新
	GetShadow()->Update();
#endif
}

//============================================================
//	描画処理
//============================================================
void CPlayerTutorial::Draw(CShader * /*pShader*/)
{
	// プレイヤーの描画
	CPlayer::Draw();
}

//============================================================
//	通常状態時の更新処理
//============================================================
void CPlayerTutorial::UpdateNormal(int *pLowMotion, int *pUpMotion)
{
	// TODO：デバッグカウンター加算
	if (GET_INPUTKEY->IsPress(DIK_0))
	{
		// レッスンカウンター加算
		CSceneTutorial::GetTutorialManager()->AddLessonCounter();
	}

	// 変数を宣言
	D3DXVECTOR3 posPlayer = GetVec3Position();	// プレイヤー位置
	D3DXVECTOR3 rotPlayer = GetVec3Rotation();	// プレイヤー向き

	// ポインタを宣言
	CStage *pStage = CScene::GetStage();				// ステージ情報
	if (pStage == nullptr) { assert(false); return; }	// ステージ非使用中

	// 重力の更新
	UpdateGravity();

	if (CSceneTutorial::GetTutorialManager()->GetLesson() >= CTutorialManager::LESSON_04)
	{ // レッスン04：攻撃操作の挑戦中、またはクリア済みの場合

		// 攻撃操作
		UpdateAttack(posPlayer, rotPlayer);
	}

	if (CSceneTutorial::GetTutorialManager()->GetLesson() >= CTutorialManager::LESSON_03)
	{ // レッスン03：回避操作の挑戦中、またはクリア済みの場合

		// 回避操作
		UpdateDodge(rotPlayer);
	}

	if (CSceneTutorial::GetTutorialManager()->GetLesson() >= CTutorialManager::LESSON_01)
	{ // レッスン01：移動・カメラ操作の挑戦中、またはクリア済みの場合

		// 移動操作・目標向き設定
		UpdateMove(pLowMotion, pUpMotion);
	}

	if (CSceneTutorial::GetTutorialManager()->GetLesson() >= CTutorialManager::LESSON_02)
	{ // レッスン02：ジャンプ操作の挑戦中、またはクリア済みの場合

		// ジャンプ操作
		UpdateJump(pLowMotion, pUpMotion);
	}

	// 位置更新
	UpdatePosition(&posPlayer);

	// 敵との当たり判定の更新
	UpdateCollEnemy(&posPlayer);

	// 着地判定
	UpdateLanding(&posPlayer);

	// 向き更新
	UpdateRotation(&rotPlayer);

	// ステージ範囲外の補正
	pStage->LimitPosition(posPlayer, GetRadius());

	// 位置を反映
	SetVec3Position(posPlayer);

	// 向きを反映
	SetVec3Rotation(rotPlayer);
}
