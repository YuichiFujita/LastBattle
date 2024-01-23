//============================================================
//
//	魔法陣処理 [magicCircle.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "magicCircle.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "objectMeshCylinder.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char *TEXTURE_FILE[] =	// テクスチャファイル
	{
		"data\\TEXTURE\\magicCircle000.png",	// 通常
	};

	const int	PRIORITY		= 5;		// 魔法陣の優先順位
	const float	ALPHA_LENGTH	= 1000.0f;	// 透明の長さ
	const POSGRID2 PART_CIRCLE	= POSGRID2(32, 2);	// 魔法陣の分割数
	const POSGRID2 PART_ALPHA	= POSGRID2(32, 1);	// 透明の分割数
}

//************************************************************
//	スタティックアサート
//************************************************************
static_assert(NUM_ARRAY(TEXTURE_FILE) == CMagicCircle::TEXTURE_MAX, "ERROR : Texture Count Mismatch");

//************************************************************
//	子クラス [CMagicCircle] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CMagicCircle::CMagicCircle() : CObjectMeshCircle(CObject::LABEL_MAGIC_CIRCLE, CObject::DIM_3D, PRIORITY), m_pAlphaCylinder(nullptr)
{

}

//============================================================
//	デストラクタ
//============================================================
CMagicCircle::~CMagicCircle()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CMagicCircle::Init(void)
{
	// メンバ変数を初期化
	m_pAlphaCylinder = nullptr;	// 魔法陣の先の透明情報

	// オブジェクトメッシュサークルの初期化
	if (FAILED(CObjectMeshCircle::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 分割数の設定
	if (FAILED(SetPattern(PART_CIRCLE)))
	{ // 分割数の設定に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// レンダーステートの情報を取得
	CRenderState *pRenderState = GetRenderState();

	// ポリゴンの両面を表示状態にする
	pRenderState->SetCulling(D3DCULL_NONE);

	// ライティングをOFFにする
	pRenderState->SetLighting(false);

	// 魔法陣の先の透明表示の生成
	m_pAlphaCylinder = CObjectMeshCylinder::Create
	( // 引数
		VEC3_ZERO,		// 位置
		VEC3_ZERO,		// 向き
		XCOL_AWHITE,	// 色
		PART_ALPHA,		// 分割数
		GRID2_ONE,		// テクスチャ分割数
		0.0f,			// 半径
		ALPHA_LENGTH	// 縦幅
	);
	if (m_pAlphaCylinder == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 優先順位を設定
	m_pAlphaCylinder->SetPriority(PRIORITY);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CMagicCircle::Uninit(void)
{
	// 魔法陣の先の透明情報の終了
	SAFE_UNINIT(m_pAlphaCylinder);

	// オブジェクトメッシュサークルの終了
	CObjectMeshCircle::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CMagicCircle::Update(void)
{
	// オブジェクトメッシュサークルの更新
	CObjectMeshCircle::Update();
}

//============================================================
//	描画処理
//============================================================
void CMagicCircle::Draw(void)
{
	// オブジェクトメッシュサークルの描画
	CObjectMeshCircle::Draw();
}

//============================================================
//	位置の設定処理
//============================================================
void CMagicCircle::SetVec3Position(const D3DXVECTOR3 &rPos)
{
	// 自身の位置を設定
	CObjectMeshCircle::SetVec3Position(rPos);

	// 魔法陣の先の透明表示の位置を設定
	m_pAlphaCylinder->SetVec3Position(rPos);
}

//============================================================
//	向きの設定処理
//============================================================
void CMagicCircle::SetVec3Rotation(const D3DXVECTOR3 &rRot)
{
	// 自身の向きを設定
	CObjectMeshCircle::SetVec3Rotation(rRot);

	// 魔法陣の先の透明表示の向きを設定
	m_pAlphaCylinder->SetVec3Rotation(rRot);
}

//============================================================
//	半径の設定処理
//============================================================
void CMagicCircle::SetRadius(const float fRadius)
{
	// 自身の半径を設定
	CObjectMeshCircle::SetRadius(fRadius);

	// 魔法陣の先の透明表示の半径を設定
	m_pAlphaCylinder->SetRadius(fRadius);
}

//============================================================
//	生成処理
//============================================================
CMagicCircle *CMagicCircle::Create
(
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rRot,	// 向き
	const float fRadius			// 半径
)
{
	// 魔法陣の生成
	CMagicCircle *pMagicCircle = new CMagicCircle;
	if (pMagicCircle == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// 魔法陣の初期化
		if (FAILED(pMagicCircle->Init()))
		{ // 初期化に失敗した場合

			// 魔法陣の破棄
			SAFE_DELETE(pMagicCircle);
			return nullptr;
		}

		// テクスチャを登録・割当
		pMagicCircle->BindTexture(GET_MANAGER->GetTexture()->Regist(TEXTURE_FILE[TEXTURE_NORMAL]));

		// 位置を設定
		pMagicCircle->SetVec3Position(rPos);

		// 向きを設定
		pMagicCircle->SetVec3Rotation(rRot);

		// 半径を設定
		pMagicCircle->SetRadius(fRadius);

		// 確保したアドレスを返す
		return pMagicCircle;
	}
}
