//============================================================
//
//	�������� [value.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "value.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char *TEXTURE_FILE[] =	// �e�N�X�`���t�@�C��
	{
		"data\\TEXTURE\\number000.png",	// �ʏ�e�N�X�`��
	};

	const int PRIORITY		= 5;	// �����̗D�揇��
	const int PTRN_WIDTH	= 10;	// �e�N�X�`���̉��̕�����
	const int PTRN_HEIGHT	= 1;	// �e�N�X�`���̏c�̕�����
}

//************************************************************
//	�X�^�e�B�b�N�A�T�[�g
//************************************************************
static_assert(NUM_ARRAY(TEXTURE_FILE) == CValue::TEXTURE_MAX, "ERROR : Texture Count Mismatch");

//************************************************************
//	�q�N���X [CValue] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CValue::CValue() : CAnim2D(CObject::LABEL_NONE, CObject::DIM_2D, PRIORITY), m_nNum(0)
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CValue::~CValue()
{

}

//============================================================
//	����������
//============================================================
HRESULT CValue::Init(void)
{
	// �����o�ϐ���������
	m_nNum = 0;	// �\�����l

	// �A�j���[�V����2D�̏�����
	if (FAILED(CAnim2D::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�N�X�`�����������̐ݒ�
	SetWidthPattern(PTRN_WIDTH);

	// �e�N�X�`���c�������̐ݒ�
	SetHeightPattern(PTRN_HEIGHT);

	// �A�j���[�V�������~
	SetEnableStop(true);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CValue::Uninit(void)
{
	// �A�j���[�V����2D�̏I��
	CAnim2D::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CValue::Update(void)
{
	// �A�j���[�V����2D�̍X�V
	CAnim2D::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CValue::Draw(void)
{
	// �A�j���[�V����2D�̕`��
	CAnim2D::Draw();
}

//============================================================
//	��������
//============================================================
CValue *CValue::Create
(
	const ETexture texture,		// �e�N�X�`��
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rSize,	// �傫��
	const D3DXVECTOR3& rRot,	// ����
	const D3DXCOLOR& rCol		// �F
)
{
	// �����̐���
	CValue *pValue = new CValue;
	if (pValue == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �����̏�����
		if (FAILED(pValue->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �����̔j��
			SAFE_DELETE(pValue);
			return nullptr;
		}

		// �e�N�X�`����ݒ�
		pValue->SetTexture(texture);

		// �ʒu��ݒ�
		pValue->SetVec3Position(rPos);

		// ������ݒ�
		pValue->SetVec3Rotation(rRot);

		// �傫����ݒ�
		pValue->SetVec3Sizing(rSize);

		// �F��ݒ�
		pValue->SetColor(rCol);

		// �m�ۂ����A�h���X��Ԃ�
		return pValue;
	}
}

//============================================================
//	�e�N�X�`���̐ݒ菈��
//============================================================
void CValue::SetTexture(const ETexture texture)
{
	// �e�N�X�`����o�^�E����
	BindTexture(GET_MANAGER->GetTexture()->Regist(TEXTURE_FILE[texture]));
}

//============================================================
//	�l�̐ݒ菈��
//============================================================
void CValue::SetNumber(const int nNum)
{
	// �����̒l����
	m_nNum = nNum;

	// �p�^�[���̐ݒ�
	CAnim2D::SetPattern(m_nNum);
}