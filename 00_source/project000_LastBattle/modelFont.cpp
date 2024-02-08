//============================================================
//
//	���f���������� [modelFont.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "modelFont.h"
#include "manager.h"
#include "model.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char *MODEL_FILE[] =	// ���f���t�@�C��
	{
		"data\\MODEL\\FONT\\name_boss000.x",	// �{�X�̖��O
	};
	const int PRIORITY = 5;	// ���f�������̗D�揇��
}

//************************************************************
//	�X�^�e�B�b�N�A�T�[�g
//************************************************************
static_assert(NUM_ARRAY(MODEL_FILE) == CModelFont::TYPE_MAX, "ERROR : Type Count Mismatch");

//************************************************************
//	�q�N���X [CModelFont] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CModelFont::CModelFont() : CObjectModel(CObject::LABEL_UI, CObject::DIM_3D, PRIORITY)
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CModelFont::~CModelFont()
{

}

//============================================================
//	����������
//============================================================
HRESULT CModelFont::Init(void)
{
	// �I�u�W�F�N�g���f���̏�����
	if (FAILED(CObjectModel::Init()))
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
void CModelFont::Uninit(void)
{
	// �I�u�W�F�N�g���f���̏I��
	CObjectModel::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CModelFont::Update(void)
{
	// �I�u�W�F�N�g���f���̍X�V
	CObjectModel::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CModelFont::Draw(void)
{
	// �I�u�W�F�N�g���f���̕`��
	CObjectModel::Draw();
}

//============================================================
//	��������
//============================================================
CModelFont *CModelFont::Create
( // ����
	const EType type,			// ���
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rRot,	// ����
	const D3DXVECTOR3& rScale	// �傫��
)
{
	// ���f�������̐���
	CModelFont *pModelFont = new CModelFont;
	if (pModelFont == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// ���f�������̏�����
		if (FAILED(pModelFont->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ���f�������̔j��
			SAFE_DELETE(pModelFont);
			return nullptr;
		}

		// ���f����o�^�E����
		pModelFont->BindModel(GET_MANAGER->GetModel()->Regist(MODEL_FILE[type]));

		// �ʒu��ݒ�
		pModelFont->SetVec3Position(rPos);

		// ������ݒ�
		pModelFont->SetVec3Rotation(rRot);

		// �g�嗦��ݒ�
		pModelFont->SetVec3Scaling(rScale);

		// �m�ۂ����A�h���X��Ԃ�
		return pModelFont;
	}
}
