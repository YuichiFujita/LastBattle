//============================================================
//
//	ボスドラゴン処理 [enemyBossDragon.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "enemyBossDragon.h"
#include "enemyAttack.h"
#include "manager.h"
#include "renderer.h"
#include "stage.h"
#include "multiModel.h"
#include "sceneGame.h"
#include "gameManager.h"
#include "magicCircle.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char *MODEL_FILE[] =	// モデルファイル
	{
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\00_waist.x",		// 腰
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\01_body.x",		// 体
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\02_jaw.x",		// 顎
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\03_head.x",		// 頭
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\04_armUL.x",		// 左上腕
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\05_armUR.x",		// 右上腕
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\06_armDL.x",		// 左下腕
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\07_armDR.x",		// 右下腕
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\08_handL.x",		// 左手
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\09_handR.x",		// 右手
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\10_legUL.x",		// 左太もも
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\11_legUR.x",		// 右太もも
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\12_legDL.x",		// 左脛
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\13_legDR.x",		// 右脛
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\14_footL.x",		// 左足
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\15_footR.x",		// 右足
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\16_wingL_00.x",	// 左翼00
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\17_wingR_00.x",	// 右翼00
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\18_wingL_01.x",	// 左翼01
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\19_wingR_01.x",	// 右翼01
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\20_wingL_02.x",	// 左翼02
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\21_wingR_02.x",	// 右翼02
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\22_wingL_03.x",	// 左翼03
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\23_wingR_03.x",	// 右翼03
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\24_tail_00.x",	// 尻尾00
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\25_tail_01.x",	// 尻尾01
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\26_tail_02.x",	// 尻尾02
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\27_tail_03.x",	// 尻尾03
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\28_tail_04.x",	// 尻尾04
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\29_tail_05.x",	// 尻尾05
	};

	const int	PRIORITY		= 6;			// ボスドラゴンの優先順位
	const int	ATK_WAIT_FRAME	= 60;			// 攻撃の余韻フレーム
	const float	SCALE_MAGIC		= 55.0f;		// 魔法陣の半径変動量
	const float	MOVE_MAGIC		= 50.0f;		// 魔法陣の上下移動量
	const float	MAGIC_CIRCLE_RADIUS	= 250.0f;	// 魔法陣の半径
	const float	MAGIC_ALPHA_RADIUS	= 350.0f;	// 魔法陣の半径
	const float	MAGIC_DELPOS_PLUSY	= 150.0f;	// 魔法陣の消失位置の加算量Y
}

//************************************************************
//	スタティックアサート
//************************************************************
static_assert(NUM_ARRAY(MODEL_FILE) == CEnemyBossDragon::MODEL_MAX, "ERROR : Model Count Mismatch");

//************************************************************
//	子クラス [CEnemyBossDragon] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEnemyBossDragon::CEnemyBossDragon(const EType type) : CEnemy(type),
	m_pAttack			(nullptr),	// 攻撃の情報
	m_pMagicCircle		(nullptr),	// 魔法陣の情報
	m_action			(ACT_NONE),	// 行動
	m_nCounterAttack	(0)			// 攻撃管理カウンター
{
	// メンバ変数をクリア
	memset(&m_teleport, 0, sizeof(m_teleport));	// テレポートの情報
}

