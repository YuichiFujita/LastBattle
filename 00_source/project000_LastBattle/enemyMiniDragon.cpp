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

//============================================================
//	���f���t�@�C���擾����
//============================================================
const char *CEnemyMiniDragon::GetModelFileName(const int nModel) const
{
	if (nModel > NONE_IDX && nModel < MODEL_MAX)
	{ // �g�p�ł���C���f�b�N�X�̏ꍇ

		// �����̃C���f�b�N�X�̃��f����Ԃ�
		return MODEL_FILE[nModel];
	}
	else { assert(false); return NONE_STRING; }	// �͈͊O
}

//============================================================
//	���[�V�����E�I�u�W�F�N�g�L�����N�^�[�̍X�V����
//============================================================
void CEnemyMiniDragon::UpdateMotion(const int nMotion)
{
	if (nMotion <= NONE_IDX || nMotion >= MOTION_MAX) { assert(false); return; }	// ���[�V���������ݒ�
	if (IsDeath()) { return; }	// ���S���Ă���

	if (IsMotionLoop())
	{ // ���[�v���郂�[�V�����̏ꍇ

		if (GetMotionType() != nMotion)
		{ // ���݂̃��[�V�������Đ����̃��[�V�����ƈ�v���Ȃ��ꍇ

			// ���݂̃��[�V�����̐ݒ�
			SetMotion(nMotion);
		}
	}
	else
	{ // ���[�v���Ȃ����[�V�����������ꍇ

#if 0
		switch (GetMotionType())
		{ // ���[�V�������Ƃ̏���
		case MOTION_ATTACK:	// �U�����[�V�����F���[�vOFF
			break;

		case MOTION_LAND:	// ���n���[�V�����F���[�vOFF

			if (nMotion != MOTION_IDOL)
			{ // �ҋ@���[�V�����ł͂Ȃ��ꍇ

				// ���݂̃��[�V�����̐ݒ�
				SetMotion(nMotion);
			}

			break;

		default:	// ��O����
			assert(false);
			break;
		}
#endif
	}

	// �I�u�W�F�N�g�L�����N�^�[�̍X�V
	CObjectChara::Update();

#if 0
	switch (GetMotionType())
	{ // ���[�V�������Ƃ̏���
	case MOTION_IDOL:	// �ҋ@���[�V�����F���[�vON
		break;

	case MOTION_DASH:	// �_�b�V�����[�V�����F���[�vOFF
	case MOTION_LAND:	// ���n���[�V�����F���[�vOFF

		if (IsMotionFinish())
		{ // ���[�V�������I�����Ă����ꍇ

			// ���݂̃��[�V�����̐ݒ�
			SetMotion(nMotion);
		}

		break;

	case MOTION_ATTACK:	// �U�����[�V�����F���[�vOFF
		break;

	default:	// ��O����
		assert(false);
		break;
	}
#endif
}
