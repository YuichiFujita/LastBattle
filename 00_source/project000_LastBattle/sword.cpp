//============================================================
//
//	������ [sword.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "sword.h"
#include "manager.h"
#include "renderer.h"
#include "orbit.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char *MODEL_FILE[] =	// ���f���t�@�C��
	{
		"data\\MODEL\\PLAYER\\15_sword.x",	// ��
	};

	const float SUB_ALPHA	= 0.05f;	// �����x�̌��Z��
	const int	ORBIT_PART	= 25;		// ������
	const COrbit::SOffset ORBIT_OFFSET = COrbit::SOffset(D3DXVECTOR3(0.0f, 0.0f, -10.0f), D3DXVECTOR3(0.0f, 0.0f, -65.0f), XCOL_CYAN);	// �I�t�Z�b�g���
}

//************************************************************
//	�X�^�e�B�b�N�A�T�[�g
//************************************************************
static_assert(NUM_ARRAY(MODEL_FILE) == CSword::MODEL_MAX, "ERROR : Model Count Mismatch");

//************************************************************
//	�q�N���X [CSword] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CSword::CSword() : CMultiModel(LABEL_NONE, DIM_3D, object::DEFAULT_PRIO),
	m_state			(STATE_NORMAL),	// ���
	m_bDisp			(false),		// �\����
	m_nCounterState	(0)				// ��ԊǗ��J�E���^�[
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CSword::~CSword()
{

}

//============================================================
//	����������
//============================================================
HRESULT CSword::Init(void)
{
	// �����o�ϐ���������
	m_state = STATE_NORMAL;	// ���
	m_bDisp = true;			// �\����
	m_nCounterState = 0;	// ��ԊǗ��J�E���^�[

	// �}���`���f���̏�����
	if (FAILED(CMultiModel::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ���f����o�^�E����
	BindModel(MODEL_FILE[MODEL_SWORD]);

	// �O�Ղ̐���
	m_pOrbit = COrbit::Create
	( // ����
		GetPtrMtxWorld(),	// �e�}�g���b�N�X
		ORBIT_OFFSET,		// �I�t�Z�b�g���
		ORBIT_PART			// ������
	);
	if (m_pOrbit == nullptr)
	{ // ��g�p���̏ꍇ

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
void CSword::Uninit(void)
{
	// �O�Ղ̏I��
	SAFE_UNINIT(m_pOrbit);

	// �}���`���f���̏I��
	CMultiModel::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CSword::Update(void)
{
	if (m_state == STATE_VANISH)
	{ // �������̏ꍇ

		// �ϐ���錾
		float fAlpha = GetAlpha();	// �����x

		// �����ɂ��Ă���
		fAlpha -= SUB_ALPHA;

		if (fAlpha <= 0.0f)
		{ // �����ɂȂ�؂����ꍇ

			// �\����OFF�ɂ���
			m_bDisp = false;

			// �O�Ղ�����
			m_pOrbit->SetState(COrbit::STATE_VANISH);
		}

		// �����x�𔽉f
		SetAlpha(fAlpha);
	}

	// �O�Ղ̍X�V
	m_pOrbit->Update();

	// �}���`���f���̍X�V
	CMultiModel::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CSword::Draw(void)
{
	if (m_bDisp)
	{ // �\����ON�̏ꍇ

		// �}���`���f���̕`��
		CMultiModel::Draw();
	}
}

//============================================================
//	��Ԏ擾����
//============================================================
int CSword::GetState(void) const
{
	// ��Ԃ�Ԃ�
	return m_state;
}

//============================================================
//	��������
//============================================================
CSword *CSword::Create
(
	CObject *pObject,			// �e�I�u�W�F�N�g
	const D3DXVECTOR3 &rPos,	// �ʒu
	const D3DXVECTOR3 &rRot		// ����
)
{
	// ���̐���
	CSword *pSword = new CSword;
	if (pSword == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// ���̏�����
		if (FAILED(pSword->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ���̔j��
			SAFE_DELETE(pSword);
			return nullptr;
		}

		// �e�I�u�W�F�N�g��ݒ�
		pSword->SetParentObject(pObject);

		// �ʒu��ݒ�
		pSword->SetVec3Position(rPos);

		// ������ݒ�
		pSword->SetVec3Rotation(rRot);

		// �m�ۂ����A�h���X��Ԃ�
		return pSword;
	}
}

//============================================================
//	��Ԃ̐ݒ菈��
//============================================================
void CSword::SetState(const EState state)
{
	if (state == m_state && state != STATE_NORMAL)
	{ // �ݒ肷���Ԃ����݂̏�Ԋ��A�ݒ肷���Ԃ��ʏ��Ԃ̏ꍇ

		// �����𔲂���
		return;
	}

	// �����̏�Ԃ�ݒ�
	m_state = state;

	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_NONE:	// �������Ȃ����
	case STATE_VANISH:	// �������
		break;

	case STATE_NORMAL:	// �ʏ���

		// �s�����ɂ���
		SetAlpha(1.0f);

		// �\����ON�ɂ���
		m_bDisp = true;

		// �O�Ղ��o��
		m_pOrbit->SetState(COrbit::STATE_NORMAL);

		break;

	default:	// ��O���
		assert(false);
		break;
	}
}
