//============================================================
//
//	�v���C���[�w�b�_�[ [player.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _PLAYER_H_
#define _PLAYER_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "objectDivChara.h"
#include "scene.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace player
{
	const int NUM_SWORD = 2;	// ���̐�
}

//************************************************************
//	�O���錾
//************************************************************
class CObjectChara;	// �I�u�W�F�N�g�L�����N�^�[�N���X
class CMultiModel;	// �}���`���f���N���X
class CBlur;		// �u���[�N���X
class CSword;		// ���N���X
class CGauge2D;		// �Q�[�W2D�N���X
class CShadow;		// �e�N���X
class CPlayControl;	// �v���C����N���X

//************************************************************
//	�N���X��`
//************************************************************
// �v���C���[�N���X
class CPlayer : public CObjectDivChara
{
public:
	// �����g���f����
	enum ELowerModel
	{
		L_MODEL_WAIST = 0,	// ��
		L_MODEL_LEGUL,		// ��������
		L_MODEL_LEGUR,		// �E������
		L_MODEL_LEGDL,		// ����
		L_MODEL_LEGDR,		// �E��
		L_MODEL_FOOTL,		// ����
		L_MODEL_FOOTR,		// �E��
		L_MODEL_MAX			// ���̗񋓌^�̑���
	};

	// �㔼�g���f����
	enum EUpperModel
	{
		U_MODEL_BODY = 0,	// ��
		U_MODEL_HEAD,		// ��
		U_MODEL_ARMUL,		// ����r
		U_MODEL_ARMUR,		// �E��r
		U_MODEL_ARMDL,		// �����r
		U_MODEL_ARMDR,		// �E���r
		U_MODEL_HANDL,		// ����
		U_MODEL_HANDR,		// �E��
		U_MODEL_MAX			// ���̗񋓌^�̑���
	};

	// �����g���[�V������
	enum ELowerMotion
	{
		L_MOTION_SPAWN = 0,			// �o�ꃂ�[�V����
		L_MOTION_IDOL,				// �ҋ@���[�V����
		L_MOTION_MOVE,				// ���s���[�V����
		L_MOTION_ATTACK_00,			// �U�����[�V������i�K��
		L_MOTION_ATTACK_01,			// �U�����[�V������i�K��
		L_MOTION_ATTACK_02,			// �U�����[�V�����O�i�K��
		L_MOTION_DODGE,				// ������[�V����
		L_MOTION_JUMP,				// �W�����v���[�V����
		L_MOTION_FALL,				// �������[�V����
		L_MOTION_LAND,				// ���n���[�V����
		L_MOTION_JUMP_ATTACK_00,	// �󒆍U�����[�V������i�K��
		L_MOTION_JUMP_ATTACK_01,	// �󒆍U�����[�V������i�K��
		L_MOTION_RIDE_IDOL,			// ���C�h�ҋ@���[�V����
		L_MOTION_RIDE_ATTACK_00,	// ���C�h�U�����[�V������i�K��
		L_MOTION_RIDE_ATTACK_01,	// ���C�h�U�����[�V������i�K��
		L_MOTION_DEATH,				// ���S���[�V����
		L_MOTION_TITLE_ATTACK,		// �^�C�g���U�����[�V����
		L_MOTION_MAX				// ���̗񋓌^�̑���
	};

	// �㔼�g���[�V������
	enum EUpperMotion
	{
		U_MOTION_SPAWN = 0,			// �o�ꃂ�[�V����
		U_MOTION_IDOL,				// �ҋ@���[�V����
		U_MOTION_MOVE,				// ���s���[�V����
		U_MOTION_ATTACK_00,			// �U�����[�V������i�K��
		U_MOTION_ATTACK_01,			// �U�����[�V������i�K��
		U_MOTION_ATTACK_02,			// �U�����[�V�����O�i�K��
		U_MOTION_DODGE,				// ������[�V����
		U_MOTION_JUMP,				// �W�����v���[�V����
		U_MOTION_FALL,				// �������[�V����
		U_MOTION_LAND,				// ���n���[�V����
		U_MOTION_JUMP_ATTACK_00,	// �󒆍U�����[�V������i�K��
		U_MOTION_JUMP_ATTACK_01,	// �󒆍U�����[�V������i�K��
		U_MOTION_RIDE_IDOL,			// ���C�h�ҋ@���[�V����
		U_MOTION_RIDE_ATTACK_00,	// ���C�h�U�����[�V������i�K��
		U_MOTION_RIDE_ATTACK_01,	// ���C�h�U�����[�V������i�K��
		U_MOTION_DEATH,				// ���S���[�V����
		U_MOTION_TITLE_ATTACK,		// �^�C�g���U�����[�V����
		U_MOTION_MAX				// ���̗񋓌^�̑���
	};

