//============================================================
//
//	���U���w�b�_�[ [attackThunder.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _ATTACK_THUNDER_H_
#define _ATTACK_THUNDER_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace attackThunder
{
	const int NUM_THUNDER = 5;	// ���̐�����
}

//************************************************************
//	�O���錾
//************************************************************
class CThunder;		// ���N���X
class CObject3D;	// �I�u�W�F�N�g3D�N���X

//************************************************************
//	�N���X��`
//************************************************************
// ���U���N���X
class CAttackThunder : public CObject
{
public:
	// ��ԗ�
	enum EState
	{
		STATE_WARN = 0,	// �x���\��
		STATE_ATTACK,	// �U���J�n
		STATE_MAX		// ���̗񋓂̑���
	};

	// �R���X�g���N�^
	CAttackThunder();

	// �f�X�g���N�^
	~CAttackThunder() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��

	// �ÓI�����o�֐�
	static CAttackThunder *Create(const D3DXVECTOR3& rPos);	// ����

private:
	// �I�[�o�[���C�h�֐�
	void Release(void) override;	// �j��
	void SetVec3Position(const D3DXVECTOR3 &rPos) override;	// �ʒu�ݒ�

	// �����o�֐�
	HRESULT SetAttack(void);	// �U����Ԃ̐ݒ�
	void CollisionPlayer(void);	// �v���C���[�Ƃ̓����蔻��

	// �����o�ϐ�
	CThunder *m_apThunder[attackThunder::NUM_THUNDER];	// ���̏��
	CObject3D *m_pWarning;		// �x���\���̏��
	D3DXVECTOR3 m_posOrigin;	// ���̌��_�ʒu
	EState m_state;				// ���
	int m_nCounterState;		// ��ԊǗ��J�E���^�[
};

#endif	// _ATTACK_THUNDER_H_
