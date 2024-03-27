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
	const int PRIORITY = 7;	// ���[�f�B���O�̗D�揇��

#ifdef _DEBUG

	const int MIN_LOAD = 0;	// �ŒჍ�[�h�J��Ԃ���

#else	// NDEBUG

	const int MIN_LOAD = 3;	// �ŒჍ�[�h�J��Ԃ���

#endif	// _DEBUG
}

//************************************************************
//	�e�N���X [CLoading] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CLoading::CLoading() :
	m_pLoad		(nullptr),	// ���[�h��ʏ��
	m_state	(LOAD_NONE)	// ���[�h���
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
	m_pLoad = nullptr;			// ���[�h��ʏ��
	m_state = LOAD_NONE;	// ���[�h���

	// ���[�h��ʂ̐���
	m_pLoad = CAnim2D::Create
	( // ����
		12,
		4,
		SCREEN_CENT,	// �ʒu
		SCREEN_SIZE		// �傫��
	);
	if (m_pLoad == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �D�揇�ʂ̐ݒ�
	m_pLoad->SetPriority(PRIORITY);

	// �e�N�X�`����o�^�E����
	m_pLoad->BindTexture("data\\TEXTURE\\loading001.tga");

	// ���x�����G�t�F�N�g�ɂ���
	m_pLoad->SetLabel(CObject::LABEL_EFFECT);

	// �ύX�t���[����ݒ�
	m_pLoad->SetCounter(2);

	// ���x�����X�N���[���ɐݒ�
	m_pLoad->SetLabel(CObject::LABEL_NONE);

	// �����X�V�E�����`���OFF�ɂ���
	m_pLoad->SetEnableUpdate(false);
	m_pLoad->SetEnableDraw(false);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CLoading::Uninit(void)
{
	if (m_funcLoad.joinable())
	{
		m_funcLoad.join();
		//m_funcLoad.detach();	// TODO�F�f�^�b�`���Ă������͑����B�̂ɔj�����Ă�
	}

	// TODO�F�ق�Ƃ͎g��Ȃ��摜�ǂݍ��݂����邩�炿���ƌ�ŏ����āI

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

		switch (m_state)
		{ // ���[�h��Ԃ��Ƃ̏���
		case LOAD_UPDATE:	// ���[�h�X�V���

			if (m_bEnd)
			{ // �������������I�������ꍇ

				// ���[�h���I������ꍇ
				if (m_pLoad->GetPattern() != 0) { break; }
				if (m_pLoad->GetLoopAnimation() < MIN_LOAD) { break; }

				// ���[�h��ʂ��\��
				m_pLoad->SetEnableDraw(false);

				// ���[�h�I����Ԃɂ���
				m_state = LOAD_END;
			}

			break;

		case LOAD_END:	// ���[�h�I�����

			m_state = LOAD_NONE;

			break;

		default:	// ��O����
			assert(false);
			break;
		}

		if (!m_pLoad->IsUpdate())
		{ // �����X�V��ON�̏ꍇ

			// ���[�h��ʂ̍X�V
			m_pLoad->Update();
		}
	}
}

//============================================================
//	�`�揈��
//============================================================
void CLoading::Draw(void)
{
	// �����`�悪OFF�̏ꍇ������
	if (!m_pLoad->IsDraw()) { return; }

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

		// ���[�h����������
		m_bEnd = false;

		// �X���b�h�Ɉ����̊֐���ݒ�
		m_funcLoad = std::thread(funcLoad, &m_bEnd);

		// ���[�h��ʂ�\��
		m_pLoad->SetEnableDraw(true);
		m_pLoad->SetPattern(0);

		// ���[�h�X�V��Ԃɂ���
		m_state = LOAD_UPDATE;
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
