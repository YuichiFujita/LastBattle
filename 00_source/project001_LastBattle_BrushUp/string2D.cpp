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
	m_ppChar	(nullptr),		// �����|���S���̏��
	m_pos		(VEC3_ZERO),	// �ʒu
	m_origin	(ORIGIN_CENTER)	// ���_

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
	m_origin	= ORIGIN_CENTER;	// ���_

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
	D3DXVECTOR3 rot = GetVec3Rotation();
	rot.z += 0.01f;
	SetVec3Rotation(rot);

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
		//m_ppChar[i]->BindTexture(-1);
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
	std::wstring wsStr,			// �w�蕶����
	const D3DXVECTOR3 &rPos,	// ���_�ʒu
	const float fHeight,		// �����c��
	const EOrigin origin,		// ���_
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
		if (FAILED(pString2D->SetFontString(pFontChar, wsStr)))
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

		// ���_��ݒ�
		pString2D->SetOrigin(origin);

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
//	������̉����擾����
//============================================================
float CString2D::GetStrWidth(void)
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
	fStrWidth += (m_ppChar[0]->GetVec3Sizing().x * 0.5f) - m_ppChar[0]->GetOffset();

	// �I�[�����̑傫�������Z
	assert(m_ppChar[nEndCharID] != nullptr);
	float fEndCharWidth = m_ppChar[nEndCharID]->GetVec3Sizing().x;	// �I�[�����̉���
	fStrWidth += fEndCharWidth - ((fEndCharWidth * 0.5f) - m_ppChar[nEndCharID]->GetOffset());

	// ������̉�����Ԃ�
	return fStrWidth;
}

//============================================================
//	���_�̐ݒ菈��
//============================================================
void CString2D::SetOrigin(const EOrigin origin)
{
	// �����̌��_��ݒ�
	m_origin = origin;

	// ���Έʒu�̐ݒ�
	SetPositionRelative();
}

