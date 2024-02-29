//============================================================
//
//	�G�U��01���� [enemyAttack01.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "enemyAttack01.h"
#include "enemyBossDragon.h"
#include "manager.h"
#include "camera.h"
#include "player.h"
#include "stage.h"
#include "collision.h"
#include "attackThunder.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const float	TELEPORT_POS_DIS	= 3600.0f;	// �e���|�[�g���̃X�e�[�W���S�ʒu���牓�����鋗��
	const float	TELEPORT_POSY		= 800.0f;	// �e���|�[�g����Y���W
	const float FIND_RADIUS			= 250.0f;	// �v���C���[���m���a
	const int	ATK_MOTION_KEY		= 1;		// �U�������L�[
	const int	ATTACK_WAIT_FRAME	= 30;		// �U����̍d���t���[��
	const int	NUM_THUNDER			= 4;		// ���̍U���Ő������闋�̐� (���S�ɂ��������邽�߃v���X1�����)
	const int	DIV_LENRAND			= 201;		// �����_�������̏�]�Z�̒l
	const int	ADD_LENRAND			= 250;		// �����_�������̉��Z�̒l
	const int	MAX_ATTACK			= 3;		// �U����
}

//************************************************************
//	�q�N���X [CEnemyAttack01] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEnemyAttack01::CEnemyAttack01(CEnemyBossDragon *pBoss) : CEnemyAttack(pBoss),
	m_state				(STATE_INIT_TELEPORT),	// ���
	m_nCounterState		(0),					// ��ԊǗ��J�E���^�[
	m_nCounterNumAtk	(0)						// �U���񐔃J�E���^�[
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CEnemyAttack01::~CEnemyAttack01()
{

}

//============================================================
//	����������
//============================================================
HRESULT CEnemyAttack01::Init(void)
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
void CEnemyAttack01::Uninit(void)
{
	// �G�U���̏I��
	CEnemyAttack::Uninit();
}

