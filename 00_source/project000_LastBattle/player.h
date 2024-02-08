//============================================================
//
//	プレイヤーヘッダー [player.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _PLAYER_H_
#define _PLAYER_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "objectDivChara.h"
#include "scene.h"

//************************************************************
//	定数宣言
//************************************************************
namespace player
{
	const int NUM_SWORD = 2;	// 剣の数
}

//************************************************************
//	前方宣言
//************************************************************
class CObjectChara;	// オブジェクトキャラクタークラス
class CMultiModel;	// マルチモデルクラス
class CSword;		// 剣クラス
class CGauge2D;		// ゲージ2Dクラス
class CShadow;		// 影クラス

//************************************************************
//	クラス定義
//************************************************************
// プレイヤークラス
class CPlayer : public CObjectDivChara
{
public:
	// 下半身モデル列挙
	enum ELowerModel
	{
		L_MODEL_WAIST = 0,	// 腰
		L_MODEL_LEGUL,		// 左太もも
		L_MODEL_LEGUR,		// 右太もも
		L_MODEL_LEGDL,		// 左脛
		L_MODEL_LEGDR,		// 右脛
		L_MODEL_FOOTL,		// 左足
		L_MODEL_FOOTR,		// 右足
		L_MODEL_MAX			// この列挙型の総数
	};

	// 上半身モデル列挙
	enum EUpperModel
	{
		U_MODEL_BODY = 0,	// 体
		U_MODEL_HEAD,		// 頭
		U_MODEL_ARMUL,		// 左上腕
		U_MODEL_ARMUR,		// 右上腕
		U_MODEL_ARMDL,		// 左下腕
		U_MODEL_ARMDR,		// 右下腕
		U_MODEL_HANDL,		// 左手
		U_MODEL_HANDR,		// 右手
		U_MODEL_MAX			// この列挙型の総数
	};

	// 下半身モーション列挙
	enum ELowerMotion
	{
		L_MOTION_SPAWN = 0,			// 登場モーション
		L_MOTION_IDOL,				// 待機モーション
		L_MOTION_MOVE,				// 歩行モーション
		L_MOTION_ATTACK_00,			// 攻撃モーション一段階目
		L_MOTION_ATTACK_01,			// 攻撃モーション二段階目
		L_MOTION_ATTACK_02,			// 攻撃モーション三段階目
		L_MOTION_JUMP,				// ジャンプモーション
		L_MOTION_LAND,				// 着地モーション
		L_MOTION_JUMP_ATTACK_00,	// 空中攻撃モーション一段階目
		L_MOTION_JUMP_ATTACK_01,	// 空中攻撃モーション二段階目
		L_MOTION_DEATH,				// 死亡モーション
		L_MOTION_MAX				// この列挙型の総数
	};

	// 上半身モーション列挙
	enum EUpperMotion
	{
		U_MOTION_SPAWN = 0,			// 登場モーション
		U_MOTION_IDOL,				// 待機モーション
		U_MOTION_MOVE,				// 歩行モーション
		U_MOTION_ATTACK_00,			// 攻撃モーション一段階目
		U_MOTION_ATTACK_01,			// 攻撃モーション二段階目
		U_MOTION_ATTACK_02,			// 攻撃モーション三段階目
		U_MOTION_JUMP,				// ジャンプモーション
		U_MOTION_LAND,				// 着地モーション
		U_MOTION_JUMP_ATTACK_00,	// 空中攻撃モーション一段階目
		U_MOTION_JUMP_ATTACK_01,	// 空中攻撃モーション二段階目
		U_MOTION_DEATH,				// 死亡モーション
		U_MOTION_MAX				// この列挙型の総数
	};

	// 状態列挙
	enum EState
	{
		STATE_NONE = 0,	// 何もしない状態
		STATE_SPAWN,	// スポーン状態
		STATE_NORMAL,	// 通常状態
		STATE_DAMAGE,	// ダメージ状態
		STATE_INVULN,	// 無敵状態
		STATE_DEATH,	// 死亡状態
		STATE_MAX		// この列挙型の総数
	};

	// コンストラクタ
	CPlayer();

	// デストラクタ
	~CPlayer() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画
	void SetState(const int nState) override;	// 状態設定
	int  GetState(void) const override;			// 状態取得
	float GetRadius(void) const override;		// 半径取得
	float GetHeight(void) const override;		// 縦幅取得
	void SetEnableUpdate(const bool bUpdate) override;	// 更新状況設定
	void SetEnableDraw(const bool bDraw) override;		// 描画状況設定
	D3DXMATRIX CalcMtxWorld(void) const override;		// マトリックス計算結果取得

	// 静的メンバ関数
	static CPlayer *Create(CScene::EMode mode);		// 生成
	static CListManager<CPlayer> *GetList(void);	// リスト取得

