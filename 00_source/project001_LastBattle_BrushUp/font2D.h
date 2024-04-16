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
	explicit CFont2D(const CObject::ELabel label = LABEL_NONE, const EDim dimension = DIM_2D, const int nPriority = object::DEFAULT_PRIO);

	// �f�X�g���N�^
	~CFont2D() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(CShader *pShader = nullptr) override;			// �`��
	void SetVec3Position(const D3DXVECTOR3& rPos) override;	// �ʒu�ݒ�
	void SetVec3Rotation(const D3DXVECTOR3& rRot) override;	// �����ݒ�
	void SetVec3Sizing(const D3DXVECTOR3& rSize) override;	// �傫���ݒ�
	void SetColor(const D3DXCOLOR& rCol) override;			// �F�ݒ�
	void SetScale(const float fScale) override;				// �g�嗦�ݒ�

	// �ÓI�����o�֐�
	static CFont2D *Create	// ����
	( // ����
		CFontChar *pFontChar,		// �t�H���g�������
		const UINT uChar,			// �w�蕶��
		const D3DXVECTOR3& rPos,	// �ʒu
		const float fScale = 1.0f,				// �g�嗦
		const D3DXVECTOR3& rRot = VEC3_ZERO,	// ����
		const D3DXCOLOR& rCol = XCOL_WHITE		// �F
	);

	// �����o�֐�
	void SetFontChar	// �t�H���g�E�����̐ݒ�
	( // ����
		CFontChar *pFontChar,	// �t�H���g�������
		const UINT uChar		// �w�蕶��
	);

	// ���_���̂��炷�����擾
	float GetOffset(void)
	{
		float fOrigin = (float)m_infoChar.glyph.gmptGlyphOrigin.x;
		if (fOrigin < 0.0f) { fOrigin *= -1.0f; }
		float fBlack = (float)m_infoChar.glyph.gmBlackBoxX;
		return (fOrigin + fBlack * 0.5f);
	}

	// ���̕����܂ł̋����擾
	float GetNext(void)
	{
		float fNext = (float)m_infoChar.glyph.gmCellIncX;
		return fNext;
	}

private:
	// �����o�ϐ�
	CFontChar::SChar m_infoChar;	// �������
	float m_fScale;	// �g�嗦
};

#endif	// _FONT2D_H_
