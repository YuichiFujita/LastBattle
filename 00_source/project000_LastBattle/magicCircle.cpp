//============================================================
//
//	���@�w���� [magicCircle.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "magicCircle.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "objectMeshCylinder.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char *TEXTURE_FILE[] =	// �e�N�X�`���t�@�C��
	{
		"data\\TEXTURE\\magicCircle000.png",	// �ʏ�
	};

	const int	PRIORITY		= 5;		// ���@�w�̗D�揇��
	const float	ALPHA_LENGTH	= 1000.0f;	// �����̒���
	const POSGRID2 PART_CIRCLE	= POSGRID2(32, 2);	// ���@�w�̕�����
	const POSGRID2 PART_ALPHA	= POSGRID2(32, 1);	// �����̕�����
}

//************************************************************
//	�X�^�e�B�b�N�A�T�[�g
//************************************************************
static_assert(NUM_ARRAY(TEXTURE_FILE) == CMagicCircle::TEXTURE_MAX, "ERROR : Texture Count Mismatch");

//************************************************************
//	�q�N���X [CMagicCircle] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CMagicCircle::CMagicCircle() : CObjectMeshCircle(CObject::LABEL_MAGIC_CIRCLE, CObject::DIM_3D, PRIORITY), m_pAlphaCylinder(nullptr)
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CMagicCircle::~CMagicCircle()
{

}

//============================================================
//	����������
//============================================================
HRESULT CMagicCircle::Init(void)
{
	// �����o�ϐ���������
	m_pAlphaCylinder = nullptr;	// ���@�w�̐�̓������

	// �I�u�W�F�N�g���b�V���T�[�N���̏�����
	if (FAILED(CObjectMeshCircle::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �������̐ݒ�
	if (FAILED(SetPattern(PART_CIRCLE)))
	{ // �������̐ݒ�Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �����_�[�X�e�[�g�̏����擾
	CRenderState *pRenderState = GetRenderState();

	// �|���S���̗��ʂ�\����Ԃɂ���
	pRenderState->SetCulling(D3DCULL_NONE);

	// ���C�e�B���O��OFF�ɂ���
	pRenderState->SetLighting(false);

	// ���@�w�̐�̓����\���̐���
	m_pAlphaCylinder = CObjectMeshCylinder::Create
	( // ����
		VEC3_ZERO,		// �ʒu
		VEC3_ZERO,		// ����
		XCOL_AWHITE,	// �F
		PART_ALPHA,		// ������
		GRID2_ONE,		// �e�N�X�`��������
		0.0f,			// ���a
		ALPHA_LENGTH	// �c��
	);
	if (m_pAlphaCylinder == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �D�揇�ʂ�ݒ�
	m_pAlphaCylinder->SetPriority(PRIORITY);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CMagicCircle::Uninit(void)
{
	// ���@�w�̐�̓������̏I��
	SAFE_UNINIT(m_pAlphaCylinder);

	// �I�u�W�F�N�g���b�V���T�[�N���̏I��
	CObjectMeshCircle::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CMagicCircle::Update(void)
{
	// �I�u�W�F�N�g���b�V���T�[�N���̍X�V
	CObjectMeshCircle::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CMagicCircle::Draw(void)
{
	// �I�u�W�F�N�g���b�V���T�[�N���̕`��
	CObjectMeshCircle::Draw();
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CMagicCircle::SetVec3Position(const D3DXVECTOR3 &rPos)
{
	// ���g�̈ʒu��ݒ�
	CObjectMeshCircle::SetVec3Position(rPos);

	// ���@�w�̐�̓����\���̈ʒu��ݒ�
	m_pAlphaCylinder->SetVec3Position(rPos);
}

//============================================================
//	�����̐ݒ菈��
//============================================================
void CMagicCircle::SetVec3Rotation(const D3DXVECTOR3 &rRot)
{
	// ���g�̌�����ݒ�
	CObjectMeshCircle::SetVec3Rotation(rRot);

	// ���@�w�̐�̓����\���̌�����ݒ�
	m_pAlphaCylinder->SetVec3Rotation(rRot);
}

//============================================================
//	���a�̐ݒ菈��
//============================================================
void CMagicCircle::SetRadius(const float fRadius)
{
	// ���g�̔��a��ݒ�
	CObjectMeshCircle::SetRadius(fRadius);

	// ���@�w�̐�̓����\���̔��a��ݒ�
	m_pAlphaCylinder->SetRadius(fRadius);
}

//============================================================
//	��������
//============================================================
CMagicCircle *CMagicCircle::Create
(
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rRot,	// ����
	const float fRadius			// ���a
)
{
	// ���@�w�̐���
	CMagicCircle *pMagicCircle = new CMagicCircle;
	if (pMagicCircle == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// ���@�w�̏�����
		if (FAILED(pMagicCircle->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ���@�w�̔j��
			SAFE_DELETE(pMagicCircle);
			return nullptr;
		}

		// �e�N�X�`����o�^�E����
		pMagicCircle->BindTexture(GET_MANAGER->GetTexture()->Regist(TEXTURE_FILE[TEXTURE_NORMAL]));

		// �ʒu��ݒ�
		pMagicCircle->SetVec3Position(rPos);

		// ������ݒ�
		pMagicCircle->SetVec3Rotation(rRot);

		// ���a��ݒ�
		pMagicCircle->SetRadius(fRadius);

		// �m�ۂ����A�h���X��Ԃ�
		return pMagicCircle;
	}
}
