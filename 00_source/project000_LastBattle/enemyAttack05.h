//============================================================
//
//	�G�U��05�w�b�_�[ [enemyAttack05.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _ENEMY_ATTACK05_H_
#define _ENEMY_ATTACK05_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "enemyAttack.h"

//************************************************************
//	�N���X��`
//************************************************************
// �G�U��05�N���X
/*
	@brief �{�X�̍U���Ǘ��N���X �܂łЂ������U��
*/
class CEnemyAttack05 : public CEnemyAttack
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
	explicit CEnemyAttack05(CEnemyBossDragon *pBoss);

	// �f�X�g���N�^
	~CEnemyAttack05() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	bool Update(void) override;		// �X�V

private:
	// �����o�֐�
	void SetEnableTeleport(void);	// �e���|�[�g���邩�̐ݒ�
	void InitTeleport(void);		// �e���|�[�g�̏�����
	void UpdateAttack(void);		// �U���̍X�V

	// �����o�ϐ�
	EState m_state;	// ���
};

#endif	// _ENEMY_ATTACK05_H_
