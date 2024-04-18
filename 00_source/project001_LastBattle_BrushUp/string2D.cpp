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

#include <locale>
#include <codecvt>

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int PRIORITY = 5;	// ������2D�̗D�揇��
}

//************************************************************
//	�q�N���X [CString2D] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CString2D::CString2D() : CObject(CObject::LABEL_UI, CObject::DIM_2D, PRIORITY),
	m_ppChar	(nullptr),		// �����|���S���̏��
	m_pos		(VEC3_ZERO),	// ������̌��_�ʒu
	m_nStrLen	(0)				// ������̒���
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
	m_ppChar	= nullptr;		// �����|���S���̏��
	m_pos		= VEC3_ZERO;	// ������̌��_�ʒu
	m_nStrLen	= 0;			// ������̒���

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CString2D::Uninit(void)
{
	for (int i = 0; i < m_nStrLen; i++)
	{
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

}

//============================================================
//	�`�揈��
//============================================================
void CString2D::Draw(CShader * /*pShader*/)
{

}

//============================================================
//	���_�ʒu�̐ݒ菈��
//============================================================
void CString2D::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// �����̈ʒu��ݒ�
	m_pos = rPos;
}

//============================================================
//	���_�ʒu�̎擾����
//============================================================
D3DXVECTOR3 CString2D::GetVec3Position(void) const
{
	// �ʒu��Ԃ�
	return m_pos;
}

//============================================================
//	�����c���̐ݒ菈��
//============================================================
void CString2D::SetHeight(const float fHeight)
{
	CTexture *pTexture = GET_MANAGER->GetTexture();	// �e�N�X�`�����

	{
#if 0
		int nTexID = GetTextureIndex();	// �t�H���g�̃e�N�X�`���C���f�b�N�X
		float fTexWidth = useful::GetTexWidthFromAspect(fHeight, nTexID);	// �e�N�X�`������
		D3DXIMAGE_INFO status = pTexture->GetInfo(nTexID).status;			// �e�N�X�`���X�e�[�^�X

		// �c���̑���������ۑ�
		m_fSizeRate = fHeight / (float)status.Height;

		// �傫����ݒ�
		SetVec3Sizing(D3DXVECTOR3(fTexWidth, fHeight, 0.0f));
#endif
	}
}

//============================================================
//	�����c���̎擾����
//============================================================
float CString2D::GetHeight(void) const
{
	return 0.0f;
}

//============================================================
//	��������
//============================================================
CString2D *CString2D::Create
(
	CFontChar *pFontChar,		// �t�H���g�������
	std::string sString,		// �w�蕶����
	const D3DXVECTOR3 &rPos,	// ���_�ʒu
	const float fHeight			// �����c��
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

		// ���_�ʒu��ݒ�
		pString2D->SetVec3Position(rPos);

		// �����c����ݒ�
		pString2D->SetHeight(fHeight);

		// �t�H���g�E�������ݒ�
		if (FAILED(pString2D->SetFontString(pFontChar, sString, fHeight)))
		{ // �ݒ�Ɏ��s�����ꍇ

			// ������2D�̔j��
			SAFE_DELETE(pString2D);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pString2D;
	}
}

//============================================================
//	�t�H���g�E������̐ݒ菈��
//============================================================
HRESULT CString2D::SetFontString
(
	CFontChar *pFontChar,	// �t�H���g�������
	std::string sString,	// �w�蕶����
	const float fHeight		// �����c��
)
{
	for (int i = 0; i < m_nStrLen; i++)
	{
		// ������j��
		SAFE_UNINIT(m_ppChar[i]);
	}

	// �������j��
	SAFE_DEL_ARRAY(m_ppChar);

	// �w�蕶�����ύX
	m_sString = sString;

	m_nStrLen = MultiByteToWideChar(CP_ACP, 0, m_sString.c_str(), NONE_IDX, nullptr, 0);
	std::wstring wideStr = std::wstring(m_nStrLen, L'\0');
	MultiByteToWideChar(CP_ACP, 0, m_sString.c_str(), NONE_IDX, &wideStr[0], m_nStrLen);

	// ������𐶐�
	m_ppChar = new CChar2D*[m_nStrLen];

#if 1
	float fPosX = m_pos.x;
	for (int i = 0; i < m_nStrLen; i++)
	{
		D3DXVECTOR3 pos = D3DXVECTOR3(fPosX, m_pos.y, 0.0f);

		// �����𐶐�
		m_ppChar[i] = CChar2D::Create
		( // ����
			pFontChar,
			wideStr[i],
			pos,
			fHeight
		);
		if (m_ppChar[i] == nullptr)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		D3DXVECTOR3 old = m_ppChar[i]->GetVec3Position();
		old.x += m_ppChar[i]->GetOffset();
		m_ppChar[i]->SetVec3Position(old);

		// ���̈ʒu�ݒ�p�Ɍ��_��ۑ�
		fPosX = m_ppChar[i]->GetVec3Position().x - m_ppChar[i]->GetOffset() + m_ppChar[i]->GetNext();
	}
#endif

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�j������
//============================================================
void CString2D::Release(void)
{
	// �I�u�W�F�N�g�̔j��
	CObject::Release();
}
