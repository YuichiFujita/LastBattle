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
#include "sceneGame.h"
#include "gameManager.h"

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

	const int ATK_WAIT_FRAME = 60;	// �U���̗]�C�t���[��
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
	m_pAttack			(nullptr),		// �U���̏��
	m_stateNormal		(NSTATE_NONE),	// �ʏ��Ԏ��̏��
	m_nCounterNormal	(0)				// �ʏ��ԊǗ��J�E���^�[
{

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
	m_pAttack			= nullptr;		// �U���̏��
	m_stateNormal		= NSTATE_WAIT;	// �ʏ��Ԏ��̏��
	m_nCounterNormal	= 0;			// �ʏ��ԊǗ��J�E���^�[

	// �G�̏�����
	if (FAILED(CEnemy::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �X�|�[����Ԃɂ���
	SetState(STATE_SPAWN);

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
	// TODO�F�U���������������
	switch (m_stateNormal)
	{ // �ʏ펞�̏�Ԃ��Ƃ̏���
	case NSTATE_WAIT:

		// �J�E���^�[�����Z
		m_nCounterNormal++;
		if (m_nCounterNormal > ATK_WAIT_FRAME)
		{ // �U���̗]�C���I�������ꍇ

			// �J�E���^�[��������
			m_nCounterNormal = 0;

			// �U���̐���
			m_pAttack = CEnemyAttack::Create((CEnemyAttack::EAttack)(rand() % CEnemyAttack::ATTACK_MAX), this);
			if (m_pAttack == nullptr) { assert(false); return; }	// �������s

			// �U����Ԃɂ���
			m_stateNormal = NSTATE_ATTACK;
		}

		break;

	case NSTATE_ATTACK:

		if (m_pAttack != nullptr)
		{ // �U�������݂���ꍇ

			// �U���̍X�V
			if (m_pAttack->Update())
			{ // �U�����I�����Ă���ꍇ

				// �U���̔j��
				SAFE_REF_RELEASE(m_pAttack);

				// �ҋ@��Ԃɂ���
				m_stateNormal = NSTATE_WAIT;
			}
		}

		break;

	default:
		assert(false);
		break;
	}
}
