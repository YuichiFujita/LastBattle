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
class CWave;			// �g���N���X

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
		ATTACK_00 = 0,	// �U��00
		//ATTACK_01,		// �U��01
		ATTACK_MAX		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CEnemyAttack();

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
	CEnemyBossDragon *GetBossDragon(void);	// �{�X�擾

private:
	// �����o�ϐ�
	CEnemyBossDragon *m_pBoss;	// �{�X�ւ̃|�C���^
};

// �G�U��00�N���X
class CEnemyAttack00 : public CEnemyAttack
{
public:
	// ��ԗ�
	enum EState
	{
		STATE_SET = 0,	// �����ݒ�
		STATE_WAVE,		// �g������
		STATE_WAIT,		// �ҋ@
		STATE_END,		// �I��
		STATE_MAX		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CEnemyAttack00();

	// �f�X�g���N�^
	~CEnemyAttack00() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	bool Update(void) override;		// �X�V

private:
	// �����o�ϐ�
	CWave *m_pWave;			// �g���̏��
	EState m_state;			// ���
	int m_nCounterState;	// ��ԊǗ��J�E���^�[
	int m_nCreateWave;		// �g���̐�����
};

#endif	// _ENEMY_ATTACK_H_
