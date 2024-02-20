//============================================================
//
//	�G�U���w�b�_�[ [enemyAttack.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _ENEMY_ATTACK_H_
#define _ENEMY_ATTACK_H_

//************************************************************
//	�O���錾
//************************************************************
class CEnemyBossDragon;	// �{�X�h���S���N���X

//************************************************************
//	�N���X��`
//************************************************************
// �G�U���N���X
class CEnemyAttack
{
public:
	// �U����
	enum EAttack
	{
		ATTACK_00 = 0,	// �U��00(�n�ʉ���g��)
		ATTACK_01,		// �U��01(�������_������)
		ATTACK_02,		// �U��02(���O������������)
		ATTACK_03,		// �U��03(���v���C���[�ʒu����)
		ATTACK_04,		// �U��04(���O���f���o��)
		ATTACK_05,		// �U��05(�Ђ������U��)
		ATTACK_06,		// �U��06(�����ۍU��)
		ATTACK_MAX		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	explicit CEnemyAttack(CEnemyBossDragon *pBoss);

	// �f�X�g���N�^
	virtual ~CEnemyAttack();

	// �������z�֐�
	virtual bool Update(void) = 0;	// �X�V

	// ���z�֐�
	virtual HRESULT Init(void);	// ������
	virtual void Uninit(void);	// �I��

	// �ÓI�����o�֐�
	static CEnemyAttack *Create	// ����
	( // ����
		const EAttack attackID,	// �U���C���f�b�N�X
		CEnemyBossDragon *pBoss	// �{�X�ւ̃|�C���^
	);
	static void Release(CEnemyAttack *&prEnemyAttack);	// �j��

	// �����o�֐�
	CEnemyBossDragon *GetBoss(void);	// �{�X�擾

private:
	// �����o�ϐ�
	CEnemyBossDragon *m_pBoss;	// �{�X�ւ̃|�C���^
};

#endif	// _ENEMY_ATTACK_H_
