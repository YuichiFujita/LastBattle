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
#include "attackThunder.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const float	TELEPORT_POS_DIS	= 3600.0f;	// �e���|�[�g���̃X�e�[�W���S�ʒu���牓�����鋗��
	const float	TELEPORT_POSY		= 800.0f;	// �e���|�[�g����Y���W

	const int	INIT_WAIT_FRAME	= 26;		// �����U���ҋ@�t���[��
	const int	NUM_ATTACK		= 8;		// �U���̐�����
	const int	ADD_WAIT_FRAME	= 4;		// ��U�����Ƃ̍U���ҋ@�t���[�����Z��
	const int	MUL_NUM_CREATE	= 6;		// ��U�����Ƃ̍U���������̏�Z��
	const float	CREATE_LENGTH	= 240.0f;	// ��U�����Ƃ̋����̏�Z��
}

//************************************************************
//	�q�N���X [CEnemyAttack02] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEnemyAttack02::CEnemyAttack02() :
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
			{ // �X�e�[�W�[�ɓ��B�����ꍇ

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
