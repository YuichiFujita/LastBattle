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
#include "enemyAttack.h"

//************************************************************
//	�O���錾
//************************************************************
class CGauge2D;		// �Q�[�W2D�N���X
class CShadow;		// �e�N���X
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
		MOTION_NONE = -1,		// ���[�V�����w�薳��
		MOTION_HOWL,			// ���K���[�V����
		MOTION_IDOL,			// �ҋ@���[�V����
		MOTION_FLY_IDOL,		// �󒆑ҋ@���[�V����
		MOTION_PUNCH_GROUND,	// �n�ʉ��胂�[�V����
		MOTION_FIRE_ATTACK,		// ���U�����[�V����
		MOTION_CLAW_ATTACK,		// �Ђ������U�����[�V����
		MOTION_TAIL_ATTACK,		// �����ۍU�����[�V����
		MOTION_FLY_ATTACK,		// �󒆍U�����[�V����
		MOTION_FLY_RUSH,		// �󒆓ːi�U�����[�V����
		MOTION_KNOCK,			// �m�b�N�o�b�N���[�V����
		MOTION_STAN,			// �X�^�����[�V����
		MOTION_HOWL_FLYUP,		// ���K��яオ�胂�[�V����
		MOTION_SHAKE_OFF,		// �󒆐U�艺�낵���[�V����
		MOTION_COME_DOWN,		// ��󕜋A���[�V����
		MOTION_DEATH,			// ���S���[�V����
		MOTION_MAX				// ���̗񋓌^�̑���
	};

	// �s����
	enum EAction
	{
		ACT_NONE = 0,		// �������Ȃ�
		ACT_MAGIC_FADEIN,	// ���@�w�t�F�[�h�C��
		ACT_MAGIC_FADEOUT,	// ���@�w�t�F�[�h�A�E�g
		ACT_PUNCH_GROUND,	// �n�ʉ���
		ACT_FIRE_ATTACK,	// ���U��
		ACT_CLAW_ATTACK,	// �Ђ������U��
		ACT_TAIL_ATTACK,	// �����ۍU��
		ACT_FLY_ATTACK,		// �󒆍U��
		ACT_MAX				// ���̗񋓌^�̑���
	};

	// �e���|�[�g��
	enum ETeleport
	{
		TELEPORT_INIT = 0,	// �e���|�[�g�̏�����
		TELEPORT_APPEAR,	// ���@�w�̏o��
		TELEPORT_MOVE,		// ���@�w�̈ړ�
		TELEPORT_DISAPPEAR,	// ���@�w�̏���
		TELEPORT_MAX		// ���̗񋓌^�̑���
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
		EMotion motion;		// �e���|�[�g�ヂ�[�V����
		bool bLook;			// �e���|�[�g��ɃJ�������������邩
	};

	// ���C�h�\����
	struct SRide
	{
		float fDir;	// �X�e�[�W�������猩���{�X�̕���
	};

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(CShader *pShader = nullptr) override;			// �`��
	int  GetWaistModelID(void) const override;				// �����f���̃C���f�b�N�X�擾
	int  GetHeadModelID(void) const override;				// �����f���̃C���f�b�N�X�擾
	bool IsRideOK(const D3DXVECTOR3& rPos) const override;	// ���C�h�\���擾
	void SetEnableDrawUI(const bool bDraw) override;		// UI�`��ݒ�
	void SetLifePriority(const int nPrio) override;			// �̗̓Q�[�W�̗D�揇�ʐݒ�
	bool Hit(const int nDamage) override;					// �q�b�g
	bool HitKnockBack(const int nDamage, const D3DXVECTOR3 &vecKnock) override;	// �m�b�N�o�b�N�q�b�g

	// �����o�֐�
	void InitNormal(void);	// �ʏ��Ԃ̏�����
	void DrawCrop(void);	// �؂蔲���p�̕`��

	void SetTeleport	// �e���|�[�g�ݒ�
	( // ����
		const D3DXVECTOR3& rPos,			// �e���|�[�g�ڕW�ʒu
		const D3DXVECTOR3& rRot,			// �e���|�[�g�ڕW����
		const EMotion motion = MOTION_IDOL,	// �e���|�[�g�ヂ�[�V����
		const bool bLook = true				// �e���|�[�g��ɃJ�������������邩
	);
	void SetActPunchGround(void);	// �n�ʉ���̍s���ݒ�
	void SetActFireAttack(void);	// ���U���̍s���ݒ�
	void SetActClawAttack(void);	// �Ђ������U���̍s���ݒ�
	void SetActTailAttack(void);	// �����ۍU���̍s���ݒ�
	void SetActFlyAttack(void);		// �󒆍U���̍s���ݒ�
	EAction GetAction(void);		// �s���擾

