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
		"data\\MODEL\\ENEMY\\boss_dragon000.x",	// �{�X�h���S��
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

	// ���f���̓o�^�E����
	BindModel(MODEL_FILE[MODEL_NORMAL]);

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
			m_pAttack = CEnemyAttack::Create((CEnemyAttack::EAttack)(rand() % 1), this);
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
