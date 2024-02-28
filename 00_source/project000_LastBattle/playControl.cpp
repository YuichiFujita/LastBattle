//============================================================
//
//	�v���C���쏈�� [playControl.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "playControl.h"
#include "renderer.h"
#include "manager.h"
#include "sound.h"
#include "fade.h"
#include "texture.h"
#include "anim2D.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char *TEX_CONTROL_FILE[] =	// ����e�N�X�`���t�@�C��
	{
		nullptr,								// ����Ȃ�
		"data\\TEXTURE\\playControl000.png",	// �R�摀��e�N�X�`��
		"data\\TEXTURE\\playControl001.png",	// �A���U������e�N�X�`��
	};

	const POSGRID2 PART = POSGRID2(1, 2);	// �e�N�X�`��������
	const int	PRIORITY		= 6;		// �v���C����\���̗D�揇��
	const int	SCALEIN_FRAME	= 8;		// �\���J�n���犮���܂ł̃t���[����
	const int	BLINK_FRAME		= 12;		// �_�Ńt���[��
	const float	MUL_SCALEPLUS	= 0.5f;		// �g�嗦���Z�ʂ̔{��
	const float	ORIGIN_SCALE	= 1.0f;		// ��b�g�嗦
	const float	ADD_SINROT		= 0.035f;	// �g�嗦���㉺������T�C���J�[�u�������Z��
	const float	MAX_ADD_SCALE	= 0.08f;	// �g�嗦�̍ő���Z��
	const float	FADE_LEVEL		= 0.05f;	// �����x�̃t���[���ϓ���
}

//************************************************************
//	�X�^�e�B�b�N�A�T�[�g
//************************************************************
static_assert(NUM_ARRAY(TEX_CONTROL_FILE) == CPlayControl::CONTROL_MAX, "ERROR : ControlTexture Count Mismatch");

//************************************************************
//	�q�N���X [CPlayControl] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CPlayControl::CPlayControl() : 
	m_pControl		(nullptr),		// ����̏��
	m_originSize	(VEC3_ZERO),	// ����̌��̑傫��
	m_contType		(CONTROL_NONE),	// �\������
	m_dispType		(DISP_NORMAL),	// �\���`��
	m_state			(STATE_NONE),	// ���
	m_nCounter		(0),			// ��ԊǗ��J�E���^�[
	m_fSinScale		(0.0f),			// �g�k����
	m_fScale		(0.0f)			// �g�嗦
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CPlayControl::~CPlayControl()
{

}