//============================================================
//	�X�V����
//============================================================
bool CEnemyAttack01::Update(void)
{
	// �|�C���^��錾
	CEnemyBossDragon *pBoss = GetBoss();	// �{�X�̏��
	CStage *pStage = CScene::GetStage();	// �X�e�[�W�̏��

	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_INIT_TELEPORT:	// �e���|�[�g�̏�����
	{
		D3DXVECTOR3 posEnemy = VEC3_ZERO;	// �G�̐ݒ�ʒu
		D3DXVECTOR3 rotEnemy = VEC3_ZERO;	// �G�̐ݒ����

		// �����_���������ɓG�̈ʒu��ݒ�
		float fRandRot = useful::RandomRot();
		posEnemy.x = sinf(fRandRot) * TELEPORT_POS_DIS;
		posEnemy.y = TELEPORT_POSY;
		posEnemy.z = cosf(fRandRot) * TELEPORT_POS_DIS;

		// �X�e�[�W���S�����ɓG�̌�����ݒ�
		D3DXVECTOR3 vec = posEnemy - pStage->GetStageLimit().center;
		rotEnemy.y = atan2f(vec.x, vec.z);

		// �{�X���e���|�[�g������
		pBoss->SetTeleport(posEnemy, rotEnemy, CEnemyBossDragon::MOTION_FLY_IDOL);

		// �e���|�[�g��Ԃɂ���
		m_state = STATE_TELEPORT;

		// �����͔������e���|�[�g�̏�ԍX�V�Ɉڍs
	}
	case STATE_TELEPORT:	// �e���|�[�g
	{
		if (pBoss->GetAction() == CEnemyBossDragon::ACT_NONE)
		{ // �����s�����Ă��Ȃ��ꍇ

			// �����˂̏�������Ԃɂ���
			m_state = STATE_INIT_THUNDER;
		}

		break;
	}
	case STATE_INIT_THUNDER:	// �g�����˂̏�����
	{
		// �󒆍U���̍s����ݒ�
		pBoss->SetActFlyAttack();

		// �g�����ˏ�Ԃɂ���
		m_state = STATE_THUNDER;

		// �����͔������g�����˂̏�ԍX�V�Ɉڍs
	}
	case STATE_THUNDER:	// �g������
	{
		if (pBoss->GetMotionType() != CEnemyBossDragon::MOTION_FLY_ATTACK)
		{ // �󒆍U�����[�V�����ł͂Ȃ��ꍇ

			// �ҋ@��Ԃɂ���
			m_state = STATE_WAIT;
		}

		if (pBoss->GetMotionKey() == ATK_MOTION_KEY && pBoss->GetMotionKeyCounter() == 0)
		{ // ���[�V�������V�������^�C�~���O�̏ꍇ

			CListManager<CPlayer> *pList = CPlayer::GetList();				// �v���C���[���X�g
			if (pList == nullptr)		 { assert(false); return false; }	// ���X�g���g�p
			if (pList->GetNumAll() != 1) { assert(false); return false; }	// �v���C���[��1�l����Ȃ�
			auto player = pList->GetList().front();							// �v���C���[���

			D3DXVECTOR3 posPlayer = player->GetVec3Position();	// �v���C���[�̈ʒu
			D3DXVECTOR3 posThunder = VEC3_ZERO;					// ���̈ʒu

			// ���U�����v���C���[�ʒu�ɐ���
			CAttackThunder::Create(posPlayer, true);

			for (int nCntAttack = 0; nCntAttack < NUM_THUNDER; nCntAttack++)
			{ // �����������J��Ԃ�

				// �v���C���[���烉���_�������E�����ɉ��������ʒu��ݒ�
				float fRandRot = useful::RandomRot();	// �����_������
				posThunder.x = posPlayer.x + sinf(fRandRot) * (rand() % DIV_LENRAND + ADD_LENRAND);
				posThunder.z = posPlayer.z + cosf(fRandRot) * (rand() % DIV_LENRAND + ADD_LENRAND);

				// ���U���������_���ʒu�ɐ���
				CAttackThunder::Create(posThunder, true);
			}

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

				// �����˂̏�������Ԃɂ���
				m_state = STATE_INIT_THUNDER;
			}
			else
			{ // �U���񐔂������ɓ��B�����ꍇ

				// �U���񐔂�������
				m_nCounterNumAtk = 0;

				// �����e���|�[�g�̏�������Ԃɂ���
				m_state = STATE_CENTER_TELEPORT_INIT;
			}
		}

		break;
	}
	case STATE_CENTER_TELEPORT_INIT:	// �����e���|�[�g�̏�����
	{
		CListManager<CPlayer> *pList = CPlayer::GetList();				// �v���C���[���X�g
		if (pList == nullptr)		 { assert(false); return false; }	// ���X�g���g�p
		if (pList->GetNumAll() != 1) { assert(false); return false; }	// �v���C���[��1�l����Ȃ�
		auto player = pList->GetList().front();							// �v���C���[���

		D3DXVECTOR3 posPlayer = player->GetVec3Position();		// �v���C���[�̈ʒu
		D3DXVECTOR3 posEnemy  = pStage->GetStageLimit().center;	// �G�̈ʒu
		D3DXVECTOR3 rotEnemy  = VEC3_ZERO;	// �G�̐ݒ����

		// �v���C���[������ݒ�
		D3DXVECTOR3 vec = posEnemy - posPlayer;
		rotEnemy.y = atan2f(vec.x, vec.z);

		// �{�X���e���|�[�g������
		pBoss->SetTeleport(posEnemy, rotEnemy, CEnemyBossDragon::MOTION_IDOL);

		// �����e���|�[�g��Ԃɂ���
		m_state = STATE_CENTER_TELEPORT;

		// �����͔����������e���|�[�g�̏�ԍX�V�Ɉڍs

		break;
	}
	case STATE_CENTER_TELEPORT:	// �����e���|�[�g
	{
		if (pBoss->GetAction() == CEnemyBossDragon::ACT_NONE)
		{ // �����s�����Ă��Ȃ��ꍇ

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

//============================================================
//	�U���I�����̗v�f�ݒ菈��
//============================================================
void CEnemyAttack01::SetRandomArray
(
	CRandom<EAttack> *pRandom,	// �����_���U���N���X
	EAttack /*oldAtk*/,			// �O��̍U��
	int /*nSameAct*/			// �����s���̘A����
)
{
	CPlayer *pPlayer = CScene::GetPlayer();	// �v���C���[�̏��
	CEnemyBossDragon *pBoss = GetBoss();	// �{�X�̏��

	// XZ���ʂ̉~�̓����蔻��
	bool bHit = collision::Circle2D
	( // ����
		pPlayer->GetVec3Position(),	// ����ʒu
		pBoss->GetVec3Position(),	// ����ڕW�ʒu
		pPlayer->GetRadius(),		// ���蔼�a
		FIND_RADIUS					// ����ڕW���a
	);
	if (bHit)
	{ // �v���C���[�Ƃ̋������߂��ꍇ

		// �ߋ����U����ǉ�
		pRandom->AddList(ATTACK_05, 5);	// �U��05(�Ђ������U��)
		pRandom->AddList(ATTACK_06, 2);	// �U��06(�����ۍU��)

		// �������U����ǉ�
		pRandom->AddList(ATTACK_04, 1);	// �U��04(���O���f���o��)
	}
	else
	{ // �v���C���[�Ƃ̋����������ꍇ

		// �������U����ǉ�
		pRandom->AddList(ATTACK_02, 1);	// �U��02(���O������������)
		pRandom->AddList(ATTACK_03, 1);	// �U��03(���v���C���[�ʒu����)
		pRandom->AddList(ATTACK_04, 1);	// �U��04(���O���f���o��)
	}
}
