//============================================================
//
//	�G�U��00���� [enemyAttack00.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "enemyAttack00.h"
#include "enemyBossDragon.h"
#include "manager.h"
#include "camera.h"
#include "player.h"
#include "multiModel.h"
#include "impact.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const float	TELEPORT_POS_DIS	= 800.0f;	// �e���|�[�g���̃v���C���[�ʒu���牓�����鋗��
	const int	ATK_1ST_MOTION_KEY	= 3;		// �U�������L�[:��i�K��
	const int	ATK_2ND_MOTION_KEY	= 7;		// �U�������L�[:��i�K��
	const int	ATK_3RD_MOTION_KEY	= 13;		// �U�������L�[:�O�i�K��
	const int	ATTACK_WAIT_FRAME	= 0;		// �U����̍d���t���[��

	namespace impact
	{
		namespace little
		{
			const CCamera::SSwing	PUNCH_SWING	= CCamera::SSwing(14.0f, 1.5f, 0.5f);	// �n�ʉ��莞�̃J�����h�� (��)
			const CWave::SGrow		GROW		= CWave::SGrow(5.5f, 0.0f, 0.0f);		// ������
			const CWave::SGrow		ADDGROW		= CWave::SGrow(0.05f, 0.0f, 0.0f);		// ����������

			const D3DXCOLOR COL = XCOL_CYAN;	// �F
			const float	HOLE_RADIUS	= 0.0f;		// ���̔��a
			const float	THICKNESS	= 0.0f;		// ����
			const float	OUTER_PLUSY	= 40.0f;	// �O����Y���W���Z��
			const float	MAX_RADIUS	= 1400.0f;	// ���a�̍ő听����
		}

		namespace big
		{
			const CCamera::SSwing	PUNCH_SWING	= CCamera::SSwing(32.0f, 1.5f, 1.5f);	// �n�ʉ��莞�̃J�����h�� (��)
			const CWave::SGrow		GROW		= CWave::SGrow(6.5f, 0.0f, 0.0f);		// ������
			const CWave::SGrow		ADDGROW		= CWave::SGrow(0.05f, 0.0f, 0.0f);		// ����������

			const D3DXCOLOR COL = XCOL_CYAN;	// �F
			const float	HOLE_RADIUS	= 0.0f;		// ���̔��a
			const float	THICKNESS	= 0.0f;		// ����
			const float	OUTER_PLUSY	= 80.0f;	// �O����Y���W���Z��
			const float	MAX_RADIUS	= 1600.0f;	// ���a�̍ő听����
		}
	}
}

//************************************************************
//	�q�N���X [CEnemyAttack00] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEnemyAttack00::CEnemyAttack00() :
	m_state			(STATE_INIT_TELEPORT),	// ���
	m_nCounterState	(0)						// ��ԊǗ��J�E���^�[
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CEnemyAttack00::~CEnemyAttack00()
{

}

