//============================================================
//
//	マネージャー処理 [manager.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "manager.h"
#include "object.h"
#include "time.h"
#include "renderer.h"
#include "sound.h"
#include "camera.h"
#include "light.h"
#include "fade.h"
#include "texture.h"
#include "model.h"
#include "retentionManager.h"
#include "debug.h"

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CManager *CManager::m_pManager = nullptr;	// マネージャーオブジェクト

//************************************************************
//	親クラス [CManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CManager::CManager() :
	m_pTime			(nullptr),	// タイムインスタンス
	m_pRenderer		(nullptr),	// レンダラーインスタンス
	m_pKeyboard		(nullptr),	// キーボードインスタンス
	m_pMouse		(nullptr),	// マウスインスタンス
	m_pPad			(nullptr),	// パッドインスタンス
	m_pSound		(nullptr),	// サウンドインスタンス
	m_pCamera		(nullptr),	// カメラインスタンス
	m_pLight		(nullptr),	// ライトインスタンス
	m_pTexture		(nullptr),	// テクスチャインスタンス
	m_pModel		(nullptr),	// モデルインスタンス
	m_pFade			(nullptr),	// フェードインスタンス
	m_pScene		(nullptr),	// シーンインスタンス
	m_pRetention	(nullptr),	// データ保存マネージャー
	m_pDebugProc	(nullptr),	// デバッグ表示
	m_pDebug		(nullptr)	// デバッグ

{

}

