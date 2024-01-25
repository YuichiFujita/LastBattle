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
#include "player.h"
#include "multiModel.h"
#include "wave.h"

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

		float fRandRot = (float)(rand() % 629 - 314) / 100.0f;	// �����_������
		D3DXVECTOR3 posPlayer = player->GetVec3Position();		// �v���C���[�̈ʒu

		D3DXVECTOR3 posEnemy = VEC3_ZERO;	// �G�̐ݒ�ʒu
		D3DXVECTOR3 rotEnemy = VEC3_ZERO;	// �G�̐ݒ����

		// �v���C���[���烉���_�������ɉ��������ʒu��ݒ�
		posEnemy.x += posPlayer.x + sinf(fRandRot) * 1200.0f;
		posEnemy.z += posPlayer.z + cosf(fRandRot) * 1200.0f;

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

			// �g���̐���
			D3DXMATRIX  mtx = pBoss->GetMultiModel(CEnemyBossDragon::MODEL_HAND_L)->GetMtxWorld();
			D3DXVECTOR3 pos = D3DXVECTOR3(mtx._41, pBoss->GetVec3Position().y, mtx._43);
			CWave::Create
			( // ����
				CWave::TEXTURE_NONE,
				pos,
				VEC3_ZERO,
				XCOL_BLUE,
				CWave::SGrow(6.0f, 0.0f, 0.0f),
				POSGRID2(64, 1),
				POSGRID2(8, 1),
				10.0f,
				0.0f,
				25.0f
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
		if (m_nCounterState >= 10)
		{ // �ҋ@�����������ꍇ

			// �J�E���^�[��������
			m_nCounterState = 0;

			if (m_nCounterNumAtk < 3)
			{
				// �e���|�[�g��������Ԃɂ���
				m_state = STATE_INIT_TELEPORT;
			}
			else
			{
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
