//============================================================
//
//	���g������ [swordWave.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "swordWave.h"
#include "player.h"
#include "collision.h"
#include "enemy.h"
#include "collSphere.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const POSGRID2	PART		= POSGRID2(64, 1);	// ������
	const POSGRID2	TEX_PART	= POSGRID2(1, 1);	// �e�N�X�`��������
	const float		SUB_ALPHA	= 0.05f;			// ���g���̓����x���Z��
	const int		DMG_WAVE	= 15;				// ���g���̃_���[�W��
}

//************************************************************
//	�q�N���X [CSwordWave] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CSwordWave::CSwordWave() :
	m_addGrow	(SGrow(0.0f, 0.0f, 0.0f))	// ����������
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CSwordWave::~CSwordWave()
{

}

//============================================================
//	����������
//============================================================
HRESULT CSwordWave::Init(void)
{
	// �����o�ϐ���������
	m_addGrow = SGrow(0.0f, 0.0f, 0.0f);	// ����������

	// �g���̏�����
	if (FAILED(CWave::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �������̐ݒ�
	if (FAILED(SetPattern(PART)))
	{ // �������̐ݒ�Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�N�X�`���̐ݒ�
	SetTexture(TEXTURE_SWORD);

	// �e�N�X�`���������̐ݒ�
	SetTexPattern(TEX_PART);

	// �����_�[�X�e�[�g�̏����擾
	CRenderState *pRenderState = GetRenderState();

	// ���u�����h�̐ݒ�
	pRenderState->SetAlphaBlend(CRenderState::BLEND_ADD);

	// Z�e�X�g�̐ݒ�
	pRenderState->SetZFunc(D3DCMP_ALWAYS);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CSwordWave::Uninit(void)
{
	// �g���̏I��
	CWave::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CSwordWave::Update(void)
{
	// �g���̍X�V
	CWave::Update();

	// �G�Ƃ̓����蔻��
	CollisionEnemy();

	// ����������������
	SGrow curGrow = GetGrow();	// ���݂̐������
	SetGrow(SGrow
	( // ����
		curGrow.fAddHoleRadius	+ m_addGrow.fAddHoleRadius,	// ���̔��a�̐�����
		curGrow.fAddThickness	+ m_addGrow.fAddThickness,	// �����̐�����
		curGrow.fSubAlpha		+ m_addGrow.fSubAlpha		// �����x�̐�����
	));
}

//============================================================
//	�`�揈��
//============================================================
void CSwordWave::Draw(CShader *pShader)
{
	// �g���̕`��
	CWave::Draw();
}

//============================================================
//	��������
//============================================================
CSwordWave *CSwordWave::Create
(
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rRot,	// ����
	const SGrow& rGrow,			// ������
	const SGrow& rAddGrow,		// ����������
	const float fHoleRadius,	// ���̔��a
	const float fThickness		// ����
)
{
	// ���g���̐���
	CSwordWave *pSwordWave = new CSwordWave;
	if (pSwordWave == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// ���g���̏�����
		if (FAILED(pSwordWave->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ���g���̔j��
			SAFE_DELETE(pSwordWave);
			return nullptr;
		}

		// �ʒu��ݒ�
		pSwordWave->SetVec3Position(rPos);

		// ������ݒ�
		pSwordWave->SetVec3Rotation(rRot);

		// ��������ݒ�
		pSwordWave->SetGrow(rGrow);

		// ������������ݒ�
		pSwordWave->SetAddGrow(rAddGrow);

		// ���̔��a��ݒ�
		pSwordWave->SetHoleRadius(fHoleRadius);

		// ������ݒ�
		pSwordWave->SetThickness(fThickness);

		// �m�ۂ����A�h���X��Ԃ�
		return pSwordWave;
	}
}

//============================================================
//	�G�Ƃ̓����蔻��
//============================================================
void CSwordWave::CollisionEnemy(void)
{
	CListManager<CEnemy> *pList = CEnemy::GetList();	// ���X�g�}�l�[�W���[
	if (pList == nullptr)		 { return; }	// ���X�g���g�p
	if (pList->GetNumAll() <= 0) { return; }	// �G�����݂��Ȃ�

	std::list<CEnemy*> list = pList->GetList();	// �G���X�g
	for (auto enemy : list)
	{ // ���X�g�̂��ׂĂ��J��Ԃ�

		int nNumParts = enemy->GetNumModel();	// �p�[�c��
		for (int nCntColl = 0; nCntColl < nNumParts; nCntColl++)
		{ // �p�[�c�����J��Ԃ�

			int nCntArray = 0;	// ������̗v�f�ԍ�
			CCollSphere *pColl = enemy->GetCollision(nCntColl);				// �~������
			std::vector<CCollSphere::SInfo> vector = pColl->GetVector();	// �~����z��
			for (auto coll : vector)
			{ // �z��̗v�f�����J��Ԃ�

				// �G�̔���ʒu���v�Z
				D3DXVECTOR3 posCollEnemy = pColl->CalcWorldPosition(nCntArray);

				// �v�f�ԍ�������
				nCntArray++;

				// �U���̓����蔻��
				bool bHit = collision::Circle3D
				( // ����
					GetVec3Position(),	// ����ʒu
					posCollEnemy,		// ����ڕW�ʒu
					GetHoleRadius() + GetThickness(),	// ���蔼�a
					coll.fRadius						// ����ڕW���a
				);
				if (bHit)
				{ // �U�������������ꍇ

					// �G�̃q�b�g����
					enemy->Hit(DMG_WAVE);
				}
			}
		}
	}
}
