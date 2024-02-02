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
	const CWave::SGrow		IMPACT_GROW		= CWave::SGrow(6.0f, 0.0f, 0.0f);		// ������
	const CWave::SGrow		IMPACT_ADDGROW	= CWave::SGrow(0.2f, 0.002f, 0.0f);		// ����������
	const CCamera::SSwing	PUNCH_SWING		= CCamera::SSwing(12.0f, 1.5f, 0.25f);	// �n�ʉ��莞�̃J�����h��

	const float	IMPACT_HOLE_RADIUS	= 10.0f;	// ���̔��a
	const float	IMPACT_THICKNESS	= 0.0f;		// ����
	const float	IMPACT_OUTER_PLUSY	= 25.0f;	// �O����Y���W���Z��
	const float	IMPACT_MAX_RADIUS	= 3000.0f;	// ���a�̍ő听����
	const float	TELEPORT_POS_DIS	= 800.0f;	// �e���|�[�g���̃v���C���[�ʒu���牓�����鋗��
	const int	ATTACK_WAIT_FRAME	= 10;		// �U����̍d���t���[��
	const int	MAX_ATTACK			= 2;		// �U����
}

//************************************************************
//	�q�N���X [CEnemyAttack00] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEnemyAttack00::CEnemyAttack00() :
	m_state				(STATE_INIT_TELEPORT),	// ���
	m_nCounterState		(0),					// ��ԊǗ��J�E���^�[
	m_nCounterNumAtk	(0)						// �U���񐔃J�E���^�[
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
	m_nCounterState  = 0;			// ��ԊǗ��J�E���^�[
	m_nCounterNumAtk = 0;			// �U���񐔃J�E���^�[

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

		if (pBoss->GetMotionKey() == pBoss->GetMotionNumKey() - 1 && pBoss->GetMotionKeyCounter() == 0)
		{ // ���[�V�������n�ʂ��������^�C�~���O�̏ꍇ

			// �J�����h���ݒ�
			GET_MANAGER->GetCamera()->SetSwing(CCamera::TYPE_MAIN, PUNCH_SWING);

			// �Ռ��g�̐���
			D3DXMATRIX  mtxHandL = pBoss->GetMultiModel(CEnemyBossDragon::MODEL_HAND_L)->GetMtxWorld();	// ����̃}�g���b�N�X
			D3DXVECTOR3 posHandL = D3DXVECTOR3(mtxHandL._41, pBoss->GetVec3Position().y, mtxHandL._43);	// ����̃��[���h���W
			CImpact::Create
			( // ����
				CWave::TEXTURE_NONE,	// ���
				posHandL,				// �ʒu
				IMPACT_GROW,			// ������
				IMPACT_ADDGROW,			// ����������
				IMPACT_HOLE_RADIUS,		// ���̔��a
				IMPACT_THICKNESS,		// ����
				IMPACT_OUTER_PLUSY,		// �O����Y���W���Z��
				IMPACT_MAX_RADIUS		// ���a�̍ő听����
			);

			// �U���񐔂����Z
			m_nCounterNumAtk++;
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

			if (m_nCounterNumAtk < MAX_ATTACK)
			{ // �U���񐔂������ɓ��B���Ă��Ȃ��ꍇ

				// �e���|�[�g��������Ԃɂ���
				m_state = STATE_INIT_TELEPORT;
			}
			else
			{ // �U���񐔂������ɓ��B�����ꍇ

				// �U���񐔂�������
				m_nCounterNumAtk = 0;

				// �I����Ԃɂ���
				m_state = STATE_END;
			}
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
