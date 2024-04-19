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
		const float fHeight			// �����c��
	);

	// �����o�֐�
	HRESULT SetFontString	// �t�H���g�E������̐ݒ�
	( // ����
		CFontChar *pFontChar,	// �t�H���g�������
		std::wstring wsStr,		// �w�蕶����
		const float fHeight		// �����c��
	);

private:
	// �I�[�o�[���C�h�֐�
	void Release(void) override;	// �j��

	// �����o�ϐ�
	CChar2D **m_ppChar;		// �����|���S���̏��
	D3DXVECTOR3 m_pos;		// ������̌��_�ʒu
	std::wstring m_wsStr;	// �w�蕶����
};

#endif	// _STRING2D_H_
