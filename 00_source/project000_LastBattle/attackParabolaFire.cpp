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
	const float	GRAVITY = -1.0f;	// 重力
}

//************************************************************
//	子クラス [CAttackParabolaFire] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CAttackParabolaFire::CAttackParabolaFire() :
	m_nCounterTime	(0),	// 攻撃時間管理カウンター
	m_fMove			(0.0f)	// 移動量
{

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
	// メンバ変数を初期化
	m_nCounterTime = 0;	// 攻撃時間管理カウンター
	m_fMove = 0.0f;		// 移動量

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
CAttackParabolaFire *CAttackParabolaFire::Create
(
	const D3DXVECTOR3& rPos,	// 位置
	const float fMove			// 移動量
)
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

		// 移動量を設定
		pAttackParabolaFire->m_fMove = fMove;

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
	float fMaxTime, fMaxPosY;

#if 1
	// 現在の経過時間から炎の位置を求める
	D3DXVECTOR2 pos = useful::CalcPosParabola(GRAVITY, m_fMove, posMaxX, posMaxY, (float)m_nCounterTime, &fMaxTime, &fMaxPosY);

	if ((float)m_nCounterTime <= fMaxTime)
	{ // 最大経過時間に到達していない場合

		// 位置を反映
		SetVec3Position(D3DXVECTOR3(pos.x, pos.y, 0.0f));
	}
#else
	// 現在の経過時間から炎の移動量を求める
	D3DXVECTOR2 move = useful::CalcMoveParabola(GRAVITY, m_fMove, posMaxX, posMaxY, (float)m_nCounterTime, &fMaxTime, &fMaxPosY);

	if ((float)m_nCounterTime <= fMaxTime)
	{ // 最大経過時間に到達していない場合

		// 移動量を反映
		SetMove(D3DXVECTOR3(move.x, move.y, 0.0f));
	}
	else
	{ // 最大経過時間に到達していない場合

		// 移動量を初期化
		SetMove(VEC3_ZERO);
	}
#endif

	// 攻撃発射からのカウンターを加算
	m_nCounterTime++;
}
