//============================================================
//
//	�t�H���g2D�w�b�_�[ [font2D.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _FONT2D_H_
#define _FONT2D_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object2D.h"
#include "fontChar.h"

//************************************************************
//	�N���X��`
//************************************************************
// �t�H���g2D�N���X
class CFont2D : public CObject2D
{
public:
	// �R���X�g���N�^
	CFont2D();

	// �f�X�g���N�^
	~CFont2D() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(CShader *pShader = nullptr) override;			// �`��
	void SetVec3Sizing(const D3DXVECTOR3& rSize) override;	// �傫���ݒ�
	void SetHeight(const float fHeight) override;			// �c���ݒ�

	// �ÓI�����o�֐�
	static CFont2D *Create	// ����
	( // ����
		CFontChar *pFontChar,		// �t�H���g�������
		const UINT uChar,			// �w�蕶��
		const D3DXVECTOR3& rPos,	// �ʒu
		const float fHeight = 100.0f,			// �c��
		const D3DXVECTOR3& rRot = VEC3_ZERO,	// ����
		const D3DXCOLOR& rCol = XCOL_WHITE		// �F
	);

	// �����o�֐�
	void SetFontChar	// �t�H���g�E�����̐ݒ�
	( // ����
		CFontChar *pFontChar,	// �t�H���g�������
		const UINT uChar		// �w�蕶��
	);
	float GetOffset(void);	// ���_�̃I�t�Z�b�g�擾
	float GetNext(void);	// ���̕����܂ł̋����擾

private:
	// �����o�ϐ�
	CFontChar::SChar m_infoChar;	// �������
	UINT m_uChar;			// �w�蕶��
	float m_fSizeRate;		// �c���̊���
	float m_fAbsOriginX;	// X���_�I�t�Z�b�g�̐�Βl
};

#endif	// _FONT2D_H_
