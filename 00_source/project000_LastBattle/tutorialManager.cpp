//============================================================
//
//	チュートリアルマネージャー処理 [tutorialManager.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "tutorialManager.h"
#include "manager.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "texture.h"
#include "gauge2D.h"
#include "object2D.h"
#include "player.h"
#include "enemy.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char *TEXTURE_FILE[] =	// テクスチャファイル
	{
		"data\\TEXTURE\\tutorial000.png",	// 操作方法表示のテクスチャ
	};
	const char *LESSON_TEX_FILE[] =	// レッスンテクスチャファイル
	{
		nullptr,						// レッスン00：テクスチャなし
		"data\\TEXTURE\\lesson000.png",	// レッスン01：移動・カメラ	説明テクスチャ
		"data\\TEXTURE\\lesson001.png",	// レッスン02：ジャンプ		説明テクスチャ
		"data\\TEXTURE\\lesson002.png",	// レッスン03：回避			説明テクスチャ
		"data\\TEXTURE\\lesson003.png",	// レッスン04：攻撃			説明テクスチャ
	};
	const char *GUIDE_TEX_FILE[] =	// 小説明テクスチャファイル
	{
		nullptr,						// レッスン00：テクスチャなし
		"data\\TEXTURE\\guide000.png",	// レッスン01：移動・カメラ	小説明のテクスチャ
		"data\\TEXTURE\\guide001.png",	// レッスン02：ジャンプ		小説明のテクスチャ
		"data\\TEXTURE\\guide002.png",	// レッスン03：回避			小説明のテクスチャ
		"data\\TEXTURE\\guide003.png",	// レッスン04：攻撃			小説明のテクスチャ
	};
	const char *CONT_TEX_FILE[] =	// 操作方法テクスチャファイル
	{
		nullptr,							// レッスン00：テクスチャなし
		nullptr,							// レッスン01：テクスチャなし
		"data\\TEXTURE\\control000.png",	// レッスン02：ジャンプ	操作方法のテクスチャ
		"data\\TEXTURE\\control001.png",	// レッスン03：回避		操作方法のテクスチャ
		"data\\TEXTURE\\control002.png",	// レッスン04：攻撃		操作方法のテクスチャ
	};
	const int NEXT_LESSON[] =		// レッスン移行カウント
	{
		0,		// レッスンなし
		240,	// レッスン01：移動・カメラ	終了カウント
		240,	// レッスン02：ジャンプ		終了カウント
		240,	// レッスン03：回避			終了カウント
		240,	// レッスン04：攻撃			終了カウント
	};
	const int NEXT_LESSON_WAIT[] =	// 次レッスン余韻カウント
	{
		0,		// レッスンなし
		90,		// レッスン01：移動・カメラ	終了時の余韻カウント
		90,		// レッスン02：ジャンプ		終了時の余韻カウント
		90,		// レッスン03：回避			終了時の余韻カウント
		90,		// レッスン04：攻撃			終了時の余韻カウント
	};

	const int PRIORITY			= 6;	// チュートリアルの優先順位
	const int TITLE_WAIT_CNT	= 30;	// タイトル遷移の余韻フレーム

	const D3DXVECTOR3 POS_GUIDE		= D3DXVECTOR3(1020.0f, 585.0f, 0.0f);	// 小説明表示の位置
	const D3DXVECTOR3 POS_CONTROL	= D3DXVECTOR3(260.0f, 585.0f, 0.0f);	// 操作方法表示の位置

	const D3DXVECTOR3 SIZE_PROGRESSION_UI	= D3DXVECTOR3(500.0f, 250.0f, 0.0f);	// チュートリアル進行時のUI表示の大きさ
	const D3DXCOLOR COL_PROGRESSION_UI_BG	= D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f);	// チュートリアル進行時のUI表示の背景色

	const int FRAME_COUNTER = 10;	// ゲージの変動フレーム
	const D3DXVECTOR3 SIZE_COUNTER	= D3DXVECTOR3(SIZE_PROGRESSION_UI.x * 0.5f, 8.0f, 0.0f);	// ゲージの大きさ
	const D3DXVECTOR3 POS_COUNTER	= D3DXVECTOR3(POS_GUIDE.x, POS_GUIDE.y - (SIZE_PROGRESSION_UI.y * 0.5f) - SIZE_COUNTER.y, 0.0f);	// ゲージの位置
	const D3DXCOLOR COL_FRONT_COUNTER	= XCOL_WHITE;	// ゲージの表ゲージ色
	const D3DXCOLOR COL_BACK_COUNTER	= XCOL_AWHITE;	// ゲージの裏ゲージ色

	const D3DXCOLOR COL_FADE_MIN	= D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// フェードのα値の最小
	const D3DXCOLOR COL_FADE_MAX	= D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.8f);	// フェードのα値の最大
	const float FADE_LEVEL = 0.025f;	// フェードのα値の加減量

	const D3DXVECTOR3 POS_EXPLAIN	= D3DXVECTOR3(SCREEN_CENT.x, 315.0f, 0.0f);	// 説明表示の位置
	const D3DXVECTOR3 SIZE_EXPLAIN	= D3DXVECTOR3(1000.0f, 597.5f, 0.0f);		// 説明表示の大きさ
	const float SET_SCALE = 0.05f;	// 拡大率の初期値
	const float ADD_SCALE = 0.1f;	// 拡大率の加算量

	const D3DXVECTOR3 POS_CLOSE		= D3DXVECTOR3(SCREEN_CENT.x, 665.0f, 0.0f);	// 操作方法表示の位置
	const D3DXVECTOR3 SIZE_CLOSE	= D3DXVECTOR3(SCREEN_WIDTH, 100.0f, 0.0f);	// 操作方法表示の大きさ
	const float ADD_ROT	= 0.02f;	// 点滅向きの加算量
}

