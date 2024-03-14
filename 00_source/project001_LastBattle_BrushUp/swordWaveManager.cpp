//============================================================
//
//	���g���}�l�[�W���[���� [swordWaveManager.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "swordWaveManager.h"
#include "swordWave.h"
#include "scene.h"
#include "player.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const CWave::SGrow GROW		= CWave::SGrow(5.5f, 0.0f, 0.06f);	// ������
	const CWave::SGrow ADD_GROW	= CWave::SGrow(0.12f, 0.0f, 0.0f);	// ����������
	const float HOLE_RAD	= 60.0f;	// ���̔��a
	const float THICKNESS	= 35.0f;	// ����
}

//************************************************************
//	�e�N���X [CSwordWaveManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CSwordWaveManager::CSwordWaveManager()
{
	// �����o�ϐ����N���A
	m_spawnTiming.clear();	// �����^�C�~���O
}

//============================================================
//	�f�X�g���N�^
//============================================================
CSwordWaveManager::~CSwordWaveManager()
{

}

//============================================================
//	����������
//============================================================
HRESULT CSwordWaveManager::Init(void)
{
	// �����o�ϐ���������
	m_spawnTiming.clear();	// �����^�C�~���O

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CSwordWaveManager::Uninit(void)
{
	// �v�f���N���A
	m_spawnTiming.clear();	// �����^�C�~���O
}

//============================================================
//	�X�V����
//============================================================
void CSwordWaveManager::Update(void)
{
	CPlayer *pPlayer = CScene::GetPlayer();	// �v���C���[���
	assert(pPlayer != nullptr);

	for (const auto& rSpawn : m_spawnTiming)
	{ // �S�v�f���J��Ԃ�

		// �����^�C�~���O�ł͂Ȃ��ꍇ���Ɉڍs
		if (rSpawn.nMotion != pPlayer->GetMotionType(CPlayer::BODY_LOWER))			{ continue; }
		if (rSpawn.nFrame  != pPlayer->GetMotionWholeCounter(CPlayer::BODY_LOWER))	{ continue; }

		D3DXMATRIX  mtxPlayer = pPlayer->CalcMtxWorld();	// �v���C���[�}�g���b�N�X
		D3DXMATRIX  mtxSpawn, mtxOffset;	// �����ʒu�E�I�t�Z�b�g�}�g���b�N�X
		D3DXVECTOR3 posSpawn, rotSpawn;		// �����ʒu�E����

		// �����ʒu��ݒ�
		D3DXMatrixTranslation(&mtxOffset, rSpawn.posOffset.x, rSpawn.posOffset.y, rSpawn.posOffset.z);	// �I�t�Z�b�g�}�g���b�N�X���쐬
		D3DXMatrixMultiply(&mtxSpawn, &mtxOffset, &mtxPlayer);	// �v���C���[�}�g���b�N�X�Ɗ|�����킹��
		posSpawn = useful::GetMatrixPosition(mtxSpawn);			// �������[���h���W���擾

		// ����������ݒ�
		rotSpawn = pPlayer->GetVec3Rotation() + rSpawn.rotOffset;
		useful::Vec3NormalizeRot(rotSpawn);	// �������K��

		// ���g���̐���
		CSwordWave::Create
		( // ����
			posSpawn,	// �ʒu
			rotSpawn,	// ����
			GROW,		// ������
			ADD_GROW,	// ����������
			HOLE_RAD,	// ���̔��a
			THICKNESS	// ����
		);
	}
}

//============================================================
//	�����^�C�~���O�̒ǉ�����
//============================================================
void CSwordWaveManager::AddTiming
(
	const D3DXVECTOR3& rPosOffset,	// �����ʒu�I�t�Z�b�g
	const D3DXVECTOR3& rRotOffset,	// ���������I�t�Z�b�g
	const int nMotion,	// �������[�V����
	const int nFrame	// �����t���[��
)
{
	// ����ݒ�
	STiming tempTiming;	// �V�K�^�C�~���O�ݒ�p
	tempTiming.posOffset = rPosOffset;	// �����ʒu�I�t�Z�b�g
	tempTiming.rotOffset = rRotOffset;	// ���������I�t�Z�b�g
	tempTiming.nMotion	 = nMotion;		// �������[�V����
	tempTiming.nFrame	 = nFrame;		// �����t���[��

	// �z��Ō���ɐ����^�C�~���O��ǉ�
	m_spawnTiming.push_back(tempTiming);
}

//============================================================
//	�����^�C�~���O�̒ǉ�����
//============================================================
void CSwordWaveManager::AddTiming(const STiming& rTiming)
{
	// �z��Ō���ɐ����^�C�~���O��ǉ�
	m_spawnTiming.push_back(rTiming);
}

//============================================================
//	��������
//============================================================
CSwordWaveManager *CSwordWaveManager::Create(void)
{
	// ���g���}�l�[�W���[�̐���
	CSwordWaveManager *pWaveManager = new CSwordWaveManager;
	if (pWaveManager == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// ���g���}�l�[�W���[�̏�����
		if (FAILED(pWaveManager->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ���g���}�l�[�W���[�̔j��
			SAFE_DELETE(pWaveManager);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pWaveManager;
	}
}

//============================================================
//	�j������
//============================================================
void CSwordWaveManager::Release(CSwordWaveManager *&prSwordWaveManager)
{
	// ���g���}�l�[�W���[�̏I��
	assert(prSwordWaveManager != nullptr);
	prSwordWaveManager->Uninit();

	// �������J��
	SAFE_DELETE(prSwordWaveManager);
}
