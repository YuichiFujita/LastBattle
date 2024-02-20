//============================================================
//
//	�G�U��03�w�b�_�[ [enemyAttack03.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _ENEMY_ATTACK03_H_
#define _ENEMY_ATTACK03_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "enemyAttack.h"

//************************************************************
//	�N���X��`
//************************************************************
// �G�U��03�N���X
/*
	@brief �{�X�̍U���Ǘ��N���X �����v���C���[�Ɍ������ĘA���ŕ���
*/
class CEnemyAttack03 : public CEnemyAttack
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
	explicit CEnemyAttack03(CEnemyBossDragon *pBoss);

	// �f�X�g���N�^
	~CEnemyAttack03() override;

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

#endif	// _ENEMY_ATTACK03_H_