	// メンバ関数
	void Hit(const int nDamage);	// ヒット
	void HitKnockBack(const int nDamage, const D3DXVECTOR3& vecKnock);	// ノックバックヒット
	void SetSpawn(void);	// スポーン設定
	void SetInvuln(void);	// 無敵設定

private:
	// 先行入力構造体
	struct SBuffering
	{
	public:
		// 先行入力の反映数の加算
		bool Add(void)
		{
			// 反映数を加算
			nNumSet++;

			if (nNumSet >= BODY_MAX)
			{ // 全モーションに反映した場合

				// 情報を初期化
				bInput  = false;	// 先行入力状況
				nNumSet = 0;		// 先行入力の反映数

				return true;
			}

			return false;
		}

		// メンバ変数
		bool bInput;	// 先行入力状況
		int  nNumSet;	// 先行入力の反映数
	};

	// 攻撃構造体
	struct SAttack : public SBuffering
	{
	public:
		// 種類列挙
		enum EType
		{
			TYPE_LAND = 0,	// 地上攻撃
			TYPE_SKY,		// 空中攻撃
			TYPE_MAX		// この列挙型の総数
		};

		// メンバ変数
		EType curType;	// 現在の攻撃種類
	};

	// 回避構造体
	struct SDodge
	{
		bool  bDodge;		// 回避状況
		float fMove;		// 回避移動量
		float fRot;			// 回避方向
		int   nWaitCounter;	// クールタイム管理カウンター
	};

	// モーション更新の関数ポインタ型エイリアス定義
	typedef void (CPlayer::*AFuncUpdateMotion)(const int);

	// オーバーライド関数
	void SetMotion(const EBody bodyID, const int nType) override;	// モーション設定

	// メンバ関数
	void LoadSetup(const EBody bodyID, const char **ppModelPass);	// セットアップ
	void UpdateMotion(const int nLowMotion, const int nUpMotion);	// モーション更新
	void UpdateMotionLower(const int nMotion);	// 下半身モーション更新
	void UpdateMotionUpper(const int nMotion);	// 上半身モーション更新
	bool IsAttack(void) const;		// 攻撃状況取得
	void SetLStickRotation(void);	// Lスティック向きを向かせる

	void UpdateSpawn(int *pLowMotion, int *pUpMotion);	// スポーン状態時の更新
	void UpdateNormal(int *pLowMotion, int *pUpMotion);	// 通常状態時の更新
	void UpdateDamage(int *pLowMotion, int *pUpMotion);	// ダメージ状態時の更新
	void UpdateInvuln(int *pLowMotion, int *pUpMotion);	// 無敵状態時の更新

	void UpdateAttack(void);		// 攻撃操作の更新
	void UpdateLandAttack(void);	// 地上攻撃操作の更新
	void UpdateSkyAttack(void);		// 空中攻撃操作の更新
	void UpdateDodge(void);			// 回避操作の更新
	void UpdateMove(int *pLowMotion, int *pUpMotion);	// 移動操作・目標向きの更新
	void UpdateJump(int *pLowMotion, int *pUpMotion);	// ジャンプ操作の更新

	void UpdateOldPosition(void);			// 過去位置の更新
	void UpdateGravity(void);				// 重力の更新
	void UpdateLanding(D3DXVECTOR3 *pPos);	// 着地状況の更新
	void UpdatePosition(D3DXVECTOR3 *pPos);	// 位置の更新
	void UpdateRotation(D3DXVECTOR3 *pRot);	// 向きの更新
	bool UpdateFadeOut(const float fAdd);	// フェードアウト状態時の更新
	bool UpdateFadeIn(const float fSub);	// フェードイン状態時の更新

	// 静的メンバ変数
	static CListManager<CPlayer> *m_pList;			// オブジェクトリスト
	static AFuncUpdateMotion m_aFuncUpdateMotion[];	// モーション更新関数

	// メンバ変数
	CListManager<CPlayer>::AIterator m_iterator;	// イテレーター
	CSword		*m_apSowrd[player::NUM_SWORD];		// 剣の情報
	CGauge2D	*m_pLife;			// 体力の情報
	CShadow		*m_pShadow;			// 影の情報
	D3DXVECTOR3	m_oldPos;			// 過去位置
	D3DXVECTOR3	m_move;				// 移動量
	D3DXVECTOR3	m_destRot;			// 目標向き
	EState		m_state;			// 状態
	int			m_nCounterState;	// 状態管理カウンター
	float		m_fSinAlpha;		// 透明向き
	bool		m_bJump;			// ジャンプ状況
	SDodge		m_dodge;			// 回避の情報
	SAttack		m_attack;			// 攻撃の情報
};

#endif	// _PLAYER_H_