//============================================================
//	デストラクタ
//============================================================
CManager::~CManager()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	//--------------------------------------------------------
	//	メンバ変数を初期化
	//--------------------------------------------------------
	m_pTime			= nullptr;	// タイムインスタンス
	m_pRenderer		= nullptr;	// レンダラーインスタンス
	m_pKeyboard		= nullptr;	// キーボードインスタンス
	m_pMouse		= nullptr;	// マウスインスタンス
	m_pPad			= nullptr;	// パッドインスタンス
	m_pSound		= nullptr;	// サウンドインスタンス
	m_pCamera		= nullptr;	// カメラインスタンス
	m_pLight		= nullptr;	// ライトインスタンス
	m_pTexture		= nullptr;	// テクスチャインスタンス
	m_pModel		= nullptr;	// モデルインスタンス
	m_pFade			= nullptr;	// フェードインスタンス
	m_pScene		= nullptr;	// シーンインスタンス
	m_pRetention	= nullptr;	// データ保存マネージャー
	m_pDebugProc	= nullptr;	// デバッグ表示
	m_pDebug		= nullptr;	// デバッグ

	//--------------------------------------------------------
	//	システムの生成
	//--------------------------------------------------------
	// タイムの生成
	m_pTime = CTime::Create();
	if (m_pTime == nullptr)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// レンダラーの生成
	m_pRenderer = CRenderer::Create(hWnd, bWindow);
	if (m_pRenderer == nullptr)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// キーボードの生成
	m_pKeyboard = CInputKeyboard::Create(hInstance, hWnd);
	if (m_pKeyboard == nullptr)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// マウスの生成
	m_pMouse = CInputMouse::Create(hInstance, hWnd);
	if (m_pMouse == nullptr)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// パッドの生成
	m_pPad = CInputPad::Create();
	if (m_pPad == nullptr)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// サウンドの生成
	m_pSound = CSound::Create(hWnd);
	if (m_pSound == nullptr)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// カメラの生成
	m_pCamera = CCamera::Create();
	if (m_pCamera == nullptr)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// ライトの生成
	m_pLight = CLight::Create();
	if (m_pLight == nullptr)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// データ保存マネージャーの生成
	m_pRetention = CRetentionManager::Create();
	if (m_pRetention == nullptr)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	//--------------------------------------------------------
	//	情報の読込・設定
	//--------------------------------------------------------
	// テクスチャの生成・読込
	m_pTexture = CTexture::Create();
	if (m_pTexture == nullptr)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// モデルの生成・読込
	m_pModel = CModel::Create();
	if (m_pModel == nullptr)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// レンダーテクスチャーの生成
	if (FAILED(m_pRenderer->CreateRenderTexture()))
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// フェードの生成・シーンの設定
	m_pFade = CFade::Create();
	if (m_pFade == nullptr)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	//--------------------------------------------------------
	//	デバッグ用
	//--------------------------------------------------------
	// デバッグ表示の生成
	m_pDebugProc = CDebugProc::Create();
	if (m_pDebugProc == nullptr)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// デバッグの生成
	m_pDebug = CDebug::Create();
	if (m_pDebug == nullptr)
	{ // 非使用中の場合

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
void CManager::Uninit(void)
{
	//--------------------------------------------------------
	//	デバッグ用
	//--------------------------------------------------------
	// デバッグ表示の破棄
	SAFE_REF_RELEASE(m_pDebugProc);

	// デバッグの破棄
	SAFE_REF_RELEASE(m_pDebug);

	//--------------------------------------------------------
	//	システムの破棄
	//--------------------------------------------------------
	// データ保存マネージャーの破棄
	SAFE_REF_RELEASE(m_pRetention);

	// シーンの破棄
	SAFE_REF_RELEASE(m_pScene);

	// フェードの破棄
	SAFE_REF_RELEASE(m_pFade);

	// ライトの破棄
	SAFE_REF_RELEASE(m_pLight);

	// カメラの破棄
	SAFE_REF_RELEASE(m_pCamera);

	// サウンドの破棄
	SAFE_REF_RELEASE(m_pSound);

	// パッドの破棄
	SAFE_REF_RELEASE(m_pPad);

	// マウスの破棄
	SAFE_REF_RELEASE(m_pMouse);

	// キーボードの破棄
	SAFE_REF_RELEASE(m_pKeyboard);

	// タイムの破棄
	SAFE_REF_RELEASE(m_pTime);

	//--------------------------------------------------------
	//	情報の破棄
	//--------------------------------------------------------
	// テクスチャの破棄
	SAFE_REF_RELEASE(m_pTexture);

	// モデルの破棄
	SAFE_REF_RELEASE(m_pModel);

	// レンダラーの破棄
	SAFE_REF_RELEASE(m_pRenderer);

	// オブジェクトの全破棄
	CObject::ReleaseAll();

	// 例外処理
	assert(CObject::GetNumAll() == 0);	// 破棄の失敗
}

//============================================================
//	更新処理
//============================================================
void CManager::Update(void)
{
	// デバッグ情報の更新・描画
	assert(m_pDebug != nullptr);
	m_pDebug->UpdateDebugControl();
	m_pDebug->DrawDebugControl();
	m_pDebug->DrawDebugData();

	// タイムの更新
	assert(m_pTime != nullptr);
	m_pTime->Update();

	// パッドの更新
	assert(m_pPad != nullptr);
	m_pPad->Update();

	// マウスの更新
	assert(m_pMouse != nullptr);
	m_pMouse->Update();

	// キーボードの更新
	assert(m_pKeyboard != nullptr);
	m_pKeyboard->Update();

	// フェードの更新
	assert(m_pFade != nullptr);
	m_pFade->Update();

	// シーンの更新
	assert(m_pScene != nullptr);
	m_pScene->Update();

	// デバッグ表示の更新
	assert(m_pDebugProc != nullptr);
	m_pDebugProc->Update();
}

//============================================================
//	描画処理
//============================================================
void CManager::Draw(void)
{
	// レンダラーの描画
	assert(m_pRenderer != nullptr);
	m_pRenderer->Draw();
}

//============================================================
//	生成処理
//============================================================
CManager *CManager::Create(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	// インスタンス使用中
	assert(m_pManager == nullptr);

	// マネージャーの生成
	m_pManager = new CManager;
	if (m_pManager == nullptr)
	{ // 生成に失敗した場合

		assert(false);
		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// マネージャーの初期化
		if (FAILED(m_pManager->Init(hInstance, hWnd, bWindow)))
		{ // 初期化に失敗した場合

			// マネージャーの破棄
			SAFE_DELETE(m_pManager);
			return nullptr;
		}

		// 確保したアドレスを返す
		return m_pManager;
	}
}

//============================================================
//	取得処理
//============================================================
CManager *CManager::GetInstance(void)
{
	// インスタンス未使用
	assert(m_pManager != nullptr);

	// マネージャーのポインタを返す
	return m_pManager;
}

//============================================================
//	破棄処理
//============================================================
void CManager::Release(CManager *&prManager)
{
	// マネージャーの終了
	assert(prManager != nullptr);
	prManager->Uninit();

	// メモリ開放
	SAFE_DELETE(prManager);
}

//============================================================
//	シーンの設定処理
//============================================================
void CManager::SetScene(const CScene::EMode mode, const int nWait)
{
	// インスタンス未使用
	assert(m_pFade != nullptr);

	// 次のシーンを設定
	m_pFade->Set(mode, nWait);
}

//============================================================
//	モードの設定処理
//============================================================
HRESULT CManager::SetMode(const CScene::EMode mode)
{
	// サウンドを停止
	assert(m_pSound != nullptr);
	m_pSound->Stop();

	// シーンを破棄
	SAFE_REF_RELEASE(m_pScene);

	// オブジェクトの全破棄
	CObject::ReleaseAll();

	// シーンの生成
	assert(m_pScene == nullptr);
	m_pScene = CScene::Create(mode);
	if (m_pScene == nullptr)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	モードの取得処理
//============================================================
CScene::EMode CManager::GetMode(void)
{
	// インスタンス未使用
	assert(m_pScene != nullptr);

	// 現在のモードを返す
	return m_pScene->GetMode();
}

//============================================================
//	タイム取得処理
//============================================================
CTime *CManager::GetTime(void)
{
	// インスタンス未使用
	assert(m_pTime != nullptr);

	// タイムのポインタを返す
	return m_pTime;
}

//============================================================
//	レンダラー取得処理
//============================================================
CRenderer *CManager::GetRenderer(void)
{
	// インスタンス未使用
	assert(m_pRenderer != nullptr);

	// レンダラーのポインタを返す
	return m_pRenderer;
}

//============================================================
//	キーボード取得処理
//============================================================
CInputKeyboard *CManager::GetKeyboard(void)
{
	// インスタンス未使用
	assert(m_pKeyboard != nullptr);

	// キーボードのポインタを返す
	return m_pKeyboard;
}

//============================================================
//	マウス取得処理
//============================================================
CInputMouse *CManager::GetMouse(void)
{
	// インスタンス未使用
	assert(m_pMouse != nullptr);

	// マウスのポインタを返す
	return m_pMouse;
}

//============================================================
//	パッド取得処理
//============================================================
CInputPad *CManager::GetPad(void)
{
	// インスタンス未使用
	assert(m_pPad != nullptr);

	// パッドのポインタを返す
	return m_pPad;
}

//============================================================
//	サウンド取得処理
//============================================================
CSound *CManager::GetSound(void)
{
	// インスタンス未使用
	assert(m_pSound != nullptr);

	// サウンドのポインタを返す
	return m_pSound;
}

//============================================================
//	カメラ取得処理
//============================================================
CCamera *CManager::GetCamera(void)
{
	// インスタンス未使用
	assert(m_pCamera != nullptr);

	// カメラのポインタを返す
	return m_pCamera;
}

//============================================================
//	ライト取得処理
//============================================================
CLight *CManager::GetLight(void)
{
	// インスタンス未使用
	assert(m_pLight != nullptr);

	// ライトのポインタを返す
	return m_pLight;
}

//============================================================
//	テクスチャ取得処理
//============================================================
CTexture *CManager::GetTexture(void)
{
	// インスタンス未使用
	assert(m_pTexture != nullptr);

	// テクスチャのポインタを返す
	return m_pTexture;
}

//============================================================
//	モデル取得処理
//============================================================
CModel *CManager::GetModel(void)
{
	// インスタンス未使用
	assert(m_pModel != nullptr);

	// モデルのポインタを返す
	return m_pModel;
}

//============================================================
//	フェード取得処理
//============================================================
CFade *CManager::GetFade(void)
{
	// インスタンス未使用
	assert(m_pFade != nullptr);

	// フェードのポインタを返す
	return m_pFade;
}

//============================================================
//	シーン取得処理
//============================================================
CScene *CManager::GetScene(void)
{
	// インスタンス未使用
	assert(m_pScene != nullptr);

	// シーンのポインタを返す
	return m_pScene;
}

//============================================================
//	データ保存マネージャー取得処理
//============================================================
CRetentionManager *CManager::GetRetention(void)
{
	// インスタンス未使用
	assert(m_pRetention != nullptr);

	// データ保存マネージャーを返す
	return m_pRetention;
}

//============================================================
//	デバッグ表示取得処理
//============================================================
CDebugProc *CManager::GetDebugProc(void)
{
	// インスタンス未使用
	assert(m_pDebugProc != nullptr);

	// デバッグ表示のポインタを返す
	return m_pDebugProc;
}

//============================================================
//	デバッグ取得処理
//============================================================
CDebug *CManager::GetDebug(void)
{
	// インスタンス未使用
	assert(m_pDebug != nullptr);

	// デバッグのポインタを返す
	return m_pDebug;
}
