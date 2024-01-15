//============================================================
//
//	オブジェクト分割キャラクターヘッダー [objectDivChara.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _OBJECT_DIVCHARA_H_
#define _OBJECT_DIVCHARA_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object.h"

//************************************************************
//	前方宣言
//************************************************************
class CObjectChara;	// オブジェクトキャラクタークラス
class CMultiModel;	// マルチモデルクラス

//************************************************************
//	クラス定義
//************************************************************
// オブジェクト分割キャラクタークラス
class CObjectDivChara : public CObject
{
public:
	// 身体列挙
	enum EBody
	{
		BODY_LOWER = 0,	// 下半身
		BODY_UPPER,		// 上半身
		BODY_MAX		// この列挙型の総数
	};

	// コンストラクタ
	explicit CObjectDivChara(const CObject::ELabel label = LABEL_NONE, const CObject::EDim dimension = DIM_3D, const int nPriority = object::DEFAULT_PRIO);

	// デストラクタ
	~CObjectDivChara() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画
	void SetVec3Position(const D3DXVECTOR3 &rPos) override;	// 位置設定
	D3DXVECTOR3 GetVec3Position(void) const override;		// 位置取得
	void SetVec3Rotation(const D3DXVECTOR3 &rRot) override;	// 向き設定
	D3DXVECTOR3 GetVec3Rotation(void) const override;		// 向き取得
	void SetAllMaterial(const D3DXMATERIAL &rMat) override;	// マテリアル全設定
	void SetEnableUpdate(const bool bUpdate) override;		// 更新状況設定
	void SetEnableDraw(const bool bDraw) override;			// 描画状況設定

	// 静的メンバ関数
	static CObjectDivChara *Create	// 生成
	( // 引数
		const D3DXVECTOR3 &rPos,				// 位置
		const D3DXVECTOR3 &rRot = VEC3_ZERO,	// 向き
		const int nUpperParentID = 0			// 上半身親インデックス
	);

	// メンバ関数
	void SetUpperParentID(const int nUpperParentID);		// 上半身の親インデックス設定
	CObjectChara *GetObjectChara(const EBody bodyID) const;	// オブジェクトキャラクター取得
	CMultiModel *GetMultiModel(const EBody bodyID, const int nModelID) const;	// マルチモデル取得

private:
	// オーバーライド関数
	void Release(void) override;	// 破棄

	// メンバ変数
	CObjectChara *m_apBody[BODY_MAX];	// 身体の情報
	int m_nUpperParentID;	// 上半身親インデックス
};

#endif	// _OBJECT_DIVCHARA_H_
