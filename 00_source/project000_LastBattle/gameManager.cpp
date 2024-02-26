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
	const D3DXVECTOR3 POS_NAME  = D3DXVECTOR3(0.0f, 100.0f, 400.0f);	// ���O�̕\���ʒu
	const D3DXVECTOR3 POS_SKIP  = D3DXVECTOR3(1030.0f, 590.0f, 0.0f);	// �X�L�b�v����̕\���ʒu
	const D3DXVECTOR3 SIZE_SKIP = D3DXVECTOR3(460.0f, 110.0f, 0.0f);	// �X�L�b�v����̕\���傫��

	const int GAMEEND_WAIT_FRAME= 120;	// ���U���g��ʂւ̑J�ڗ]�C�t���[��
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
	case STATE_END:
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
	CTimerManager *pTimer = CSceneGame::GetTimerManager();	// �^�C�}�[�}�l�[�W���[�̏��
	CPlayer *pPlayer = CScene::GetPlayer();	// �v���C���[�̏��
	CEnemy *pBoss = CScene::GetBoss();		// �{�X�̏��

	// �����`���ݒ�
	pTimer->SetEnableLogoDraw(bDraw);	// �^�C�}�[���S
	pTimer->SetEnableDraw(bDraw);		// �^�C�}�[
	pPlayer->SetEnableDrawUI(bDraw);	// �v���C���[�֘AUI
	pBoss->SetEnableDrawUI(bDraw);		// �{�X�֘AUI
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
	// �t�F�[�h���̏ꍇ������
	if (GET_MANAGER->GetFade()->GetState() != CFade::FADE_NONE) { return; }

	CPlayer *pPlayer = CScene::GetPlayer();	// �v���C���[���
	CEnemy  *pBoss   = CScene::GetBoss();	// �{�X���

	// �{�X�̖��O���f���̍X�V
	m_pBossName->Update();

	// �X�L�b�v���̍X�V
	m_pSkip->Update();

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
		// �J�n���o�̏I��
		EndStart();

		break;
	}
	default:
		assert(false);
		break;
	}

	// �J�n���o�̃X�L�b�v
	SkipStart();
}

//============================================================
//	�J�n���o�̏I������
//============================================================
void CGameManager::EndStart(void)
{
	CPlayer *pPlayer = CScene::GetPlayer();	// �v���C���[���
	CEnemy  *pBoss   = CScene::GetBoss();	// �{�X���
	CTimerManager *pTimer = CSceneGame::GetTimerManager();	// �^�C�}�[�}�l�[�W���[�̏��
	CCinemaScope *pScope  = CSceneGame::GetCinemaScope();	// �V�l�}�X�R�[�v�̏��

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
	GET_MANAGER->GetCamera()->SetState(CCamera::STATE_FOLLOW);
	GET_MANAGER->GetCamera()->SetDestFollow();	// �J�����ڕW�ʒu�̏�����

	// �{�X�̖��O���f���̏I��
	SAFE_UNINIT(m_pBossName);

	// �X�L�b�v�̔j��
	SAFE_REF_RELEASE(m_pSkip);

	// �^�C�}�[�̌v���J�n
	pTimer->Start();

	// �ʏ��Ԃɂ���
	m_state = STATE_NORMAL;
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
			pPad->InitTrigger(CInputPad::KEY_START);
		}
	}
}