//============================================================
//	����������
//============================================================
HRESULT CPlayControl::Init(void)
{
	// �����o�ϐ���������
	m_pControl		= nullptr;		// ����̏��
	m_originSize	= VEC3_ZERO;	// ����̌��̑傫��
	m_contType		= CONTROL_NONE;	// �\������
	m_dispType		= DISP_NORMAL;	// �\���`��
	m_state			= STATE_NONE;	// ���
	m_nCounter		= 0;			// ��ԊǗ��J�E���^�[
	m_fSinScale		= 0.0f;			// �g�k����
	m_fScale		= 0.0f;			// �g�嗦

	// ������̐���
	m_pControl = CAnim2D::Create
	( // ����
		PART.x,		// �e�N�X�`����������
		PART.y,		// �e�N�X�`���c������
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

	// �D�揇�ʂ�ݒ�
	m_pControl->SetPriority(PRIORITY);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CPlayControl::Uninit(void)
{
	// ������̏I��
	SAFE_UNINIT(m_pControl);
}

//============================================================
//	�X�V����
//============================================================
void CPlayControl::Update(void)
{
	if (m_state != STATE_NONE)
	{ // �������Ȃ���Ԃł͂Ȃ��ꍇ

		D3DXVECTOR3 sizeCont = m_pControl->GetVec3Sizing();	// ����\���̑傫��
		D3DXCOLOR colCont = m_pControl->GetColor();			// ����\���̐F

		switch (m_state)
		{ // ��Ԃ��Ƃ̏���
		case STATE_SCALEIN:
		{
			// �g�嗦��ݒ�
			m_fScale = (easeing::OutCubic((SCALEIN_FRAME - m_nCounter), 0, SCALEIN_FRAME) * MUL_SCALEPLUS) + 1.0f;

			// ����\���̑傫����ݒ�
			sizeCont = m_originSize * m_fScale;

			// �J�E���^�[�����Z
			m_nCounter++;
			if (m_nCounter > SCALEIN_FRAME)
			{ // �\�����Ԃ𒴂����ꍇ

				// �J�E���^�[������
				m_nCounter = 0;

				// �\����Ԃɂ���
				m_state = STATE_DISP;
			}

			break;
		}
		case STATE_DISP:
		{
			float fAddScale = 0.0f;	// �g�嗦�̉��Z��

			// �T�C���J�[�u��������]
			m_fSinScale += ADD_SINROT;
			useful::NormalizeRot(m_fSinScale);	// �������K��

			// �g�嗦���Z�ʂ����߂�
			fAddScale = (MAX_ADD_SCALE / 2.0f) * (sinf(m_fSinScale) - 1.0f);

			// �g�嗦��ݒ�
			m_fScale = ORIGIN_SCALE + fAddScale;

			// ����\���̑傫����ݒ�
			sizeCont = m_originSize * m_fScale;

			break;
		}
		case STATE_FADEOUT:
		{
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
		}
		default:
			assert(false);
			break;
		}

		// �傫���E�F�𔽉f
		m_pControl->SetVec3Sizing(sizeCont);
		m_pControl->SetColor(colCont);
	}

	// ������̍X�V
	m_pControl->Update();
}

//============================================================
//	�`�揈��
//============================================================
void CPlayControl::Draw(void)
{

}

//============================================================
//	�\���ݒ菈��
//============================================================
void CPlayControl::SetDisp
(
	EControl contType,	// �\������
	EDisp dispType		// �\���`��
)
{
	// ���ɕ\�����̏ꍇ������
	if (m_state == STATE_DISP) { return; }

	// �\�������ݒ�
	m_contType = contType;
	m_pControl->BindTexture(TEX_CONTROL_FILE[m_contType]);

	// �\���`����ݒ�
	m_dispType = dispType;
	switch (m_dispType)
	{ // �\���`�����Ƃ̏���
	case DISP_NORMAL:
		m_pControl->SetCounter(0);
		break;

	case DISP_BLINK:
		m_pControl->SetCounter(BLINK_FRAME);
		break;

	default:
		assert(false);
		break;
	}

	// ����������
	m_nCounter	= 0;
	m_fSinScale	= 0.0f;
	m_fScale	= 1.0f;

	// �p�^�[����������
	m_pControl->SetPattern(0);

	// ����\���̑傫����������
	D3DXVECTOR3 sizeCont = m_pControl->GetVec3Sizing();						// ����\���̑傫��
	m_fScale = easeing::OutCubic(SCALEIN_FRAME, 0, SCALEIN_FRAME) + 1.0f;	// �����g�嗦��ݒ�
	sizeCont = m_originSize * m_fScale;		// ����\���̑傫����ݒ�
	m_pControl->SetVec3Sizing(sizeCont);	// �傫�����f

	// ����\����s�����ɂ���
	D3DXCOLOR colCont = m_pControl->GetColor();	// ����\���̐F
	colCont.a = 1.0f;				// �s�����ɂ���
	m_pControl->SetColor(colCont);	// �F���f

	// �\���J�n��Ԃɂ���
	m_state = STATE_SCALEIN;
}

//============================================================
//	��\���ݒ菈��
//============================================================
void CPlayControl::SetHide(const bool bFlash)
{
	if (bFlash)
	{ // �t���b�V����ON�̏ꍇ

		// �p�^�[����i�߂�
		m_pControl->SetPattern(1);
	}

	// �\���I����Ԃɂ���
	m_state = STATE_FADEOUT;
}

//============================================================
//	�\���擾����
//============================================================
bool CPlayControl::IsDisp(void)
{
	// �\���󋵂�ݒ�
	bool bDisp = (m_state == STATE_DISP || m_state == STATE_SCALEIN);

	// �\���󋵂�Ԃ�
	return bDisp;
}

//============================================================
//	��������
//============================================================
CPlayControl *CPlayControl::Create
(
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rSize	// �傫��
)
{
	// �v���C����̐���
	CPlayControl *pPlayControl = new CPlayControl;
	if (pPlayControl == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �v���C����̏�����
		if (FAILED(pPlayControl->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �v���C����̔j��
			SAFE_DELETE(pPlayControl);
			return nullptr;
		}

		// �ʒu��ݒ�
		pPlayControl->m_pControl->SetVec3Position(rPos);

		// �傫����ݒ�
		pPlayControl->m_pControl->SetVec3Sizing(rSize);
		pPlayControl->m_originSize = rSize;

		// �m�ۂ����A�h���X��Ԃ�
		return pPlayControl;
	}
}

//============================================================
//	�j������
//============================================================
void CPlayControl::Release(CPlayControl *&prPlayControl)
{
	// �v���C����̏I��
	assert(prPlayControl != nullptr);
	prPlayControl->Uninit();

	// �������J��
	SAFE_DELETE(prPlayControl);
}
