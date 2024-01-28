//============================================================
//
//	������ [thunder.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "thunder.h"
#include "orbit.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const D3DXCOLOR COL[] =	// �O�Ղ̐F
	{
		D3DXCOLOR(1.0f, 1.0f, 0.35f, 1.0f),
		XCOL_YELLOW,
	};
	const D3DXVECTOR3 OFFSET[] =	// �O�Ղ̃I�t�Z�b�g
	{
		D3DXVECTOR3(0.0f, 40.0f, 0.0f),
		D3DXVECTOR3(0.0f, 90.0f, 0.0f),
	};

	const int	PRIORITY	= 7;	// ���̗D�揇��
	const int	PART		= 50;	// �O�Ղ̕�����

	const float	DOWN_MOVE	= 100.0f;	// ���̉��ړ���
	const float	LAND_POSY	= -100.0f;	// ���̒��nY���W
	const float	SPAWN_POSY	= 2500.0f;	// ���̐���Y���W
}

//************************************************************
//	�q�N���X [CThunder] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CThunder::CThunder() : CObject(CObject::LABEL_THUNDER, DIM_3D, PRIORITY), m_posOrbit(VEC3_ZERO)
{
	// �����o�ϐ����N���A
	memset(&m_aOrbit[0], 0, sizeof(m_aOrbit));	// �O�Ղ̏��
}

//============================================================
//	�f�X�g���N�^
//============================================================
CThunder::~CThunder()
{

}

//============================================================
//	����������
//============================================================
HRESULT CThunder::Init(void)
{
	// �����o�ϐ���������
	memset(&m_aOrbit[0], 0, sizeof(m_aOrbit));	// �O�Ղ̏��
	m_posOrbit = VEC3_ZERO;	// �O�Ղ̐����ʒu

	for (int nCntOrbit = 0; nCntOrbit < thunder::NUM_ORBIT; nCntOrbit++)
	{ // �O�Ղ̐��������J��Ԃ�

		// �O�Ղ̐���
		m_aOrbit[nCntOrbit].pOrbit = COrbit::Create
		( // ����
			&m_aOrbit[nCntOrbit].mtxWorld,									// �e�}�g���b�N�X
			COrbit::SOffset(VEC3_ZERO, OFFSET[nCntOrbit], COL[nCntOrbit]),	// �I�t�Z�b�g���
			PART															// ������
		);
		if (m_aOrbit[nCntOrbit].pOrbit == nullptr)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �D�揇�ʂ�ݒ�
		m_aOrbit[nCntOrbit].pOrbit->SetPriority(PRIORITY);
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CThunder::Uninit(void)
{
	for (int nCntOrbit = 0; nCntOrbit < thunder::NUM_ORBIT; nCntOrbit++)
	{ // �O�Ղ̐��������J��Ԃ�

		// �O�Ղ̏I��
		SAFE_UNINIT(m_aOrbit[nCntOrbit].pOrbit);
	}

	// ���I�u�W�F�N�g��j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CThunder::Update(void)
{
	if (m_posOrbit.y > LAND_POSY)
	{ // �n�ʂɓ��B���Ă��Ȃ��ꍇ

		// �ʒu��������
		m_posOrbit.y -= DOWN_MOVE;
		if (m_posOrbit.y <= LAND_POSY)
		{ // �n�ʂɓ��B�����ꍇ

			// �ʒu��␳
			m_posOrbit.y = LAND_POSY;

			for (int nCntOrbit = 0; nCntOrbit < thunder::NUM_ORBIT; nCntOrbit++)
			{ // �O�Ղ̐��������J��Ԃ�

				// �O�Ղ�������Ԃɂ���
				m_aOrbit[nCntOrbit].pOrbit->SetState(COrbit::STATE_VANISH);
			}
		}
	}

	D3DXMATRIX  mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DXVECTOR3 rotRand;			// �����_������
	bool bRelease = true;			// �j�����邩�ǂ���
	for (int nCntOrbit = 0; nCntOrbit < thunder::NUM_ORBIT; nCntOrbit++)
	{ // �O�Ղ̐��������J��Ԃ�

		// �����_���Ɍ�����ݒ�
		rotRand.x = (float)(rand() % 629 - 314) / 100.0f;
		rotRand.y = (float)(rand() % 629 - 314) / 100.0f;
		rotRand.z = (float)(rand() % 629 - 314) / 100.0f;

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&m_aOrbit[nCntOrbit].mtxWorld);

		// �����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, rotRand.y, rotRand.x, rotRand.z);
		D3DXMatrixMultiply(&m_aOrbit[nCntOrbit].mtxWorld, &m_aOrbit[nCntOrbit].mtxWorld, &mtxRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, m_posOrbit.x, m_posOrbit.y, m_posOrbit.z);
		D3DXMatrixMultiply(&m_aOrbit[nCntOrbit].mtxWorld, &m_aOrbit[nCntOrbit].mtxWorld, &mtxTrans);

		// �O�Ղ̍X�V
		m_aOrbit[nCntOrbit].pOrbit->Update();

		if (bRelease)
		{ // ����j������ݒ�̏ꍇ

			if (m_aOrbit[nCntOrbit].pOrbit->GetState() != COrbit::STATE_NONE)
			{ // �O�Ղ��������Ă��Ȃ��ꍇ

				// �j���ł��Ȃ��悤�ɂ���
				bRelease = false;
			}
		}
	}

	if (bRelease)
	{ // �j���\�ȏꍇ

		// ���g���I��
		Uninit();

		// �����𔲂���
		return;
	}
}

//============================================================
//	�`�揈��
//============================================================
void CThunder::Draw(void)
{

}

//============================================================
//	��������
//============================================================
CThunder *CThunder::Create(const D3DXVECTOR3 &rPos)
{
	// ���̐���
	CThunder *pThunder = new CThunder;
	if (pThunder == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// ���̏�����
		if (FAILED(pThunder->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ���̔j��
			SAFE_DELETE(pThunder);
			return nullptr;
		}

		// �ʒu��ݒ�
		pThunder->SetVec3Position(rPos);

		// �m�ۂ����A�h���X��Ԃ�
		return pThunder;
	}
}

//============================================================
//	�j������
//============================================================
void CThunder::Release(void)
{
	// �I�u�W�F�N�g�̔j��
	CObject::Release();
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CThunder::SetVec3Position(const D3DXVECTOR3 &rPos)
{
	// �ʒu��ݒ�
	m_posOrbit = D3DXVECTOR3(rPos.x, SPAWN_POSY, rPos.z);

	// ���[���h�}�g���b�N�X��������
	D3DXMATRIX  mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DXVECTOR3 rotRand;			// �����_������
	for (int nCntOrbit = 0; nCntOrbit < thunder::NUM_ORBIT; nCntOrbit++)
	{ // �O�Ղ̐��������J��Ԃ�

		// �����������_����]
		rotRand.x = (float)(rand() % 629 - 314) / 100.0f;
		rotRand.y = (float)(rand() % 629 - 314) / 100.0f;
		rotRand.z = (float)(rand() % 629 - 314) / 100.0f;

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&m_aOrbit[nCntOrbit].mtxWorld);

		// �����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, rotRand.y, rotRand.x, rotRand.z);
		D3DXMatrixMultiply(&m_aOrbit[nCntOrbit].mtxWorld, &m_aOrbit[nCntOrbit].mtxWorld, &mtxRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, m_posOrbit.x, m_posOrbit.y, m_posOrbit.z);
		D3DXMatrixMultiply(&m_aOrbit[nCntOrbit].mtxWorld, &m_aOrbit[nCntOrbit].mtxWorld, &mtxTrans);
	}
}
