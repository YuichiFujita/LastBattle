//============================================================
//
//	�u���[�w�b�_�[ [blur.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _BLUR_H_
#define _BLUR_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object.h"
#include "motion.h"

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
class CObjectModel;	// �I�u�W�F�N�g���f���N���X

//************************************************************
//	�N���X��`
//************************************************************
// �u���[�N���X
class CBlur : public CObject
{
public:
	// ��ԗ�
	enum EState
	{
		STATE_NONE = 0,	// �������Ȃ����
		STATE_NORMAL,	// �ʏ���
		STATE_VANISH,	// �������
		STATE_MAX		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CBlur();

	// �f�X�g���N�^
	~CBlur();

	// �u���[���\����
	struct SInfo
	{
		CObjectModel *apCharaParts[motion::MAX_PARTS];	// �p�[�c�̏��
		int nNumParts;	// �p�[�c��
	};

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(CShader *pShader = nullptr) override;	// �`��
	int GetState(void) const override;				// ��Ԏ擾

	// �ÓI�����o�֐�
	static CBlur *Create	// ����
	( // ����
		CObjectChara *pParent,		// �e�I�u�W�F�N�g
		D3DXMATERIAL& rMat,			// �u���[�}�e���A��
		const float fStartAlpha,	// �u���[�J�n�����x
		const int nMaxLength		// �ێ��I�u�W�F�N�g�ő吔
	);

	// �����o�֐�
	void SetState(const EState state);	// ��Ԑݒ�
	void SetMaterial(const D3DXMATERIAL& rMat)	{ m_mat = rMat; }	// �u���[�}�e���A���ݒ�
	D3DXMATERIAL GetMaterial(void) const		{ return m_mat; }	// �u���[�}�e���A���擾
	void SetStartAlpha(const float fStartAlpha)	{ m_fStartAlpha = fStartAlpha; }	// �u���[�̊J�n�����x�ݒ�
	float GetStartAlpha(void) const				{ return m_fStartAlpha; }			// �u���[�̊J�n�����x�擾
	void SetMaxLength(const int nMaxLength)		{ m_nMaxLength = nMaxLength; }		// �ێ��I�u�W�F�N�g���ݒ�
	int GetMaxLength(void) const				{ return m_nMaxLength; }			// �ێ��I�u�W�F�N�g���擾

private:
	// �I�[�o�[���C�h�֐�
	void Release(void) override;	// �j��

	// �����o�ϐ�
	std::vector<SInfo*> m_oldObject;	// �ߋ��I�u�W�F�N�g���
	CObjectChara *m_pParent;	// �e�I�u�W�F�N�g
	D3DXMATERIAL m_mat;			// �u���[���f�}�e���A��
	float	m_fStartAlpha;		// �u���[�̊J�n�����x
	int		m_nMaxLength;		// �ێ�����e�I�u�W�F�N�g�̍ő吔
	EState	m_state;			// ���
	int		m_nCounterState;	// ��ԊǗ��J�E���^�[

};

#endif	// _BLUR_H_
