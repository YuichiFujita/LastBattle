//============================================================
//
//	�{�X�h���S������ [enemyBossDragon.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "enemyBossDragon.h"
#include "enemyAttack.h"
#include "manager.h"
#include "renderer.h"
#include "stage.h"
#include "multiModel.h"
#include "sceneGame.h"
#include "gameManager.h"
#include "magicCircle.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char *MODEL_FILE[] =	// ���f���t�@�C��
	{
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\00_waist.x",		// ��
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\01_body.x",		// ��
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\02_jaw.x",		// �{
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\03_head.x",		// ��
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\04_armUL.x",		// ����r
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\05_armUR.x",		// �E��r
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\06_armDL.x",		// �����r
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\07_armDR.x",		// �E���r
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\08_handL.x",		// ����
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\09_handR.x",		// �E��
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\10_legUL.x",		// ��������
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\11_legUR.x",		// �E������
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\12_legDL.x",		// ����
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\13_legDR.x",		// �E��
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\14_footL.x",		// ����
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\15_footR.x",		// �E��
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\16_wingL_00.x",	// ����00
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\17_wingR_00.x",	// �E��00
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\18_wingL_01.x",	// ����01
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\19_wingR_01.x",	// �E��01
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\20_wingL_02.x",	// ����02
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\21_wingR_02.x",	// �E��02
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\22_wingL_03.x",	// ����03
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\23_wingR_03.x",	// �E��03
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\24_tail_00.x",	// �K��00
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\25_tail_01.x",	// �K��01
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\26_tail_02.x",	// �K��02
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\27_tail_03.x",	// �K��03
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\28_tail_04.x",	// �K��04
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\29_tail_05.x",	// �K��05
	};

	const int	PRIORITY		= 6;			// �{�X�h���S���̗D�揇��
	const int	ATK_WAIT_FRAME	= 60;			// �U���̗]�C�t���[��
	const float	SCALE_MAGIC		= 55.0f;		// ���@�w�̔��a�ϓ���
	const float	MOVE_MAGIC		= 50.0f;		// ���@�w�̏㉺�ړ���
	const float	MAGIC_CIRCLE_RADIUS	= 250.0f;	// ���@�w�̔��a
	const float	MAGIC_ALPHA_RADIUS	= 350.0f;	// ���@�w�̔��a
	const float	MAGIC_DELPOS_PLUSY	= 150.0f;	// ���@�w�̏����ʒu�̉��Z��Y
}

//************************************************************
//	�X�^�e�B�b�N�A�T�[�g
//************************************************************
static_assert(NUM_ARRAY(MODEL_FILE) == CEnemyBossDragon::MODEL_MAX, "ERROR : Model Count Mismatch");

//************************************************************
//	�q�N���X [CEnemyBossDragon] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEnemyBossDragon::CEnemyBossDragon(const EType type) : CEnemy(type),
	m_pAttack			(nullptr),	// �U���̏��
	m_pMagicCircle		(nullptr),	// ���@�w�̏��
	m_action			(ACT_NONE),	// �s��
	m_nCounterAttack	(0)			// �U���Ǘ��J�E���^�[
{
	// �����o�ϐ����N���A
	memset(&m_teleport, 0, sizeof(m_teleport));	// �e���|�[�g�̏��
}

//============================================================
//	�f�X�g���N�^
//============================================================
CEnemyBossDragon::~CEnemyBossDragon()
{

}

