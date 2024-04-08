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

//************************************************************
//	�q�N���X [CFont2D] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CFont2D::CFont2D(const CObject::ELabel label, const EDim dimension, const int nPriority) : CObject2D(label, dimension, nPriority),
	m_fHeight	(0.0f)	// �c��
{

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
	m_fHeight = 0.0f;	// �c��

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
//	��������
//============================================================
CFont2D *CFont2D::Create
(
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rSize,	// �傫��
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

		// �ʒu��ݒ�
		pFont2D->SetVec3Position(rPos);

		// ������ݒ�
		pFont2D->SetVec3Rotation(rRot);

		// �傫����ݒ�
		pFont2D->SetVec3Sizing(rSize);

		// �F��ݒ�
		pFont2D->SetColor(rCol);

		// �m�ۂ����A�h���X��Ԃ�
		return pFont2D;
	}
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CFont2D::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// �ʒu�̐ݒ�
	CObject2D::SetVec3Position(rPos);

	// �t�H���g�̃e�N�X�`�����W�̐ݒ�
	//CObject2D::SetFontTex(m_nPattern, m_nWidthPtrn, m_nHeightPtrn);
}

//============================================================
//	�����̐ݒ菈��
//============================================================
void CFont2D::SetVec3Rotation(const D3DXVECTOR3& rRot)
{
	// �����̐ݒ�
	CObject2D::SetVec3Rotation(rRot);

	// �t�H���g�̃e�N�X�`�����W�̐ݒ�
	//CObject2D::SetFontTex(m_nPattern, m_nWidthPtrn, m_nHeightPtrn);
}

//============================================================
//	�傫���̐ݒ菈��
//============================================================
void CFont2D::SetVec3Sizing(const D3DXVECTOR3& /*rSize*/)
{
	// �傫���̐ݒ�֐��͔䗦���ς���Ă��܂��̂Ŏg�p�ł��Ȃ�
	// �傫���ύX�̍ۂ͓��N���X�� SetHegiht �𗘗p���Ă�������
	assert(false);
}

//============================================================
//	�F�̐ݒ菈��
//============================================================
void CFont2D::SetColor(const D3DXCOLOR& rCol)
{
	// �F�̐ݒ�
	CObject2D::SetColor(rCol);

	// �t�H���g�̃e�N�X�`�����W�̐ݒ�
	//CObject2D::SetFontTex(m_nPattern, m_nWidthPtrn, m_nHeightPtrn);
}

//============================================================
//	�c���̐ݒ菈��
//============================================================
void CFont2D::SetHeight(const float fHeight)
{
	// �����̏c����ݒ�
	m_fHeight = fHeight;

	// �c������ɉ��������߂�
	float fWidth = useful::GetTexWidthFromAspect(fHeight, GetTexID());

	// �傫����ݒ�
	SetVec3Sizing(D3DXVECTOR3(fWidth, fHeight, 0.0f));
}
