//============================================================
//
//	���U������ [attackThunder.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "attackThunder.h"
#include "manager.h"
#include "thunder.h"
#include "object3D.h"
#include "scene.h"
#include "stage.h"
#include "collision.h"
#include "impact.h"
#include "player.h"
#include "camera.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char *TEXTURE_WARNING = "data\\TEXTURE\\warning000.png";	// �x���\���̃e�N�X�`���t�@�C��

	const CCamera::SSwing LAND_SWING = CCamera::SSwing(8.0f, 1.5f, 0.25f);	// �����e���̃J�����h��

	const int	WARNING_PRIO	= 0;		// �x���\���̗D�揇��
	const int	DMG_THUNDER		= 15;		// ���̃_���[�W��
	const float COLL_RADIUS		= 120.0f;	// ����̔��a

	const int	DIV_DIRRAND	= 21;			// �����̏�]�Z�̒l
	const int	SUB_DIRRAND	= 10;			// �����̌��Z�̒l
	const float SHIFT_POS_LENGTH = 40.0f;	// �������ʒu�̂��炷����

	// �Ռ��g�̏��
	namespace impact
	{
		const CWave::SGrow GROW		= CWave::SGrow(4.5f, 2.0f, 0.0f);	// ������
		const CWave::SGrow ADDGROW	= CWave::SGrow(0.0f, 0.0f, 0.0f);	// ����������
		const float	HOLE_RADIUS	= 80.0f;	// ���̔��a
		const float	THICKNESS	= 1.0f;		// ����
		const float	OUTER_PLUSY	= 20.0f;	// �O����Y���W���Z��
		const float	MAX_RADIUS	= 0.0f;		// ���a�̍ő听����
	}
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CListManager<CAttackThunder> *CAttackThunder::m_pList = nullptr;	// �I�u�W�F�N�g���X�g

//************************************************************
//	�q�N���X [CAttackThunder] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CAttackThunder::CAttackThunder() : CObject(CObject::LABEL_THUNDER, DIM_3D),
	m_pWarning		(nullptr),		// �x���\���̏��
	m_posOrigin		(VEC3_ZERO),	// ���̌��_�ʒu
	m_state			(STATE_WARN),	// ���
	m_nCounterState	(0),			// ��ԊǗ��J�E���^�[
	m_nWarnFrame	(0)				// �x���\���t���[����
{
	// �����o�ϐ����N���A
	memset(&m_apThunder[0], 0, sizeof(m_apThunder));	// ���̏��
}

//============================================================
//	�f�X�g���N�^
//============================================================
CAttackThunder::~CAttackThunder()
{

}