	// ��ԗ�
	enum EState
	{
		STATE_NONE = 0,	// �������Ȃ����
		STATE_SPAWN,	// �X�|�[�����
		STATE_NORMAL,	// �ʏ���
		STATE_RIDE,		// ���C�h���
		STATE_RIDE_END,	// ���C�h�I�����
		STATE_KNOCK,	// �m�b�N�o�b�N���
		STATE_DAMAGE,	// �_���[�W���
		STATE_INVULN,	// ���G���
		STATE_DEATH,	// ���S���
		STATE_MAX		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CPlayer();

	// �f�X�g���N�^
	~CPlayer() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(CShader *pShader = nullptr) override;		// �`��
	void SetState(const int nState) override;			// ��Ԑݒ�
	int GetState(void) const override;					// ��Ԏ擾
	float GetRadius(void) const override;				// ���a�擾
	float GetHeight(void) const override;				// �c���擾
	void SetEnableUpdate(const bool bUpdate) override;	// �X�V�󋵐ݒ�
	void SetEnableDraw(const bool bDraw) override;		// �`��󋵐ݒ�
	D3DXMATRIX CalcMtxWorld(void) const override;		// �}�g���b�N�X�v�Z���ʎ擾

	// ���z�֐�
	virtual void SetStag(const int /*nStag*/) {}			// ���o��Ԑݒ�
	virtual int GetStag(void) const { return NONE_IDX; }	// ���o��Ԏ擾

	// �ÓI�����o�֐�
	static CPlayer *Create(CScene::EMode mode);		// ����
	static CListManager<CPlayer> *GetList(void);	// ���X�g�擾

	// �����o�֐�
	void SetDestRotation(const D3DXVECTOR3& rRot) { m_destRot = rRot; }	// �ڕW�����ݒ�
	D3DXVECTOR3 GetDestRotation(void) const		  { return m_destRot; }	// �ڕW�����擾
	bool HitKnockBack(const int nDamage, const D3DXVECTOR3& rVecKnock);	// �m�b�N�o�b�N�q�b�g
	bool Hit(const int nDamage);				// �q�b�g
	void SetEnableDrawUI(const bool bDraw);		// UI�`��ݒ�
	void SetLifePriority(const int nPrio);		// �̗̓Q�[�W�̗D�揇�ʐݒ�
	void SetNoneTwinSword(void);				// ���̏�ԏ�����
	void SetLanding(const D3DXVECTOR3& rPos);	// �������n����ʒu�ݒ�
	void InitNormal(void);	// �ʏ��Ԃ̏�����
	void SetSpawn(void);	// �X�|�[���ݒ�
	void SetInvuln(void);	// ���G�ݒ�
	void SetRide(void);		// ���C�h�ݒ�
	void SetRideEnd(void);	// ���C�h�I���ݒ�

protected:
	// ���z�֐�
	virtual void UpdateNormal(int *pLowMotion, int *pUpMotion);	// �ʏ��Ԏ��̍X�V

	// �I�[�o�[���C�h�֐�
	void SetMotion	// ���[�V�����ݒ�
	( // ����
		const EBody bodyID,			// �g�̃C���f�b�N�X
		const int nType,			// ���[�V�������
		const int nBlendFrame = 0	// �u�����h�t���[��
	) override;

	// �����o�֐�
	CSword *GetSword(const int nSword);		// ���̎擾
	CShadow *GetShadow(void);				// �e�̎擾
	void UpdateOldPosition(void);			// �ߋ��ʒu�̍X�V
	void UpdateGravity(void);				// �d�͂̍X�V
	bool UpdateLanding(D3DXVECTOR3 *pPos);	// ���n�󋵂̍X�V
	void UpdatePosition(D3DXVECTOR3 *pPos);	// �ʒu�̍X�V
	void UpdateRotation(D3DXVECTOR3 *pRot);	// �����̍X�V
	bool UpdateFadeOut(const float fAdd);	// �t�F�[�h�A�E�g��Ԏ��̍X�V
	bool UpdateFadeIn(const float fSub);	// �t�F�[�h�C����Ԏ��̍X�V

