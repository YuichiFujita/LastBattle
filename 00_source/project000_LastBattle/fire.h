//============================================================
//
//	炎ヘッダー [fire.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _FIRE_H_
#define _FIRE_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object.h"

//************************************************************
//	前方宣言
//************************************************************
class CObject3D;	// オブジェクト3Dクラス

//************************************************************
//	クラス定義
//************************************************************
// 炎クラス
class CFire : public CObject
{
public:
	// コンストラクタ
	CFire();

	// デストラクタ
	~CFire() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画
	void SetVec3Position(const D3DXVECTOR3 &rPos) override;	// 位置設定
	D3DXVECTOR3 GetVec3Position(void) const override;		// 位置取得

	// 静的メンバ関数
	static CFire *Create	// 生成
	( // 引数
		const D3DXVECTOR3& rPos	// 位置
	);
	static CListManager<CFire> *GetList(void);	// リスト取得

private:
	// オーバーライド関数
	void Release(void) override;	// 破棄

	// メンバ関数
	void CollisionPlayer(void);	// プレイヤーとの当たり判定

	// 静的メンバ変数
	static CListManager<CFire> *m_pList;	// オブジェクトリスト

	// メンバ変数
	CListManager<CFire>::AIterator m_iterator;	// イテレーター
	D3DXVECTOR3	m_pos;	// 位置
	D3DXVECTOR3	m_move;	// 移動量
};

#endif	// _FIRE_H_
