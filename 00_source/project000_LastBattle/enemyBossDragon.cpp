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
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "stage.h"
#include "camera.h"
#include "multiModel.h"
#include "sceneGame.h"
#include "gameManager.h"
#include "gauge2D.h"
#include "magicCircle.h"
#include "retentionManager.h"
#include "random.h"

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

	const CCamera::SSwing LAND_SWING = CCamera::SSwing(10.0f, 1.5f, 0.12f);		// 着地のカメラ揺れ
	const CCamera::SSwing HOWL_SWING = CCamera::SSwing(14.0f, 2.0f, 0.075f);	// 咆哮のカメラ揺れ
	const int	BLEND_FRAME		= 16;			// モーションのブレンドフレーム
	const int	LAND_MOTION_KEY	= 9;			// モーションの着地の瞬間キー
	const int	HOWL_MOTION_KEY	= 13;			// モーションの咆哮の開始キー
	const int	HOWL_WAIT_FRAME	= 40;			// 咆哮の余韻フレーム
	const int	ATK_WAIT_FRAME	= 30;			// 攻撃の余韻フレーム
	const float	REV_ROTA		= 0.15f;		// 向き変更の補正係数
	const float	SCALE_MAGIC		= 35.0f;		// 魔法陣の半径変動量
	const float	MOVE_MAGIC		= 30.0f;		// 魔法陣の上下移動量
	const float	MAGIC_CIRCLE_RADIUS	= 400.0f;	// 魔法陣の半径
	const float	MAGIC_ALPHA_RADIUS	= 400.0f;	// 魔法陣の透明半径
	const float	MAGIC_DELPOS_PLUSY	= 250.0f;	// 魔法陣の消失位置の加算量Y

	// 体力の情報
	namespace lifeInfo
	{
		const char *TEXTURE_FRAME = "data\\TEXTURE\\lifeframe001.png";	// 体力フレーム表示のテクスチャファイル

		const D3DXVECTOR3	POS				= D3DXVECTOR3(760.0f, 650.0f, 0.0f);			// 位置
		const D3DXCOLOR		COL_FRONT		= D3DXCOLOR(0.77f, 0.19f, 0.94f, 1.0f);			// 表ゲージ色
		const D3DXCOLOR		COL_BACK		= D3DXCOLOR(0.02f, 0.008f, 0.03f, 1.0f);		// 裏ゲージ色
		const D3DXVECTOR3	SIZE_GAUGE		= D3DXVECTOR3(460.0f, 20.0f, 0.0f);				// ゲージ大きさ
		const D3DXVECTOR3	SIZE_FRAME		= SIZE_GAUGE + D3DXVECTOR3(16.5f, 16.5f, 0.0f);	// フレーム大きさ
		const int			CHANGE_FRAME	= 10;	// 表示値変動フレーム
	}
}