	bool UpdateAttack	// �U������̍X�V
	( // ����
		const D3DXVECTOR3& rPos,	// �v���C���[�ʒu
		const D3DXVECTOR3& rRot		// �v���C���[����
	);
	bool UpdateLandAttack(void);	// �n��U������̍X�V
	bool UpdateSkyAttack(void);		// �󒆍U������̍X�V
	bool UpdateRideAttack(void);	// �R��U������̍X�V
	bool UpdateRide(const D3DXVECTOR3& rPos);			// �R�摀��̍X�V
	bool UpdateDodge(const D3DXVECTOR3& rRot);			// ��𑀍�̍X�V
	bool UpdateMove(int *pLowMotion, int *pUpMotion);	// �ړ�����E�ڕW�����̍X�V
	bool UpdateJump(int *pLowMotion, int *pUpMotion);	// �W�����v����̍X�V
	void UpdateCollEnemy(D3DXVECTOR3 *pPos);			// �G�Ƃ̓����蔻��̍X�V

private:
	// ��s���͍\����
	struct SBuffering
	{
	public:
		// ��s���͂̔��f���̉��Z
		bool Add(void)
		{
			// ���f�������Z
			nNumSet++;

			if (nNumSet >= BODY_MAX)
			{ // �S���[�V�����ɔ��f�����ꍇ

				// ����������
				bInput  = false;	// ��s���͏�
				nNumSet = 0;		// ��s���͂̔��f��

				return true;
			}

			return false;
		}

		// �����o�ϐ�
		bool bInput;	// ��s���͏�
		int  nNumSet;	// ��s���͂̔��f��
	};

	// �U���\����
	struct SAttack : public SBuffering
	{
	public:
		// ��ޗ�
		enum EType
		{
			TYPE_LAND = 0,	// �n��U��
			TYPE_SKY,		// �󒆍U��
			TYPE_MAX		// ���̗񋓌^�̑���
		};

		// �����o�ϐ�
		EType curType;	// ���݂̍U�����
	};

	// ����\����
	struct SDodge
	{
		bool  bDodge;		// �����
		float fMove;		// ����ړ���
		float fRot;			// ������
		int   nWaitCounter;	// �N�[���^�C���Ǘ��J�E���^�[
	};

	// �W�����v�\����
	struct SJump
	{
		bool bJump;			// �W�����v��
		bool bInputPress;	// �v���X���͏�
		int  nPressCounter;	// �v���X���͊Ǘ��J�E���^�[
	};

	// ���[�V�����X�V�̊֐��|�C���^�^�G�C���A�X��`
	typedef void (CPlayer::*AFuncUpdateMotion)(const int);

	// �����o�֐�
	void LoadSetup(const EBody bodyID, const char **ppModelPass);	// �Z�b�g�A�b�v
	void UpdateMotion(const int nLowMotion, const int nUpMotion);	// ���[�V�����X�V
	void UpdateMotionLower(const int nMotion);	// �����g���[�V�����X�V
	void UpdateMotionUpper(const int nMotion);	// �㔼�g���[�V�����X�V

	void SetDestLookEnemy	// �ڕW������G�ɂ���
	( // ����
		const D3DXVECTOR3& rPosPlayer,	// �v���C���[�ʒu
		const D3DXVECTOR3& rRotPlayer,	// �v���C���[����
		const float fRate				// �����␳�W��
	);
	void SetLStickRotation(void);	// L�X�e�B�b�N��������������
	bool IsAttack(void) const;		// �U���󋵎擾

	void UpdateSpawn(void);		// �X�|�[����Ԏ��̍X�V
	void UpdateKnock(void);		// �m�b�N�o�b�N��Ԏ��̍X�V
	void UpdateRideEnd(void);	// ���C�h�I����Ԏ��̍X�V
	void UpdateRide(int *pLowMotion, int *pUpMotion);	// ���C�h��Ԏ��̍X�V
	void UpdateDamage(int *pLowMotion, int *pUpMotion);	// �_���[�W��Ԏ��̍X�V
	void UpdateInvuln(int *pLowMotion, int *pUpMotion);	// ���G��Ԏ��̍X�V
	void UpdateDeath(int *pLowMotion, int *pUpMotion);	// ���S��Ԏ��̍X�V

	// �ÓI�����o�ϐ�
	static CListManager<CPlayer> *m_pList;			// �I�u�W�F�N�g���X�g
	static AFuncUpdateMotion m_aFuncUpdateMotion[];	// ���[�V�����X�V�֐�

	// �����o�ϐ�
	CListManager<CPlayer>::AIterator m_iterator;	// �C�e���[�^�[
	CSword			*m_apSword[player::NUM_SWORD];	// ���̏��
	CBlur			*m_apBlur[BODY_MAX];			// �u���[�̏��
	CPlayControl	*m_pPlayControl;	// �v���C����̏��
	CGauge2D		*m_pLife;			// �̗͂̏��
	CShadow			*m_pShadow;			// �e�̏��
	D3DXVECTOR3		m_oldPos;			// �ߋ��ʒu
	D3DXVECTOR3		m_move;				// �ړ���
	D3DXVECTOR3		m_destRot;			// �ڕW����
	EState			m_state;			// ���
	int				m_nCounterState;	// ��ԊǗ��J�E���^�[
	float			m_fSinAlpha;		// ��������
	SJump			m_jump;				// �W�����v�̏��
	SDodge			m_dodge;			// ����̏��
	SAttack			m_attack;			// �U���̏��
};

#endif	// _PLAYER_H_
