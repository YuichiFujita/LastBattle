//============================================================
//
//	�G�U��05���� [enemyAttack05.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "enemyAttack05.h"
#include "enemyBossDragon.h"
#include "player.h"
#include "stage.h"
#include "multiModel.h"
#include "collision.h"
#include "collSphere.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const CEnemyBossDragon::EModel COLL_PARTS[] =	// �U���������������p�[�c
	{
		CEnemyBossDragon::MODEL_ARM_UL,	// ����r
		CEnemyBossDragon::MODEL_ARM_DL,	// �����r
		CEnemyBossDragon::MODEL_HAND_L,	// ����
	};

	const float FIND_RADIUS		 = 250.0f;	// �v���C���[���m���a
	const float TELEPORT_POS_DIS = 125.0f;	// �e���|�[�g���̃v���C���[�ʒu���牓�����鋗��
	const float	SCALE_LEFT_ARM	 = 1.25f;	// ���r�̊g�嗦
	const float	SCALE_LEFT_HAND	 = 1.4f;	// ����̊g�嗦
	const int	SCALE_MOTION_KEY = 1;		// ���r�g��L�[
	const int	DMG_CLAW		 = 15;		// �܂̃_���[�W��
}

//************************************************************
//	�q�N���X [CEnemyAttack05] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEnemyAttack05::CEnemyAttack05(CEnemyBossDragon *pBoss) : CEnemyAttack(pBoss),
	m_state	(STATE_INIT_TELEPORT)	// ���
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CEnemyAttack05::~CEnemyAttack05()
{

}