//************************************************************
//	マクロ定義
//************************************************************
// ランダム攻撃のON/OFF
#if 1
#define RANDOM_ATTACK_ON	// ランダム攻撃
#define ATTACK (CEnemyAttack::ATTACK_03)
#endif

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
	m_pLife				(nullptr),			// 体力の情報
	m_pAttack			(nullptr),			// 攻撃の情報
	m_pMagicCircle		(nullptr),			// 魔法陣の情報
	m_action			(ACT_NONE),			// 行動
	m_nCounterSameAct	(0),				// 同じ行動の連続数
	m_nCounterAttack	(0),				// 攻撃管理カウンター
	m_oldAtk	(CEnemyAttack::ATTACK_00),	// 前回の攻撃
	m_curAtk	(CEnemyAttack::ATTACK_00)	// 今回の攻撃
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
	memset(&m_teleport, 0, sizeof(m_teleport));		// テレポートの情報
	m_oldAtk			= CEnemyAttack::ATTACK_00;	// 前回の攻撃
	m_curAtk			= CEnemyAttack::ATTACK_00;	// 今回の攻撃
	m_pLife				= nullptr;	// 体力の情報
	m_pAttack			= nullptr;	// 攻撃の情報
	m_pMagicCircle		= nullptr;	// 魔法陣の情報
	m_action			= ACT_NONE;	// 行動
	m_nCounterSameAct	= 0;		// 同じ行動の連続数
	m_nCounterAttack	= 0;		// 攻撃管理カウンター

	// 敵の初期化
	if (FAILED(CEnemy::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 体力の生成
	m_pLife = CGauge2D::Create
	( // 引数
		GetStatusInfo().nMaxLife,	// 最大表示値
		lifeInfo::CHANGE_FRAME,		// 表示値変動フレーム
		lifeInfo::POS,				// 位置
		lifeInfo::SIZE_GAUGE,		// ゲージ大きさ
		lifeInfo::COL_FRONT,		// 表ゲージ色
		lifeInfo::COL_BACK,			// 裏ゲージ色
		true,						// 枠描画状況
		lifeInfo::TEXTURE_FRAME,	// フレームテクスチャパス
		lifeInfo::SIZE_FRAME		// 枠大きさ
	);

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

	// なにもしない状態にする
	SetState(STATE_NONE);

	// 自動描画をOFFにする
	SetEnableDraw(false);

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
void CEnemyBossDragon::Draw(CShader *pShader)
{
	CDrawBossShader	*pDrawBossShader = CDrawBossShader::GetInstance();	// ボス描画シェーダー情報
	if (pDrawBossShader->IsEffectOK())
	{ // エフェクトが使用可能な場合

		// 敵の描画
		CEnemy::Draw(pDrawBossShader);
	}
	else
	{ // エフェクトが使用不可能な場合

		// エフェクトエラー
		assert(false);

		// 敵の描画
		CEnemy::Draw(pShader);
	}
}

//============================================================
//	腰モデルのインデックス取得処理
//============================================================
int CEnemyBossDragon::GetWaistModelID(void) const
{
	// 腰モデルのインデックスを返す
	return MODEL_WAIST;
}

//============================================================
//	頭モデルのインデックス取得処理
//============================================================
int CEnemyBossDragon::GetHeadModelID(void) const
{
	// 頭モデルのインデックスを返す
	return MODEL_HEAD;
}

//============================================================
//	UI描画の設定処理
//============================================================
void CEnemyBossDragon::SetEnableDrawUI(const bool bDraw)
{
	// UI描画の設定
	CEnemy::SetEnableDrawUI(bDraw);

	// UIオブジェクトに描画状況を反映
	m_pLife->SetEnableDraw(bDraw);	// 体力
}

//============================================================
//	ヒット処理
//============================================================
bool CEnemyBossDragon::Hit(const int nDamage)
{
	if (IsDeath())						{ return false; }	// 死亡済み
	if (GetState() != STATE_NORMAL)		{ return false; }	// 通常状態以外
	if (m_action == ACT_MAGIC_FADEIN)	{ return false; }	// 魔法陣フェードイン中
	if (m_action == ACT_MAGIC_FADEOUT)	{ return false; }	// 魔法陣フェードアウト中
	if (m_pLife->GetNum() <= 0)			{ return false; }	// 体力なし

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

		if (GET_MANAGER->GetMode() == CScene::MODE_GAME)
		{ // ゲーム画面の場合

			// リザルト画面に遷移させる
			CSceneGame::GetGameManager()->TransitionResult(CRetentionManager::WIN_CLEAR);
		}
	}

	return true;
}

//============================================================
//	ノックバックヒット処理
//============================================================
bool CEnemyBossDragon::HitKnockBack(const int /*nDamage*/, const D3DXVECTOR3 & /*vecKnock*/)
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
	PLAY_SOUND->Play(CSound::LABEL_SE_HIT);	// ヒット音

#endif

	return false;
}