//============================================================
//	����������
//============================================================
HRESULT CEnemyBossDragon::Init(void)
{
	// �����o�ϐ���������
	memset(&m_teleport, 0, sizeof(m_teleport));	// �e���|�[�g�̏��
	m_pAttack			= nullptr;	// �U���̏��
	m_pMagicCircle		= nullptr;	// ���@�w�̏��
	m_action			= ACT_NONE;	// �s��
	m_nCounterAttack	= 0;		// �U���Ǘ��J�E���^�[

	// �G�̏�����
	if (FAILED(CEnemy::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �D�揇�ʂ�ݒ�
	SetPriority(PRIORITY);

	// �X�|�[����Ԃɂ���
	SetState(STATE_SPAWN);

	// ���@�w�̐���
	m_pMagicCircle = CMagicCircle::Create(VEC3_ZERO, VEC3_ZERO, 0.0f, MAGIC_ALPHA_RADIUS);
	if (m_pMagicCircle == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �����`���OFF�ɂ���
	m_pMagicCircle->SetEnableDraw(false);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CEnemyBossDragon::Uninit(void)
{
	// �U���̔j��
	SAFE_REF_RELEASE(m_pAttack);

	// �G�̏I��
	CEnemy::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CEnemyBossDragon::Update(void)
{
	// �G�̍X�V
	CEnemy::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CEnemyBossDragon::Draw(void)
{
	// �G�̕`��
	CEnemy::Draw();
}

//============================================================
//	�q�b�g����
//============================================================
void CEnemyBossDragon::Hit(const int nDamage)
{
	// �q�b�g����
	CEnemy::Hit(nDamage);

	if (GetLife() <= 0)
	{ // �̗͂��Ȃ��Ȃ����ꍇ

		if (GET_MANAGER->GetMode() == CScene::MODE_GAME)
		{ // �Q�[����ʂ̏ꍇ

			// �Q�[����ʂ��I����Ԃɂ���
			CSceneGame::GetGameManager()->SetState(CGameManager::STATE_END);
		}
	}
}

//============================================================
//	�m�b�N�o�b�N�q�b�g����
//============================================================
void CEnemyBossDragon::HitKnockBack(const int nDamage, const D3DXVECTOR3& vecKnock)
{
	// �m�b�N�o�b�N�q�b�g����
	CEnemy::HitKnockBack(nDamage, vecKnock);
	
	if (GetLife() <= 0)
	{ // �̗͂��Ȃ��Ȃ����ꍇ

		if (GET_MANAGER->GetMode() == CScene::MODE_GAME)
		{ // �Q�[����ʂ̏ꍇ

			// �Q�[����ʂ��I����Ԃɂ���
			CSceneGame::GetGameManager()->SetState(CGameManager::STATE_END);
		}
	}
}

//============================================================
//	�e���|�[�g�̐ݒ菈��
//============================================================
void CEnemyBossDragon::SetTeleport(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rRot)
{
	// �e���|�[�g��̈ʒu�E������ۑ�
	m_teleport.pos = rPos;
	m_teleport.rot = rRot;

	// ���@�w�̈ʒu�E���������݈ʒu�̑����ɐݒ�
	m_pMagicCircle->SetVec3Position(GetVec3Position());
	m_pMagicCircle->SetVec3Rotation(GetVec3Rotation());

	// ���@�w�̎����`���ON�ɂ���
	m_pMagicCircle->SetEnableDraw(true);

	// ���@�w�̏o����Ԃɂ���
	m_teleport.state = TELEPORT_APPEAR;

	// ���@�w�ɂ��t�F�[�h�C���̍s�����Ƃ点��
	m_action = ACT_MAGIC_FADEIN;
}

//============================================================
//	���f���t�@�C���擾����
//============================================================
const char *CEnemyBossDragon::GetModelFileName(const int nModel) const
{
	if (nModel > NONE_IDX && nModel < MODEL_MAX)
	{ // �g�p�ł���C���f�b�N�X�̏ꍇ

		// �����̃C���f�b�N�X�̃��f����Ԃ�
		return MODEL_FILE[nModel];
	}
	else { assert(false); return NONE_STRING; }	// �͈͊O
}

//============================================================
//	�ʏ��Ԏ��̍X�V����
//============================================================
void CEnemyBossDragon::UpdateNormal(void)
{
	// TODO�F�e���|�[�g������
	if (GET_INPUTKEY->IsTrigger(DIK_0))
	{
		SetTeleport(VEC3_ZERO, VEC3_ZERO);
	}

	// �U���̍X�V
	UpdateAttack();

	// �s���̍X�V
	UpdateAction();
}

//============================================================
//	�U���̍X�V����
//============================================================
void CEnemyBossDragon::UpdateAttack(void)
{
	if (m_pAttack == nullptr)
	{ // �U�������݂��Ȃ��ꍇ

		// �J�E���^�[�����Z
		m_nCounterAttack++;
		if (m_nCounterAttack > ATK_WAIT_FRAME)
		{ // �U���̗]�C���I�������ꍇ

			// �J�E���^�[��������
			m_nCounterAttack = 0;

			// �U���̐���
			m_pAttack = CEnemyAttack::Create
			( // ����
				(CEnemyAttack::EAttack)(rand() % CEnemyAttack::ATTACK_MAX),	// �U���C���f�b�N�X
				this	// ���g�̃|�C���^
			);
			if (m_pAttack == nullptr)
			{ // �����Ɏ��s�����ꍇ

				// �֐��𔲂���
				assert(false);
				return;
			}
		}
	}
	else
	{ // �U�������݂���ꍇ

		// �U���̍X�V
		if (m_pAttack->Update())
		{ // �U�����I�����Ă���ꍇ

			// �U���̔j��
			SAFE_REF_RELEASE(m_pAttack);

			// �������Ȃ��s�����Ƃ点��
			m_action = ACT_NONE;
		}
	}
}

//============================================================
//	�s���̍X�V����
//============================================================
void CEnemyBossDragon::UpdateAction(void)
{
	// �ϐ���錾
	D3DXVECTOR3 posEnemy = GetVec3Position();	// �G�ʒu
	D3DXVECTOR3 rotEnemy = GetVec3Rotation();	// �G����

	// �|�C���^��錾
	CStage *pStage = CScene::GetStage();				// �X�e�[�W���
	if (pStage == nullptr) { assert(false); return; }	// �X�e�[�W��g�p��

	// �ߋ��ʒu�̍X�V
	UpdateOldPosition();

	// �d�͂̍X�V
	UpdateGravity();

	switch (m_action)
	{ // �s�����Ƃ̏���
	case ACT_NONE:	// �������Ȃ�
		break;

	case ACT_MAGIC_FADEIN:	// ���@�w�t�F�[�h�C��

		// ���@�w�t�F�[�h�C���s�����̍X�V
		UpdateMagicFadeIn(&posEnemy, &rotEnemy);

		break;

	case ACT_MAGIC_FADEOUT:	// ���@�w�t�F�[�h�A�E�g

		// ���@�w�t�F�[�h�A�E�g�s�����̍X�V
		UpdateMagicFadeOut(posEnemy);

		break;

	case ACT_PUNCH_GROUND:	// �n�ʉ���

		// �n�ʉ���s�����̍X�V
		UpdatePunchGround();

		break;

	default:	// ��O����
		assert(false);
		break;
	}

	// �ʒu�X�V
	UpdatePosition(&posEnemy);

	// ���n����
	UpdateLanding(&posEnemy);

	// �X�e�[�W�͈͊O�̕␳
	pStage->LimitPosition(posEnemy, GetStatusInfo().fRadius);

	// �ʒu�𔽉f
	SetVec3Position(posEnemy);

	// �����𔽉f
	SetVec3Rotation(rotEnemy);
}

//============================================================
//	���@�w�t�F�[�h�C���s�����̍X�V����
//============================================================
void CEnemyBossDragon::UpdateMagicFadeIn(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pRot)
{
	// �ϐ���錾
	float fMagicPosY = m_pMagicCircle->GetVec3Position().y;	// ���@�w�̈ʒu

	switch (m_teleport.state)
	{ // �e���|�[�g��Ԃ��Ƃ̏���
	case TELEPORT_APPEAR:	// ���@�w�̏o��
	{
		// �ϐ���錾
		float fMagicRadius = m_pMagicCircle->GetRadius();	// ���@�w�̔��a

		// ���@�w���g��
		fMagicRadius += SCALE_MAGIC;
		if (fMagicRadius >= MAGIC_CIRCLE_RADIUS)
		{ // �g�債�������ꍇ

			// ���a��␳
			fMagicRadius = MAGIC_CIRCLE_RADIUS;

			// ���@�w�̈ړ���Ԃɂ���
			m_teleport.state = TELEPORT_MOVE;
		}

		// ���@�w�̔��a�𔽉f
		m_pMagicCircle->SetRadius(fMagicRadius);

		break;
	}
	case TELEPORT_MOVE:	// ���@�w�̈ړ�
	{
		// �ϐ���錾
		float fDelPosY = GetMultiModel(MODEL_HEAD)->GetMtxWorld()._42 + MAGIC_DELPOS_PLUSY;	// ���@�w�̏����ʒu

		// ���@�w����ɓ�����
		fMagicPosY += MOVE_MAGIC;
		if (fMagicPosY >= fDelPosY)
		{ // �����ʒu�ɓ��B�����ꍇ

			// �ʒu��␳
			fMagicPosY = fDelPosY;

			// ���@�w�̏�����Ԃɂ���
			m_teleport.state = TELEPORT_DISAPPEAR;
		}

		break;
	}
	case TELEPORT_DISAPPEAR:	// ���@�w�̏���
	{
		// �ϐ���錾
		float fMagicRadius = m_pMagicCircle->GetRadius();	// ���@�w�̔��a

		// ���@�w���k��
		fMagicRadius -= SCALE_MAGIC;
		if (fMagicRadius <= 0.0f)
		{ // �k�����������ꍇ

			// ���a��␳
			fMagicRadius = 0.0f;

			// �{�X�̈ʒu�E�������e���|�[�g��ɕύX
			*pPos = m_teleport.pos;
			*pRot = m_teleport.rot;

			// ���@�w�̏o����Ԃɂ���
			m_teleport.state = TELEPORT_APPEAR;

			// ���@�w�ɂ��t�F�[�h�A�E�g�̍s�����Ƃ点��
			m_action = ACT_MAGIC_FADEOUT;
		}

		// ���@�w�̔��a�𔽉f
		m_pMagicCircle->SetRadius(fMagicRadius);

		break;
	}
	default:	// ��O����
		assert(false);
		break;
	}

	// ���@�w�̈ʒu�𔽉f
	m_pMagicCircle->SetVec3Position(D3DXVECTOR3(pPos->x, fMagicPosY, pPos->z));
}

//============================================================
//	���@�w�t�F�[�h�A�E�g�s�����̍X�V����
//============================================================
void CEnemyBossDragon::UpdateMagicFadeOut(const D3DXVECTOR3& rPos)
{
	// �ϐ���錾
	float fMagicPosY = m_pMagicCircle->GetVec3Position().y;	// ���@�w�̈ʒu

	switch (m_teleport.state)
	{ // �e���|�[�g��Ԃ��Ƃ̏���
	case TELEPORT_APPEAR:	// ���@�w�̏o��
	{
		// �ϐ���錾
		float fMagicRadius = m_pMagicCircle->GetRadius();	// ���@�w�̔��a

		// ���@�w���g��
		fMagicRadius += SCALE_MAGIC;
		if (fMagicRadius >= MAGIC_CIRCLE_RADIUS)
		{ // �g�債�������ꍇ

			// ���a��␳
			fMagicRadius = MAGIC_CIRCLE_RADIUS;

			// ���@�w�̈ړ���Ԃɂ���
			m_teleport.state = TELEPORT_MOVE;
		}

		// ���@�w�̔��a�𔽉f
		m_pMagicCircle->SetRadius(fMagicRadius);

		break;
	}
	case TELEPORT_MOVE:	// ���@�w�̈ړ�
	{
		// �ϐ���錾
		float fDelPosY = GetVec3Position().y;	// ���@�w�̏����ʒu

		// ���@�w�����ɓ�����
		fMagicPosY -= MOVE_MAGIC;
		if (fMagicPosY <= fDelPosY)
		{ // �����ʒu�ɓ��B�����ꍇ

			// �ʒu��␳
			fMagicPosY = fDelPosY;

			// ���@�w�̏�����Ԃɂ���
			m_teleport.state = TELEPORT_DISAPPEAR;
		}

		break;
	}
	case TELEPORT_DISAPPEAR:	// ���@�w�̏���
	{
		// �ϐ���錾
		float fMagicRadius = m_pMagicCircle->GetRadius();	// ���@�w�̔��a

		// ���@�w���k��
		fMagicRadius -= SCALE_MAGIC;
		if (fMagicRadius <= 0.0f)
		{ // �k�����������ꍇ

			// ���a��␳
			fMagicRadius = 0.0f;

			// ���@�w�̎����`���OFF�ɂ���
			m_pMagicCircle->SetEnableDraw(false);

			// �������Ȃ��s�����Ƃ点��
			m_action = ACT_NONE;
		}

		// ���@�w�̔��a�𔽉f
		m_pMagicCircle->SetRadius(fMagicRadius);

		break;
	}
	default:	// ��O����
		assert(false);
		break;
	}

	// ���@�w�̈ʒu�𔽉f
	m_pMagicCircle->SetVec3Position(D3DXVECTOR3(rPos.x, fMagicPosY, rPos.z));
}

//============================================================
//	�n�ʉ���s�����̍X�V����
//============================================================
void CEnemyBossDragon::UpdatePunchGround(void)
{

}
