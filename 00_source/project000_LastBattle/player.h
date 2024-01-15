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
#include "object.h"
#include "scene.h"

//************************************************************
//	前方宣言
//************************************************************
class CObjectChara;	// オブジェクトキャラクタークラス
class CMultiModel;	// マルチモデルクラス
class CShadow;		// 影クラス
class COrbit;		// 軌跡クラス

//************************************************************
//	クラス定義
//************************************************************
// プレイヤークラス
class CPlayer : public CObject
{
public:
	// 身体列挙
	enum EBody
	{
		BODY_LOWER = 0,	// 下半身
		BODY_UPPER,		// 上半身
		BODY_MAX		// この列挙型の総数
	};

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
		U_MODEL_SWORDL,		// 左剣
		U_MODEL_SWORDR,		// 右剣
		U_MODEL_MAX			// この列挙型の総数
	};

	// 下半身モーション列挙
	enum ELowerMotion
	{
		L_MOTION_IDOL = 0,	// 待機モーション
		L_MOTION_MAX		// この列挙型の総数
	};

	// 上半身モーション列挙
	enum EUpperMotion
	{
		U_MOTION_IDOL = 0,	// 待機モーション
		U_MOTION_MAX		// この列挙型の総数
	};

	// 状態列挙
	enum EState
	{
		STATE_NONE = 0,	// 何もしない状態
		STATE_SPAWN,	// スポーン状態
		STATE_NORMAL,	// 通常状態
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
	void Hit(void) override;		// ヒット
	void SetVec3Position(const D3DXVECTOR3 &rPos) override;	// 位置設定
	D3DXVECTOR3 GetVec3Position(void) const override;		// 位置取得
	void SetVec3Rotation(const D3DXVECTOR3 &rRot) override;	// 向き設定
	D3DXVECTOR3 GetVec3Rotation(void) const override;		// 向き取得
	void SetAllMaterial(const D3DXMATERIAL &rMat) override;	// マテリアル全設定

	void SetState(const int nState) override;	// 状態設定
	int GetState(void) const override;			// 状態取得
	float GetRadius(void) const override;		// 半径取得
	float GetHeight(void) const override;		// 縦幅取得

	void SetEnableUpdate(const bool bUpdate) override;	// 更新状況設定
	void SetEnableDraw(const bool bDraw) override;		// 描画状況設定
	D3DXMATRIX CalcMtxWorld(void) const override;		// マトリックス計算結果取得

	// 静的メンバ関数
	static CPlayer *Create(CScene::EMode mode);		// 生成
	static CListManager<CPlayer> *GetList(void);	// リスト取得

	// メンバ関数
	CMultiModel *GetMultiModel(const EBody bodyID, const int nModelID) const;	// マルチモデル取得
	void SetSpawn(void);	// 出現設定

private:
	// オーバーライド関数
	void Release(void) override;	// 破棄

	// メンバ関数
	void UpdateSpawn(int *pLowMotion, int *pUpMotion);	// スポーン状態時の更新
	void UpdateNormal(int *pLowMotion, int *pUpMotion);	// 通常状態時の更新
	void UpdateMotion(const int nLowMotion, const int nUpMotion);	// モーション・オブジェクトキャラクターの更新

	void UpdateOldPosition(void);			// 過去位置の更新
	void UpdateMove(void);					// 移動量・目標向きの更新
	void UpdateJump(void);					// ジャンプの更新
	void UpdateGravity(void);				// 重力の更新
	bool UpdateLanding(D3DXVECTOR3 *pPos);	// 着地状況の更新
	void UpdatePosition(D3DXVECTOR3 *pPos);	// 位置の更新
	void UpdateRotation(D3DXVECTOR3 *pRot);	// 向きの更新
	bool UpdateFadeOut(const float fAdd);	// フェードアウト状態時の更新
	bool UpdateFadeIn(const float fSub);	// フェードイン状態時の更新

	void LoadSetup(const EBody bodyID, const char **ppModelPass);	// セットアップ

	// 静的メンバ変数
	static CListManager<CPlayer> *m_pList;	// オブジェクトリスト

	// メンバ変数
	CListManager<CPlayer>::AIterator m_iterator;	// イテレーター
	CObjectChara *m_apBody[BODY_MAX];	// 身体の情報
	CShadow	*m_pShadow;			// 影の情報
	COrbit	*m_pOrbit;			// 軌跡の情報
	D3DXVECTOR3	m_pos;			// 位置
	D3DXVECTOR3	m_oldPos;		// 過去位置
	D3DXVECTOR3	m_move;			// 移動量
	D3DXVECTOR3	m_rot;			// 向き
	D3DXVECTOR3	m_destRot;		// 目標向き
	EState	m_state;			// 状態
	int		m_nCounterState;	// 状態管理カウンター
	bool	m_bJump;			// ジャンプ状況
};

#endif	// _PLAYER_H_
