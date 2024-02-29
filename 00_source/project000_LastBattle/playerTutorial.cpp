//============================================================
//
//	�v���C���[�`���[�g���A������ [playerTutorial.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "playerTutorial.h"
#include "manager.h"
#include "camera.h"
#include "sceneTutorial.h"
#include "tutorialManager.h"
#include "renderer.h"
#include "sound.h"
#include "stage.h"
#include "sword.h"
#include "shadow.h"
#include "anim2D.h"
#include "effect3D.h"
#include "particle3D.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const D3DXVECTOR3 POS = D3DXVECTOR3(0.0f, 800.0f, 0.0f);	// �v���C���[�����ʒu
}

//************************************************************
//	�q�N���X [CPlayerTutorial] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CPlayerTutorial::CPlayerTutorial()
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CPlayerTutorial::~CPlayerTutorial()
{

}

//============================================================
//	����������
//============================================================
HRESULT CPlayerTutorial::Init(void)
{
	// �v���C���[�̏�����
	if (FAILED(CPlayer::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �ʏ��Ԃ̏�����
	InitNormal();

	// �ҋ@���[�V������ݒ�
	//SetMotion(BODY_LOWER, L_MOTION_IDOL);
	//SetMotion(BODY_UPPER, U_MOTION_IDOL);

	// �����X�V�E�����`���ON�ɂ���
	SetEnableUpdate(true);
	SetEnableDraw(true);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CPlayerTutorial::Uninit(void)
{
	// �v���C���[�̏I��
	CPlayer::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CPlayerTutorial::Update(void)
{
#if 1
	if (GET_MANAGER->GetMode() == CScene::MODE_TUTORIAL)
	{ // �`���[�g���A����ʂ̏ꍇ

		CCamera *pCamera = GET_MANAGER->GetCamera();								// �J�������
		CTutorialManager *pTutorialManager = CSceneTutorial::GetTutorialManager();	// �`���[�g���A���}�l�[�W���[���
		CTutorialManager::EState state = pTutorialManager->GetState();				// �`���[�g���A�����
		if (state != CTutorialManager::STATE_PROGRESSION
		&&  state != CTutorialManager::STATE_NEXTWAIT)
		{ // ���b�X���m�F��Ԃ̏ꍇ

			// �J�����̍X�V��OFF�ɂ���
			pCamera->SetEnableUpdate(false);
			return;
		}
		else
		{ // ���b�X���m�F��Ԃł͂Ȃ��ꍇ

			// �J�����̍X�V��ON�ɂ���
			pCamera->SetEnableUpdate(true);
		}
	}

	// �v���C���[�̍X�V
	CPlayer::Update();
#else
	// �I�u�W�F�N�g�����L�����N�^�[�̍X�V
	CObjectDivChara::Update();

	// �ߋ��ʒu�̍X�V
	UpdateOldPosition();



	bool bColl[] =	// ����ON/OFF��
	{
		IsLeftWeaponCollision(BODY_UPPER),	// ����̕���̍U������
		IsRightWeaponCollision(BODY_UPPER),	// �E��̕���̍U������
	};
	for (int nCntSword = 0; nCntSword < player::NUM_SWORD; nCntSword++)
	{ // ���̐����J��Ԃ�

		CSword *pSword = GetSword(nCntSword);	// ���̏��

		// ���̍U�������ݒ�
		pSword->SetEnableAttack(bColl[nCntSword]);

		// ���̍X�V
		pSword->Update();
	}

	// �e�̍X�V
	GetShadow()->Update();
#endif
}

//============================================================
//	�`�揈��
//============================================================
void CPlayerTutorial::Draw(CShader * /*pShader*/)
{
	// �v���C���[�̕`��
	CPlayer::Draw();
}

//============================================================
//	�ʏ��Ԏ��̍X�V����
//============================================================
void CPlayerTutorial::UpdateNormal(int *pLowMotion, int *pUpMotion)
{
	// TODO�F�f�o�b�O�J�E���^�[���Z
	if (GET_INPUTKEY->IsPress(DIK_0))
	{
		// ���b�X���J�E���^�[���Z
		CSceneTutorial::GetTutorialManager()->AddLessonCounter();
	}

	// �ϐ���錾
	D3DXVECTOR3 posPlayer = GetVec3Position();	// �v���C���[�ʒu
	D3DXVECTOR3 rotPlayer = GetVec3Rotation();	// �v���C���[����

	// �|�C���^��錾
	CStage *pStage = CScene::GetStage();				// �X�e�[�W���
	if (pStage == nullptr) { assert(false); return; }	// �X�e�[�W��g�p��

	// �d�͂̍X�V
	UpdateGravity();

	if (CSceneTutorial::GetTutorialManager()->GetLesson() >= CTutorialManager::LESSON_04)
	{ // ���b�X��04�F�U������̒��풆�A�܂��̓N���A�ς݂̏ꍇ

		// �U������
		UpdateAttack(posPlayer, rotPlayer);
	}

	if (CSceneTutorial::GetTutorialManager()->GetLesson() >= CTutorialManager::LESSON_03)
	{ // ���b�X��03�F��𑀍�̒��풆�A�܂��̓N���A�ς݂̏ꍇ

		// ��𑀍�
		UpdateDodge(rotPlayer);
	}

	if (CSceneTutorial::GetTutorialManager()->GetLesson() >= CTutorialManager::LESSON_01)
	{ // ���b�X��01�F�ړ��E�J��������̒��풆�A�܂��̓N���A�ς݂̏ꍇ

		// �ړ�����E�ڕW�����ݒ�
		UpdateMove(pLowMotion, pUpMotion);
	}

	if (CSceneTutorial::GetTutorialManager()->GetLesson() >= CTutorialManager::LESSON_02)
	{ // ���b�X��02�F�W�����v����̒��풆�A�܂��̓N���A�ς݂̏ꍇ

		// �W�����v����
		UpdateJump(pLowMotion, pUpMotion);
	}

	// �ʒu�X�V
	UpdatePosition(&posPlayer);

	// �G�Ƃ̓����蔻��̍X�V
	UpdateCollEnemy(&posPlayer);

	// ���n����
	UpdateLanding(&posPlayer);

	// �����X�V
	UpdateRotation(&rotPlayer);

	// �X�e�[�W�͈͊O�̕␳
	pStage->LimitPosition(posPlayer, GetRadius());

	// �ʒu�𔽉f
	SetVec3Position(posPlayer);

	// �����𔽉f
	SetVec3Rotation(rotPlayer);
}
