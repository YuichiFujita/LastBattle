//============================================================
//
//	������2D���� [string2D.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "string2D.h"
#include "manager.h"
#include "texture.h"
#include "fontChar.h"
#include "char2D.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int PRIORITY = 6;	// ������2D�̗D�揇��
}

//************************************************************
//	�q�N���X [CString2D] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CString2D::CString2D() : CObject(CObject::LABEL_UI, CObject::DIM_2D, PRIORITY),
	m_ppChar	(nullptr),		// �����|���S���̏��
	m_pos		(VEC3_ZERO),	// �ʒu
	m_alignX	(XALIGN_CENTER)	// ���z�u
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CString2D::~CString2D()
{

}

//============================================================
//	����������
//============================================================
HRESULT CString2D::Init(void)
{
	// �����o�ϐ���������
	m_ppChar	= nullptr;			// �����|���S���̏��
	m_pos		= VEC3_ZERO;		// �ʒu
	m_alignX	= XALIGN_CENTER;	// ���z�u

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CString2D::Uninit(void)
{
	for (int i = 0; i < (int)m_wsStr.size(); i++)
	{ // ���������J��Ԃ�

		// ������j��
		SAFE_UNINIT(m_ppChar[i]);
	}

	// �������j��
	SAFE_DEL_ARRAY(m_ppChar);

	// �I�u�W�F�N�g��j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CString2D::Update(void)
{
	// TODO
#if 1
	if (GET_INPUTKEY->IsPress(DIK_W))
	{
		m_pos.y -= 1.0f;
	}
	if (GET_INPUTKEY->IsPress(DIK_S))
	{
		m_pos.y += 1.0f;
	}
	if (GET_INPUTKEY->IsPress(DIK_A))
	{
		m_pos.x -= 1.0f;
	}
	if (GET_INPUTKEY->IsPress(DIK_D))
	{
		m_pos.x += 1.0f;
	}
#endif

	// ���Έʒu�̐ݒ�
	SetPositionRelative();
}

//============================================================
//	�`�揈��
//============================================================
void CString2D::Draw(CShader * /*pShader*/)
{

}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CString2D::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// �����̈ʒu��ݒ�
	m_pos = rPos;

	// ���Έʒu�̐ݒ�
	SetPositionRelative();
}

//============================================================
//	�ʒu�̎擾����
//============================================================
D3DXVECTOR3 CString2D::GetVec3Position(void) const
{
	// �ʒu��Ԃ�
	return m_pos;
}

//============================================================
//	�����̐ݒ菈��
//============================================================
void CString2D::SetVec3Rotation(const D3DXVECTOR3& rRot)
{
	for (int i = 0; i < (int)m_wsStr.size(); i++)
	{ // ���������J��Ԃ�

		// ���������̐ݒ�
		assert(m_ppChar[i] != nullptr);
		m_ppChar[i]->SetVec3Rotation(rRot);
	}

	// ���Έʒu�̐ݒ�
	SetPositionRelative();
}

//============================================================
//	�����擾����
//============================================================
D3DXVECTOR3 CString2D::GetVec3Rotation(void) const
{
	// �����񂪂Ȃ��ꍇ������
	if ((int)m_wsStr.size() <= 0) { assert(false); return VEC3_ZERO; }

	// �擪�̌�����Ԃ�
	assert(m_ppChar[0] != nullptr);
	return m_ppChar[0]->GetVec3Rotation();
}

//============================================================
//	�����c���̐ݒ菈��
//============================================================
void CString2D::SetHeight(const float fHeight)
{
	for (int i = 0; i < (int)m_wsStr.size(); i++)
	{ // ���������J��Ԃ�

		// �����c���̐ݒ�
		assert(m_ppChar[i] != nullptr);
		m_ppChar[i]->SetHeight(fHeight);

		// TODO
#if 0
		m_ppChar[i]->BindTexture(-1);
#endif
	}

	// ���Έʒu�̐ݒ�
	SetPositionRelative();
}

//============================================================
//	�����c���̎擾����
//============================================================
float CString2D::GetHeight(void) const
{
	// �����񂪂Ȃ��ꍇ������
	if ((int)m_wsStr.size() <= 0) { assert(false); return 0.0f; }

	// �擪�̕����c����Ԃ�
	assert(m_ppChar[0] != nullptr);
	return m_ppChar[0]->GetHeight();
}

//============================================================
//	��������
//============================================================
CString2D *CString2D::Create
(
	CFontChar *pFontChar,		// �t�H���g�������
	const std::wstring &rStr,	// �w�蕶����
	const D3DXVECTOR3 &rPos,	// ���_�ʒu
	const float fHeight,		// �����c��
	const EAlignX alignX,		// ���z�u
	const D3DXVECTOR3& rRot		// ���_����
)
{
	// ������2D�̐���
	CString2D *pString2D = new CString2D;
	if (pString2D == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// ������2D�̏�����
		if (FAILED(pString2D->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ������2D�̔j��
			SAFE_DELETE(pString2D);
			return nullptr;
		}

		// �t�H���g�E�������ݒ�
		if (FAILED(pString2D->SetFontString(pFontChar, rStr)))
		{ // �ݒ�Ɏ��s�����ꍇ

			// ������2D�̔j��
			SAFE_DELETE(pString2D);
			return nullptr;
		}

		// ���_�ʒu��ݒ�
		pString2D->SetVec3Position(rPos);

		// ���_������ݒ�
		pString2D->SetVec3Rotation(rRot);

		// �����c����ݒ�
		pString2D->SetHeight(fHeight);

		// ���z�u��ݒ�
		pString2D->SetAlignX(alignX);

		// �m�ۂ����A�h���X��Ԃ�
		return pString2D;
	}
}

//============================================================
//	�t�H���g�E������̐ݒ菈��
//============================================================
HRESULT CString2D::SetFontString
(
	CFontChar *pFontChar,		// �t�H���g�������
	const std::wstring &rStr	// �w�蕶����
)
{
	// ��������ۑ�����
	int nOldStrLen = (int)m_wsStr.size();	// �j�����镶����
	int nCurStrLen = (int)rStr.size();		// �������镶����

	// �w�蕶�����ύX
	m_wsStr = rStr;

	//--------------------------------------------------------
	//	���Ɏg�p���Ă��镶����j��
	//--------------------------------------------------------
	for (int i = 0; i < nOldStrLen; i++)
	{ // �j�����镶�������J��Ԃ�

		// ������j��
		SAFE_UNINIT(m_ppChar[i]);
	}

	// �������j��
	SAFE_DEL_ARRAY(m_ppChar);

	//--------------------------------------------------------
	//	�V���������𐶐�
	//--------------------------------------------------------
	// �������i�[����z��𐶐�
	m_ppChar = new CChar2D*[nCurStrLen];
	for (int i = 0; i < nCurStrLen; i++)
	{ // �������镶�������J��Ԃ�

		// �����𐶐�
		m_ppChar[i] = CChar2D::Create
		( // ����
			pFontChar,	// �t�H���g�������
			m_wsStr[i]	// �w�蕶��
		);
		if (m_ppChar[i] == nullptr)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}
	}

	// ���Έʒu�̐ݒ�
	SetPositionRelative();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�����擾����
//============================================================
CChar2D *CString2D::GetChar2D(const int nCharID) const
{
	// �����񂪂Ȃ��ꍇ������
	int nStrLen = (int)m_wsStr.size();
	if (nStrLen <= 0) { assert(false); return nullptr; }

	// �C���f�b�N�X���͈͊O�̏ꍇ������
	if (nCharID <= NONE_IDX || nCharID >= nStrLen) { assert(false); return nullptr; }

	// �����C���f�b�N�X�̕�����Ԃ�
	return m_ppChar[nCharID];
}

//============================================================
//	������̉����擾����
//============================================================
float CString2D::GetStrWidth(void) const
{
	// �����񂪂Ȃ��ꍇ������
	if ((int)m_wsStr.size() <= 0) { assert(false); return 0.0f; }

	float fStrWidth = 0.0f;	// ������̉���
	int nEndCharID = (int)m_wsStr.size() - 1;	// �I�[�����̃C���f�b�N�X

	for (int i = 0; i < nEndCharID; i++)
	{ // �I�[�����𔲂������������J��Ԃ�

		// ���̕����܂ł̋��������Z
		assert(m_ppChar[i] != nullptr);
		fStrWidth += m_ppChar[i]->GetNext();
	}

	// �擪�����̖��������傫�������Z
	assert(m_ppChar[0] != nullptr);
	float fStartCharWidth = m_ppChar[0]->GetVec3Sizing().x * 0.5f;
	fStrWidth += m_ppChar[0]->GetOffsetOrigin();
	fStrWidth -= fStartCharWidth + m_ppChar[0]->GetOffsetBlackBoxLU().x;

	// �I�[�����̑傫�������Z
	assert(m_ppChar[nEndCharID] != nullptr);
	float fEndCharWidth = m_ppChar[nEndCharID]->GetVec3Sizing().x * 0.5f;
	fStrWidth += fEndCharWidth - m_ppChar[nEndCharID]->GetOffsetOrigin();
	fStrWidth += m_ppChar[nEndCharID]->GetOffsetBlackBoxRD().x;

	// ������̉�����Ԃ�
	return fStrWidth;
}

//============================================================
//	���z�u�̐ݒ菈��
//============================================================
void CString2D::SetAlignX(const CString2D::EAlignX alignX)
{
	// �����̉��z�u��ݒ�
	m_alignX = alignX;

	// ���Έʒu�̐ݒ�
	SetPositionRelative();
}

//============================================================
//	���z�u�̎擾����
//============================================================
CString2D::EAlignX CString2D::GetAlignX(void) const
{
	// ���z�u��Ԃ�
	return m_alignX;
}

//============================================================
//	�j������
//============================================================
void CString2D::Release(void)
{
	// �I�u�W�F�N�g�̔j��
	CObject::Release();
}

//============================================================
//	���Έʒu�̐ݒ菈��
//============================================================
void CString2D::SetPositionRelative(void)
{
	// �����񂪂Ȃ��ꍇ������
	if ((int)m_wsStr.size() <= 0) { assert(false); return; }

	assert(m_ppChar[0] != nullptr);
	float fStrWidth		= GetStrWidth() * 0.5f;	// ������S�̂̉���

	// TODO
#if 1
	float fHeadOffset	= m_ppChar[0]->GetOffsetBlackBoxLU().x;	// �擪�����̌��_�I�t�Z�b�g
#else
	float fHeadOffset	= m_ppChar[0]->GetOffsetOrigin();		// �擪�����̌��_�I�t�Z�b�g
#endif

	float fHeadRot		= m_ppChar[0]->GetVec3Rotation().z - HALF_PI;	// �擪�����̌���
	float fHeadWidth	= m_ppChar[0]->GetVec3Sizing().x * 0.5f;		// �擪�����̉���

	// TODO
#if 0
	float fStartOffset	= fStrWidth - (fHeadWidth - fHeadOffset) + (fStrWidth * (m_alignX - 1));	// �����̊J�n�ʒu�I�t�Z�b�g
#else
	float fStartOffset	= fStrWidth + (fHeadOffset) + (fStrWidth * (m_alignX - 1));	// �����̊J�n�ʒu�I�t�Z�b�g
#endif

	D3DXVECTOR3 posStart;	// �����̊J�n�ʒu
	posStart.x = m_pos.x + sinf(fHeadRot) * fStartOffset;	// �J�n�ʒuX
	posStart.y = m_pos.y + cosf(fHeadRot) * fStartOffset;	// �J�n�ʒuY

	for (int i = 0; i < (int)m_wsStr.size(); i++)
	{ // ���������J��Ԃ�

		assert(m_ppChar[i] != nullptr);
		float fOffset = m_ppChar[i]->GetOffsetOrigin();	// ���_�I�t�Z�b�g
		float fNext = m_ppChar[i]->GetNext();		// �������܂ł̋���

		// �ݒ���W�Ɍ��_�I�t�Z�b�g��^����
		posStart.x -= sinf(fHeadRot) * fOffset;
		posStart.y -= cosf(fHeadRot) * fOffset;

		// �ʒu�𔽉f
		m_ppChar[i]->SetVec3Position(posStart);

		// ���̐ݒ���W�̊J�n�_��ۑ�
		posStart.x += sinf(fHeadRot) * (fOffset - fNext);
		posStart.y += cosf(fHeadRot) * (fOffset - fNext);
	}
}
