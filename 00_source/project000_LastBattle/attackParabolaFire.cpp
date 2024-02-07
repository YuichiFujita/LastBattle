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
	const float	MOVE	= 16.0f;	// ���̈ړ���
	const float	REV_ROT	= 0.08f;	// ���̌����ύX�␳�W��
}

//************************************************************
//	�q�N���X [CAttackParabolaFire] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CAttackParabolaFire::CAttackParabolaFire()
{
	nCnt = 0;
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
CAttackParabolaFire *CAttackParabolaFire::Create(const D3DXVECTOR3& rPos)
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
	const float v0x = 10.0f;
	const float g = -1.0f;

	nCnt++;

#if 0
	if (nCnt <= posMaxX / v0x)
	{
		// �ړ��ʂ𔽉f
		D3DXVECTOR2 pos = CalcParabola(g, v0x, posMaxX, posMaxY, (float)nCnt);
		SetVec3Position(D3DXVECTOR3(pos.x, pos.y, 0.0f));
	}
	else
	{
		SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}
#else
	if (nCnt <= posMaxX / v0x)
	{
		// �ړ��ʂ𔽉f
		D3DXVECTOR2 move = CalcMoveParabola(g, v0x, posMaxX, posMaxY, (float)nCnt);
		SetMove(D3DXVECTOR3(move.x, move.y, 0.0f));
	}
	else
	{
		SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}
#endif

	GET_MANAGER->GetDebugProc()->Print(CDebugProc::POINT_CENTER, "%f %f %f\n", GetVec3Position().x, GetVec3Position().y, GetVec3Position().z);
}

D3DXVECTOR2 CAttackParabolaFire::CalcParabola(float g, float v0x, float maxX, float maxY, float t)
{
	D3DXVECTOR2 vecRet = VEC2_ZERO;

	// x���������߂�
	vecRet.x = v0x * t;

	// �^�C���ő�l�����߂�
	const float tMax = maxX / v0x;
	const float v0y  = -0.5f * g * tMax;

	float timeHalf = tMax * 0.5f;
	float tempMaxY = 0.5f * g * (timeHalf * timeHalf) + v0y * timeHalf;
	float rateMaxY = maxY / tempMaxY;

	// y���������߂�
	vecRet.y = (0.5f * g * (t * t) + v0y * t) * rateMaxY;

	return vecRet;
}

D3DXVECTOR2 CAttackParabolaFire::CalcMoveParabola(float g, float v0x, float maxX, float maxY, float t)
{
	D3DXVECTOR2 vecRet = VEC2_ZERO;

	// x���������߂�
	vecRet.x = v0x;

	// �^�C���ő�l�����߂�
	const float tMax = maxX / v0x;
	const float v0y = -0.5f * g * tMax;

	float timeHalf = tMax * 0.5f;
	float tempMaxY = 0.5f * g * (timeHalf * timeHalf) + v0y * timeHalf;
	float rateMaxY = maxY / tempMaxY;

	// y���������߂�
	vecRet.y = (g * t + v0y) * rateMaxY;

	return vecRet;
}
