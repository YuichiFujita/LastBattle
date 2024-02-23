//============================================================
//
//	�G�U��03���� [enemyAttack03.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "enemyAttack03.h"
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
	const float	TELEPORT_POSY	= 170.0f;	// �e���|�[�g����Y���W
	const float FIND_RADIUS		= 250.0f;	// �v���C���[���m���a

	const int	MAX_SUB_WARN_WAIT	= 65;	// �x���\���t���[���̍ő匸�Z��
	const int	INIT_WAIT_FRAME		= 22;	// �����U���ҋ@�t���[��
	const int	NUM_ATTACK			= 20;	// �U���̐�����
	const int	SUB_WAIT_FRAME		= 1;	// ��U�����Ƃ̍U���ҋ@�t���[�����Z��
}

//************************************************************
//	�q�N���X [CEnemyAttack03] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEnemyAttack03::CEnemyAttack03(CEnemyBossDragon *pBoss) : CEnemyAttack(pBoss),
	m_state				(STATE_INIT_TELEPORT),	// ���
	m_nCounterWait		(0),					// �]�C�Ǘ��J�E���^�[
	m_nCounterNumAtk	(0),					// �U���񐔃J�E���^�[
	m_nWaitFrame		(0)						// �]�C�t���[����
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CEnemyAttack03::~CEnemyAttack03()
{

}

//============================================================
//	����������
//============================================================
HRESULT CEnemyAttack03::Init(void)
{
	// �����o�ϐ���������
	m_state				= STATE_INIT_TELEPORT;	// ���
	m_nCounterWait		= 0;					// �]�C�Ǘ��J�E���^�[
	m_nCounterNumAtk	= 0;					// �U���񐔃J�E���^�[
	m_nWaitFrame		= INIT_WAIT_FRAME;		// �]�C�t���[����

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
void CEnemyAttack03::Uninit(void)
{
	// �G�U���̏I��
	CEnemyAttack::Uninit();
}

//============================================================
//	�X�V����
//============================================================
bool CEnemyAttack03::Update(void)
{
	// �|�C���^��錾
	CEnemyBossDragon *pBoss = GetBoss();	// �{�X�̏��

	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_INIT_TELEPORT:	// �e���|�[�g�̏�����
	{
		// �e���|�[�g�̏�����
		InitTeleport();

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
	case STATE_INIT_THUNDER:	// �����˂̏�����
	{
		// �󒆍U���̍s����ݒ�
		pBoss->SetActFlyAttack();

		// �����ˏ�Ԃɂ���
		m_state = STATE_THUNDER;

		// �����͔����������˂̏�ԍX�V�Ɉڍs
	}
	case STATE_THUNDER:	// ������
	{
		// �����˂̍X�V
		UpdateThunder();

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
void CEnemyAttack03::SetRandomArray
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
		pRandom->AddList(ATTACK_00, 1);	// �U��00(�n�ʉ���g��)
		pRandom->AddList(ATTACK_04, 1);	// �U��04(���O���f���o��)
	}
	else
	{ // �v���C���[�Ƃ̋����������ꍇ

		// �������U����ǉ�
		pRandom->AddList(ATTACK_00, 1);	// �U��00(�n�ʉ���g��)
		pRandom->AddList(ATTACK_04, 1);	// �U��04(���O���f���o��)
	}
}

//============================================================
//	�e���|�[�g�̏���������
//============================================================
void CEnemyAttack03::InitTeleport(void)
{
	// �|�C���^��錾
	CStage  *pStage = CScene::GetStage();	// �X�e�[�W�̏��
	CPlayer *pPlayer = CScene::GetPlayer();	// �v���C���[�̏��
	CEnemyBossDragon *pBoss = GetBoss();	// �{�X�̏��

	CStage::SStageLimit stageLimit = pStage->GetStageLimit();	// �X�e�[�W�͈͏��
	D3DXVECTOR3 posPlayer = pPlayer->GetVec3Position();			// �v���C���[�̈ʒu
	D3DXVECTOR3 posEnemy = VEC3_ZERO;	// �G�̐ݒ�ʒu
	D3DXVECTOR3 rotEnemy = VEC3_ZERO;	// �G�̐ݒ����

	// �G�̈ʒu�𒆉��ɐݒ�
	posEnemy.x += stageLimit.center.x;
	posEnemy.y = TELEPORT_POSY;
	posEnemy.z += stageLimit.center.z;

	// �v���C���[��������������
	D3DXVECTOR3 vecPlayer = posEnemy - posPlayer;
	rotEnemy.y = atan2f(vecPlayer.x, vecPlayer.z);

	// �{�X���e���|�[�g������
	pBoss->SetTeleport(posEnemy, rotEnemy, CEnemyBossDragon::MOTION_FLY_IDOL);

	// �e���|�[�g��Ԃɂ���
	m_state = STATE_TELEPORT;
}

//============================================================
//	�����˂̍X�V����
//============================================================
void CEnemyAttack03::UpdateThunder(void)
{
	// �|�C���^��錾
	CPlayer *pPlayer = CScene::GetPlayer();	// �v���C���[�̏��
	CEnemyBossDragon *pBoss = GetBoss();	// �{�X�̏��

	// �v���C���[����{�X�ւ̃x�N�g�������߂�
	D3DXVECTOR3 vecPlayer = pBoss->GetVec3Position() - pPlayer->GetVec3Position();

	// �ڕW�������擾
	D3DXVECTOR3 rotDestEnemy = pBoss->GetDestRotation();

	// �ڕW�������v���C���[�����ɂ���
	rotDestEnemy.y = atan2f(vecPlayer.x, vecPlayer.z);

	// �ڕW������ݒ�
	pBoss->SetDestRotation(rotDestEnemy);

	// �J�E���^�[�����Z
	m_nCounterWait++;
	if (m_nCounterWait > m_nWaitFrame)
	{ // �]�C�t���[�����I�������ꍇ

		// �J�E���^�[��������
		m_nCounterWait = 0;

		// �]�C�̒�����Z�k
		m_nWaitFrame -= SUB_WAIT_FRAME;

		CListManager<CPlayer> *pList = CPlayer::GetList();		// �v���C���[���X�g
		if (pList == nullptr)		 { assert(false); return; }	// ���X�g���g�p
		if (pList->GetNumAll() != 1) { assert(false); return; }	// �v���C���[��1�l����Ȃ�
		auto player = pList->GetList().front();					// �v���C���[���

		// ���U�����v���C���[�ʒu�ɐ���
		int nWarnFrame = attackThunder::WARN_FRAME - (MAX_SUB_WARN_WAIT / NUM_ATTACK) * m_nCounterNumAtk;	// �x���\���t���[��
		CAttackThunder::Create(player->GetVec3Position(), nWarnFrame);

		// �U���񐔂����Z
		m_nCounterNumAtk++;
		if (m_nCounterNumAtk >= NUM_ATTACK)
		{ // �X�e�[�W�[�ɓ��B�����ꍇ

			// �I����Ԃɂ���
			m_state = STATE_END;
		}
	}
}
