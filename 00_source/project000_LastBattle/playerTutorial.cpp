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

}

//************************************************************
//	�q�N���X [CPlayerTutorial] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CPlayerTutorial::CPlayerTutorial() : m_oldCamRot(VEC3_ZERO)
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
	// �����o�ϐ���������
	m_oldCamRot = VEC3_ZERO;	// �J�����̉ߋ�����

	// �v���C���[�̏�����
	if (FAILED(CPlayer::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �ʏ��Ԃ̏�����
	InitNormal();

	// �����X�V�E�����`���ON�ɂ���
	SetEnableUpdate(true);
	SetEnableDraw(true);

	// �J������Ǐ]��Ԃɐݒ�
	CCamera *pCamera = GET_MANAGER->GetCamera();	// �J�������
	pCamera->SetState(CCamera::STATE_FOLLOW);		// �J������Ǐ]��Ԃɐݒ�
	pCamera->SetDestFollow();						// �J�����ڕW�ʒu�̏�����

	// �J�����̉ߋ�������ۑ�
	m_oldCamRot = pCamera->GetRotation();

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
	// ��Ԃ��ʏ�ȊO�Ȃ�G���[
	assert(GetState() == STATE_NORMAL);

	CCamera *pCamera = GET_MANAGER->GetCamera();	// �J�������
	CTutorialManager *pTutorialManager = CSceneTutorial::GetTutorialManager();	// �`���[�g���A���}�l�[�W���[���
	if (pTutorialManager->GetLesson() == CTutorialManager::LESSON_01)
	{ // ���b�X��01�F�ړ��E�J��������ɒ��풆�̏ꍇ

		D3DXVECTOR3 curCamRot = pCamera->GetRotation();	// ���݂̃J��������
		if (fabsf(m_oldCamRot.x - curCamRot.x) >= 0.001f
		||  fabsf(m_oldCamRot.y - curCamRot.y) >= 0.001f
		||  fabsf(m_oldCamRot.z - curCamRot.z) >= 0.001f)
		{ // �J�����̌����������Ă����ꍇ

			// ���b�X���J�E���^�[���Z
			pTutorialManager->AddLessonCounter();
		}
	}

	// �J�����̉ߋ�������ۑ�
	m_oldCamRot = pCamera->GetRotation();

	// �v���C���[�̍X�V
	CPlayer::Update();
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
	// �|�C���^��錾
	CTutorialManager *pTutorialManager = CSceneTutorial::GetTutorialManager();	// �`���[�g���A���}�l�[�W���[���
	CStage *pStage = CScene::GetStage();				// �X�e�[�W���
	if (pStage == nullptr) { assert(false); return; }	// �X�e�[�W��g�p��

	// �ϐ���錾
	D3DXVECTOR3 posPlayer = GetVec3Position();		// �v���C���[�ʒu
	D3DXVECTOR3 rotPlayer = GetVec3Rotation();		// �v���C���[����
	int nLesson = pTutorialManager->GetLesson();	// ���b�X���ԍ�

	// �d�͂̍X�V
	UpdateGravity();

	if (nLesson >= CTutorialManager::LESSON_04)
	{ // ���b�X��04�F�U������̒��풆�A�܂��̓N���A�ς݂̏ꍇ

		// �U������
		if (UpdateAttack(posPlayer, rotPlayer))
		{ // �U�����Ă����ꍇ

			if (nLesson == CTutorialManager::LESSON_04)
			{ // ���b�X��04�F�W�����v����ɒ��풆�̏ꍇ

				// ���b�X���J�E���^�[���Z
				pTutorialManager->AddLessonCounter();
			}
		}
	}

	if (nLesson >= CTutorialManager::LESSON_03)
	{ // ���b�X��03�F��𑀍�̒��풆�A�܂��̓N���A�ς݂̏ꍇ

		// ��𑀍�
		if (UpdateDodge(rotPlayer))
		{ // ������Ă����ꍇ

			if (nLesson == CTutorialManager::LESSON_03)
			{ // ���b�X��03�F�W�����v����ɒ��풆�̏ꍇ

				// ���b�X���J�E���^�[���Z
				pTutorialManager->AddLessonCounter();
			}
		}
	}

	if (nLesson >= CTutorialManager::LESSON_01)
	{ // ���b�X��01�F�ړ��E�J��������̒��풆�A�܂��̓N���A�ς݂̏ꍇ

		// �ړ�����E�ڕW�����ݒ�
		if (UpdateMove(pLowMotion, pUpMotion))
		{ // �ړ����Ă����ꍇ

			if (nLesson == CTutorialManager::LESSON_01)
			{ // ���b�X��01�F�ړ��E�J��������ɒ��풆�̏ꍇ

				// ���b�X���J�E���^�[���Z
				pTutorialManager->AddLessonCounter();
			}
		}
	}

	if (nLesson >= CTutorialManager::LESSON_02)
	{ // ���b�X��02�F�W�����v����̒��풆�A�܂��̓N���A�ς݂̏ꍇ

		// �W�����v����
		if (UpdateJump(pLowMotion, pUpMotion))
		{ // �W�����v���Ă����ꍇ

			if (nLesson == CTutorialManager::LESSON_02)
			{ // ���b�X��02�F�W�����v����ɒ��풆�̏ꍇ

				// ���b�X���J�E���^�[���Z
				pTutorialManager->AddLessonCounter();
			}
		}
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
