//============================================================
//
//	�{�X�h���S���w�b�_�[ [enemyBossDragon.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _ENEMY_BOSS_DRAGON_H_
#define _ENEMY_BOSS_DRAGON_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "enemy.h"

//************************************************************
//	�O���錾
//************************************************************
class CEnemyAttack;	// �G�U���N���X

//************************************************************
//	�N���X��`
//************************************************************
// �{�X�h���S���N���X
class CEnemyBossDragon : public CEnemy
{
public:
	// ��ޗ�
	enum EModel
	{
		MODEL_NORMAL = 0,	// �ʏ�
		MODEL_MAX			// ���̗񋓌^�̑���
	};

	// �ʏ��ԗ�
	enum ENormalState
	{
		NSTATE_NONE = 0,	// �������Ȃ����
		NSTATE_WAIT,		// �ҋ@���
		NSTATE_ATTACK,		// �U�����
		NSTATE_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	explicit CEnemyBossDragon(const EType type);

	// �f�X�g���N�^
	~CEnemyBossDragon() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��
	void Hit(const int nDamage) override;	// �q�b�g
	void HitKnockBack(const int nDamage, const D3DXVECTOR3 &vecKnock) override;	// �m�b�N�o�b�N�q�b�g

private:
	// �I�[�o�[���C�h�֐�
	void UpdateNormal(void) override;	// �ʏ��Ԏ��̍X�V

	// �����o�ϐ�
	CEnemyAttack *m_pAttack;	// �U���̏��
	ENormalState m_stateNormal;	// �ʏ��Ԏ��̏��
	int m_nCounterNormal;		// �ʏ��ԊǗ��J�E���^�[
};

#endif	// _ENEMY_BOSS_DRAGON_H_
