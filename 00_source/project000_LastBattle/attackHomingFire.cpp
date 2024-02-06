//============================================================
//
//	�z�[�~���O�U�������� [attackHomingFire.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "attackHomingFire.h"
#include "player.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const float	MOVE	= 7.6f;		// ���̈ړ���
	const float	REV_ROT	= 0.08f;	// ���̌����ύX�␳�W��
}

//************************************************************
//	�q�N���X [CAttackHomingFire] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CAttackHomingFire::CAttackHomingFire()
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CAttackHomingFire::~CAttackHomingFire()
{

}

//============================================================
//	����������
//============================================================
HRESULT CAttackHomingFire::Init(void)
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
void CAttackHomingFire::Uninit(void)
{
	// ���̏I��
	CFire::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CAttackHomingFire::Update(void)
{
	// ���ړ��ʂ̍X�V
	UpdateHomingMove();

	// ���̍X�V
	CFire::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CAttackHomingFire::Draw(void)
{
	// ���̕`��
	CFire::Draw();
}

//============================================================
//	��������
//============================================================
CAttackHomingFire *CAttackHomingFire::Create(const D3DXVECTOR3& rPos)
{
	// �z�[�~���O�U�����̐���
	CAttackHomingFire *pAttackHomingFire = new CAttackHomingFire;
	if (pAttackHomingFire == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �z�[�~���O�U�����̏�����
		if (FAILED(pAttackHomingFire->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �z�[�~���O�U�����̔j��
			SAFE_DELETE(pAttackHomingFire);
			return nullptr;
		}

		// �ʒu��ݒ�
		pAttackHomingFire->SetVec3Position(rPos);

		// �m�ۂ����A�h���X��Ԃ�
		return pAttackHomingFire;
	}
}

//============================================================
//	���ړ��ʂ̍X�V����
//============================================================
void CAttackHomingFire::UpdateHomingMove(void)
{
	CListManager<CPlayer> *pList = CPlayer::GetList();		// �v���C���[���X�g
	if (pList == nullptr)		 { assert(false); return; }	// ���X�g���g�p
	if (pList->GetNumAll() != 1) { assert(false); return; }	// �v���C���[��1�l����Ȃ�
	auto player = pList->GetList().front();					// �v���C���[���

	D3DXVECTOR3 posPlayer	 = player->GetVec3Position();	// �v���C���[�̒��S�ʒu
				posPlayer.y += player->GetHeight() * 0.5f;	// �����̔�����ɂ�����

	D3DXVECTOR3 posFire		= GetVec3Position();	// ���̈ʒu
	D3DXVECTOR3 vecCurMove = GetMove();				// ���̌��݈ړ��x�N�g��
	D3DXVECTOR3 vecDestMove = posPlayer - posFire;	// ���̖ڕW�ړ��x�N�g��
	float fCurPhi, fCurTheta, fDestPhi, fDestTheta;	// ���̃z�[�~���O�v�Z�p

	// ���݁E�ڕW���������߂�
	useful::VecToRot(vecCurMove,  &fCurPhi,  &fCurTheta);
	useful::VecToRot(vecDestMove, &fDestPhi, &fDestTheta);

	// �z�[�~���O�̌v�Z
	CalcHoming(&fCurPhi,   fDestPhi);
	CalcHoming(&fCurTheta, fDestTheta);

	// �x�N�g�������߂�
	useful::RotToVec(fCurPhi, fCurTheta, &vecCurMove);

	// �ړ��ʂ𔽉f
	SetMove(vecCurMove * MOVE);
}

//============================================================
//	�z�[�~���O�̌v�Z����
//============================================================
void CAttackHomingFire::CalcHoming(float *pCurRot, float fDestRot)
{
	// �ϐ���錾
	float fDiffRot = 0.0f;	// ��������

	// �ڕW�����̐��K��
	useful::NormalizeRot(fDestRot);

	// �ڕW�����܂ł̍������v�Z
	fDiffRot = fDestRot - *pCurRot;

	// ���������̐��K��
	useful::NormalizeRot(fDiffRot);

	// �����̍X�V
	*pCurRot += fDiffRot * REV_ROT;

	// �����̐��K��
	useful::NormalizeRot(*pCurRot);
}
