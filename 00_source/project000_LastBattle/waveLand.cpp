//============================================================
//
//	���n�g������ [waveLand.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "waveLand.h"
#include "player.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const POSGRID2	PART			= POSGRID2(64, 1);	// ������
	const POSGRID2	TEX_PART		= POSGRID2(32, 1);	// �e�N�X�`��������
	const float		SUB_ALPHA		= 0.05f;			// ���n�g���̓����x���Z��
	const float		MOVE_TEXU		= -0.05f;			// �e�N�X�`�����ړ���
	const int		ALPHA_NUMREF	= 10;				// ���e�X�g�̎Q�ƒl
}

//************************************************************
//	�q�N���X [CWaveLand] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CWaveLand::CWaveLand() :
	m_fMaxGrowRadius	(0.0f),						// ���a�̍ő听����
	m_addGrow			(SGrow(0.0f, 0.0f, 0.0f))	// ����������
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CWaveLand::~CWaveLand()
{

}

//============================================================
//	����������
//============================================================
HRESULT CWaveLand::Init(void)
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

	// �����W�̈ړ��ʂ�ݒ�
	SetMoveU(MOVE_TEXU);

	// �����_�[�X�e�[�g�̏����擾
	CRenderState *pRenderState = GetRenderState();

	// ���u�����h�̐ݒ�
	pRenderState->SetAlphaBlend(CRenderState::BLEND_ADD);

	// ���e�X�g�̐ݒ�
	pRenderState->SetAlphaTest(true);			// ���e�X�g�̗L�� / �����̐ݒ�
	pRenderState->SetAlphaFunc(D3DCMP_GREATER);	// ���e�X�g�̐ݒ�
	pRenderState->SetAlphaNumRef(ALPHA_NUMREF);	// ���e�X�g�̎Q�ƒl�ݒ�

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CWaveLand::Uninit(void)
{
	// �g���̏I��
	CWave::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CWaveLand::Update(void)
{
	// �g���̍X�V
	CWave::Update();

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
void CWaveLand::Draw(void)
{
	// �g���̕`��
	CWave::Draw();
}

//============================================================
//	��������
//============================================================
CWaveLand *CWaveLand::Create
(
	const ETexture texture,		// ���
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXCOLOR& rCol,		// �F
	const SGrow& rGrow,			// ������
	const SGrow& rAddGrow,		// ����������
	const float fHoleRadius,	// ���̔��a
	const float fThickness,		// ����
	const float fOuterPlusY,	// �O����Y���W���Z��
	const float fMaxGrowRadius	// ���a�̍ő听����
)
{
	// ���n�g���̐���
	CWaveLand *pWaveLand = new CWaveLand;
	if (pWaveLand == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// ���n�g���̏�����
		if (FAILED(pWaveLand->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ���n�g���̔j��
			SAFE_DELETE(pWaveLand);
			return nullptr;
		}

		// �e�N�X�`����ݒ�
		pWaveLand->SetTexture(texture);

		// �ʒu��ݒ�
		pWaveLand->SetVec3Position(rPos);

		// �F��ݒ�
		pWaveLand->SetColor(rCol);

		// ��������ݒ�
		pWaveLand->SetGrow(rGrow);

		// ������������ݒ�
		pWaveLand->SetAddGrow(rAddGrow);

		// ���̔��a��ݒ�
		pWaveLand->SetHoleRadius(fHoleRadius);

		// ������ݒ�
		pWaveLand->SetThickness(fThickness);

		// �O����Y���W���Z�ʂ�ݒ�
		pWaveLand->SetOuterPlusY(fOuterPlusY);

		// ���a�̍ő听���ʂ�ݒ�
		pWaveLand->SetMaxGrowRadius(fMaxGrowRadius);

		// �m�ۂ����A�h���X��Ԃ�
		return pWaveLand;
	}
}
