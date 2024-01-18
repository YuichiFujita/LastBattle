//============================================================
//
//	モーション処理 [motion.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "motion.h"
#include "multiModel.h"

//************************************************************
//	親クラス [CMotion] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CMotion::CMotion() :
	m_ppModel	(nullptr),	// モデル情報
	m_nNumModel	(0),		// モデルのパーツ数
	m_bUpdate	(true)		// 更新状況
{
	// メンバ変数をクリア
	memset(&m_info, 0, sizeof(m_info));	// モーション情報
}

//============================================================
//	デストラクタ
//============================================================
CMotion::~CMotion()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CMotion::Init(void)
{
	// メンバ変数をクリア
	memset(&m_info, 0, sizeof(m_info));	// モーション情報
	m_ppModel	= nullptr;	// モデル情報
	m_nNumModel	= 0;		// モデルのパーツ数
	m_bUpdate	= true;		// 更新状況

	// モーションを終了状態にする
	m_info.bFinish = true;

	for (int nCntMotion = 0; nCntMotion < motion::MAX_MOTION; nCntMotion++)
	{ // モーションの最大数分繰り返す

		// 攻撃判定情報を初期化
		m_info.aMotionInfo[nCntMotion].collLeft.nMin  = NONE_IDX;
		m_info.aMotionInfo[nCntMotion].collLeft.nMax  = NONE_IDX;
		m_info.aMotionInfo[nCntMotion].collRight.nMin = NONE_IDX;
		m_info.aMotionInfo[nCntMotion].collRight.nMax = NONE_IDX;

		// 武器表示をOFFにする
		m_info.aMotionInfo[nCntMotion].bWeaponDisp = false;
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CMotion::Uninit(void)
{

}

//============================================================
//	更新処理
//============================================================
void CMotion::Update(void)
{
	if (!m_bUpdate)										{ return; }	// 更新しない
	if (m_info.aMotionInfo[m_info.nType].nNumKey <= 0)	{ return; }	// キー数未設定

	// 変数を宣言
	D3DXVECTOR3 diffPos;		// 次ポーズまでの差分位置
	D3DXVECTOR3 diffRot;		// 次ポーズまでの差分向き
	D3DXVECTOR3 currentPos;		// 現在フレームの位置
	D3DXVECTOR3 currentRot;		// 現在フレームの向き
	int nType = m_info.nType;	// モーション種類
	int nPose = m_info.nPose;	// モーションポーズ番号
	int nNextPose;				// 次のモーションポーズ番号

	// 次のモーションポーズ番号を求める
	nNextPose = (nPose + 1) % m_info.aMotionInfo[nType].nNumKey;

	// パーツの位置の更新
	for (int nCntKey = 0; nCntKey < m_nNumModel; nCntKey++)
	{ // モデルのパーツ数分繰り返す

		// 位置・向きの差分を求める
		diffPos = m_info.aMotionInfo[nType].aKeyInfo[nNextPose].aKey[nCntKey].pos - m_info.aMotionInfo[nType].aKeyInfo[nPose].aKey[nCntKey].pos;
		diffRot = m_info.aMotionInfo[nType].aKeyInfo[nNextPose].aKey[nCntKey].rot - m_info.aMotionInfo[nType].aKeyInfo[nPose].aKey[nCntKey].rot;

		// 差分向きの正規化
		useful::Vec3NormalizeRot(diffRot);

		// 現在のパーツの位置・向きを更新
		float fRate = (float)m_info.nKeyCounter / (float)m_info.aMotionInfo[nType].aKeyInfo[nPose].nFrame;	// キーフレーム割合
		m_ppModel[nCntKey]->SetVec3Position(m_info.aMotionInfo[nType].aKeyInfo[nPose].aKey[nCntKey].pos + diffPos * fRate);
		m_ppModel[nCntKey]->SetVec3Rotation(m_info.aMotionInfo[nType].aKeyInfo[nPose].aKey[nCntKey].rot + diffRot * fRate);
	}

	// モーションの遷移の更新
	if (m_info.nKeyCounter < m_info.aMotionInfo[nType].aKeyInfo[nPose].nFrame)
	{ // 現在のポーズの再生が終了しない場合

		// カウンターを加算
		m_info.nKeyCounter++;
		m_info.nWholeCounter++;
	}
	else
	{ // 現在のポーズの再生が終了した場合

		// 次のポーズに移行
		if (m_info.aMotionInfo[nType].bLoop)
		{ // モーションがループする場合

			// キーカウンターを初期化
			m_info.nKeyCounter = 0;

			// ポーズカウントを加算
			m_info.nPose = (m_info.nPose + 1) % m_info.aMotionInfo[nType].nNumKey;	// 最大値で0に戻す

			if (m_info.nPose == 0)
			{ // ポーズが最初に戻った場合

				// 全体カウンターを初期化
				m_info.nWholeCounter = 0;
			}
		}
		else
		{ // モーションがループしない場合

			if (m_info.nPose < m_info.aMotionInfo[nType].nNumKey - 2)
			{ // 現在のポーズが最終のポーズではない場合

				// キーカウンターを初期化
				m_info.nKeyCounter = 0;

				// ポーズカウントを加算
				m_info.nPose++;
			}
			else
			{ // 現在のポーズが最終のポーズの場合

				// モーションを終了状態にする
				m_info.bFinish = true;
			}
		}
	}
}

//============================================================
//	設定処理
//============================================================
void CMotion::Set(const int nType)
{
	// モーション情報を初期化
	m_info.nPose		 = 0;		// モーションポーズ番号
	m_info.nKeyCounter	 = 0;		// モーションキーカウンター
	m_info.nWholeCounter = 0;		// モーション全体カウンター
	m_info.bFinish		 = false;	// モーション終了状況

	// 引数のモーションの種類を設定
	m_info.nType = nType;

	// モーションを再生状態にする
	m_info.bFinish = false;

	// パーツの位置の初期化
	for (int nCntKey = 0; nCntKey < m_nNumModel; nCntKey++)
	{ // モデルのパーツ数分繰り返す

		// 初期位置と初期向きを設定
		m_ppModel[nCntKey]->SetVec3Position(m_info.aMotionInfo[nType].aKeyInfo[m_info.nPose].aKey[nCntKey].pos);
		m_ppModel[nCntKey]->SetVec3Rotation(m_info.aMotionInfo[nType].aKeyInfo[m_info.nPose].aKey[nCntKey].rot);
	}
}

//============================================================
//	モーション情報の設定処理
//============================================================
void CMotion::SetInfo(const SMotionInfo info)
{
	// 引数のモーション情報を設定
	m_info.aMotionInfo[m_info.nNumMotion] = info;

	// モーションの情報数を加算
	m_info.nNumMotion++;

	// 例外処理
	assert(m_info.nNumMotion <= motion::MAX_MOTION);	// モーション数オーバー
}

//============================================================
//	更新状況の設定処理
//============================================================
void CMotion::SetEnableUpdate(const bool bUpdate)
{
	// 引数の更新状況を設定
	m_bUpdate = bUpdate;
}

//============================================================
//	モデル情報の設定処理
//============================================================
void CMotion::SetModel(CMultiModel **ppModel, const int nNum)
{
	// 引数のモデル情報を設定
	m_ppModel = ppModel;

	// 引数のモデルのパーツ数を設定
	m_nNumModel = nNum;

	// 例外処理
	assert(m_nNumModel <= motion::MAX_PARTS);	// パーツ数オーバー
}

//============================================================
//	種類取得処理
//============================================================
int CMotion::GetType(void) const
{
	// 現在のモーションの種類を返す
	return m_info.nType;
}

//============================================================
//	ポーズ番号取得処理
//============================================================
int CMotion::GetPose(void) const
{
	// 現在のポーズ番号を返す
	return m_info.nPose;
}

//============================================================
//	モーションキーカウンター取得処理
//============================================================
int CMotion::GetKeyCounter(void) const
{
	// 現在のモーションキーカウンターを返す
	return m_info.nKeyCounter;
}

//============================================================
//	モーション全体カウンター取得処理
//============================================================
int CMotion::GetWholeCounter(void) const
{
	// 現在のモーション全体カウンターを返す
	return m_info.nWholeCounter;
}

//============================================================
//	終了取得処理
//============================================================
bool CMotion::IsFinish(void) const
{
	// 現在のモーションの終了状況を返す
	return m_info.bFinish;
}

//============================================================
//	ループ取得処理
//============================================================
bool CMotion::IsLoop(const int nType) const
{
	// 引数モーションのループのON/OFF状況を返す
	return m_info.aMotionInfo[nType].bLoop;
}

//============================================================
//	武器表示の取得処理
//============================================================
bool CMotion::IsWeaponDisp(const int nType) const
{
	// 引数モーションの武器表示のON/OFF状況を返す
	return m_info.aMotionInfo[nType].bWeaponDisp;
}

//============================================================
//	左の攻撃判定フラグの取得処理
//============================================================
bool CMotion::IsLeftWeaponCollision(void)
{
	SMotionInfo *pMotionInfo = &m_info.aMotionInfo[m_info.nType];	// 現在のモーション情報
	if (pMotionInfo->collLeft.nMin == NONE_IDX) { return false; }	// 開始カウント未設定
	if (pMotionInfo->collLeft.nMax == NONE_IDX) { return false; }	// 終了カウント未設定

	if (m_info.nWholeCounter >= pMotionInfo->collLeft.nMin
	&&  m_info.nWholeCounter <= pMotionInfo->collLeft.nMax)
	{ // カウンターが開始と終了の範囲内の場合

		return true;
	}

	return false;
}

//============================================================
//	右の攻撃判定フラグの取得処理
//============================================================
bool CMotion::IsRightWeaponCollision(void)
{
	SMotionInfo *pMotionInfo = &m_info.aMotionInfo[m_info.nType];	// 現在のモーション情報
	if (pMotionInfo->collRight.nMin == NONE_IDX) { return false; }	// 開始カウント未設定
	if (pMotionInfo->collRight.nMax == NONE_IDX) { return false; }	// 終了カウント未設定

	if (m_info.nWholeCounter >= pMotionInfo->collRight.nMin
	&&  m_info.nWholeCounter <= pMotionInfo->collRight.nMax)
	{ // カウンターが開始と終了の範囲内の場合

		return true;
	}

	return false;
}

//============================================================
//	生成処理
//============================================================
CMotion *CMotion::Create(void)
{
	// モーションの生成
	CMotion *pMotion = new CMotion;
	if (pMotion == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// モーションの初期化
		if (FAILED(pMotion->Init()))
		{ // 初期化に失敗した場合

			// モーションの破棄
			SAFE_DELETE(pMotion);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pMotion;
	}
}

//============================================================
//	破棄処理
//============================================================
void CMotion::Release(CMotion *&prMotion)
{
	// モーションの終了
	assert(prMotion != nullptr);
	prMotion->Uninit();

	// メモリ開放
	SAFE_DELETE(prMotion);
}
