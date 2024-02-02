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
#include "scene.h"
#include "stage.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const D3DXCOLOR COL[] =	// �O�Ղ̐F
	{
		D3DXCOLOR(1.0f, 1.0f, 0.1f, 1.0f),
		D3DXCOLOR(1.0f, 1.0f, 0.4f, 1.0f),
	};
	const D3DXVECTOR3 OFFSET[] =	// �O�Ղ̃I�t�Z�b�g
	{
		D3DXVECTOR3(10.0f, 0.0f, 0.0f),
		D3DXVECTOR3(32.0f, 0.0f, 0.0f),
	};
	const int DIV_DIRRAND[] = { 6, 36 };	// �����̏�]�Z�̒l
	const int ADD_DIRRAND[] = { 10, 80 };	// �����̌��Z�̒l

	const int	PRIORITY		= 7;		// ���̗D�揇��
	const int	PART			= 20;		// �O�Ղ̕�����
	const int	MAX_SHIFT_POSY	= 120;		// �O�Ղ�Y���W���炵�ʂ̍ő�l
	const float	DOWN_MOVE		= 200.0f;	// ���̉��ړ���
	const float	SPAWN_POSY		= 2500.0f;	// ���̐���Y���W
}

//************************************************************
//	�q�N���X [CThunder] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CThunder::CThunder() : CObject(CObject::LABEL_THUNDER, DIM_3D, PRIORITY),
	m_posOrbit	(VEC3_ZERO),	// �O�Ղ̐����ʒu
	m_bLand		(false)			// �O�Ղ̒��n����
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
	m_posOrbit	= VEC3_ZERO;	// �O�Ղ̐����ʒu
	m_bLand		= false;		// �O�Ղ̒��n����

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
	// �|�C���^��錾
	CStage *pStage = CScene::GetStage();	// �X�e�[�W�̏��
	assert(pStage != nullptr);	// �X�e�[�W���g�p

	if (!m_bLand)
	{ // �n�ʂɓ��B���Ă��Ȃ��ꍇ

		// �ʒu��������
		m_posOrbit.y -= DOWN_MOVE;

		float fLandPosY = pStage->GetStageLimit().fField;
		if (m_posOrbit.y <= fLandPosY)
		{ // �n�ʂɓ��B�����ꍇ

			// �ʒu��␳
			m_posOrbit.y = fLandPosY;

			// ���n�ς݂ɂ���
			m_bLand = true;
		}
	}

	D3DXMATRIX  mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DXVECTOR3 rotRand;			// �����_������
	bool bRelease = true;			// �j�����邩�ǂ���
	for (int nCntOrbit = 0; nCntOrbit < thunder::NUM_ORBIT; nCntOrbit++)
	{ // �O�Ղ̐��������J��Ԃ�

		if (m_bLand)
		{ // �n�ʂɓ��B���Ă���ꍇ

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&m_aOrbit[nCntOrbit].mtxWorld);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, m_posOrbit.x, m_posOrbit.y, m_posOrbit.z);
			D3DXMatrixMultiply(&m_aOrbit[nCntOrbit].mtxWorld, &m_aOrbit[nCntOrbit].mtxWorld, &mtxTrans);

			// �O�Ղ�������Ԃɂ���
			m_aOrbit[nCntOrbit].pOrbit->SetState(COrbit::STATE_VANISH);
		}
		else
		{ // �n�ʂɓ��B���Ă��Ȃ��ꍇ

			// �����_���Ɍ�����ݒ�
			rotRand.x = useful::RandomRot();
			rotRand.y = useful::RandomRot();
			rotRand.z = useful::RandomRot();

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&m_aOrbit[nCntOrbit].mtxWorld);

			// �����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, rotRand.y, rotRand.x, rotRand.z);
			D3DXMatrixMultiply(&m_aOrbit[nCntOrbit].mtxWorld, &m_aOrbit[nCntOrbit].mtxWorld, &mtxRot);

			// �O�Ղ̈ʒu���炵�ʂ�ݒ�
			float fRandShiftLength = 0.0f;	// �ʒu���炵�̒���
			fRandShiftLength  = (float)((rand() % DIV_DIRRAND[nCntOrbit] + ADD_DIRRAND[nCntOrbit]));	// ���炷�����̐�Βl�����߂�
			fRandShiftLength *= (rand() % 2 == 0) ? +1.0f : -1.0f;										// �ǂ���̕����ɂ��炷����ݒ�

			// �ʒu�𔽉f
			D3DXMatrixTranslation
			( // ����
				&mtxTrans,
				m_posOrbit.x + sinf(rotRand.y) * fRandShiftLength,
				m_posOrbit.y + (float)(rand() % MAX_SHIFT_POSY),
				m_posOrbit.z + cosf(rotRand.y) * fRandShiftLength
			);
			D3DXMatrixMultiply(&m_aOrbit[nCntOrbit].mtxWorld, &m_aOrbit[nCntOrbit].mtxWorld, &mtxTrans);
		}

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
		rotRand.x = useful::RandomRot();
		rotRand.y = useful::RandomRot();
		rotRand.z = useful::RandomRot();

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

//============================================================
//	�ʒu�擾����
//============================================================
D3DXVECTOR3 CThunder::GetVec3Position(void) const
{
	// �O�Ղ̐����ʒu��Ԃ�
	return m_posOrbit;
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
