//============================================================
//
//	プレイヤー処理 [player.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "player.h"
#include "manager.h"
#include "sceneGame.h"
#include "gameManager.h"
#include "renderer.h"
#include "sound.h"
#include "camera.h"
#include "texture.h"
#include "collision.h"
#include "fade.h"
#include "retentionManager.h"

#include "objectChara.h"
#include "multiModel.h"
#include "blur.h"
#include "sword.h"
#include "swordWaveManager.h"
#include "gauge2D.h"
#include "shadow.h"
#include "stage.h"
#include "field.h"
#include "enemy.h"

#include "effect3D.h"
#include "particle3D.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char *LOWER_MODEL_FILE[] =	// 下半身モデルファイル
	{
		"data\\MODEL\\PLAYER\\00_waist.x",	// 腰
		"data\\MODEL\\PLAYER\\09_legUL.x",	// 左太もも
		"data\\MODEL\\PLAYER\\10_legUR.x",	// 右太もも
		"data\\MODEL\\PLAYER\\11_legDL.x",	// 左脛
		"data\\MODEL\\PLAYER\\12_legDR.x",	// 右脛
		"data\\MODEL\\PLAYER\\13_footL.x",	// 左足
		"data\\MODEL\\PLAYER\\14_footR.x",	// 右足
	};
	const char *UPPER_MODEL_FILE[] =	// 上半身モデルファイル
	{
		"data\\MODEL\\PLAYER\\01_body.x",	// 体
		"data\\MODEL\\PLAYER\\02_head.x",	// 頭
		"data\\MODEL\\PLAYER\\03_armUL.x",	// 左上腕
		"data\\MODEL\\PLAYER\\04_armUR.x",	// 右上腕
		"data\\MODEL\\PLAYER\\05_armDL.x",	// 左下腕
		"data\\MODEL\\PLAYER\\06_armDR.x",	// 右下腕
		"data\\MODEL\\PLAYER\\07_handL.x",	// 左手
		"data\\MODEL\\PLAYER\\08_handR.x",	// 右手
	};

	const char *SETUP_TXT[] =	// セットアップテキスト相対パス
	{
		"data\\TXT\\playerMotionLower.txt",	// 下半身
		"data\\TXT\\playerMotionUpper.txt",	// 上半身
	};
	const char **MODEL_PASS[] =	// モデル最大数
	{
		&LOWER_MODEL_FILE[0],	// 下半身
		&UPPER_MODEL_FILE[0],	// 上半身
	};
	const D3DXVECTOR3 SWORD_OFFSET[] =	// 剣のオフセット位置
	{
		D3DXVECTOR3(-5.8f, 0.0f, 0.0f),	// 右剣
		D3DXVECTOR3(5.8f, 0.0f, 0.0f),	// 左剣
	};
	const D3DXVECTOR3 SPAWN_POS = D3DXVECTOR3(0.0f, 0.0f, -600.0f);	// スポーン位置
	const D3DXVECTOR3 SPAWN_ROT = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);	// スポーン向き

	const int	PRIORITY	= 3;		// プレイヤーの優先順位
	const float	INIT_JUMP	= 4.0f;		// ジャンプ量の初期値
	const float	PULS_JUMP	= 4.15f;	// ジャンプ高度上昇量
	const float	GRAVITY		= 0.85f;	// 重力
	const float	RADIUS		= 20.0f;	// 半径
	const float	HEIGHT		= 80.0f;	// 縦幅
	const float	REV_ROTA	= 0.15f;	// 向き変更の補正係数
	const float	MOVE_REV	= 0.5f;		// 移動量の補正係数
	const float	DODGE_REV	= 0.12f;	// 回避の移動量の減衰係数
	const float	JUMP_REV	= 0.16f;	// 空中の移動量の減衰係数
	const float	LAND_REV	= 0.16f;	// 地上の移動量の減衰係数
	const float	STICK_REV	= 0.00015f;	// 移動操作スティックの傾き量の補正係数
	const float	ADD_ALPHA	= 0.03f;	// 透明度の加算量
	const float	LOOK_RADIUS	= 500.0f;	// 敵の方向を向かせる際の敵検知半径
	const float	LOOK_REV	= 0.5f;		// 敵の方向を向かせる際の補正係数
	const int	WALK_SOUND	= 4;		// 歩行の効果音のキータイミング

	const int	DAMAGE_FRAME	= 14;		// ダメージ状態の維持フレーム
	const int	INVULN_FRAME	= 120;		// 無敵状態の維持フレーム
	const float	INVULN_ALPHA	= 0.7f;		// 無敵状態の基礎透明度
	const float	ADD_SINROT		= 0.2f;		// 透明度をふわふわさせる際のサインカーブ向き加算量
	const float	MAX_ADD_ALPHA	= 0.25f;	// 透明度の最大加算量
	const float	DODGE_REV_MOVE	= 0.065f;	// 回避移動量の減算係数
	const float	DODGE_MIN_MOVE	= 0.35f;	// 回避移動量の最小値
	const float	DODGE_SIDE_MOVE	= 3.2f;		// 回避の横移動量
	const float	DODGE_JUMP_MOVE	= 9.0f;		// 回避の上移動量
	const int	DODGE_WAIT_FRAME	= 100;	// 回避のフールタイムフレーム
	const int	PRESS_JUMP_FRAME	= 10;	// ジャンプ高度上昇の受付入力時間
	const int	ATTACK_BUFFER_FRAME = 8;	// 攻撃の先行入力可能フレーム

	const D3DXVECTOR3 SHADOW_SIZE	= D3DXVECTOR3(80.0f, 0.0f, 80.0f);		// 影の大きさ
	const char *TEXTURE_LIFEFRAME	= "data\\TEXTURE\\lifeframe002.png";	// 体力フレーム表示のテクスチャファイル

	// 体力の情報
	namespace lifeInfo
	{
		const D3DXVECTOR3	POS			= D3DXVECTOR3(260.0f, 170.0f, 0.0f);		// 位置
		const D3DXVECTOR3	SIZE_GAUGE	= D3DXVECTOR3(200.0f, 20.0f, 0.0f);			// ゲージ大きさ
		const D3DXCOLOR		COL_FRONT	= D3DXCOLOR(0.93f, 0.92f, 0.25f, 1.0f);		// 表ゲージ色
		const D3DXCOLOR		COL_BACK	= D3DXCOLOR(0.03f, 0.03f, 0.008f, 1.0f);	// 裏ゲージ色
		const int	MAX_LIFE		= 100;	// 最大表示値
		const int	CHANGE_FRAME	= 40;	// 表示値変動フレーム
	}

	// ブラーの情報
	namespace blurInfo
	{
		const float	START_ALPHA	= 0.4f;	// ブラー開始透明度
		const int	MAX_LENGTH	= 12;	// 保持オブジェクト最大数
	}
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CListManager<CPlayer> *CPlayer::m_pList = nullptr;			// オブジェクトリスト
CPlayer::AFuncUpdateMotion CPlayer::m_aFuncUpdateMotion[] =	// モーション更新関数
{
	&CPlayer::UpdateMotionLower,	// 下半身モーションの更新
	&CPlayer::UpdateMotionUpper,	// 上半身モーションの更新
};

//************************************************************
//	スタティックアサート
//************************************************************
static_assert(NUM_ARRAY(LOWER_MODEL_FILE) == CPlayer::L_MODEL_MAX, "ERROR : Model Count Mismatch");
static_assert(NUM_ARRAY(UPPER_MODEL_FILE) == CPlayer::U_MODEL_MAX, "ERROR : Model Count Mismatch");
static_assert(NUM_ARRAY(SETUP_TXT)    == CPlayer::BODY_MAX, "ERROR : Body Count Mismatch");
static_assert(NUM_ARRAY(SWORD_OFFSET) == player::NUM_SWORD, "ERROR : Body Count Mismatch");

//************************************************************
//	子クラス [CPlayer] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CPlayer::CPlayer() : CObjectDivChara(CObject::LABEL_PLAYER, CObject::DIM_3D, PRIORITY),
	m_pLife			(nullptr),		// 体力の情報
	m_pShadow		(nullptr),		// 影の情報
	m_oldPos		(VEC3_ZERO),	// 過去位置
	m_move			(VEC3_ZERO),	// 移動量
	m_destRot		(VEC3_ZERO),	// 目標向き
	m_state			(STATE_NONE),	// 状態
	m_fSinAlpha		(0.0f),			// 透明向き
	m_nCounterState	(0)				// 状態管理カウンター
{
	// メンバ変数をクリア
	memset(&m_apSowrd,	0, sizeof(m_apSowrd));	// 剣の情報
	memset(&m_apBlur,	0, sizeof(m_apBlur));	// ブラーの情報
	memset(&m_jump,		0, sizeof(m_jump));		// ジャンプの情報
	memset(&m_dodge,	0, sizeof(m_dodge));	// 回避の情報
	memset(&m_attack,	0, sizeof(m_attack));	// 攻撃の情報
}

