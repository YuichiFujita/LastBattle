//============================================================
//
//	���w�b�_�[ [sword.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _SWORD_H_
#define _SWORD_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "multiModel.h"

//************************************************************
//	�O���錾
//************************************************************
class COrbit;	// �O�ՃN���X

//************************************************************
//	�N���X��`
//************************************************************
// ���N���X
class CSword : public CMultiModel
{
public:
	// ���f����
	enum EModel
	{
		MODEL_SWORD = 0,	// ��
		MODEL_MAX			// ���̗񋓌^�̑���
	};

	// ��ԗ�
	enum EState
	{
		STATE_NONE = 0,	// �������Ȃ����
		STATE_NORMAL,	// �ʏ���
		STATE_VANISH,	// �������
		STATE_MAX		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CSword();

	// �f�X�g���N�^
	~CSword() override;

	// ����\����
	struct SColl
	{
		D3DXVECTOR3 offset;	// ����I�t�Z�b�g
		float fRadius;		// ���蔼�a
	};

	// ������\����
	struct SCollInfo
	{
		int nNumColl;	// ���萔
		SColl *pColl;	// ������
	};

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;		// ������
	void Uninit(void) override;			// �I��
	void Update(void) override;			// �X�V
	void Draw(void) override;			// �`��
	int GetState(void) const override;	// ��Ԏ擾

	// �ÓI�����o�֐�
	static void LoadSetup(void);	// �Z�b�g�A�b�v
	static CSword *Create	// ����
	( // ����
		CObject *pObject,						// �e�I�u�W�F�N�g
		const D3DXVECTOR3 &rPos = VEC3_ZERO,	// �ʒu
		const D3DXVECTOR3 &rRot = VEC3_ZERO		// ����
	);

	// �����o�֐�
	void SetState(const EState state);	// ��Ԑݒ�

private:
	// �ÓI�����o�ϐ�
	static SCollInfo m_collInfo;	// ������

	// �����o�ϐ�
	COrbit	*m_pOrbit;			// �O�Ղ̏��
	EState	m_state;			// ���
	int		m_nCounterState;	// ��ԊǗ��J�E���^�[
	bool	m_bDisp;			// �\����
};

#endif	// _SWORD_H_
