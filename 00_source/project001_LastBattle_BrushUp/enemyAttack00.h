//============================================================
//
//	�G�U��00�w�b�_�[ [enemyAttack00.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _ENEMY_ATTACK00_H_
#define _ENEMY_ATTACK00_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "enemyAttack.h"

//************************************************************
//	�N���X��`
//************************************************************
// �G�U��00�N���X
/*
	@brief �{�X�̍U���Ǘ��N���X �n�ʂ������Ĕg�����o��
*/
class CEnemyAttack00 : public CEnemyAttack
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
	explicit CEnemyAttack00(CEnemyBossDragon *pBoss);

	// �f�X�g���N�^
	~CEnemyAttack00() override;

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
	void UpdateWave(void);		// �g�����˂̍X�V

	// �����o�ϐ�
	EState m_state;			// ���
	int m_nCounterState;	// ��ԊǗ��J�E���^�[
};

#endif	// _ENEMY_ATTACK00_H_