//============================================================
//	����������
//============================================================
HRESULT CEnemyAttack05::Init(void)
{
	// �����o�ϐ���������
	m_state = STATE_INIT_TELEPORT;	// ���

	// �G�U���̏�����
	if (FAILED(CEnemyAttack::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e���|�[�g���邩��ݒ�
	SetEnableTeleport();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CEnemyAttack05::Uninit(void)
{
	// �G�U���̏I��
	CEnemyAttack::Uninit();
}

//============================================================
//	�X�V����
//============================================================
bool CEnemyAttack05::Update(void)
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

			// �U���̏�������Ԃɂ���
			m_state = STATE_INIT_ATTACK;
		}

		break;
	}
	case STATE_INIT_ATTACK:	// �U���̏�����
	{
		// �Ђ������U���̍s����ݒ�
		pBoss->SetActClawAttack();

		// �U����Ԃɂ���
		m_state = STATE_ATTACK;

		// �����͔������U���̏�ԍX�V�Ɉڍs
	}
	case STATE_ATTACK:	// �U��
	{
		// �U���̍X�V
		UpdateAttack();

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
void CEnemyAttack05::SetRandomArray
(
	CRandom<EAttack> *pRandom,	// �����_���U���N���X
	EAttack oldAtk,				// �O��̍U��
	int nSameAct				// �����s���̘A����
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

		// ����U����ǉ�
		if (oldAtk == ATTACK_05)
		{ // �O��U���������Ɠ���̏ꍇ

			pRandom->AddList(ATTACK_05, 5 - nSameAct);	// �U��05(�Ђ������U��)
		}

		// �ߋ����U����ǉ�
		pRandom->AddList(ATTACK_06, 2);	// �U��06(�����ۍU��)

		// �������U����ǉ�
		pRandom->AddList(ATTACK_02, 1);	// �U��02(���O������������)
		pRandom->AddList(ATTACK_03, 1);	// �U��03(���v���C���[�ʒu����)
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

//============================================================
//	�e���|�[�g���邩�̐ݒ菈��
//============================================================
void CEnemyAttack05::SetEnableTeleport(void)
{
	// �|�C���^��錾
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

		// �U���̏�������Ԃɂ���
		m_state = STATE_INIT_ATTACK;
	}
	else
	{ // �v���C���[�Ƃ̋����������ꍇ

		// �e���|�[�g�̏�������Ԃɂ���
		m_state = STATE_INIT_TELEPORT;
	}
}

//============================================================
//	�e���|�[�g�̏���������
//============================================================
void CEnemyAttack05::InitTeleport(void)
{
	// �|�C���^��錾
	CStage  *pStage  = CScene::GetStage();	// �X�e�[�W�̏��
	CPlayer *pPlayer = CScene::GetPlayer();	// �v���C���[�̏��
	CEnemyBossDragon *pBoss = GetBoss();	// �{�X�̏��

	CStage::SStageLimit stageLimit = pStage->GetStageLimit();	// �X�e�[�W�͈͏��
	D3DXVECTOR3 posPlayer = pPlayer->GetVec3Position();			// �v���C���[�̈ʒu
	D3DXVECTOR3 rotPlayer = pPlayer->GetVec3Rotation();			// �v���C���[�̌���
	D3DXVECTOR3 posEnemy = VEC3_ZERO;	// �G�̐ݒ�ʒu
	D3DXVECTOR3 rotEnemy = VEC3_ZERO;	// �G�̐ݒ����

	// �v���C���[�̔w��Ɉʒu��ݒ�
	posEnemy.x += posPlayer.x + sinf(rotPlayer.y) * TELEPORT_POS_DIS;
	posEnemy.z += posPlayer.z + cosf(rotPlayer.y) * TELEPORT_POS_DIS;

	// �v���C���[��������������
	D3DXVECTOR3 vecPlayer = posEnemy - posPlayer;
	rotEnemy.y = atan2f(vecPlayer.x, vecPlayer.z);

	// �{�X���e���|�[�g������
	pBoss->SetTeleport(posEnemy, rotEnemy);

	// �e���|�[�g��Ԃɂ���
	m_state = STATE_TELEPORT;
}

//============================================================
//	�U���̍X�V����
//============================================================
void CEnemyAttack05::UpdateAttack(void)
{
	// �|�C���^��錾
	CPlayer *pPlayer = CScene::GetPlayer();	// �v���C���[�̏��
	CEnemyBossDragon *pBoss = GetBoss();	// �{�X�̏��

	// �U���̏I������
	if (pBoss->GetMotionType() != CEnemyBossDragon::MOTION_CLAW_ATTACK)
	{ // �Ђ��������[�V�������I������ꍇ

		// ��̃T�C�Y�����ɖ߂�
		pBoss->GetMultiModel(CEnemyBossDragon::MODEL_ARM_UL)->SetVec3Scaling(VEC3_ALL(1.0f));
		pBoss->GetMultiModel(CEnemyBossDragon::MODEL_HAND_L)->SetVec3Scaling(VEC3_ALL(1.0f));

		// �I����Ԃɂ���
		m_state = STATE_END;

		// �����𔲂���
		return;
	}

	// ��̋��剻����
	if (pBoss->GetMotionKey() == SCALE_MOTION_KEY && pBoss->GetMotionKeyCounter() == 0)
	{ // ���U��~�낵�n�߂��^�C�~���O�̏ꍇ

		// ��̃T�C�Y���g��
		pBoss->GetMultiModel(CEnemyBossDragon::MODEL_ARM_UL)->SetVec3Scaling(VEC3_ALL(SCALE_LEFT_ARM));
		pBoss->GetMultiModel(CEnemyBossDragon::MODEL_HAND_L)->SetVec3Scaling(VEC3_ALL(SCALE_LEFT_HAND));
	}

	// �U������
	if (pBoss->IsLeftWeaponCollision())
	{ // �U�����肪ON�̏ꍇ

		// �v���C���[����ʒu�����߂�
		D3DXVECTOR3 heightPlayer  = D3DXVECTOR3(0.0f, pPlayer->GetHeight(), 0.0f);		// �v���C���[�̏c��
		D3DXVECTOR3 posCentPlayer = pPlayer->GetVec3Position() + (heightPlayer * 0.5f);	// �v���C���[�̒��S�ʒu

		for (int nCntColl = 0; nCntColl < NUM_ARRAY(COLL_PARTS); nCntColl++)
		{ // ������Ƃ�p�[�c�����J��Ԃ�

			// �p�[�c�g�嗦���擾
			D3DXMATRIX  mtxParts = pBoss->GetMultiModel(COLL_PARTS[nCntColl])->GetMtxWorld();	// �p�[�c�}�g���b�N�X
			D3DXVECTOR3 scaleParts = useful::GetMatrixScaling(mtxParts);	// �p�[�c�g�嗦

			// ����z����擾
			CCollSphere *pColl = pBoss->GetCollision(COLL_PARTS[nCntColl]);	// �~������
			std::vector<CCollSphere::SInfo> vector = pColl->GetVector();	// �~����z��

			// ������̗v�f�ԍ�
			int nCntArray = 0;

			for (auto coll : vector)
			{ // �z��̗v�f�����J��Ԃ�

				// �G�̔���ʒu���v�Z
				D3DXVECTOR3 posCollEnemy = pColl->CalcWorldPosition(nCntArray);

				// �v�f�ԍ�������
				nCntArray++;

				// �U���̓����蔻��
				bool bHit = collision::Circle3D
				( // ����
					posCollEnemy,					// ����ʒu
					posCentPlayer,					// ����ڕW�ʒu
					coll.fRadius * scaleParts.x,	// ���蔼�a
					pPlayer->GetRadius()			// ����ڕW���a
				);
				if (bHit)
				{ // �U�������������ꍇ

					// �v���C���[�̃q�b�g����
					pPlayer->Hit(DMG_CLAW);
				}
			}
		}
	}
}
