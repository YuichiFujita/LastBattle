//============================================================
//
//	�G�U��02�w�b�_�[ [enemyAttack02.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _ENEMY_ATTACK02_H_
#define _ENEMY_ATTACK02_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "enemyAttack.h"

//************************************************************
//	�N���X��`
//************************************************************
// �G�U��02�N���X
/*
	@brief �{�X�̍U���Ǘ��N���X �����O������X�e�[�W���S�Ɍ������ĕ���
*/
class CEnemyAttack02 : public CEnemyAttack
{
public:
	// ��ԗ�
	enum EState
	{
		STATE_INIT_TELEPORT = 0,	// �e���|�[�g�̏�����
		STATE_TELEPORT,				// �e���|�[�g
		STATE_INIT_THUNDER,			// �����˂̏�����
		STATE_THUNDER,				// ������
		STATE_CENTER_TELEPORT_INIT,	// �����e���|�[�g�̏�����
		STATE_CENTER_TELEPORT,		// �����e���|�[�g
		STATE_END,					// �I��
		STATE_MAX					// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CEnemyAttack02();

	// �f�X�g���N�^
	~CEnemyAttack02() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	bool Update(void) override;		// �X�V

private:
	// �����o�ϐ�
	EState m_state;			// ���
	int m_nCounterWait;		// �]�C�Ǘ��J�E���^�[
	int m_nCounterNumAtk;	// �U���񐔃J�E���^�[
	int m_nWaitFrame;		// �]�C�t���[����
};

#endif	// _ENEMY_ATTACK02_H_