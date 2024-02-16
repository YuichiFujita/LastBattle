//============================================================
//
//	敵攻撃処理 [enemyAttack.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "enemyAttack.h"
#include "enemyAttack00.h"
#include "enemyAttack01.h"
#include "enemyAttack02.h"
#include "enemyAttack03.h"
#include "enemyAttack04.h"
#include "enemyBossDragon.h"
#include "manager.h"

//************************************************************
//	親クラス [CEnemyAttack] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEnemyAttack::CEnemyAttack() : m_pBoss(nullptr)
{

}

//============================================================
//	デストラクタ
//============================================================
CEnemyAttack::~CEnemyAttack()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CEnemyAttack::Init(void)
{
	// メンバ変数を初期化
	m_pBoss = nullptr;	// ボスへのポインタ

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CEnemyAttack::Uninit(void)
{

}

//============================================================
//	生成処理
//============================================================
CEnemyAttack *CEnemyAttack::Create(const EAttack attackID, CEnemyBossDragon *pBoss)
{
	// ポインタを宣言
	CEnemyAttack *pEnemyAttack = nullptr;	// 敵攻撃情報

	// 敵攻撃の生成
	switch (attackID)
	{ // 種類ごとの処理
	case ATTACK_00:
		pEnemyAttack = new CEnemyAttack00;
		break;

	case ATTACK_01:
		pEnemyAttack = new CEnemyAttack01;
		break;

	case ATTACK_02:
		pEnemyAttack = new CEnemyAttack02;
		break;

	case ATTACK_03:
		pEnemyAttack = new CEnemyAttack03;
		break;

	case ATTACK_04:
		pEnemyAttack = new CEnemyAttack04;
		break;

	default:	// 例外処理
		assert(false);
		break;
	}

	if (pEnemyAttack == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// 敵攻撃の初期化
		if (FAILED(pEnemyAttack->Init()))
		{ // 初期化に失敗した場合

			// 敵攻撃の破棄
			SAFE_DELETE(pEnemyAttack);
			return nullptr;
		}

		// ボスへのポインタを設定
		pEnemyAttack->m_pBoss = pBoss;

		// 確保したアドレスを返す
		return pEnemyAttack;
	}
}

//============================================================
//	破棄処理
//============================================================
void CEnemyAttack::Release(CEnemyAttack *&prEnemyAttack)
{
	// 敵攻撃の終了
	assert(prEnemyAttack != nullptr);
	prEnemyAttack->Uninit();

	// メモリ開放
	SAFE_DELETE(prEnemyAttack);
}

//============================================================
//	ボス取得処理
//============================================================
CEnemyBossDragon *CEnemyAttack::GetBoss(void)
{
	// ボスの情報を返す
	return m_pBoss;
}
