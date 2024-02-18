//============================================================
//
//	���̔���f�o�b�O�\������ [debugCollSphere.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "debugCollSphere.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int PRIORITY = 7;	// ���̔���f�o�b�O�\���̗D�揇��
}

//************************************************************
//	�q�N���X [CDebugCollSphere] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CDebugCollSphere::CDebugCollSphere() : CObjectMeshSphere(CObject::LABEL_DEBUG, CObject::DIM_3D, PRIORITY)
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CDebugCollSphere::~CDebugCollSphere()
{

}

//============================================================
//	����������
//============================================================
HRESULT CDebugCollSphere::Init(void)
{
	// �I�u�W�F�N�g���b�V���X�t�B�A�̏�����
	if (FAILED(CObjectMeshSphere::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �D�揇�ʂ�ݒ�
	SetPriority(PRIORITY);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CDebugCollSphere::Uninit(void)
{
	// �I�u�W�F�N�g���b�V���X�t�B�A�̏I��
	CObjectMeshSphere::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CDebugCollSphere::Update(void)
{
	// �I�u�W�F�N�g���b�V���X�t�B�A�̍X�V
	CObjectMeshSphere::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CDebugCollSphere::Draw(CShader *pShader)
{
	// �I�u�W�F�N�g���b�V���X�t�B�A�̕`��
	CObjectMeshSphere::Draw();
}

//============================================================
//	��������
//============================================================
CDebugCollSphere *CDebugCollSphere::Create
(
	const D3DXVECTOR3& rPos,	// �ʒu
	const float fRadius,		// ���a
	const D3DXCOLOR& rCol,		// �F
	const D3DXVECTOR3& rRot,	// ����
	const POSGRID2& rPart,		// ������
	const POSGRID2& rTexPart	// �e�N�X�`��������
)
{
	// ���̔���f�o�b�O�\���̐���
	CDebugCollSphere *pDebugCollSphere = new CDebugCollSphere;
	if (pDebugCollSphere == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// ���̔���f�o�b�O�\���̏�����
		if (FAILED(pDebugCollSphere->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ���̔���f�o�b�O�\���̔j��
			SAFE_DELETE(pDebugCollSphere);
			return nullptr;
		}

		// �ʒu��ݒ�
		pDebugCollSphere->SetVec3Position(rPos);

		// ������ݒ�
		pDebugCollSphere->SetVec3Rotation(rRot);

		// �F��ݒ�
		pDebugCollSphere->SetColor(rCol);

		// ���a��ݒ�
		pDebugCollSphere->SetRadius(fRadius);

		// ��������ݒ�
		if (FAILED(pDebugCollSphere->SetPattern(rPart)))
		{ // �������̐ݒ�Ɏ��s�����ꍇ

			// ���̔���f�o�b�O�\���̔j��
			SAFE_DELETE(pDebugCollSphere);
			return nullptr;
		}

		// �e�N�X�`����������ݒ�
		pDebugCollSphere->SetTexPattern(rTexPart);

		// �m�ۂ����A�h���X��Ԃ�
		return pDebugCollSphere;
	}
}
