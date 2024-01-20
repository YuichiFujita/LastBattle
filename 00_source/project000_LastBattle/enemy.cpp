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
#include "stage.h"

#include "enemyBossDragon.h"
#include "enemyMiniDragon.h"

#include "gauge3D.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	// 基本情報
	const int	PRIORITY	= 3;		// 敵の優先順位
	const float	GRAVITY		= 1.0f;		// 重力
	const float	JUMP_REV	= 0.16f;	// 空中の移動量の減衰係数
	const float	LAND_REV	= 0.16f;	// 地上の移動量の減衰係数
	const float	ADD_ALPHA	= 0.03f;	// 透明度の加算量

	const int	DAMAGE_FRAME	= 4;		// ダメージ状態の維持フレーム
	const int	INVULN_FRAME	= 12;		// 無敵状態の維持フレーム
	const float	INVULN_ALPHA	= 0.7f;		// 無敵状態の基礎透明度
	const float	ADD_SINROT		= 0.1f;		// 透明度をふわふわさせる際のサインカーブ向き加算量
	const float	MAX_ADD_ALPHA	= 0.25f;	// 透明度の最大加算量

	// 体力の情報
	namespace lifeInfo
	{
		const D3DXVECTOR3	GAUGE_SIZE	= D3DXVECTOR3(200.0f, 20.0f, 0.0f);	// ゲージの大きさ
		const D3DXCOLOR		FRONT_COL	= XCOL_YELLOW;	// 表ゲージ色
		const D3DXCOLOR		BACK_COL	= XCOL_RED;		// 裏ゲージ色

		const int	CHANGE_FRAME		= 10;		// 表示値変動フレーム
		const float	PLUS_HEIGHT_POSY	= 20.0f;	// 体力表示Y座標の身長からの加算量
	}
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CListManager<CEnemy> *CEnemy::m_pList = nullptr;	// オブジェクトリスト
CEnemy::SStatusInfo CEnemy::m_aStatusInfo[CEnemy::TYPE_MAX] =	// ステータス情報
{
	// ボスドラゴンのステータス情報
	{
		50,		// 最大体力
		250.0f,	// 半径
		500.0f,	// 縦幅
		0.25f,	// 視認の補正係数
		250.0f,	// 判定半径
	},

	// ミニドラゴンのステータス情報
	{
		20,		// 最大体力
		45.0f,	// 半径
		100.0f,	// 縦幅
		0.25f,	// 視認の補正係数
		75.0f,	// 判定半径
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
	m_pLife			(nullptr),				// 体力の情報
	m_oldPos		(VEC3_ZERO),			// 過去位置
	m_move			(VEC3_ZERO),			// 移動量
	m_state			(STATE_SPAWN),			// 状態
	m_fSinAlpha		(0.0f),					// 透明向き
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
	m_pLife		= nullptr;		// 体力の情報
	m_oldPos	= VEC3_ZERO;	// 過去位置
	m_move		= VEC3_ZERO;	// 移動量
	m_state		= STATE_SPAWN;	// 状態
	m_fSinAlpha = 0.0f;			// 透明向き
	m_bJump		= false;		// ジャンプ状況
	m_nCounterState	= 0;		// 状態管理カウンター

	// オブジェクトモデルの初期化
	if (FAILED(CObjectModel::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 体力の生成
	float fPosUp = m_status.fHeight + lifeInfo::PLUS_HEIGHT_POSY;	// 表示Y位置の加算量
	m_pLife = CGauge3D::Create
	( // 引数
		this,					// ゲージ表示オブジェクト
		m_status.nMaxLife,		// 最大表示値
		lifeInfo::CHANGE_FRAME,	// 表示値変動フレーム
		fPosUp,					// 表示Y位置の加算量
		lifeInfo::GAUGE_SIZE,	// ゲージ大きさ
		lifeInfo::FRONT_COL,	// 表ゲージ色
		lifeInfo::BACK_COL		// 裏ゲージ色
	);

	if (m_pList == nullptr)
	{ // リストマネージャーが存在しない場合

		// リストマネージャーの生成
		m_pList = CListManager<CEnemy>::Create();
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
void CEnemy::Uninit(void)
{
	// 自身の終了を伝える
	m_pLife->DeleteGauge();

	// リストから自身のオブジェクトを削除
	m_pList->DeleteList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}

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

//============================================================
//	状態の設定処理
//============================================================
void CEnemy::SetState(const int nState)
{
	// 引数の状態を設定
	m_state = (EState)nState;

	switch (m_state)
	{ // 状態ごとの処理
	case STATE_NORMAL:	// 通常状態
	case STATE_DAMAGE:	// ダメージ状態
	case STATE_DEATH:	// 死亡状態
		break;

	case STATE_SPAWN:	// スポーン状態

		// スポーン状態の設定
		SetSpawn();

		break;

	case STATE_INVULN:	// 無敵状態

		// 無敵状態の設定
		SetInvuln();

		break;

	default:	// 例外処理
		assert(false);
		break;
	}
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
//	生成処理
//============================================================
CEnemy *CEnemy::Create(const EType type, const D3DXVECTOR3& rPos, const D3DXVECTOR3& rRot)
{
	// ポインタを宣言
	CEnemy *pEnemy = nullptr;	// 敵情報

	// 敵の生成
	switch (type)
	{ // 種類ごとの処理
	case TYPE_BOSS_DRAGON:

		// ボスドラゴンを生成
		pEnemy = new CEnemyBossDragon(type);

		break;

	case TYPE_MINI_DRAGON:

		// ミニドラゴンを生成
		pEnemy = new CEnemyMiniDragon(type);

		break;

	default:	// 例外処理
		assert(false);
		break;
	}

	if (pEnemy == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// 敵の初期化
		if (FAILED(pEnemy->Init()))
		{ // 初期化に失敗した場合

			// 敵の破棄
			SAFE_DELETE(pEnemy);
			return nullptr;
		}

		// 位置を設定
		pEnemy->SetVec3Position(rPos);

		// 向きを設定
		pEnemy->SetVec3Rotation(rRot);

		// 確保したアドレスを返す
		return pEnemy;
	}
}

//============================================================
//	リスト取得処理
//============================================================
CListManager<CEnemy> *CEnemy::GetList(void)
{
	// オブジェクトリストを返す
	return m_pList;
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
//	ヒット処理
//============================================================
void CEnemy::Hit(const int nDamage)
{
	if (IsDeath())				 { return; }	// 死亡済み
	if (m_state != STATE_NORMAL) { return; }	// 通常状態以外
	if (m_pLife->GetNum() <= 0)	 { return; }	// 体力なし

	// 変数を宣言
	D3DXVECTOR3 posEnemy = GetVec3Position();	// 敵位置
	D3DXVECTOR3 rotEnemy = GetVec3Rotation();	// 敵向き

	// 体力にダメージを与える
	m_pLife->AddNum(-nDamage);

	if (m_pLife->GetNum() > 0)
	{ // 体力が残っている場合

		// ダメージ状態にする
		SetState(STATE_DAMAGE);
	}
	else
	{ // 体力が残っていない場合

		// 死亡状態にする
		SetState(STATE_DEATH);
	}
}

//============================================================
//	ノックバックヒット処理
//============================================================
void CEnemy::HitKnockBack(const int /*nDamage*/, const D3DXVECTOR3& /*vecKnock*/)
{
#if 0

	if (IsDeath())				 { return; }	// 死亡済み
	if (m_state != STATE_NORMAL) { return; }	// 通常状態以外

	// 変数を宣言
	D3DXVECTOR3 posPlayer = GetVec3Position();	// プレイヤー位置
	D3DXVECTOR3 rotPlayer = GetVec3Rotation();	// プレイヤー向き

	// カウンターを初期化
	m_nCounterState = 0;

	// ノックバック移動量を設定
	m_move.x = KNOCK_SIDE * vecKnock.x;
	m_move.y = KNOCK_UP;
	m_move.z = KNOCK_SIDE * vecKnock.z;

	// ノックバック方向に向きを設定
	rotPlayer.y = atan2f(vecKnock.x, vecKnock.z);	// 吹っ飛び向きを計算
	SetVec3Rotation(rotPlayer);	// 向きを設定

	// 空中状態にする
	m_bJump = true;

	// ノック状態を設定
	SetState(STATE_KNOCK);

	// サウンドの再生
	CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_HIT);	// ヒット音

#endif
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
//	過去位置取得処理
//============================================================
D3DXVECTOR3 CEnemy::GetOldPosition(void) const
{
	// 過去位置を返す
	return m_oldPos;
}

//============================================================
//	移動量の設定処理
//============================================================
void CEnemy::SetMovePosition(const D3DXVECTOR3 &rMove)
{
	// 引数の移動量を設定
	m_move = rMove;
}

//============================================================
//	移動量取得処理
//============================================================
D3DXVECTOR3 CEnemy::GetMovePosition(void) const
{
	// 移動量を返す
	return m_move;
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
//	スポーン状態の設定処理
//============================================================
void CEnemy::SetSpawn(void)
{
	// カウンターを初期化
	m_nCounterState = 0;	// 状態管理カウンター

	// 移動量を初期化
	m_move = VEC3_ZERO;

	// 描画を再開
	SetEnableDraw(true);

	// マテリアルを再設定
	ResetMaterial();

	// 透明度を透明に再設定
	SetAlpha(0.0f);
}

//============================================================
//	無敵状態の設定処理
//============================================================
void CEnemy::SetInvuln(void)
{
	// カウンターを初期化
	m_nCounterState = 0;	// 状態管理カウンター

	// 透明向きを初期化
	m_fSinAlpha = 0.0f;

	// 描画を再開
	SetEnableDraw(true);

	// マテリアルを再設定
	ResetMaterial();

	// 無敵の基礎透明度を設定
	SetAlpha(INVULN_ALPHA);
}

//============================================================
//	スポーン状態時の更新処理
//============================================================
void CEnemy::UpdateSpawn(void)
{
	// 変数を宣言
	D3DXVECTOR3 posEnemy = GetVec3Position();	// 敵位置
	D3DXVECTOR3 rotEnemy = GetVec3Rotation();	// 敵向き

	// ポインタを宣言
	CStage *pStage = CScene::GetStage();				// ステージ情報
	if (pStage == nullptr) { assert(false); return; }	// ステージ非使用中

	// 位置更新
	UpdatePosition(&posEnemy);

	// 着地判定
	UpdateLanding(&posEnemy);

	// ステージ範囲外の補正
	pStage->LimitPosition(posEnemy, m_status.fRadius);

	// 位置を反映
	SetVec3Position(posEnemy);

	// 向きを反映
	SetVec3Rotation(rotEnemy);

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
	// 変数を宣言
	D3DXVECTOR3 posEnemy = GetVec3Position();	// 敵位置
	D3DXVECTOR3 rotEnemy = GetVec3Rotation();	// 敵向き

	// ポインタを宣言
	CStage *pStage = CScene::GetStage();				// ステージ情報
	if (pStage == nullptr) { assert(false); return; }	// ステージ非使用中

	// 位置更新
	UpdatePosition(&posEnemy);

	// 着地判定
	UpdateLanding(&posEnemy);

	// ステージ範囲外の補正
	pStage->LimitPosition(posEnemy, m_status.fRadius);

	// 位置を反映
	SetVec3Position(posEnemy);

	// 向きを反映
	SetVec3Rotation(rotEnemy);
}

//============================================================
//	ダメージ状態時の更新処理
//============================================================
void CEnemy::UpdateDamage(void)
{
	// 身体の色を赤くする
	SetAllMaterial(material::DamageRed());

	// 通常動作の更新
	UpdateNormal();

	// カウンターを加算
	m_nCounterState++;
	if (m_nCounterState > DAMAGE_FRAME)
	{ // ダメージ状態の終了フレームになった場合

		// カウンターを初期化
		m_nCounterState = 0;

		// 身体の色を元に戻す
		ResetMaterial();

		// 無敵状態にする
		SetState(STATE_INVULN);
	}
}

//============================================================
//	無敵状態時の更新処理
//============================================================
void CEnemy::UpdateInvuln(void)
{
	// 変数を宣言
	float fAddAlpha = 0.0f;	// 透明度の加算量

	// サインカーブ向きを回転
	m_fSinAlpha += ADD_SINROT;
	useful::NormalizeRot(m_fSinAlpha);	// 向き正規化

	// 透明度加算量を求める
	fAddAlpha = (MAX_ADD_ALPHA / 2.0f) * (sinf(m_fSinAlpha) - 1.0f);

	// 透明度を設定
	SetAlpha(INVULN_ALPHA + fAddAlpha);

	// 通常動作の更新
	UpdateNormal();

	// カウンターを加算
	m_nCounterState++;
	if (m_nCounterState > INVULN_FRAME)
	{ // 無敵状態の終了フレームになった場合

		// カウンターを初期化
		m_nCounterState = 0;

		// 不透明にする
		SetAlpha(1.0f);

		// 通常状態を設定
		SetState(STATE_NORMAL);
	}
}

//============================================================
//	死亡状態時の更新処理
//============================================================
void CEnemy::UpdateDeath(void)
{
	// 自身の終了
	Uninit();
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
//	重力の更新処理
//============================================================
void CEnemy::UpdateGravity(void)
{
	// 重力を加算
	m_move.y -= GRAVITY;
}

//============================================================
//	着地状況の更新処理
//============================================================
bool CEnemy::UpdateLanding(D3DXVECTOR3 *pPos)
{
	// 変数を宣言
	bool bLand = false;	// 着地状況

	// ジャンプしている状態にする
	m_bJump = true;

	// 地面・制限位置の着地判定
	if (CScene::GetStage()->LandFieldPosition(*pPos, m_move)
	||  CScene::GetStage()->LandLimitPosition(*pPos, m_move, 0.0f))
	{ // プレイヤーが着地していた場合

		// 着地している状態にする
		bLand = true;

		// ジャンプしていない状態にする
		m_bJump = false;
	}

	// 着地状況を返す
	return bLand;
}

//============================================================
//	位置の更新処理
//============================================================
void CEnemy::UpdatePosition(D3DXVECTOR3 *pPos)
{
	// 位置を移動
	*pPos += m_move;

	// 移動量を減衰
	if (m_bJump)
	{ // 空中の場合

		m_move.x += (0.0f - m_move.x) * JUMP_REV;
		m_move.z += (0.0f - m_move.z) * JUMP_REV;
	}
	else
	{ // 地上の場合

		m_move.x += (0.0f - m_move.x) * LAND_REV;
		m_move.z += (0.0f - m_move.z) * LAND_REV;
	}
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