//============================================================
//	����������
//============================================================
HRESULT CEnemyAttack00::Init(void)
{
	// �����o�ϐ���������
	m_state = STATE_INIT_TELEPORT;	// ���
	m_nCounterState = 0;			// ��ԊǗ��J�E���^�[

	// �G�U���̏�����
	if (FAILED(CEnemyAttack::Init()))
	{ // �������Ɏ��s�����ꍇ

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
void CEnemyAttack00::Uninit(void)
{
	// �G�U���̏I��
	CEnemyAttack::Uninit();
}

//============================================================
//	�X�V����
//============================================================
bool CEnemyAttack00::Update(void)
{
	// �|�C���^��錾
	CEnemyBossDragon *pBoss = GetBoss();	// �{�X�̏��

	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_INIT_TELEPORT:	// �e���|�[�g�̏�����
	{
		CListManager<CPlayer> *pList = CPlayer::GetList();				// �v���C���[���X�g
		if (pList == nullptr)		 { assert(false); return false; }	// ���X�g���g�p
		if (pList->GetNumAll() != 1) { assert(false); return false; }	// �v���C���[��1�l����Ȃ�
		auto player = pList->GetList().front();							// �v���C���[���

		float fRandRot = useful::RandomRot();				// �����_������
		D3DXVECTOR3 posPlayer = player->GetVec3Position();	// �v���C���[�̈ʒu

		D3DXVECTOR3 posEnemy = VEC3_ZERO;	// �G�̐ݒ�ʒu
		D3DXVECTOR3 rotEnemy = VEC3_ZERO;	// �G�̐ݒ����

		// �v���C���[���烉���_�������ɉ��������ʒu��ݒ�
		posEnemy.x += posPlayer.x + sinf(fRandRot) * TELEPORT_POS_DIS;
		posEnemy.z += posPlayer.z + cosf(fRandRot) * TELEPORT_POS_DIS;

		// �v���C���[������ݒ�
		D3DXVECTOR3 vec = posEnemy - posPlayer;
		rotEnemy.y = atan2f(vec.x, vec.z);

		// �{�X���e���|�[�g������
		pBoss->SetTeleport(posEnemy, rotEnemy);

		// �e���|�[�g��Ԃɂ���
		m_state = STATE_TELEPORT;

		// �����͔������e���|�[�g�̏�ԍX�V�Ɉڍs
	}
	case STATE_TELEPORT:	// �e���|�[�g
	{
		if (pBoss->GetAction() == CEnemyBossDragon::ACT_NONE)
		{ // �����s�����Ă��Ȃ��ꍇ

			// �g�����˂̏�������Ԃɂ���
			m_state = STATE_INIT_WAVE;
		}

		break;
	}
	case STATE_INIT_WAVE:	// �g�����˂̏�����
	{
		// �n�ʉ���̍s����ݒ�
		pBoss->SetActPunchGround();

		// �g�����ˏ�Ԃɂ���
		m_state = STATE_WAVE;

		// �����͔������g�����˂̏�ԍX�V�Ɉڍs
	}
	case STATE_WAVE:	// �g������
	{
		if (pBoss->GetMotionType() != CEnemyBossDragon::MOTION_PUNCH_GROUND)
		{ // �n�ʉ��胂�[�V�����ł͂Ȃ��ꍇ

			// �ҋ@��Ԃɂ���
			m_state = STATE_WAIT;
		}

		if (pBoss->GetMotionKeyCounter() != 1) { break; }	// �}�g���b�N�X���L�[�̊J�n�ʒu�ɂȂ��Ă��Ȃ��ꍇ������

		switch (pBoss->GetMotionKey())
		{ // ���[�V�����L�[���Ƃ̏���
		case ATK_1ST_MOTION_KEY:	// ������ځF�E��
		{
			D3DXVECTOR3 posHand;	// ��̃��[���h���W

			// ����̃}�g���b�N�X���擾
			D3DXMATRIX mtxHandL = pBoss->GetMultiModel(CEnemyBossDragon::MODEL_HAND_L)->GetMtxWorld();

			// ����̃��[���h���W��ݒ�
			posHand = useful::GetMtxWorldPosition(mtxHandL);

			// Y���W�͑����ɂ���
			posHand.y = pBoss->GetVec3Position().y;

			// �J�����h���ݒ�
			GET_MANAGER->GetCamera()->SetSwing(CCamera::TYPE_MAIN, impact::little::PUNCH_SWING);

			// �Ռ��g�̐���
			CImpact::Create
			( // ����
				CWave::TEXTURE_IMPACT,			// ���
				posHand,						// �ʒu
				impact::little::COL,			// �F
				impact::little::GROW,			// ������
				impact::little::ADDGROW,		// ����������
				impact::little::HOLE_RADIUS,	// ���̔��a
				impact::little::THICKNESS,		// ����
				impact::little::OUTER_PLUSY,	// �O����Y���W���Z��
				impact::little::MAX_RADIUS		// ���a�̍ő听����
			);

			break;
		}
		case ATK_2ND_MOTION_KEY:	// ������ځF����
		{
			D3DXVECTOR3 posHand;	// ��̃��[���h���W

			// �E��̃}�g���b�N�X���擾
			D3DXMATRIX mtxHandR = pBoss->GetMultiModel(CEnemyBossDragon::MODEL_HAND_R)->GetMtxWorld();

			// �E��̃��[���h���W��ݒ�
			posHand = useful::GetMtxWorldPosition(mtxHandR);

			// Y���W�͑����ɂ���
			posHand.y = pBoss->GetVec3Position().y;

			// �J�����h���ݒ�
			GET_MANAGER->GetCamera()->SetSwing(CCamera::TYPE_MAIN, impact::little::PUNCH_SWING);

			// �Ռ��g�̐���
			CImpact::Create
			( // ����
				CWave::TEXTURE_IMPACT,			// ���
				posHand,						// �ʒu
				impact::little::COL,			// �F
				impact::little::GROW,			// ������
				impact::little::ADDGROW,		// ����������
				impact::little::HOLE_RADIUS,	// ���̔��a
				impact::little::THICKNESS,		// ����
				impact::little::OUTER_PLUSY,	// �O����Y���W���Z��
				impact::little::MAX_RADIUS		// ���a�̍ő听����
			);

			break;
		}
		case ATK_3RD_MOTION_KEY:	// ����O��ځF����
		{
			D3DXMATRIX  mtxHandL = pBoss->GetMultiModel(CEnemyBossDragon::MODEL_HAND_L)->GetMtxWorld();	// ����̃}�g���b�N�X
			D3DXMATRIX  mtxHandR = pBoss->GetMultiModel(CEnemyBossDragon::MODEL_HAND_R)->GetMtxWorld();	// �E��̃}�g���b�N�X
			D3DXVECTOR3 posHandL = useful::GetMtxWorldPosition(mtxHandL);	// ����̃��[���h���W
			D3DXVECTOR3 posHandR = useful::GetMtxWorldPosition(mtxHandR);	// �E��̃��[���h���W
			D3DXVECTOR3 posHand;	// ��̃��[���h���W

			// ����̒��ԃ��[���h���W��ݒ�
			D3DXVec3Lerp(&posHand, &posHandL, &posHandR, 0.5f);

			// Y���W�͑����ɂ���
			posHand.y = pBoss->GetVec3Position().y;

			// �J�����h���ݒ�
			GET_MANAGER->GetCamera()->SetSwing(CCamera::TYPE_MAIN, impact::big::PUNCH_SWING);

			// �Ռ��g�̐���
			CImpact::Create
			( // ����
				CWave::TEXTURE_IMPACT,		// ���
				posHand,					// �ʒu
				impact::big::COL,			// �F
				impact::big::GROW,			// ������
				impact::big::ADDGROW,		// ����������
				impact::big::HOLE_RADIUS,	// ���̔��a
				impact::big::THICKNESS,		// ����
				impact::big::OUTER_PLUSY,	// �O����Y���W���Z��
				impact::big::MAX_RADIUS		// ���a�̍ő听����
			);

			break;
		}
		}

		break;
	}
	case STATE_WAIT:	// �ҋ@
	{
		// �J�E���^�[�����Z
		m_nCounterState++;
		if (m_nCounterState >= ATTACK_WAIT_FRAME)
		{ // �ҋ@�����������ꍇ

			// �J�E���^�[��������
			m_nCounterState = 0;

			// �I����Ԃɂ���
			m_state = STATE_END;
		}

		break;
	}
	case STATE_END:	// �I��
	{
		// �U���I����Ԃ�
		return true;
	}
	default:	// ��O����
		assert(false);
		break;
	}

	// �U����I����Ԃ�
	return false;
}
