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
class CMagicCircle;	// ���@�w�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �{�X�h���S���N���X
class CEnemyBossDragon : public CEnemy
{
public:
	// ���f����
	enum EModel
	{
		MODEL_WAIST = 0,	// ��
		MODEL_BODY,			// ��
		MODEL_JAW,			// �{
		MODEL_HEAD,			// ��
		MODEL_ARM_UL,		// ����r
		MODEL_ARM_UR,		// �E��r
		MODEL_ARM_DL,		// �����r
		MODEL_ARM_DR,		// �E���r
		MODEL_HAND_L,		// ����
		MODEL_HAND_R,		// �E��
		MODEL_LEG_UL,		// ��������
		MODEL_LEG_UR,		// �E������
		MODEL_LEG_DL,		// ����
		MODEL_LEG_DR,		// �E��
		MODEL_FOOT_L,		// ����
		MODEL_FOOT_R,		// �E��
		MODEL_WING_L_00,	// ����00
		MODEL_WING_R_00,	// �E��00
		MODEL_WING_L_01,	// ����01
		MODEL_WING_R_01,	// �E��01
		MODEL_WING_L_02,	// ����02
		MODEL_WING_R_02,	// �E��02
		MODEL_WING_L_03,	// ����03
		MODEL_WING_R_03,	// �E��03
		MODEL_TAIL_00,		// �K��00
		MODEL_TAIL_01,		// �K��01
		MODEL_TAIL_02,		// �K��02
		MODEL_TAIL_03,		// �K��03
		MODEL_TAIL_04,		// �K��04
		MODEL_TAIL_05,		// �K��05
		MODEL_MAX			// ���̗񋓌^�̑���
	};

	// ���[�V������
	enum EMotion
	{
		MOTION_IDOL = 0,		// �ҋ@���[�V����
		MOTION_PUNCH_GROUND,	// �n�ʉ��胂�[�V����
		MOTION_MAX				// ���̗񋓌^�̑���
	};

	// �s����
	enum EAction
	{
		ACT_NONE = 0,		// �������Ȃ�
		ACT_MAGIC_FADEIN,	// ���@�w�t�F�[�h�C��
		ACT_MAGIC_FADEOUT,	// ���@�w�t�F�[�h�A�E�g
		ACT_PUNCH_GROUND,	// �n�ʉ���
		ACT_MAX				// ���̗񋓌^�̑���
	};

	// �e���|�[�g��
	enum ETeleport
	{
		TELEPORT_SET_APPER = 0,	// ���@�w�̏o���ݒ�
		TELEPORT_APPEAR,		// ���@�w�̏o��
		TELEPORT_MOVE,			// ���@�w�̈ړ�
		TELEPORT_DISAPPEAR,		// ���@�w�̏���
		TELEPORT_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	explicit CEnemyBossDragon(const EType type);

	// �f�X�g���N�^
	~CEnemyBossDragon() override;

	// �e���|�[�g�\����
	struct STeleport
	{
		D3DXVECTOR3 pos;	// �e���|�[�g�ڕW�ʒu
		D3DXVECTOR3 rot;	// �e���|�[�g�ڕW����
		ETeleport state;	// �e���|�[�g���
	};

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��
	void Hit(const int nDamage) override;	// �q�b�g
	void HitKnockBack(const int nDamage, const D3DXVECTOR3 &vecKnock) override;	// �m�b�N�o�b�N�q�b�g

	// �����o�֐�
	void SetTeleport(const D3DXVECTOR3& rPos, const D3DXVECTOR3 &rRot);	// �e���|�[�g�ݒ�
	void SetActPunchGround(void);	// �n�ʉ���s���ݒ�
	EAction GetAction(void);		// �s���擾

private:
	// �I�[�o�[���C�h�֐�
	const char *GetModelFileName(const int nModel) const override;	// ���f���t�@�C���擾
	void UpdateMotion(void) override;	// ���[�V�����E�I�u�W�F�N�g�L�����N�^�[�̍X�V
	void UpdateNormal(void) override;	// �ʏ��Ԏ��̍X�V

	// �����o�֐�
	void UpdateAttack(void);	// �U���X�V
	void UpdateAction(void);	// �s���X�V

	void UpdateMagicFadeIn(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pRot);	// ���@�w�t�F�[�h�C���s�����̍X�V
	void UpdateMagicFadeOut(const D3DXVECTOR3& rPos);	// ���@�w�t�F�[�h�A�E�g�s�����̍X�V
	void UpdatePunchGround(void);	// �n�ʉ���s�����̍X�V

	// �����o�ϐ�
	CEnemyAttack *m_pAttack;		// �U���̏��
	CMagicCircle *m_pMagicCircle;	// ���@�w�̏��
	STeleport m_teleport;	// �e���|�[�g�̏��
	EAction m_action;		// �s��
	int m_nCounterAttack;	// �U���Ǘ��J�E���^�[
};

#endif	// _ENEMY_BOSS_DRAGON_H_
