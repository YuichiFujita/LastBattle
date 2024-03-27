//============================================================
//
//	���[�f�B���O���� [loading.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "loading.h"
#include "manager.h"
#include "renderer.h"
#include "object.h"
#include "anim2D.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char* TEXTURE_FILE = "data\\TEXTURE\\loading000.tga";	// ���[�h��ʃe�N�X�`��

	const POSGRID2 PTRN = POSGRID2(12, 4);	// �e�N�X�`��������
	const int	PRIORITY	= 7;			// ���[�f�B���O�̗D�揇��
	const int	ANIM_WAIT	= 2;			// �A�j���[�V�����̕ύX�t���[��
	const float	FADE_LEVEL	= 0.05f;		// �t�F�[�h�̃��l�̉�����

#ifdef _DEBUG

	const int MIN_LOAD = 0;	// �ŒჍ�[�h�J��Ԃ���

#else	// NDEBUG

	const int MIN_LOAD = 2;	// �ŒჍ�[�h�J��Ԃ���

#endif	// _DEBUG
}

//************************************************************
//	�e�N���X [CLoading] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CLoading::CLoading() :
	m_pLoad	(nullptr),		// ���[�h��ʏ��
	m_state	(LOAD_NONE),	// ���[�h���
	m_bEnd	(false)			// ���[�h�I����
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CLoading::~CLoading()
{

}

//============================================================
//	����������
//============================================================
HRESULT CLoading::Init(void)
{
	// �����o�ϐ���������
	m_pLoad	= nullptr;		// ���[�h��ʏ��
	m_state	= LOAD_NONE;	// ���[�h���
	m_bEnd	= false;		// ���[�h�I����

	// ���[�h��ʂ̐���
	m_pLoad = CAnim2D::Create
	( // ����
		PTRN.x,			// �e�N�X�`����������
		PTRN.y,			// �e�N�X�`���c������
		SCREEN_CENT,	// �ʒu
		SCREEN_SIZE,	// �傫��
		VEC3_ZERO,		// ����
		XCOL_AWHITE		// �F
	);
	if (m_pLoad == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�N�X�`����o�^�E����
	m_pLoad->BindTexture(TEXTURE_FILE);

	// �D�揇�ʂ̐ݒ�
	m_pLoad->SetPriority(PRIORITY);

	// ���x���w��Ȃ��ɐݒ�
	m_pLoad->SetLabel(CObject::LABEL_NONE);

	// �ύX�t���[���̐ݒ�
	m_pLoad->SetCounter(ANIM_WAIT);

	// ���[�h�̃A�j���[�V�������~
	m_pLoad->SetEnableStop(true);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CLoading::Uninit(void)
{
	if (m_funcLoad.joinable())
	{ // �Ǎ��֐��̏������I�����Ă��Ȃ����ꍇ

		// �����I����ҋ@
		m_funcLoad.join();
		
		// �����������ƃ��C���X���b�h��
		// ���̔j����ɓǍ����s���������j�󂷂�
	}

	// ���[�h��ʂ̏I��
	SAFE_UNINIT(m_pLoad);
}

//============================================================
//	�X�V����
//============================================================
void CLoading::Update(void)
{
	if (m_state != LOAD_NONE)
	{ // �������Ȃ���Ԃł͂Ȃ��ꍇ

		D3DXCOLOR colLoad = m_pLoad->GetColor();	// �t�F�[�h�F
		switch (m_state)
		{ // ���[�h��Ԃ��Ƃ̏���
		case LOAD_FADEOUT:	// ���[�h�̕\���J�n���

			// ���l�����Z
			colLoad.a += FADE_LEVEL;
			if (colLoad.a >= 1.0f)
			{ // ���l�� 1.0f ���������ꍇ

				// ���l��␳
				colLoad.a = 1.0f;

				// ���[�h�̃A�j���[�V�������Đ�
				m_pLoad->SetEnableStop(false);

				// ���[�h�X�V��Ԃɂ���
				m_state = LOAD_UPDATE;
			}

			break;

		case LOAD_UPDATE:	// ���[�h�X�V���

			if (m_bEnd)
			{ // �������������I�������ꍇ

#ifdef NDEBUG
				// ���[�h�̃L���������ꍇ������
				if (m_pLoad->GetPattern() != 0) { break; }
#endif	// NDEBUG

				// �ŒჍ�[�h�񐔖����̏ꍇ������
				if (m_pLoad->GetLoopAnimation() < MIN_LOAD) { break; }

				// ���[�h�̃A�j���[�V�������~
				m_pLoad->SetEnableStop(true);

				// ���[�h�̕\���I����Ԃɂ���
				m_state = LOAD_FADEIN;
			}

			break;

		case LOAD_FADEIN:	// ���[�h�̕\���I�����

			// ���l�����Z
			colLoad.a -= FADE_LEVEL;
			if (colLoad.a <= 0.0f)
			{ // ���l�� 0.0f����������ꍇ

				// ���l��␳
				colLoad.a = 0.0f;

				// �������Ȃ���Ԃɂ���
				m_state = LOAD_NONE;
			}

			break;

		default:	// ��O����
			assert(false);
			break;
		}

		// �F�̍X�V
		m_pLoad->SetColor(colLoad);

		// ���[�h��ʂ̍X�V
		m_pLoad->Update();
	}
}

//============================================================
//	�`�揈��
//============================================================
void CLoading::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// �f�o�C�X�̃|�C���^

	// �T���v���[�X�e�[�g��ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);	// U�����̃��b�s���O�𖳌���
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);	// V�����̃��b�s���O�𖳌���

	// ���[�h��ʂ̕`��
	m_pLoad->Draw();

	// �T���v���[�X�e�[�g��ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// U�����̃��b�s���O��L����
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// V�����̃��b�s���O��L����
}

//============================================================
//	���[�h�J�n�ݒ菈��
//============================================================
void CLoading::Set(std::function<HRESULT(bool*)> funcLoad)
{
	if (m_state == LOAD_NONE)
	{ // ���[�f�B���O���s���Ă��Ȃ��ꍇ

		// ���[�h�������ɂ���
		m_bEnd = false;

		// �X���b�h�Ɉ����̊֐���ݒ�
		m_funcLoad = std::thread(funcLoad, &m_bEnd);

		// ���[�h��ʂ̃p�^�[����擪�ɂ���
		m_pLoad->SetPattern(0);

		// ���[�h�̕\���J�n��Ԃɂ���
		m_state = LOAD_FADEOUT;
	}
}

//============================================================
//	��������
//============================================================
CLoading *CLoading::Create(void)
{
	// ���[�f�B���O�̐���
	CLoading *pLoading = new CLoading;
	if (pLoading == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// ���[�f�B���O�̏�����
		if (FAILED(pLoading->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ���[�f�B���O�̔j��
			SAFE_DELETE(pLoading);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pLoading;
	}
}

//============================================================
//	�j������
//============================================================
void CLoading::Release(CLoading *&prLoading)
{
	// ���[�f�B���O�̏I��
	assert(prLoading != nullptr);
	prLoading->Uninit();

	// �������J��
	SAFE_DELETE(prLoading);
}
