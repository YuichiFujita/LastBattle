//============================================================
//
//	�Q�[���}�l�[�W���[���� [gameManager.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "gameManager.h"
#include "manager.h"
#include "fade.h"
#include "scene.h"
#include "sceneGame.h"
#include "cinemaScope.h"
#include "timerManager.h"
#include "retentionManager.h"
#include "camera.h"
#include "player.h"
#include "enemy.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int GAMEEND_WAIT_FRAME = 120;	// ���U���g��ʂւ̑J�ڗ]�C�t���[��
}

//************************************************************
//	�e�N���X [CGameManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CGameManager::CGameManager() :
	m_state			(STATE_NONE),		// ���
	m_startState	(START_INIT_SCOPE)	// �J�n���
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CGameManager::~CGameManager()
{

}

//============================================================
//	����������
//============================================================
HRESULT CGameManager::Init(void)
{
	// �����o�ϐ���������
	m_state		 = STATE_START;			// ���
	m_startState = START_INIT_SCOPE;	// �J�n���

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CGameManager::Uninit(void)
{

}

//============================================================
//	�X�V����
//============================================================
void CGameManager::Update(void)
{
	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_NONE:
	case STATE_END:
		break;

	case STATE_START:

		// �J�n��Ԃ̍X�V
		UpdateStart();

		break;

	case STATE_NORMAL:

		if (CSceneGame::GetTimerManager()->GetState() == CTimerManager::STATE_END)
		{ // �v�����I�����Ă����ꍇ

			// ���U���g��ʂɑJ�ڂ�����
			TransitionResult(CRetentionManager::WIN_FAILED);
		}

		break;

	default:	// ��O����
		assert(false);
		break;
	}
}

//============================================================
//	��Ԏ擾����
//============================================================
CGameManager::EState CGameManager::GetState(void) const
{
	// ��Ԃ�Ԃ�
	return m_state;
}

//============================================================
//	���U���g��ʑJ�ڏ���
//============================================================
void CGameManager::TransitionResult(const CRetentionManager::EWin win)
{
	// �I����Ԃɂ���
	m_state = STATE_END;

	// ���U���g����ۑ�
	GET_RETENTION->SetResult(win, CSceneGame::GetTimerManager()->Get());

	// �^�C�}�[�̌v���I��
	CSceneGame::GetTimerManager()->End();

	// ���U���g��ʂɑJ��
	GET_MANAGER->SetScene(CScene::MODE_RESULT, GAMEEND_WAIT_FRAME);
}

//============================================================
//	��������
//============================================================
CGameManager *CGameManager::Create(void)
{
	// �Q�[���}�l�[�W���[�̐���
	CGameManager *pGameManager = new CGameManager;
	if (pGameManager == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �Q�[���}�l�[�W���[�̏�����
		if (FAILED(pGameManager->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �Q�[���}�l�[�W���[�̔j��
			SAFE_DELETE(pGameManager);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pGameManager;
	}
}

//============================================================
//	�j������
//============================================================
void CGameManager::Release(CGameManager *&prGameManager)
{
	// �Q�[���}�l�[�W���[�̏I��
	assert(prGameManager != nullptr);
	prGameManager->Uninit();

	// �������J��
	SAFE_DELETE(prGameManager);
}

//============================================================
//	�J�n��Ԃ̍X�V����
//============================================================
void CGameManager::UpdateStart(void)
{
	if (GET_MANAGER->GetFade()->GetState() != CFade::FADE_NONE) { return; }	// �t�F�[�h���̏ꍇ������

	CPlayer *pPlayer = CScene::GetPlayer();	// �v���C���[���
	CEnemy  *pBoss   = CScene::GetBoss();	// �{�X���

	switch (m_startState)
	{ // �J�n��Ԃ��Ƃ̏���
	case START_INIT_SCOPE:	// �V�l�}�X�R�[�v�C�����������
	{
		// �X�R�[�v�C��������
		CSceneGame::GetCinemaScope()->SetScopeIn();

		// �V�l�}�X�R�[�v�C����Ԃɂ���
		m_startState = START_SCOPE;

		// �����𔲂����ɉ��̏����𑱍s
	}
	case START_SCOPE:	// �V�l�}�X�R�[�v�C�����
	{
		if (CSceneGame::GetCinemaScope()->GetState() == CCinemaScope::STATE_NONE)
		{ // �\�������������ꍇ

			// �v���C���[���o��������
			pPlayer->SetSpawn();

			// �v���C���[�X�|�[����Ԃɂ���
			m_startState = START_PLAYER;
		}

		break;
	}
	case START_PLAYER:	// �v���C���[�X�|�[�����
	{
		if (pPlayer->GetState() == CPlayer::STATE_NONE)
		{ // �X�|�[�����o���I�������ꍇ

			// �{�X���o��������
			pBoss->SetState(CEnemy::STATE_SPAWN);

			// �{�X�X�|�[����Ԃɂ���
			m_startState = START_BOSS;
		}

		break;
	}
	case START_BOSS:	// �{�X�X�|�[�����
	{
		if (pBoss->GetState() == CEnemy::STATE_NONE)
		{ // �X�|�[�����o���I�������ꍇ

			// �I����Ԃɂ���
			m_startState = START_END;
		}

		break;
	}
	case START_END:	// �I�����
	{
		// �ʏ��Ԃɂ���
		pPlayer->SetState(CPlayer::STATE_NORMAL);	// �v���C���[
		pBoss->SetState(CEnemy::STATE_NORMAL);		// �{�X

		// �X�R�[�v�A�E�g������
		CSceneGame::GetCinemaScope()->SetScopeOut();

		// �J������Ǐ]��Ԃɐݒ�
		GET_MANAGER->GetCamera()->SetState(CCamera::STATE_FOLLOW);
		GET_MANAGER->GetCamera()->SetDestFollow();	// �J�����ڕW�ʒu�̏�����

		// �^�C�}�[�̌v���J�n
		CSceneGame::GetTimerManager()->Start();

		// �ʏ��Ԃɂ���
		m_state = STATE_NORMAL;

		break;
	}
	default:
		assert(false);
		break;
	}
}
