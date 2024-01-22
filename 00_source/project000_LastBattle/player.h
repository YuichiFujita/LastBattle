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
class CSword;		// ���N���X
class CShadow;		// �e�N���X

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
		L_MOTION_IDOL = 0,	// �ҋ@���[�V����
		L_MOTION_MOVE,		// ���s���[�V����
		L_MOTION_ATTACK_00,	// �U�����[�V������i�K��
		L_MOTION_ATTACK_01,	// �U�����[�V������i�K��
		L_MOTION_MAX		// ���̗񋓌^�̑���
	};

	// �㔼�g���[�V������
	enum EUpperMotion
	{
		U_MOTION_IDOL = 0,	// �ҋ@���[�V����
		U_MOTION_MOVE,		// ���s���[�V����
		U_MOTION_ATTACK_00,	// �U�����[�V������i�K��
		U_MOTION_ATTACK_01,	// �U�����[�V������i�K��
		U_MOTION_MAX		// ���̗񋓌^�̑���
	};

	// ��ԗ�
	enum EState
	{
		STATE_NONE = 0,	// �������Ȃ����
		STATE_SPAWN,	// �X�|�[�����
		STATE_NORMAL,	// �ʏ���
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
	void Draw(void) override;		// �`��
	void Hit(void) override;		// �q�b�g

	void SetState(const int nState) override;	// ��Ԑݒ�
	int  GetState(void) const override;			// ��Ԏ擾
	float GetRadius(void) const override;		// ���a�擾
	float GetHeight(void) const override;		// �c���擾

	void SetEnableUpdate(const bool bUpdate) override;	// �X�V�󋵐ݒ�
	void SetEnableDraw(const bool bDraw) override;		// �`��󋵐ݒ�
	D3DXMATRIX CalcMtxWorld(void) const override;		// �}�g���b�N�X�v�Z���ʎ擾

	// �ÓI�����o�֐�
	static CPlayer *Create(CScene::EMode mode);		// ����
	static CListManager<CPlayer> *GetList(void);	// ���X�g�擾

	// �����o�֐�
	void SetSpawn(void);	// �o���ݒ�

private:
	// ����\����
	struct SDodge
	{
		bool  bDodge;	// �����
		float fMove;	// ����ړ���
		float fRot;		// ������
	};

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

	// ���[�V�����X�V�̊֐��|�C���^�^�G�C���A�X��`
	typedef void (CPlayer::*AFuncUpdateMotion)(const int);

	// �I�[�o�[���C�h�֐�
	void SetMotion(const EBody bodyID, const int nType) override;	// ���[�V�����ݒ�

	// �����o�֐�
	void LoadSetup(const EBody bodyID, const char **ppModelPass);	// �Z�b�g�A�b�v
	void UpdateMotion(const int nLowMotion, const int nUpMotion);	// ���[�V�����E�I�u�W�F�N�g�L�����N�^�[�̍X�V
	void UpdateMotionLower(const int nMotion);	// �����g���[�V�����̍X�V
	void UpdateMotionUpper(const int nMotion);	// �㔼�g���[�V�����̍X�V
	bool IsAttack(void) const;		// �U���󋵎擾
	void SetLStickRotation(void);	// L�X�e�B�b�N��������������

	void UpdateSpawn(int *pLowMotion, int *pUpMotion);	// �X�|�[����Ԏ��̍X�V
	void UpdateNormal(int *pLowMotion, int *pUpMotion);	// �ʏ��Ԏ��̍X�V

	void UpdateAttack(void);	// �U������̍X�V
	void UpdateDodge(void);		// ��𑀍�̍X�V
	void UpdateMove(int *pLowMotion, int *pUpMotion);	// �ړ�����E�ڕW�����̍X�V
	void UpdateJump(int *pLowMotion, int *pUpMotion);	// �W�����v����̍X�V

	void UpdateOldPosition(void);			// �ߋ��ʒu�̍X�V
	void UpdateGravity(void);				// �d�͂̍X�V
	bool UpdateLanding(D3DXVECTOR3 *pPos);	// ���n�󋵂̍X�V
	void UpdatePosition(D3DXVECTOR3 *pPos);	// �ʒu�̍X�V
	void UpdateRotation(D3DXVECTOR3 *pRot);	// �����̍X�V
	bool UpdateFadeOut(const float fAdd);	// �t�F�[�h�A�E�g��Ԏ��̍X�V
	bool UpdateFadeIn(const float fSub);	// �t�F�[�h�C����Ԏ��̍X�V

	// �ÓI�����o�ϐ�
	static CListManager<CPlayer> *m_pList;			// �I�u�W�F�N�g���X�g
	static AFuncUpdateMotion m_aFuncUpdateMotion[];	// ���[�V�����X�V�֐�

	// �����o�ϐ�
	CListManager<CPlayer>::AIterator m_iterator;	// �C�e���[�^�[
	CSword		*m_apSowrd[player::NUM_SWORD];		// ���̏��
	CShadow		*m_pShadow;			// �e�̏��
	D3DXVECTOR3	m_oldPos;			// �ߋ��ʒu
	D3DXVECTOR3	m_move;				// �ړ���
	D3DXVECTOR3	m_destRot;			// �ڕW����
	EState		m_state;			// ���
	int			m_nCounterState;	// ��ԊǗ��J�E���^�[
	bool		m_bJump;			// �W�����v��
	SDodge		m_dodge;			// ����̏��
	SBuffering	m_buffAttack;		// �U���̐�s����
};

#endif	// _PLAYER_H_
