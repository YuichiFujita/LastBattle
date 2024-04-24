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
#include "timerUI.h"
#include "retentionManager.h"
#include "modelFont.h"
#include "skip.h"
#include "camera.h"
#include "player.h"
#include "enemy.h"
#include "enemyBossDragon.h"
#include "multiModel.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const D3DXVECTOR3 POS_NAME	 = D3DXVECTOR3(0.0f, 60.0f, 400.0f);	// ���O�̕\���ʒu
	const D3DXVECTOR3 POS_SKIP	 = D3DXVECTOR3(1092.0f, 673.0f, 0.0f);	// �X�L�b�v����̕\���ʒu
	const D3DXVECTOR3 SIZE_SKIP	 = D3DXVECTOR3(381.0f, 77.0f, 0.0f);	// �X�L�b�v����̕\���傫��
	const int CHANGE_UI_PRIORITY = 5;	// �V�l�}�X�R�[�v�I������UI�D�揇��
	const int GAMEEND_WAIT_FRAME = 180;	// ���U���g��ʂւ̑J�ڗ]�C�t���[��
}

//************************************************************
//	�e�N���X [CGameManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CGameManager::CGameManager() :
	m_pBossName		(nullptr),		// �{�X�̖��O���f�����
	m_pSkip			(nullptr),		// �X�L�b�v���
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

	// �����o�ϐ���������
	m_pBossName	 = nullptr;			// �{�X�̖��O���f�����
	m_pSkip		 = nullptr;			// �X�L�b�v���
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

	// �X�L�b�v���̐���
	m_pSkip = CSkip::Create
	( // ����
		POS_SKIP,	// �ʒu
		SIZE_SKIP	// �傫��
	);
	if (m_pSkip == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �X�R�[�v�C��������
	pScope->SetScopeIn();

	// �v���C���[���o��������
	pPlayer->SetSpawn();

	// �Q�[��UI�̎����`���OFF�ɂ���
	SetDrawGameUI(false);

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

	// �X�L�b�v�̔j��
	SAFE_REF_RELEASE(m_pSkip);
}

//============================================================
//	�X�V����
//============================================================
void CGameManager::Update(void)
{
	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_NONE:
	case STATE_NORMAL:
	case STATE_STAGING:
		break;

	case STATE_START:

		// �J�n��Ԃ̍X�V
		UpdateStart();

		break;

	default:	// ��O����
		assert(false);
		break;
	}
}

//============================================================
//	��Ԑݒ菈��
//============================================================
void CGameManager::SetState(const EState state)
{
	// ��Ԃ�ݒ�
	m_state = state;
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
//	�Q�[����ʂ�UI�`��ݒ菈��
//============================================================
void CGameManager::SetDrawGameUI(const bool bDraw)
{
	// �|�C���^��錾
	CTimerUI *pTimer = CSceneGame::GetTimerUI();	// �^�C�}�[�}�l�[�W���[�̏��
	CPlayer *pPlayer = CScene::GetPlayer();			// �v���C���[�̏��
	CEnemy *pBoss = CScene::GetBoss();				// �{�X�̏��

	// �����`���ݒ�
	pTimer->SetEnableDraw(bDraw);		// �^�C�}�[
	pPlayer->SetEnableDrawUI(bDraw);	// �v���C���[�֘AUI
	pBoss->SetEnableDrawUI(bDraw);		// �{�X�֘AUI
}

//============================================================
//	���U���g��ʑJ�ڏ���
//============================================================
void CGameManager::TransitionResult(const CRetentionManager::EWin win)
{
	// �^�C�}�[�̌v���I��
	CSceneGame::GetTimerUI()->End();

	// ���U���g����ۑ�
	GET_RETENTION->SetResult(win, CSceneGame::GetTimerUI()->Get());

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
	// �t�F�[�h���̏ꍇ������
	if (GET_MANAGER->GetFade()->GetState() != CFade::FADE_NONE) { return; }

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

			// �J�n���o�̏I��
			EndStart();

			// �I����Ԃɂ���
			m_startState = START_END;
		}

		break;
	}
	case START_END:	// �I�����
	{
		CCinemaScope *pScope = CSceneGame::GetCinemaScope();	// �V�l�}�X�R�[�v�̏��

		if (pScope->GetState() == CCinemaScope::STATE_NONE)
		{ // ���o���I�������ꍇ

			CTimerUI *pTimer  = CSceneGame::GetTimerUI();	// �^�C�}�[�}�l�[�W���[�̏��

			// UI�D�揇�ʂ��V�l�}�X�R�[�v����ɂ���
			pPlayer->SetLifePriority(CHANGE_UI_PRIORITY);
			pBoss->SetLifePriority(CHANGE_UI_PRIORITY);
			pTimer->SetPriority(CHANGE_UI_PRIORITY);

			// �ʏ��Ԃɂ���
			m_state = STATE_NORMAL;
		}

		break;
	}
	default:
		assert(false);
		break;
	}

	// �J�n���o�̃X�L�b�v
	SkipStart();

	// �{�X�̖��O���f���̍X�V
	if (m_pBossName != nullptr)
	m_pBossName->Update();

	// �X�L�b�v���̍X�V
	if (m_pSkip != nullptr)
	m_pSkip->Update();
}

