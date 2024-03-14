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
CAttackParabolaFire::CAttackParabolaFire(const D3DXVECTOR3 &rPosDest) :
	m_posDest		(rPosDest),		// �ڕW�ʒu
	m_posOrigin		(VEC3_ZERO),	// ���_�ʒu
	m_fLength		(0.0f),			// �ړ�����
	m_fPhi			(0.0f),			// ���ʊp
	m_fTheta		(0.0f),			// �p
	m_fMove			(0.0f),			// �ړ���
	m_nCounterTime	(0)				// �U�����ԊǗ��J�E���^�[
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
	m_posOrigin	= VEC3_ZERO;	// ���_�ʒu
	m_fLength	= 0.0f;			// �ړ�����
	m_fPhi		= 0.0f;			// ���ʊp
	m_fTheta	= 0.0f;			// �p
	m_fMove		= 0.0f;			// �ړ���
	m_nCounterTime = 0;			// �U�����ԊǗ��J�E���^�[

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
void CAttackParabolaFire::Draw(CShader *pShader)
{
	// ���̕`��
	CFire::Draw(pShader);
}

//============================================================
//	��������
//============================================================
CAttackParabolaFire *CAttackParabolaFire::Create
(
	const D3DXVECTOR3& rCurPos,		// �����ʒu
	const D3DXVECTOR3& rDestPos,	// �ڕW�ʒu
	const float fMove				// �ړ���
)
{
	// �J�[�u�U�����̐���
	CAttackParabolaFire *pAttackParabolaFire = new CAttackParabolaFire(rDestPos);
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

		// �����ʒu��ݒ�
		pAttackParabolaFire->SetVec3Position(rCurPos);
		pAttackParabolaFire->m_posOrigin = rCurPos;

		// ���̈ړ��p�����[�^�[��������
		pAttackParabolaFire->InitParabolaParam(rCurPos);

		// �ړ��ʂ�ݒ�
		pAttackParabolaFire->m_fMove = fMove;

		// �m�ۂ����A�h���X��Ԃ�
		return pAttackParabolaFire;
	}
}

//============================================================
//	���ړ��p�����[�^�[�̏���������
//============================================================
void CAttackParabolaFire::InitParabolaParam(const D3DXVECTOR3& rCurPos)
{
	D3DXVECTOR3 vecLength = m_posDest - rCurPos;		// �ڕW�ʒu�ւ̃x�N�g��
	m_fLength = D3DXVec3Length(&vecLength);				// �x�N�g���̒���
	useful::VecToRot(vecLength, &m_fPhi, &m_fTheta);	// �x�N�g���������ɕϊ�
}

//============================================================
//	���ړ��ʂ̍X�V����
//============================================================
void CAttackParabolaFire::UpdateParabolaMove(void)
{
#if 0
#if 0
#if 0
	// ���݂̌o�ߎ��Ԃ��牊�̈ʒu�����߂�
	float fMaxTime;	// �o�ߎ���
	D3DXVECTOR2 pos = useful::CalcPosParabola(GRAVITY, m_fMove, m_posDest.x, m_posDest.y, (float)m_nCounterTime, &fMaxTime);

	if ((float)m_nCounterTime <= fMaxTime)
	{ // �ő�o�ߎ��Ԃɓ��B���Ă��Ȃ��ꍇ

		// �ʒu�𔽉f
		SetVec3Position(D3DXVECTOR3(pos.x, pos.y, 0.0f));
	}
#else
	// ���݂̌o�ߎ��Ԃ��牊�̈ړ��ʂ����߂�
	float fMaxTime;	// �o�ߎ���
	D3DXVECTOR2 move = useful::CalcMoveParabola(GRAVITY, m_fMove, m_posDest.x, m_posDest.y, (float)m_nCounterTime, &fMaxTime);

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
#else
	// ���݂̌o�ߎ��Ԃ��牊�̈ʒu�����߂�
	float fMaxTime;	// �o�ߎ���
	D3DXVECTOR2 posPara = useful::CalcPosParabola(GRAVITY, m_fMove, m_fLength, 1000.0f, (float)m_nCounterTime, &fMaxTime);

	if ((float)m_nCounterTime <= fMaxTime)
	{ // �ő�o�ߎ��Ԃɓ��B���Ă��Ȃ��ꍇ

		D3DXVECTOR2 posPara = useful::CalcPosParabola(GRAVITY, m_fMove, m_fLength, 1000.0f, fMaxTime);

		D3DXVECTOR3 pos;	// �ʒu
		D3DXMATRIX mtxTrans, mtxRot, mtxRoot, mtxWorld;	// �}�g���b�N�X�v�Z�p

		// ������
		D3DXMatrixIdentity(&mtxWorld);

		// Yaw�i���ʊp�j��Pitch�i�p�j�����]�s����쐬
		//D3DXMatrixRotationYawPitchRoll(&mtxRot, m_fPhi, m_fTheta, 0.0f);
		//D3DXMatrixMultiply(&mtxWorld, &mtxRot, &mtxWorld);

		// ���ʊp���f
		D3DXMatrixRotationX(&mtxRoot, m_fPhi);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRoot);

		// �p���f
		D3DXMatrixRotationY(&mtxRot, m_fTheta);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

		// �ʒu���f
		D3DXMatrixTranslation(&mtxTrans, 0.0f, posPara.y, -posPara.x);
		D3DXMatrixMultiply(&mtxWorld, &mtxTrans, &mtxWorld);

		//D3DXMatrixMultiply(&mtxWorld, &mtxRot, &mtxTrans);

		// �ʒu��ݒ�
		pos = useful::GetMatrixPosition(mtxWorld);

		// �ʒu�𔽉f
		SetVec3Position(pos + m_posOrigin);

		GET_MANAGER->GetDebugProc()->Print(CDebugProc::POINT_CENTER, "%f %f %f\n", pos.x, pos.y, pos.z);
	}
#endif
#endif

	// �U�����˂���̌o�߃J�E���^�[�����Z
	m_nCounterTime++;
}
