//============================================================
//
//	雑魚敵ヘッダー [enemySmall.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _ENEMY_SMALL_H_
#define _ENEMY_SMALL_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "enemy.h"

//************************************************************
//	前方宣言
//************************************************************
class CGauge3D;	// ゲージ3Dクラス

//************************************************************
//	クラス定義
//************************************************************
// 雑魚敵クラス
class CEnemySmall : public CEnemy
{
public:
	// コンストラクタ
	explicit CEnemySmall(const EType type);

	// デストラクタ
	~CEnemySmall() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画
	void Hit(const int nDamage) override;	// ヒット
	void HitKnockBack(const int nDamage, const D3DXVECTOR3 &vecKnock) override;	// ノックバックヒット

private:
	// メンバ変数
	CGauge3D *m_pLife;	// 体力の情報
};

#endif	// _ENEMY_SMALL_H_
