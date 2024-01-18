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

#include "objectChara.h"
#include "multiModel.h"
#include "sword.h"
#include "shadow.h"
#include "stage.h"
#include "field.h"

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
	const int MODEL_MAX[] =	// モデル最大数
	{
		CPlayer::L_MODEL_MAX,	// 下半身
		CPlayer::U_MODEL_MAX,	// 上半身
	};
	const D3DXVECTOR3 SWORD_OFFSET[] =	// 剣のオフセット位置
	{
		D3DXVECTOR3(-5.8f, 0.0f, 0.0f),	// 右剣
		D3DXVECTOR3(5.8f, 0.0f, 0.0f),	// 左剣
	};

	const int	PRIORITY	= 3;		// プレイヤーの優先順位
	const float	JUMP		= 18.0f;	// ジャンプ上昇量
	const float	GRAVITY		= 0.85f;	// 重力
	const float	RADIUS		= 20.0f;	// 半径
	const float	HEIGHT		= 100.0f;	// 縦幅
	const float	REV_ROTA	= 0.15f;	// 向き変更の補正係数
	const float	MOVE_REV	= 0.5f;		// 移動量の補正係数
	const float	JUMP_REV	= 0.16f;	// 通常状態時の空中の移動量の減衰係数
	const float	LAND_REV	= 0.16f;	// 通常状態時の地上の移動量の減衰係数
	const float	STICK_REV	= 0.00015f;	// 移動操作スティックの傾き量の補正係数
	const float	ADD_ALPHA	= 0.03f;	// 透明度の加算量
	const int	WALK_SOUND	= 4;		// 歩行の効果音のキータイミング

	const D3DXVECTOR3 SHADOW_SIZE	= D3DXVECTOR3(80.0f, 0.0f, 80.0f);	// 影の大きさ
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
static_assert(NUM_ARRAY(SETUP_TXT) == CPlayer::BODY_MAX, "ERROR : Body Count Mismatch");
static_assert(NUM_ARRAY(SWORD_OFFSET) == player::NUM_SWORD, "ERROR : Body Count Mismatch");