//************************************************************
//	スタティックアサート
//************************************************************
static_assert(NUM_ARRAY(TEXTURE_FILE)		== CTutorialManager::TEXTURE_MAX,	"ERROR : Texture Count Mismatch");
static_assert(NUM_ARRAY(LESSON_TEX_FILE)	== CTutorialManager::LESSON_MAX,	"ERROR : Lesson Count Mismatch");
static_assert(NUM_ARRAY(GUIDE_TEX_FILE)		== CTutorialManager::LESSON_MAX,	"ERROR : Lesson Count Mismatch");
static_assert(NUM_ARRAY(CONT_TEX_FILE)		== CTutorialManager::LESSON_MAX,	"ERROR : Lesson Count Mismatch");
static_assert(NUM_ARRAY(NEXT_LESSON)		== CTutorialManager::LESSON_MAX,	"ERROR : Lesson Count Mismatch");
static_assert(NUM_ARRAY(NEXT_LESSON_WAIT)	== CTutorialManager::LESSON_MAX,	"ERROR : Lesson Count Mismatch");

//************************************************************
//	親クラス [CTutorialManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CTutorialManager::CTutorialManager() :
	m_pCounterLesson	(nullptr),		// レッスン管理カウンターの情報
	m_pFade				(nullptr),		// フェードの情報
	m_pExplain			(nullptr),		// 説明表示の情報
	m_pClose			(nullptr),		// 説明終了表示の情報
	m_pGuide			(nullptr),		// 小説明表示の情報
	m_pGuideBG			(nullptr),		// 小説明表示の背景情報
	m_pControl			(nullptr),		// 操作方法表示の情報
	m_pControlBG		(nullptr),		// 操作方法表示の背景情報
	m_state				(STATE_NONE),	// 状態
	m_nLesson			(0),			// レッスン
	m_fScale			(0.0f),			// ポリゴン拡大率
	m_fSinRot			(0.0f),			// ポリゴン点滅向き
	m_nCounterState		(0)				// 状態管理カウンター
{

}

