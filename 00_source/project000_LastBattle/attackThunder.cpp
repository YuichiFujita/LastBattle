//============================================================
//
//	���U������ [attackThunder.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "attackThunder.h"
#include "thunder.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{

}

//************************************************************
//	�q�N���X [CAttackThunder] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CAttackThunder::CAttackThunder() : CObject(CObject::LABEL_THUNDER, DIM_3D)
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CAttackThunder::~CAttackThunder()
{

}

//============================================================
//	����������
//============================================================
HRESULT CAttackThunder::Init(void)
{
	for (int nCntThunder = 0; nCntThunder < attackThunder::NUM_THUNDER; nCntThunder++)
	{ // ���̐��������J��Ԃ�

		// ���̐���
		m_apThunder[nCntThunder] = CThunder::Create(VEC3_ZERO);
		if (m_apThunder[nCntThunder] == nullptr)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CAttackThunder::Uninit(void)
{
	for (int nCntThunder = 0; nCntThunder < attackThunder::NUM_THUNDER; nCntThunder++)
	{ // ���̐��������J��Ԃ�

		// ���̏I��
		SAFE_UNINIT(m_apThunder[nCntThunder]);
	}

	// ���U���I�u�W�F�N�g��j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CAttackThunder::Update(void)
{
	for (int nCntThunder = 0; nCntThunder < attackThunder::NUM_THUNDER; nCntThunder++)
	{ // ���̐��������J��Ԃ�

		// ���̍X�V
		m_apThunder[nCntThunder]->Update();
	}
}

//============================================================
//	�`�揈��
//============================================================
void CAttackThunder::Draw(void)
{

}

//============================================================
//	��������
//============================================================
CAttackThunder *CAttackThunder::Create(const D3DXVECTOR3 &rPos)
{
	// ���U���̐���
	CAttackThunder *pAttackThunder = new CAttackThunder;
	if (pAttackThunder == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// ���U���̏�����
		if (FAILED(pAttackThunder->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ���U���̔j��
			SAFE_DELETE(pAttackThunder);
			return nullptr;
		}

		// �ʒu��ݒ�
		pAttackThunder->SetVec3Position(rPos);

		// �m�ۂ����A�h���X��Ԃ�
		return pAttackThunder;
	}
}

//============================================================
//	�j������
//============================================================
void CAttackThunder::Release(void)
{
	// �I�u�W�F�N�g�̔j��
	CObject::Release();
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CAttackThunder::SetVec3Position(const D3DXVECTOR3 &rPos)
{

}