//************************************************************
//	子クラス [CPlayer] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CPlayer::CPlayer() : CObjectDivChara(CObject::LABEL_PLAYER, CObject::DIM_3D, PRIORITY),
	m_pShadow		(nullptr),		// 影の情報
	m_oldPos		(VEC3_ZERO),	// 過去位置
	m_move			(VEC3_ZERO),	// 移動量
	m_destRot		(VEC3_ZERO),	// 目標向き
	m_state			(STATE_NONE),	// 状態
	m_bJump			(false),		// ジャンプ状況
	m_bInputAttack	(false),		// 攻撃の先行入力
	m_nCounterState	(0)				// 状態管理カウンター
{

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
	m_pShadow		= nullptr;		// 影の情報
	m_oldPos		= VEC3_ZERO;	// 過去位置
	m_move			= VEC3_ZERO;	// 移動量
	m_destRot		= VEC3_ZERO;	// 目標向き
	m_state			= STATE_NONE;	// 状態
	m_bJump			= true;			// ジャンプ状況
	m_bInputAttack	= false;		// 攻撃の先行入力
	m_nCounterState	= 0;			// 状態管理カウンター

	// オブジェクト分割キャラクターの初期化
	if (FAILED(CObjectDivChara::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
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

		// 剣の生成
		m_apSowrd[nCntSword] = CSword::Create
		( // 引数
			GetMultiModel(BODY_UPPER, U_MODEL_HANDL + nCntSword),	// 親オブジェクト
			SWORD_OFFSET[nCntSword]
		);
		if (m_apSowrd[nCntSword] == nullptr)
		{ // 非使用中の場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}
	}

	// 影の生成
	m_pShadow = CShadow::Create(CShadow::TEXTURE_NORMAL, SHADOW_SIZE, this);
	if (m_pShadow == nullptr)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

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
		UpdateSpawn((int*)&curLowMotion, (int*)&curUpMotion);

		break;

	case STATE_NORMAL:

		// 通常状態の更新
		UpdateNormal((int*)&curLowMotion, (int*)&curUpMotion);

		break;

	default:
		assert(false);
		break;
	}

	// TODO：攻撃モーションデバッグ用
	if (GET_INPUTKEY->IsTrigger(DIK_0))
	{
		curLowMotion = L_MOTION_ATTACK_00;	// 現在の下半身モーション
		curUpMotion  = U_MOTION_ATTACK_00;	// 現在の上半身モーション
	}
	if (GET_INPUTKEY->IsTrigger(DIK_9))
	{
		SetMotion(BODY_LOWER, curLowMotion);
		SetMotion(BODY_UPPER, curUpMotion);
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

	// モーション・オブジェクトキャラクターの更新
	UpdateMotion(curLowMotion, curUpMotion);
}

//============================================================
//	描画処理
//============================================================
void CPlayer::Draw(void)
{
	// オブジェクト分割キャラクターの描画
	CObjectDivChara::Draw();

	for (int nCntSword = 0; nCntSword < player::NUM_SWORD; nCntSword++)
	{ // 剣の数分繰り返す

		// 剣の描画
		m_apSowrd[nCntSword]->Draw();
	}
}

//============================================================
//	ヒット処理
//============================================================
void CPlayer::Hit(void)
{
	// 変数を宣言
	D3DXVECTOR3 posPlayer = GetVec3Position();	// プレイヤー位置
	D3DXVECTOR3 rotPlayer = GetVec3Rotation();	// プレイヤー向き

#if 0

	if (IsDeath() != true)
	{ // 死亡フラグが立っていない場合

		if (m_state == STATE_NORMAL)
		{ // 通常状態の場合

			// カウンターを初期化
			m_nCounterState = 0;

			// 待機モーションを設定
			SetMotion(MOTION_IDOL);

			// 爆発パーティクルを生成
			CParticle3D::Create(CParticle3D::TYPE_SMALL_EXPLOSION, D3DXVECTOR3(posPlayer.x, posPlayer.y + basic::HEIGHT * 0.5f, posPlayer.z));

			// サウンドの再生
			GET_MANAGER->GetSound()->Play(CSound::LABEL_SE_HIT);	// ヒット音
		}
	}

#endif
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
//	出現の設定処理
//============================================================
void CPlayer::SetSpawn(void)
{
	// 変数を宣言
	D3DXVECTOR3 setPos = VEC3_ZERO;	// 位置設定用
	D3DXVECTOR3 setRot = VEC3_ZERO;	// 向き設定用

	// 情報を初期化
	SetState(STATE_SPAWN);	// スポーン状態の設定

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

	// 描画を再開
	SetEnableDraw(true);

	// 待機モーションを設定
	SetMotion(BODY_LOWER, L_MOTION_IDOL);
	SetMotion(BODY_UPPER, U_MOTION_IDOL);

	// マテリアルを再設定
	ResetMaterial();

	// 透明度を透明に再設定
	SetAlpha(0.0f);

	// 追従カメラの目標位置の設定
	GET_MANAGER->GetCamera()->SetDestFollow();

	// サウンドの再生
	GET_MANAGER->GetSound()->Play(CSound::LABEL_SE_SPAWN);	// 生成音
}

//============================================================
//	モーションの設定処理
//============================================================
void CPlayer::SetMotion(const EBody bodyID, const int nType)
{
	if (bodyID > NONE_IDX && bodyID < BODY_MAX)
	{ // 正規インデックスの場合

		// 引数インデックスのモーションを設定
		CObjectDivChara::SetMotion(bodyID, nType);

		for (int nCntSword = 0; nCntSword < player::NUM_SWORD; nCntSword++)
		{ // 剣の数分繰り返す

			// 剣の状態を設定
			m_apSowrd[nCntSword]->SetState((IsWeaponDisp(BODY_UPPER)) ? CSword::STATE_NORMAL : CSword::STATE_VANISH);
		}
	}
	else { assert(false); }	// インデックスエラー
}

//============================================================
//	モーション・オブジェクトキャラクターの更新処理
//============================================================
void CPlayer::UpdateMotion(const int nLowMotion, const int nUpMotion)
{
	if (nLowMotion <= NONE_IDX || nLowMotion >= L_MOTION_MAX) { assert(false); return; }	// 下半身のモーションが未設定
	if (nUpMotion  <= NONE_IDX || nUpMotion  >= L_MOTION_MAX) { assert(false); return; }	// 上半身のモーションが未設定
	if (IsDeath()) { return; }	// 死亡している

	const int aMotion[] = { nLowMotion, nUpMotion };	// モーション情報
	for (int nCntBody = 0; nCntBody < BODY_MAX; nCntBody++)
	{ // 分割した身体の数分繰り返す

		if (IsMotionLoop((EBody)nCntBody))
		{ // ループするモーションの場合

			if (GetMotionType((EBody)nCntBody) != aMotion[nCntBody])
			{ // 現在のモーションが再生中のモーションと一致しない場合

				// 現在のモーションの設定
				SetMotion((EBody)nCntBody, aMotion[nCntBody]);
			}
		}

		// 各半身ごとのモーション更新
		(this->*(m_aFuncUpdateMotion[nCntBody]))(aMotion[nCntBody]);
	}

	// オブジェクト分割キャラクターの更新
	CObjectDivChara::Update();
}

//============================================================
//	下半身モーションの更新処理
//============================================================
void CPlayer::UpdateMotionLower(const int nMotion)
{
	switch (GetMotionType(BODY_LOWER))
	{ // モーションごとの処理
	case L_MOTION_IDOL:	// 待機モーション：ループON
		assert(!m_bInputAttack);

		break;

	case L_MOTION_MOVE:	// 移動モーション：ループON

		if (GetMotionPose(BODY_LOWER) % WALK_SOUND == 0 && GetMotionKeyCounter(BODY_LOWER) == 0)
		{ // 足がついたタイミングの場合
			assert(!m_bInputAttack);

			// 歩行音の再生
			PLAY_SOUND(CSound::LABEL_SE_WALK_BUILD);
		}

		break;
	
	case L_MOTION_ATTACK_00:	// 攻撃モーション一段階目：ループOFF
	
		if (IsMotionCancel(BODY_LOWER))
		{ // モーションキャンセルができる場合

			if (m_bInputAttack)
			{ // 攻撃が先行入力されている場合

				// 現在のモーションの設定
				SetMotion(BODY_LOWER, L_MOTION_ATTACK_01);

				// 先行入力を初期化
				m_bInputAttack = false;

				// 処理を抜ける
				break;
			}
		}

		if (IsMotionFinish(BODY_LOWER))
		{ // モーションが終了した場合

			// 現在のモーションの設定
			SetMotion(BODY_LOWER, nMotion);

			// 先行入力を初期化
			m_bInputAttack = false;
		}

		break;

	case L_MOTION_ATTACK_01:	// 攻撃モーション二段階目：ループOFF

		if (IsMotionFinish(BODY_LOWER))
		{ // モーションが終了していた場合

			// 現在のモーションの設定
			SetMotion(BODY_LOWER, nMotion);
		}

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
	switch (GetMotionType(BODY_UPPER))
	{ // モーションごとの処理
	case U_MOTION_IDOL:	// 待機モーション：ループON
	case U_MOTION_MOVE:	// 移動モーション：ループON
		assert(!m_bInputAttack);

		break;
	
	case U_MOTION_ATTACK_00:	// 攻撃モーション一段階目：ループOFF
	
		if (IsMotionCancel(BODY_UPPER))
		{ // モーションキャンセルができる場合

			if (m_bInputAttack)
			{ // 攻撃が先行入力されている場合

				// 現在のモーションの設定
				SetMotion(BODY_UPPER, U_MOTION_ATTACK_01);

				// 先行入力を初期化
				m_bInputAttack = false;

				// 処理を抜ける
				break;
			}
		}

		if (IsMotionFinish(BODY_UPPER))
		{ // モーションが終了した場合

			// 現在のモーションの設定
			SetMotion(BODY_UPPER, nMotion);

			// 先行入力を初期化
			m_bInputAttack = false;
		}
	
		break;

	case U_MOTION_ATTACK_01:	// 攻撃モーション二段階目：ループOFF

		if (IsMotionFinish(BODY_UPPER))
		{ // モーションが終了していた場合

			// 現在のモーションの設定
			SetMotion(BODY_UPPER, nMotion);
		}

		break;
	
	default:	// 例外処理
		assert(false);
		break;
	}
}

//============================================================
//	スポーン状態時の更新処理
//============================================================
void CPlayer::UpdateSpawn(int *pLowMotion, int *pUpMotion)
{
	// フェードアウト状態時の更新
	if (UpdateFadeOut(ADD_ALPHA))
	{ // 不透明になり切った場合

		// 状態を設定
		SetState(STATE_NORMAL);
	}

	// 通常状態の更新
	UpdateNormal(pLowMotion, pUpMotion);
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

	// 移動操作・目標向き設定
	UpdateMove(pLowMotion, pUpMotion);

	// 攻撃操作
	UpdateAttack(pLowMotion, pUpMotion);

	// ジャンプ操作
	UpdateJump(pLowMotion, pUpMotion);

	// 位置更新
	UpdatePosition(&posPlayer);

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
//	剣の表示設定処理
//============================================================
void CPlayer::SetSwordDisp(const bool bDisp)
{
	if (bDisp)
	{ // 見える設定された場合

		for (int nCntSword = 0; nCntSword < player::NUM_SWORD; nCntSword++)
		{ // 剣の数分繰り返す

			// 剣を出す
			m_apSowrd[nCntSword]->SetState(CSword::STATE_NORMAL);
		}
	}
	else
	{ // 見えない設定された場合

		for (int nCntSword = 0; nCntSword < player::NUM_SWORD; nCntSword++)
		{ // 剣の数分繰り返す

			// 剣を消す
			m_apSowrd[nCntSword]->SetState(CSword::STATE_VANISH);
		}
	}
}

//============================================================
//	移動操作・目標向きの更新処理
//============================================================
void CPlayer::UpdateMove(int *pLowMotion, int *pUpMotion)
{
	// ポインタを宣言
	CInputPad *pPad  = GET_INPUTPAD;				// パッド
	CCamera *pCamera = GET_MANAGER->GetCamera();	// カメラ

	// 変数を宣言
	D3DXVECTOR3 vecLStick = D3DXVECTOR3((float)pPad->GetPressLStickX(), (float)pPad->GetPressLStickY(), 0.0f);	// スティック各軸の倒し量
	float fLStick = sqrtf(vecLStick.x * vecLStick.x + vecLStick.y * vecLStick.y) * 0.5f;	// スティックの倒し量

	if (pad::DEAD_ZONE < fLStick)
	{ // デッドゾーン以上の場合

		// 変数を宣言
		float fMove = fLStick * STICK_REV;	// プレイヤー移動量

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
	if (GET_INPUTPAD->IsTrigger(CInputPad::KEY_A))
	{
		if (!m_bJump)
		{ // ジャンプしていない場合

			// 上移動量を与える
			m_move.y += JUMP;

			// ジャンプ中にする
			m_bJump = true;
		}
	}
}

//============================================================
//	攻撃操作の更新処理
//============================================================
void CPlayer::UpdateAttack(int *pLowMotion, int *pUpMotion)
{
	if (GET_INPUTPAD->IsTrigger(CInputPad::KEY_X))
	{
		// 現在のモーションを取得
		EUpperMotion curMotion = (EUpperMotion)GetMotionType(BODY_UPPER);

		if (curMotion != U_MOTION_ATTACK_00
		&&  curMotion != U_MOTION_ATTACK_01)
		{ // 攻撃中ではない場合	// TODO：攻撃追加したら記述

			// 攻撃モーションを指定
			*pLowMotion = L_MOTION_ATTACK_00;
			*pUpMotion  = U_MOTION_ATTACK_00;
		}
		else
		{ // 攻撃中の場合

			if (curMotion != U_MOTION_ATTACK_01)	// TODO：一番最後の攻撃にする
			{ // 最終攻撃モーションではない場合

				// 現在のモーションの残りフレームを計算
				int nWholeFrame = GetMotionWholeFrame(BODY_UPPER) - GetMotionWholeCounter(BODY_UPPER);
				if (nWholeFrame < 10)	// TODO：マジックナンバー
				{ // 先行入力が可能な場合

					// 先行入力を受け付ける
					m_bInputAttack = true;
				}
			}
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
bool CPlayer::UpdateLanding(D3DXVECTOR3 *pPos)
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
void CPlayer::UpdatePosition(D3DXVECTOR3 *pPos)
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
	CMotion::SMotionInfo info;		// ポーズの代入用
	D3DXVECTOR3 pos = VEC3_ZERO;	// 位置の代入用
	D3DXVECTOR3 rot = VEC3_ZERO;	// 向きの代入用
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
				memset(&info, 0, sizeof(info));

				// キャンセルフレームをなしにする
				info.nCancelFrame = NONE_IDX;

				// 攻撃判定情報を初期化
				info.collLeft.nMin  = NONE_IDX;
				info.collLeft.nMax  = NONE_IDX;
				info.collRight.nMin = NONE_IDX;
				info.collRight.nMax = NONE_IDX;

				// 武器表示をOFFにする
				info.bWeaponDisp = false;

				do
				{ // 読み込んだ文字列が END_MOTIONSET ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "WEAPON") == 0)
					{ // 読み込んだ文字列が WEAPON の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &nWeapon);		// 武器表示のON/OFFを読み込む

						// 読み込んだ値をbool型に変換
						info.bWeaponDisp = (nWeapon == 0) ? false : true;
					}
					else if (strcmp(&aString[0], "LOOP") == 0)
					{ // 読み込んだ文字列が LOOP の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &nLoop);		// ループのON/OFFを読み込む

						// 読み込んだ値をbool型に変換
						info.bLoop = (nLoop == 0) ? false : true;
					}
					else if (strcmp(&aString[0], "NUM_KEY") == 0)
					{ // 読み込んだ文字列が NUM_KEY の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &info.nNumKey);	// キーの総数を読み込む
					}
					else if (strcmp(&aString[0], "CANCEL") == 0)
					{ // 読み込んだ文字列が CANCEL の場合

						fscanf(pFile, "%s", &aString[0]);			// = を読み込む (不要)
						fscanf(pFile, "%d", &info.nCancelFrame);	// キャンセル可能フレームを読み込む
					}
					else if (strcmp(&aString[0], "LEFT_COLL") == 0)
					{ // 読み込んだ文字列が LEFT_COLL の場合

						fscanf(pFile, "%s", &aString[0]);			// = を読み込む (不要)
						fscanf(pFile, "%d", &info.collLeft.nMin);	// 判定を出す開始フレームを読み込む
						fscanf(pFile, "%d", &info.collLeft.nMax);	// 判定を消す終了フレームを読み込む
					}
					else if (strcmp(&aString[0], "RIGHT_COLL") == 0)
					{ // 読み込んだ文字列が RIGHT_COLL の場合

						fscanf(pFile, "%s", &aString[0]);			// = を読み込む (不要)
						fscanf(pFile, "%d", &info.collRight.nMin);	// 判定を出す開始フレームを読み込む
						fscanf(pFile, "%d", &info.collRight.nMax);	// 判定を消す終了フレームを読み込む
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

								fscanf(pFile, "%s", &aString[0]);						// = を読み込む (不要)
								fscanf(pFile, "%d", &info.aKeyInfo[nNowPose].nFrame);	// キーが切り替わるまでのフレーム数を読み込む
							}
							else if (strcmp(&aString[0], "KEY") == 0)
							{ // 読み込んだ文字列が KEY の場合

								do
								{ // 読み込んだ文字列が END_KEY ではない場合ループ

									// ファイルから文字列を読み込む
									fscanf(pFile, "%s", &aString[0]);

									if (strcmp(&aString[0], "POS") == 0)
									{ // 読み込んだ文字列が POS の場合

										fscanf(pFile, "%s", &aString[0]);									// = を読み込む (不要)
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].pos.x);	// X位置を読み込む
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].pos.y);	// Y位置を読み込む
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].pos.z);	// Z位置を読み込む

										// 読み込んだ位置にパーツの初期位置を加算
										info.aKeyInfo[nNowPose].aKey[nNowKey].pos += GetPartsPosition(bodyID, nNowKey);
									}
									else if (strcmp(&aString[0], "ROT") == 0)
									{ // 読み込んだ文字列が ROT の場合

										fscanf(pFile, "%s", &aString[0]);									// = を読み込む (不要)
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].rot.x);	// X向きを読み込む
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].rot.y);	// Y向きを読み込む
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].rot.z);	// Z向きを読み込む

										// 読み込んだ向きにパーツの初期向きを加算
										info.aKeyInfo[nNowPose].aKey[nNowKey].rot += GetPartsRotation(bodyID, nNowKey);

										// 初期向きを正規化
										useful::NormalizeRot(info.aKeyInfo[nNowPose].aKey[nNowKey].rot.x);
										useful::NormalizeRot(info.aKeyInfo[nNowPose].aKey[nNowKey].rot.y);
										useful::NormalizeRot(info.aKeyInfo[nNowPose].aKey[nNowKey].rot.z);
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
				SetMotionInfo(bodyID, info);
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
