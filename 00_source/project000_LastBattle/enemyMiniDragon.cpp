//============================================================
//
//	�~�j�h���S������ [enemyMiniDragon.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "enemyMiniDragon.h"
#include "manager.h"
#include "renderer.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char *MODEL_FILE[] =	// ���f���t�@�C��
	{
		"data\\MODEL\\ENEMY\\mini_dragon000.x",	// �~�j�h���S��
	};
}

//************************************************************
//	�X�^�e�B�b�N�A�T�[�g
//************************************************************
static_assert(NUM_ARRAY(MODEL_FILE) == CEnemyMiniDragon::MODEL_MAX, "ERROR : Model Count Mismatch");

//************************************************************
//	�q�N���X [CEnemyMiniDragon] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEnemyMiniDragon::CEnemyMiniDragon(const EType type) : CEnemy(type)
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CEnemyMiniDragon::~CEnemyMiniDragon()
{

}

//============================================================
//	����������
//============================================================
HRESULT CEnemyMiniDragon::Init(void)
{
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
void CEnemyMiniDragon::Uninit(void)
{
	// �G�̏I��
	CEnemy::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CEnemyMiniDragon::Update(void)
{
	// �ߋ��ʒu�̍X�V
	UpdateOldPosition();

	// �d�͂̍X�V
	UpdateGravity();

	switch (GetState())
	{ // ��Ԃ��Ƃ̏���
	case STATE_SPAWN:	// ���G���

		// �X�|�[������̍X�V
		UpdateSpawn();

		break;

	case STATE_NORMAL:	// �ʏ���

		// �ʏ퓮��̍X�V
		UpdateNormal();

		break;

	case STATE_DAMAGE:	// �_���[�W���

		// �_���[�W����̍X�V
		UpdateDamage();

		break;

	case STATE_INVULN:	// ���G���

		// ���G���쎞�̍X�V
		UpdateInvuln();

		break;

	case STATE_DEATH:	// ���S���

		// ���S���쎞�̍X�V
		UpdateDeath();

		break;

	default:	// ��O����
		assert(false);
		break;
	}

	// �G�̍X�V
	CEnemy::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CEnemyMiniDragon::Draw(void)
{
	// �G�̕`��
	CEnemy::Draw();
}
