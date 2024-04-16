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

// TODO
#include "manager.h"
#include "font.h"
#include "fontChar.h"
#include "font2D.h"

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
	UINT aChar[] =
	{
		 L'H',
		 L'a',
		 L'l',
		 L'l',
		 L'o',
		 L'W',
		 L'o',
		 L'r',
		 L'l',
		 L'd',
		 L'！',
		 L'ま',
		 L'み',
		 L'む',
		 L'め',
		 L'も',
		 L'|',
		 L'こ',
		 L'ん',
		 L'に',
		 L'ち',
		 L'は',
		 L'！',
		 L'冠',
		 L'婚',
		 L'葬',
		 L'祭',
		 L'？',
	};
	CFont *pFont = GET_MANAGER->GetFont();

	for (int nHeight = 0; nHeight < 10; nHeight++)
	{
		float fPosX = 50.0f;
		int nWMax = 26;
		//int nWMax = NUM_ARRAY(aChar);
		for (int nWidth = 0; nWidth < nWMax; nWidth++)
		{
			//CFontChar *pFontChar = pFont->RegistName("ＭＳ Ｐ明朝").pFontChar;
			//CFontChar *pFontChar = pFont->RegistName("ＭＳ Ｐゴシック").pFontChar;
			//CFontChar *pFontChar = pFont->RegistName("わんぱくルイカ").pFontChar;
			CFontChar *pFontChar = pFont->RegistName("零ゴシック").pFontChar;
			//CFontChar *pFontChar = pFont->RegistName("JFドットK14-2004").pFontChar;
			//CFontChar *pFontChar = pFont->RegistName("BIZ UDPゴシック").pFontChar;
			//CFontChar *pFontChar = pFont->RegistName("HGP創英角ﾎﾟｯﾌﾟ体").pFontChar;
			UINT uChar = L'a' + nWidth + (nHeight * nWMax);
			//UINT uChar = L'あ' + nWidth + (nHeight * nWMax);
			//UINT uChar = aChar[nWidth + (nHeight * nWMax)];
			//D3DXVECTOR3 pos = D3DXVECTOR3(40.0f + (nWidth * 60.0f), 50.0f + (nHeight * 100.0f), 0.0f);
			D3DXVECTOR3 pos = D3DXVECTOR3(fPosX, 50.0f + (nHeight * 60.0f), 0.0f);

#if 0
			// フォント2Dの生成
			CFont2D *p = CFont2D::Create
			( // 引数
				pFontChar,
				uChar,
				pos,
				1.0f,
				VEC3_ZERO,
				XCOL_CYAN
			);
			if (p == nullptr)
			{ // 生成に失敗した場合

				// 失敗を返す
				assert(false);
				return E_FAIL;
			}

			D3DXVECTOR3 oold = p->GetVec3Position();
			oold.x += p->GetVec3Sizing().x * 0.5f + 1.0f;
			p->SetVec3Position(oold);

			// テクスチャを剝がす
			p->BindTexture(NONE_IDX);

			// 優先順位の設定
			p->SetPriority(7);

			// ラベルの設定
			p->SetLabel(CObject::LABEL_EFFECT);
#endif

			// フォント2Dの生成
			CFont2D *ppsx = CFont2D::Create
			( // 引数
				pFontChar,
				uChar,
				pos,
				1.0f,
				VEC3_ZERO,
				XCOL_WHITE
			);
			if (ppsx == nullptr)
			{ // 生成に失敗した場合

				// 失敗を返す
				assert(false);
				return E_FAIL;
			}

			D3DXVECTOR3 old = ppsx->GetVec3Position();
			old.x += ppsx->GetVec3Sizing().x * 0.5f + 1.0f;
			ppsx->SetVec3Position(old);

			// 優先順位の設定
			ppsx->SetPriority(7);

			// ラベルの設定
			ppsx->SetLabel(CObject::LABEL_EFFECT);

			// 次の位置設定用に原点を保存
			fPosX = ppsx->GetVec3Position().x - (ppsx->GetVec3Sizing().x * 0.5f + 1.0f) + ppsx->GetNext();
		}
	}
#else
	for (int nHeight = 0; nHeight < 1; nHeight++)
	{
		for (int nWidth = 0; nWidth < 64; nWidth++)
		{
			float REV_SCALE = 1.0f;

			CFont *pFont = GET_MANAGER->GetFont();
			UINT uChar = L'a' + nWidth + (nHeight * 1);
			CFontChar::SChar infoChar = pFont->RegistChar(uChar, "ＭＳ Ｐ明朝");
			float fNextSpace = ((float)infoChar.glyph.gmptGlyphOrigin.x * REV_SCALE);

			// フォント2Dの生成
			CFont2D *p = CFont2D::Create
			( // 引数
				pFont->RegistName("ＭＳ Ｐ明朝").pFontChar,
				uChar,
				D3DXVECTOR3(50.0f + (nWidth * 50.0f), 200.0f + (nHeight * 200.0f), 0.0f)
			);
			if (p == nullptr)
			{ // 生成に失敗した場合

				// 失敗を返す
				assert(false);
				return E_FAIL;
			}

			//D3DXVECTOR3 pos = p->GetVec3Position();
			////pos.x += fabsf(p->GetVec3Sizing().x - fSizeGap);
			//pos.x -= ((float)infoChar.glyph.gmptGlyphOrigin.x * REV_SCALE);
			//pos.y += ((float)(infoChar.text.tmHeight - infoChar.text.tmDescent - infoChar.glyph.gmptGlyphOrigin.y)) * REV_SCALE * 0.5f;
			//p->SetVec3Position(pos);

			// 優先順位の設定
			p->SetPriority(7);

			// ラベルの設定
			p->SetLabel(CObject::LABEL_EFFECT);

			//fSizeGap = p->GetVec3Sizing().x;

			//// 次の位置までの空白量を加算
			//fNext = (p->GetVec3Position().x/* - p->GetVec3Sizing().x * 0.5f*/ + p->GetNext() * REV_SCALE);
		}
	}
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
