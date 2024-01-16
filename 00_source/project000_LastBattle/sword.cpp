//============================================================
//
//	剣処理 [sword.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "sword.h"
#include "manager.h"
#include "renderer.h"
#include "orbit.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char *MODEL_FILE[] =	// モデルファイル
	{
		"data\\MODEL\\PLAYER\\15_sword.x",	// 剣
	};

	const float SUB_ALPHA	= 0.05f;	// 透明度の減算量
	const int	ORBIT_PART	= 25;		// 分割数
	const COrbit::SOffset ORBIT_OFFSET = COrbit::SOffset(D3DXVECTOR3(0.0f, 0.0f, -10.0f), D3DXVECTOR3(0.0f, 0.0f, -65.0f), XCOL_CYAN);	// オフセット情報
}

//************************************************************
//	スタティックアサート
//************************************************************
static_assert(NUM_ARRAY(MODEL_FILE) == CSword::MODEL_MAX, "ERROR : Model Count Mismatch");

//************************************************************
//	子クラス [CSword] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CSword::CSword() : CMultiModel(LABEL_NONE, DIM_3D, object::DEFAULT_PRIO),
	m_state			(STATE_NORMAL),	// 状態
	m_bDisp			(false),		// 表示状況
	m_nCounterState	(0)				// 状態管理カウンター
{

}

//============================================================
//	デストラクタ
//============================================================
CSword::~CSword()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CSword::Init(void)
{
	// メンバ変数を初期化
	m_state = STATE_NORMAL;	// 状態
	m_bDisp = true;			// 表示状況
	m_nCounterState = 0;	// 状態管理カウンター

	// マルチモデルの初期化
	if (FAILED(CMultiModel::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// モデルを登録・割当
	BindModel(MODEL_FILE[MODEL_SWORD]);

	// 軌跡の生成
	m_pOrbit = COrbit::Create
	( // 引数
		GetPtrMtxWorld(),	// 親マトリックス
		ORBIT_OFFSET,		// オフセット情報
		ORBIT_PART			// 分割数
	);
	if (m_pOrbit == nullptr)
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
void CSword::Uninit(void)
{
	// 軌跡の終了
	SAFE_UNINIT(m_pOrbit);

	// マルチモデルの終了
	CMultiModel::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CSword::Update(void)
{
	if (m_state == STATE_VANISH)
	{ // 消失中の場合

		// 変数を宣言
		float fAlpha = GetAlpha();	// 透明度

		// 透明にしていく
		fAlpha -= SUB_ALPHA;

		if (fAlpha <= 0.0f)
		{ // 透明になり切った場合

			// 表示をOFFにする
			m_bDisp = false;

			// 軌跡を消す
			m_pOrbit->SetState(COrbit::STATE_VANISH);
		}

		// 透明度を反映
		SetAlpha(fAlpha);
	}

	// 軌跡の更新
	m_pOrbit->Update();

	// マルチモデルの更新
	CMultiModel::Update();
}

//============================================================
//	描画処理
//============================================================
void CSword::Draw(void)
{
	if (m_bDisp)
	{ // 表示がONの場合

		// マルチモデルの描画
		CMultiModel::Draw();
	}
}

//============================================================
//	状態取得処理
//============================================================
int CSword::GetState(void) const
{
	// 状態を返す
	return m_state;
}

//============================================================
//	生成処理
//============================================================
CSword *CSword::Create
(
	CObject *pObject,			// 親オブジェクト
	const D3DXVECTOR3 &rPos,	// 位置
	const D3DXVECTOR3 &rRot		// 向き
)
{
	// 剣の生成
	CSword *pSword = new CSword;
	if (pSword == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// 剣の初期化
		if (FAILED(pSword->Init()))
		{ // 初期化に失敗した場合

			// 剣の破棄
			SAFE_DELETE(pSword);
			return nullptr;
		}

		// 親オブジェクトを設定
		pSword->SetParentObject(pObject);

		// 位置を設定
		pSword->SetVec3Position(rPos);

		// 向きを設定
		pSword->SetVec3Rotation(rRot);

		// 確保したアドレスを返す
		return pSword;
	}
}

//============================================================
//	状態の設定処理
//============================================================
void CSword::SetState(const EState state)
{
	if (state == m_state && state != STATE_NORMAL)
	{ // 設定する状態が現在の状態且つ、設定する状態が通常状態の場合

		// 処理を抜ける
		return;
	}

	// 引数の状態を設定
	m_state = state;

	switch (m_state)
	{ // 状態ごとの処理
	case STATE_NONE:	// 何もしない状態
	case STATE_VANISH:	// 消失状態
		break;

	case STATE_NORMAL:	// 通常状態

		// 不透明にする
		SetAlpha(1.0f);

		// 表示をONにする
		m_bDisp = true;

		// 軌跡を出す
		m_pOrbit->SetState(COrbit::STATE_NORMAL);

		break;

	default:	// 例外状態
		assert(false);
		break;
	}
}
