//============================================================
//
//	カーブ攻撃炎処理 [attackParabolaFire.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "attackParabolaFire.h"
#include "player.h"

// TODO
#include "manager.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const float	MOVE	= 16.0f;	// 炎の移動量
	const float	REV_ROT	= 0.08f;	// 炎の向き変更補正係数
}

//************************************************************
//	子クラス [CAttackParabolaFire] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CAttackParabolaFire::CAttackParabolaFire()
{
	nCnt = 0;
}

//============================================================
//	デストラクタ
//============================================================
CAttackParabolaFire::~CAttackParabolaFire()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CAttackParabolaFire::Init(void)
{
	// 炎の初期化
	if (FAILED(CFire::Init()))
	{ // 初期化に失敗した場合

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
void CAttackParabolaFire::Uninit(void)
{
	// 炎の終了
	CFire::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CAttackParabolaFire::Update(void)
{
	// 炎移動量の更新
	UpdateParabolaMove();

	// 炎の更新
	CFire::Update();
}

//============================================================
//	描画処理
//============================================================
void CAttackParabolaFire::Draw(void)
{
	// 炎の描画
	CFire::Draw();
}

//============================================================
//	生成処理
//============================================================
CAttackParabolaFire *CAttackParabolaFire::Create(const D3DXVECTOR3& rPos)
{
	// カーブ攻撃炎の生成
	CAttackParabolaFire *pAttackParabolaFire = new CAttackParabolaFire;
	if (pAttackParabolaFire == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// カーブ攻撃炎の初期化
		if (FAILED(pAttackParabolaFire->Init()))
		{ // 初期化に失敗した場合

			// カーブ攻撃炎の破棄
			SAFE_DELETE(pAttackParabolaFire);
			return nullptr;
		}

		// 位置を設定
		pAttackParabolaFire->SetVec3Position(rPos);

		// 確保したアドレスを返す
		return pAttackParabolaFire;
	}
}

//============================================================
//	炎移動量の更新処理
//============================================================
void CAttackParabolaFire::UpdateParabolaMove(void)
{
	float posMaxX = 1000.0f;
	float posMaxY = 300.0f;
	const float v0x = 10.0f;
	const float g = -1.0f;

	nCnt++;

#if 0
	if (nCnt <= posMaxX / v0x)
	{
		// 移動量を反映
		D3DXVECTOR2 pos = CalcParabola(g, v0x, posMaxX, posMaxY, (float)nCnt);
		SetVec3Position(D3DXVECTOR3(pos.x, pos.y, 0.0f));
	}
	else
	{
		SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}
#else
	if (nCnt <= posMaxX / v0x)
	{
		// 移動量を反映
		D3DXVECTOR2 move = CalcMoveParabola(g, v0x, posMaxX, posMaxY, (float)nCnt);
		SetMove(D3DXVECTOR3(move.x, move.y, 0.0f));
	}
	else
	{
		SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}
#endif

	GET_MANAGER->GetDebugProc()->Print(CDebugProc::POINT_CENTER, "%f %f %f\n", GetVec3Position().x, GetVec3Position().y, GetVec3Position().z);
}

D3DXVECTOR2 CAttackParabolaFire::CalcParabola(float g, float v0x, float maxX, float maxY, float t)
{
	D3DXVECTOR2 vecRet = VEC2_ZERO;

	// x成分を求める
	vecRet.x = v0x * t;

	// タイム最大値を求める
	const float tMax = maxX / v0x;
	const float v0y  = -0.5f * g * tMax;

	float timeHalf = tMax * 0.5f;
	float tempMaxY = 0.5f * g * (timeHalf * timeHalf) + v0y * timeHalf;
	float rateMaxY = maxY / tempMaxY;

	// y成分を求める
	vecRet.y = (0.5f * g * (t * t) + v0y * t) * rateMaxY;

	return vecRet;
}

D3DXVECTOR2 CAttackParabolaFire::CalcMoveParabola(float g, float v0x, float maxX, float maxY, float t)
{
	D3DXVECTOR2 vecRet = VEC2_ZERO;

	// x成分を求める
	vecRet.x = v0x;

	// タイム最大値を求める
	const float tMax = maxX / v0x;
	const float v0y = -0.5f * g * tMax;

	float timeHalf = tMax * 0.5f;
	float tempMaxY = 0.5f * g * (timeHalf * timeHalf) + v0y * timeHalf;
	float rateMaxY = maxY / tempMaxY;

	// y成分を求める
	vecRet.y = (g * t + v0y) * rateMaxY;

	return vecRet;
}
