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
#include "timerManager.h"
#include "cinemaScope.h"
#include "pause.h"
#include "stage.h"
#include "player.h"
#include "enemy.h"
#include "sword.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	namespace timerUI
	{
#ifdef _DEBUG

		const int TIME_LIMIT = 120;	// ��������

#else	// NDEBUG

		const int TIME_LIMIT = 240;	// ��������

#endif	// _DEBUG

		const D3DXVECTOR3 POS			= D3DXVECTOR3(710.0f, 50.0f, 0.0f);	// �^�C�}�[�ʒu
		const D3DXVECTOR3 VAL_SIZE		= D3DXVECTOR3(72.0f, 96.0f, 0.0f);	// �^�C�}�[�����傫��
		const D3DXVECTOR3 PART_SIZE		= D3DXVECTOR3(42.0f, 96.0f, 0.0f);	// �^�C�}�[��؂�傫��
		const D3DXVECTOR3 VAL_SPACE		= D3DXVECTOR3(VAL_SIZE.x * 0.85f, 0.0f, 0.0f);	// �^�C�}�[������
		const D3DXVECTOR3 PART_SPACE	= D3DXVECTOR3(PART_SIZE.x * 0.85f, 0.0f, 0.0f);	// �^�C�}�[��؂��
	}
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CGameManager	*CSceneGame::m_pGameManager		= nullptr;	// �Q�[���}�l�[�W���[
CTimerManager	*CSceneGame::m_pTimerManager	= nullptr;	// �^�C�}�[�}�l�[�W���[
CCinemaScope	*CSceneGame::m_pCinemaScope		= nullptr;	// �V�l�}�X�R�[�v
CPause			*CSceneGame::m_pPause			= nullptr;	// �|�[�Y

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
	// �G�̑S�Z�b�g�A�b�v�Ǎ�
	CEnemy::LoadAllSetup();

	// ���̃Z�b�g�A�b�v�Ǎ�
	CSword::LoadSetup();

	// �^�C�}�[�}�l�[�W���[�̐���
	m_pTimerManager = CTimerManager::Create
	( // ����
		CTimerManager::TIME_SEC,	// �ݒ�^�C��
		timerUI::TIME_LIMIT,		// ��������
		timerUI::POS,				// �ʒu
		timerUI::VAL_SIZE,			// �����̑傫��
		timerUI::PART_SIZE,			// ��؂�̑傫��
		timerUI::VAL_SPACE,			// �����̋�
		timerUI::PART_SPACE			// ��؂�̋�
	);
	if (m_pTimerManager == nullptr)
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ���S�̎����`���ON�ɂ���
	m_pTimerManager->SetEnableLogoDraw(true);

	// �V�[���̏�����
	CScene::Init();	// �X�e�[�W�E�v���C���[�̐���


	//// TODO�F�G�̐���
	//CEnemy::Create(CEnemy::TYPE_MINI_DRAGON, VEC3_ZERO + D3DXVECTOR3(100.0f, 0.0f, 0.0f), VEC3_ZERO);
	//CEnemy::Create(CEnemy::TYPE_MINI_DRAGON, VEC3_ZERO - D3DXVECTOR3(100.0f, 0.0f, 0.0f), VEC3_ZERO);
	//CEnemy::Create(CEnemy::TYPE_MINI_DRAGON, VEC3_ZERO, VEC3_ZERO);


	// �V�l�}�X�R�[�v�̐���
	m_pCinemaScope = CCinemaScope::Create();
	if (m_pCinemaScope == nullptr)
	{ // �����Ɏ��s�����ꍇ

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

	//--------------------------------------------------------
	//	�����ݒ�
	//--------------------------------------------------------
	// BGM�̍Đ�
	GET_MANAGER->GetSound()->Play(CSound::LABEL_BGM_GAME);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CSceneGame::Uninit(void)
{
	// �Q�[���}�l�[�W���[�̔j��
	SAFE_REF_RELEASE(m_pGameManager);

	// �^�C�}�[�}�l�[�W���[�̔j��
	SAFE_REF_RELEASE(m_pTimerManager);

	// �V�l�}�X�R�[�v�̔j��
	SAFE_REF_RELEASE(m_pCinemaScope);

	// �|�[�Y�̔j��
	SAFE_REF_RELEASE(m_pPause);

	// �V�[���̏I��
	CScene::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CSceneGame::Update(void)
{
	// �Q�[���}�l�[�W���[�̍X�V
	assert(m_pGameManager != nullptr);
	m_pGameManager->Update();

	// �^�C�}�[�}�l�[�W���[�̍X�V
	assert(m_pTimerManager != nullptr);
	m_pTimerManager->Update();

	// �|�[�Y�̍X�V
	assert(m_pPause != nullptr);
	m_pPause->Update();

	if (!m_pPause->IsPause())
	{ // �|�[�Y���ł͂Ȃ��ꍇ

		// �V�l�}�X�R�[�v�̍X�V
		assert(m_pCinemaScope != nullptr);
		m_pCinemaScope->Update();

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
//	�^�C�}�[�}�l�[�W���[�擾����
//============================================================
CTimerManager *CSceneGame::GetTimerManager(void)
{
	// �C���X�^���X���g�p
	assert(m_pTimerManager != nullptr);

	// �^�C�}�[�}�l�[�W���[�̃|�C���^��Ԃ�
	return m_pTimerManager;
}

//============================================================
//	�V�l�}�X�R�[�v�擾����
//============================================================
CCinemaScope *CSceneGame::GetCinemaScope(void)
{
	// �C���X�^���X���g�p
	assert(m_pCinemaScope != nullptr);

	// �V�l�}�X�R�[�v�̃|�C���^��Ԃ�
	return m_pCinemaScope;
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
