//============================================================
//
//	�v���C���[�^�C�g������ [playerTitle.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "playerTitle.h"
#include "manager.h"
#include "camera.h"
#include "sceneTitle.h"
#include "titleManager.h"
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
	const char* TEXTURE_FILE = "data\\TEXTURE\\slash000.png";	// �a���e�N�X�`��
	const D3DXVECTOR3 POS = D3DXVECTOR3(0.0f, 800.0f, 0.0f);	// �v���C���[�����ʒu

	// �a�����
	namespace slash
	{
		const D3DXVECTOR3 POS	= SCREEN_CENT + D3DXVECTOR3(0.0f, -90.0f, 0.0f);	// �ʒu
		const D3DXVECTOR3 SIZE	= SCREEN_SIZE;		// �傫��
		const POSGRID2	  PART	= POSGRID2(6, 6);	// �e�N�X�`��������
	}
}

//************************************************************
//	�q�N���X [CPlayerTitle] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CPlayerTitle::CPlayerTitle() :
	m_pSlash	(nullptr),	// �a���̏��
	m_stag		(STAG_NONE)	// ���o���
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CPlayerTitle::~CPlayerTitle()
{

}

//============================================================
//	����������
//============================================================
HRESULT CPlayerTitle::Init(void)
{
	// �����o�ϐ���������
	m_pSlash = nullptr;	// �a���̏��
	m_stag = STAG_FALL;	// ���o���

	// �v���C���[�̏�����
	if (FAILED(CPlayer::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �a���̐���
	m_pSlash = CAnim2D::Create
	( // ����
		slash::PART.x,	// �e�N�X�`���̉��̕�����
		slash::PART.y,	// �e�N�X�`���̏c�̕�����
		slash::POS,		// �ʒu
		slash::SIZE		// �傫��
	);
	if (m_pSlash == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�N�X�`����o�^�E����
	m_pSlash->BindTexture(TEXTURE_FILE);

	// ���x�����G�t�F�N�g�ɂ���
	m_pSlash->SetLabel(LABEL_EFFECT);

	// �ύX�t���[����ݒ�
	m_pSlash->SetCounter(1);

	// �Đ����~������
	m_pSlash->SetEnableStop(true);

	// ���Z�����ɂ���
	m_pSlash->GetRenderState()->SetAlphaBlend(CRenderState::BLEND_ADD);

	// �ʒu��ݒ�
	SetVec3Position(POS);

	// �������[�V������ݒ�
	SetMotion(BODY_LOWER, L_MOTION_FALL);
	SetMotion(BODY_UPPER, U_MOTION_FALL);

	// UI�����`���OFF�ɂ���
	SetEnableDrawUI(false);

	// �����X�V�E�����`���ON�ɂ���
	SetEnableUpdate(true);
	SetEnableDraw(true);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CPlayerTitle::Uninit(void)
{
	// �v���C���[�̏I��
	CPlayer::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CPlayerTitle::Update(void)
{
	// �I�u�W�F�N�g�����L�����N�^�[�̍X�V
	CObjectDivChara::Update();

	// �ߋ��ʒu�̍X�V
	UpdateOldPosition();

	switch (m_stag)
	{ // ���o��Ԃ��Ƃ̏���
	case STAG_NONE:
		break;

	case STAG_FALL:

		// �������o�̍X�V
		UpdateStagFall();
		break;

	case STAG_LAND:

		// ���n���o�̍X�V
		UpdateStagLand();
		break;

	case STAG_IDOL:

		// �ҋ@���o�̍X�V
		UpdateStagIdol();
		break;

	case STAG_ATTACK:

		// �U�����o�̍X�V
		UpdateStagAttack();
		break;

	default:
		assert(false);
		break;
	}

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
}

//============================================================
//	�`�揈��
//============================================================
void CPlayerTitle::Draw(CShader * /*pShader*/)
{
	// �v���C���[�̕`��
	CPlayer::Draw();
}

//============================================================
//	�������o�̍X�V����
//============================================================
void CPlayerTitle::UpdateStagFall(void)
{
	// �ϐ���錾
	D3DXVECTOR3 posPlayer = GetVec3Position();	// �v���C���[�ʒu
	D3DXVECTOR3 rotPlayer = GetVec3Rotation();	// �v���C���[����

	// �|�C���^��錾
	CStage *pStage = CScene::GetStage();				// �X�e�[�W���
	if (pStage == nullptr) { assert(false); return; }	// �X�e�[�W��g�p��

	// �d�͂̍X�V
	UpdateGravity();

	// �ʒu�X�V
	UpdatePosition(&posPlayer);

	// ���n����
	if (UpdateLanding(&posPlayer))
	{ // ���n�����ꍇ

		// ���n���[�V������ݒ�
		SetMotion(BODY_LOWER, L_MOTION_LAND);
		SetMotion(BODY_UPPER, U_MOTION_LAND);

		// ���n���o�ɂ���
		m_stag = STAG_LAND;
	}

	// �����X�V
	UpdateRotation(&rotPlayer);

	// �X�e�[�W�͈͊O�̕␳
	pStage->LimitPosition(posPlayer, GetRadius());

	// �ʒu�𔽉f
	SetVec3Position(posPlayer);

	// �����𔽉f
	SetVec3Rotation(rotPlayer);
}

//============================================================
//	���n���o�̍X�V����
//============================================================
void CPlayerTitle::UpdateStagLand(void)
{
	if (IsMotionFinish(BODY_LOWER)
	&&  IsMotionFinish(BODY_UPPER))
	{ // ���n���[�V�������I�������ꍇ

		// �ҋ@���[�V������ݒ�
		SetMotion(BODY_LOWER, L_MOTION_IDOL);
		SetMotion(BODY_UPPER, U_MOTION_IDOL);

		// �ҋ@���o�ɂ���
		m_stag = STAG_IDOL;
	}
}

//============================================================
//	�ҋ@���o�̍X�V����
//============================================================
void CPlayerTitle::UpdateStagIdol(void)
{
	CCamera *pCamera = GET_MANAGER->GetCamera();	// �J�������
	if (pCamera->GetLook().nCounterForce == 0
	&&  pCamera->GetState() == CCamera::STATE_TITLE_ATK)
	{ // �J�����̍ŏI�ڕW�ɂȂ��Ă���ꍇ

		// �^�C�g���U�����[�V������ݒ�
		SetMotion(BODY_LOWER, L_MOTION_TITLE_ATTACK);
		SetMotion(BODY_UPPER, U_MOTION_TITLE_ATTACK);

		// �U�����o�ɂ���
		m_stag = STAG_ATTACK;
	}
}

//============================================================
//	�U�����o�̍X�V����
//============================================================
void CPlayerTitle::UpdateStagAttack(void)
{
	if (IsMotionFinish(BODY_LOWER))
	{ // ���[�V�������I�������ꍇ

		// ���̃��[�h�ɑJ��
		CSceneTitle::GetTitleManager()->Transition();
	}

	if (GetMotionKey(BODY_LOWER) == 0 && GetMotionKeyCounter(BODY_LOWER) == 1)
	{ // ���[�V�������n�܂����ꍇ

		// �a�����Đ�
		m_pSlash->SetEnableStop(false);
	}

	if (m_pSlash->GetLoopAnimation() >= 1)
	{ // ��x�Đ����I������ꍇ

		// �a�����~
		m_pSlash->SetEnableStop(true);
	}
}
