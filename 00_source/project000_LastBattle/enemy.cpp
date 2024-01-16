//============================================================
//
//	敵処理 [enemy.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "enemy.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "scene.h"
#include "sceneGame.h"
#include "stage.h"
#include "player.h"
#include "collision.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int	PRIORITY	= 3;		// 敵の優先順位
	const float	GRAVITY		= 1.0f;		// 重力
	const float	JUMP_REV	= 0.16f;	// 空中の移動量の減衰係数
	const float	LAND_REV	= 0.16f;	// 地上の移動量の減衰係数
	const float	ADD_ALPHA	= 0.03f;	// 透明度の加算量
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CEnemy::SStatusInfo CEnemy::m_aStatusInfo[CEnemy::TYPE_MAX] =	// ステータス情報
{
	// ミニドラゴンのステータス情報
	{
		50.0f,	// 半径
		40.0f,	// 縦幅
		0.25f	// 視認の補正係数
	},
};

//************************************************************
//	子クラス [CEnemy] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEnemy::CEnemy(const EType type) : CObjectModel(CObject::LABEL_ENEMY, CObject::DIM_3D, PRIORITY),
	m_type			(type),					// 種類定数
	m_status		(m_aStatusInfo[type]),	// ステータス定数
	m_oldPos		(VEC3_ZERO),			// 過去位置
	m_movePos		(VEC3_ZERO),			// 位置移動量
	m_moveRot		(VEC3_ZERO),			// 向き変更量
	m_state			(STATE_SPAWN),			// 状態
	m_bJump			(false),				// ジャンプ状況
	m_nCounterState	(0)						// 状態管理カウンター
{

}

