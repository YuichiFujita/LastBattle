//============================================================
//
//	オブジェクトメッシュスフィア処理 [objectMeshSphere.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "objectMeshSphere.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "objectMeshDome.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const POSGRID2 MIN_PART		= POSGRID2(1, 2);	// 分割数の最小値
	const POSGRID2 MIN_TEXPART	= GRID2_ONE;		// テクスチャ分割数の最小値
}

//************************************************************
//	子クラス [CObjectMeshSphere] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CObjectMeshSphere::CObjectMeshSphere(const CObject::ELabel label, const CObject::EDim dimension, const int nPriority) : CObject(label, dimension, nPriority),
	m_part		(GRID2_ZERO),	// 分割数
	m_texPart	(GRID2_ZERO)	// テクスチャ分割数
{
	// メンバ変数をクリア
	memset(&m_apDome[0], 0, sizeof(m_apDome));	// 半球の情報
}

//============================================================
//	デストラクタ
//============================================================
CObjectMeshSphere::~CObjectMeshSphere()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CObjectMeshSphere::Init(void)
{
	// メンバ変数を初期化
	memset(&m_apDome[0], 0, sizeof(m_apDome));	// 半球の情報
	m_part		= MIN_PART;		// 分割数
	m_texPart	= MIN_TEXPART;	// テクスチャ分割数

	for (int i = 0; i < DOME_MAX; i++)
	{ // 半球の総数分繰り返す

		// メッシュドームの生成
		m_apDome[i] = CObjectMeshDome::Create
		( // 引数
			VEC3_ZERO,		// 位置
			VEC3_ZERO,		// 向き
			XCOL_WHITE,		// 色
			MIN_PART,		// 分割数
			MIN_TEXPART,	// テクスチャ分割数
			0.0f			// 半径
		);
	}

	// 分割数を設定
	if (FAILED(SetPattern(MIN_PART)))
	{ // 分割数の設定に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CObjectMeshSphere::Uninit(void)
{
	for (int i = 0; i < DOME_MAX; i++)
	{ // 半球の総数分繰り返す

		// メッシュドームの終了
		SAFE_UNINIT(m_apDome[i]);
	}

	// オブジェクトメッシュスフィアを破棄
	Release();
}

//============================================================
//	更新処理
//============================================================
void CObjectMeshSphere::Update(void)
{
	for (int i = 0; i < DOME_MAX; i++)
	{ // 半球の総数分繰り返す

		// メッシュドームの更新
		m_apDome[i]->Update();
	}
}

//============================================================
//	描画処理
//============================================================
void CObjectMeshSphere::Draw(void)
{
	for (int i = 0; i < DOME_MAX; i++)
	{ // 半球の総数分繰り返す

		// メッシュドームの描画
		m_apDome[i]->Draw();
	}
}

//============================================================
//	テクスチャ割当処理 (インデックス)
//============================================================
void CObjectMeshSphere::BindTexture(const int nTextureID)
{
	if (nTextureID >= NONE_IDX)
	{ // テクスチャインデックスが使用可能な場合

		for (int i = 0; i < DOME_MAX; i++)
		{ // 半球の総数分繰り返す

			// テクスチャインデックスを割当
			m_apDome[i]->BindTexture(nTextureID);
		}
	}
	else { assert(false); }	// 範囲外
}

//============================================================
//	テクスチャ割当処理 (パス)
//============================================================
void CObjectMeshSphere::BindTexture(const char *pTexturePass)
{
	// ポインタを宣言
	CTexture *pTexture = GET_MANAGER->GetTexture();	// テクスチャへのポインタ

	if (pTexturePass != nullptr)
	{ // 割り当てるテクスチャパスがある場合

		for (int i = 0; i < DOME_MAX; i++)
		{ // 半球の総数分繰り返す

			// テクスチャインデックスを割当
			m_apDome[i]->BindTexture(pTexture->Regist(pTexturePass));
		}
	}
	else
	{ // 割り当てるテクスチャパスがない場合

		for (int i = 0; i < DOME_MAX; i++)
		{ // 半球の総数分繰り返す

			// テクスチャなしインデックスを割当
			m_apDome[i]->BindTexture(NONE_IDX);
		}
	}
}

//============================================================
//	位置の設定処理
//============================================================
void CObjectMeshSphere::SetVec3Position(const D3DXVECTOR3& rPos)
{
	for (int i = 0; i < DOME_MAX; i++)
	{ // 半球の総数分繰り返す

		// 引数の位置を設定
		m_apDome[i]->SetVec3Position(rPos);
	}
}

//============================================================
//	位置取得処理
//============================================================
D3DXVECTOR3 CObjectMeshSphere::GetVec3Position(void) const
{
	// 位置を返す
	return m_apDome[DOME_TOP]->GetVec3Position();
}

//============================================================
//	向きの設定処理
//============================================================
void CObjectMeshSphere::SetVec3Rotation(const D3DXVECTOR3& rRot)
{
	for (int i = 0; i < DOME_MAX; i++)
	{ // 半球の総数分繰り返す

		// 引数の向きを設定
		D3DXVECTOR3 rot = rRot;
		rot.x += ((float)i * D3DX_PI);
		m_apDome[i]->SetVec3Rotation(rot);
	}
}

//============================================================
//	向き取得処理
//============================================================
D3DXVECTOR3 CObjectMeshSphere::GetVec3Rotation(void) const
{
	// 向きを返す
	return m_apDome[DOME_TOP]->GetVec3Rotation();
}

//============================================================
//	色の設定処理
//============================================================
void CObjectMeshSphere::SetColor(const D3DXCOLOR& rCol)
{
	for (int i = 0; i < DOME_MAX; i++)
	{ // 半球の総数分繰り返す

		// 引数の色を設定
		m_apDome[i]->SetColor(rCol);
	}
}

//============================================================
//	色取得処理
//============================================================
D3DXCOLOR CObjectMeshSphere::GetColor(void) const
{
	// 色を返す
	return m_apDome[DOME_TOP]->GetColor();
}

//============================================================
//	半径の設定処理
//============================================================
void CObjectMeshSphere::SetRadius(const float fRadius)
{
	for (int i = 0; i < DOME_MAX; i++)
	{ // 半球の総数分繰り返す

		// 引数の半径を設定
		m_apDome[i]->SetRadius(fRadius);
	}
}

//============================================================
//	半径取得処理
//============================================================
float CObjectMeshSphere::GetRadius(void) const
{
	// 半径を返す
	return m_apDome[DOME_TOP]->GetRadius();
}

//============================================================
//	生成処理
//============================================================
CObjectMeshSphere *CObjectMeshSphere::Create
(
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rRot,	// 向き
	const D3DXCOLOR& rCol,		// 色
	const POSGRID2& rPart,		// 分割数
	const POSGRID2& rTexPart,	// テクスチャ分割数
	const float fRadius			// 半径
)
{
	// オブジェクトメッシュスフィアの生成
	CObjectMeshSphere *pMeshSphere = new CObjectMeshSphere;
	if (pMeshSphere == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// オブジェクトメッシュスフィアの初期化
		if (FAILED(pMeshSphere->Init()))
		{ // 初期化に失敗した場合

			// オブジェクトメッシュスフィアの破棄
			SAFE_DELETE(pMeshSphere);
			return nullptr;
		}

		// 位置を設定
		pMeshSphere->SetVec3Position(rPos);

		// 向きを設定
		pMeshSphere->SetVec3Rotation(rRot);

		// 色を設定
		pMeshSphere->SetColor(rCol);

		// 半径を設定
		pMeshSphere->SetRadius(fRadius);

		// 分割数を設定
		if (FAILED(pMeshSphere->SetPattern(rPart)))
		{ // 分割数の設定に失敗した場合

			// オブジェクトメッシュスフィアの破棄
			SAFE_DELETE(pMeshSphere);
			return nullptr;
		}

		// テクスチャ分割数を設定
		pMeshSphere->SetTexPattern(rTexPart);

		// 確保したアドレスを返す
		return pMeshSphere;
	}
}

//============================================================
//	分割数の設定処理
//============================================================
HRESULT CObjectMeshSphere::SetPattern(const POSGRID2& rPart)
{
	for (int i = 0; i < DOME_MAX; i++)
	{ // 半球の総数分繰り返す

		// 引数の分割数を設定
		if (FAILED(m_apDome[i]->SetPattern(rPart)))
		{ // 設定に失敗した場合

			// 失敗を返す
			assert(false);
			return S_OK;
		}
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	分割数取得処理
//============================================================
POSGRID2 CObjectMeshSphere::GetPattern(void) const
{
	// 分割数を返す
	return m_apDome[DOME_TOP]->GetPattern();
}

//============================================================
//	テクスチャ分割数の設定処理
//============================================================
void CObjectMeshSphere::SetTexPattern(const POSGRID2& rTexPart)
{
	if (rTexPart.x >= MIN_TEXPART.x
	&&  rTexPart.y >= MIN_TEXPART.y)
	{ // テクスチャ分割数が最低値以上の場合

		for (int i = 0; i < DOME_MAX; i++)
		{ // 半球の総数分繰り返す

			// 引数のテクスチャ分割数を設定
			m_apDome[i]->SetTexPattern(rTexPart);
		}
	}
	else { assert(false); }	// 最低値未満
}

//============================================================
//	テクスチャ分割数取得処理
//============================================================
POSGRID2 CObjectMeshSphere::GetTexPattern(void) const
{
	// テクスチャ分割数を返す
	return m_apDome[DOME_TOP]->GetTexPattern();
}

//============================================================
//	破棄処理
//============================================================
void CObjectMeshSphere::Release(void)
{
	// オブジェクトの破棄
	CObject::Release();
}
