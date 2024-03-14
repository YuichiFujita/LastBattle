//============================================================
//
//	�G�U��06���� [enemyAttack06.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "enemyAttack06.h"
#include "enemyBossDragon.h"
#include "manager.h"
#include "sound.h"
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
		CEnemyBossDragon::MODEL_TAIL_00,	// �K��00
		CEnemyBossDragon::MODEL_TAIL_01,	// �K��01
		CEnemyBossDragon::MODEL_TAIL_02,	// �K��02
		CEnemyBossDragon::MODEL_TAIL_03,	// �K��03
		CEnemyBossDragon::MODEL_TAIL_04,	// �K��04
		CEnemyBossDragon::MODEL_TAIL_05,	// �K��05
	};

	const float FIND_RADIUS		 = 250.0f;	// �v���C���[���m���a
	const float TELEPORT_POS_DIS = 125.0f;	// �e���|�[�g���̃v���C���[�ʒu���牓�����鋗��
	const float	SCALE_TAIL		 = 1.65f;	// �K���̊g�嗦
	const float	SCALE_COLL_PLUS	 = 2.25f;	// �K���̔���̊g�嗦
	const int	UP_MOTION_KEY	 = 0;		// �K���g��L�[
	const int	UP_MOTION_CNT	 = 0;		// �K���g��J�E���^�[
	const int	DOWN_MOTION_KEY	 = 5;		// �K���k���L�[
	const int	DOWN_MOTION_CNT	 = 1;		// �K���k���J�E���^�[
	const int	DMG_TAIL		 = 15;		// �K���̃_���[�W��
}

//************************************************************
//	�q�N���X [CEnemyAttack06] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEnemyAttack06::CEnemyAttack06(CEnemyBossDragon *pBoss) : CEnemyAttack(pBoss),
	m_state	(STATE_INIT_TELEPORT)	// ���
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CEnemyAttack06::~CEnemyAttack06()
{

}

//============================================================
//	����������
//============================================================
HRESULT CEnemyAttack06::Init(void)
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
void CEnemyAttack06::Uninit(void)
{
	// �G�U���̏I��
	CEnemyAttack::Uninit();
}

//============================================================
//	�X�V����
//============================================================
bool CEnemyAttack06::Update(void)
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
		// �����ۍU���̍s����ݒ�
		pBoss->SetActTailAttack();

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
void CEnemyAttack06::SetRandomArray
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
		pRandom->AddList(ATTACK_05, 2);	// �U��05(�Ђ������U��)

		// �������U����ǉ�
		pRandom->AddList(ATTACK_02, 2);	// �U��02(���O������������)
		pRandom->AddList(ATTACK_03, 1);	// �U��03(���v���C���[�ʒu����)
		pRandom->AddList(ATTACK_04, 2);	// �U��04(���O���f���o��)
	}
	else
	{ // �v���C���[�Ƃ̋����������ꍇ

		// �������U����ǉ�
		pRandom->AddList(ATTACK_02, 1);	// �U��02(���O������������)
		pRandom->AddList(ATTACK_03, 2);	// �U��03(���v���C���[�ʒu����)
		pRandom->AddList(ATTACK_04, 1);	// �U��04(���O���f���o��)
	}
}

//============================================================
//	�e���|�[�g���邩�̐ݒ菈��
//============================================================
void CEnemyAttack06::SetEnableTeleport(void)
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
void CEnemyAttack06::InitTeleport(void)
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
void CEnemyAttack06::UpdateAttack(void)
{
	// �|�C���^��錾
	CPlayer *pPlayer = CScene::GetPlayer();	// �v���C���[�̏��
	CEnemyBossDragon *pBoss = GetBoss();	// �{�X�̏��

	// �U���̏I������
	if (pBoss->GetMotionType() != CEnemyBossDragon::MOTION_TAIL_ATTACK)
	{ // �����ۃ��[�V�������I������ꍇ

		// �����ۂ̃T�C�Y�����ɖ߂�
		pBoss->GetMultiModel(CEnemyBossDragon::MODEL_TAIL_00)->SetVec3Scaling(VEC3_ALL(1.0f));

		// �I����Ԃɂ���
		m_state = STATE_END;

		// �����𔲂���
		return;
	}

	// �����ۂ̃T�C�Y�ύX����
	if (pBoss->GetMotionKey() == UP_MOTION_KEY && pBoss->GetMotionKeyCounter() == UP_MOTION_CNT)
	{ // �����ۂ�U��~�낵�n�߂��^�C�~���O�̏ꍇ

		// �����ۂ̃T�C�Y���g��
		pBoss->GetMultiModel(CEnemyBossDragon::MODEL_TAIL_00)->SetVec3Scaling(VEC3_ALL(SCALE_TAIL));

		// �����ۉ��̍Đ�
		PLAY_SOUND(CSound::LABEL_SE_TAIL);
	}
	else if (pBoss->GetMotionKey() == DOWN_MOTION_KEY && pBoss->GetMotionKeyCounter() == DOWN_MOTION_CNT)
	{ // �����ۂ�U��~�낵�n�߂��^�C�~���O�̏ꍇ

		// �����ۂ̃T�C�Y�����ɖ߂�
		pBoss->GetMultiModel(CEnemyBossDragon::MODEL_TAIL_00)->SetVec3Scaling(VEC3_ALL(1.0f));
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
					posCollEnemy,	// ����ʒu
					posCentPlayer,	// ����ڕW�ʒu
					coll.fRadius * scaleParts.x * SCALE_COLL_PLUS,	// ���蔼�a
					pPlayer->GetRadius()							// ����ڕW���a
				);
				if (bHit)
				{ // �U�������������ꍇ

					// �v���C���[�̃q�b�g����
					D3DXVECTOR3 vecKnock = posCentPlayer - pBoss->GetVec3Position();
					pPlayer->HitKnockBack(DMG_TAIL, vecKnock);
				}
			}
		}
	}
}
