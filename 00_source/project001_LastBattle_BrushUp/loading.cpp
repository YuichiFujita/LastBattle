//============================================================
//
//	ローディング処理 [loading.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "loading.h"
#include "manager.h"
#include "object.h"
#include "anim2D.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int PRIORITY = 7;	// ローディングの優先順位
}

//************************************************************
//	親クラス [CLoading] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CLoading::CLoading() :
	m_pLoad		(nullptr),	// ロード画面情報
	m_stateLoad	(LOAD_NONE)	// ロード状態
{

}

//============================================================
//	デストラクタ
//============================================================
CLoading::~CLoading()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CLoading::Init(void)
{
	// メンバ変数を初期化
	m_pLoad = nullptr;			// ロード画面情報
	m_stateLoad = LOAD_NONE;	// ロード状態

	//// ロード画面の生成
	//m_pLoad = CAnim2D::Create
	//( // 引数
	//	20,
	//	3,
	//	SCREEN_CENT,	// 位置
	//	SCREEN_SIZE		// 大きさ
	//);
	//if (m_pLoad == nullptr)
	//{ // 生成に失敗した場合

	//	// 失敗を返す
	//	assert(false);
	//	return E_FAIL;
	//}

	//// 優先順位の設定
	//m_pLoad->SetPriority(PRIORITY);

	//// テクスチャを登録・割当
	//m_pLoad->BindTexture("data\\TEXTURE\\loading000.png");

	//// ラベルをエフェクトにする
	//m_pLoad->SetLabel(CObject::LABEL_EFFECT);

	//// 変更フレームを設定
	//m_pLoad->SetCounter(1);

#if 0
	// モードの設定
	GET_MANAGER->SetMode(m_modeNext);
#endif

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CLoading::Uninit(void)
{
	// ロード画面の終了
	SAFE_UNINIT(m_pLoad);
}

//============================================================
//	更新処理
//============================================================
void CLoading::Update(void)
{
	if (m_stateLoad != LOAD_NONE)
	{ // 何もしない状態ではない場合

		switch (m_stateLoad)
		{ // ロード状態ごとの処理
		case LOAD_UPDATE:	// ロード更新状態

			if (!m_funcInit.joinable())
			{ // 初期化処理が終了した場合

				// ロード画面を非表示
				//m_pLoad->SetEnableDraw(false);

				// ロード終了状態にする
				m_stateLoad = LOAD_END;
			}

			break;

		case LOAD_END:	// ロード終了状態

			m_stateLoad = LOAD_NONE;

			break;

		default:	// 例外処理
			assert(false);
			break;
		}

		// ロード画面の更新
		//m_pLoad->Update();
	}
}

//============================================================
//	ロード設定処理
//============================================================
void CLoading::Set(AFuncInit *pFuncInit)
{
	if (m_stateLoad == LOAD_NONE)
	{ // ローディングが行われていない場合

		//// 関数がない場合エラー
		//if (pFuncInit == nullptr) { assert(false);  return; }

		//// スレッドに引数の関数を設定
		//m_funcInit = std::thread(*pFuncInit);

		// ロード画面を表示
		//m_pLoad->SetEnableDraw(true);
		//m_pLoad->SetPattern(0);

		// ロード更新状態にする
		m_stateLoad = LOAD_UPDATE;
	}
}

//============================================================
//	ロード状態取得処理
//============================================================
CLoading::ELoading CLoading::GetState(void) const
{
	// 現在のロード状態を返す
	return m_stateLoad;
}

//============================================================
//	生成処理
//============================================================
CLoading *CLoading::Create(void)
{
	// ローディングの生成
	CLoading *pLoading = new CLoading;
	if (pLoading == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// ローディングの初期化
		if (FAILED(pLoading->Init()))
		{ // 初期化に失敗した場合

			// ローディングの破棄
			SAFE_DELETE(pLoading);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pLoading;
	}
}

//============================================================
//	破棄処理
//============================================================
void CLoading::Release(CLoading *&prLoading)
{
	// ローディングの終了
	assert(prLoading != nullptr);
	prLoading->Uninit();

	// メモリ開放
	SAFE_DELETE(prLoading);
}
