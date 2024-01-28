//============================================================
//
//	雷ヘッダー [thunder.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _THUNDER_H_
#define _THUNDER_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object.h"

//************************************************************
//	定数宣言
//************************************************************
namespace thunder
{
	const int NUM_ORBIT = 2;	// 軌跡の生成数
}

//************************************************************
//	前方宣言
//************************************************************
class COrbit;	// 軌跡クラス

//************************************************************
//	クラス定義
//************************************************************
// 雷クラス
class CThunder : public CObject
{
public:
	// コンストラクタ
	CThunder();

	// デストラクタ
	~CThunder() override;

	// 雷軌跡の構造体
	struct SOrbitInfo
	{
		COrbit *pOrbit;			// 軌跡の情報
		D3DXMATRIX mtxWorld;	// ワールドマトリックス
	};

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画

	// 静的メンバ関数
	static CThunder *Create(const D3DXVECTOR3& rPos);	// 生成

private:
	// オーバーライド関数
	void Release(void) override;	// 破棄
	void SetVec3Position(const D3DXVECTOR3 &rPos) override;	// 位置設定

	// メンバ変数
	SOrbitInfo  m_aOrbit[thunder::NUM_ORBIT];	// 軌跡の情報
	D3DXVECTOR3 m_posOrbit;	// 軌跡の生成位置
};

#endif	// _THUNDER_H_