//============================================================
//	通常状態の初期化処理
//============================================================
void CEnemyBossDragon::InitNormal(void)
{
	// 変数を宣言
	D3DXVECTOR3 posEnemy = GetVec3Position();	// 敵位置

	// 通常状態の初期化
	CEnemy::InitNormal();

	// 通常状態にする
	SetState(STATE_NORMAL);

	// 自動描画をONにする
	SetEnableDraw(true);

	// 透明度を不透明に再設定
	SetAlpha(1.0f);

	// マテリアルを再設定
	ResetMaterial();

	// 待機モーションを設定
	SetMotion(MOTION_IDOL);

	// 着地判定
	UpdateLanding(&posEnemy);

	// 位置範囲外の補正
	LimitPosition(&posEnemy);

	// 位置を反映
	SetVec3Position(posEnemy);
}

//============================================================
//	切り抜き用の描画処理
//============================================================
void CEnemyBossDragon::DrawCrop(void)
{
	// 自動描画がOFFの場合抜ける
	if (!IsDraw()) { return; }

	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// デバイス情報
	CMonoShader	*pMonoShader  = CMonoShader::GetInstance();	// 単色描画シェーダー情報
	if (pMonoShader->IsEffectOK())
	{ // エフェクトが使用可能な場合

		// ピクセル描画色を画面クリア色に設定
		pMonoShader->SetColor(GET_RENDERER->GetClearColor());

		// ステンシルテストを有効にする
		pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

		// 比較参照値を設定する
		pDevice->SetRenderState(D3DRS_STENCILREF, 1);

		// ステンシルマスクを指定する 
		pDevice->SetRenderState(D3DRS_STENCILMASK, 255);

		// ステンシル比較関数を指定する
		pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_GREATEREQUAL);

		// ステンシル結果に対しての反映設定
		pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);	// Zテスト・ステンシルテスト成功
		pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);		// Zテスト・ステンシルテスト失敗
		pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);		// Zテスト失敗・ステンシルテスト成功

		// 敵の描画
		CEnemy::Draw(pMonoShader);

		// ステンシルテストを無効にする
		pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
	}
	else { assert(false); }
}

//============================================================
//	テレポートの設定処理
//============================================================
void CEnemyBossDragon::SetTeleport
(
	const D3DXVECTOR3 &rPos,	// テレポート目標位置
	const D3DXVECTOR3 &rRot,	// テレポート目標向き
	const EMotion motion,		// テレポート後モーション
	const bool bLook			// テレポート先にカメラを向かせるか
)
{
	// テレポート後の位置・向きを保存
	m_teleport.pos = rPos;
	m_teleport.rot = rRot;

	// 魔法陣の位置・向きを現在位置の足元に設定
	m_pMagicCircle->SetVec3Position(GetVec3Position());
	m_pMagicCircle->SetVec3Rotation(GetVec3Rotation());

	if (motion == MOTION_NONE
	||  motion == MOTION_IDOL
	||  motion == MOTION_FLY_IDOL)
	{ // 設定できるモーションの場合

		// テレポート後のモーションを設定
		m_teleport.motion = motion;
	}
	else
	{ // 設定できないモーションの場合

		// モーション指定なしを設定
		assert(false);
		m_teleport.motion = MOTION_NONE;
	}

	// テレポート後のカメラ補正を設定
	m_teleport.bLook = bLook;

	// テレポートの初期化状態にする
	m_teleport.state = TELEPORT_INIT;

	// 魔法陣によるフェードインの行動をとらせる
	m_action = ACT_MAGIC_FADEIN;
}

//============================================================
//	地面殴りの行動設定処理
//============================================================
void CEnemyBossDragon::SetActPunchGround(void)
{
	// 地面殴りモーションを設定
	SetMotion(MOTION_PUNCH_GROUND, BLEND_FRAME);

	// 地面を殴る行動をとらせる
	m_action = ACT_PUNCH_GROUND;
}

