//============================================================
//
//	�J�[�u�U�������� [attackParabolaFire.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "attackParabolaFire.h"
#include "player.h"

// TODO
#include "manager.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const float	GRAVITY = -1.0f;	// �d��
}

//************************************************************
//	�q�N���X [CAttackParabolaFire] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CAttackParabolaFire::CAttackParabolaFire() :
	m_nCounterTime	(0),	// �U�����ԊǗ��J�E���^�[
	m_fMove			(0.0f)	// �ړ���
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CAttackParabolaFire::~CAttackParabolaFire()
{

}

//============================================================
//	����������
//============================================================
HRESULT CAttackParabolaFire::Init(void)
{
	// �����o�ϐ���������
	m_nCounterTime = 0;	// �U�����ԊǗ��J�E���^�[
	m_fMove = 0.0f;		// �ړ���

	// ���̏�����
	if (FAILED(CFire::Init()))
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
void CAttackParabolaFire::Uninit(void)
{
	// ���̏I��
	CFire::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CAttackParabolaFire::Update(void)
{
	// ���ړ��ʂ̍X�V
	UpdateParabolaMove();

	// ���̍X�V
	CFire::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CAttackParabolaFire::Draw(void)
{
	// ���̕`��
	CFire::Draw();
}

//============================================================
//	��������
//============================================================
CAttackParabolaFire *CAttackParabolaFire::Create
(
	const D3DXVECTOR3& rPos,	// �ʒu
	const float fMove			// �ړ���
)
{
	// �J�[�u�U�����̐���
	CAttackParabolaFire *pAttackParabolaFire = new CAttackParabolaFire;
	if (pAttackParabolaFire == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �J�[�u�U�����̏�����
		if (FAILED(pAttackParabolaFire->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �J�[�u�U�����̔j��
			SAFE_DELETE(pAttackParabolaFire);
			return nullptr;
		}

		// �ʒu��ݒ�
		pAttackParabolaFire->SetVec3Position(rPos);

		// �ړ��ʂ�ݒ�
		pAttackParabolaFire->m_fMove = fMove;

		// �m�ۂ����A�h���X��Ԃ�
		return pAttackParabolaFire;
	}
}

//============================================================
//	���ړ��ʂ̍X�V����
//============================================================
void CAttackParabolaFire::UpdateParabolaMove(void)
{
	float posMaxX = 1000.0f;
	float posMaxY = 300.0f;
	float fMaxTime, fMaxPosY;

#if 1
	// ���݂̌o�ߎ��Ԃ��牊�̈ʒu�����߂�
	D3DXVECTOR2 pos = useful::CalcPosParabola(GRAVITY, m_fMove, posMaxX, posMaxY, (float)m_nCounterTime, &fMaxTime, &fMaxPosY);

	if ((float)m_nCounterTime <= fMaxTime)
	{ // �ő�o�ߎ��Ԃɓ��B���Ă��Ȃ��ꍇ

		// �ʒu�𔽉f
		SetVec3Position(D3DXVECTOR3(pos.x, pos.y, 0.0f));
	}
#else
	// ���݂̌o�ߎ��Ԃ��牊�̈ړ��ʂ����߂�
	D3DXVECTOR2 move = useful::CalcMoveParabola(GRAVITY, m_fMove, posMaxX, posMaxY, (float)m_nCounterTime, &fMaxTime, &fMaxPosY);

	if ((float)m_nCounterTime <= fMaxTime)
	{ // �ő�o�ߎ��Ԃɓ��B���Ă��Ȃ��ꍇ

		// �ړ��ʂ𔽉f
		SetMove(D3DXVECTOR3(move.x, move.y, 0.0f));
	}
	else
	{ // �ő�o�ߎ��Ԃɓ��B���Ă��Ȃ��ꍇ

		// �ړ��ʂ�������
		SetMove(VEC3_ZERO);
	}
#endif

	// �U�����˂���̃J�E���^�[�����Z
	m_nCounterTime++;
}
