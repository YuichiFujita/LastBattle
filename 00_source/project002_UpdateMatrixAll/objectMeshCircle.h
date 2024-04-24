//============================================================
//
//	オブジェクトメッシュサークルヘッダー [objectMeshCircle.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _OBJECT_MESHCIRCLE_H_
#define _OBJECT_MESHCIRCLE_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object.h"
#include "renderState.h"

//************************************************************
//	クラス定義
//************************************************************
// オブジェクトメッシュサークルクラス
class CObjectMeshCircle : public CObject
{
public:
	// コンストラクタ
	explicit CObjectMeshCircle(const CObject::ELabel label = LABEL_NONE, const CObject::EDim dimension = DIM_3D, const int nPriority = object::DEFAULT_PRIO);

	// デストラクタ
	~CObjectMeshCircle() override;

	// メッシュサークル構造体
	struct SMeshCircle
	{
		D3DXVECTOR3	pos;		// 位置
		D3DXVECTOR3	rot;		// 向き
		D3DXCOLOR	col;		// 色
		D3DXMATRIX	mtxWorld;	// ワールドマトリックス
		float		fRadius;	// 半径
	};

	// オーバーライド関数
	HRESULT Init(void) override;		// 初期化
	void Uninit(void) override;			// 終了
	void Update(void) override;			// 更新
	void UpdateMatrix(void) override;	// マトリックス更新
	void Draw(void) override;			// 描画
	void BindTexture(const int nTextureID) override;		// テクスチャ割当 (インデックス)
	void BindTexture(const char *pTexturePass) override;	// テクスチャ割当 (パス)
	void SetVec3Position(const D3DXVECTOR3& rPos) override;	// 位置設定
	D3DXVECTOR3 GetVec3Position(void) const override;		// 位置取得
	void SetVec3Rotation(const D3DXVECTOR3& rRot) override;	// 向き設定
	D3DXVECTOR3 GetVec3Rotation(void) const override;		// 向き取得
	void SetColor(const D3DXCOLOR& rCol) override;			// 色設定
	D3DXCOLOR GetColor(void) const override;				// 色取得
	void SetRadius(const float fRadius) override;			// 半径設定
	float GetRadius(void) const override;					// 半径取得

	// 静的メンバ関数
	static CObjectMeshCircle *Create	// 生成
	( // 引数
		const D3DXVECTOR3& rPos,	// 位置
		const D3DXVECTOR3& rRot,	// 向き
		const D3DXCOLOR& rCol,		// 色
		const POSGRID2& rPart,		// 分割数
		const float fRadius			// 半径
	);

	// メンバ関数
	HRESULT SetPattern(const POSGRID2& rPart);	// 分割数設定
	POSGRID2 GetPattern(void) const;			// 分割数取得
	CRenderState *GetRenderState(void);			// レンダーステート情報取得

protected:
	// メンバ関数
	void SetVtx(void);	// 頂点情報の設定
	void SetIdx(void);	// インデックス情報の設定

private:
	// オーバーライド関数
	void Release(void) override;	// 破棄

	// メンバ変数
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;	// インデックスバッファへのポインタ
	CRenderState *m_pRenderState;		// レンダーステートの情報

	SMeshCircle m_meshCircle;	// メッシュサークルの情報
	POSGRID2 m_part;	// 分割数
	int m_nNumVtx;		// 必要頂点数
	int m_nNumIdx;		// 必要インデックス数
	int m_nTextureID;	// テクスチャインデックス
};

#endif	// _OBJECT_MESHCIRCLE_H_
