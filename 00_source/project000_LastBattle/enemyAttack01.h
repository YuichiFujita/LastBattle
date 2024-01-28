//============================================================
//
//	�G�U��01�w�b�_�[ [enemyAttack01.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _ENEMY_ATTACK01_H_
#define _ENEMY_ATTACK01_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "enemyAttack.h"

//************************************************************
//	�N���X��`
//************************************************************
// �G�U��01�N���X
/*
	@brief �{�X�̍U���Ǘ��N���X �������
*/
class CEnemyAttack01 : public CEnemyAttack
{
public:
	// ��ԗ�
	enum EState
	{
		STATE_INIT_TELEPORT = 0,	// �e���|�[�g�̏�����
		STATE_TELEPORT,				// �e���|�[�g
		STATE_INIT_WAVE,			// �g�����˂̏�����
		STATE_WAVE,					// �g������
		STATE_WAIT,					// �ҋ@
		STATE_END,					// �I��
		STATE_MAX					// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CEnemyAttack01();

	// �f�X�g���N�^
	~CEnemyAttack01() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	bool Update(void) override;		// �X�V

private:
	// �����o�ϐ�
	EState m_state;			// ���
	int m_nCounterState;	// ��ԊǗ��J�E���^�[
	int m_nCounterNumAtk;	// �U���񐔃J�E���^�[
};

#endif	// _ENEMY_ATTACK01_H_
