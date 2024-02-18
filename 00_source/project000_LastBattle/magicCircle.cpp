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
#include "objectMeshRing.h"

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
	const float	ALPHA_LENGTH	= -1000.0f;	// 透明の長さ
	const POSGRID2 PART_CIRCLE	= POSGRID2(32, 2);	// 魔法陣の分割数
	const POSGRID2 PART_ALPHA	= POSGRID2(32, 1);	// 透明の分割数
}

//************************************************************
//	スタティックアサート
//************************************************************
static_assert(NUM_ARRAY(TEXTURE_FILE) == CMagicCircle::TEXTURE_MAX, "ERROR : Texture Count Mismatch");

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CListManager<CMagicCircle> *CMagicCircle::m_pList = nullptr;	// オブジェクトリスト

//************************************************************
//	子クラス [CMagicCircle] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CMagicCircle::CMagicCircle() : CObjectMeshCircle(CObject::LABEL_MAGIC_CIRCLE, CObject::DIM_3D, PRIORITY),
	m_pAlphaCylinder	(nullptr),	// 魔法陣の先の透明情報
	m_pAlphaRing		(nullptr)	// 魔法陣の空白の透明情報
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
	m_pAlphaRing = nullptr;		// 魔法陣の空白の透明情報

	//--------------------------------------------------------
	//	基底クラスの初期化・設定
	//--------------------------------------------------------
	{
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

		// Zテストを絶対成功にする
		//pRenderState->SetZFunc(D3DCMP_ALWAYS);

		// ポリゴンの両面を表示状態にする
		pRenderState->SetCulling(D3DCULL_NONE);

		// ライティングをOFFにする
		pRenderState->SetLighting(false);
	}

	//--------------------------------------------------------
	//	魔法陣の透明シリンダーの初期化・設定
	//--------------------------------------------------------
	{
		// 魔法陣の先の透明表示の生成
		m_pAlphaCylinder = CObjectMeshCylinder::Create
		( // 引数
			VEC3_ZERO,		// 位置
			VEC3_ZERO,		// 向き
			XCOL_WHITE,	// 色
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

		// 自動描画をOFFにする
		m_pAlphaCylinder->SetEnableDraw(false);

		// Zテストを絶対成功にする
		//m_pAlphaCylinder->GetRenderState()->SetZFunc(D3DCMP_ALWAYS);

		// ポリゴンの両面を表示状態にする
		m_pAlphaCylinder->GetRenderState()->SetCulling(D3DCULL_NONE);

		// ライティングをOFFにする
		m_pAlphaCylinder->GetRenderState()->SetLighting(false);
	}

	//--------------------------------------------------------
	//	魔法陣の透明リングの初期化・設定
	//--------------------------------------------------------
	{
		// 魔法陣の空白の透明表示の生成
		m_pAlphaRing = CObjectMeshRing::Create
		( // 引数
			VEC3_ZERO,		// 位置
			VEC3_ZERO,		// 向き
			XCOL_WHITE,	// 色
			PART_ALPHA,		// 分割数
			GRID2_ONE,		// テクスチャ分割数
			0.0f,			// 穴の半径
			0.0f,			// 太さ
			0.0f			// 外周のY座標加算量
		);
		if (m_pAlphaRing == nullptr)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// 自動描画をOFFにする
		m_pAlphaRing->SetEnableDraw(false);

		// Zテストを絶対成功にする
		//m_pAlphaRing->GetRenderState()->SetZFunc(D3DCMP_ALWAYS);

		// ポリゴンの両面を表示状態にする
		m_pAlphaRing->GetRenderState()->SetCulling(D3DCULL_NONE);

		// ライティングをOFFにする
		m_pAlphaRing->GetRenderState()->SetLighting(false);
	}

	if (m_pList == nullptr)
	{ // リストマネージャーが存在しない場合

		// リストマネージャーの生成
		m_pList = CListManager<CMagicCircle>::Create();
		if (m_pList == nullptr)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}
	}

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);

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

	// 魔法陣の空白の透明情報の終了
	SAFE_UNINIT(m_pAlphaRing);

	// リストから自身のオブジェクトを削除
	m_pList->DeleteList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}

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
void CMagicCircle::Draw(CShader *pShader)
{
	// オブジェクトメッシュサークルの描画
	CObjectMeshCircle::Draw();

	// 魔法陣の先の透明情報の描画
	m_pAlphaCylinder->Draw();

	// 魔法陣の空白の透明情報の描画
	m_pAlphaRing->Draw();
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

	// 魔法陣の空白の透明表示の位置を設定
	m_pAlphaRing->SetVec3Position(rPos);
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

	// 魔法陣の空白の透明表示の向きを設定
	m_pAlphaRing->SetVec3Rotation(rRot);
}

