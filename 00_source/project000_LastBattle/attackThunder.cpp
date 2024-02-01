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
#include "manager.h"
#include "thunder.h"
#include "object3D.h"
#include "scene.h"
#include "stage.h"
#include "collision.h"
#include "impact.h"
#include "player.h"
#include "camera.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char *TEXTURE_WARNING = "data\\TEXTURE\\warning000.png";	// 警告表示のテクスチャファイル

	const CCamera::SSwing LAND_SWING = CCamera::SSwing(6.0f, 1.5f, 0.25f);	// 雷着弾時のカメラ揺れ

	const int	WARNING_PRIO	= 0;		// 警告表示の優先順位
	const int	DMG_THUNDER		= 15;		// 雷のダメージ量
	const float COLL_RADIUS		= 120.0f;	// 判定の半径
}

//************************************************************
//	子クラス [CAttackThunder] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CAttackThunder::CAttackThunder() : CObject(CObject::LABEL_THUNDER, DIM_3D),
	m_pWarning		(nullptr),		// 警告表示の情報
	m_posOrigin		(VEC3_ZERO),	// 雷の原点位置
	m_state			(STATE_WARN),	// 状態
	m_nCounterState	(0)				// 状態管理カウンター
{
	// メンバ変数をクリア
	memset(&m_apThunder[0], 0, sizeof(m_apThunder));	// 雷の情報
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
	// ポインタを宣言
	CStage *pStage = CScene::GetStage();	// ステージの情報
	assert(pStage != nullptr);	// ステージ未使用

	// メンバ変数をクリア
	memset(&m_apThunder[0], 0, sizeof(m_apThunder));	// 雷の情報
	m_pWarning	= nullptr;		// 警告表示の情報
	m_posOrigin = VEC3_ZERO;	// 雷の原点位置
	m_state		= STATE_WARN;	// 状態
	m_nCounterState = 0;		// 状態管理カウンター

	// 警告表示の生成
	m_pWarning = CObject3D::Create
	( // 引数
		D3DXVECTOR3(0.0f, pStage->GetStageLimit().fField, 0.0f),	// 位置
		D3DXVECTOR3(COLL_RADIUS * 2.0f, 0.0f, COLL_RADIUS * 2.0f)	// 大きさ
	);
	if (m_pWarning == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// テクスチャを登録・割当
	m_pWarning->BindTexture(TEXTURE_WARNING);

	// 優先順位を設定
	m_pWarning->SetPriority(WARNING_PRIO);

	// レンダーステートの設定
	CRenderState *pRenderState = m_pWarning->GetRenderState();	// レンダーステートの情報
	pRenderState->SetZFunc(D3DCMP_ALWAYS);						// Zテストを絶対成功にする
	pRenderState->SetAlphaBlend(CRenderState::BLEND_ADD);		// 加算合成にする

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CAttackThunder::Uninit(void)
{
	// 警告表示の終了
	SAFE_UNINIT(m_pWarning);

	// 雷攻撃オブジェクトを破棄
	Release();
}

//============================================================
//	更新処理
//============================================================
void CAttackThunder::Update(void)
{
	switch (m_state)
	{ // 状態ごとの処理
	case STATE_WARN:

		// カウンターを加算
		m_nCounterState++;
		if (m_nCounterState > 60)
		{ // 警告表示が終了した場合

			// 攻撃開始の設定
			if (FAILED(SetAttack())) { assert(false); }
		}

		// 警告表示の更新
		m_pWarning->Update();

		break;

	case STATE_ATTACK:

		for (int nCntThunder = 0; nCntThunder < attackThunder::NUM_THUNDER; nCntThunder++)
		{ // 雷の生成数分繰り返す

			if (m_apThunder[nCntThunder]->IsLanding())
			{ // 雷が地面に到達している場合

				// カメラ揺れを設定
				GET_MANAGER->GetCamera()->SetSwing(CCamera::TYPE_MAIN, LAND_SWING);

				// 衝撃波の生成
				CImpact::Create
				( // 引数
					CWave::TEXTURE_NONE,			// 種類
					D3DXVECTOR3(0.0f, 50.0f, 0.0f),	// 位置
					CWave::SGrow(4.5f, 2.0f, 0.0f),	// 成長量
					CWave::SGrow(0.0f, 0.0f, 0.0f),	// 成長加速量
					80.0f,	// 穴の半径
					1.0f,	// 太さ
					20.0f,	// 外周のY座標加算量
					0.0f,	// 半径の最大成長量
					false	// 当たり判定
				);

				// 自身を終了
				Uninit();

				// 処理を抜ける
				return;
			}
		}

		// プレイヤーとの当たり判定
		CollisionPlayer();

		break;

	default:
		assert(false);
		break;
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

		// 雷の原点位置を設定
		pAttackThunder->m_posOrigin = rPos;

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
void CAttackThunder::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// 雷の位置を設定
	m_apThunder[0]->SetVec3Position(rPos);

	for (int nCntThunder = 0; nCntThunder < attackThunder::NUM_THUNDER - 1; nCntThunder++)
	{ // 雷の生成数分繰り返す

		D3DXVECTOR3 pos = VEC3_ZERO;
		float fRot = ((D3DX_PI * 2.0f) / (float)(attackThunder::NUM_THUNDER - 1)) * nCntThunder - D3DX_PI + ((rand() % 20 - 10) * 0.01f);
		//float fRandDis = 40.0f + (float)(rand() % 11);

		useful::NormalizeRot(fRot);

		pos.x = sinf(fRot) * 40.0f;
		pos.z = cosf(fRot) * 40.0f;

		// 雷の位置を設定
		int nSetID = nCntThunder + 1;
		m_apThunder[nSetID]->SetVec3Position(pos);
	}
}

//============================================================
//	攻撃開始の設定処理
//============================================================
HRESULT CAttackThunder::SetAttack(void)
{
	// 攻撃開始状態にする
	m_state = STATE_ATTACK;

	// 状態管理カウンター
	m_nCounterState = 0;

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

	// 位置を設定
	SetVec3Position(m_posOrigin);

	// 成功を返す
	return S_OK;
}

//============================================================
//	プレイヤーとの当たり判定
//============================================================
void CAttackThunder::CollisionPlayer(void)
{
	CListManager<CPlayer> *pList = CPlayer::GetList();		// プレイヤーリスト
	if (pList == nullptr)		 { assert(false); return; }	// リスト未使用
	if (pList->GetNumAll() != 1) { assert(false); return; }	// プレイヤーが1人じゃない
	auto player = pList->GetList().front();					// プレイヤー情報

	D3DXVECTOR3 posPlayer	= player->GetVec3Position();	// プレイヤーの位置
	float fRadiusPlayer		= player->GetRadius();			// プレイヤーの半径
	float fHeightPlayer		= player->GetHeight();			// プレイヤーの縦幅

	if (collision::Circle2D(m_posOrigin, posPlayer, COLL_RADIUS, fRadiusPlayer))
	{ // 衝撃波に当たっている場合

		if (m_apThunder[0]->GetVec3Position().y < posPlayer.y + fHeightPlayer)
		{ // プレイヤーの頭が雷に当たっている場合

			// プレイヤーのヒット処理
			player->Hit(DMG_THUNDER);
		}
	}
}
