//============================================================
//
//	�Q�[����ʏ��� [sceneGame.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "sceneGame.h"
#include "manager.h"
#include "sound.h"
#include "camera.h"

#include "gameManager.h"
#include "pause.h"
#include "edit.h"

#include "stage.h"
#include "player.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{

}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CGameManager	*CSceneGame::m_pGameManager	= nullptr;	// �Q�[���}�l�[�W���[
CPause			*CSceneGame::m_pPause		= nullptr;	// �|�[�Y
CEdit			*CSceneGame::m_pEdit		= nullptr;	// �G�f�B�b�g

//************************************************************
//	�q�N���X [CSceneGame] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CSceneGame::CSceneGame(const EMode mode) : CScene(mode)
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CSceneGame::~CSceneGame()
{

}

//============================================================
//	����������
//============================================================
HRESULT CSceneGame::Init(void)
{
	//--------------------------------------------------------
	//	��������
	//--------------------------------------------------------
	// �V�[���̏�����
	CScene::Init();	// �X�e�[�W�E�v���C���[�̐���

	// �G�f�B�b�g�̐���
	m_pEdit = CEdit::Create(CEdit::MODE_PLAY);
	if (m_pEdit == nullptr)
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �|�[�Y�̐���
	m_pPause = CPause::Create();
	if (m_pPause == nullptr)
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �Q�[���}�l�[�W���[�̐���
	m_pGameManager = CGameManager::Create();
	if (m_pGameManager == nullptr)
	{ // ��g�p���̏ꍇ

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
void CSceneGame::Uninit(void)
{
	// �G�f�B�b�g�̔j��
	SAFE_REF_RELEASE(m_pEdit);

	// �|�[�Y�̔j��
	SAFE_REF_RELEASE(m_pPause);

	// �Q�[���}�l�[�W���[�̔j��
	SAFE_REF_RELEASE(m_pGameManager);

	// �V�[���̏I��
	CScene::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CSceneGame::Update(void)
{
	// �G�f�B�b�g�̍X�V
	assert(m_pEdit != nullptr);
	m_pEdit->DrawEditControl();
	m_pEdit->DrawEditData();
	m_pEdit->Update();

	// �|�[�Y�̍X�V
	assert(m_pPause != nullptr);
	m_pPause->Update();

	// �Q�[���}�l�[�W���[�̍X�V
	assert(m_pGameManager != nullptr);
	m_pGameManager->Update();

	if (!m_pPause->IsPause() && m_pEdit->GetMode() == CEdit::MODE_PLAY)
	{ // �|�[�Y���ł͂Ȃ����A�v���C��Ԃ̏ꍇ

		// �V�[���̍X�V
		CScene::Update();
	}

#ifdef _DEBUG

	else
	{ // �|�[�Y���̏ꍇ

		if (GET_MANAGER->GetCamera()->GetState() == CCamera::STATE_CONTROL)
		{ // �J�����������Ԃ̏ꍇ

			// �J�����̍X�V
			GET_MANAGER->GetCamera()->Update();
		}
	}

#endif	// _DEBUG

}

//============================================================
//	�`�揈��
//============================================================
void CSceneGame::Draw(void)
{

}

//============================================================
//	�Q�[���}�l�[�W���[�擾����
//============================================================
CGameManager *CSceneGame::GetGameManager(void)
{
	// �C���X�^���X���g�p
	assert(m_pGameManager != nullptr);

	// �Q�[���}�l�[�W���[�̃|�C���^��Ԃ�
	return m_pGameManager;
}

//============================================================
//	�|�[�Y�擾����
//============================================================
CPause *CSceneGame::GetPause(void)
{
	// �C���X�^���X���g�p
	assert(m_pPause != nullptr);

	// �|�[�Y�̃|�C���^��Ԃ�
	return m_pPause;
}

//============================================================
//	�G�f�B�b�g�ݒ菈��
//============================================================
void CSceneGame::SetEdit(CEdit *pEdit)
{
	// �C���X�^���X���g�p
	assert(pEdit != nullptr);

	// �G�f�B�b�g�̃|�C���^��ݒ�
	m_pEdit = pEdit;
}

//============================================================
//	�G�f�B�b�g�擾����
//============================================================
CEdit *CSceneGame::GetEdit(void)
{
	// �C���X�^���X���g�p
	assert(m_pEdit != nullptr);

	// �G�f�B�b�g�̃|�C���^��Ԃ�
	return m_pEdit;
}
