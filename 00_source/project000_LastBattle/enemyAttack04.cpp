//============================================================
//
//	�G�U��04���� [enemyAttack04.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "enemyAttack04.h"
#include "enemyBossDragon.h"
#include "player.h"
#include "stage.h"
#include "multiModel.h"
#include "attackMoveFire.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int	ATK_MOTION_KEY	= 2;	// �U�������L�[
	const int	WAIT_FRAME		= 150;	// �U���ҋ@�t���[��
	const int	NUM_FIRE		= 8;	// ���̐�����
	const int	NUM_ATTACK		= 3;	// �U���̐�����
}

//************************************************************
//	�q�N���X [CEnemyAttack04] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEnemyAttack04::CEnemyAttack04(CEnemyBossDragon *pBoss) : CEnemyAttack(pBoss),
	m_state				(STATE_INIT_TELEPORT),	// ���
	m_nCounterWait		(0),					// �]�C�Ǘ��J�E���^�[
	m_nCounterNumAtk	(0)						// �U���񐔃J�E���^�[
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CEnemyAttack04::~CEnemyAttack04()
{

}

//============================================================
//	����������
//============================================================
HRESULT CEnemyAttack04::Init(void)
{
	// �����o�ϐ���������
	m_state				= STATE_INIT_TELEPORT;	// ���
	m_nCounterWait		= 0;					// �]�C�Ǘ��J�E���^�[
	m_nCounterNumAtk	= 0;					// �U���񐔃J�E���^�[

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
void CEnemyAttack04::Uninit(void)
{
	// �G�U���̏I��
	CEnemyAttack::Uninit();
}

//============================================================
//	�X�V����
//============================================================
bool CEnemyAttack04::Update(void)
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
		// ���U���̍s����ݒ�
		//pBoss->SetActFire();

		// �U���ҋ@��Ԃɂ���
		m_state = STATE_WAIT;

		// �����͔������U���ҋ@�̏�ԍX�V�Ɉڍs
	}
	case STATE_WAIT:	// �U���ҋ@
	{
		if (pBoss->GetMotionKey() == 2
		&&  pBoss->GetMotionKeyCounter() == 1)
		{ // ���U��グ���^�C�~���O�̏ꍇ

			// �����ˏ�Ԃɂ���
			m_state = STATE_FIRE;
		}

		break;
	}
	case STATE_FIRE:	// ������
	{
		// �����˂̍X�V
		UpdateFire();

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
//	�e���|�[�g�̏���������
//============================================================
void CEnemyAttack04::InitTeleport(void)
{
	// �|�C���^��錾
	CStage  *pStage  = CScene::GetStage();	// �X�e�[�W�̏��
	CPlayer *pPlayer = CScene::GetPlayer();	// �v���C���[�̏��
	CEnemyBossDragon *pBoss = GetBoss();	// �{�X�̏��

	CStage::SStageLimit stageLimit = pStage->GetStageLimit();	// �X�e�[�W�͈͏��
	D3DXVECTOR3 posPlayer = pPlayer->GetVec3Position();			// �v���C���[�̈ʒu
	D3DXVECTOR3 posEnemy = VEC3_ZERO;	// �G�̐ݒ�ʒu
	D3DXVECTOR3 rotEnemy = VEC3_ZERO;	// �G�̐ݒ����

	// ���S����v���C���[�ւ̌��������߂�
	D3DXVECTOR3 vec = stageLimit.center - posPlayer;
	float fRot = atan2f(vec.x, vec.z);

	// �ő�����������ʒu��ݒ�
	posEnemy.x += stageLimit.center.x + sinf(fRot) * stageLimit.fRadius * 0.5f;
	posEnemy.z += stageLimit.center.z + cosf(fRot) * stageLimit.fRadius * 0.5f;

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
void CEnemyAttack04::UpdateFire(void)
{
	// �J�E���^�[�����Z
	m_nCounterWait++;
	if (m_nCounterWait > WAIT_FRAME)
	{ // �]�C�t���[�����I�������ꍇ

		// �J�E���^�[��������
		m_nCounterWait = 0;

		// ���̐���
		CreateFire();

		// �U���񐔂����Z
		m_nCounterNumAtk++;
		if (m_nCounterNumAtk >= NUM_ATTACK)
		{ // �U�����X�e�[�W�[�ɓ��B�����ꍇ

			// �I����Ԃɂ���
			m_state = STATE_END;
		}
	}
}

//============================================================
//	���̐�������
//============================================================
void CEnemyAttack04::CreateFire(void)
{
	CEnemyBossDragon *pBoss	= GetBoss();	// �{�X�̏��
	CMultiModel	*pObjJaw	= pBoss->GetMultiModel(CEnemyBossDragon::MODEL_JAW);	// �{���f��
	D3DXMATRIX	mtxJaw		= pObjJaw->GetMtxWorld();				// �{�}�g���b�N�X
	D3DXVECTOR3	posJaw		= useful::GetMatrixPosition(mtxJaw);	// �{���[���h���W

	// TODO�F���[�V�����ς��������
	posJaw.y = 100.0f;

	int nNumFire = NUM_FIRE + m_nCounterNumAtk * 3;	// �U��������

	float fFf = D3DX_PI * 0.25f;
	float fRotRate = (D3DX_PI - fFf * 2.0f) / (nNumFire - 1);	// �U�������̊���

	float fStartRot	= pBoss->GetVec3Rotation().y + (D3DX_PI * 0.5f) + fFf;	// �U�������J�n����

	for (int nCntAttack = 0; nCntAttack < nNumFire; nCntAttack++)
	{ // �U���̐��������J��Ԃ�

		float fMoveRot = fStartRot + fRotRate * nCntAttack;	// ���̐�������
		D3DXVECTOR3 vecMove = D3DXVECTOR3(sinf(fMoveRot), 0.0f, cosf(fMoveRot));	// ���̈ړ�����

		// ���U���̐���
		CAttackMoveFire::Create
		( // ����
			posJaw,		// �����ʒu
			vecMove,	// �ړ�����
			16.5f,		// �ړ���
			0.09f,		// ������
			100			// ����
		);
	}
}
