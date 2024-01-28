//============================================================
//
//	�Ռ��g���� [impact.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "impact.h"
#include "player.h"
#include "collision.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const POSGRID2	PART		= POSGRID2(64, 1);	// ������
	const POSGRID2	TEX_PART	= POSGRID2(8, 1);	// �e�N�X�`��������
	const float		SUB_ALPHA	= 0.05f;			// �Ռ��g�̓����x���Z��
	const int		DMG_IMPACT	= 15;				// �Ռ��g�̃_���[�W��
}

//************************************************************
//	�q�N���X [CImpact] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CImpact::CImpact() :
	m_fMaxGrowRadius	(0.0f),						// ���a�̍ő听����
	m_addGrow			(SGrow(0.0f, 0.0f, 0.0f))	// ����������
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CImpact::~CImpact()
{

}

//============================================================
//	����������
//============================================================
HRESULT CImpact::Init(void)
{
	// �����o�ϐ���������
	m_fMaxGrowRadius = 0.0f;				// ���a�̍ő听����
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

	// �e�N�X�`���������̐ݒ�
	SetTexPattern(TEX_PART);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CImpact::Uninit(void)
{
	// �g���̏I��
	CWave::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CImpact::Update(void)
{
	// �g���̍X�V
	CWave::Update();

	// �v���C���[�Ƃ̓����蔻��
	CollisionPlayer();

	if (GetHoleRadius() + GetThickness() > m_fMaxGrowRadius)
	{ // ���a�̍ő�l�ɓ��B�����ꍇ

		// �����x�����Z
		SGrow curGrow = GetGrow();		// ���݂̐������
		D3DXCOLOR curCol = GetColor();	// ���݂̐F
		SetColor(D3DXCOLOR(curCol.r, curCol.g, curCol.b, curCol.a - curGrow.fSubAlpha - SUB_ALPHA));
	}
	else
	{ // ���a�̍ő�l�ɓ��B���Ă��Ȃ��ꍇ

		// ����������������
		SGrow curGrow = GetGrow();	// ���݂̐������
		SetGrow(SGrow
		( // ����
			curGrow.fAddHoleRadius	+ m_addGrow.fAddHoleRadius,	// ���̔��a�̐�����
			curGrow.fAddThickness	+ m_addGrow.fAddThickness,	// �����̐�����
			curGrow.fSubAlpha		+ m_addGrow.fSubAlpha		// �����x�̐�����
		));
	}
}

//============================================================
//	�`�揈��
//============================================================
void CImpact::Draw(void)
{
	// �g���̕`��
	CWave::Draw();
}

//============================================================
//	��������
//============================================================
CImpact *CImpact::Create
(
	const ETexture texture,		// ���
	const D3DXVECTOR3& rPos,	// �ʒu
	const SGrow& rGrow,			// ������
	const SGrow& rAddGrow,		// ����������
	const float fHoleRadius,	// ���̔��a
	const float fThickness,		// ����
	const float fOuterPlusY,	// �O����Y���W���Z��
	const float fMaxGrowRadius	// ���a�̍ő听����
)
{
	// �Ռ��g�̐���
	CImpact *pImpact = new CImpact;
	if (pImpact == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �Ռ��g�̏�����
		if (FAILED(pImpact->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �Ռ��g�̔j��
			SAFE_DELETE(pImpact);
			return nullptr;
		}

		// �e�N�X�`����ݒ�
		pImpact->SetTexture(texture);

		// �ʒu��ݒ�
		pImpact->SetVec3Position(rPos);

		// ��������ݒ�
		pImpact->SetGrow(rGrow);

		// ������������ݒ�
		pImpact->SetAddGrow(rAddGrow);

		// ���̔��a��ݒ�
		pImpact->SetHoleRadius(fHoleRadius);

		// ������ݒ�
		pImpact->SetThickness(fThickness);

		// �O����Y���W���Z�ʂ�ݒ�
		pImpact->SetOuterPlusY(fOuterPlusY);

		// ���a�̍ő听���ʂ�ݒ�
		pImpact->SetMaxGrowRadius(fMaxGrowRadius);

		// �m�ۂ����A�h���X��Ԃ�
		return pImpact;
	}
}

//============================================================
//	�v���C���[�Ƃ̓����蔻��
//============================================================
void CImpact::CollisionPlayer(void)
{
	CListManager<CPlayer> *pList = CPlayer::GetList();		// �v���C���[���X�g
	if (pList == nullptr)		 { assert(false); return; }	// ���X�g���g�p
	if (pList->GetNumAll() != 1) { assert(false); return; }	// �v���C���[��1�l����Ȃ�
	auto player = pList->GetList().front();					// �v���C���[���

	D3DXVECTOR3 posPlayer	= player->GetVec3Position();	// �v���C���[�̈ʒu
	D3DXVECTOR3 posImpact	= GetVec3Position();			// �Ռ��g�̈ʒu
	float fRadiusPlayer		= player->GetRadius();			// �v���C���[�̔��a
	float fRadiusInImpact	= GetHoleRadius() - fRadiusPlayer;					// �Ռ��g�̓������a
	float fRadiusOutImpact	= GetHoleRadius() + GetThickness() + fRadiusPlayer;	// �Ռ��g�̊O�����a

	bool bHitIn  = !collision::Circle2D(posImpact, posPlayer, fRadiusInImpact,  0.0f);	// �Ռ��g�̓��~�̊O���ɂ���
	bool bHitOut =  collision::Circle2D(posImpact, posPlayer, fRadiusOutImpact, 0.0f);	// �Ռ��g�̊O�~�̓����ɂ���
	if (bHitIn && bHitOut)
	{ // �Ռ��g�ɓ������Ă���ꍇ

		float fPlayerFoot = posPlayer.y;						// �v���C���[���ʒu
		float fPlayerHead = posPlayer.y + player->GetHeight();	// �v���C���[���ʒu
		float fImpactDown = posImpact.y;						// �Ռ��g���ʒu
		float fImpactUp   = posImpact.y + GetOuterPlusY();		// �Ռ��g��ʒu
		if (fPlayerFoot <= fImpactDown
		&&  fPlayerHead >= fImpactUp)
		{ // �㉺�͈͓̔��̏ꍇ

			// �v���C���[�̃q�b�g����
			player->Hit(DMG_IMPACT);
		}
	}
}
