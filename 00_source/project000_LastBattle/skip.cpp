//============================================================
//
//	�X�L�b�v���쏈�� [skip.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "skip.h"
#include "renderer.h"
#include "manager.h"
#include "sound.h"
#include "fade.h"
#include "texture.h"
#include "object2D.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char *TEXTURE_FILE = "data\\TEXTURE\\skip000.png";	// �X�L�b�v����\���e�N�X�`��
	const int	PRIORITY	 = 6;		// �X�L�b�v����\���̗D�揇��
	const int	DISP_FRAME	 = 240;		// ���S�s�����t���[��
	const float	FADE_LEVEL	 = 0.05f;	// �����x�̃t���[���ϓ���
}

//************************************************************
//	�q�N���X [CSkip] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CSkip::CSkip() : 
	m_pControl	(nullptr),		// ����̏��
	m_state		(STATE_NONE),	// ���
	m_nCounter	(0)				// ��ԊǗ��J�E���^�[
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CSkip::~CSkip()
{

}

//============================================================
//	����������
//============================================================
HRESULT CSkip::Init(void)
{
	// �����o�ϐ���������
	m_pControl	= nullptr;		// ����̏��
	m_state		= STATE_NONE;	// ���
	m_nCounter	= 0;			// ��ԊǗ��J�E���^�[

	// ������̐���
	m_pControl = CObject2D::Create
	( // ����
		VEC3_ZERO,	// �ʒu
		VEC3_ZERO,	// �傫��
		VEC3_ZERO,	// ����
		XCOL_AWHITE	// �F
	);
	if (m_pControl == nullptr)
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�N�X�`����o�^�E����
	m_pControl->BindTexture(TEXTURE_FILE);

	// �D�揇�ʂ�ݒ�
	m_pControl->SetPriority(PRIORITY);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CSkip::Uninit(void)
{
	// ������̏I��
	SAFE_UNINIT(m_pControl);
}

//============================================================
//	�X�V����
//============================================================
void CSkip::Update(void)
{
	if (m_state != STATE_NONE)
	{ // �������Ȃ���Ԃł͂Ȃ��ꍇ

		D3DXCOLOR colCont = m_pControl->GetColor();	// ����\���̐F

		switch (m_state)
		{ // ��Ԃ��Ƃ̏���
		case STATE_FADEOUT:

			// ���l�����Z
			colCont.a += FADE_LEVEL;
			if (colCont.a >= 1.0f)
			{ // �s�����ɂȂ����ꍇ

				// �����x��␳
				colCont.a = 1.0f;

				// �\����Ԃɂ���
				m_state = STATE_DISP;
			}

			break;

		case STATE_DISP:

			// �J�E���^�[�����Z
			m_nCounter++;
			if (m_nCounter > DISP_FRAME)
			{ // �\�����Ԃ𒴂����ꍇ

				// �J�E���^�[������
				m_nCounter = 0;

				// �t�F�[�h�C����Ԃɂ���
				m_state = STATE_FADEIN;
			}

			break;

		case STATE_FADEIN:

			// ���l�����Z
			colCont.a -= FADE_LEVEL;
			if (colCont.a <= 0.0f)
			{ // �����ɂȂ����ꍇ

				// �����x��␳
				colCont.a = 0.0f;

				// �������Ȃ���Ԃɂ���
				m_state = STATE_NONE;
			}

			break;

		default:
			assert(false);
			break;
		}

		// �F�𔽉f
		m_pControl->SetColor(colCont);
	}

	// ������̍X�V
	m_pControl->Update();
}

//============================================================
//	�`�揈��
//============================================================
void CSkip::Draw(void)
{

}

//============================================================
//	�\���ݒ菈��
//============================================================
void CSkip::SetDisp(void)
{
	// ���ɕ\�����̏ꍇ������
	if (m_state == STATE_DISP) { return; }

	// �J�E���^�[��������
	m_nCounter = 0;

	// �t�F�[�h�A�E�g��Ԃɂ���
	m_state = STATE_FADEOUT;
}

//============================================================
//	�\���擾����
//============================================================
bool CSkip::IsDisp(void)
{
	// �\���󋵂�ݒ�
	bool bDisp = (m_state == STATE_DISP);

	// �\���󋵂�Ԃ�
	return bDisp;
}

//============================================================
//	��������
//============================================================
CSkip *CSkip::Create
(
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rSize	// �傫��
)
{
	// �X�L�b�v����̐���
	CSkip *pSkip = new CSkip;
	if (pSkip == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �X�L�b�v����̏�����
		if (FAILED(pSkip->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �X�L�b�v����̔j��
			SAFE_DELETE(pSkip);
			return nullptr;
		}

		// �ʒu��ݒ�
		pSkip->m_pControl->SetVec3Position(rPos);

		// �傫����ݒ�
		pSkip->m_pControl->SetVec3Sizing(rSize);

		// �m�ۂ����A�h���X��Ԃ�
		return pSkip;
	}
}

//============================================================
//	�j������
//============================================================
void CSkip::Release(CSkip *&prSkip)
{
	// �X�L�b�v����̏I��
	assert(prSkip != nullptr);
	prSkip->Uninit();

	// �������J��
	SAFE_DELETE(prSkip);
}