//============================================================
//	デストラクタ
//============================================================
CEnemyBossDragon::~CEnemyBossDragon()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CEnemyBossDragon::Init(void)
{
	// メンバ変数を初期化
	memset(&m_teleport, 0, sizeof(m_teleport));	// テレポートの情報
	m_pAttack			= nullptr;	// 攻撃の情報
	m_pMagicCircle		= nullptr;	// 魔法陣の情報
	m_action			= ACT_NONE;	// 行動
	m_nCounterAttack	= 0;		// 攻撃管理カウンター

	// 敵の初期化
	if (FAILED(CEnemy::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 優先順位を設定
	SetPriority(PRIORITY);

	// スポーン状態にする
	SetState(STATE_SPAWN);

	// 魔法陣の生成
	m_pMagicCircle = CMagicCircle::Create(VEC3_ZERO, VEC3_ZERO, 0.0f, MAGIC_ALPHA_RADIUS);
	if (m_pMagicCircle == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 自動描画をOFFにする
	m_pMagicCircle->SetEnableDraw(false);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CEnemyBossDragon::Uninit(void)
{
	// 攻撃の破棄
	SAFE_REF_RELEASE(m_pAttack);

	// 敵の終了
	CEnemy::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CEnemyBossDragon::Update(void)
{
	// 敵の更新
	CEnemy::Update();
}

//============================================================
//	描画処理
//============================================================
void CEnemyBossDragon::Draw(void)
{
	// 敵の描画
	CEnemy::Draw();
}

//============================================================
//	ヒット処理
//============================================================
void CEnemyBossDragon::Hit(const int nDamage)
{
	// ヒット処理
	CEnemy::Hit(nDamage);

	if (GetLife() <= 0)
	{ // 体力がなくなった場合

		if (GET_MANAGER->GetMode() == CScene::MODE_GAME)
		{ // ゲーム画面の場合

			// ゲーム画面を終了状態にする
			CSceneGame::GetGameManager()->SetState(CGameManager::STATE_END);
		}
	}
}

//============================================================
//	ノックバックヒット処理
//============================================================
void CEnemyBossDragon::HitKnockBack(const int nDamage, const D3DXVECTOR3& vecKnock)
{
	// ノックバックヒット処理
	CEnemy::HitKnockBack(nDamage, vecKnock);
	
	if (GetLife() <= 0)
	{ // 体力がなくなった場合

		if (GET_MANAGER->GetMode() == CScene::MODE_GAME)
		{ // ゲーム画面の場合

			// ゲーム画面を終了状態にする
			CSceneGame::GetGameManager()->SetState(CGameManager::STATE_END);
		}
	}
}

//============================================================
//	テレポートの設定処理
//============================================================
void CEnemyBossDragon::SetTeleport(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rRot)
{
	// テレポート後の位置・向きを保存
	m_teleport.pos = rPos;
	m_teleport.rot = rRot;

	// 魔法陣の位置・向きを現在位置の足元に設定
	m_pMagicCircle->SetVec3Position(GetVec3Position());
	m_pMagicCircle->SetVec3Rotation(GetVec3Rotation());

	// 魔法陣の自動描画をONにする
	m_pMagicCircle->SetEnableDraw(true);

	// 魔法陣の出現状態にする
	m_teleport.state = TELEPORT_APPEAR;

	// 魔法陣によるフェードインの行動をとらせる
	m_action = ACT_MAGIC_FADEIN;
}

//============================================================
//	モデルファイル取得処理
//============================================================
const char *CEnemyBossDragon::GetModelFileName(const int nModel) const
{
	if (nModel > NONE_IDX && nModel < MODEL_MAX)
	{ // 使用できるインデックスの場合

		// 引数のインデックスのモデルを返す
		return MODEL_FILE[nModel];
	}
	else { assert(false); return NONE_STRING; }	// 範囲外
}

//============================================================
//	通常状態時の更新処理
//============================================================
void CEnemyBossDragon::UpdateNormal(void)
{
	// TODO：テレポートお試し
	if (GET_INPUTKEY->IsTrigger(DIK_0))
	{
		SetTeleport(VEC3_ZERO, VEC3_ZERO);
	}

	// 攻撃の更新
	UpdateAttack();

	// 行動の更新
	UpdateAction();
}

//============================================================
//	攻撃の更新処理
//============================================================
void CEnemyBossDragon::UpdateAttack(void)
{
	if (m_pAttack == nullptr)
	{ // 攻撃が存在しない場合

		// カウンターを加算
		m_nCounterAttack++;
		if (m_nCounterAttack > ATK_WAIT_FRAME)
		{ // 攻撃の余韻が終了した場合

			// カウンターを初期化
			m_nCounterAttack = 0;

			// 攻撃の生成
			m_pAttack = CEnemyAttack::Create
			( // 引数
				(CEnemyAttack::EAttack)(rand() % CEnemyAttack::ATTACK_MAX),	// 攻撃インデックス
				this	// 自身のポインタ
			);
			if (m_pAttack == nullptr)
			{ // 生成に失敗した場合

				// 関数を抜ける
				assert(false);
				return;
			}
		}
	}
	else
	{ // 攻撃が存在する場合

		// 攻撃の更新
		if (m_pAttack->Update())
		{ // 攻撃が終了している場合

			// 攻撃の破棄
			SAFE_REF_RELEASE(m_pAttack);

			// 何もしない行動をとらせる
			m_action = ACT_NONE;
		}
	}
}

//============================================================
//	行動の更新処理
//============================================================
void CEnemyBossDragon::UpdateAction(void)
{
	// 変数を宣言
	D3DXVECTOR3 posEnemy = GetVec3Position();	// 敵位置
	D3DXVECTOR3 rotEnemy = GetVec3Rotation();	// 敵向き

	// ポインタを宣言
	CStage *pStage = CScene::GetStage();				// ステージ情報
	if (pStage == nullptr) { assert(false); return; }	// ステージ非使用中

	// 過去位置の更新
	UpdateOldPosition();

	// 重力の更新
	UpdateGravity();

	switch (m_action)
	{ // 行動ごとの処理
	case ACT_NONE:	// 何もしない
		break;

	case ACT_MAGIC_FADEIN:	// 魔法陣フェードイン

		// 魔法陣フェードイン行動時の更新
		UpdateMagicFadeIn(&posEnemy, &rotEnemy);

		break;

	case ACT_MAGIC_FADEOUT:	// 魔法陣フェードアウト

		// 魔法陣フェードアウト行動時の更新
		UpdateMagicFadeOut(posEnemy);

		break;

	case ACT_PUNCH_GROUND:	// 地面殴り

		// 地面殴り行動時の更新
		UpdatePunchGround();

		break;

	default:	// 例外処理
		assert(false);
		break;
	}

	// 位置更新
	UpdatePosition(&posEnemy);

	// 着地判定
	UpdateLanding(&posEnemy);

	// ステージ範囲外の補正
	pStage->LimitPosition(posEnemy, GetStatusInfo().fRadius);

	// 位置を反映
	SetVec3Position(posEnemy);

	// 向きを反映
	SetVec3Rotation(rotEnemy);
}

//============================================================
//	魔法陣フェードイン行動時の更新処理
//============================================================
void CEnemyBossDragon::UpdateMagicFadeIn(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pRot)
{
	// 変数を宣言
	float fMagicPosY = m_pMagicCircle->GetVec3Position().y;	// 魔法陣の位置

	switch (m_teleport.state)
	{ // テレポート状態ごとの処理
	case TELEPORT_APPEAR:	// 魔法陣の出現
	{
		// 変数を宣言
		float fMagicRadius = m_pMagicCircle->GetRadius();	// 魔法陣の半径

		// 魔法陣を拡大
		fMagicRadius += SCALE_MAGIC;
		if (fMagicRadius >= MAGIC_CIRCLE_RADIUS)
		{ // 拡大しきった場合

			// 半径を補正
			fMagicRadius = MAGIC_CIRCLE_RADIUS;

			// 魔法陣の移動状態にする
			m_teleport.state = TELEPORT_MOVE;
		}

		// 魔法陣の半径を反映
		m_pMagicCircle->SetRadius(fMagicRadius);

		break;
	}
	case TELEPORT_MOVE:	// 魔法陣の移動
	{
		// 変数を宣言
		float fDelPosY = GetMultiModel(MODEL_HEAD)->GetMtxWorld()._42 + MAGIC_DELPOS_PLUSY;	// 魔法陣の消失位置

		// 魔法陣を上に動かす
		fMagicPosY += MOVE_MAGIC;
		if (fMagicPosY >= fDelPosY)
		{ // 消失位置に到達した場合

			// 位置を補正
			fMagicPosY = fDelPosY;

			// 魔法陣の消失状態にする
			m_teleport.state = TELEPORT_DISAPPEAR;
		}

		break;
	}
	case TELEPORT_DISAPPEAR:	// 魔法陣の消失
	{
		// 変数を宣言
		float fMagicRadius = m_pMagicCircle->GetRadius();	// 魔法陣の半径

		// 魔法陣を縮小
		fMagicRadius -= SCALE_MAGIC;
		if (fMagicRadius <= 0.0f)
		{ // 縮小しきった場合

			// 半径を補正
			fMagicRadius = 0.0f;

			// ボスの位置・向きをテレポート先に変更
			*pPos = m_teleport.pos;
			*pRot = m_teleport.rot;

			// 魔法陣の出現状態にする
			m_teleport.state = TELEPORT_APPEAR;

			// 魔法陣によるフェードアウトの行動をとらせる
			m_action = ACT_MAGIC_FADEOUT;
		}

		// 魔法陣の半径を反映
		m_pMagicCircle->SetRadius(fMagicRadius);

		break;
	}
	default:	// 例外処理
		assert(false);
		break;
	}

	// 魔法陣の位置を反映
	m_pMagicCircle->SetVec3Position(D3DXVECTOR3(pPos->x, fMagicPosY, pPos->z));
}

//============================================================
//	魔法陣フェードアウト行動時の更新処理
//============================================================
void CEnemyBossDragon::UpdateMagicFadeOut(const D3DXVECTOR3& rPos)
{
	// 変数を宣言
	float fMagicPosY = m_pMagicCircle->GetVec3Position().y;	// 魔法陣の位置

	switch (m_teleport.state)
	{ // テレポート状態ごとの処理
	case TELEPORT_APPEAR:	// 魔法陣の出現
	{
		// 変数を宣言
		float fMagicRadius = m_pMagicCircle->GetRadius();	// 魔法陣の半径

		// 魔法陣を拡大
		fMagicRadius += SCALE_MAGIC;
		if (fMagicRadius >= MAGIC_CIRCLE_RADIUS)
		{ // 拡大しきった場合

			// 半径を補正
			fMagicRadius = MAGIC_CIRCLE_RADIUS;

			// 魔法陣の移動状態にする
			m_teleport.state = TELEPORT_MOVE;
		}

		// 魔法陣の半径を反映
		m_pMagicCircle->SetRadius(fMagicRadius);

		break;
	}
	case TELEPORT_MOVE:	// 魔法陣の移動
	{
		// 変数を宣言
		float fDelPosY = GetVec3Position().y;	// 魔法陣の消失位置

		// 魔法陣を下に動かす
		fMagicPosY -= MOVE_MAGIC;
		if (fMagicPosY <= fDelPosY)
		{ // 消失位置に到達した場合

			// 位置を補正
			fMagicPosY = fDelPosY;

			// 魔法陣の消失状態にする
			m_teleport.state = TELEPORT_DISAPPEAR;
		}

		break;
	}
	case TELEPORT_DISAPPEAR:	// 魔法陣の消失
	{
		// 変数を宣言
		float fMagicRadius = m_pMagicCircle->GetRadius();	// 魔法陣の半径

		// 魔法陣を縮小
		fMagicRadius -= SCALE_MAGIC;
		if (fMagicRadius <= 0.0f)
		{ // 縮小しきった場合

			// 半径を補正
			fMagicRadius = 0.0f;

			// 魔法陣の自動描画をOFFにする
			m_pMagicCircle->SetEnableDraw(false);

			// 何もしない行動をとらせる
			m_action = ACT_NONE;
		}

		// 魔法陣の半径を反映
		m_pMagicCircle->SetRadius(fMagicRadius);

		break;
	}
	default:	// 例外処理
		assert(false);
		break;
	}

	// 魔法陣の位置を反映
	m_pMagicCircle->SetVec3Position(D3DXVECTOR3(rPos.x, fMagicPosY, rPos.z));
}

//============================================================
//	地面殴り行動時の更新処理
//============================================================
void CEnemyBossDragon::UpdatePunchGround(void)
{

}
