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
//	�C���N���[�h�t�@�C��
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
	// ���_��
	enum EOrigin
	{
		ORIGIN_LEFT = 0,	// �����_
		ORIGIN_CENTER,		// ���S���_
		ORIGIN_RIGHT,		// �E���_
		ORIGIN_MAX,			// ���̗񋓌^�̑���
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
	void SetHeight(const float fHeight) override;			// �c���ݒ�
	float GetHeight(void) const override;					// �c���擾

	// �ÓI�����o�֐�
	static CString2D *Create	// ����
	( // ����
		CFontChar *pFontChar,		// �t�H���g�������
		std::wstring wsStr,			// �w�蕶����
		const D3DXVECTOR3 &rPos,	// ���_�ʒu
		const float fHeight,		// �����c��
		const EOrigin origin = ORIGIN_CENTER	// ���_
	);

	// �����o�֐�
	HRESULT SetFontString	// �t�H���g�E������̐ݒ�
	( // ����
		CFontChar *pFontChar,	// �t�H���g�������
		std::wstring wsStr		// �w�蕶����
	);
	float GetStrWidth(void);	// ������̉����擾
	void SetOrigin(const EOrigin origin);	// ���_�ݒ�
	EOrigin GetOrigin(void) const;			// ���_�擾

private:
	// �I�[�o�[���C�h�֐�
	void Release(void) override;	// �j��

	// �����o�֐�
	void SetPositionRelative(void);	// ���Έʒu�ݒ�

	// �����o�ϐ�
	CChar2D **m_ppChar;		// �����|���S���̏��
	D3DXVECTOR3 m_pos;		// �ʒu
	EOrigin m_origin;		// ���_
	std::wstring m_wsStr;	// �w�蕶����
};

#endif	// _STRING2D_H_