//============================================================
//	�J�n���o�̏I������
//============================================================
void CGameManager::EndStart(void)
{
	CPlayer  *pPlayer = CScene::GetPlayer();		// �v���C���[���
	CEnemy   *pBoss   = CScene::GetBoss();			// �{�X���
	CCamera  *pCamera = GET_MANAGER->GetCamera();	// �J�������
	CTimerUI *pTimer  = CSceneGame::GetTimerUI();	// �^�C�}�[�}�l�[�W���[�̏��
	CCinemaScope *pScope = CSceneGame::GetCinemaScope();	// �V�l�}�X�R�[�v�̏��

	// �ʏ��Ԃ̐ݒ�E������
	pPlayer->InitNormal();	// �v���C���[
	pBoss->InitNormal();	// �{�X

	// �{�X�̖��O���f���̎����`���OFF�ɂ���
	m_pBossName->SetEnableDraw(false);

	// �Q�[��UI�̎����`���ON�ɂ���
	SetDrawGameUI(true);

	// �X�R�[�v�A�E�g������
	pScope->SetScopeOut();

	// �J������Ǐ]��Ԃɐݒ�
	pCamera->SetState(CCamera::STATE_FOLLOW);
	pCamera->SetDestFollow();	// �J�����ڕW�ʒu�̏�����

	// �{�X�̖��O���f���̏I��
	SAFE_UNINIT(m_pBossName);

	// �X�L�b�v�̔j��
	SAFE_REF_RELEASE(m_pSkip);

	// �^�C�}�[�̌v���J�n
	pTimer->Start();
}

//============================================================
//	�J�n���o�̃X�L�b�v����
//============================================================
void CGameManager::SkipStart(void)
{
	// �J�n���o���I���ς݂̏ꍇ������
	if (m_startState == START_END) { return; }

	CInputKeyboard	*pKeyboard	= GET_INPUTKEY;	// �L�[�{�[�h
	CInputPad		*pPad		= GET_INPUTPAD;	// �p�b�h

	if (pKeyboard->IsAnyTrigger() || pPad->IsAnyTrigger())
	{ // ����̑��삪�s��ꂽ�ꍇ

		// �X�L�b�v�����\��������
		m_pSkip->SetDisp();
	}

	if (pKeyboard->IsTrigger(DIK_RETURN) || pPad->IsTrigger(CInputPad::KEY_START))
	{ // �X�L�b�v���삪�s��ꂽ�ꍇ

		if (m_pSkip->IsDisp())
		{ // �X�L�b�v���삪�\������Ă���ꍇ

			// �J�n���o���I����Ԃɂ���
			m_startState = START_END;

			// �J�n���o�̏I��
			EndStart();

			// �g���K�[���������� (�J�n����̃|�[�Y�h�~)
			pPad->InitTrigger();
		}
	}
}
