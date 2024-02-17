//============================================================
//
//	�G�U��04�w�b�_�[ [enemyAttack04.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _ENEMY_ATTACK04_H_
#define _ENEMY_ATTACK04_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "enemyAttack.h"

//************************************************************
//	�N���X��`
//************************************************************
// �G�U��04�N���X
/*
	@brief �{�X�̍U���Ǘ��N���X �����O������f���o��
*/
class CEnemyAttack04 : public CEnemyAttack
{
public:
	// ��ԗ�
	enum EState
	{
		STATE_INIT_TELEPORT = 0,	// �e���|�[�g�̏�����
		STATE_TELEPORT,				// �e���|�[�g
		STATE_INIT_WAIT,			// �U���ҋ@�̏�����
		STATE_WAIT,					// �U���ҋ@
		STATE_FIRE,					// ������
		STATE_END,					// �I��
		STATE_MAX					// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CEnemyAttack04();

	// �f�X�g���N�^
	~CEnemyAttack04() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	bool Update(void) override;		// �X�V

private:
	// �����o�֐�
	void InitTeleport(void);	// �e���|�[�g�̏�����
	void UpdateFire(void);		// �����˂̍X�V
	void CreateFire(void);		// ���̐���

	// �����o�ϐ�
	EState m_state;			// ���
	int m_nCounterWait;		// �]�C�Ǘ��J�E���^�[
	int m_nCounterNumAtk;	// �U���񐔃J�E���^�[
};

#endif	// _ENEMY_ATTACK04_H_
