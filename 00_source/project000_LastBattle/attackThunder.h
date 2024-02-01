//============================================================
//
//	雷攻撃ヘッダー [attackThunder.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _ATTACK_THUNDER_H_
#define _ATTACK_THUNDER_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object.h"

//************************************************************
//	定数宣言
//************************************************************
namespace attackThunder
{
	const int NUM_THUNDER = 5;	// 雷の生成数
}

//************************************************************
//	前方宣言
//************************************************************
class CThunder;		// 雷クラス
class CObject3D;	// オブジェクト3Dクラス

//************************************************************
//	クラス定義
//************************************************************
// 雷攻撃クラス
class CAttackThunder : public CObject
{
public:
	// 状態列挙
	enum EState
	{
		STATE_WARN = 0,	// 警告表示
		STATE_ATTACK,	// 攻撃開始
		STATE_MAX		// この列挙の総数
	};

	// コンストラクタ
	CAttackThunder();

	// デストラクタ
	~CAttackThunder() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画

	// 静的メンバ関数
	static CAttackThunder *Create(const D3DXVECTOR3& rPos);	// 生成

private:
	// オーバーライド関数
	void Release(void) override;	// 破棄
	void SetVec3Position(const D3DXVECTOR3 &rPos) override;	// 位置設定

	// メンバ関数
	HRESULT SetAttack(void);	// 攻撃状態の設定
	void CollisionPlayer(void);	// プレイヤーとの当たり判定

	// メンバ変数
	CThunder *m_apThunder[attackThunder::NUM_THUNDER];	// 雷の情報
	CObject3D *m_pWarning;		// 警告表示の情報
	D3DXVECTOR3 m_posOrigin;	// 雷の原点位置
	EState m_state;				// 状態
	int m_nCounterState;		// 状態管理カウンター
};

#endif	// _ATTACK_THUNDER_H_
