//============================================================
//
//	������2D�w�b�_�[ [string2D.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _STRING2D_H_
#define _STRING2D_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object.h"

//************************************************************
//	�O���錾
//************************************************************
class CFontChar;	// �t�H���g�����N���X
class CChar2D;		// ����2D�N���X

//************************************************************
//	�N���X��`
//************************************************************
// ������2D�N���X
class CString2D : public CObject
{
public:
	// ���z�u��
	enum EAlignX
	{
		XALIGN_LEFT = 0,	// ������
		XALIGN_CENTER,		// ��������
		XALIGN_RIGHT,		// �E����
		XALIGN_MAX,			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CString2D();

	// �f�X�g���N�^
	~CString2D() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(CShader *pShader = nullptr) override;			// �`��
	void SetVec3Position(const D3DXVECTOR3& rPos) override;	// �ʒu�ݒ�
	D3DXVECTOR3 GetVec3Position(void) const override;		// �ʒu�擾
	void SetVec3Rotation(const D3DXVECTOR3& rRot) override;	// �����ݒ�
	D3DXVECTOR3 GetVec3Rotation(void) const override;		// �����擾
	void SetColor(const D3DXCOLOR& rCol) override;			// �F�ݒ�
	D3DXCOLOR GetColor(void) const override;				// �F�擾
	void SetHeight(const float fHeight) override;			// �c���ݒ�
	float GetHeight(void) const override;					// �c���擾

	// �ÓI�����o�֐�
	static CString2D *Create	// ����
	( // ����
		CFontChar *pFontChar,		// �t�H���g�������
		const std::wstring &rStr,	// �w�蕶����
		const D3DXVECTOR3 &rPos,	// ���_�ʒu
		const float fHeight = 100.0f,			// �����c��
		const EAlignX alignX = XALIGN_CENTER,	// ���z�u
		const D3DXVECTOR3& rRot = VEC3_ZERO,	// ���_����
		const D3DXCOLOR& rCol = XCOL_WHITE		// �F
	);

	// �����o�֐�
	HRESULT SetFontString	// �t�H���g�E������̐ݒ�
	( // ����
		CFontChar *pFontChar,		// �t�H���g�������
		const std::wstring &rStr	// �w�蕶����
	);
	CChar2D *GetChar2D(const int nCharID) const;	// �����̎擾
	float GetStrWidth(void) const;					// ������̉����擾
	void SetAlignX(const EAlignX align);			// ���z�u�ݒ�
	EAlignX GetAlignX(void) const;					// ���z�u�擾

private:
	// �I�[�o�[���C�h�֐�
	void Release(void) override;	// �j��

	// �����o�֐�
	void SetPositionRelative(void);	// ���Έʒu�ݒ�

	// �����o�ϐ�
	CChar2D **m_ppChar;		// �����|���S���̏��
	D3DXVECTOR3 m_pos;		// �ʒu
	EAlignX m_alignX;		// ���z�u
	std::wstring m_wsStr;	// �w�蕶����
};

#endif	// _STRING2D_H_
