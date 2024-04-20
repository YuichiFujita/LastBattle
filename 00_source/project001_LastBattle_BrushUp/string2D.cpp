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
	const int PRIORITY = 5;	// ������2D�̗D�揇��
}

//************************************************************
//	�q�N���X [CString2D] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CString2D::CString2D() : CObject(CObject::LABEL_UI, CObject::DIM_2D, PRIORITY),
	m_ppChar	(nullptr),	// �����|���S���̏��
	m_pos		(VEC3_ZERO)	// ������̌��_�ʒu
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

	SetPositionRelative();
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

	// ���Έʒu�̐ݒ�
	SetPositionRelative();
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
	for (int i = 0; i < (int)m_wsStr.size(); i++)
	{ // ���������J��Ԃ�

		// �����c���̐ݒ�
		assert(m_ppChar[i] != nullptr);
		m_ppChar[i]->SetHeight(fHeight);

		// TODO
		m_ppChar[i]->BindTexture(-1);
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

	// �擪�̕����c���̕Ԃ�
	assert(m_ppChar[0] != nullptr);
	return m_ppChar[0]->GetHeight();
}

//============================================================
//	��������
//============================================================
CString2D *CString2D::Create
(
	CFontChar *pFontChar,		// �t�H���g�������
	std::wstring wsStr,			// �w�蕶����
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

		// �t�H���g�E�������ݒ�
		if (FAILED(pString2D->SetFontString(pFontChar, wsStr)))
		{ // �ݒ�Ɏ��s�����ꍇ

			// ������2D�̔j��
			SAFE_DELETE(pString2D);
			return nullptr;
		}

		// ���_�ʒu��ݒ�
		pString2D->SetVec3Position(rPos);

		// �����c����ݒ�
		pString2D->SetHeight(fHeight);

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
	std::wstring wsStr		// �w�蕶����
)
{
	// ��������ۑ�����
	int nOldStrLen = (int)m_wsStr.size();	// �j�����镶����
	int nCurStrLen = (int)wsStr.size();		// �������镶����

	// �w�蕶�����ύX
	m_wsStr = wsStr;

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

//============================================================
//	���Έʒu�̐ݒ菈��
//============================================================
void CString2D::SetPositionRelative(void)
{
#if 0
	float fPosX = m_pos.x;	// �ŏ��̕�����X���W
	for (int i = 0; i < (int)m_wsStr.size(); i++)
	{ // ���������J��Ԃ�

		// �I�t�Z�b�g�����炷
		D3DXVECTOR3 pos = D3DXVECTOR3(fPosX, m_pos.y, 0.0f);
		pos.x += m_ppChar[i]->GetOffset();
		m_ppChar[i]->SetVec3Position(pos);

		// ���̈ʒu�ݒ�p�Ɍ��_��ۑ�
		fPosX = m_ppChar[i]->GetVec3Position().x - m_ppChar[i]->GetOffset() + m_ppChar[i]->GetNext();
	}
#else
	float fPosX = m_pos.x - (GetStrWidth() * 0.5f) + ((m_ppChar[0]->GetVec3Sizing().x * 0.5f) - m_ppChar[0]->GetOffset());	// �ŏ��̕�����X���W
	for (int i = 0; i < (int)m_wsStr.size(); i++)
	{ // ���������J��Ԃ�

		// �I�t�Z�b�g�����炷
		D3DXVECTOR3 pos = D3DXVECTOR3(fPosX, m_pos.y, 0.0f);
		pos.x += m_ppChar[i]->GetOffset();
		m_ppChar[i]->SetVec3Position(pos);

		// ���̈ʒu�ݒ�p�Ɍ��_��ۑ�
		fPosX = m_ppChar[i]->GetVec3Position().x - m_ppChar[i]->GetOffset() + m_ppChar[i]->GetNext();
	}
#endif
}

//============================================================
//	������̉����擾����
//============================================================
float CString2D::GetStrWidth(void)
{
	float fWidth = 0.0f;

#if 1
#if 0
	for (int i = 0; i < (int)m_wsStr.size(); i++)
	{ // ���������J��Ԃ�

		assert(m_ppChar[i] != nullptr);
		fWidth += m_ppChar[i]->GetNext();
	}

	// �擪�����̖��������傫�������Z
	fWidth -= m_ppChar[0]->GetVec3Sizing().x * 0.5f - m_ppChar[0]->GetOffset();

	// �I�[�����̌��Z
	int n = (int)m_wsStr.size() - 1;
	float f = m_ppChar[n]->GetVec3Sizing().x * 0.5f - m_ppChar[n]->GetOffset();
	fWidth -= m_ppChar[n]->GetNext() - m_ppChar[n]->GetVec3Sizing().x - f - 1.0f;
#else
	for (int i = 0; i < (int)m_wsStr.size() - 1; i++)
	{ // ���������J��Ԃ�

		assert(m_ppChar[i] != nullptr);
		fWidth += m_ppChar[i]->GetNext();
	}

	// �擪�����̖��������傫�������Z
	fWidth += m_ppChar[0]->GetVec3Sizing().x * 0.5f - m_ppChar[0]->GetOffset();

	// 
	int n = (int)m_wsStr.size() - 1;
	float f = m_ppChar[n]->GetVec3Sizing().x * 0.5f - m_ppChar[n]->GetOffset();
	fWidth += m_ppChar[n]->GetVec3Sizing().x - f;

	//// �I�[�����̌��Z
	//int n = (int)m_wsStr.size() - 1;
	//float f = m_ppChar[n]->GetVec3Sizing().x * 0.5f - m_ppChar[n]->GetOffset();
	//fWidth -= m_ppChar[n]->GetNext() - m_ppChar[n]->GetVec3Sizing().x - f - 1.0f;
#endif
#else
	float fMinPosX = 0.0f;
	float fMaxPosX = 0.0f;
	for (int i = 0; i < (int)m_wsStr.size(); i++)
	{ // ���������J��Ԃ�

		assert(m_ppChar[i] != nullptr);
		fWidth += m_ppChar[i]->GetNext();
	}
#endif

	return fWidth;
}