//============================================================
//	デストラクタ
//============================================================
CEnemy::~CEnemy()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CEnemy::Init(void)
{
	// メンバ変数を初期化
	m_oldPos	= VEC3_ZERO;	// 過去位置
	m_movePos	= VEC3_ZERO;	// 位置移動量
	m_moveRot	= VEC3_ZERO;	// 向き変更量
	m_state		= STATE_SPAWN;	// 状態
	m_bJump		= false;		// ジャンプ状況
	m_nCounterState	= 0;		// 状態管理カウンター

	// オブジェクトモデルの初期化
	if (FAILED(CObjectModel::Init()))
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
void CEnemy::Uninit(void)
{
	// オブジェクトモデルの終了
	CObjectModel::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CEnemy::Update(void)
{
	// オブジェクトモデルの更新
	CObjectModel::Update();
}

//============================================================
//	描画処理
//============================================================
void CEnemy::Draw(void)
{
	// オブジェクトモデルの描画
	CObjectModel::Draw();
}

#if 0
//============================================================
//	ノックバックヒット処理
//============================================================
void CEnemy::HitKnockBack(const int nDmg, const D3DXVECTOR3&vecKnock)
{
	if (IsDeath())				 { return; }	// 死亡済み
	if (m_state != STATE_NORMAL) { return; }	// 通常状態以外

	// 変数を宣言
	D3DXVECTOR3 posPlayer = GetVec3Position();	// プレイヤー位置
	D3DXVECTOR3 rotPlayer = GetVec3Rotation();	// プレイヤー向き

	// カウンターを初期化
	m_nCounterState = 0;

	// ノックバック移動量を設定
	m_movePos.x = KNOCK_SIDE * vecKnock.x;
	m_movePos.y = KNOCK_UP;
	m_movePos.z = KNOCK_SIDE * vecKnock.z;

	// ノックバック方向に向きを設定
	rotPlayer.y = atan2f(vecKnock.x, vecKnock.z);	// 吹っ飛び向きを計算
	SetVec3Rotation(rotPlayer);	// 向きを設定

	// 空中状態にする
	m_bJump = true;

	// ノック状態を設定
	SetState(STATE_KNOCK);

	// サウンドの再生
	CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_HIT);	// ヒット音
}
#endif

//============================================================
//	生成処理
//============================================================
CEnemy *CEnemy::Create(const EType type, const D3DXVECTOR3& rPos, const D3DXVECTOR3& rRot)
{
	// ポインタを宣言
	CEnemy *pEnemy = nullptr;	// 敵生成用

	if (pEnemy == nullptr)
	{ // 使用されていない場合

		// メモリ確保
		switch (type)
		{ // 種類ごとの処理
		case TYPE_MINI_DRAGON:

			// ミニドラゴンを生成
			pEnemy = new (type);

			break;

		default:	// 例外処理
			assert(false);
			break;
		}
	}
	else { assert(false); return nullptr; }	// 使用中

	if (pEnemy != nullptr)
	{ // 使用されている場合
		
		// 敵の初期化
		if (FAILED(pEnemy->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pEnemy;
			pEnemy = NULL;

			// 失敗を返す
			return NULL;
		}

		// 位置を設定
		pEnemy->SetVec3Position(rPos);

		// 向きを設定
		pEnemy->SetVec3Rotation(rRot);

		// 確保したアドレスを返す
		return pEnemy;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	ステータス情報取得処理
//============================================================
CEnemy::SStatusInfo CEnemy::GetStatusInfo(const int nType)
{
	// 引数の種類のステータス情報を返す
	return m_aStatusInfo[nType];
}

//============================================================
//	過去位置の更新処理
//============================================================
void CEnemy::UpdateOldPosition(void)
{
	// 過去位置を更新
	m_oldPos = GetVec3Position();
}

//============================================================
//	位置移動量の設定処理
//============================================================
void CEnemy::SetMovePosition(const D3DXVECTOR3& rMove)
{
	// 引数の位置移動量を設定
	m_movePos = rMove;
}

//============================================================
//	向き変更量の設定処理
//============================================================
void CEnemy::SetMoveRotation(const D3DXVECTOR3& rMove)
{
	// 引数の向きを設定
	m_moveRot = rMove;

	// 向きの正規化
	useful::Vec3NormalizeRot(m_moveRot);
}

//============================================================
//	状態の設定処理
//============================================================
void CEnemy::SetState(const int nState)
{
	// 引数の状態を設定
	m_state = (EState)nState;
}

//============================================================
//	過去位置取得処理
//============================================================
D3DXVECTOR3 CEnemy::GetOldPosition(void) const
{
	// 過去位置を返す
	return m_oldPos;
}

//============================================================
//	位置移動量取得処理
//============================================================
D3DXVECTOR3 CEnemy::GetMovePosition(void) const
{
	// 位置移動量を返す
	return m_movePos;
}

//============================================================
//	向き変更量取得処理
//============================================================
D3DXVECTOR3 CEnemy::GetMoveRotation(void) const
{
	// 向き変更量を返す
	return m_moveRot;
}

//============================================================
//	状態取得処理
//============================================================
int CEnemy::GetState(void) const
{
	// 状態を返す
	return m_state;
}

//============================================================
//	種類取得処理
//============================================================
int CEnemy::GetType(void) const
{
	// 種類を返す
	return m_type;
}

//============================================================
//	半径取得処理
//============================================================
float CEnemy::GetRadius(void) const
{
	// 半径を返す
	return m_status.fRadius;
}

//============================================================
//	縦幅取得処理
//============================================================
float CEnemy::GetHeight(void) const
{
	// 縦幅を返す
	return m_status.fHeight;
}

//============================================================
//	ステータス情報取得処理
//============================================================
CEnemy::SStatusInfo CEnemy::GetStatusInfo(void) const
{
	// ステータス情報を返す
	return m_status;
}

//============================================================
//	スポーン状態時の更新処理
//============================================================
void CEnemy::UpdateSpawn(void)
{
	// フェードアウト状態時の更新
	if (UpdateFadeOut(ADD_ALPHA))
	{ // 不透明になり切った場合

		// 通常状態にする
		m_state = STATE_NORMAL;
	}
}

//============================================================
//	通常状態時の更新処理
//============================================================
void CEnemy::UpdateNormal(void)
{

}

//============================================================
//	対象の視認処理
//============================================================
void CEnemy::UpdateLook(const D3DXVECTOR3& rPosLook, const D3DXVECTOR3& rPosEnemy, D3DXVECTOR3 *pRotEnemy)
{
	// 変数を宣言
	float fDestRot = 0.0f;	// 目標向き
	float fDiffRot = 0.0f;	// 向き

	// 視認対象の方向を代入
	fDestRot = atan2f(rPosEnemy.x - rPosLook.x, rPosEnemy.z - rPosLook.z);	// 目標向き

	// 差分向きを求める
	fDiffRot = fDestRot - pRotEnemy->y;
	useful::NormalizeRot(fDiffRot);		// 差分向きの正規化

	// 向きの更新
	pRotEnemy->y += fDiffRot * m_status.fLookRev;
	useful::NormalizeRot(pRotEnemy->y);	// 向きの正規化
}

//============================================================
//	フェードアウト状態時の更新処理
//============================================================
bool CEnemy::UpdateFadeOut(const float fAdd)
{
	// 変数を宣言
	bool bAlpha = false;		// 透明状況
	float fAlpha = GetAlpha();	// 透明度

	// プレイヤー自身の描画を再開
	CObject::SetEnableDraw(true);

	// 透明度を上げる
	fAlpha += fAdd;

	if (fAlpha >= GetMaxAlpha())
	{ // 透明度が上がり切った場合

		// 透明度を補正
		fAlpha = GetMaxAlpha();

		// 不透明になり切った状態にする
		bAlpha = true;
	}

	// 透明度を設定
	SetAlpha(fAlpha);

	// 透明状況を返す
	return bAlpha;
}

//============================================================
//	フェードイン状態時の更新処理
//============================================================
bool CEnemy::UpdateFadeIn(const float fSub)
{
	// 変数を宣言
	bool bAlpha = false;		// 透明状況
	float fAlpha = GetAlpha();	// 透明度

	// 透明度を下げる
	fAlpha -= fSub;

	if (fAlpha <= 0.0f)
	{ // 透明度が下がり切った場合

		// 透明度を補正
		fAlpha = 0.0f;

		// 透明になり切った状態にする
		bAlpha = true;

		// プレイヤー自身の描画を停止
		CObject::SetEnableDraw(false);
	}

	// 透明度を設定
	SetAlpha(fAlpha);

	// 透明状況を返す
	return bAlpha;
}
