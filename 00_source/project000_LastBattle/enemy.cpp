//============================================================
//
//	�G���� [enemy.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "enemy.h"
#include "manager.h"
#include "renderer.h"
#include "stage.h"
#include "enemyMiniDragon.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int	PRIORITY	= 3;		// �G�̗D�揇��
	const float	GRAVITY		= 1.0f;		// �d��
	const float	JUMP_REV	= 0.16f;	// �󒆂̈ړ��ʂ̌����W��
	const float	LAND_REV	= 0.16f;	// �n��̈ړ��ʂ̌����W��
	const float	ADD_ALPHA	= 0.03f;	// �����x�̉��Z��
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CListManager<CEnemy> *CEnemy::m_pList = nullptr;	// �I�u�W�F�N�g���X�g
CEnemy::SStatusInfo CEnemy::m_aStatusInfo[CEnemy::TYPE_MAX] =	// �X�e�[�^�X���
{
	// �~�j�h���S���̃X�e�[�^�X���
	{
		50.0f,	// ���a
		40.0f,	// �c��
		0.25f,	// ���F�̕␳�W��
		75.0f,	// ���蔼�a
	},
};

//************************************************************
//	�q�N���X [CEnemy] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEnemy::CEnemy(const EType type) : CObjectModel(CObject::LABEL_ENEMY, CObject::DIM_3D, PRIORITY),
	m_type			(type),					// ��ޒ萔
	m_status		(m_aStatusInfo[type]),	// �X�e�[�^�X�萔
	m_oldPos		(VEC3_ZERO),			// �ߋ��ʒu
	m_move			(VEC3_ZERO),			// �ړ���
	m_state			(STATE_SPAWN),			// ���
	m_bJump			(false),				// �W�����v��
	m_nCounterState	(0)						// ��ԊǗ��J�E���^�[
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CEnemy::~CEnemy()
{

}

