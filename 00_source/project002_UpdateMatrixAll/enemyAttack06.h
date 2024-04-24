//============================================================
//
//	�G�U��06�w�b�_�[ [enemyAttack06.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _ENEMY_ATTACK06_H_
#define _ENEMY_ATTACK06_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "enemyAttack.h"

//************************************************************
//	�N���X��`
//************************************************************
// �G�U��06�N���X
/*
	@brief �{�X�̍U���Ǘ��N���X �����ۂœガ�����U��
*/
class CEnemyAttack06 : public CEnemyAttack
{
public:
	// ��ԗ�
	enum EState
	{
		STATE_INIT_TELEPORT = 0,	// �e���|�[�g�̏�����
		STATE_TELEPORT,				// �e���|�[�g
		STATE_INIT_ATTACK,			// �U���̏�����
		STATE_ATTACK,				// �U��
		STATE_END,					// �I��
		STATE_MAX					// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	explicit CEnemyAttack06(CEnemyBossDragon *pBoss);

	// �f�X�g���N�^
	~CEnemyAttack06() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	bool Update(void) override;		// �X�V
	void SetRandomArray				// �U���I�����̗v�f�ݒ�
	( // ����
		CRandom<EAttack> *pRandom,	// �����_���U���N���X
		EAttack oldAtk,				// �O��̍U��
		int nSameAct				// �����s���̘A����
	) override;

private:
	// �����o�֐�
	void SetEnableTeleport(void);	// �e���|�[�g���邩�̐ݒ�
	void InitTeleport(void);		// �e���|�[�g�̏�����
	void UpdateAttack(void);		// �U���̍X�V

	// �����o�ϐ�
	EState m_state;	// ���
};

#endif	// _ENEMY_ATTACK06_H_