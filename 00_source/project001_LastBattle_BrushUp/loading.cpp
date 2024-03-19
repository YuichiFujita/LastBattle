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
#include "object.h"
#include "anim2D.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int PRIORITY = 7;	// ���[�f�B���O�̗D�揇��
}

//************************************************************
//	�e�N���X [CLoading] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CLoading::CLoading() :
	m_pLoad		(nullptr),	// ���[�h��ʏ��
	m_stateLoad	(LOAD_NONE)	// ���[�h���
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
	m_stateLoad = LOAD_NONE;	// ���[�h���

	//// ���[�h��ʂ̐���
	//m_pLoad = CAnim2D::Create
	//( // ����
	//	20,
	//	3,
	//	SCREEN_CENT,	// �ʒu
	//	SCREEN_SIZE		// �傫��
	//);
	//if (m_pLoad == nullptr)
	//{ // �����Ɏ��s�����ꍇ

	//	// ���s��Ԃ�
	//	assert(false);
	//	return E_FAIL;
	//}

	//// �D�揇�ʂ̐ݒ�
	//m_pLoad->SetPriority(PRIORITY);

	//// �e�N�X�`����o�^�E����
	//m_pLoad->BindTexture("data\\TEXTURE\\loading000.png");

	//// ���x�����G�t�F�N�g�ɂ���
	//m_pLoad->SetLabel(CObject::LABEL_EFFECT);

	//// �ύX�t���[����ݒ�
	//m_pLoad->SetCounter(1);

#if 0
	// ���[�h�̐ݒ�
	GET_MANAGER->SetMode(m_modeNext);
#endif

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CLoading::Uninit(void)
{
	// ���[�h��ʂ̏I��
	SAFE_UNINIT(m_pLoad);
}

//============================================================
//	�X�V����
//============================================================
void CLoading::Update(void)
{
	if (m_stateLoad != LOAD_NONE)
	{ // �������Ȃ���Ԃł͂Ȃ��ꍇ

		switch (m_stateLoad)
		{ // ���[�h��Ԃ��Ƃ̏���
		case LOAD_UPDATE:	// ���[�h�X�V���

			if (!m_funcInit.joinable())
			{ // �������������I�������ꍇ

				// ���[�h��ʂ��\��
				//m_pLoad->SetEnableDraw(false);

				// ���[�h�I����Ԃɂ���
				m_stateLoad = LOAD_END;
			}

			break;

		case LOAD_END:	// ���[�h�I�����

			m_stateLoad = LOAD_NONE;

			break;

		default:	// ��O����
			assert(false);
			break;
		}

		// ���[�h��ʂ̍X�V
		//m_pLoad->Update();
	}
}

//============================================================
//	���[�h�ݒ菈��
//============================================================
void CLoading::Set(AFuncInit *pFuncInit)
{
	if (m_stateLoad == LOAD_NONE)
	{ // ���[�f�B���O���s���Ă��Ȃ��ꍇ

		//// �֐����Ȃ��ꍇ�G���[
		//if (pFuncInit == nullptr) { assert(false);  return; }

		//// �X���b�h�Ɉ����̊֐���ݒ�
		//m_funcInit = std::thread(*pFuncInit);

		// ���[�h��ʂ�\��
		//m_pLoad->SetEnableDraw(true);
		//m_pLoad->SetPattern(0);

		// ���[�h�X�V��Ԃɂ���
		m_stateLoad = LOAD_UPDATE;
	}
}

//============================================================
//	���[�h��Ԏ擾����
//============================================================
CLoading::ELoading CLoading::GetState(void) const
{
	// ���݂̃��[�h��Ԃ�Ԃ�
	return m_stateLoad;
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
