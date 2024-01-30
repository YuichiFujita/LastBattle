//============================================================
//
//	雷攻撃処理 [attackThunder.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "attackThunder.h"
#include "thunder.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{

}

//************************************************************
//	子クラス [CAttackThunder] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CAttackThunder::CAttackThunder() : CObject(CObject::LABEL_THUNDER, DIM_3D)
{

}

//============================================================
//	デストラクタ
//============================================================
CAttackThunder::~CAttackThunder()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CAttackThunder::Init(void)
{
	for (int nCntThunder = 0; nCntThunder < attackThunder::NUM_THUNDER; nCntThunder++)
	{ // 雷の生成数分繰り返す

		// 雷の生成
		m_apThunder[nCntThunder] = CThunder::Create(VEC3_ZERO);
		if (m_apThunder[nCntThunder] == nullptr)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CAttackThunder::Uninit(void)
{
	for (int nCntThunder = 0; nCntThunder < attackThunder::NUM_THUNDER; nCntThunder++)
	{ // 雷の生成数分繰り返す

		// 雷の終了
		SAFE_UNINIT(m_apThunder[nCntThunder]);
	}

	// 雷攻撃オブジェクトを破棄
	Release();
}

//============================================================
//	更新処理
//============================================================
void CAttackThunder::Update(void)
{
	for (int nCntThunder = 0; nCntThunder < attackThunder::NUM_THUNDER; nCntThunder++)
	{ // 雷の生成数分繰り返す

		// 雷の更新
		m_apThunder[nCntThunder]->Update();
	}
}

//============================================================
//	描画処理
//============================================================
void CAttackThunder::Draw(void)
{

}

//============================================================
//	生成処理
//============================================================
CAttackThunder *CAttackThunder::Create(const D3DXVECTOR3 &rPos)
{
	// 雷攻撃の生成
	CAttackThunder *pAttackThunder = new CAttackThunder;
	if (pAttackThunder == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// 雷攻撃の初期化
		if (FAILED(pAttackThunder->Init()))
		{ // 初期化に失敗した場合

			// 雷攻撃の破棄
			SAFE_DELETE(pAttackThunder);
			return nullptr;
		}

		// 位置を設定
		pAttackThunder->SetVec3Position(rPos);

		// 確保したアドレスを返す
		return pAttackThunder;
	}
}

//============================================================
//	破棄処理
//============================================================
void CAttackThunder::Release(void)
{
	// オブジェクトの破棄
	CObject::Release();
}

//============================================================
//	位置の設定処理
//============================================================
void CAttackThunder::SetVec3Position(const D3DXVECTOR3 &rPos)
{

}
