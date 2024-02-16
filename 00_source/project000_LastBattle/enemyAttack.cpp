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
#include "enemyAttack00.h"
#include "enemyAttack01.h"
#include "enemyAttack02.h"
#include "enemyAttack03.h"
#include "enemyAttack04.h"
#include "enemyBossDragon.h"
#include "manager.h"

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

	case ATTACK_01:
		pEnemyAttack = new CEnemyAttack01;
		break;

	case ATTACK_02:
		pEnemyAttack = new CEnemyAttack02;
		break;

	case ATTACK_03:
		pEnemyAttack = new CEnemyAttack03;
		break;

	case ATTACK_04:
		pEnemyAttack = new CEnemyAttack04;
		break;

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
CEnemyBossDragon *CEnemyAttack::GetBoss(void)
{
	// �{�X�̏���Ԃ�
	return m_pBoss;
}
