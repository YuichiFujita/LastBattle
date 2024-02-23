//============================================================
//
//	�G�U��02���� [enemyAttack02.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "enemyAttack02.h"
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
	const int	ATK_MOTION_KEY	= 2;		// �U�������L�[
	const int	INIT_WAIT_FRAME	= 26;		// �����U���ҋ@�t���[��
	const int	NUM_ATTACK		= 8;		// �U���̐�����
	const int	ADD_WAIT_FRAME	= 4;		// ��U�����Ƃ̍U���ҋ@�t���[�����Z��
	const int	MUL_NUM_CREATE	= 9;		// ��U�����Ƃ̍U���������̏�Z��
	const float	CREATE_LENGTH	= 240.0f;	// ��U�����Ƃ̋����̏�Z��
	const float FIND_RADIUS		= 250.0f;	// �v���C���[���m���a
}

//************************************************************
//	�q�N���X [CEnemyAttack02] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEnemyAttack02::CEnemyAttack02(CEnemyBossDragon *pBoss) : CEnemyAttack(pBoss),
	m_state				(STATE_INIT_TELEPORT),	// ���
	m_nCounterWait		(0),					// �]�C�Ǘ��J�E���^�[
	m_nCounterNumAtk	(0),					// �U���񐔃J�E���^�[
	m_nWaitFrame		(0)						// �]�C�t���[����
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CEnemyAttack02::~CEnemyAttack02()
{

}

//============================================================
//	����������
//============================================================
HRESULT CEnemyAttack02::Init(void)
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
void CEnemyAttack02::Uninit(void)
{
	// �G�U���̏I��
	CEnemyAttack::Uninit();
}

//============================================================
//	�X�V����
//============================================================
bool CEnemyAttack02::Update(void)
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

			// �U���ҋ@�̏�������Ԃɂ���
			m_state = STATE_INIT_WAIT;
		}

		break;
	}
	case STATE_INIT_WAIT:	// �U���ҋ@�̏�����
	{
		// �󒆍U���̍s����ݒ�
		pBoss->SetActFlyAttack();

		// �U���ҋ@��Ԃɂ���
		m_state = STATE_WAIT;

		// �����͔������U���ҋ@�̏�ԍX�V�Ɉڍs
	}
	case STATE_WAIT:	// �U���ҋ@
	{
		if (pBoss->GetMotionKey() == ATK_MOTION_KEY
		&&  pBoss->GetMotionKeyCounter() == 1)
		{ // ���U��グ���^�C�~���O�̏ꍇ

			// �����ˏ�Ԃɂ���
			m_state = STATE_THUNDER;
		}

		break;
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
void CEnemyAttack02::SetRandomArray
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
void CEnemyAttack02::InitTeleport(void)
{
	// �|�C���^��錾
	CStage  *pStage  = CScene::GetStage();	// �X�e�[�W�̏��
	CPlayer *pPlayer = CScene::GetPlayer();	// �v���C���[�̏��
	CEnemyBossDragon *pBoss = GetBoss();	// �{�X�̏��

	CStage::SStageLimit stageLimit = pStage->GetStageLimit();	// �X�e�[�W�͈͏��
	D3DXVECTOR3 posPlayer = pPlayer->GetVec3Position();			// �v���C���[�̈ʒu
	D3DXVECTOR3 posEnemy = VEC3_ZERO;	// �G�̐ݒ�ʒu
	D3DXVECTOR3 rotEnemy = VEC3_ZERO;	// �G�̐ݒ����

	// �G�̈ʒu�𒆉��ɐݒ�
	posEnemy.x += stageLimit.center.x;
	posEnemy.z += stageLimit.center.z;

	// �v���C���[��������������
	D3DXVECTOR3 vecPlayer = posEnemy - posPlayer;
	rotEnemy.y = atan2f(vecPlayer.x, vecPlayer.z);

	// �{�X���e���|�[�g������
	pBoss->SetTeleport(posEnemy, rotEnemy);

	// �e���|�[�g��Ԃɂ���
	m_state = STATE_TELEPORT;
}

//============================================================
//	�����˂̍X�V����
//============================================================
void CEnemyAttack02::UpdateThunder(void)
{
	// �J�E���^�[�����Z
	m_nCounterWait++;
	if (m_nCounterWait > m_nWaitFrame)
	{ // �]�C�t���[�����I�������ꍇ

		// �J�E���^�[��������
		m_nCounterWait = 0;

		// �]�C�̒���������
		m_nWaitFrame += ADD_WAIT_FRAME;

		int   nLoop = m_nCounterNumAtk * MUL_NUM_CREATE + 1;	// �U��������
		float fRotRate = (D3DX_PI * 2.0f) / nLoop;				// �U�������̊���
		for (int nCntAttack = 0; nCntAttack < nLoop; nCntAttack++)
		{ // �����������J��Ԃ�

			float fThunderRot = fRotRate * nCntAttack;	// ���̐�������
			D3DXVECTOR3 posThunder = VEC3_ZERO;			// ���̐����ʒu
			posThunder.x = sinf(fThunderRot) * m_nCounterNumAtk * CREATE_LENGTH;
			posThunder.z = cosf(fThunderRot) * m_nCounterNumAtk * CREATE_LENGTH;

			// ���U���������_���ʒu�ɐ���
			CAttackThunder::Create(posThunder);
		}

		// �U���񐔂����Z
		m_nCounterNumAtk++;
		if (m_nCounterNumAtk >= NUM_ATTACK)
		{ // �U�����X�e�[�W�[�ɓ��B�����ꍇ

			// �I����Ԃɂ���
			m_state = STATE_END;
		}
	}
}
