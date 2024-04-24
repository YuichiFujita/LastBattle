//============================================================
//
//	���i�U�������� [attackMoveFire.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "attackMoveFire.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{

}

//************************************************************
//	�q�N���X [CAttackMoveFire] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CAttackMoveFire::CAttackMoveFire() :
	m_dirMove	(VEC3_ZERO),	// �ړ������x�N�g��
	m_fMove		(0.0f),			// �ړ���
	m_fAddMove	(0.0f)			// ������
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CAttackMoveFire::~CAttackMoveFire()
{

}

//============================================================
//	����������
//============================================================
HRESULT CAttackMoveFire::Init(void)
{
	// �����o�ϐ���������
	m_dirMove	= VEC3_ZERO;	// �ړ������x�N�g��
	m_fMove		= 0.0f;			// �ړ���
	m_fAddMove	= 0.0f;			// ������

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
void CAttackMoveFire::Uninit(void)
{
	// ���̏I��
	CFire::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CAttackMoveFire::Update(void)
{
	// �ړ��̍X�V
	UpdateMove();

	// ���̍X�V
	CFire::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CAttackMoveFire::Draw(CShader *pShader)
{
	// ���̕`��
	CFire::Draw(pShader);
}

//============================================================
//	��������
//============================================================
CAttackMoveFire *CAttackMoveFire::Create
(
	const D3DXVECTOR3& rPos,		// �����ʒu
	const D3DXVECTOR3& rVecMove,	// �ړ�����
	const float fMove,				// �ړ���
	const float fAddMove,			// ������
	const int nLife					// ����
)
{
	// ���i�U�����̐���
	CAttackMoveFire *pAttackMoveFire = new CAttackMoveFire;
	if (pAttackMoveFire == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// ���i�U�����̏�����
		if (FAILED(pAttackMoveFire->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ���i�U�����̔j��
			SAFE_DELETE(pAttackMoveFire);
			return nullptr;
		}

		// �ʒu��ݒ�
		pAttackMoveFire->SetVec3Position(rPos);

		// �ړ�������ݒ�
		pAttackMoveFire->SetMoveDir(rVecMove);

		// �ړ��ʂ�ݒ�
		pAttackMoveFire->SetMoveValue(fMove);

		// �����ʂ�ݒ�
		pAttackMoveFire->SetAddMoveValue(fAddMove);

		// ������ݒ�
		pAttackMoveFire->SetLife(nLife);

		// �m�ۂ����A�h���X��Ԃ�
		return pAttackMoveFire;
	}
}

//============================================================
//	�ړ������̐ݒ菈��
//============================================================
void CAttackMoveFire::SetMoveDir(const D3DXVECTOR3& rMove)
{
	// �ړ�������ݒ�
	m_dirMove = rMove;

	// �ړ������𐳋K��
	D3DXVec3Normalize(&m_dirMove, &m_dirMove);
}

//============================================================
//	�ړ��̍X�V����
//============================================================
void CAttackMoveFire::UpdateMove(void)
{
	// �ړ��ʂ𔽉f
	SetMove(m_dirMove * m_fMove);

	// �ړ��ʂ�����
	m_fMove += m_fAddMove;
}
