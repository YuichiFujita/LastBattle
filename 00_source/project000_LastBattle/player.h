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
	const int NUM_ORBIT = 2;	// �O�Ղ̐�
}

//************************************************************
//	�O���錾
//************************************************************
class CObjectChara;	// �I�u�W�F�N�g�L�����N�^�[�N���X
class CMultiModel;	// �}���`���f���N���X
class COrbit;		// �O�ՃN���X
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
		U_MODEL_SWORDL,		// ����
		U_MODEL_SWORDR,		// �E��
		U_MODEL_MAX			// ���̗񋓌^�̑���
	};

	// �����g���[�V������
	enum ELowerMotion
	{
		L_MOTION_IDOL = 0,	// �ҋ@���[�V����
		L_MOTION_MAX		// ���̗񋓌^�̑���
	};

	// �㔼�g���[�V������
	enum EUpperMotion
	{
		U_MOTION_IDOL = 0,	// �ҋ@���[�V����
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
	// �����o�֐�
	void UpdateSpawn(int *pLowMotion, int *pUpMotion);	// �X�|�[����Ԏ��̍X�V
	void UpdateNormal(int *pLowMotion, int *pUpMotion);	// �ʏ��Ԏ��̍X�V
	void UpdateMotion(const int nLowMotion, const int nUpMotion);	// ���[�V�����E�I�u�W�F�N�g�L�����N�^�[�̍X�V

	void UpdateOldPosition(void);			// �ߋ��ʒu�̍X�V
	void UpdateMove(void);					// �ړ��ʁE�ڕW�����̍X�V
	void UpdateJump(void);					// �W�����v�̍X�V
	void UpdateGravity(void);				// �d�͂̍X�V
	bool UpdateLanding(D3DXVECTOR3 *pPos);	// ���n�󋵂̍X�V
	void UpdatePosition(D3DXVECTOR3 *pPos);	// �ʒu�̍X�V
	void UpdateRotation(D3DXVECTOR3 *pRot);	// �����̍X�V
	bool UpdateFadeOut(const float fAdd);	// �t�F�[�h�A�E�g��Ԏ��̍X�V
	bool UpdateFadeIn(const float fSub);	// �t�F�[�h�C����Ԏ��̍X�V

	void LoadSetup(const EBody bodyID, const char **ppModelPass);	// �Z�b�g�A�b�v

	// �ÓI�����o�ϐ�
	static CListManager<CPlayer> *m_pList;	// �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CPlayer>::AIterator m_iterator;	// �C�e���[�^�[
	COrbit	*m_apOrbit[player::NUM_ORBIT];			// �O�Ղ̏��
	CShadow	*m_pShadow;			// �e�̏��
	D3DXVECTOR3	m_oldPos;		// �ߋ��ʒu
	D3DXVECTOR3	m_move;			// �ړ���
	D3DXVECTOR3	m_destRot;		// �ڕW����
	EState	m_state;			// ���
	int		m_nCounterState;	// ��ԊǗ��J�E���^�[
	bool	m_bJump;			// �W�����v��
};

#endif	// _PLAYER_H_
