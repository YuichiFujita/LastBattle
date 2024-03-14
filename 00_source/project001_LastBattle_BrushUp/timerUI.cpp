//============================================================
//
//	�^�C�}�[UI���� [timerUI.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "timerUI.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "object2D.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char *TEXTURE_FILE[] =	// �e�N�X�`���t�@�C��
	{
		"data\\TEXTURE\\timer001.png",	// �w�i�e�N�X�`��
	};

	const int PRIORITY = 5;	// �^�C�}�[UI�̗D�揇��
}

//************************************************************
//	�X�^�e�B�b�N�A�T�[�g
//************************************************************
static_assert(NUM_ARRAY(TEXTURE_FILE) == CTimerUI::TEXTURE_MAX, "ERROR : Texture Count Mismatch");

//************************************************************
//	�q�N���X [CTimerUI] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CTimerUI::CTimerUI() :
	m_pBG		(nullptr),	// �w�i�̏��
	m_offsetBG	(VEC3_ZERO)	// �w�i�̃I�t�Z�b�g
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CTimerUI::~CTimerUI()
{

}

//============================================================
//	����������
//============================================================
HRESULT CTimerUI::Init(void)
{
	// �����o�ϐ���������
	m_pBG = nullptr;		// �w�i�̏��
	m_offsetBG = VEC3_ZERO;	// �w�i�̃I�t�Z�b�g

	// �w�i�̐���
	m_pBG = CObject2D::Create(VEC3_ZERO);
	if (m_pBG == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�N�X�`����o�^�E����
	m_pBG->BindTexture(TEXTURE_FILE[TEXTURE_BG]);

	// �^�C�}�[�}�l�[�W���[�̏�����
	if (FAILED(CTimerManager::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �D�揇�ʂ�ݒ�
	SetPriority(PRIORITY);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CTimerUI::Uninit(void)
{
	// �w�i�̏I��
	SAFE_UNINIT(m_pBG);

	// �^�C�}�[�}�l�[�W���[�̏I��
	CTimerManager::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CTimerUI::Update(void)
{
	// �w�i�̍X�V
	m_pBG->Update();

	// �^�C�}�[�}�l�[�W���[�̍X�V
	CTimerManager::Update();
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CTimerUI::SetPosition(const D3DXVECTOR3& rPos)
{
	// �����̈ʒu��ݒ�
	CTimerManager::SetPosition(rPos);

	// ���Έʒu�̐ݒ�
	SetPositionRelative();
}

//============================================================
//	�D�揇�ʂ̐ݒ菈��
//============================================================
void CTimerUI::SetPriority(const int nPriority)
{
	// �w�i�̗D�揇�ʂ�ݒ�
	m_pBG->SetPriority(nPriority);

	// �D�揇�ʂ̐ݒ�
	CTimerManager::SetPriority(nPriority);
}

//============================================================
//	�X�V�󋵂̐ݒ菈��
//============================================================
void CTimerUI::SetEnableUpdate(const bool bUpdate)
{
	// �w�i�̍X�V�󋵂�ݒ�
	m_pBG->SetEnableUpdate(bUpdate);

	// �X�V�󋵂̐ݒ�
	CTimerManager::SetEnableUpdate(bUpdate);
}

//============================================================
//	�`��󋵂̐ݒ菈��
//============================================================
void CTimerUI::SetEnableDraw(const bool bDraw)
{
	// �w�i�̍X�V�󋵂�ݒ�
	m_pBG->SetEnableDraw(bDraw);

	// �`��󋵂̐ݒ�
	CTimerManager::SetEnableDraw(bDraw);
}

//============================================================
//	��������
//============================================================
CTimerUI *CTimerUI::Create
(
	const ETime time,				// �ݒ�^�C��
	const long nTime,				// ��������
	const D3DXVECTOR3& rPos,		// �ʒu
	const D3DXVECTOR3& rSizeValue,	// �����̑傫��
	const D3DXVECTOR3& rSizePart,	// ��؂�̑傫��
	const D3DXVECTOR3& rSpaceValue,	// �����̋�
	const D3DXVECTOR3& rSpacePart,	// ��؂�̋�
	const D3DXVECTOR3& rOffsetBG,	// �w�i�̃I�t�Z�b�g
	const D3DXVECTOR3& rSizeBG		// �w�i�̑傫��
)
{
	// �^�C�}�[UI�̐���
	CTimerUI *pTimerUI = new CTimerUI;
	if (pTimerUI == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �^�C�}�[UI�̏�����
		if (FAILED(pTimerUI->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �^�C�}�[UI�̔j��
			SAFE_DELETE(pTimerUI);
			return nullptr;
		}

		// �������Ԃ�ݒ�
		pTimerUI->SetLimit(time, nTime);

		// �ʒu��ݒ�
		pTimerUI->SetPosition(rPos);

		// �����̑傫����ݒ�
		pTimerUI->SetSizingValue(rSizeValue);

		// ��؂�̑傫����ݒ�
		pTimerUI->SetSizingPart(rSizePart);

		// �����̋󔒂�ݒ�
		pTimerUI->SetSpaceValue(rSpaceValue);

		// ��؂�̋󔒂�ݒ�
		pTimerUI->SetSpacePart(rSpacePart);

		// �I�t�Z�b�g��ݒ�
		pTimerUI->SetOffset(rOffsetBG);

		// �w�i�̑傫����ݒ�
		pTimerUI->SetSizingBG(rSizeBG);

		// �m�ۂ����A�h���X��Ԃ�
		return pTimerUI;
	}
}

//============================================================
//	�j������
//============================================================
void CTimerUI::Release(CTimerUI *&prTimerUI)
{
	// �^�C�}�[UI�̏I��
	assert(prTimerUI != nullptr);
	prTimerUI->Uninit();

	// �������J��
	SAFE_DELETE(prTimerUI);
}

//============================================================
//	�I�t�Z�b�g�̐ݒ菈��
//============================================================
void CTimerUI::SetOffset(const D3DXVECTOR3& rOffset)
{
	// �I�t�Z�b�g�̐ݒ�
	m_offsetBG = rOffset;

	// ���Έʒu�̐ݒ�
	SetPositionRelative();
}

//============================================================
//	�w�i�̑傫���ݒ菈��
//============================================================
void CTimerUI::SetSizingBG(const D3DXVECTOR3& rSize)
{
	// �w�i�̑傫����ݒ�
	assert(m_pBG != nullptr);
	m_pBG->SetVec3Sizing(rSize);
}

//============================================================
//	���Έʒu�̐ݒ菈��
//============================================================
void CTimerUI::SetPositionRelative(void)
{
	// �w�i�ʒu��ݒ�
	m_pBG->SetVec3Position(GetPosition() + m_offsetBG);
}