//============================================================
//	����������
//============================================================
HRESULT CEnemy::Init(void)
{
	// �����o�ϐ���������
	m_oldPos	= VEC3_ZERO;	// �ߋ��ʒu
	m_move		= VEC3_ZERO;	// �ړ���
	m_state		= STATE_SPAWN;	// ���
	m_bJump		= false;		// �W�����v��
	m_nCounterState	= 0;		// ��ԊǗ��J�E���^�[

	// �I�u�W�F�N�g���f���̏�����
	if (FAILED(CObjectModel::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	if (m_pList == nullptr)
	{ // ���X�g�}�l�[�W���[�����݂��Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̐���
		m_pList = CListManager<CEnemy>::Create();
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
void CEnemy::Uninit(void)
{
	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DeleteList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

	// �I�u�W�F�N�g���f���̏I��
	CObjectModel::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CEnemy::Update(void)
{
	// �I�u�W�F�N�g���f���̍X�V
	CObjectModel::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CEnemy::Draw(void)
{
	// �I�u�W�F�N�g���f���̕`��
	CObjectModel::Draw();
}

#if 0
//============================================================
//	�m�b�N�o�b�N�q�b�g����
//============================================================
void CEnemy::HitKnockBack(const int nDmg, const D3DXVECTOR3&vecKnock)
{
	if (IsDeath())				 { return; }	// ���S�ς�
	if (m_state != STATE_NORMAL) { return; }	// �ʏ��ԈȊO

	// �ϐ���錾
	D3DXVECTOR3 posPlayer = GetVec3Position();	// �v���C���[�ʒu
	D3DXVECTOR3 rotPlayer = GetVec3Rotation();	// �v���C���[����

	// �J�E���^�[��������
	m_nCounterState = 0;

	// �m�b�N�o�b�N�ړ��ʂ�ݒ�
	m_movePos.x = KNOCK_SIDE * vecKnock.x;
	m_movePos.y = KNOCK_UP;
	m_movePos.z = KNOCK_SIDE * vecKnock.z;

	// �m�b�N�o�b�N�����Ɍ�����ݒ�
	rotPlayer.y = atan2f(vecKnock.x, vecKnock.z);	// ������ь������v�Z
	SetVec3Rotation(rotPlayer);	// ������ݒ�

	// �󒆏�Ԃɂ���
	m_bJump = true;

	// �m�b�N��Ԃ�ݒ�
	SetState(STATE_KNOCK);

	// �T�E���h�̍Đ�
	CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_HIT);	// �q�b�g��
}
#endif

//============================================================
//	��Ԃ̐ݒ菈��
//============================================================
void CEnemy::SetState(const int nState)
{
	// �����̏�Ԃ�ݒ�
	m_state = (EState)nState;
}

//============================================================
//	��Ԏ擾����
//============================================================
int CEnemy::GetState(void) const
{
	// ��Ԃ�Ԃ�
	return m_state;
}

//============================================================
//	��ގ擾����
//============================================================
int CEnemy::GetType(void) const
{
	// ��ނ�Ԃ�
	return m_type;
}

//============================================================
//	���a�擾����
//============================================================
float CEnemy::GetRadius(void) const
{
	// ���a��Ԃ�
	return m_status.fRadius;
}

//============================================================
//	�c���擾����
//============================================================
float CEnemy::GetHeight(void) const
{
	// �c����Ԃ�
	return m_status.fHeight;
}

//============================================================
//	��������
//============================================================
CEnemy *CEnemy::Create(const EType type, const D3DXVECTOR3& rPos, const D3DXVECTOR3& rRot)
{
	// �|�C���^��錾
	CEnemy *pEnemy = nullptr;	// �G���

	// �G�̐���
	switch (type)
	{ // ��ނ��Ƃ̏���
	case TYPE_MINI_DRAGON:

		// �~�j�h���S���𐶐�
		pEnemy = new CEnemyMiniDragon(type);

		break;

	default:	// ��O����
		assert(false);
		break;
	}

	if (pEnemy == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �G�̏�����
		if (FAILED(pEnemy->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �G�̔j��
			SAFE_DELETE(pEnemy);
			return nullptr;
		}

		// �ʒu��ݒ�
		pEnemy->SetVec3Position(rPos);

		// ������ݒ�
		pEnemy->SetVec3Rotation(rRot);

		// �m�ۂ����A�h���X��Ԃ�
		return pEnemy;
	}
}

//============================================================
//	���X�g�擾����
//============================================================
CListManager<CEnemy> *CEnemy::GetList(void)
{
	// �I�u�W�F�N�g���X�g��Ԃ�
	return m_pList;
}

//============================================================
//	�X�e�[�^�X���擾����
//============================================================
CEnemy::SStatusInfo CEnemy::GetStatusInfo(const int nType)
{
	// �����̎�ނ̃X�e�[�^�X����Ԃ�
	return m_aStatusInfo[nType];
}

//============================================================
//	�ߋ��ʒu�̍X�V����
//============================================================
void CEnemy::UpdateOldPosition(void)
{
	// �ߋ��ʒu���X�V
	m_oldPos = GetVec3Position();
}

//============================================================
//	�ߋ��ʒu�擾����
//============================================================
D3DXVECTOR3 CEnemy::GetOldPosition(void) const
{
	// �ߋ��ʒu��Ԃ�
	return m_oldPos;
}

//============================================================
//	�ړ��ʂ̐ݒ菈��
//============================================================
void CEnemy::SetMovePosition(const D3DXVECTOR3 &rMove)
{
	// �����̈ړ��ʂ�ݒ�
	m_move = rMove;
}

//============================================================
//	�ړ��ʎ擾����
//============================================================
D3DXVECTOR3 CEnemy::GetMovePosition(void) const
{
	// �ړ��ʂ�Ԃ�
	return m_move;
}

//============================================================
//	�X�e�[�^�X���擾����
//============================================================
CEnemy::SStatusInfo CEnemy::GetStatusInfo(void) const
{
	// �X�e�[�^�X����Ԃ�
	return m_status;
}

//============================================================
//	�X�|�[����Ԏ��̍X�V����
//============================================================
void CEnemy::UpdateSpawn(void)
{
	// �ϐ���錾
	D3DXVECTOR3 posEnemy = GetVec3Position();	// �G�ʒu
	D3DXVECTOR3 rotEnemy = GetVec3Rotation();	// �G����

	// �|�C���^��錾
	CStage *pStage = CScene::GetStage();				// �X�e�[�W���
	if (pStage == nullptr) { assert(false); return; }	// �X�e�[�W��g�p��

	// �d�͂̍X�V
	UpdateGravity();

	// �ʒu�X�V
	UpdatePosition(&posEnemy);

	// ���n����
	UpdateLanding(&posEnemy);

	// �X�e�[�W�͈͊O�̕␳
	pStage->LimitPosition(posEnemy, m_status.fRadius);

	// �ʒu�𔽉f
	SetVec3Position(posEnemy);

	// �����𔽉f
	SetVec3Rotation(rotEnemy);

	// �t�F�[�h�A�E�g��Ԏ��̍X�V
	if (UpdateFadeOut(ADD_ALPHA))
	{ // �s�����ɂȂ�؂����ꍇ

		// �ʏ��Ԃɂ���
		m_state = STATE_NORMAL;
	}
}

//============================================================
//	�ʏ��Ԏ��̍X�V����
//============================================================
void CEnemy::UpdateNormal(void)
{
	// �ϐ���錾
	D3DXVECTOR3 posEnemy = GetVec3Position();	// �G�ʒu
	D3DXVECTOR3 rotEnemy = GetVec3Rotation();	// �G����

	// �|�C���^��錾
	CStage *pStage = CScene::GetStage();				// �X�e�[�W���
	if (pStage == nullptr) { assert(false); return; }	// �X�e�[�W��g�p��

	// �d�͂̍X�V
	UpdateGravity();

	// �ʒu�X�V
	UpdatePosition(&posEnemy);

	// ���n����
	UpdateLanding(&posEnemy);

	// �X�e�[�W�͈͊O�̕␳
	pStage->LimitPosition(posEnemy, m_status.fRadius);

	// �ʒu�𔽉f
	SetVec3Position(posEnemy);

	// �����𔽉f
	SetVec3Rotation(rotEnemy);
}

//============================================================
//	�Ώۂ̎��F����
//============================================================
void CEnemy::UpdateLook(const D3DXVECTOR3& rPosLook, const D3DXVECTOR3& rPosEnemy, D3DXVECTOR3 *pRotEnemy)
{
	// �ϐ���錾
	float fDestRot = 0.0f;	// �ڕW����
	float fDiffRot = 0.0f;	// ����

	// ���F�Ώۂ̕�������
	fDestRot = atan2f(rPosEnemy.x - rPosLook.x, rPosEnemy.z - rPosLook.z);	// �ڕW����

	// �������������߂�
	fDiffRot = fDestRot - pRotEnemy->y;
	useful::NormalizeRot(fDiffRot);		// ���������̐��K��

	// �����̍X�V
	pRotEnemy->y += fDiffRot * m_status.fLookRev;
	useful::NormalizeRot(pRotEnemy->y);	// �����̐��K��
}

//============================================================
//	�d�͂̍X�V����
//============================================================
void CEnemy::UpdateGravity(void)
{
	// �d�͂����Z
	m_move.y -= GRAVITY;
}

//============================================================
//	���n�󋵂̍X�V����
//============================================================
bool CEnemy::UpdateLanding(D3DXVECTOR3 *pPos)
{
	// �ϐ���錾
	bool bLand = false;	// ���n��

	// �W�����v���Ă����Ԃɂ���
	m_bJump = true;

	// �n�ʁE�����ʒu�̒��n����
	if (CScene::GetStage()->LandFieldPosition(*pPos, m_move)
	||  CScene::GetStage()->LandLimitPosition(*pPos, m_move, 0.0f))
	{ // �v���C���[�����n���Ă����ꍇ

		// ���n���Ă����Ԃɂ���
		bLand = true;

		// �W�����v���Ă��Ȃ���Ԃɂ���
		m_bJump = false;
	}

	// ���n�󋵂�Ԃ�
	return bLand;
}

//============================================================
//	�ʒu�̍X�V����
//============================================================
void CEnemy::UpdatePosition(D3DXVECTOR3 *pPos)
{
	// �ʒu���ړ�
	*pPos += m_move;

	// �ړ��ʂ�����
	if (m_bJump)
	{ // �󒆂̏ꍇ

		m_move.x += (0.0f - m_move.x) * JUMP_REV;
		m_move.z += (0.0f - m_move.z) * JUMP_REV;
	}
	else
	{ // �n��̏ꍇ

		m_move.x += (0.0f - m_move.x) * LAND_REV;
		m_move.z += (0.0f - m_move.z) * LAND_REV;
	}
}

//============================================================
//	�t�F�[�h�A�E�g��Ԏ��̍X�V����
//============================================================
bool CEnemy::UpdateFadeOut(const float fAdd)
{
	// �ϐ���錾
	bool bAlpha = false;		// ������
	float fAlpha = GetAlpha();	// �����x

	// �v���C���[���g�̕`����ĊJ
	CObject::SetEnableDraw(true);

	// �����x���グ��
	fAlpha += fAdd;

	if (fAlpha >= GetMaxAlpha())
	{ // �����x���オ��؂����ꍇ

		// �����x��␳
		fAlpha = GetMaxAlpha();

		// �s�����ɂȂ�؂�����Ԃɂ���
		bAlpha = true;
	}

	// �����x��ݒ�
	SetAlpha(fAlpha);

	// �����󋵂�Ԃ�
	return bAlpha;
}

//============================================================
//	�t�F�[�h�C����Ԏ��̍X�V����
//============================================================
bool CEnemy::UpdateFadeIn(const float fSub)
{
	// �ϐ���錾
	bool bAlpha = false;		// ������
	float fAlpha = GetAlpha();	// �����x

	// �����x��������
	fAlpha -= fSub;

	if (fAlpha <= 0.0f)
	{ // �����x��������؂����ꍇ

		// �����x��␳
		fAlpha = 0.0f;

		// �����ɂȂ�؂�����Ԃɂ���
		bAlpha = true;

		// �v���C���[���g�̕`����~
		CObject::SetEnableDraw(false);
	}

	// �����x��ݒ�
	SetAlpha(fAlpha);

	// �����󋵂�Ԃ�
	return bAlpha;
}
