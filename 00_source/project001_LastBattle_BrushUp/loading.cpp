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
#include "renderer.h"
#include "object.h"
#include "anim2D.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int PRIORITY = 7;	// ローディングの優先順位

#ifdef _DEBUG

	const int MIN_LOAD = 0;	// 最低ロード繰り返し数

#else	// NDEBUG

	const int MIN_LOAD = 3;	// 最低ロード繰り返し数

#endif	// _DEBUG
}

//************************************************************
//	親クラス [CLoading] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CLoading::CLoading() :
	m_pLoad		(nullptr),	// ロード画面情報
	m_state	(LOAD_NONE)	// ロード状態
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
	m_state = LOAD_NONE;	// ロード状態

	// ロード画面の生成
	m_pLoad = CAnim2D::Create
	( // 引数
		12,
		4,
		SCREEN_CENT,	// 位置
		SCREEN_SIZE		// 大きさ
	);
	if (m_pLoad == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 優先順位の設定
	m_pLoad->SetPriority(PRIORITY);

	// テクスチャを登録・割当
	m_pLoad->BindTexture("data\\TEXTURE\\loading001.tga");

	// ラベルをエフェクトにする
	m_pLoad->SetLabel(CObject::LABEL_EFFECT);

	// 変更フレームを設定
	m_pLoad->SetCounter(2);

	// ラベルをスクリーンに設定
	m_pLoad->SetLabel(CObject::LABEL_NONE);

	// 自動更新・自動描画をOFFにする
	m_pLoad->SetEnableUpdate(false);
	m_pLoad->SetEnableDraw(false);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CLoading::Uninit(void)
{
	if (m_funcLoad.joinable())
	{
		m_funcLoad.join();
		//m_funcLoad.detach();	// TODO：デタッチしても処理は続く。故に破棄しても
	}

	// TODO：ほんとは使わない画像読み込みもあるからちゃんと後で消して！

	// ロード画面の終了
	SAFE_UNINIT(m_pLoad);
}

//============================================================
//	更新処理
//============================================================
void CLoading::Update(void)
{
	if (m_state != LOAD_NONE)
	{ // 何もしない状態ではない場合

		switch (m_state)
		{ // ロード状態ごとの処理
		case LOAD_UPDATE:	// ロード更新状態

			if (m_bEnd)
			{ // 初期化処理が終了した場合

				// ロードが終わった場合
				if (m_pLoad->GetPattern() != 0) { break; }
				if (m_pLoad->GetLoopAnimation() < MIN_LOAD) { break; }

				// ロード画面を非表示
				m_pLoad->SetEnableDraw(false);

				// ロード終了状態にする
				m_state = LOAD_END;
			}

			break;

		case LOAD_END:	// ロード終了状態

			m_state = LOAD_NONE;

			break;

		default:	// 例外処理
			assert(false);
			break;
		}

		if (!m_pLoad->IsUpdate())
		{ // 自動更新がONの場合

			// ロード画面の更新
			m_pLoad->Update();
		}
	}
}

//============================================================
//	描画処理
//============================================================
void CLoading::Draw(void)
{
	// 自動描画がOFFの場合抜ける
	if (!m_pLoad->IsDraw()) { return; }

	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// デバイスのポインタ

	// サンプラーステートを設定
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);	// U方向のラッピングを無効化
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);	// V方向のラッピングを無効化

	// ロード画面の描画
	m_pLoad->Draw();

	// サンプラーステートを設定
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// U方向のラッピングを有効化
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// V方向のラッピングを有効化
}

//============================================================
//	ロード開始設定処理
//============================================================
void CLoading::Set(std::function<HRESULT(bool*)> funcLoad)
{
	if (m_state == LOAD_NONE)
	{ // ローディングが行われていない場合

		// ロード情報を初期化
		m_bEnd = false;

		// スレッドに引数の関数を設定
		m_funcLoad = std::thread(funcLoad, &m_bEnd);

		// ロード画面を表示
		m_pLoad->SetEnableDraw(true);
		m_pLoad->SetPattern(0);

		// ロード更新状態にする
		m_state = LOAD_UPDATE;
	}
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