//============================================================
//	����������
//============================================================
HRESULT CAttackThunder::Init(void)
{
	// �����o�ϐ����N���A
	memset(&m_apThunder[0], 0, sizeof(m_apThunder));	// ���̏��
	m_pWarning	= nullptr;		// �x���\���̏��
	m_posOrigin = VEC3_ZERO;	// ���̌��_�ʒu
	m_state		= STATE_WARN;	// ���
	m_nCounterState = 0;		// ��ԊǗ��J�E���^�[
	m_nWarnFrame	= 0;		// �x���\���t���[����

	// �x���\���̐���
	m_pWarning = CObject3D::Create
	( // ����
		VEC3_ZERO,	// �ʒu
		D3DXVECTOR3(COLL_RADIUS * 2.0f, 0.0f, COLL_RADIUS * 2.0f)	// �傫��
	);
	if (m_pWarning == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�N�X�`����o�^�E����
	m_pWarning->BindTexture(TEXTURE_WARNING);

	// �D�揇�ʂ�ݒ�
	m_pWarning->SetPriority(WARNING_PRIO);

	// �����_�[�X�e�[�g�̐ݒ�
	CRenderState *pRenderState = m_pWarning->GetRenderState();	// �����_�[�X�e�[�g�̏��
	pRenderState->SetZFunc(D3DCMP_ALWAYS);						// Z�e�X�g���ΐ����ɂ���
	pRenderState->SetAlphaBlend(CRenderState::BLEND_ADD);		// ���Z�����ɂ���

	if (m_pList == nullptr)
	{ // ���X�g�}�l�[�W���[�����݂��Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̐���
		m_pList = CListManager<CAttackThunder>::Create();
		if (m_pList == nullptr)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CAttackThunder::Uninit(void)
{
	// �x���\���̏I��
	SAFE_UNINIT(m_pWarning);

	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DeleteList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

	// ���U���I�u�W�F�N�g��j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CAttackThunder::Update(void)
{
	// �|�C���^��錾
	CStage *pStage = CScene::GetStage();	// �X�e�[�W�̏��
	assert(pStage != nullptr);	// �X�e�[�W���g�p

	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_WARN:

		// �J�E���^�[�����Z
		m_nCounterState++;
		if (m_nCounterState > m_nWarnFrame)
		{ // �x���\�����I�������ꍇ

			// �U���J�n�̐ݒ�
			if (FAILED(SetAttack())) { assert(false); }
		}

		// �x���\���̍X�V
		m_pWarning->Update();

		break;

	case STATE_ATTACK:

		for (int nCntThunder = 0; nCntThunder < attackThunder::NUM_THUNDER; nCntThunder++)
		{ // ���̐��������J��Ԃ�

			if (m_apThunder[nCntThunder]->IsLanding())
			{ // �����n�ʂɓ��B���Ă���ꍇ

				// �J�����h���ݒ�
				GET_MANAGER->GetCamera()->SetSwing(CCamera::TYPE_MAIN, LAND_SWING);

				// �Ռ��g�̐���
				D3DXVECTOR3 posImpact = D3DXVECTOR3(m_posOrigin.x, pStage->GetStageLimit().fField, m_posOrigin.z);
				CImpact::Create
				( // ����
					CWave::TEXTURE_NONE,	// ���
					posImpact,				// �ʒu
					impact::GROW,			// ������
					impact::ADDGROW,		// ����������
					impact::HOLE_RADIUS,	// ���̔��a
					impact::THICKNESS,		// ����
					impact::OUTER_PLUSY,	// �O����Y���W���Z��
					impact::MAX_RADIUS,		// ���a�̍ő听����
					false					// �����蔻��
				);

				// ���g���I��
				Uninit();

				// �����𔲂���
				return;
			}
		}

		// �v���C���[�Ƃ̓����蔻��
		CollisionPlayer();

		break;

	default:
		assert(false);
		break;
	}
}

//============================================================
//	�`�揈��
//============================================================
void CAttackThunder::Draw(void)
{

}

//============================================================
//	��������
//============================================================
CAttackThunder *CAttackThunder::Create
(
	const D3DXVECTOR3 &rPos,	// �ʒu
	const int nWarnFrame		// �x���\���t���[����
)
{
	// ���U���̐���
	CAttackThunder *pAttackThunder = new CAttackThunder;
	if (pAttackThunder == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// ���U���̏�����
		if (FAILED(pAttackThunder->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ���U���̔j��
			SAFE_DELETE(pAttackThunder);
			return nullptr;
		}

		// ���̌��_�ʒu��ݒ�
		pAttackThunder->SetOriginPosition(rPos);

		// �x���\���t���[����ݒ�
		pAttackThunder->m_nWarnFrame = nWarnFrame;

		// �m�ۂ����A�h���X��Ԃ�
		return pAttackThunder;
	}
}

//============================================================
//	���X�g�擾����
//============================================================
CListManager<CAttackThunder> *CAttackThunder::GetList(void)
{
	// �I�u�W�F�N�g���X�g��Ԃ�
	return m_pList;
}

//============================================================
//	�j������
//============================================================
void CAttackThunder::Release(void)
{
	// �I�u�W�F�N�g�̔j��
	CObject::Release();
}

//============================================================
//	���_�ʒu�̐ݒ菈��
//============================================================
void CAttackThunder::SetOriginPosition(const D3DXVECTOR3 &rPos)
{
	// �|�C���^��錾
	CStage *pStage = CScene::GetStage();	// �X�e�[�W�̏��
	assert(pStage != nullptr);	// �X�e�[�W���g�p

	// ���_�ʒu��ݒ�
	m_posOrigin = rPos;

	// �X�e�[�W�͈͊O�̕␳
	pStage->LimitPosition(m_posOrigin, COLL_RADIUS);

	// �x���\���̈ʒu��ݒ�
	D3DXVECTOR3 posWarn = D3DXVECTOR3(m_posOrigin.x, pStage->GetStageLimit().fField, m_posOrigin.z);
	m_pWarning->SetVec3Position(posWarn);
}

//============================================================
//	���ʒu�̐ݒ菈��
//============================================================
void CAttackThunder::SetThunderPosition(const D3DXVECTOR3 &rPos)
{
	// �ŏ��̗��̈ʒu��^�񒆂ɐݒ�
	m_apThunder[0]->SetVec3Position(rPos);

	for (int nCntThunder = 0; nCntThunder < attackThunder::NUM_THUNDER - 1; nCntThunder++)
	{ // ���̐��������J��Ԃ�

		// ���̐���������ݒ�
		float fRotRate = (D3DX_PI * 2.0f) / (float)(attackThunder::NUM_THUNDER - 1);	// ���������̕�������
		float fRotDir  = (fRotRate * nCntThunder - D3DX_PI) + ((rand() % DIV_DIRRAND - SUB_DIRRAND) * 0.01f);	// ��������
		useful::NormalizeRot(fRotDir);	// �����𐳋K��

		// ���̈ʒu��ݒ�
		D3DXVECTOR3 posThunder = VEC3_ZERO;
		posThunder.x = rPos.x + sinf(fRotDir) * SHIFT_POS_LENGTH;
		posThunder.z = rPos.z + cosf(fRotDir) * SHIFT_POS_LENGTH;

		// ���̈ʒu��ݒ�
		int nSetID = nCntThunder + 1;	// �ʒu��ݒ肷�闋�̃C���f�b�N�X
		m_apThunder[nSetID]->SetVec3Position(posThunder);
	}
}

//============================================================
//	�U���J�n�̐ݒ菈��
//============================================================
HRESULT CAttackThunder::SetAttack(void)
{
	// �U���J�n��Ԃɂ���
	m_state = STATE_ATTACK;

	// ��ԊǗ��J�E���^�[
	m_nCounterState = 0;

	for (int nCntThunder = 0; nCntThunder < attackThunder::NUM_THUNDER; nCntThunder++)
	{ // ���̐��������J��Ԃ�

		// ���̐���
		m_apThunder[nCntThunder] = CThunder::Create(VEC3_ZERO);
		if (m_apThunder[nCntThunder] == nullptr)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}
	}

	// ���ʒu��ݒ�
	SetThunderPosition(m_posOrigin);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�v���C���[�Ƃ̓����蔻��
//============================================================
void CAttackThunder::CollisionPlayer(void)
{
	CListManager<CPlayer> *pList = CPlayer::GetList();		// �v���C���[���X�g
	if (pList == nullptr)		 { assert(false); return; }	// ���X�g���g�p
	if (pList->GetNumAll() != 1) { assert(false); return; }	// �v���C���[��1�l����Ȃ�
	auto player = pList->GetList().front();					// �v���C���[���

	D3DXVECTOR3 posPlayer	= player->GetVec3Position();	// �v���C���[�̈ʒu
	float fRadiusPlayer		= player->GetRadius();			// �v���C���[�̔��a
	float fHeightPlayer		= player->GetHeight();			// �v���C���[�̏c��

	if (collision::Circle2D(m_posOrigin, posPlayer, COLL_RADIUS, fRadiusPlayer))
	{ // �Ռ��g�ɓ������Ă���ꍇ

		if (m_apThunder[0]->GetVec3Position().y < posPlayer.y + fHeightPlayer)
		{ // �v���C���[�̓������ɓ������Ă���ꍇ

			// �v���C���[�̃q�b�g����
			player->Hit(DMG_THUNDER);
		}
	}
}
