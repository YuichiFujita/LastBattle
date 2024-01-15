//============================================================
//
//	オブジェクト分割キャラクター処理 [objectDivChara.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "objectDivChara.h"
#include "manager.h"
#include "renderer.h"
#include "objectChara.h"
#include "multiModel.h"

//************************************************************
//	子クラス [CObjectDivChara] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CObjectDivChara::CObjectDivChara(const CObject::ELabel label, const CObject::EDim dimension, const int nPriority) : CObject(label, dimension, nPriority), m_nUpperParentID(0)
{
	// メンバ変数をクリア
	memset(&m_apBody[0], 0, sizeof(m_apBody));	// 身体の情報
}

//============================================================
//	デストラクタ
//============================================================
CObjectDivChara::~CObjectDivChara()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CObjectDivChara::Init(void)
{
	// メンバ変数を初期化
	memset(&m_apBody[0], 0, sizeof(m_apBody));	// 身体の情報
	m_nUpperParentID = 0;	// 上半身親インデックス

	for (int i = 0; i < BODY_MAX; i++)
	{ // 分割した身体の数分繰り返す

		// オブジェクトキャラクターの生成
		m_apBody[i] = CObjectChara::Create(VEC3_ZERO);
		if (m_apBody[i] == nullptr)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// 自動描画をOFFにする
		m_apBody[i]->SetEnableDraw(false);
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CObjectDivChara::Uninit(void)
{
	for (int i = 0; i < BODY_MAX; i++)
	{ // 分割した身体の数分繰り返す

		// オブジェクトキャラクターの終了
		m_apBody[i]->Uninit();
	}

	// オブジェクト分割キャラクターを破棄
	Release();
}

//============================================================
//	更新処理
//============================================================
void CObjectDivChara::Update(void)
{
	for (int i = 0; i < BODY_MAX; i++)
	{ // 分割した身体の数分繰り返す

		// オブジェクトキャラクターの更新
		m_apBody[i]->Update();
	}
}

//============================================================
//	描画処理
//============================================================
void CObjectDivChara::Draw(void)
{
	// 変数を宣言
	D3DXVECTOR3 pos = GetVec3Position();	// 下半身の位置
	D3DXVECTOR3 rot = GetVec3Rotation();	// 下半身の向き
	D3DXMATRIX  mtxRot, mtxTrans, mtxWorld;	// 計算用マトリックス

	// 変数配列を宣言
	D3DXMATRIX *apMtxWorld[] = { &mtxWorld, GetMultiModel(BODY_LOWER, m_nUpperParentID)->GetPtrMtxWorld() };
	static_assert(NUM_ARRAY(apMtxWorld) == BODY_MAX, "ERROR : Body Count Mismatch");

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// デバイスのポインタ

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	// 下半身のワールドマトリックスの反映
	m_apBody[BODY_LOWER]->SetMtxWorld(mtxWorld);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	for (int nCntChara = 0; nCntChara < BODY_MAX; nCntChara++)
	{ // 分割した身体の数分繰り返す

		for (int nCntParts = 0; nCntParts < m_apBody[nCntChara]->GetNumModel(); nCntParts++)
		{ // パーツの総数分繰り返す

			// パーツの描画
			GetMultiModel((EBody)nCntChara, nCntParts)->Draw();
		}
	}
}

//============================================================
//	位置の設定処理
//============================================================
void CObjectDivChara::SetVec3Position(const D3DXVECTOR3 &rPos)
{
	// 引数の位置を設定
	m_apBody[BODY_LOWER]->SetVec3Position(rPos);
}

//============================================================
//	位置取得処理
//============================================================
D3DXVECTOR3 CObjectDivChara::GetVec3Position(void) const
{
	// 位置を返す
	return m_apBody[BODY_LOWER]->GetVec3Position();
}

//============================================================
//	向きの設定処理
//============================================================
void CObjectDivChara::SetVec3Rotation(const D3DXVECTOR3 &rRot)
{
	// 引数の向きを設定
	m_apBody[BODY_LOWER]->SetVec3Rotation(rRot);
}

//============================================================
//	向き取得処理
//============================================================
D3DXVECTOR3 CObjectDivChara::GetVec3Rotation(void) const
{
	// 向きを返す
	return m_apBody[BODY_LOWER]->GetVec3Rotation();
}

//============================================================
//	マテリアル全設定処理
//============================================================
void CObjectDivChara::SetAllMaterial(const D3DXMATERIAL &rMat)
{
	for (int i = 0; i < BODY_MAX; i++)
	{ // 分割した身体の数分繰り返す

		// 引数のマテリアルを全マテリアルに設定
		m_apBody[i]->SetAllMaterial(rMat);
	}
}

//============================================================
//	更新状況の設定処理
//============================================================
void CObjectDivChara::SetEnableUpdate(const bool bUpdate)
{
	// 引数の更新状況を設定
	CObject::SetEnableUpdate(bUpdate);	// 自身

	for (int i = 0; i < BODY_MAX; i++)
	{ // 分割した身体の数分繰り返す

		m_apBody[i]->SetEnableUpdate(bUpdate);	// 上・下半身
	}
}

//============================================================
//	描画状況の設定処理
//============================================================
void CObjectDivChara::SetEnableDraw(const bool bDraw)
{
	// 引数の描画状況を設定
	CObject::SetEnableDraw(bDraw);	// 自身

	for (int i = 0; i < BODY_MAX; i++)
	{ // 分割した身体の数分繰り返す

		m_apBody[i]->SetEnableDraw(bDraw);	// 上・下半身
	}
}

//============================================================
//	生成処理
//============================================================
CObjectDivChara *CObjectDivChara::Create
(
	const D3DXVECTOR3 &rPos,	// 位置
	const D3DXVECTOR3 &rRot,	// 向き
	const int nUpperParentID	// 上半身親インデックス
)
{
	// オブジェクト分割キャラクターの生成
	CObjectDivChara *pObjectDivChara = new CObjectDivChara;
	if (pObjectDivChara == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// オブジェクト分割キャラクターの初期化
		if (FAILED(pObjectDivChara->Init()))
		{ // 初期化に失敗した場合

			// オブジェクト分割キャラクターの破棄
			SAFE_DELETE(pObjectDivChara);
			return nullptr;
		}

		// 上半身の親インデックスを設定
		pObjectDivChara->SetUpperParentID(nUpperParentID);

		// 位置を設定
		pObjectDivChara->SetVec3Position(rPos);

		// 向きを設定
		pObjectDivChara->SetVec3Rotation(rRot);

		// 確保したアドレスを返す
		return pObjectDivChara;
	}
}

//============================================================
//	上半身の親インデックスの設定処理
//============================================================
void CObjectDivChara::SetUpperParentID(const int nUpperParentID)
{
	if (nUpperParentID > NONE_IDX && nUpperParentID < m_apBody[BODY_LOWER]->GetNumModel())
	{ // 正規インデックスの場合

		// 上半身の親インデックスを設定
		m_nUpperParentID = nUpperParentID;
	}
	else { assert(false); }	// インデックスエラー
}

//============================================================
//	オブジェクトキャラクター取得処理
//============================================================
CObjectChara *CObjectDivChara::GetObjectChara(const EBody bodyID) const
{
	if (bodyID > NONE_IDX && bodyID < BODY_MAX)
	{ // 正規インデックスの場合

		// 引数インデックスの半身を返す
		return m_apBody[bodyID];
	}

	// インデックスエラー
	assert(false);
	return nullptr;
}

//============================================================
//	マルチモデル取得処理
//============================================================
CMultiModel *CObjectDivChara::GetMultiModel(const EBody bodyID, const int nModelID) const
{
	if (bodyID > NONE_IDX && bodyID < BODY_MAX)
	{
		if (nModelID > NONE_IDX && nModelID < m_apBody[bodyID]->GetNumModel())
		{ // 正規インデックスの場合

			// 引数インデックスのパーツを返す
			return m_apBody[bodyID]->GetMultiModel(nModelID);
		}
	}

	// インデックスエラー
	assert(false);
	return nullptr;
}

//============================================================
//	破棄処理
//============================================================
void CObjectDivChara::Release(void)
{
	// オブジェクトの破棄
	CObject::Release();
}