private:
	// �I�[�o�[���C�h�֐�
	const char *GetModelFileName(const int nModel) const override;	// ���f���t�@�C���擾
	void UpdateMotion(void) override;		// ���[�V�����̍X�V
	void SetSpawn(void) override;			// �X�|�[����Ԃ̐ݒ�
	void SetKnock(void) override;			// �m�b�N�o�b�N��Ԃ̐ݒ�
	void SetStan(void) override;			// �X�^����Ԃ̐ݒ�
	void SetRideFlyUp(void) override;		// ���C�h��яオ���Ԃ̐ݒ�
	void SetRideRotate(void) override;		// ���C�h�����Ԃ̐ݒ�
	void SetRideEnd(void) override;			// ���C�h�I����Ԃ̐ݒ�
	void SetDeath(void) override;			// ���S��Ԃ̐ݒ�
	void UpdateSpawn(void) override;		// �X�|�[����Ԏ��̍X�V
	void UpdateNormal(void) override;		// �ʏ��Ԏ��̍X�V
	void UpdateKnock(void) override;		// �m�b�N�o�b�N��Ԏ��̍X�V
	void UpdateStan(void) override;			// �X�^����Ԏ��̍X�V
	void UpdateRideFlyUp(void) override;	// ���C�h��яオ���Ԏ��̍X�V
	void UpdateRideRotate(void) override;	// ���C�h�����Ԏ��̍X�V
	void UpdateRideEnd(void) override;		// ���C�h�I����Ԏ��̍X�V
	void UpdateDeath(void) override;		// ���S��Ԏ��̍X�V

	// �����o�֐�
	void LimitPosition(D3DXVECTOR3 *pPos);	// �ʒu�͈͊O�̕␳
	void SelectAttack(void);	// �U���I��
	void UpdateAttack(void);	// �U���X�V
	void UpdateAction(void);	// �s���X�V
	void SetKnockStaging(void);	// �m�b�N�o�b�N���o�̐ݒ�
	void SetDeathStaging(void);	// ���S���o�̐ݒ�
	bool IsFly(void) const;		// ��s�t���O�̎擾

	void UpdateMagicFadeIn(const D3DXVECTOR3& rPos);	// ���@�w�t�F�[�h�C���s�����̍X�V
	void UpdateMagicFadeOut	// ���@�w�t�F�[�h�A�E�g�s�����̍X�V
	( // ����
		D3DXVECTOR3 *pPos,		// �ʒu
		D3DXVECTOR3 *pRot,		// ����
		D3DXVECTOR3 *pDestRot	// �ڕW����
	);
	void UpdatePunchGround(void);	// �n�ʉ���̍s�����̍X�V
	void UpdateFireAttack(void);	// ���U���̍s�����̍X�V
	void UpdateClawAttack(void);	// �Ђ������U���̍s�����̍X�V
	void UpdateTailAttack(void);	// �����ۍU���̍s�����̍X�V
	void UpdateFlyAttack(void);		// �󒆍U���̍s�����̍X�V

	// �����o�ϐ�
	CGauge2D *m_pLife;				// �̗͂̏��
	CShadow *m_pShadow;				// �e�̏��
	CEnemyAttack *m_pAttack;		// �U���̏��
	CMagicCircle *m_pMagicCircle;	// ���@�w�̏��
	CEnemyAttack::EAttack m_oldAtk;	// �O��̍U��
	CEnemyAttack::EAttack m_curAtk;	// ����̍U��
	STeleport m_teleport;	// �e���|�[�g�̏��
	SRide m_ride;			// ���C�h�̏��
	EAction m_action;		// �s��
	int m_nCounterSameAct;	// �����s���̘A����
	int m_nCounterAttack;	// �U���Ǘ��J�E���^�[
	int m_nCounterRotate;	// ���C�h����J�E���^�[
	int m_nCounterFlash;	// �t���b�V���Ǘ��J�E���^�[
};

#endif	// _ENEMY_BOSS_DRAGON_H_