//============================================================
//	デストラクタ
//============================================================
CTutorialManager::~CTutorialManager()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CTutorialManager::Init(void)
{
	// メンバ変数を初期化
	m_pCounterLesson	= nullptr;		// レッスン管理カウンターの情報
	m_pFade				= nullptr;		// フェードの情報
	m_pExplain			= nullptr;		// 説明表示の情報
	m_pClose			= nullptr;		// 説明終了表示の情報
	m_pGuide			= nullptr;		// 小説明表示の情報
	m_pGuideBG			= nullptr;		// 小説明表示の背景情報
	m_pControl			= nullptr;		// 操作方法表示の情報
	m_pControlBG		= nullptr;		// 操作方法表示の背景情報
	m_state				= STATE_FADEIN;	// 状態
	m_nLesson			= LESSON_NONE;	// レッスン
	m_fScale			= 0.0f;			// ポリゴン拡大率
	m_fSinRot			= -HALF_PI;		// ポリゴン点滅向き
	m_nCounterState		= 0;			// 状態管理カウンター

	//--------------------------------------------------------
	//	レッスン管理カウンターの生成・設定
	//--------------------------------------------------------
	// レッスン管理カウンターの生成
	m_pCounterLesson = CGauge2D::Create
	( // 引数
		NEXT_LESSON[0],		// 最大表示値
		FRAME_COUNTER,		// 表示値変動フレーム
		POS_COUNTER,		// 位置
		SIZE_COUNTER,		// ゲージ大きさ
		COL_FRONT_COUNTER,	// 表ゲージ色
		COL_BACK_COUNTER	// 裏ゲージ色
	);
	if (m_pCounterLesson == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 優先順位を設定
	m_pCounterLesson->SetPriority(PRIORITY);

	//--------------------------------------------------------
	//	小説明表示の背景生成・設定
	//--------------------------------------------------------
	// 小説明表示の背景生成
	m_pGuideBG = CObject2D::Create
	( // 引数
		POS_GUIDE,				// 位置
		SIZE_PROGRESSION_UI,	// 大きさ
		VEC3_ZERO,				// 向き
		COL_PROGRESSION_UI_BG	// 色
	);
	if (m_pGuideBG == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 優先順位を設定
	m_pGuideBG->SetPriority(PRIORITY);

	//--------------------------------------------------------
	//	小説明表示の生成・設定
	//--------------------------------------------------------
	// 小説明表示の生成
	m_pGuide = CObject2D::Create
	( // 引数
		POS_GUIDE,			// 位置
		SIZE_PROGRESSION_UI	// 大きさ
	);
	if (m_pGuide == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 優先順位を設定
	m_pGuide->SetPriority(PRIORITY);

	//--------------------------------------------------------
	//	操作方法表示の背景生成・設定
	//--------------------------------------------------------
	// 操作方法表示の背景生成
	m_pControlBG = CObject2D::Create
	( // 引数
		POS_CONTROL,			// 位置
		SIZE_PROGRESSION_UI,	// 大きさ
		VEC3_ZERO,				// 向き
		COL_PROGRESSION_UI_BG	// 色
	);
	if (m_pControlBG == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 優先順位を設定
	m_pControlBG->SetPriority(PRIORITY);

	//--------------------------------------------------------
	//	操作方法表示の生成・設定
	//--------------------------------------------------------
	// 操作方法表示の生成
	m_pControl = CObject2D::Create
	( // 引数
		POS_CONTROL,		// 位置
		SIZE_PROGRESSION_UI	// 大きさ
	);
	if (m_pControl == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 優先順位を設定
	m_pControl->SetPriority(PRIORITY);

	//--------------------------------------------------------
	//	フェードの生成・設定
	//--------------------------------------------------------
	// フェードの生成
	m_pFade = CObject2D::Create
	( // 引数
		SCREEN_CENT,	// 位置
		SCREEN_SIZE,	// 大きさ
		VEC3_ZERO,		// 向き
		COL_FADE_MIN	// 色
	);
	if (m_pFade == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 優先順位を設定
	m_pFade->SetPriority(PRIORITY);

	//--------------------------------------------------------
	//	選択背景の生成・設定
	//--------------------------------------------------------
	// 選択背景の生成
	m_pExplain = CObject2D::Create
	( // 引数
		POS_EXPLAIN,	// 位置
		SIZE_EXPLAIN * SET_SCALE	// 大きさ
	);
	if (m_pExplain == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 優先順位を設定
	m_pExplain->SetPriority(PRIORITY);

	// 描画を停止
	m_pExplain->SetEnableDraw(false);

	//--------------------------------------------------------
	//	説明終了表示の生成・設定
	//--------------------------------------------------------
	// 説明終了表示の生成
	m_pClose = CObject2D::Create
	( // 引数
		POS_CLOSE,	// 位置
		SIZE_CLOSE,	// 大きさ
		VEC3_ZERO,	// 向き
		XCOL_AWHITE	// 色
	);
	if (m_pClose == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// テクスチャを登録・割当
	m_pClose->BindTexture(TEXTURE_FILE[TEXTURE_CONTROL]);

	// 優先順位を設定
	m_pClose->SetPriority(PRIORITY);

	// 進行状態時に表示するUIの描画を停止
	SetEnableProgressionDraw(false);

	// 次レッスンへの移行
	NextLesson();

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CTutorialManager::Uninit(void)
{
	// 小説明表示の背景終了
	SAFE_UNINIT(m_pGuideBG);

	// 小説明表示の終了
	SAFE_UNINIT(m_pGuide);

	// 操作方法表示の背景終了
	SAFE_UNINIT(m_pControlBG);

	// 操作方法表示の終了
	SAFE_UNINIT(m_pControl);

	// フェードの終了
	SAFE_UNINIT(m_pFade);

	// 選択背景の終了
	SAFE_UNINIT(m_pExplain);

	// 説明終了の終了
	SAFE_UNINIT(m_pClose);
}

//============================================================
//	更新処理
//============================================================
void CTutorialManager::Update(void)
{
	switch (m_state)
	{ // 状態ごとの処理
	case STATE_NONE:		// 何もしない状態

		// 無し

		break;

	case STATE_FADEIN:		// フェードイン状態

		// フェードインの更新
		UpdateFadeIn();

		break;

	case STATE_EXPLAIN:		// 説明表示状態

		// 説明表示の更新
		UpdateExplain();

		break;

	case STATE_FADEWAIT:	// フェード待機状態

		// フェード待機の更新
		UpdateFadeWait();

		break;

	case STATE_FADEOUT:		// フェードアウト状態

		// フェードアウトの更新
		UpdateFadeOut();

		break;

	case STATE_PROGRESSION:	// 進行状態

		// 無し

		break;

	case STATE_NEXTWAIT:	// 次レッスン待機状態

		// 次レッスン待機の更新
		UpdateNextWait();

		break;

	case STATE_END:			// 終了状態

		// シーンの設定
		GET_MANAGER->SetScene(CScene::MODE_TITLE, TITLE_WAIT_CNT);	// タイトル画面

		break;

	default:	// 例外処理
		assert(false);
		break;
	}

	// 小説明表示の背景更新
	m_pGuideBG->Update();

	// 小説明表示の更新
	m_pGuide->Update();

	// 操作方法表示の背景更新
	m_pControlBG->Update();

	// 操作方法表示の更新
	m_pControl->Update();

	// フェードの更新
	m_pFade->Update();

	// 選択背景の更新
	m_pExplain->Update();

	// 説明終了の更新
	m_pClose->Update();
}

//============================================================
//	レッスンカウンターの加算処理
//============================================================
void CTutorialManager::AddLessonCounter(void)
{
	if (m_nLesson < LESSON_MAX && m_state != STATE_NEXTWAIT)
	{ // レッスンがまだある且つ、次レッスンの待機中ではない場合

		// レッスンカウンターを加算
		m_pCounterLesson->AddNum(1);

		if (m_pCounterLesson->GetNum() >= NEXT_LESSON[m_nLesson])
		{ // レッスンを次に進めるカウントまで到達した場合

			// 状態を設定
			m_state = STATE_NEXTWAIT;	// 次レッスン待機状態

			// 決定音01の再生
			PLAY_SOUND(CSound::LABEL_SE_DECISION_001);
		}
	}
}

//============================================================
//	レッスン取得処理
//============================================================
int CTutorialManager::GetLesson(void) const
{
	// 現在のレッスンを返す
	return m_nLesson;
}

//============================================================
//	状態取得処理
//============================================================
CTutorialManager::EState CTutorialManager::GetState(void) const
{
	// 状態を返す
	return m_state;
}

//============================================================
//	生成処理
//============================================================
CTutorialManager *CTutorialManager::Create(void)
{
	// チュートリアルマネージャーの生成
	CTutorialManager *pTutorialManager = new CTutorialManager;
	if (pTutorialManager == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// チュートリアルマネージャーの初期化
		if (FAILED(pTutorialManager->Init()))
		{ // 初期化に失敗した場合

			// チュートリアルマネージャーの破棄
			SAFE_DELETE(pTutorialManager);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pTutorialManager;
	}
}

//============================================================
//	破棄処理
//============================================================
void CTutorialManager::Release(CTutorialManager *&prTutorialManager)
{
	// チュートリアルマネージャーの終了
	assert(prTutorialManager != nullptr);
	prTutorialManager->Uninit();

	// メモリ開放
	SAFE_DELETE(prTutorialManager);
}

//============================================================
//	レッスン移行カウント取得処理
//============================================================
int CTutorialManager::GetNextLessonCounter(const int nID)
{
	if (nID < LESSON_MAX)
	{ // インデックスが範囲内の場合

		// 引数のインデックスのレッスン移行カウントを返す
		return NEXT_LESSON[nID];
	}
	else { assert(false); return NONE_IDX; }	// 範囲外
}

//============================================================
//	進行状態時に表示するUIの描画状況の設定処理
//============================================================
void CTutorialManager::SetEnableProgressionDraw(const bool bDraw)
{
	//--------------------------------------------------------
	//	引数の描画状況を設定
	//--------------------------------------------------------
	// レッスン管理カウンターの描画状況を設定
	m_pCounterLesson->SetEnableDraw(bDraw);

	// 小説明表示の背景描画状況を設定
	m_pGuideBG->SetEnableDraw(bDraw);

	// 小説明表示の描画状況を設定
	m_pGuide->SetEnableDraw(bDraw);

	//--------------------------------------------------------
	//	描画状況を状況に応じて変更して設定
	//--------------------------------------------------------
	// 変数を宣言
	bool bDrawControl = bDraw;	// 操作方法の表示状況

	if (m_nLesson != LESSON_02	// レッスン02：前後加速
	&&  m_nLesson != LESSON_03	// レッスン03：左右加速
	&&  m_nLesson != LESSON_04)	// レッスン04：攻撃
	{ // 上記のレッスンのいずれでもない場合

		// 操作方法を表示しない状態にする
		bDrawControl = false;
	}

	// 操作方法表示の背景描画状況を設定
	m_pControlBG->SetEnableDraw(bDrawControl);

	// 操作方法表示の描画状況を設定
	m_pControl->SetEnableDraw(bDrawControl);
}

//============================================================
//	フェードインの更新処理
//============================================================
void CTutorialManager::UpdateFadeIn(void)
{
	// 変数を宣言
	D3DXCOLOR colFade = m_pFade->GetColor();	// フェードの色

	if (colFade.a < COL_FADE_MAX.a)
	{ // 透明量が設定値未満の場合

		// 透明度を加算
		colFade.a += FADE_LEVEL;
	}
	else
	{ // 透明量が設定値以上の場合

		// 透明度を補正
		colFade.a = COL_FADE_MAX.a;

		// 説明表示の描画を再開
		m_pExplain->SetEnableDraw(true);

		// 説明表示の初期ポリゴン拡大率を設定
		m_fScale = SET_SCALE;

		// 状態を設定
		m_state = STATE_EXPLAIN;	// 説明表示状態

		// 便箋めくり音の再生
		//PLAY_SOUND(CSound::LABEL_SE_PAPER);	// TODO
	}

	// 透明度を反映
	m_pFade->SetColor(colFade);
}

//============================================================
//	説明表示の更新処理
//============================================================
void CTutorialManager::UpdateExplain(void)
{
	if (m_fScale < 1.0f)
	{ // 拡大率が最小値未満の場合

		// 拡大率を加算
		m_fScale += ADD_SCALE;

		// 説明表示の大きさを設定
		m_pExplain->SetVec3Sizing(SIZE_EXPLAIN * m_fScale);
	}
	else
	{ // 拡大率が最小値以下の場合

		// 説明表示の大きさを設定
		m_pExplain->SetVec3Sizing(SIZE_EXPLAIN);

		// 状態を設定
		m_state = STATE_FADEWAIT;	// フェード待機状態
	}
}

//============================================================
//	フェード待機の更新処理
//============================================================
void CTutorialManager::UpdateFadeWait(void)
{
	// 変数を宣言
	D3DXCOLOR colControl = m_pClose->GetColor();	// 説明終了表示の色

	// ポインタを宣言
	CInputKeyboard	*pKeyboard	= GET_INPUTKEY;	// キーボード
	CInputPad		*pPad		= GET_INPUTPAD;	// パッド

	if (pKeyboard->IsTrigger(DIK_RETURN)
	||  pKeyboard->IsTrigger(DIK_SPACE)
	||  pPad->IsTrigger(CInputPad::KEY_A)
	||  pPad->IsTrigger(CInputPad::KEY_B)
	||  pPad->IsTrigger(CInputPad::KEY_X)
	||  pPad->IsTrigger(CInputPad::KEY_Y)
	||  pPad->IsTrigger(CInputPad::KEY_START))
	{
		// ポリゴン点滅向きを初期化
		m_fSinRot = -HALF_PI;

		// 説明表示の初期ポリゴン拡大率を設定
		m_fScale = SET_SCALE;

		// 説明表示の大きさを設定
		m_pExplain->SetVec3Sizing(SIZE_EXPLAIN * m_fScale);

		// 説明表示の描画を停止
		m_pExplain->SetEnableDraw(false);

		// 状態を設定
		m_state = STATE_FADEOUT;	// フェードアウト状態
	}

	// 説明終了表示の点滅向きを加算
	m_fSinRot += ADD_ROT;
	useful::NormalizeRot(m_fSinRot);	// 向きを補正

	// 説明終了表示のα値を変更
	colControl.a = (1.0f / 2.0f) * (sinf(m_fSinRot) + 1.0f);

	// 説明終了表示の色を反映
	m_pClose->SetColor(colControl);
}

//============================================================
//	フェードアウトの更新処理
//============================================================
void CTutorialManager::UpdateFadeOut(void)
{
	// 変数を宣言
	D3DXCOLOR colFade = m_pFade->GetColor();	// フェードの色

	if (colFade.a > COL_FADE_MIN.a)
	{ // 透明量が設定値より大きい場合

		// 透明度を減算
		colFade.a -= FADE_LEVEL;
	}
	else
	{ // 透明量が設定値以下の場合

		// 透明度を補正
		colFade.a = COL_FADE_MIN.a;


		// TODO：必要なら処理を考える
#if 0
		// プレイヤーを再出現させる
		CScene::GetPlayer()->SetRespawn(PLAY_SPAWN_POS);

		switch (m_nLesson)
		{ // レッスンごとの処理
		case LESSON_05:	// レッスン05：マナ回復

			// マナの回復をできるように変更
			CScene::GetPlayer()->SetEnableHealMana(true);

			// マナを空にする
			CScene::GetPlayer()->SetMana(0);

			break;

		case LESSON_06:	// レッスン06：敵への攻撃

			// 敵ランダム生成
			CEnemy::RandomSpawn(NEXT_LESSON[LESSON_06], CEnemy::TYPE_HUMAN);

			break;
		}
#endif

		// 進行状態時に表示するUIの描画を再開
		SetEnableProgressionDraw(true);

		// 状態を設定
		m_state = STATE_PROGRESSION;	// 進行状態
	}

	// 透明度を反映
	m_pFade->SetColor(colFade);
}

//============================================================
//	次レッスン待機の更新処理
//============================================================
void CTutorialManager::UpdateNextWait(void)
{
	if (m_nCounterState < NEXT_LESSON_WAIT[m_nLesson])
	{ // カウンターが余韻時間未満の場合

		// カウンターを加算
		m_nCounterState++;
	}
	else
	{ // 余韻時間が終了した場合

		// カウンターを初期化
		m_nCounterState = 0;

		// 次レッスンへの移行
		NextLesson();

		// 進行状態時に表示するUIの描画を停止
		SetEnableProgressionDraw(false);
	}
}

//============================================================
//	次レッスンへの移行処理
//============================================================
void CTutorialManager::NextLesson(void)
{
	//--------------------------------------------------------
	//	レッスン・カウンターを更新
	//--------------------------------------------------------
	// レッスンを次に進める
	m_nLesson++;

	// レッスンカウンターの最大値を設定
	m_pCounterLesson->SetMaxNum(NEXT_LESSON[m_nLesson]);

	// レッスンカウンターを初期化
	m_pCounterLesson->SetNum(0);

	//--------------------------------------------------------
	//	レッスン表示を更新
	//--------------------------------------------------------
	// レッスン説明テクスチャを登録・割当
	m_pExplain->BindTexture(LESSON_TEX_FILE[m_nLesson]);

	// レッスン小説明テクスチャを登録・割当
	m_pGuide->BindTexture(GUIDE_TEX_FILE[m_nLesson]);

	// レッスン操作方法テクスチャを登録・割当
	m_pControl->BindTexture(CONT_TEX_FILE[m_nLesson]);

	//--------------------------------------------------------
	//	状態を更新
	//--------------------------------------------------------
	if (m_nLesson < LESSON_MAX)
	{ // レッスンがまだある場合

		// 状態を設定
		m_state = STATE_FADEIN;	// フェードイン状態
	}
	else
	{ // レッスンがもうない場合

		// 状態を設定
		m_state = STATE_END;	// 終了状態
	}
}
