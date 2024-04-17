//============================================================
//
//	�t�H���g2D���� [font2D.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "font2D.h"
#include "manager.h"
#include "texture.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int PRIORITY = 6;	// �t�H���g2D�̗D�揇��
}

//************************************************************
//	�q�N���X [CFont2D] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CFont2D::CFont2D() : CObject2D(CObject::LABEL_UI, CObject::DIM_2D, PRIORITY),
	m_uChar			(0),	// �w�蕶��
	m_fSizeRate		(0.0f),	// �c���̊���
	m_fAbsOriginX	(0.0f)	// X���_�I�t�Z�b�g�̐�Βl
{
	// �����o�ϐ����N���A
	memset(&m_infoChar, 0, sizeof(m_infoChar));	// �������
}

//============================================================
//	�f�X�g���N�^
//============================================================
CFont2D::~CFont2D()
{

}

//============================================================
//	����������
//============================================================
HRESULT CFont2D::Init(void)
{
	// �����o�ϐ���������
	memset(&m_infoChar, 0, sizeof(m_infoChar));	// �������
	m_uChar			= 0;	// �w�蕶��
	m_fSizeRate		= 1.0f;	// �c���̊���
	m_fAbsOriginX	= 0.0f;	// X���_�I�t�Z�b�g�̐�Βl

	// �I�u�W�F�N�g2D�̏�����
	if (FAILED(CObject2D::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CFont2D::Uninit(void)
{
	// �I�u�W�F�N�g2D�̏I��
	CObject2D::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CFont2D::Update(void)
{
	// �I�u�W�F�N�g2D�̍X�V
	CObject2D::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CFont2D::Draw(CShader *pShader)
{
	// �I�u�W�F�N�g2D�̕`��
	CObject2D::Draw(pShader);
}

//============================================================
//	�傫���̐ݒ菈��
//============================================================
void CFont2D::SetVec3Sizing(const D3DXVECTOR3& /*rSize*/)
{
	/*
		�������ő傫�������R�ɕς������
		�����̔䗦�����������Ȃ�̂ƁA
		�c���̊����Ȃǂ��ύX�ł��Ȃ�����
		�傫���ύX�� SetHeight �𗘗p���ĂˁB
	*/

	assert(false);
}

//============================================================
//	�c���̐ݒ菈��
//============================================================
void CFont2D::SetHeight(const float fHeight)
{
	int nTexID = GetTextureIndex();	// �t�H���g�̃e�N�X�`���C���f�b�N�X
	float fTexWidth = useful::GetTexWidthFromAspect(fHeight, nTexID);			// �e�N�X�`������
	D3DXIMAGE_INFO status = GET_MANAGER->GetTexture()->GetInfo(nTexID).status;	// �e�N�X�`���X�e�[�^�X

	// �c���̑���������ۑ�
	m_fSizeRate = fHeight / (float)status.Height;

	// �傫����ݒ�
	CObject2D::SetVec3Sizing(D3DXVECTOR3(fTexWidth, fHeight, 0.0f));
}

//============================================================
//	��������
//============================================================
CFont2D *CFont2D::Create
(
	CFontChar *pFontChar,		// �t�H���g�������
	const UINT uChar,			// �w�蕶��
	const D3DXVECTOR3& rPos,	// �ʒu
	const float fHeight,		// �c��
	const D3DXVECTOR3& rRot,	// ����
	const D3DXCOLOR& rCol		// �F
)
{
	// �t�H���g2D�̐���
	CFont2D *pFont2D = new CFont2D;
	if (pFont2D == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �t�H���g2D�̏�����
		if (FAILED(pFont2D->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �t�H���g2D�̔j��
			SAFE_DELETE(pFont2D);
			return nullptr;
		}

		// �t�H���g�E������ݒ�
		pFont2D->SetFontChar(pFontChar, uChar);

		// �ʒu��ݒ�
		pFont2D->SetVec3Position(rPos);

		// ������ݒ�
		pFont2D->SetVec3Rotation(rRot);

		// �c����ݒ�
		pFont2D->SetHeight(fHeight);

		// �F��ݒ�
		pFont2D->SetColor(rCol);

		// �m�ۂ����A�h���X��Ԃ�
		return pFont2D;
	}
}

//============================================================
//	�t�H���g�E�����̐ݒ菈��
//============================================================
void CFont2D::SetFontChar
(
	CFontChar *pFontChar,	// �t�H���g�������
	const UINT uChar		// �w�蕶��
)
{
	// �t�H���g�����̎擾
	m_infoChar = pFontChar->Regist(uChar);

	// �w�蕶����ۑ�
	m_uChar = uChar;

	// X���_�I�t�Z�b�g�̐�Βl��ۑ�
	m_fAbsOriginX = fabsf((float)m_infoChar.glyph.gmptGlyphOrigin.x);

	// �e�N�X�`�����쐬�E����
	BindTexture(m_infoChar.nTexID);
}

//============================================================
//	���_�̃I�t�Z�b�g�擾����
//============================================================
float CFont2D::GetOffset(void)
{
	// �����u���b�N�{�b�N�X�̉���
	float fBlackBoxX = (float)m_infoChar.glyph.gmBlackBoxX;

	// �������_�̃I�t�Z�b�g��Ԃ�
	return (m_fAbsOriginX + fBlackBoxX * 0.5f) * m_fSizeRate;
}

//============================================================
//	���̕����܂ł̋����擾����
//============================================================
float CFont2D::GetNext(void)
{
	// ���̕����̋�����Ԃ�
	return (float)m_infoChar.glyph.gmCellIncX * m_fSizeRate;
}
