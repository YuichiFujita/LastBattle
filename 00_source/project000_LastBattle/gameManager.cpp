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
#include "modelFont.h"
#include "camera.h"
#include "player.h"
#include "enemy.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const D3DXVECTOR3 POS_NAME = D3DXVECTOR3(0.0f, 100.0f, 400.0f);	// ���O�̕\���ʒu

	const int GAMEEND_WAIT_FRAME = 120;	// ���U���g��ʂւ̑J�ڗ]�C�t���[��
}

//************************************************************
//	�e�N���X [CGameManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CGameManager::CGameManager() :
	m_pBossName		(nullptr),		// �{�X�̖��O���f�����
	m_state			(STATE_NONE),	// ���
	m_startState	(START_PLAYER)	// �J�n���
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
	// �|�C���^��錾
	CCinemaScope *pScope = CSceneGame::GetCinemaScope();	// �V�l�}�X�R�[�v�̏��
	CPlayer *pPlayer = CScene::GetPlayer();	// �v���C���[�̏��
	CEnemy *pBoss = CScene::GetBoss();		// �{�X�̏��

	// �����o�ϐ���������
	m_pBossName	 = nullptr;			// �{�X�̖��O���f�����
	m_state		 = STATE_START;		// ���
	m_startState = START_PLAYER;	// �J�n���

	// �{�X�̖��O���f���̐���
	m_pBossName = CModelFont::Create
	( // ����
		CModelFont::TYPE_BOSS_NAME,	// ���
		POS_NAME					// �ʒu
	);
	if (m_pBossName == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �����`���OFF�ɂ���
	m_pBossName->SetEnableDraw(false);

	// �X�R�[�v�C��������
	pScope->SetScopeIn();

	// �v���C���[���o��������
	pPlayer->SetSpawn();

	// UI�̎����`���OFF�ɂ���
	pPlayer->SetEnableDrawUI(false);	// �v���C���[�֘AUI
	pBoss->SetEnableDrawUI(false);		// �{�X�֘AUI

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CGameManager::Uninit(void)
{
	// �{�X�̖��O���f���̏I��
	SAFE_UNINIT(m_pBossName);
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

		// �{�X�̖��O���f���̍X�V
		m_pBossName->Update();

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
	case START_PLAYER:	// �v���C���[�X�|�[�����
	{
		if (pPlayer->GetState() == CPlayer::STATE_NONE)
		{ // �X�|�[�����o���I�������ꍇ

			// �{�X���o��������
			pBoss->SetState(CEnemy::STATE_SPAWN);

			// �{�X�̖��O���f���̎����`���ON�ɂ���
			m_pBossName->SetEnableDraw(true);

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

		// �{�X�̖��O���f���̎����`���OFF�ɂ���
		m_pBossName->SetEnableDraw(false);

		// UI�̎����`���OFF�ɂ���
		pPlayer->SetEnableDrawUI(true);	// �v���C���[�֘AUI
		pBoss->SetEnableDrawUI(true);	// �{�X�֘AUI

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
