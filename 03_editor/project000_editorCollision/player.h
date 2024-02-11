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
#include "objectChara.h"
#include <map>
#include <string>

//************************************************************
//	�O���錾
//************************************************************
class CMultiModel;	// �}���`���f���N���X
class CShadow;		// �e�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �v���C���[�N���X
class CPlayer : public CObjectChara
{
public:
	// ��ԗ�
	enum EState
	{
		STATE_NONE = 0,	// �������Ȃ����
		STATE_NORMAL,	// �ʏ���
		STATE_DEATH,	// ���S���
		STATE_MAX		// ���̗񋓌^�̑���
	};

	// �������[�V������
	enum EMotion
	{
		MOTION_IDOL = 0,	// �ҋ@���[�V�����F���[�vON
		MOTION_MOVE,		// �ړ����[�V�����F���[�vON
		MOTION_JUMP,		// �W�����v���[�V�����F���[�vOFF
		MOTION_LAND,		// ���n���[�V�����F���[�vOFF
		MOTION_MAX			// ���̗񋓌^�̑���
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
	void SetState(const int nState) override;	// ��Ԑݒ�
	int GetState(void) const override;			// ��Ԏ擾
	float GetRadius(void) const override;		// ���a�擾
	float GetHeight(void) const override;		// �c���擾
	void SetEnableUpdate(const bool bUpdate) override;	// �X�V�󋵐ݒ�
	void SetEnableDraw(const bool bDraw) override;		// �`��󋵐ݒ�

	// �ÓI�����o�֐�
	static CPlayer *Create(void);	// ����

private:
	// �����o�֐�
	void LoadSetup(void);				// �Z�b�g�A�b�v
	void UpdateNormal(int *pMotion);	// �ʏ��Ԏ��̍X�V
	void UpdateMove(int *pMotion);		// �ړ�����E�ڕW�����̍X�V
	void UpdateJump(int *pMotion);		// �W�����v����̍X�V

	void UpdateMotion(int nMotion);			// ���[�V�����̍X�V
	void UpdateOldPosition(void);			// �ߋ��ʒu�̍X�V
	void UpdateGravity(void);				// �d�͂̍X�V
	void UpdateLanding(D3DXVECTOR3 *pPos);	// ���n�󋵂̍X�V
	void UpdatePosition(D3DXVECTOR3 *pPos);	// �ʒu�̍X�V
	void UpdateRotation(D3DXVECTOR3 *pRot);	// �����̍X�V

	// �����o�ϐ�
	std::map<int, std::string> m_modelFileName;	// ���f���t�@�C����
	CShadow		*m_pShadow;			// �e�̏��
	D3DXVECTOR3	m_oldPos;			// �ߋ��ʒu
	D3DXVECTOR3	m_move;				// �ړ���
	D3DXVECTOR3	m_destRot;			// �ڕW����
	EState		m_state;			// ���
	int			m_nCounterState;	// ��ԊǗ��J�E���^�[
	bool		m_bJump;			// �W�����v��
};

#endif	// _PLAYER_H_