//============================================================
//	半径の設定処理
//============================================================
void CMagicCircle::SetRadius(const float fRadius)
{
	// 自身の半径を設定
	CObjectMeshCircle::SetRadius(fRadius);

	// 魔法陣の空白を隙間を埋める大きさに設定
	m_pAlphaRing->SetHoleRadius(fRadius);
	m_pAlphaRing->SetThickness(m_pAlphaCylinder->GetRadius() - fRadius);
}

//============================================================
//	生成処理
//============================================================
CMagicCircle *CMagicCircle::Create
(
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rRot,	// 向き
	const float fRadius,		// 半径
	const float fAlphaRadius	// 透明半径
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

		// 透明半径を設定
		pMagicCircle->SetAlphaRadius(fAlphaRadius);

		// 確保したアドレスを返す
		return pMagicCircle;
	}
}

//============================================================
//	リスト取得処理
//============================================================
CListManager<CMagicCircle>* CMagicCircle::GetList(void)
{
	// オブジェクトリストを返す
	return m_pList;
}

//============================================================
//	切り抜き用の描画処理
//============================================================
void CMagicCircle::DrawCrop(void)
{
	// 自動描画がOFFの場合抜ける
	if (!IsDraw()) { return; }

	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// デバイス情報
	CTexture		*pTexture = GET_MANAGER->GetTexture();	// テクスチャ情報
	CStencilShader	*pStencilShader = CStencilShader::GetInstance();	// ステンシルシェーダー情報

	if (pDevice == nullptr || pTexture == nullptr || pStencilShader == nullptr)
	{ // 情報が無いものがあった場合

		// 処理を抜ける
		assert(false);
		return;
	}

	if (pStencilShader->IsEffectOK())
	{ // エフェクトが使用可能な場合

		// ピクセル描画色を設定
		pStencilShader->SetColor(XCOL_WHITE);

		//if (GET_INPUTKEY->IsPress(DIK_0))
		{
			// ステンシルテストを有効にする
			pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
		}

		// 比較参照値を設定する
		pDevice->SetRenderState(D3DRS_STENCILREF, 1);

		// ステンシルマスクを指定する 
		pDevice->SetRenderState(D3DRS_STENCILMASK, 255);

		// ステンシル比較関数を指定する
		pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);

		// ステンシル結果に対しての反映設定
		pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCRSAT);	// Zテスト・ステンシルテスト成功
		pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);		// Zテスト・ステンシルテスト失敗
		pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);		// Zテスト失敗・ステンシルテスト成功

		// 魔法陣の先の透明情報の描画
		m_pAlphaCylinder->Draw(pStencilShader);

		// 魔法陣の空白の透明情報の描画
		m_pAlphaRing->Draw(pStencilShader);

		// ステンシルテストを無効にする
		pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
	}
}

//============================================================
//	透明半径の設定処理
//============================================================
void CMagicCircle::SetAlphaRadius(const float fRadius)
{
	// 魔法陣の先の透明表示の半径を設定
	m_pAlphaCylinder->SetRadius(fRadius);

	// 魔法陣の空白を隙間を埋める大きさに設定
	float fThisRadius = GetRadius();	// 魔法陣自身の半径
	m_pAlphaRing->SetHoleRadius(fThisRadius);
	m_pAlphaRing->SetThickness(fRadius - fThisRadius);
}