//============================================================
//	���_�擾����
//============================================================
CString2D::EOrigin CString2D::GetOrigin(void) const
{
	// ���_��Ԃ�
	return m_origin;
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

#if 0
	assert(m_ppChar[0] != nullptr);
	float fStrWidth = GetStrWidth() * 0.5f;	// ������S�̂̉���
	float fHeadWidth = m_ppChar[0]->GetVec3Sizing().x * 0.5f;	// �擪�����̉���
	float fStartPosX = m_pos.x - fStrWidth + (fHeadWidth - m_ppChar[0]->GetOffset());	// �J�nX���W

	// �J�nX���W�����_�ɉ����Ă��炷
	fStartPosX -= fStrWidth * (m_origin - 1);

	for (int i = 0; i < (int)m_wsStr.size(); i++)
	{ // ���������J��Ԃ�

		// �����̐ݒ���W
		D3DXVECTOR3 posChar = D3DXVECTOR3(fStartPosX, m_pos.y, 0.0f);

		// �ݒ���W�Ɍ��_�I�t�Z�b�g��^����
		assert(m_ppChar[i] != nullptr);
		posChar.x += m_ppChar[i]->GetOffset();

		// �ʒu�𔽉f
		m_ppChar[i]->SetVec3Position(posChar);

		// ���̐ݒ���W�̊J�n�_��ۑ�
		fStartPosX = m_ppChar[i]->GetVec3Position().x - m_ppChar[i]->GetOffset() + m_ppChar[i]->GetNext();
	}
#else
#if 0
	assert(m_ppChar[0] != nullptr);
	float fStrWidth = GetStrWidth() * 0.5f;	// ������S�̂̉���
	float fHeadRot = m_ppChar[0]->GetVec3Rotation().z - HALF_PI;	// �擪�����̌���
	float fHeadWidth = m_ppChar[0]->GetVec3Sizing().x * 0.5f;		// �擪�����̉���

	D3DXVECTOR3 posStart = VEC3_ZERO;
	posStart.x = m_pos.x - sinf(fHeadRot) * ((fStrWidth + fHeadWidth - m_ppChar[0]->GetOffset()) * (fStrWidth * (m_origin - 1)));
	posStart.y = m_pos.y - cosf(fHeadRot) * ((fStrWidth + fHeadWidth - m_ppChar[0]->GetOffset()) * (fStrWidth * (m_origin - 1)));

	for (int i = 0; i < (int)m_wsStr.size(); i++)
	{ // ���������J��Ԃ�

#if 0
		// �ݒ���W�Ɍ��_�I�t�Z�b�g��^����
		assert(m_ppChar[i] != nullptr);
		D3DXVECTOR3 posChar = VEC3_ZERO;
		posChar.x = posStart.x + sinf(fHeadRot) * m_ppChar[i]->GetOffset();
		posChar.y = posStart.y + cosf(fHeadRot) * m_ppChar[i]->GetOffset();

		// �ʒu�𔽉f
		m_ppChar[i]->SetVec3Position(posChar);

		// ���̐ݒ���W�̊J�n�_��ۑ�
		posStart.x = posChar.x - sinf(fHeadRot) * (m_ppChar[i]->GetOffset() + m_ppChar[i]->GetNext());
		posStart.y = posChar.y - cosf(fHeadRot) * (m_ppChar[i]->GetOffset() + m_ppChar[i]->GetNext());
#else
		// �ݒ���W�Ɍ��_�I�t�Z�b�g��^����
		assert(m_ppChar[i] != nullptr);
		D3DXVECTOR3 posChar;
		posChar.x = posStart.x + sinf(fHeadRot) * m_ppChar[i]->GetOffset();
		posChar.y = posStart.y + cosf(fHeadRot) * m_ppChar[i]->GetOffset();

		// �ʒu�𔽉f
		m_ppChar[i]->SetVec3Position(posChar);

		// ���̐ݒ���W�̊J�n�_��ۑ�
		posStart.x = m_ppChar[i]->GetVec3Position().x - sinf(fHeadRot) * (m_ppChar[i]->GetOffset() + m_ppChar[i]->GetNext());
		posStart.y = m_ppChar[i]->GetVec3Position().y - cosf(fHeadRot) * (m_ppChar[i]->GetOffset() + m_ppChar[i]->GetNext());
#endif
	}
#else
	assert(m_ppChar[0] != nullptr);
	float fStrWidth = GetStrWidth() * 0.5f;	// ������S�̂̉���
	float fHeadRot = m_ppChar[0]->GetVec3Rotation().z - HALF_PI;	// �擪�����̌���
	float fHeadWidth = m_ppChar[0]->GetVec3Sizing().x * 0.5f;		// �擪�����̉���

#if 1
	D3DXVECTOR3 posStart;
	posStart.x = m_pos.x + sinf(fHeadRot) * (fStrWidth - (fHeadWidth - m_ppChar[0]->GetOffset()) + (fStrWidth * (m_origin - 1)));
	posStart.y = m_pos.y + cosf(fHeadRot) * (fStrWidth - (fHeadWidth - m_ppChar[0]->GetOffset()) + (fStrWidth * (m_origin - 1)));
#else
	float fStartPosX = fStrWidth + (fHeadWidth - m_ppChar[0]->GetOffset());	// �J�nX���W

	// �J�nX���W�����_�ɉ����Ă��炷
	fStartPosX -= fStrWidth * (m_origin - 1);

	D3DXVECTOR3 posStart = VEC3_ZERO;
	posStart.x = m_pos.x + sinf(fHeadRot) * fStartPosX;
	posStart.y = m_pos.y + cosf(fHeadRot) * fStartPosX;
#endif

	//for (int i = 0; i < 1; i++)
	for (int i = 0; i < (int)m_wsStr.size(); i++)
	{ // ���������J��Ԃ�

#if 1
		// �ݒ���W�Ɍ��_�I�t�Z�b�g��^����
		assert(m_ppChar[i] != nullptr);
		posStart.x -= sinf(fHeadRot) * m_ppChar[i]->GetOffset();
		posStart.y -= cosf(fHeadRot) * m_ppChar[i]->GetOffset();

		// �ʒu�𔽉f
		m_ppChar[i]->SetVec3Position(posStart);

		// ���̐ݒ���W�̊J�n�_��ۑ�
		posStart.x += sinf(fHeadRot) * (m_ppChar[i]->GetOffset() - m_ppChar[i]->GetNext());
		posStart.y += cosf(fHeadRot) * (m_ppChar[i]->GetOffset() - m_ppChar[i]->GetNext());
#else

#endif
	}
#endif
#endif
}
