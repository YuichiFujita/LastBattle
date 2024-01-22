//============================================================
//
//	敵ヘッダー [enemy.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _ENEMY_H_
#define _ENEMY_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "objectModel.h"

//************************************************************
//	前方宣言
//************************************************************
class CGauge3D;	// ゲージ3Dクラス

//************************************************************
//	クラス定義
//************************************************************
// 敵クラス
class CEnemy : public CObjectModel
{
public:
	// 種類列挙
	enum EType
	{
		TYPE_BOSS_DRAGON = 0,	// ボスドラゴン
		TYPE_MINI_DRAGON,		// ミニドラゴン
		TYPE_MAX				// この列挙型の総数
	};

	// 状態列挙
	enum EState
	{
		STATE_SPAWN = 0,	// スポーン状態
		STATE_NORMAL,		// 通常状態
		STATE_DAMAGE,		// ダメージ状態
		STATE_INVULN,		// 無敵状態
		STATE_DEATH,		// 死亡状態
		STATE_MAX			// この列挙型の総数
	};

	// コンストラクタ
	explicit CEnemy(const EType type);

	// デストラクタ
	virtual ~CEnemy();

	// ステータス構造体
	struct SStatusInfo
	{
		int   nMaxLife;		// 最大体力
		float fRadius;		// 半径
		float fHeight;		// 縦幅
		float fLookRev;		// 視認の補正係数
		float fCollRadius;	// 判定半径
	};

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画
	void SetState(const int nState) override;	// 状態設定
	int GetState(void) const override;			// 状態取得
	int GetType(void) const override;			// 種類取得
	float GetRadius(void) const override;		// 半径取得
	float GetHeight(void) const override;		// 縦幅取得

	// 仮想関数
	virtual void Hit(const int nDamage);	// ヒット
	virtual void HitKnockBack(const int nDamage, const D3DXVECTOR3 &vecKnock);	// ノックバックヒット

	// 静的メンバ関数
	static CEnemy *Create	// 生成
	( // 引数
		const EType type,			// 種類
		const D3DXVECTOR3& rPos,	// 位置
		const D3DXVECTOR3& rRot		// 向き
	);
	static CListManager<CEnemy> *GetList(void);			// リスト取得
	static SStatusInfo GetStatusInfo(const int nType);	// ステータス情報取得

	// メンバ関数
	void UpdateOldPosition(void);					// 過去位置更新
	D3DXVECTOR3 GetOldPosition(void) const;			// 過去位置取得
	void SetMovePosition(const D3DXVECTOR3& rMove);	// 位置移動量設定
	D3DXVECTOR3 GetMovePosition(void) const;		// 位置移動量取得
	SStatusInfo GetStatusInfo(void) const;			// ステータス情報取得
	int GetLife(void) const;						// 体力取得

protected:
	// 仮想関数
	virtual void SetSpawn(void);		// スポーン状態の設定
	virtual void SetInvuln(void);		// 無敵状態の設定
	virtual void UpdateSpawn(void);		// スポーン状態時の更新
	virtual void UpdateNormal(void);	// 通常状態時の更新
	virtual void UpdateDamage(void);	// ダメージ状態時の更新
	virtual void UpdateInvuln(void);	// 無敵状態時の更新
	virtual void UpdateDeath(void);		// 死亡状態時の更新

	// メンバ関数
	void UpdateLook	// 対象視認
	( // 引数
		const D3DXVECTOR3 &rPosLook,	// 視認対象位置
		const D3DXVECTOR3 &rPosEnemy,	// 敵位置
		D3DXVECTOR3 *pRotEnemy			// 敵向き
	);
	void UpdateGravity(void);				// 重力の更新
	bool UpdateLanding(D3DXVECTOR3 *pPos);	// 着地状況の更新
	void UpdatePosition(D3DXVECTOR3 *pPos);	// 位置の更新
	bool UpdateFadeOut(const float fAdd);	// フェードアウト状態時の更新
	bool UpdateFadeIn(const float fSub);	// フェードイン状態時の更新

private:
	// 状態更新の関数ポインタ型エイリアス定義
	typedef void (CEnemy::*AFuncUpdateState)(void);

	// 静的メンバ変数
	static CListManager<CEnemy> *m_pList;			// オブジェクトリスト
	static SStatusInfo m_aStatusInfo[TYPE_MAX];		// ステータス情報
	static AFuncUpdateState m_aFuncUpdateState[];	// 状態更新関数

	// メンバ変数
	CListManager<CEnemy>::AIterator m_iterator;	// イテレーター
	CGauge3D	*m_pLife;		// 体力の情報
	D3DXVECTOR3	m_oldPos;		// 過去位置
	D3DXVECTOR3	m_move;			// 移動量
	EState	m_state;			// 状態
	int		m_nCounterState;	// 状態管理カウンター
	float	m_fSinAlpha;		// 透明向き
	bool	m_bJump;			// ジャンプ状況
	const EType m_type;			// 種類定数
	const SStatusInfo m_status;	// ステータス定数
};

#endif	// _ENEMY_H_
