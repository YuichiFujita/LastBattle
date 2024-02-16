//============================================================
//
//	GUwb_[ [enemyAttack.h]
//	AuthorF¡cEê
//
//============================================================
//************************************************************
//	ñdCN[hh~
//************************************************************
#ifndef _ENEMY_ATTACK_H_
#define _ENEMY_ATTACK_H_

//************************************************************
//	Oûé¾
//************************************************************
class CEnemyBossDragon;	// {XhSNX

//************************************************************
//	NXè`
//************************************************************
// GUNX
class CEnemyAttack
{
public:
	// Uñ
	enum EAttack
	{
		ATTACK_00 = 0,	// U00(nÊ£èg®)
		ATTACK_01,		// U01(_¶¬)
		ATTACK_02,		// U02(Oüü©¢¶¬)
		ATTACK_03,		// U03(vC[Êu¶¬)
		ATTACK_04,		// U04(Oüf«oµ)
		ATTACK_MAX		// ±Ìñ^Ì
	};

	// RXgN^
	CEnemyAttack();

	// fXgN^
	virtual ~CEnemyAttack();

	// ¼zÖ
	virtual bool Update(void) = 0;	// XV

	// ¼zÖ
	virtual HRESULT Init(void);	// ú»
	virtual void Uninit(void);	// I¹

	// ÃIoÖ
	static CEnemyAttack *Create	// ¶¬
	( // ø
		const EAttack attackID,	// UCfbNX
		CEnemyBossDragon *pBoss	// {XÖÌ|C^
	);
	static void Release(CEnemyAttack *&prEnemyAttack);	// jü

	// oÖ
	CEnemyBossDragon *GetBoss(void);	// {Xæ¾

private:
	// oÏ
	CEnemyBossDragon *m_pBoss;	// {XÖÌ|C^
};

#endif	// _ENEMY_ATTACK_H_
