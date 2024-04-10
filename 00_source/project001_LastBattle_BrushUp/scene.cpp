//============================================================
//
//	シーン処理 [scene.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "scene.h"
#include "manager.h"
#include "renderer.h"
#include "light.h"
#include "camera.h"
#include "sceneTitle.h"
#include "sceneTutorial.h"
#include "sceneGame.h"
#include "sceneResult.h"
#include "sceneRanking.h"

#include "stage.h"
#include "player.h"
#include "enemy.h"

// TODO：文字
#include "object2D.h"
#include "font.h"
#include "object2D.h"
#include "font2D.h"

// TODO：スキンメッシュ
#include "objectSkinMesh3D.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	// ボスの情報
	namespace boss
	{
		const D3DXVECTOR3 SPAWN_POS = D3DXVECTOR3(0.0f, 0.0f, 600.0f);	// スポーン位置
		const D3DXVECTOR3 SPAWN_ROT = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// スポーン向き
	}
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CStage *CScene::m_pStage = nullptr;	// ステージの情報

//************************************************************
//	親クラス [CScene] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CScene::CScene(const EMode mode) : m_mode(mode)
{

}

//============================================================
//	デストラクタ
//============================================================
CScene::~CScene()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CScene::Init(void)
{
	// ステージの生成
	m_pStage = CStage::Create(m_mode);
	if (m_pStage == nullptr)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	if (m_mode == MODE_GAME)
	{ // ゲーム画面の場合

		// ボスの生成
		CEnemy::Create
		( // 引数
			CEnemy::TYPE_BOSS_DRAGON,	// 種類
			boss::SPAWN_POS,			// 位置
			boss::SPAWN_ROT				// 向き
		);
	}

	// プレイヤーの生成
	CPlayer::Create(m_mode);

#if 1
#if 0
	// フォント2Dの生成
	CFont2D *pCharA = CFont2D::Create
	( // 引数
		GET_MANAGER->GetFont()->RegistName("わんぱくルイカ").pFontChar,
		L'h',
		D3DXVECTOR3(150.0f, 150.0f, 0.0f)
	);

	// フォント2Dの生成
	CFont2D *pCharB = CFont2D::Create
	( // 引数
		GET_MANAGER->GetFont()->RegistName("わんぱくルイカ").pFontChar,
		L'i',
		D3DXVECTOR3(pCharA->GetVec3Position().x + pCharA->GetNext() * 0.17f, 150.0f, 0.0f)
	);

	// フォント2Dの生成
	CFont2D *pCharC = CFont2D::Create
	( // 引数
		GET_MANAGER->GetFont()->RegistName("わんぱくルイカ").pFontChar,
		L'j',
		D3DXVECTOR3(pCharB->GetVec3Position().x + pCharB->GetNext() * 0.17f, 150.0f, 0.0f)
	);

	// フォント2Dの生成
	CFont2D *pCharD = CFont2D::Create
	( // 引数
		GET_MANAGER->GetFont()->RegistName("わんぱくルイカ").pFontChar,
		L'k',
		D3DXVECTOR3(pCharC->GetVec3Position().x + pCharC->GetNext() * 0.17f, 150.0f, 0.0f)
	);
#else
#if 1
	for (int nHeight = 0; nHeight < 8; nHeight++)
	{
		float fNext = 150.0f;
		for (int nWidth = 0; nWidth < 16; nWidth++)
		{
			// フォント2Dの生成
			CFont2D *p = CFont2D::Create
			( // 引数
				GET_MANAGER->GetFont()->RegistName("わんぱくルイカ").pFontChar,
				L'a' + nWidth + (nHeight * 16),
				D3DXVECTOR3(fNext - ((float)GET_MANAGER->GetFont()->RegistChar(L'a' + nWidth + (nHeight * 16), "わんぱくルイカ").glyph.gmptGlyphOrigin.x * 0.17f), 150.0f + (nHeight * 100.0f), 0.0f)
			);
			if (p == nullptr)
			{ // 生成に失敗した場合

				// 失敗を返す
				assert(false);
				return E_FAIL;
			}

			// 優先順位の設定
			p->SetPriority(7);

			// ラベルの設定
			p->SetLabel(CObject::LABEL_EFFECT);

			// 次の位置までの空白量を加算
			fNext = (p->GetVec3Position().x/* - p->GetVec3Sizing().x * 0.5f*/ + p->GetNext() * 0.17f);
		}
	}
#else
#if 1
	for (int i = 0; i < 300; i++)
	{
		// フォント2Dの生成
		CFont2D *p = CFont2D::Create
		( // 引数
			GET_MANAGER->GetFont()->RegistName("Times New Roman").pFontChar,
			L'a' + i,
			D3DXVECTOR3(1255.0f - ((i % 25) * 50.0f), 25.0f + ((i / 25) * 50.0f), 0.0f),	// 位置
			1.0f,		// 大きさ
			VEC3_ZERO,	// 向き
			XCOL_WHITE	// 色
		);
		if (p == nullptr)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// 文字テクスチャを割当
		//p->BindTexture(GET_MANAGER->GetFont()->RegistChar(L'あ' + i, "わんぱくルイカ").nTexID);
		//p->BindTexture(GET_MANAGER->GetFont()->RegistChar(L'a' + i, "Times New Roman").nTexID);

		// 優先順位の設定
		p->SetPriority(7);

		// ラベルの設定
		p->SetLabel(CObject::LABEL_EFFECT);
	}
#else
	for (int i = 0; i < 300; i++)
	{
		// オブジェクト2Dの生成
		CObject2D *p = CObject2D::Create
		( // 引数
			D3DXVECTOR3(1255.0f - ((i % 25) * 50.0f), 25.0f + ((i / 25) * 50.0f), 0.0f),	// 位置
			D3DXVECTOR3(50.0f, 50.0f, 0.0f),	// 大きさ
			VEC3_ZERO,	// 向き
			XCOL_WHITE	// 色
		);
		if (p == nullptr)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// 文字テクスチャを割当
		//p->BindTexture(GET_MANAGER->GetFont()->RegistChar(L'あ' + i, "わんぱくルイカ").nTexID);
		p->BindTexture(GET_MANAGER->GetFont()->RegistChar(L'a' + i, "Times New Roman").nTexID);

		// 優先順位の設定
		p->SetPriority(7);

		// ラベルの設定
		p->SetLabel(CObject::LABEL_EFFECT);
	}

#if 0
	// オブジェクト2Dの生成
	CObject2D *p = CObject2D::Create
	( // 引数
		D3DXVECTOR3(1255.0f, 25.0f, 0.0f),	// 位置
		D3DXVECTOR3(50.0f, 50.0f, 0.0f),	// 大きさ
		VEC3_ZERO,	// 向き
		XCOL_BLACK	// 色
	);
	if (p == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 文字テクスチャを割当
	//p->BindTexture(GET_MANAGER->GetFont()->RegistChar(L'あ', "わんぱくルイカ").nTexID);
	p->BindTexture(GET_MANAGER->GetFont()->RegistChar(L'あ', "わんぱくルイカ", true).nTexID);

	// 優先順位の設定
	p->SetPriority(7);

	// ラベルの設定
	p->SetLabel(CObject::LABEL_EFFECT);
#endif
#endif
#endif
#endif
#endif

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CScene::Uninit(void)
{
	// ステージの破棄
	SAFE_REF_RELEASE(m_pStage);
}

//============================================================
//	更新処理
//============================================================
void CScene::Update(void)
{
	// ポインタを宣言
	CManager	*pManager	= GET_MANAGER;				// マネージャー
	CLight		*pLight		= pManager->GetLight();		// ライト
	CCamera		*pCamera	= pManager->GetCamera();	// カメラ
	CRenderer	*pRenderer	= pManager->GetRenderer();	// レンダラー

	// ステージの更新
	assert(m_pStage != nullptr);
	m_pStage->Update();

	// ライトの更新
	assert(pLight != nullptr);
	pLight->Update();

	// カメラの更新
	assert(pCamera != nullptr);
	pCamera->Update();

	// レンダラーの更新
	assert(pRenderer != nullptr);
	pRenderer->Update();
}

//============================================================
//	生成処理
//============================================================
CScene *CScene::Create(EMode mode)
{
	// ポインタを宣言
	CScene *pScene = nullptr;	// シーン情報

	// シーンの生成
	switch (mode)
	{ // モードごとの処理
	case MODE_TITLE:
		pScene = new CSceneTitle(mode);
		break;

	case MODE_TUTORIAL:
		pScene = new CSceneTutorial(mode);
		break;

	case MODE_GAME:
		pScene = new CSceneGame(mode);
		break;

	case MODE_RESULT:
		pScene = new CSceneResult(mode);
		break;

	case MODE_RANKING:
		pScene = new CSceneRanking(mode);
		break;

	default:	// 例外処理
		assert(false);
		break;
	}

	if (pScene == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// 確保したアドレスを返す
		return pScene;
	}
}

//============================================================
//	破棄処理
//============================================================
void CScene::Release(CScene *&prScene)
{
	// シーンの終了
	assert(prScene != nullptr);
	prScene->Uninit();

	// メモリ開放
	SAFE_DELETE(prScene);
}

//============================================================
//	ステージ取得処理
//============================================================
CStage *CScene::GetStage(void)
{
	// インスタンス未使用
	assert(m_pStage != nullptr);

	// ステージのポインタを返す
	return m_pStage;
}

//============================================================
//	プレイヤー取得処理
//============================================================
CPlayer *CScene::GetPlayer(void)
{
	CListManager<CPlayer> *pListManager = CPlayer::GetList();	// プレイヤーリストマネージャー
	if (pListManager == nullptr)		 { return nullptr; }	// リスト未使用の場合抜ける
	if (pListManager->GetNumAll() != 1)	 { return nullptr; }	// プレイヤーが1人ではない場合抜ける
	CPlayer *pPlayer = pListManager->GetList().front();			// プレイヤーの情報

	// プレイヤーのポインタを返す
	return pPlayer;
}

//============================================================
//	ボス取得処理
//============================================================
CEnemy *CScene::GetBoss(void)
{
	CListManager<CEnemy> *pListManager = CEnemy::GetList();	// 敵リストマネージャー
	if (pListManager == nullptr) { return nullptr; }		// リスト未使用

	std::list<CEnemy*> listEnemy = pListManager->GetList();	// 敵リスト
	for (CEnemy *pEnemy : listEnemy)
	{ // 要素数分繰り返す

		if (pEnemy->GetType() == CEnemy::TYPE_BOSS_DRAGON)
		{ // 敵がボスの場合

			// 敵のポインタを返す
			return pEnemy;
		}
	}

	return nullptr;
}

//============================================================
//	モードの設定処理
//============================================================
void CScene::SetMode(const EMode mode)
{
	// 引数のモードを設定
	m_mode = mode;
}

//============================================================
//	モード取得処理
//============================================================
CScene::EMode CScene::GetMode(void) const
{
	// 現在のモードを返す
	return m_mode;
}
