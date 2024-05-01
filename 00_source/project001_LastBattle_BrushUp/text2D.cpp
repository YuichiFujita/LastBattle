//============================================================
//
//	�e�L�X�g2D���� [text2D.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "text2D.h"
#include "manager.h"
#include "texture.h"
#include "fontChar.h"
#include "char2D.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int PRIORITY = 6;	// �e�L�X�g2D�̗D�揇��
}

//************************************************************
//	�q�N���X [CText2D] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CText2D::CText2D() : CObject(CObject::LABEL_UI, CObject::DIM_2D, PRIORITY),
	m_pFontChar		(nullptr),					// �t�H���g����
	m_pos			(VEC3_ZERO),				// �ʒu
	m_rot			(VEC3_ZERO),				// ����
	m_col			(XCOL_WHITE),				// �F
	m_alignX		(CString2D::XALIGN_CENTER),	// ���z�u
	m_alignY		(YALIGN_CENTER),			// �c�z�u
	m_fCharHeight	(0.0f),						// �����̏c��
	m_fLineHeight	(0.0f)						// �����̍s��
{
	// �����񃊃X�g���N���A
	m_listString.clear();
}

//============================================================
//	�f�X�g���N�^
//============================================================
CText2D::~CText2D()
{

}

//============================================================
//	����������
//============================================================
HRESULT CText2D::Init(void)
{
	// �����o�ϐ���������
	m_pFontChar		= nullptr;					// �t�H���g����
	m_pos			= VEC3_ZERO;				// �ʒu
	m_rot			= VEC3_ZERO;				// ����
	m_col			= XCOL_WHITE;				// �F
	m_alignX		= CString2D::XALIGN_CENTER;	// ���z�u
	m_alignY		= YALIGN_CENTER;			// �c�z�u
	m_fCharHeight	= 0.0f;						// �����̏c��
	m_fLineHeight	= 0.0f;						// �����̍s��

	// �����񃊃X�g��������
	m_listString.clear();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CText2D::Uninit(void)
{
	for (auto& rList : m_listString)
	{ // ������̊i�[�����J��Ԃ�

		// ������̏I��
		SAFE_UNINIT(rList);
	}

	// �����񃊃X�g���N���A
	m_listString.clear();

	// �I�u�W�F�N�g��j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CText2D::Update(void)
{
	// ���Έʒu�̐ݒ�
	SetPositionRelative();
}

//============================================================
//	�`�揈��
//============================================================
void CText2D::Draw(CShader * /*pShader*/)
{

}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CText2D::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// �����̈ʒu��ݒ�
	m_pos = rPos;

	// ���Έʒu�̐ݒ�
	SetPositionRelative();
}

//============================================================
//	�ʒu�̎擾����
//============================================================
D3DXVECTOR3 CText2D::GetVec3Position(void) const
{
	// �ʒu��Ԃ�
	return m_pos;
}

//============================================================
//	�����̐ݒ菈��
//============================================================
void CText2D::SetVec3Rotation(const D3DXVECTOR3& rRot)
{
	// �ݒ肷�������ۑ�
	m_rot = rRot;

	for (auto& rList : m_listString)
	{ // ������̊i�[�����J��Ԃ�

		// ����������̐ݒ�
		assert(rList != nullptr);
		rList->SetVec3Rotation(rRot);
	}

	// ���Έʒu�̐ݒ�
	SetPositionRelative();
}

//============================================================
//	�����擾����
//============================================================
D3DXVECTOR3 CText2D::GetVec3Rotation(void) const
{
	// �ۑ����ꂽ������Ԃ�
	return m_rot;
}

//============================================================
//	�F�̐ݒ菈��
//============================================================
void CText2D::SetColor(const D3DXCOLOR& rCol)
{
	// �ݒ肷��F��ۑ�
	m_col = rCol;

	for (auto& rList : m_listString)
	{ // ������̊i�[�����J��Ԃ�

		// ������F�̐ݒ�
		assert(rList != nullptr);
		rList->SetColor(rCol);
	}
}

//============================================================
//	�F�擾����
//============================================================
D3DXCOLOR CText2D::GetColor(void) const
{
	// �ۑ����ꂽ�F��Ԃ�
	return m_col;
}

//============================================================
//	��������
//============================================================
CText2D *CText2D::Create
(
	CFontChar *pFontChar,				// �t�H���g�������
	const D3DXVECTOR3& rPos,			// ���_�ʒu
	const float fCharHeight,			// �����c��
	const float fLineHeight,			// �s�ԏc��
	const CString2D::EAlignX alignX,	// ���z�u
	const EAlignY alignY,				// �c�z�u
	const D3DXVECTOR3& rRot,			// ���_����
	const D3DXCOLOR& rCol				// �F
)
{
	// �e�L�X�g2D�̐���
	CText2D *pText2D = new CText2D;
	if (pText2D == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �e�L�X�g2D�̏�����
		if (FAILED(pText2D->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �e�L�X�g2D�̔j��
			SAFE_DELETE(pText2D);
			return nullptr;
		}

		// �t�H���g��ݒ�
		pText2D->SetFont(pFontChar);

		// ���_�ʒu��ݒ�
		pText2D->SetVec3Position(rPos);

		// ���_������ݒ�
		pText2D->SetVec3Rotation(rRot);

		// �F��ݒ�
		pText2D->SetColor(rCol);

		// �����c����ݒ�
		pText2D->SetCharHeight(fCharHeight);

		// �s�ԏc����ݒ�
		pText2D->SetLineHeight(fLineHeight);

		// ���z�u��ݒ�
		pText2D->SetAlignX(alignX);

		// �c�z�u��ݒ�
		pText2D->SetAlignY(alignY);

		// �m�ۂ����A�h���X��Ԃ�
		return pText2D;
	}
}

//============================================================
//	�t�H���g�̐ݒ菈��
//============================================================
void CText2D::SetFont(CFontChar *pFontChar)
{
	// �����̃t�H���g��ۑ�
	assert(pFontChar != nullptr);
	m_pFontChar = pFontChar;
}

//============================================================
//	������̒ǉ�����
//============================================================
HRESULT CText2D::AddString(const std::wstring& rStr)
{
	// ������I�u�W�F�N�g�𐶐�
	CString2D *pStr = CString2D::Create
	( // ����
		m_pFontChar,	// �t�H���g�������
		rStr,			// �w�蕶����
		m_pos,			// ���_�ʒu
		m_fCharHeight,	// �����c��
		m_alignX,		// ���z�u
		m_rot,			// ���_����
		m_col			// �F
	);
	if (pStr == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �Ō���ɐ��������������ǉ�
	m_listString.push_back(pStr);

	// ���Έʒu�̐ݒ�
	SetPositionRelative();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	������̎擾����
//============================================================
CString2D *CText2D::GetString2D(const int nStrID) const
{
	// �����񂪂Ȃ��ꍇ������
	int nStrSize = (int)m_listString.size();
	if (nStrSize <= 0) { assert(false); return nullptr; }

	// �C���f�b�N�X���͈͊O�̏ꍇ������
	if (nStrID <= NONE_IDX || nStrID >= nStrSize) { assert(false); return nullptr; }

	// �����C���f�b�N�X�̕������Ԃ�
	auto itr = m_listString.begin();	// �擪�C�e���[�^�[
	std::advance(itr, nStrID);			// �C�e���[�^�[��i�߂�
	return *itr;						// �C�e���[�^�[�̒��g��Ԃ�
}

//============================================================
//	�e�L�X�g�̏c���擾����
//============================================================
float CText2D::GetTextHeight(void) const
{
	// �����񂪂Ȃ��ꍇ������
	if ((int)m_listString.size() <= 0) { assert(false); return 0.0f; }

	float fTextHeight = 0.0f;	// �e�L�X�g�̏c��
	for (auto& rList : m_listString)
	{ // ������̊i�[�����J��Ԃ�

		// ������c�������Z
		assert(rList != nullptr);
		fTextHeight += rList->GetHeight();
	}

	// �e�L�X�g���̑S�s�ԕ������Z
	fTextHeight += m_fLineHeight * ((int)m_listString.size() - 1);

	// �e�L�X�g�̏c����Ԃ�
	return fTextHeight;
}

//============================================================
//	�����c���̐ݒ菈��
//============================================================
void CText2D::SetCharHeight(const float fHeight)
{
	// �ݒ肷�镶���̏c����ۑ�
	m_fCharHeight = fHeight;

	for (auto& rList : m_listString)
	{ // ������̊i�[�����J��Ԃ�

		// ������c���̐ݒ�
		assert(rList != nullptr);
		rList->SetHeight(fHeight);
	}

	// ���Έʒu�̐ݒ�
	SetPositionRelative();
}

//============================================================
//	�����c���̎擾����
//============================================================
float CText2D::GetCharHeight(void) const
{
	// �ۑ����ꂽ�����̏c����Ԃ�
	return m_fCharHeight;
}

//============================================================
//	�s�ԏc���̐ݒ菈��
//============================================================
void CText2D::SetLineHeight(const float fHeight)
{
	// �����̍s�Ԃ�ݒ�
	m_fLineHeight = fHeight;

	// ���Έʒu�̐ݒ�
	SetPositionRelative();
}

//============================================================
//	�s�ԏc���̎擾����
//============================================================
float CText2D::GetLineHeight(void) const
{
	// �s�Ԃ�Ԃ�
	return m_fLineHeight;
}

//============================================================
//	���z�u�̐ݒ菈��
//============================================================
void CText2D::SetAlignX(const CString2D::EAlignX alignX)
{
	// �����̉��z�u��ݒ�
	m_alignX = alignX;

	for (auto& rList : m_listString)
	{ // ������̊i�[�����J��Ԃ�

		// �����񉡕��̐ݒ�
		assert(rList != nullptr);
		rList->SetAlignX(alignX);
	}

	// ���Έʒu�̐ݒ�
	SetPositionRelative();
}

//============================================================
//	���z�u�̎擾����
//============================================================
CString2D::EAlignX CText2D::GetAlignX(void) const
{
	// ���z�u��Ԃ�
	return m_alignX;
}

//============================================================
//	�c�z�u�̐ݒ菈��
//============================================================
void CText2D::SetAlignY(const EAlignY alignY)
{
	// �����̏c�z�u��ݒ�
	m_alignY = alignY;

	// ���Έʒu�̐ݒ�
	SetPositionRelative();
}

//============================================================
//	�c�z�u�̎擾����
//============================================================
CText2D::EAlignY CText2D::GetAlignY(void) const
{
	// �c�z�u��Ԃ�
	return m_alignY;
}

//============================================================
//	�j������
//============================================================
void CText2D::Release(void)
{
	// �I�u�W�F�N�g�̔j��
	CObject::Release();
}

//============================================================
//	���Έʒu�̐ݒ菈��
//============================================================
void CText2D::SetPositionRelative(void)
{
	// �����񂪂Ȃ��ꍇ������
	if ((int)m_listString.size() <= 0) { return; }

	float fTextHeight	= GetTextHeight() * 0.5f;	// �e�L�X�g�S�̂̏c��
	float fFrontHeight	= m_listString.front()->GetHeight() * 0.5f;	// �擪������̏c��
	float fStartOffset	= -fTextHeight + fFrontHeight - (fTextHeight * (m_alignY - 1));	// ������̊J�n�ʒu�I�t�Z�b�g

	D3DXVECTOR3 posStart;	// �����̊J�n�ʒu
	posStart.x = m_pos.x + sinf(m_rot.z) * fStartOffset;	// �J�n�ʒuX
	posStart.y = m_pos.y + cosf(m_rot.z) * fStartOffset;	// �J�n�ʒuY

	for (auto& rList : m_listString)
	{ // ������̊i�[�����J��Ԃ�

		// �ʒu�𔽉f
		assert(rList != nullptr);
		rList->SetVec3Position(posStart);

		// �ݒ���W�ɂ�^����
		posStart.x += sinf(m_rot.z) * (m_fLineHeight + (rList->GetHeight()));
		posStart.y += cosf(m_rot.z) * (m_fLineHeight + (rList->GetHeight()));
	}
}
