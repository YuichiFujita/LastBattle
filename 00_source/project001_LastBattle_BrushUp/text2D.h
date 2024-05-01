//============================================================
//
//	�e�L�X�g2D�w�b�_�[ [text2D.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _TEXT2D_H_
#define _TEXT2D_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object.h"
#include "string2D.h"

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
class CFontChar;	// �t�H���g�����N���X

//************************************************************
//	�N���X��`
//************************************************************
// �e�L�X�g2D�N���X
class CText2D : public CObject
{
public:
	// �c�z�u��
	enum EAlignY
	{
		YALIGN_TOP = 0,	// �㑵��
		YALIGN_CENTER,	// ��������
		YALIGN_BOTTOM,	// ������
		YALIGN_MAX,		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CText2D();

	// �f�X�g���N�^
	~CText2D() override;

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

	// �ÓI�����o�֐�
	static CText2D *Create	// ����
	( // ����
		CFontChar *pFontChar,		// �t�H���g�������
		const D3DXVECTOR3& rPos,	// ���_�ʒu
		const float fCharHeight,	// �����c��
		const float fLineHeight,	// �s�ԏc��
		const CString2D::EAlignX alignX = CString2D::XALIGN_CENTER,	// ���z�u
		const EAlignY alignY = YALIGN_CENTER,	// �c�z�u
		const D3DXVECTOR3& rRot = VEC3_ZERO,	// ���_����
		const D3DXCOLOR& rCol = XCOL_WHITE		// �F
	);

	// �����o�֐�
	void SetFont(CFontChar *pFontChar);				// �t�H���g�̐ݒ�
	HRESULT AddString(const std::wstring& rStr);	// ������̒ǉ�
	CString2D *GetString2D(const int nStrID) const;	// ������̎擾
	float GetTextHeight(void) const;				// �e�L�X�g�̏c���擾
	void SetCharHeight(const float fHeight);		// �����̏c���ݒ�
	float GetCharHeight(void) const;				// �����̏c���擾
	void SetLineHeight(const float fHeight);		// �s�Ԃ̏c���ݒ�
	float GetLineHeight(void) const;				// �s�Ԃ̏c���擾
	void SetAlignX(const CString2D::EAlignX align);	// ���z�u�ݒ�
	CString2D::EAlignX GetAlignX(void) const;		// ���z�u�擾
	void SetAlignY(const EAlignY align);			// �c�z�u�ݒ�
	EAlignY GetAlignY(void) const;					// �c�z�u�擾

private:
	// �I�[�o�[���C�h�֐�
	void Release(void) override;	// �j��

	// �����o�֐�
	void SetPositionRelative(void);	// ���Έʒu�ݒ�

	// �����o�ϐ�
	std::list<CString2D*> m_listString;	// �����񃊃X�g
	CFontChar *m_pFontChar;				// �t�H���g����
	D3DXVECTOR3 m_pos;					// �ʒu
	D3DXVECTOR3 m_rot;					// ����
	D3DXCOLOR m_col;					// �F
	CString2D::EAlignX m_alignX;		// ���z�u
	EAlignY m_alignY;					// �c�z�u
	float m_fCharHeight;				// �����̏c��
	float m_fLineHeight;				// �����̍s��
};

#endif	// _TEXT2D_H_
