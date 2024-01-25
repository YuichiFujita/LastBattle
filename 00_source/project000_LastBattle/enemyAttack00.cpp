//============================================================
//
//	�G�U��00���� [enemyAttack00.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "enemyAttack00.h"
#include "enemyBossDragon.h"
#include "manager.h"
#include "multiModel.h"
#include "wave.h"

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
	m_state = STATE_SET;	// ���
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
	// �|�C���^��錾
	CEnemyBossDragon *pBoss = GetBoss();	// �{�X�̏��

	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_SET:

		// �n�ʉ���̍s����ݒ�
		pBoss->SetActPunchGround();

		// �g�����ˏ�Ԃɂ���
		m_state = STATE_WAVE;

		// ���̂܂܏����𔲂����Ɏ���

	case STATE_WAVE:

		if (m_pWave != nullptr)
		{ // �g��������ꍇ

			// �g���̑��x���グ��
			CWave::SGrow grow = m_pWave->GetGrow();
			m_pWave->SetGrow(CWave::SGrow(grow.fAddHoleRadius + 0.25f, grow.fAddThickness, grow.fSubAlpha));
		}

		if (pBoss->GetMotionKey() == pBoss->GetMotionNumKey() - 1
		&&  pBoss->GetMotionKeyCounter() == 0)
		{ // �U���̗]�C���I�������ꍇ

			if (m_pWave != nullptr)
			{ // �g��������ꍇ

				// �����ɂ��Ă����悤�ɂ���
				m_pWave->SetGrow(CWave::SGrow(6.0f, 0.0f, 0.05f));
			}

			//// �g���̐����������Z
			//m_nCreateWave++;
			//if (m_nCreateWave > 3)
			//{ // �����d�؂����ꍇ

			//	// ��������������
			//	m_nCreateWave = 0;

				// �ҋ@��Ԃɂ���
				m_state = STATE_WAIT;

			//	return false;
			//}
			
			// �g���̐���
			D3DXMATRIX  mtx = pBoss->GetMultiModel(CEnemyBossDragon::MODEL_HAND_L)->GetMtxWorld();
			D3DXVECTOR3 pos = D3DXVECTOR3(mtx._41, pBoss->GetVec3Position().y, mtx._43);
			m_pWave = CWave::Create
			( // ����
				CWave::TEXTURE_NONE,
				pos,
				VEC3_ZERO,
				XCOL_BLUE,
				CWave::SGrow(6.0f, 0.0f, 0.0f),
				POSGRID2(64, 1),
				POSGRID2(8, 1),
				10.0f,
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

		break;

	case STATE_END:

		// �U���I����Ԃ�
		return true;

	default:
		assert(false);
		break;
	}

	return false;
}