//============================================================
//	炎攻撃の行動設定処理
//============================================================
void CEnemyBossDragon::SetActFireAttack(void)
{
	// 炎攻撃モーションを設定
	SetMotion(MOTION_FIRE_ATTACK, BLEND_FRAME);

	// 炎を吐く行動をとらせる
	m_action = ACT_FIRE_ATTACK;
}

//============================================================
//	ひっかき攻撃の行動設定処理
//============================================================
void CEnemyBossDragon::SetActClawAttack(void)
{
	// ひっかき攻撃モーションを設定
	SetMotion(MOTION_CLAW_ATTACK, BLEND_FRAME);

	// ひっかき攻撃の行動をとらせる
	m_action = ACT_CLAW_ATTACK;
}

//============================================================
//	しっぽ攻撃の行動設定処理
//============================================================
void CEnemyBossDragon::SetActTailAttack(void)
{
	// しっぽ攻撃モーションを設定
	SetMotion(MOTION_TAIL_ATTACK, BLEND_FRAME);

	// しっぽ攻撃の行動をとらせる
	m_action = ACT_TAIL_ATTACK;
}

//============================================================
//	空中攻撃の行動設定処理
//============================================================
void CEnemyBossDragon::SetActFlyAttack(void)
{
	// 空中攻撃モーションを設定
	SetMotion(MOTION_FLY_ATTACK, BLEND_FRAME);

	// 空中で攻撃する行動をとらせる
	m_action = ACT_FLY_ATTACK;
}

