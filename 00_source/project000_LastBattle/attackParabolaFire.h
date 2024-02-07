//============================================================
//
//	カーブ攻撃炎ヘッダー [attackParabolaFire.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _ATTACK_PARABOLA_FIRE_H_
#define _ATTACK_PARABOLA_FIRE_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "fire.h"

//************************************************************
//	クラス定義
//************************************************************
// カーブ攻撃炎クラス
class CAttackParabolaFire : public CFire
{
public:
	// コンストラクタ
	CAttackParabolaFire();

	// デストラクタ
	~CAttackParabolaFire() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画

	// 静的メンバ関数
	static CAttackParabolaFire *Create(const D3DXVECTOR3& rPos);	// 生成

private:
	// メンバ関数
	void UpdateParabolaMove(void);	// 炎移動量の更新

	// TODO：後でuseful持ってく
	D3DXVECTOR2 CalcParabola(float g, float v0x, float maxX, float maxY, float t);
	D3DXVECTOR2 CalcMoveParabola(float g, float v0x, float maxX, float maxY, float t);

	int nCnt;
};

#endif	// _ATTACK_PARABOLA_FIRE_H_