//============================================================
//	デストラクタ
//============================================================
CPlayer::~CPlayer()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CPlayer::Init(void)
{
	// メンバ変数を初期化
	memset(&m_apSowrd,	0, sizeof(m_apSowrd));	// 剣の情報
	memset(&m_apBlur,	0, sizeof(m_apBlur));	// ブラーの情報
	memset(&m_jump,		0, sizeof(m_jump));		// ジャンプの情報
	memset(&m_dodge,	0, sizeof(m_dodge));	// 回避の情報
	memset(&m_attack,	0, sizeof(m_attack));	// 攻撃の情報
	m_pLife			= nullptr;		// 体力の情報
	m_pShadow		= nullptr;		// 影の情報
	m_oldPos		= VEC3_ZERO;	// 過去位置
	m_move			= VEC3_ZERO;	// 移動量
	m_destRot		= VEC3_ZERO;	// 目標向き
	m_state			= STATE_NONE;	// 状態
	m_fSinAlpha		= 0.0f;			// 透明向き
	m_nCounterState	= 0;			// 状態管理カウンター

	// オブジェクト分割キャラクターの初期化
	if (FAILED(CObjectDivChara::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	for (int nCntSword = 0; nCntSword < player::NUM_SWORD; nCntSword++)
	{ // 剣の数分繰り返す

		// 剣の生成
		m_apSowrd[nCntSword] = CSword::Create(nullptr, SWORD_OFFSET[nCntSword]);
		if (m_apSowrd[nCntSword] == nullptr)
		{ // 非使用中の場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}
	}

	// セットアップの読込
	LoadSetup(BODY_LOWER, MODEL_PASS[BODY_LOWER]);
	LoadSetup(BODY_UPPER, MODEL_PASS[BODY_UPPER]);

	// モデル情報の設定
	SetModelInfo();

	// 上半身の親インデックスの設定
	SetUpperParentID(L_MODEL_WAIST);

	for (int nCntSword = 0; nCntSword < player::NUM_SWORD; nCntSword++)
	{ // 剣の数分繰り返す

		// 親オブジェクト (持ち手) の設定
		m_apSowrd[nCntSword]->SetParentObject(GetMultiModel(BODY_UPPER, U_MODEL_HANDL + nCntSword));
	}

	for (int nCntBlur = 0; nCntBlur < BODY_MAX; nCntBlur++)
	{ // ブラーの数分繰り返す

		// ブラーの情報
		D3DXMATERIAL mat = material::GlowCyan();	// ブラーマテリアル
		m_apBlur[nCntBlur] = CBlur::Create
		( // 引数
			GetObjectChara((CObjectDivChara::EBody)nCntBlur),	// 親オブジェクト
			mat,					// ブラーマテリアル
			blurInfo::START_ALPHA,	// ブラー開始透明度
			blurInfo::MAX_LENGTH	// 保持オブジェクト最大数
		);
		if (m_apBlur[nCntBlur] == nullptr)
		{ // 非使用中の場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}
	}

	// 体力の生成
	m_pLife = CGauge2D::Create
	( // 引数
		lifeInfo::MAX_LIFE,		// 最大表示値
		lifeInfo::CHANGE_FRAME,	// 表示値変動フレーム
		lifeInfo::POS,			// 位置
		lifeInfo::SIZE_GAUGE,	// ゲージ大きさ
		lifeInfo::COL_FRONT,	// 表ゲージ色
		lifeInfo::COL_BACK,		// 裏ゲージ色
		true,
		TEXTURE_LIFEFRAME,
		lifeInfo::SIZE_GAUGE + D3DXVECTOR3(16.5f, 16.5f, 0.0f)
	);

	// 影の生成
	m_pShadow = CShadow::Create(CShadow::TEXTURE_NORMAL, SHADOW_SIZE, this);
	if (m_pShadow == nullptr)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 自動描画をOFFにする
	SetEnableDraw(false);

	if (m_pList == nullptr)
	{ // リストマネージャーが存在しない場合

		// リストマネージャーの生成
		m_pList = CListManager<CPlayer>::Create();
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
void CPlayer::Uninit(void)
{
	for (int nCntSword = 0; nCntSword < player::NUM_SWORD; nCntSword++)
	{ // 剣の数分繰り返す

		// 剣の終了
		SAFE_UNINIT(m_apSowrd[nCntSword]);
	}

	// 影の終了
	m_pShadow->DeleteObjectParent();	// 親オブジェクトを削除
	SAFE_UNINIT(m_pShadow);

	// リストから自身のオブジェクトを削除
	m_pList->DeleteList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}

	// オブジェクト分割キャラクターの終了
	CObjectDivChara::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CPlayer::Update(void)
{
	// 変数を宣言
	ELowerMotion curLowMotion = L_MOTION_IDOL;	// 現在の下半身モーション
	EUpperMotion curUpMotion  = U_MOTION_IDOL;	// 現在の上半身モーション

	// オブジェクト分割キャラクターの更新
	CObjectDivChara::Update();

	// 過去位置の更新
	UpdateOldPosition();

	// 重力の更新
	UpdateGravity();

	switch (m_state)
	{ // 状態ごとの処理
	case STATE_NONE:
		break;

	case STATE_SPAWN:

		// スポーン状態時の更新
		UpdateSpawn();

		break;

	case STATE_NORMAL:

		// 通常状態の更新
		UpdateNormal((int*)&curLowMotion, (int*)&curUpMotion);

		break;

	case STATE_DAMAGE:

		// 通常状態の更新
		UpdateDamage((int*)&curLowMotion, (int*)&curUpMotion);

		break;

	case STATE_INVULN:

		// 無敵状態の更新
		UpdateInvuln((int*)&curLowMotion, (int*)&curUpMotion);

		break;

	case STATE_DEATH:

		// 死亡モーションにする
		curLowMotion = L_MOTION_DEATH;
		curUpMotion  = U_MOTION_DEATH;

		break;

	default:
		assert(false);
		break;
	}

	bool bColl[] =	// 判定ON/OFF状況
	{
		IsLeftWeaponCollision(BODY_UPPER),	// 左手の武器の攻撃判定
		IsRightWeaponCollision(BODY_UPPER),	// 右手の武器の攻撃判定
	};
	for (int nCntSword = 0; nCntSword < player::NUM_SWORD; nCntSword++)
	{ // 剣の数分繰り返す

		// 剣の攻撃判定を設定
		m_apSowrd[nCntSword]->SetEnableAttack(bColl[nCntSword]);

		// 剣の更新
		m_apSowrd[nCntSword]->Update();
	}

	// 影の更新
	m_pShadow->Update();

	// モーションの更新
	UpdateMotion(curLowMotion, curUpMotion);
}

//============================================================
//	描画処理
//============================================================
void CPlayer::Draw(CShader *pShader)
{
	CToonShader	*pToonShader = CToonShader::GetInstance();	// トゥーンシェーダー情報
	if (pToonShader->IsEffectOK())
	{ // エフェクトが使用可能な場合

		// オブジェクト分割キャラクターの描画
		CObjectDivChara::Draw(pToonShader);

		for (int nCntSword = 0; nCntSword < player::NUM_SWORD; nCntSword++)
		{ // 剣の数分繰り返す

			// 剣の描画
			m_apSowrd[nCntSword]->Draw(pToonShader);
		}
	}
	else
	{ // エフェクトが使用不可能な場合

		// エフェクトエラー
		assert(false);

		// オブジェクト分割キャラクターの描画
		CObjectDivChara::Draw(pShader);

		for (int nCntSword = 0; nCntSword < player::NUM_SWORD; nCntSword++)
		{ // 剣の数分繰り返す

			// 剣の描画
			m_apSowrd[nCntSword]->Draw(pShader);
		}
	}
}

//============================================================
//	状態の設定処理
//============================================================
void CPlayer::SetState(const int nState)
{
	if (nState > NONE_IDX && nState < STATE_MAX)
	{ // 範囲内の場合

		// 引数の状態を設定
		m_state = (EState)nState;
	}
	else { assert(false); }
}

//============================================================
//	状態取得処理
//============================================================
int CPlayer::GetState(void) const
{
	// 状態を返す
	return m_state;
}

//============================================================
//	半径取得処理
//============================================================
float CPlayer::GetRadius(void) const
{
	// 半径を返す
	return RADIUS;
}

//============================================================
//	縦幅取得処理
//============================================================
float CPlayer::GetHeight(void) const
{
	// 縦幅を返す
	return HEIGHT;
}

//============================================================
//	更新状況の設定処理
//============================================================
void CPlayer::SetEnableUpdate(const bool bUpdate)
{
	// 引数の更新状況を設定
	CObject::SetEnableUpdate(bUpdate);		// 自身
	m_pShadow->SetEnableUpdate(bUpdate);	// 影
}

//============================================================
//	描画状況の設定処理
//============================================================
void CPlayer::SetEnableDraw(const bool bDraw)
{
	// 引数の描画状況を設定
	CObject::SetEnableDraw(bDraw);		// 自身
	m_pShadow->SetEnableDraw(bDraw);	// 影
}

//============================================================
//	マトリックス計算結果の取得処理
//============================================================
D3DXMATRIX CPlayer::CalcMtxWorld(void) const
{
	// 変数を宣言
	D3DXMATRIX  mtxRot, mtxTrans, mtxWorld;		// 計算用マトリックス
	D3DXVECTOR3 posPlayer = GetVec3Position();	// プレイヤー位置
	D3DXVECTOR3 rotPlayer = GetVec3Rotation();	// プレイヤー向き

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rotPlayer.y, rotPlayer.x, rotPlayer.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, posPlayer.x, posPlayer.y, posPlayer.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	// ワールドマトリックスを返す
	return mtxWorld;
}

//============================================================
//	生成処理
//============================================================
CPlayer *CPlayer::Create(CScene::EMode mode)
{
	// ポインタを宣言
	CPlayer *pPlayer = nullptr;	// プレイヤー情報

	// プレイヤーの生成
	switch (mode)
	{ // モードごとの処理
	case CScene::MODE_TITLE:
	case CScene::MODE_RESULT:
	case CScene::MODE_RANKING:

		break;

	case CScene::MODE_TUTORIAL:
	case CScene::MODE_GAME:

		// メモリ確保
		pPlayer = new CPlayer;	// プレイヤー

		break;

	default:	// 例外処理
		assert(false);
		break;
	}

	if (pPlayer == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// プレイヤーの初期化
		if (FAILED(pPlayer->Init()))
		{ // 初期化に失敗した場合

			// プレイヤーの破棄
			SAFE_DELETE(pPlayer);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pPlayer;
	}
}

//============================================================
//	リスト取得処理
//============================================================
CListManager<CPlayer> *CPlayer::GetList(void)
{
	// オブジェクトリストを返す
	return m_pList;
}

//============================================================
//	UI描画の設定処理
//============================================================
void CPlayer::SetEnableDrawUI(const bool bDraw)
{
	// UIオブジェクトに描画状況を反映
	m_pLife->SetEnableDraw(bDraw);	// 体力
}

//============================================================
//	ヒット処理
//============================================================
void CPlayer::Hit(const int nDamage)
{
	if (IsDeath())				 { return; }	// 死亡済み
	if (m_state != STATE_NORMAL) { return; }	// 通常状態以外
	if (m_pLife->GetNum() <= 0)	 { return; }	// 体力なし

	// 変数を宣言
	D3DXVECTOR3 posPlayer = GetVec3Position();	// プレイヤー位置
	D3DXVECTOR3 rotPlayer = GetVec3Rotation();	// プレイヤー向き

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

		if (GET_MANAGER->GetMode() == CScene::MODE_GAME)
		{ // ゲーム画面の場合

			// リザルト画面に遷移させる
			CSceneGame::GetGameManager()->TransitionResult(CRetentionManager::WIN_FAILED);
		}
	}
}

//============================================================
//	ノックバックヒット処理
//============================================================
void CPlayer::HitKnockBack(const int /*nDamage*/, const D3DXVECTOR3 & /*vecKnock*/)
{
#if 0

	if (IsDeath()) { return; }	// 死亡済み
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
//	通常状態の初期化処理
//============================================================
void CPlayer::InitNormal(void)
{
	// 変数を宣言
	D3DXVECTOR3 posPlayer = GetVec3Position();	// プレイヤー位置

	// ポインタを宣言
	CStage *pStage = CScene::GetStage();				// ステージ情報
	if (pStage == nullptr) { assert(false); return; }	// ステージ非使用中

	// 通常状態にする
	SetState(STATE_NORMAL);

	// カウンターを初期化
	m_nCounterState = 0;	// 状態管理カウンター

	// 移動量を初期化
	m_move = VEC3_ZERO;

	// 自動描画をONにする
	SetEnableDraw(true);

	// 透明度を不透明に再設定
	SetAlpha(1.0f);

	// マテリアルを再設定
	ResetMaterial();

	// 待機モーションを設定
	SetMotion(BODY_LOWER, L_MOTION_IDOL);
	SetMotion(BODY_UPPER, U_MOTION_IDOL);

	// 着地判定
	UpdateLanding(&posPlayer);

	// ステージ範囲外の補正
	pStage->LimitPosition(posPlayer, RADIUS);

	// 位置を反映
	SetVec3Position(posPlayer);

	for (int nCntSword = 0; nCntSword < player::NUM_SWORD; nCntSword++)
	{ // 剣の数分繰り返す

		// 剣の自動描画をOFFにする
		m_apSowrd[nCntSword]->SetState(CSword::STATE_NONE);
	}
}

//============================================================
//	スポーンの設定処理
//============================================================
void CPlayer::SetSpawn(void)
{
	// 変数を宣言
	D3DXVECTOR3 setPos = SPAWN_POS;	// 位置設定用
	D3DXVECTOR3 setRot = SPAWN_ROT;	// 向き設定用

	// スポーン状態にする
	SetState(STATE_SPAWN);

	// カウンターを初期化
	m_nCounterState = 0;	// 状態管理カウンター

	// 着地判定
	UpdateLanding(&setPos);

	// 位置を設定
	SetVec3Position(setPos);
	m_oldPos = setPos;

	// 向きを設定
	SetVec3Rotation(setRot);
	m_destRot = setRot;

	// 移動量を初期化
	m_move = VEC3_ZERO;

	// 自動描画をONにする
	SetEnableDraw(true);

	// マテリアルを再設定
	ResetMaterial();

	// 出現モーションを設定
	SetMotion(BODY_LOWER, L_MOTION_SPAWN);
	SetMotion(BODY_UPPER, U_MOTION_SPAWN);

	// カメラをプレイヤー注目状態に設定
	GET_MANAGER->GetCamera()->SetState(CCamera::STATE_LOOK_PLAYER);
	GET_MANAGER->GetCamera()->SetDestLookPlayer();	// カメラ目標位置の初期化
}

//============================================================
//	無敵の設定処理
//============================================================
void CPlayer::SetInvuln(void)
{
	// 無敵状態にする
	SetState(STATE_INVULN);

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
//	モーションの設定処理
//============================================================
void CPlayer::SetMotion
(
	const EBody bodyID,		// 身体インデックス
	const int nType,		// モーション種類
	const int nBlendFrame	// ブレンドフレーム
)
{
	if (bodyID > NONE_IDX && bodyID < BODY_MAX)
	{ // 正規インデックスの場合

		// 引数インデックスのモーションを設定
		CObjectDivChara::SetMotion(bodyID, nType, nBlendFrame);

		for (int nCntSword = 0; nCntSword < player::NUM_SWORD; nCntSword++)
		{ // 剣の数分繰り返す

			// 剣の状態を設定
			m_apSowrd[nCntSword]->SetState((IsWeaponDisp(BODY_UPPER)) ? CSword::STATE_NORMAL : CSword::STATE_VANISH);
		}
	}
	else { assert(false); }	// インデックスエラー
}

//============================================================
//	モーションの更新処理
//============================================================
void CPlayer::UpdateMotion(const int nLowMotion, const int nUpMotion)
{
	if (nLowMotion <= NONE_IDX || nLowMotion >= L_MOTION_MAX) { assert(false); return; }	// 下半身のモーションが未設定
	if (nUpMotion  <= NONE_IDX || nUpMotion  >= L_MOTION_MAX) { assert(false); return; }	// 上半身のモーションが未設定
	if (IsDeath()) { return; }	// 死亡している

	const int aMotion[] = { nLowMotion, nUpMotion };	// モーション情報
	for (int nCntBody = 0; nCntBody < BODY_MAX; nCntBody++)
	{ // 分割した身体の数分繰り返す

		// 各半身ごとのモーション更新
		(this->*(m_aFuncUpdateMotion[nCntBody]))(aMotion[nCntBody]);
	}
}

//============================================================
//	下半身モーションの更新処理
//============================================================
void CPlayer::UpdateMotionLower(const int nMotion)
{
	if (IsMotionLoop(BODY_LOWER))
	{ // ループするモーション中の場合

		if (GetMotionType(BODY_LOWER) != nMotion)
		{ // 現在のモーションが再生中のモーションと一致しない場合

			// 現在のモーションの設定
			SetMotion(BODY_LOWER, nMotion);
		}
	}
	else
	{ // ループしないモーション中の場合

		switch (GetMotionType(BODY_LOWER))
		{ // モーションごとの処理
		case L_MOTION_LAND:	// 着地モーション：ループOFF

			if (nMotion != L_MOTION_IDOL)
			{ // 待機モーション以外の場合

				// 現在のモーションの設定
				SetMotion(BODY_LOWER, nMotion);
			}

			break;

		// TODO：攻撃モーション追加したらここにも記述
		case L_MOTION_ATTACK_00:		// 攻撃モーション一段階目：ループOFF
		case L_MOTION_ATTACK_01:		// 攻撃モーション二段階目：ループOFF
		case L_MOTION_ATTACK_02:		// 攻撃モーション三段階目：ループOFF
		case L_MOTION_JUMP_ATTACK_00:	// 空中攻撃モーション一段階目：ループOFF
		case L_MOTION_JUMP_ATTACK_01:	// 空中攻撃モーション二段階目：ループOFF

			if (IsMotionCancel(BODY_LOWER))
			{ // キャンセルできる場合

				if (nMotion != L_MOTION_IDOL)
				{ // 待機モーション以外の場合

					// 現在のモーションの設定
					SetMotion(BODY_LOWER, nMotion);
				}
			}

			break;
		}
	}

	switch (GetMotionType(BODY_LOWER))
	{ // モーションごとの処理
	case L_MOTION_SPAWN:	// 登場モーション：ループOFF
	case L_MOTION_IDOL:		// 待機モーション：ループON
		break;

	case L_MOTION_MOVE:	// 移動モーション：ループON

		if (GetMotionKey(BODY_LOWER) % WALK_SOUND == 0 && GetMotionKeyCounter(BODY_LOWER) == 0)
		{ // 足がついたタイミングの場合

			// 歩行音の再生
			PLAY_SOUND(CSound::LABEL_SE_WALK_BUILD);
		}

		break;
	
	case L_MOTION_ATTACK_00:	// 攻撃モーション一段階目：ループOFF
	case L_MOTION_ATTACK_01:	// 攻撃モーション二段階目：ループOFF

		if (IsMotionCombo(BODY_LOWER))
		{ // モーションコンボができる場合

			if (m_attack.bInput)
			{ // 攻撃が先行入力されている場合

				// 次の攻撃モーションを設定
				SetMotion(BODY_LOWER, GetMotionType(BODY_LOWER) + 1);

				// 入力反映を加算
				if (m_attack.Add())	// 全モーションに反映したら先行入力を初期化
				{ // 全モーションに反映済みの場合

					// Lスティックの向きにプレイヤーを向かせる
					SetLStickRotation();
				}

				// 処理を抜ける
				break;
			}
		}

		if (IsMotionFinish(BODY_LOWER))
		{ // モーションが終了した場合

			// 現在のモーションの設定
			SetMotion(BODY_LOWER, nMotion);
		}

		break;

	case L_MOTION_ATTACK_02:	// 攻撃モーション三段階目：ループOFF

		if (IsMotionFinish(BODY_LOWER))
		{ // モーションが終了していた場合

			// 現在のモーションの設定
			SetMotion(BODY_LOWER, nMotion);
		}

		break;
	
	case L_MOTION_JUMP:	// ジャンプモーション：ループOFF
		break;

	case L_MOTION_LAND:	// ジャンプモーション：ループOFF

		if (IsMotionFinish(BODY_LOWER))
		{ // モーションが終了していた場合

			// 現在のモーションの設定
			SetMotion(BODY_LOWER, nMotion);
		}

		break;

	case L_MOTION_JUMP_ATTACK_00:	// 空中攻撃モーション一段階目：ループOFF

		if (IsMotionCombo(BODY_LOWER))
		{ // モーションコンボができる場合

			if (m_attack.bInput)
			{ // 攻撃が先行入力されている場合

				// 次の攻撃モーションを設定
				SetMotion(BODY_LOWER, GetMotionType(BODY_LOWER) + 1);

				// 入力反映を加算
				if (m_attack.Add())	// 全モーションに反映したら先行入力を初期化
				{ // 全モーションに反映済みの場合

					// Lスティックの向きにプレイヤーを向かせる
					SetLStickRotation();
				}

				// 処理を抜ける
				break;
			}
		}

		if (IsMotionFinish(BODY_LOWER))
		{ // モーションが終了した場合

			// 現在のモーションの設定
			SetMotion(BODY_LOWER, nMotion);
		}

		break;

	case L_MOTION_JUMP_ATTACK_01:	// 空中攻撃モーション二段階目：ループOFF

		if (IsMotionFinish(BODY_LOWER))
		{ // モーションが終了していた場合

			// 現在のモーションの設定
			SetMotion(BODY_LOWER, nMotion);
		}

		break;

	case L_MOTION_DEATH:	// 死亡モーション：ループON
		break;

	default:	// 例外処理
		assert(false);
		break;
	}
}

//============================================================
//	上半身モーションの更新処理
//============================================================
void CPlayer::UpdateMotionUpper(const int nMotion)
{
	if (IsMotionLoop(BODY_UPPER))
	{ // ループするモーション中の場合

		if (GetMotionType(BODY_UPPER) != nMotion)
		{ // 現在のモーションが再生中のモーションと一致しない場合

			// 現在のモーションの設定
			SetMotion(BODY_UPPER, nMotion);
		}
	}
	else
	{ // ループしないモーション中の場合

		switch (GetMotionType(BODY_UPPER))
		{ // モーションごとの処理
		case U_MOTION_LAND:	// 着地モーション：ループOFF

			if (nMotion != U_MOTION_IDOL)
			{ // 待機モーション以外の場合

				// 現在のモーションの設定
				SetMotion(BODY_UPPER, nMotion);
			}

			break;

		// TODO：攻撃モーション追加したらここにも記述
		case U_MOTION_ATTACK_00:		// 攻撃モーション一段階目：ループOFF
		case U_MOTION_ATTACK_01:		// 攻撃モーション二段階目：ループOFF
		case U_MOTION_ATTACK_02:		// 攻撃モーション三段階目：ループOFF
		case U_MOTION_JUMP_ATTACK_00:	// 空中攻撃モーション一段階目：ループOFF
		case U_MOTION_JUMP_ATTACK_01:	// 空中攻撃モーション二段階目：ループOFF

			if (IsMotionCancel(BODY_UPPER))
			{ // キャンセルできる場合

				if (nMotion != U_MOTION_IDOL)
				{ // 待機モーション以外の場合

					// 現在のモーションの設定
					SetMotion(BODY_UPPER, nMotion);
				}
			}

			break;
		}
	}

	switch (GetMotionType(BODY_UPPER))
	{ // モーションごとの処理
	case U_MOTION_SPAWN:	// 登場モーション：ループOFF
	case U_MOTION_IDOL:		// 待機モーション：ループON
	case U_MOTION_MOVE:		// 移動モーション：ループON
		break;
	
	case U_MOTION_ATTACK_00:	// 攻撃モーション一段階目：ループOFF
	case U_MOTION_ATTACK_01:	// 攻撃モーション二段階目：ループOFF

		if (IsMotionCombo(BODY_UPPER))
		{ // モーションコンボができる場合

			if (m_attack.bInput)
			{ // 攻撃が先行入力されている場合

				// 現在のモーションの設定
				SetMotion(BODY_UPPER, GetMotionType(BODY_UPPER) + 1);

				// 入力反映を加算
				if (m_attack.Add())	// 全モーションに反映したら先行入力を初期化
				{ // 全モーションに反映済みの場合

					// Lスティックの向きにプレイヤーを向かせる
					SetLStickRotation();
				}

				// 処理を抜ける
				break;
			}
		}

		if (IsMotionFinish(BODY_UPPER))
		{ // モーションが終了した場合

			// 現在のモーションの設定
			SetMotion(BODY_UPPER, nMotion);
		}
	
		break;

	case U_MOTION_ATTACK_02:	// 攻撃モーション三段階目：ループOFF

		if (IsMotionFinish(BODY_UPPER))
		{ // モーションが終了していた場合

			// 現在のモーションの設定
			SetMotion(BODY_UPPER, nMotion);
		}

		break;
	
	case U_MOTION_JUMP:	// ジャンプモーション：ループOFF
		break;

	case U_MOTION_LAND:	// ジャンプモーション：ループOFF

		if (IsMotionFinish(BODY_UPPER))
		{ // モーションが終了していた場合

			// 現在のモーションの設定
			SetMotion(BODY_UPPER, nMotion);
		}

		break;

	case U_MOTION_JUMP_ATTACK_00:	// 空中攻撃モーション一段階目：ループOFF

		if (IsMotionCombo(BODY_UPPER))
		{ // モーションコンボができる場合

			if (m_attack.bInput)
			{ // 攻撃が先行入力されている場合

				// 現在のモーションの設定
				SetMotion(BODY_UPPER, GetMotionType(BODY_UPPER) + 1);

				// 入力反映を加算
				if (m_attack.Add())	// 全モーションに反映したら先行入力を初期化
				{ // 全モーションに反映済みの場合

					// Lスティックの向きにプレイヤーを向かせる
					SetLStickRotation();
				}

				if (m_move.y < 0.0f)
				{ // 縦移動量がマイナスの場合

					// 上移動量をプラスにする
					m_move.y = 10.0f;
				}
				else
				{ // 縦移動量がプラスの場合

					// 上移動量を加える
					m_move.y += 4.0f;
				}

				// 処理を抜ける
				break;
			}
		}

		if (IsMotionFinish(BODY_UPPER))
		{ // モーションが終了した場合

			// 現在のモーションの設定
			SetMotion(BODY_UPPER, nMotion);
		}

		break;

	case U_MOTION_JUMP_ATTACK_01:	// 空中攻撃モーション二段階目：ループOFF

		if (IsMotionFinish(BODY_UPPER))
		{ // モーションが終了していた場合

			// 現在のモーションの設定
			SetMotion(BODY_UPPER, nMotion);
		}

		break;

	case U_MOTION_DEATH:	// 死亡モーション：ループON
		break;

	default:	// 例外処理
		assert(false);
		break;
	}
}

//============================================================
//	目標向きを敵にする処理
//============================================================
void CPlayer::SetDestLookEnemy
(
	const D3DXVECTOR3& rPosPlayer,	// プレイヤー位置
	const D3DXVECTOR3& rRotPlayer,	// プレイヤー向き
	const float fRate				// 向き補正係数
)
{
	CEnemy *pEnemy = nullptr;	// 向きを補正する敵

	// 向きの補正をする敵を指定
	{
		CListManager<CEnemy> *pList = CEnemy::GetList();	// リストマネージャー
		if (pList == nullptr)		 { return; }	// リスト未使用
		if (pList->GetNumAll() <= 0) { return; }	// 敵が存在しない

		float fMinLength = 0.0f;	// 一番プレイヤーに近い距離
		std::list<CEnemy*> list = pList->GetList();	// 敵リスト
		for (auto enemy : list)
		{ // リストのすべてを繰り返す

			// XZ平面の円の当たり判定
			float fLength = 0.0f;	// プレイヤーからの距離
			bool bHit = collision::Circle2D
			( // 引数
				rPosPlayer,					// 判定位置
				enemy->GetVec3Position(),	// 判定目標位置
				LOOK_RADIUS,	// 判定半径
				0.0f,			// 判定目標半径
				&fLength		// 判定目標との距離
			);
			if (bHit)
			{ // 判定内の場合

				if (pEnemy == nullptr)
				{ // 近い敵が設定されていない場合

					// 初期情報を保存
					pEnemy = enemy;			// 一番プレイヤーに近い敵
					fMinLength = fLength;	// 一番プレイヤーに近い距離
				}
				else
				{ // 近い敵が設定されている場合

					if (fLength < fMinLength)
					{ // 設定中の敵よりさらに近い場合

						// 情報を保存
						pEnemy = enemy;			// 一番プレイヤーに近い敵
						fMinLength = fLength;	// 一番プレイヤーに近い距離
					}
				}
			}
		}
	}

	// 敵の方向を向かせる
	{
		if (pEnemy == nullptr) { return; }	// 敵指定ない場合抜ける

		D3DXVECTOR3 posEnemy = pEnemy->GetVec3Position();	// 敵位置
		float fDestRotY, fDiffRotY;	// 目標・差分向き

		// 目標向きを設定
		fDestRotY = atan2f(rPosPlayer.x - posEnemy.x, rPosPlayer.z - posEnemy.z);
		useful::NormalizeRot(fDestRotY);	// 向き正規化

		// 差分向きを設定
		fDiffRotY = fDestRotY - rRotPlayer.y;
		useful::NormalizeRot(fDiffRotY);	// 向き正規化

		// 実際の目標向きを設定
		m_destRot.y += fDiffRotY * fRate;
	}
}

//============================================================
//	Lスティックの向きにプレイヤーを向かせる処理
//============================================================
void CPlayer::SetLStickRotation(void)
{
	CCamera *pCamera = GET_MANAGER->GetCamera();	// カメラ
	CInputPad *pPad = GET_INPUTPAD;	// パッド
	D3DXVECTOR3 vec = VEC3_ZERO;	// ベクトル

	float fLTilt = pPad->GetPressLStickTilt();	// スティックの傾き量
	if (pad::DEAD_ZONE < fLTilt)
	{ // デッドゾーン以上の場合

		// スティック傾き方向のベクトルを作成
		vec.x = sinf(pPad->GetPressLStickRot() + pCamera->GetVec3Rotation().y + HALF_PI);
		vec.z = cosf(pPad->GetPressLStickRot() + pCamera->GetVec3Rotation().y + HALF_PI);

		// スティック傾き方向を向きに設定
		m_destRot.y = atan2f(-vec.x, -vec.z);	// 目標向き
		SetVec3Rotation(m_destRot);				// 向き
	}
}

//============================================================
//	攻撃状況の取得処理
//============================================================
bool CPlayer::IsAttack(void) const
{
	// 変数を宣言
	EUpperMotion curMotion = (EUpperMotion)GetMotionType(BODY_UPPER);	// 現在のモーション

	// TODO：攻撃追加したら記述

	// 攻撃状況を設定
	bool bAttack = (curMotion == U_MOTION_ATTACK_00
				 || curMotion == U_MOTION_ATTACK_01
				 || curMotion == U_MOTION_ATTACK_02
				 || curMotion == U_MOTION_JUMP_ATTACK_00
				 || curMotion == U_MOTION_JUMP_ATTACK_01);

	// 攻撃状況を返す
	return bAttack;
}

//============================================================
//	スポーン状態時の更新処理
//============================================================
void CPlayer::UpdateSpawn(void)
{
	// 別モーションの指定がされている場合エラー
	assert(GetMotionType(BODY_LOWER) == L_MOTION_SPAWN);

	if (IsMotionCancel(BODY_LOWER))
	{ // モーションがキャンセルできる場合

		// 何もしない状態にする
		SetState(STATE_NONE);
	}
}

//============================================================
//	通常状態時の更新処理
//============================================================
void CPlayer::UpdateNormal(int *pLowMotion, int *pUpMotion)
{
	// 変数を宣言
	D3DXVECTOR3 posPlayer = GetVec3Position();	// プレイヤー位置
	D3DXVECTOR3 rotPlayer = GetVec3Rotation();	// プレイヤー向き

	// ポインタを宣言
	CStage *pStage = CScene::GetStage();				// ステージ情報
	if (pStage == nullptr) { assert(false); return; }	// ステージ非使用中

	// 攻撃操作
	UpdateAttack(posPlayer, rotPlayer);

	// 回避操作
	UpdateDodge();

	// 移動操作・目標向き設定
	UpdateMove(pLowMotion, pUpMotion);

	// ジャンプ操作
	UpdateJump(pLowMotion, pUpMotion);

	// 位置更新
	UpdatePosition(&posPlayer);

	// 敵との当たり判定の更新
	UpdateCollEnemy(&posPlayer);

	// 着地判定
	UpdateLanding(&posPlayer);

	// 向き更新
	UpdateRotation(&rotPlayer);

	// ステージ範囲外の補正
	pStage->LimitPosition(posPlayer, RADIUS);

	// 位置を反映
	SetVec3Position(posPlayer);

	// 向きを反映
	SetVec3Rotation(rotPlayer);
}

//============================================================
//	ダメージ状態時の更新処理
//============================================================
void CPlayer::UpdateDamage(int *pLowMotion, int *pUpMotion)
{
	// 身体の色を赤くする
	SetAllMaterial(material::DamageRed());

	// 通常状態の更新
	UpdateNormal(pLowMotion, pUpMotion);

	// カウンターを加算
	m_nCounterState++;
	if (m_nCounterState > DAMAGE_FRAME)
	{ // ダメージ状態の終了フレームになった場合

		// カウンターを初期化
		m_nCounterState = 0;

		// 身体の色を元に戻す
		ResetMaterial();

		// 無敵状態にする
		SetInvuln();
	}
}

//============================================================
//	無敵状態時の更新処理
//============================================================
void CPlayer::UpdateInvuln(int *pLowMotion, int *pUpMotion)
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

	// 通常状態の更新
	UpdateNormal(pLowMotion, pUpMotion);

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
//	攻撃操作の更新処理
//============================================================
void CPlayer::UpdateAttack
(
	const D3DXVECTOR3& rPos,	// プレイヤー位置
	const D3DXVECTOR3& rRot		// プレイヤー向き
)
{
	if (m_dodge.bDodge)	{ return; }	// 回避中の場合抜ける

	if (GET_INPUTPAD->IsTrigger(CInputPad::KEY_X))
	{
		if (!m_jump.bJump)
		{ // ジャンプしていない場合

			// 地上攻撃操作の更新
			UpdateLandAttack();
		}
		else
		{ // ジャンプしている場合

			// 空中攻撃操作の更新
			UpdateSkyAttack();
		}

		// 目標向きを敵方向にする
		SetDestLookEnemy
		( // 引数
			rPos,		// プレイヤー位置
			rRot,		// プレイヤー向き
			LOOK_REV	// 向き補正係数
		);
	}
}

//============================================================
//	地上攻撃操作の更新処理
//============================================================
void CPlayer::UpdateLandAttack(void)
{
	if (!IsAttack())
	{ // 攻撃中ではない場合

		// 攻撃モーションを指定
		SetMotion(BODY_LOWER, L_MOTION_ATTACK_00);
		SetMotion(BODY_UPPER, U_MOTION_ATTACK_00);
	}
	else
	{ // 攻撃中の場合

		if (GetMotionType(BODY_UPPER) != U_MOTION_ATTACK_02)	// TODO：一番最後の攻撃にする
		{ // 最終攻撃モーションではない場合

			// コンボ可能までの残りフレームを計算
			int nWholeFrame = GetMotionComboFrame(BODY_UPPER) - GetMotionWholeCounter(BODY_UPPER);
			if (nWholeFrame < ATTACK_BUFFER_FRAME)
			{ // 先行入力が可能な場合

				// 先行入力を受け付ける
				m_attack.bInput = true;
			}
		}
	}
}

//============================================================
//	空中攻撃操作の更新処理
//============================================================
void CPlayer::UpdateSkyAttack(void)
{
	if (!IsAttack())
	{ // 攻撃中ではない場合

		// 攻撃モーションを指定
		SetMotion(BODY_LOWER, L_MOTION_JUMP_ATTACK_00);
		SetMotion(BODY_UPPER, U_MOTION_JUMP_ATTACK_00);

		if (m_move.y < 0.0f)
		{ // 縦移動量がマイナスの場合

			// 上移動量をプラスにする
			m_move.y = 10.0f;
		}
		else
		{ // 縦移動量がプラスの場合

			// 上移動量を加える
			m_move.y += 4.0f;
		}
	}
	else
	{ // 攻撃中の場合

		if (GetMotionType(BODY_UPPER) != U_MOTION_JUMP_ATTACK_01)	// TODO：一番最後の攻撃にする
		{ // 最終攻撃モーションではない場合

			// コンボ可能までの残りフレームを計算
			int nWholeFrame = GetMotionComboFrame(BODY_UPPER) - GetMotionWholeCounter(BODY_UPPER);
			if (nWholeFrame < ATTACK_BUFFER_FRAME)
			{ // 先行入力が可能な場合

				// 先行入力を受け付ける
				m_attack.bInput = true;
			}
		}
	}
}

//============================================================
//	回避操作の更新処理
//============================================================
void CPlayer::UpdateDodge(void)
{
	CInputPad *pPad  = GET_INPUTPAD;				// パッド
	CCamera *pCamera = GET_MANAGER->GetCamera();	// カメラ

	if (m_dodge.bDodge)
	{ // 回避中の場合

		// 回避方向に移動量を与える
		m_move.x += sinf(m_dodge.fRot) * m_dodge.fMove;
		m_move.z += cosf(m_dodge.fRot) * m_dodge.fMove;

		// 移動量を減算
		m_dodge.fMove += (0.0f - m_dodge.fMove) * DODGE_REV_MOVE;
		if (m_dodge.fMove <= DODGE_MIN_MOVE)
		{ // 移動量が下がり切った場合

			// 回避フラグをOFFにする
			m_dodge.bDodge = false;
		}
	}
	else
	{ // 回避中ではない場合

		if (IsAttack() && !IsMotionCancel(BODY_LOWER)) { return; }	// 攻撃中且つモーションがキャンセルできない場合抜ける
		if (m_jump.bJump) { return; }	// ジャンプ中の場合抜ける

		if (m_dodge.nWaitCounter > 0)
		{ // クールタイムが残っている場合

			// クールタイムを減算
			m_dodge.nWaitCounter--;
		}

		if (pPad->IsTrigger(CInputPad::KEY_B))
		{
			if (m_dodge.nWaitCounter <= 0)
			{ // クールタイムがない場合

				if (pad::DEAD_ZONE < pPad->GetPressLStickTilt())
				{ // スティックの傾き量がデッドゾーン以上の場合

					// 回避移動量を設定
					m_dodge.fMove = DODGE_SIDE_MOVE;

					// 回避方向を設定
					m_dodge.fRot = pPad->GetPressLStickRot() + pCamera->GetVec3Rotation().y + HALF_PI;

					// スティック入力方向に移動量を与える
					m_move.x += sinf(m_dodge.fRot) * m_dodge.fMove;
					m_move.z += cosf(m_dodge.fRot) * m_dodge.fMove;

					// 上移動量を与える
					m_move.y += DODGE_JUMP_MOVE;

					// 目標向きを設定
					m_destRot.y = atan2f(-m_move.x, -m_move.z);

					// ジャンプ中にする
					m_jump.bJump = true;

					// 回避中にする
					m_dodge.bDodge = true;

					// クールタイムを設定
					m_dodge.nWaitCounter = DODGE_WAIT_FRAME;
				}
			}
		}
	}
}

//============================================================
//	移動操作・目標向きの更新処理
//============================================================
void CPlayer::UpdateMove(int *pLowMotion, int *pUpMotion)
{
	if (IsAttack() && !IsMotionCancel(BODY_LOWER)) { return; }	// 攻撃中且つモーションがキャンセルできない場合抜ける
	if (m_dodge.bDodge)	{ return; }	// 回避中の場合抜ける

	CInputPad *pPad  = GET_INPUTPAD;				// パッド
	CCamera *pCamera = GET_MANAGER->GetCamera();	// カメラ

	float fLTilt = pPad->GetPressLStickTilt();	// スティックの傾き量
	if (pad::DEAD_ZONE < fLTilt)
	{ // デッドゾーン以上の場合

		// 変数を宣言
		float fMove = fLTilt * STICK_REV;	// プレイヤー移動量

		// 移動量を更新
		m_move.x += sinf(pPad->GetPressLStickRot() + pCamera->GetVec3Rotation().y + HALF_PI) * fMove * MOVE_REV;
		m_move.z += cosf(pPad->GetPressLStickRot() + pCamera->GetVec3Rotation().y + HALF_PI) * fMove * MOVE_REV;

		// 目標向きを設定
		m_destRot.y = atan2f(-m_move.x, -m_move.z);

		// 上下に移動モーションを設定
		*pLowMotion = L_MOTION_MOVE;
		*pUpMotion  = U_MOTION_MOVE;
	}
}

//============================================================
//	ジャンプ操作の更新処理
//============================================================
void CPlayer::UpdateJump(int *pLowMotion, int *pUpMotion)
{
	if (m_dodge.bDodge)	{ return; }	// 回避中の場合抜ける
	if (IsAttack() && !IsMotionCancel(BODY_LOWER)) { return; }	// 攻撃中且つモーションがキャンセルできない場合抜ける

	if (GET_INPUTPAD->IsTrigger(CInputPad::KEY_A))
	{
		if (!m_jump.bJump)
		{ // ジャンプしていない場合

			// 上移動量を与える
			m_move.y += INIT_JUMP;

			// ジャンプ中にする
			m_jump.bJump = true;

			// プレス入力中にする
			m_jump.bInputPress = true;

			// 上下にジャンプモーションを設定
			*pLowMotion = L_MOTION_JUMP;
			*pUpMotion  = U_MOTION_JUMP;
		}
	}

	if (m_jump.bJump && m_jump.bInputPress)
	{ // プレス入力が解除されていない場合

		if (GET_INPUTPAD->IsPress(CInputPad::KEY_A))
		{ // プレス中の場合

			if (m_jump.nPressCounter < PRESS_JUMP_FRAME)
			{ // 入力可能時間内の場合

				// ジャンプ高度上昇量の補正係数
				float fRate = easeing::InQuint((PRESS_JUMP_FRAME - m_jump.nPressCounter), 0, PRESS_JUMP_FRAME);

				// 上移動量を与える
				m_move.y += GRAVITY + PULS_JUMP * fRate;

				// 入力時間カウンターを加算
				m_jump.nPressCounter++;
			}
		}
		else
		{ // 入力がなくなった場合

			// 入力時間カウンターを初期化
			m_jump.nPressCounter = 0;

			// プレス入力を解除
			m_jump.bInputPress = false;
		}
	}
}

//============================================================
//	過去位置の更新処理
//============================================================
void CPlayer::UpdateOldPosition(void)
{
	// 過去位置を更新
	m_oldPos = GetVec3Position();
}

//============================================================
//	重力の更新処理
//============================================================
void CPlayer::UpdateGravity(void)
{
	// 重力を加算
	m_move.y -= GRAVITY;
}

//============================================================
//	着地状況の更新処理
//============================================================
void CPlayer::UpdateLanding(D3DXVECTOR3 *pPos)
{
	// ジャンプしている状態にする
	m_jump.bJump = true;

	// 地面・制限位置の着地判定
	if (CScene::GetStage()->LandFieldPosition(*pPos, m_move)
	||  CScene::GetStage()->LandLimitPosition(*pPos, m_move, 0.0f))
	{ // プレイヤーが着地していた場合

		// ジャンプフラグをOFFにする
		m_jump.bJump = false;

		// 回避フラグをOFFにする
		m_dodge.bDodge = false;
	}

	if (!m_jump.bJump)
	{ // 空中にいない場合

		if (GetMotionType(BODY_LOWER) == L_MOTION_JUMP
		&&  GetMotionType(BODY_UPPER) == U_MOTION_JUMP)
		{ // 上下モーションがジャンプ中の場合

			// 着地モーションを指定
			SetMotion(BODY_LOWER, L_MOTION_LAND);
			SetMotion(BODY_UPPER, U_MOTION_LAND);
		}
	}
}

//============================================================
//	敵との当たり判定の更新処理
//============================================================
void CPlayer::UpdateCollEnemy(D3DXVECTOR3 *pPos)
{
	CListManager<CEnemy> *pList = CEnemy::GetList();	// リストマネージャー
	if (pList == nullptr)		 { return; }	// リスト未使用
	if (pList->GetNumAll() <= 0) { return; }	// 敵が存在しない

	D3DXVECTOR3 heightPlayer = D3DXVECTOR3(0.0f, HEIGHT, 0.0f);	// プレイヤーの縦幅
	D3DXVECTOR3 posPlayerCent = *pPos + heightPlayer * 0.5f;	// プレイヤーの中心位置
	std::list<CEnemy*> list = pList->GetList();	// 敵リスト
	for (auto enemy : list)
	{ // リストのすべてを繰り返す

		float fRadiusEnemy = enemy->GetStatusInfo().fCollRadius;	// 敵の半径
		int   nHeadID = enemy->GetHeadModelID();					// 敵の頭モデルインデックス
		int   nWaistID = enemy->GetWaistModelID();					// 敵の腰モデルインデックス
		D3DXMATRIX  mtxEnemyHead = enemy->GetMultiModel(nHeadID)->GetMtxWorld();	// 敵の頭のワールドマトリックス
		D3DXVECTOR3 posEnemyHead = useful::GetMatrixPosition(mtxEnemyHead);			// 敵の頭の位置
		D3DXMATRIX  mtxEnemyWaist = enemy->GetMultiModel(nWaistID)->GetMtxWorld();	// 敵の腰のワールドマトリックス
		D3DXVECTOR3 posEnemyWaist = useful::GetMatrixPosition(mtxEnemyWaist);		// 敵の腰の位置
		D3DXVECTOR3 posEnemyOrigin = enemy->GetVec3Position();						// 敵の原点位置

		float fHeightEnemy = posEnemyHead.y - posEnemyOrigin.y;	// 敵の縦幅
		D3DXVECTOR3 posEnemyCent = D3DXVECTOR3	// 敵の中心位置
		(
			posEnemyWaist.x,
			posEnemyOrigin.y + fHeightEnemy * 0.5f,
			posEnemyWaist.z
		);

		// 敵との押し戻し判定
		collision::ResponseCapsule3D
		( // 引数
			&posPlayerCent,	// 判定位置
			posEnemyCent,	// 判定目標位置
			RADIUS,			// 判定半径
			fRadiusEnemy,	// 判定目標半径
			fHeightEnemy	// 判定目標縦幅
		);

		// プレイヤーの中心位置から縦幅分減算
		*pPos = posPlayerCent - heightPlayer * 0.5f;
	}
}

//============================================================
//	位置の更新処理
//============================================================
void CPlayer::UpdatePosition(D3DXVECTOR3 *pPos)
{
	// 位置を移動
	*pPos += m_move;

	// 移動量を減衰
	if (m_dodge.bDodge)
	{ // 回避中の場合

		m_move.x += (0.0f - m_move.x) * DODGE_REV;
		m_move.z += (0.0f - m_move.z) * DODGE_REV;
	}
	else
	{ // 回避中ではない場合

		if (m_jump.bJump)
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
}

//============================================================
//	向きの更新処理
//============================================================
void CPlayer::UpdateRotation(D3DXVECTOR3 *pRot)
{
	// 変数を宣言
	float fDiffRot = 0.0f;	// 差分向き

	// 目標向きの正規化
	useful::NormalizeRot(m_destRot.y);

	// 目標向きまでの差分を計算
	fDiffRot = m_destRot.y - pRot->y;

	// 差分向きの正規化
	useful::NormalizeRot(fDiffRot);

	// 向きの更新
	pRot->y += fDiffRot * REV_ROTA;

	// 向きの正規化
	useful::NormalizeRot(pRot->y);
}

//============================================================
//	フェードアウト状態時の更新処理
//============================================================
bool CPlayer::UpdateFadeOut(const float fAdd)
{
	// 変数を宣言
	bool bEndAlpha = false;		// 透明状況
	float fAlpha = GetAlpha();	// 透明度

	// プレイヤー自身の描画を再開
	CObject::SetEnableDraw(true);

	// 透明度を上げる
	fAlpha += fAdd;

	float fMaxAlpha = GetMaxAlpha();
	if (fAlpha >= fMaxAlpha)
	{ // 透明度が上がり切った場合

		// 透明度を補正
		fAlpha = fMaxAlpha;

		// 不透明になり切った状態にする
		bEndAlpha = true;
	}

	// 透明度を設定
	SetAlpha(fAlpha);

	// 透明状況を返す
	return bEndAlpha;
}

//============================================================
//	フェードイン状態時の更新処理
//============================================================
bool CPlayer::UpdateFadeIn(const float fSub)
{
	// 変数を宣言
	bool bEndAlpha = false;		// 透明状況
	float fAlpha = GetAlpha();	// 透明度

	// 透明度を下げる
	fAlpha -= fSub;

	if (fAlpha <= 0.0f)
	{ // 透明度が下がり切った場合

		// 透明度を補正
		fAlpha = 0.0f;

		// 透明になり切った状態にする
		bEndAlpha = true;

		// プレイヤー自身の描画を停止
		CObject::SetEnableDraw(false);
	}

	// 透明度を設定
	SetAlpha(fAlpha);

	// 透明状況を返す
	return bEndAlpha;
}

//============================================================
//	セットアップ処理
//============================================================
void CPlayer::LoadSetup(const EBody bodyID, const char **ppModelPass)
{
	// 変数を宣言
	CSwordWaveManager::STiming waveTiming;	// 波動スポーンの代入用
	CMotion::SMotionInfo keyInfo;	// キーの代入用
	D3DXVECTOR3 pos = VEC3_ZERO;	// 位置の代入用
	D3DXVECTOR3 rot = VEC3_ZERO;	// 向きの代入用
	int nSword		= NONE_IDX;		// 剣インデックス
	int nID			= 0;	// インデックスの代入用
	int nParentID	= 0;	// 親インデックスの代入用
	int nNowPose	= 0;	// 現在のポーズ番号
	int nNowKey		= 0;	// 現在のキー番号
	int nWeapon		= 0;	// 武器表示のON/OFFの変換用
	int nLoop		= 0;	// ループのON/OFFの変換用
	int nEnd		= 0;	// テキスト読み込み終了の確認用

	// 変数配列を宣言
	char aString[MAX_STRING];	// テキストの文字列の代入用

	// ポインタを宣言
	FILE *pFile;	// ファイルポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(SETUP_TXT[bodyID], "r");

	if (pFile != nullptr)
	{ // ファイルが開けた場合

		do
		{ // 読み込んだ文字列が EOF ではない場合ループ

			// ファイルから文字列を読み込む
			nEnd = fscanf(pFile, "%s", &aString[0]);	// テキストを読み込みきったら EOF を返す

			// キャラクターの設定
			if (strcmp(&aString[0], "CHARACTERSET") == 0)
			{ // 読み込んだ文字列が CHARACTERSET の場合

				do
				{ // 読み込んだ文字列が END_CHARACTERSET ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "PARTSSET") == 0)
					{ // 読み込んだ文字列が PARTSSET の場合

						do
						{ // 読み込んだ文字列が END_PARTSSET ではない場合ループ

							// ファイルから文字列を読み込む
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "INDEX") == 0)
							{ // 読み込んだ文字列が INDEX の場合

								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%d", &nID);			// モデルのインデックスを読み込む
							}
							else if (strcmp(&aString[0], "PARENT") == 0)
							{ // 読み込んだ文字列が PARENT の場合

								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%d", &nParentID);	// モデルの親のインデックスを読み込む
							}
							else if (strcmp(&aString[0], "POS") == 0)
							{ // 読み込んだ文字列が POS の場合

								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%f", &pos.x);		// X座標を読み込む
								fscanf(pFile, "%f", &pos.y);		// Y座標を読み込む
								fscanf(pFile, "%f", &pos.z);		// Z座標を読み込む
							}
							else if (strcmp(&aString[0], "ROT") == 0)
							{ // 読み込んだ文字列が ROT の場合

								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%f", &rot.x);		// X向きを読み込む
								fscanf(pFile, "%f", &rot.y);		// Y向きを読み込む
								fscanf(pFile, "%f", &rot.z);		// Z向きを読み込む
							}
						} while (strcmp(&aString[0], "END_PARTSSET") != 0);	// 読み込んだ文字列が END_PARTSSET ではない場合ループ

						// パーツ情報の設定
						SetPartsInfo(bodyID, nID, nParentID, pos, rot, ppModelPass[nID]);
					}
				} while (strcmp(&aString[0], "END_CHARACTERSET") != 0);		// 読み込んだ文字列が END_CHARACTERSET ではない場合ループ
			}

			// モーションの設定
			else if (strcmp(&aString[0], "MOTIONSET") == 0)
			{ // 読み込んだ文字列が MOTIONSET の場合

				// 現在のポーズ番号を初期化
				nNowPose = 0;

				// ポーズ代入用の変数を初期化
				memset(&keyInfo, 0, sizeof(keyInfo));

				// キャンセル・コンボフレームをなしにする
				keyInfo.nCancelFrame = NONE_IDX;
				keyInfo.nComboFrame  = NONE_IDX;

				// 攻撃判定情報を初期化
				keyInfo.collLeft.nMin  = NONE_IDX;
				keyInfo.collLeft.nMax  = NONE_IDX;
				keyInfo.collRight.nMin = NONE_IDX;
				keyInfo.collRight.nMax = NONE_IDX;

				// 武器表示をOFFにする
				keyInfo.bWeaponDisp = false;

				do
				{ // 読み込んだ文字列が END_MOTIONSET ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "WEAPON") == 0)
					{ // 読み込んだ文字列が WEAPON の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &nWeapon);		// 武器表示のON/OFFを読み込む

						// 読み込んだ値をbool型に変換
						keyInfo.bWeaponDisp = (nWeapon == 0) ? false : true;
					}
					else if (strcmp(&aString[0], "LOOP") == 0)
					{ // 読み込んだ文字列が LOOP の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &nLoop);		// ループのON/OFFを読み込む

						// 読み込んだ値をbool型に変換
						keyInfo.bLoop = (nLoop == 0) ? false : true;
					}
					else if (strcmp(&aString[0], "NUM_KEY") == 0)
					{ // 読み込んだ文字列が NUM_KEY の場合

						fscanf(pFile, "%s", &aString[0]);		// = を読み込む (不要)
						fscanf(pFile, "%d", &keyInfo.nNumKey);	// キーの総数を読み込む
					}
					else if (strcmp(&aString[0], "CANCEL") == 0)
					{ // 読み込んだ文字列が CANCEL の場合

						fscanf(pFile, "%s", &aString[0]);			// = を読み込む (不要)
						fscanf(pFile, "%d", &keyInfo.nCancelFrame);	// キャンセル可能フレームを読み込む
					}
					else if (strcmp(&aString[0], "COMBO") == 0)
					{ // 読み込んだ文字列が COMBO の場合

						fscanf(pFile, "%s", &aString[0]);			// = を読み込む (不要)
						fscanf(pFile, "%d", &keyInfo.nComboFrame);	// コンボ可能フレームを読み込む
					}
					else if (strcmp(&aString[0], "LEFT_COLL") == 0)
					{ // 読み込んだ文字列が LEFT_COLL の場合

						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%d", &keyInfo.collLeft.nMin);	// 判定を出す開始フレームを読み込む
						fscanf(pFile, "%d", &keyInfo.collLeft.nMax);	// 判定を消す終了フレームを読み込む
					}
					else if (strcmp(&aString[0], "RIGHT_COLL") == 0)
					{ // 読み込んだ文字列が RIGHT_COLL の場合

						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%d", &keyInfo.collRight.nMin);	// 判定を出す開始フレームを読み込む
						fscanf(pFile, "%d", &keyInfo.collRight.nMax);	// 判定を消す終了フレームを読み込む
					}
					else if (strcmp(&aString[0], "WAVE") == 0)
					{ // 読み込んだ文字列が WAVE の場合

						do
						{ // 読み込んだ文字列が END_WAVE ではない場合ループ

							// ファイルから文字列を読み込む
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "SWORD") == 0)
							{ // 読み込んだ文字列が SWORD の場合

								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%d", &nSword);		// 剣インデックスを読み込む
							}
							else if (strcmp(&aString[0], "FRAME") == 0)
							{ // 読み込んだ文字列が FRAME の場合

								fscanf(pFile, "%s", &aString[0]);			// = を読み込む (不要)
								fscanf(pFile, "%d", &waveTiming.nFrame);	// 生成フレームを読み込む
							}
							else if (strcmp(&aString[0], "OFFSETPOS") == 0)
							{ // 読み込んだ文字列が OFFSETPOS の場合

								fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
								fscanf(pFile, "%f", &waveTiming.posOffset.x);	// X位置オフセットを読み込む
								fscanf(pFile, "%f", &waveTiming.posOffset.y);	// Y位置オフセットを読み込む
								fscanf(pFile, "%f", &waveTiming.posOffset.z);	// Z位置オフセットを読み込む
							}
							else if (strcmp(&aString[0], "OFFSETROT") == 0)
							{ // 読み込んだ文字列が OFFSETROT の場合

								fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
								fscanf(pFile, "%f", &waveTiming.rotOffset.x);	// X向きオフセットを読み込む
								fscanf(pFile, "%f", &waveTiming.rotOffset.y);	// Y向きオフセットを読み込む
								fscanf(pFile, "%f", &waveTiming.rotOffset.z);	// Z向きオフセットを読み込む
							}
						} while (strcmp(&aString[0], "END_WAVE") != 0);	// 読み込んだ文字列が END_WAVE ではない場合ループ

						if (nSword > NONE_IDX && nSword < player::NUM_SWORD)
						{ // 剣インデックスが範囲内の場合

							// 剣の波動タイミングを追加
							waveTiming.nMotion = GetMotionNumType(bodyID);	// 現在のモーションを設定
							m_apSowrd[nSword]->GetWaveManager()->AddTiming(waveTiming);
						}
						else { assert(false); }	// 範囲外
					}
					else if (strcmp(&aString[0], "KEYSET") == 0)
					{ // 読み込んだ文字列が KEYSET の場合

						// 現在のキー番号を初期化
						nNowKey = 0;

						do
						{ // 読み込んだ文字列が END_KEYSET ではない場合ループ

							// ファイルから文字列を読み込む
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "FRAME") == 0)
							{ // 読み込んだ文字列が FRAME の場合

								fscanf(pFile, "%s", &aString[0]);							// = を読み込む (不要)
								fscanf(pFile, "%d", &keyInfo.aKeyInfo[nNowPose].nFrame);	// キーが切り替わるまでのフレーム数を読み込む
							}
							else if (strcmp(&aString[0], "MOVE") == 0)
							{ // 読み込んだ文字列が MOVE の場合

								fscanf(pFile, "%s", &aString[0]);							// = を読み込む (不要)
								fscanf(pFile, "%f", &keyInfo.aKeyInfo[nNowPose].move.x);	// キーが切り替わるまでの移動量を読み込む
								fscanf(pFile, "%f", &keyInfo.aKeyInfo[nNowPose].move.y);	// キーが切り替わるまでの移動量を読み込む
								fscanf(pFile, "%f", &keyInfo.aKeyInfo[nNowPose].move.z);	// キーが切り替わるまでの移動量を読み込む
							}
							else if (strcmp(&aString[0], "KEY") == 0)
							{ // 読み込んだ文字列が KEY の場合

								do
								{ // 読み込んだ文字列が END_KEY ではない場合ループ

									// ファイルから文字列を読み込む
									fscanf(pFile, "%s", &aString[0]);

									if (strcmp(&aString[0], "POS") == 0)
									{ // 読み込んだ文字列が POS の場合

										fscanf(pFile, "%s", &aString[0]);										// = を読み込む (不要)
										fscanf(pFile, "%f", &keyInfo.aKeyInfo[nNowPose].aKey[nNowKey].pos.x);	// X位置を読み込む
										fscanf(pFile, "%f", &keyInfo.aKeyInfo[nNowPose].aKey[nNowKey].pos.y);	// Y位置を読み込む
										fscanf(pFile, "%f", &keyInfo.aKeyInfo[nNowPose].aKey[nNowKey].pos.z);	// Z位置を読み込む

										// 読み込んだ位置にパーツの初期位置を加算
										keyInfo.aKeyInfo[nNowPose].aKey[nNowKey].pos += GetPartsPosition(bodyID, nNowKey);
									}
									else if (strcmp(&aString[0], "ROT") == 0)
									{ // 読み込んだ文字列が ROT の場合

										fscanf(pFile, "%s", &aString[0]);										// = を読み込む (不要)
										fscanf(pFile, "%f", &keyInfo.aKeyInfo[nNowPose].aKey[nNowKey].rot.x);	// X向きを読み込む
										fscanf(pFile, "%f", &keyInfo.aKeyInfo[nNowPose].aKey[nNowKey].rot.y);	// Y向きを読み込む
										fscanf(pFile, "%f", &keyInfo.aKeyInfo[nNowPose].aKey[nNowKey].rot.z);	// Z向きを読み込む

										// 読み込んだ向きにパーツの初期向きを加算
										keyInfo.aKeyInfo[nNowPose].aKey[nNowKey].rot += GetPartsRotation(bodyID, nNowKey);

										// 初期向きを正規化
										useful::NormalizeRot(keyInfo.aKeyInfo[nNowPose].aKey[nNowKey].rot.x);
										useful::NormalizeRot(keyInfo.aKeyInfo[nNowPose].aKey[nNowKey].rot.y);
										useful::NormalizeRot(keyInfo.aKeyInfo[nNowPose].aKey[nNowKey].rot.z);
									}

								} while (strcmp(&aString[0], "END_KEY") != 0);	// 読み込んだ文字列が END_KEY ではない場合ループ

								// 現在のキー番号を加算
								nNowKey++;
							}
						} while (strcmp(&aString[0], "END_KEYSET") != 0);	// 読み込んだ文字列が END_KEYSET ではない場合ループ

						// 現在のポーズ番号を加算
						nNowPose++;
					}
				} while (strcmp(&aString[0], "END_MOTIONSET") != 0);	// 読み込んだ文字列が END_MOTIONSET ではない場合ループ

				// モーション情報の設定
				SetMotionInfo(bodyID, keyInfo);
			}
		} while (nEnd != EOF);	// 読み込んだ文字列が EOF ではない場合ループ
		
		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(nullptr, "プレイヤーセットアップファイルの読み込みに失敗！", "警告！", MB_ICONWARNING);
	}
}