//============================================================
//	行動取得処理
//============================================================
CEnemyBossDragon::EAction CEnemyBossDragon::GetAction(void)
{
	// 現在の行動を返す
	return m_action;
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
//	モーションの更新処理
//============================================================
void CEnemyBossDragon::UpdateMotion(void)
{
	if (IsDeath()) { return; }	// 死亡している

	switch (GetMotionType())
	{ // モーションごとの処理
	case MOTION_HOWL:			// 咆哮モーション
	case MOTION_IDOL:			// 待機モーション
	case MOTION_FLY_IDOL:		// 空中待機モーション
		break;

	case MOTION_PUNCH_GROUND:	// 地面殴りモーション

		if (IsMotionFinish())
		{ // モーションが終了していた場合

			// 待機モーションに移行
			SetMotion(MOTION_IDOL, BLEND_FRAME);
		}

		break;

	case MOTION_CLAW_ATTACK:	// ひっかき攻撃モーション

		if (IsMotionFinish())
		{ // モーションが終了していた場合

			// 待機モーションに移行
			SetMotion(MOTION_IDOL, BLEND_FRAME);
		}

		break;

	case MOTION_FIRE_ATTACK:	// 炎攻撃モーション

		if (IsMotionFinish())
		{ // モーションが終了していた場合

			// 待機モーションに移行
			SetMotion(MOTION_IDOL, BLEND_FRAME);
		}

		break;

	case MOTION_TAIL_ATTACK:	// しっぽ攻撃モーション

		if (IsMotionFinish())
		{ // モーションが終了していた場合

			// 待機モーションに移行
			SetMotion(MOTION_IDOL, BLEND_FRAME);
		}

		break;

	case MOTION_FLY_ATTACK:		// 空中攻撃モーション

		if (IsMotionFinish())
		{ // モーションが終了していた場合

			// 空中待機モーションに移行
			SetMotion(MOTION_FLY_IDOL, BLEND_FRAME);
		}

		break;

	case MOTION_FLY_RUSH:		// 空中突進攻撃モーション

		// TODO：使うようになったらなんに遷移するか指定
		assert(false);

		break;

	default:	// 例外処理
		assert(false);
		break;
	}
}

//============================================================
//	スポーン状態の設定処理
//============================================================
void CEnemyBossDragon::SetSpawn(void)
{
	// 変数を宣言
	D3DXVECTOR3 posEnemy = GetVec3Position();	// 敵位置
	D3DXVECTOR3 rotEnemy = GetVec3Rotation();	// 敵向き

	// スポーン状態の設定
	CEnemy::SetSpawn();

	// 透明度を不透明に再設定
	SetAlpha(1.0f);

	// 自動描画をONにする
	SetEnableDraw(true);

	// 咆哮モーションを設定
	SetMotion(MOTION_HOWL);

	// 着地判定
	UpdateLanding(&posEnemy);

	// 位置範囲外の補正
	LimitPosition(&posEnemy);

	// 位置を反映
	SetVec3Position(posEnemy);

	// 向きを反映
	SetVec3Rotation(rotEnemy);
	SetDestRotation(rotEnemy);

	// カメラをボス注目状態に設定
	GET_MANAGER->GetCamera()->SetState(CCamera::STATE_LOOK_BOSS);
	GET_MANAGER->GetCamera()->SetDestLookBoss();	// カメラ目標位置の初期化
}

//============================================================
//	スポーン状態時の更新処理
//============================================================
void CEnemyBossDragon::UpdateSpawn(void)
{
	// 変数を宣言
	D3DXVECTOR3 posEnemy = GetVec3Position();	// 敵位置
	assert(GetMotionType() == MOTION_HOWL);		// モーションが咆哮中じゃない

	if (GetMotionKey() == LAND_MOTION_KEY && GetMotionKeyCounter() == 0)
	{ // モーションが着地したタイミングの場合

		// 着地のカメラ揺れ設定
		GET_MANAGER->GetCamera()->SetSwing(CCamera::TYPE_MAIN, LAND_SWING);
	}
	else if (GetMotionKey() == HOWL_MOTION_KEY && GetMotionKeyCounter() == 0)
	{ // モーションが口を開け始めたタイミングの場合

		// 咆哮のカメラ揺れ設定
		GET_MANAGER->GetCamera()->SetSwing(CCamera::TYPE_MAIN, HOWL_SWING);

		// ドラゴン咆哮の再生
		PLAY_SOUND(CSound::LABEL_SE_DRAGON_ROAR);
	}
	else if (IsMotionFinish())
	{ // モーションが終了していた場合

		// カウンターを加算
		m_nCounterAttack++;
		if (m_nCounterAttack > HOWL_WAIT_FRAME)
		{ // 余韻が終了した場合

			// カウンターを初期化
			m_nCounterAttack = 0;

			// なにもしない状態にする
			SetState(STATE_NONE);

			// 待機モーションにする
			SetMotion(MOTION_IDOL, BLEND_FRAME);
		}
	}

	// 着地判定
	UpdateLanding(&posEnemy);

	// 位置範囲外の補正
	LimitPosition(&posEnemy);

	// 位置を反映
	SetVec3Position(posEnemy);
}

//============================================================
//	通常状態時の更新処理
//============================================================
void CEnemyBossDragon::UpdateNormal(void)
{
	// 攻撃の更新
	UpdateAttack();

	// 行動の更新
	UpdateAction();
}

//============================================================
//	死亡状態時の更新処理
//============================================================
void CEnemyBossDragon::UpdateDeath(void)
{

}

//============================================================
//	位置範囲外の補正処理
//============================================================
void CEnemyBossDragon::LimitPosition(D3DXVECTOR3 *pPos)
{
	// ポインタを宣言
	CStage *pStage = CScene::GetStage();				// ステージ情報
	if (pStage == nullptr) { assert(false); return; }	// ステージ非使用中

	if (!IsFly())
	{ // 飛行していない場合

		// ステージ範囲外の補正
		pStage->LimitPosition(*pPos, GetStatusInfo().fRadius);
	}
}

//============================================================
//	攻撃の選択処理
//============================================================
void CEnemyBossDragon::SelectAttack(void)
{
	// 前回の攻撃を保存
	m_oldAtk = m_curAtk;

	// ランダム生成情報を生成
	CRandom<CEnemyAttack::EAttack> *pRandom = CRandom<CEnemyAttack::EAttack>::Create();
	if (pRandom == nullptr) { return; }		// 生成ミス

	// 生成可能な攻撃を設定
	if (m_pAttack == nullptr) { return; }	// 攻撃クラス無し
	m_pAttack->SetRandomArray(pRandom, m_oldAtk, m_nCounterSameAct);

	// 攻撃を取得
	m_curAtk = pRandom->GetRandomNum();

	// 連続攻撃回数を設定
	if (m_oldAtk == m_curAtk)
	{ // 前回の攻撃と同一の場合

		m_nCounterSameAct++;
	}
	else
	{ // 前回の攻撃と別の場合

		m_nCounterSameAct = 0;
	}

	// ランダム生成情報を破棄
	CRandom<CEnemyAttack::EAttack>::Release(pRandom);
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
#ifndef RANDOM_ATTACK_ON
				m_curAtk, this
#else
				ATTACK, this
#endif
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

			// 次攻撃の選択
			SelectAttack();

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
	D3DXVECTOR3 posEnemy = GetVec3Position();		// 敵位置
	D3DXVECTOR3 rotEnemy = GetVec3Rotation();		// 敵向き
	D3DXVECTOR3 rotDestEnemy = GetDestRotation();	// 敵目標向き

	// 過去位置の更新
	UpdateOldPosition();

	if (!IsFly())
	{ // 飛行していない場合

		// 重力の更新
		UpdateGravity();
	}

	switch (m_action)
	{ // 行動ごとの処理
	case ACT_NONE:	// 何もしない
		break;

	case ACT_MAGIC_FADEIN:	// 魔法陣フェードイン

		// 魔法陣フェードイン行動時の更新
		UpdateMagicFadeIn(posEnemy);

		break;

	case ACT_MAGIC_FADEOUT:	// 魔法陣フェードアウト

		// 魔法陣フェードアウト行動時の更新
		UpdateMagicFadeOut(&posEnemy, &rotEnemy, &rotDestEnemy);

		break;

	case ACT_PUNCH_GROUND:	// 地面殴り

		// 地面殴りの行動時の更新
		UpdatePunchGround();

		break;

	case ACT_FIRE_ATTACK:	// 炎攻撃

		// 炎攻撃の行動時の更新
		UpdateFireAttack();

		break;

	case ACT_CLAW_ATTACK:	// ひっかき攻撃

		// ひっかき攻撃の行動時の更新
		UpdateClawAttack();

		break;

	case ACT_TAIL_ATTACK:	// しっぽ攻撃

		// しっぽ攻撃の行動時の更新
		UpdateTailAttack();

		break;

	case ACT_FLY_ATTACK:	// 空中攻撃

		// 空中攻撃の行動時の更新
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

	// 向き更新
	UpdateRotation(&rotEnemy, &rotDestEnemy);

	// 位置範囲外の補正
	LimitPosition(&posEnemy);

	// 位置を反映
	SetVec3Position(posEnemy);

	// 向きを反映
	SetVec3Rotation(rotEnemy);

	// 目標向きを反映
	SetDestRotation(rotDestEnemy);
}

//============================================================
//	飛行フラグ取得処理
//============================================================
bool CEnemyBossDragon::IsFly(void) const
{
	// 変数を宣言
	EMotion curMotion = (EMotion)GetMotionType();	// 現在のモーション

	// TODO：空中モーション追加したら記述

	// 飛行状況を設定
	bool bFly = (curMotion == MOTION_FLY_IDOL
			  || curMotion == MOTION_FLY_ATTACK
			  || curMotion == MOTION_FLY_RUSH
			  || m_teleport.motion == MOTION_FLY_IDOL
			  || m_teleport.motion == MOTION_FLY_ATTACK
			  || m_teleport.motion == MOTION_FLY_RUSH);

	// 飛行状況を返す
	return bFly;
}

//============================================================
//	魔法陣フェードイン行動時の更新処理
//============================================================
void CEnemyBossDragon::UpdateMagicFadeIn(const D3DXVECTOR3& rPos)
{
	// 変数を宣言
	float fMagicPosY = m_pMagicCircle->GetVec3Position().y;	// 魔法陣の位置

	switch (m_teleport.state)
	{ // テレポート状態ごとの処理
	case TELEPORT_INIT:		// テレポートの初期化
	{
		// 魔法陣の自動描画をONにする
		m_pMagicCircle->SetEnableDraw(true);

		// 魔法陣の位置を足の下にする
		fMagicPosY = rPos.y;

		// 魔法陣の出現状態にする
		m_teleport.state = TELEPORT_APPEAR;

		// 処理を抜けずにこのまま魔法陣の出現に移行
	}
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

			// テレポートの初期化状態にする
			m_teleport.state = TELEPORT_INIT;

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
	m_pMagicCircle->SetVec3Position(D3DXVECTOR3(rPos.x, fMagicPosY, rPos.z));
}

//============================================================
//	魔法陣フェードアウト行動時の更新処理
//============================================================
void CEnemyBossDragon::UpdateMagicFadeOut(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pRot, D3DXVECTOR3 *pDestRot)
{
	// 変数を宣言
	float fMagicPosY = m_pMagicCircle->GetVec3Position().y;	// 魔法陣の位置

	switch (m_teleport.state)
	{ // テレポート状態ごとの処理
	case TELEPORT_INIT:		// テレポートの初期化
	{
		if (m_teleport.motion > MOTION_NONE
		&&  m_teleport.motion < MOTION_MAX)
		{ // モーション指定がある場合

			// モーションを設定
			SetMotion(m_teleport.motion, BLEND_FRAME);
		}

		// ボスの位置・向き・目標向きをテレポート先に変更
		*pPos = m_teleport.pos;
		*pRot = m_teleport.rot;
		*pDestRot = m_teleport.rot;

		// 位置範囲外の補正
		LimitPosition(pPos);

		if (m_teleport.bLook)
		{ // テレポート先を視認する場合

			// テレポート後のボスを視認させる
			GET_MANAGER->GetCamera()->SetFollowLook(*pPos);
		}

		// 魔法陣の位置を頭の上にする
		fMagicPosY  = GetMultiModel(MODEL_HEAD)->GetMtxWorld()._42 + MAGIC_DELPOS_PLUSY;
		fMagicPosY += pPos->y - GetOldPosition().y;	// 過去位置から今フレームでずれたY座標分を加算

		// 魔法陣の出現状態にする
		m_teleport.state = TELEPORT_APPEAR;

		// 処理を抜けずにこのまま魔法陣の出現に移行
	}
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
	m_pMagicCircle->SetVec3Position(D3DXVECTOR3(pPos->x, fMagicPosY, pPos->z));
}

//============================================================
//	地面殴り行動時の更新処理
//============================================================
void CEnemyBossDragon::UpdatePunchGround(void)
{

}

//============================================================
//	炎攻撃の行動時の更新処理
//============================================================
void CEnemyBossDragon::UpdateFireAttack(void)
{

}

//============================================================
//	ひっかき攻撃の行動時の更新処理
//============================================================
void CEnemyBossDragon::UpdateClawAttack(void)
{

}

//============================================================
//	しっぽ攻撃の行動時の更新処理
//============================================================
void CEnemyBossDragon::UpdateTailAttack(void)
{

}

//============================================================
//	空中攻撃の行動時の更新処理
//============================================================
void CEnemyBossDragon::UpdateFlyAttack(void)
{

}
