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
	@brief �{�X�̍U���Ǘ��N���X ���𒆐S����X�e�[�W�O���Ɍ������ĕ���
*/
class CEnemyAttack02 : public CEnemyAttack
{
public:
	// ��ԗ�
	enum EState
	{
		STATE_INIT_TELEPORT = 0,	// �e���|�[�g�̏�����
		STATE_TELEPORT,				// �e���|�[�g
		STATE_INIT_WAIT,			// �U���ҋ@�̏�����
		STATE_WAIT,					// �U���ҋ@
		STATE_THUNDER,				// ������
		STATE_END,					// �I��
		STATE_MAX					// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	explicit CEnemyAttack02(CEnemyBossDragon *pBoss);

	// �f�X�g���N�^
	~CEnemyAttack02() override;

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
	void InitTeleport(void);	// �e���|�[�g�̏�����
	void UpdateThunder(void);	// �����˂̍X�V

	// �����o�ϐ�
	EState m_state;			// ���
	int m_nCounterWait;		// �]�C�Ǘ��J�E���^�[
	int m_nCounterNumAtk;	// �U���񐔃J�E���^�[
	int m_nWaitFrame;		// �]�C�t���[����
};

#endif	// _ENEMY_ATTACK02_H_
