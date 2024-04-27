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
	const int NUM_THUNDER	= 3;	// ���̐�����
	const int WARN_FRAME	= 80;	// �x���\�������t���[��
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
	HRESULT Init(void) override;		// ������
	void Uninit(void) override;			// �I��
	void Update(void) override;			// �X�V
	void UpdateMatrix(void) override;	// �}�g���b�N�X�X�V
	void Draw(void) override;			// �`��

	// �ÓI�����o�֐�
	static CAttackThunder *Create	// ����
	( // ����
		const D3DXVECTOR3& rPos,							// �ʒu
		const bool bSound,									// ���ʉ��Đ��t���O
		const int nWarnFrame = attackThunder::WARN_FRAME	// �x���\���t���[����
	);
	static CListManager<CAttackThunder> *GetList(void);		// ���X�g�擾

private:
	// �I�[�o�[���C�h�֐�
	void Release(void) override;	// �j��

	// �����o�֐�
	void SetOriginPosition(const D3DXVECTOR3 &rPos);	// ���_�ʒu�ݒ�
	void SetThunderPosition(const D3DXVECTOR3 &rPos);	// ���ʒu�ݒ�
	HRESULT SetAttack(void);		// �U����Ԃ̐ݒ�
	void CollisionPlayer(void);		// �v���C���[�Ƃ̓����蔻��

	// �ÓI�����o�ϐ�
	static CListManager<CAttackThunder> *m_pList;	// �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CAttackThunder>::AIterator m_iterator;	// �C�e���[�^�[
	CThunder *m_apThunder[attackThunder::NUM_THUNDER];	// ���̏��
	CObject3D *m_pWarning;		// �x���\���̏��
	D3DXVECTOR3 m_posOrigin;	// ���̌��_�ʒu
	EState m_state;				// ���
	int m_nCounterState;		// ��ԊǗ��J�E���^�[
	int m_nWarnFrame;			// �x���\���t���[����
	bool m_bSound;				// ���ʉ��Đ��t���O
};

#endif	// _ATTACK_THUNDER_H_
