//============================================================
//
//	エディット当たり判定処理 [editColl.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "editColl.h"
#include "manager.h"
#include "scene.h"
#include "player.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{

}

//************************************************************
//	マクロ定義
//************************************************************
#define KEY_UP_PARTS	(DIK_UP)	// パーツ変更キー
#define NAME_UP_PARTS	("↑")		// パーツ変更表示
#define KEY_DOWN_PARTS	(DIK_DOWN)	// パーツ変更キー
#define NAME_DOWN_PARTS	("↓")		// パーツ変更表示

//************************************************************
//	子クラス [CEditColl] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEditColl::CEditColl() :
	m_nSelectParts	(0)	// 選択パーツ
{

}

//============================================================
//	デストラクタ
//============================================================
CEditColl::~CEditColl()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CEditColl::Init(void)
{
	CPlayer *pPlayer = CScene::GetPlayer();	// プレイヤー情報

	// メンバ変数を初期化
	m_nSelectParts = 0;	// 選択パーツ

	// エディットの初期化
	if (FAILED(CEdit::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// キーを初期化 (Tポーズにする)
	pPlayer->GetMotion()->Reset();

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CEditColl::Uninit(void)
{
	CPlayer *pPlayer = CScene::GetPlayer();	// プレイヤー情報

	// マテリアルを再設定
	pPlayer->ResetMaterial();

	// エディットの終了
	CEdit::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CEditColl::Update(void)
{
	// パーツ選択の更新
	SelectParts();

	// エディットの更新
	CEdit::Update();
}

//============================================================
//	描画処理
//============================================================
void CEditColl::Draw(void)
{
	// エディットの描画
	CEdit::Draw();
}

//============================================================
//	エディット操作表示
//============================================================
void CEditColl::DrawEditControl(void)
{
	// ポインタを宣言
	CDebugProc *pDebug = CManager::GetInstance()->GetDebugProc();	// デバッグの情報

	// エディット操作表示
	CEdit::DrawEditControl();

	pDebug->Print(CDebugProc::POINT_RIGHT, "パーツ変更：[%s/%s]\n", NAME_UP_PARTS, NAME_DOWN_PARTS);
}

//============================================================
//	エディット情報表示
//============================================================
void CEditColl::DrawEditData(void)
{
	// ポインタを宣言
	CDebugProc *pDebug = CManager::GetInstance()->GetDebugProc();	// デバッグの情報

	// エディット情報表示
	CEdit::DrawEditData();

	pDebug->Print(CDebugProc::POINT_RIGHT, "%d：[パーツ]\n", m_nSelectParts);
}

//============================================================
//	パーツ選択の更新処理
//============================================================
void CEditColl::SelectParts(void)
{
	CPlayer *pPlayer = CScene::GetPlayer();	// プレイヤー情報
	CInputKeyboard *pKey = GET_INPUTKEY;	// キーボード情報

	// 選択パーツ変更
	if (pKey->IsTrigger(KEY_UP_PARTS))
	{
		int nNumParts = pPlayer->GetNumModel();	// パーツの総数
		m_nSelectParts = (m_nSelectParts + (nNumParts - 1)) % nNumParts;
	}
	else if (pKey->IsTrigger(KEY_DOWN_PARTS))
	{
		int nNumParts = pPlayer->GetNumModel();	// パーツの総数
		m_nSelectParts = (m_nSelectParts + 1) % nNumParts;
	}

	// マテリアルを再設定
	pPlayer->ResetMaterial();

	// 選択パーツのマテリアルを変更
	pPlayer->SetPartsMaterial(material::Blue(), m_nSelectParts);
}
