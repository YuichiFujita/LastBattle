//============================================================
//
//	�G�U������ [enemyAttack.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "enemyAttack.h"
#include "manager.h"
#include "enemyBossDragon.h"
#include "wave.h"

//************************************************************
//	�e�N���X [CEnemyAttack] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEnemyAttack::CEnemyAttack() : m_pBoss(nullptr)
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CEnemyAttack::~CEnemyAttack()
{

}

//============================================================
//	����������
//============================================================
HRESULT CEnemyAttack::Init(void)
{
	// �����o�ϐ���������
	m_pBoss = nullptr;	// �{�X�ւ̃|�C���^

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CEnemyAttack::Uninit(void)
{

}

//============================================================
//	��������
//============================================================
CEnemyAttack *CEnemyAttack::Create(const EAttack attackID, CEnemyBossDragon *pBoss)
{
	// �|�C���^��錾
	CEnemyAttack *pEnemyAttack = nullptr;	// �G�U�����

	// �G�U���̐���
	switch (attackID)
	{ // ��ނ��Ƃ̏���
	case ATTACK_00:
		pEnemyAttack = new CEnemyAttack00;
		break;

	//case ATTACK_01:
		//pEnemyAttack = new CEnemyAttack01;
		//break;

	default:	// ��O����
		assert(false);
		break;
	}

	if (pEnemyAttack == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �G�U���̏�����
		if (FAILED(pEnemyAttack->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �G�U���̔j��
			SAFE_DELETE(pEnemyAttack);
			return nullptr;
		}

		// �{�X�ւ̃|�C���^��ݒ�
		pEnemyAttack->m_pBoss = pBoss;

		// �m�ۂ����A�h���X��Ԃ�
		return pEnemyAttack;
	}
}

//============================================================
//	�j������
//============================================================
void CEnemyAttack::Release(CEnemyAttack *&prEnemyAttack)
{
	// �G�U���̏I��
	assert(prEnemyAttack != nullptr);
	prEnemyAttack->Uninit();

	// �������J��
	SAFE_DELETE(prEnemyAttack);
}

//============================================================
//	�{�X�擾����
//============================================================
CEnemyBossDragon *CEnemyAttack::GetBossDragon(void)
{
	// �{�X�̏���Ԃ�
	return m_pBoss;
}

//************************************************************
//	�q�N���X [CEnemyAttack00] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEnemyAttack00::CEnemyAttack00() :
	m_pWave			(nullptr),		// �g���̏��
	m_state			(STATE_WAVE),	// ���
	m_nCounterState (0),			// ��ԊǗ��J�E���^�[
	m_nCreateWave	(0)				// �g���̐�����
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CEnemyAttack00::~CEnemyAttack00()
{

}

//============================================================
//	����������
//============================================================
HRESULT CEnemyAttack00::Init(void)
{
	// �����o�ϐ���������
	m_pWave = nullptr;		// �g���̏��
	m_state = STATE_WAVE;	// ���
	m_nCounterState	= 0;	// ��ԊǗ��J�E���^�[
	m_nCreateWave	= 0;	// �g���̐�����

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
void CEnemyAttack00::Uninit(void)
{
	// �G�U���̏I��
	CEnemyAttack::Uninit();
}

//============================================================
//	�X�V����
//============================================================
bool CEnemyAttack00::Update(void)
{
	// TODO�F�U���������������
	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_WAVE:

		if (m_pWave != nullptr)
		{ // �g��������ꍇ

			// �g���̑��x���グ��
			CWave::SGrow grow = m_pWave->GetGrow();
			m_pWave->SetGrow(CWave::SGrow(grow.fAddHoleRadius + 0.1f, grow.fAddThickness, grow.fSubAlpha));
		}

		// �J�E���^�[�����Z
		m_nCounterState++;
		if (m_nCounterState > 120)
		{ // �U���̗]�C���I�������ꍇ

			// �J�E���^�[��������
			m_nCounterState = 0;

			if (m_pWave != nullptr)
			{ // �g��������ꍇ

				// �����ɂ��Ă����悤�ɂ���
				m_pWave->SetGrow(CWave::SGrow(6.0f, 0.0f, 0.05f));
			}

			// �g���̐����������Z
			m_nCreateWave++;
			if (m_nCreateWave > 3)
			{ // �����d�؂����ꍇ

				// ��������������
				m_nCreateWave = 0;

				// �ҋ@��Ԃɂ���
				m_state = STATE_WAIT;

				return false;
			}
			
			// �g���̐���
			m_pWave = CWave::Create
			( // ����
				CWave::TEXTURE_NONE,
				GetBossDragon()->GetVec3Position(),
				VEC3_ZERO,
				XCOL_BLUE,
				CWave::SGrow(2.0f * (m_nCreateWave + 1), 0.0f, 0.0f),
				POSGRID2(64, 1),
				POSGRID2(8, 1),
				100.0f,
				0.0f, 
				25.0f
			);
			if (m_pWave == nullptr)
			{ // �����Ɏ��s�����ꍇ

				assert(false);
				return false;
			}
		}

		break;

	case STATE_WAIT:

		// �J�E���^�[�����Z
		m_nCounterState++;
		if (m_nCounterState > 120)
		{ // �U���̗]�C���I�������ꍇ

			// �I����Ԃɂ���
			m_state = STATE_END;
		}

	case STATE_END:

		// �U���I����Ԃ�
		return true;

	default:
		assert(false);
		break;
	}

	return false;
}
