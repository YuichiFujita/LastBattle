//============================================================
//
//	�G���G���� [enemySmall.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "enemySmall.h"
#include "manager.h"
#include "renderer.h"
#include "gauge3D.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char *TEXTURE_LIFEFRAME = "data\\TEXTURE\\lifeframe000.png";	// �̗̓t���[���\���̃e�N�X�`���t�@�C��

	// �̗͂̏��
	namespace lifeInfo
	{
		const D3DXCOLOR		FRONT_COL	= D3DXCOLOR(1.0f, 0.3f, 0.4f, 1.0f);			// �\�Q�[�W�F
		const D3DXCOLOR		BACK_COL	= D3DXCOLOR(0.03f, 0.008f, 0.01f, 1.0f);		// ���Q�[�W�F
		const D3DXVECTOR3	GAUGE_SIZE	= D3DXVECTOR3(65.0f, 9.0f, 0.0f);				// �Q�[�W�̑傫��
		const D3DXVECTOR3	FRAME_SIZE	= GAUGE_SIZE + D3DXVECTOR3(0.5f, 0.5f, 0.0f);	// �t���[���̑傫��
		const int	CHANGE_FRAME		= 10;		// �\���l�ϓ��t���[��
		const float	PLUS_HEIGHT_POSY	= 20.0f;	// �̗͕\��Y���W�̐g������̉��Z��
	}
}

//************************************************************
//	�q�N���X [CEnemySmall] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEnemySmall::CEnemySmall(const EType type) : CEnemy(type), m_pLife(nullptr)
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CEnemySmall::~CEnemySmall()
{

}

//============================================================
//	����������
//============================================================
HRESULT CEnemySmall::Init(void)
{
	// �����o�ϐ���������
	m_pLife = nullptr;	// �̗͂̏��

	// �G�̏�����
	if (FAILED(CEnemy::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �̗͂̐���
	SStatusInfo status = GetStatusInfo();	// �X�e�[�^�X���
	float fPosUp = status.fHeight + lifeInfo::PLUS_HEIGHT_POSY;	// �\��Y�ʒu�̉��Z��
	m_pLife = CGauge3D::Create
	( // ����
		this,					// �Q�[�W�\���I�u�W�F�N�g
		status.nMaxLife,		// �ő�\���l
		lifeInfo::CHANGE_FRAME,	// �\���l�ϓ��t���[��
		fPosUp,					// �\��Y�ʒu�̉��Z��
		lifeInfo::GAUGE_SIZE,	// �Q�[�W�傫��
		lifeInfo::FRONT_COL,	// �\�Q�[�W�F
		lifeInfo::BACK_COL,		// ���Q�[�W�F
		true,					// �g�`���
		TEXTURE_LIFEFRAME,		// �t���[���e�N�X�`���p�X
		lifeInfo::FRAME_SIZE	// �g�傫��
	);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CEnemySmall::Uninit(void)
{
	// ���g�̏I����`����
	m_pLife->DeleteGauge();

	// �G�̏I��
	CEnemy::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CEnemySmall::Update(void)
{
	// �G�̍X�V
	CEnemy::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CEnemySmall::Draw(CShader *pShader)
{
	// �G�̕`��
	CEnemy::Draw(pShader);
}

//============================================================
//	�q�b�g����
//============================================================
void CEnemySmall::Hit(const int nDamage)
{
	if (IsDeath())					{ return; }	// ���S�ς�
	if (GetState() != STATE_NORMAL)	{ return; }	// �ʏ��ԈȊO
	if (m_pLife->GetNum() <= 0)		{ return; }	// �̗͂Ȃ�

	// �ϐ���錾
	D3DXVECTOR3 posEnemy = GetVec3Position();	// �G�ʒu
	D3DXVECTOR3 rotEnemy = GetVec3Rotation();	// �G����

	// �̗͂Ƀ_���[�W��^����
	m_pLife->AddNum(-nDamage);

	if (m_pLife->GetNum() > 0)
	{ // �̗͂��c���Ă���ꍇ

		// �_���[�W��Ԃɂ���
		SetState(STATE_DAMAGE);
	}
	else
	{ // �̗͂��c���Ă��Ȃ��ꍇ

		// ���S��Ԃɂ���
		SetState(STATE_DEATH);
	}
}

//============================================================
//	�m�b�N�o�b�N�q�b�g����
//============================================================
void CEnemySmall::HitKnockBack(const int /*nDamage*/, const D3DXVECTOR3 & /*vecKnock*/)
{
#if 0

	if (IsDeath()) { return; }	// ���S�ς�
	if (m_state != STATE_NORMAL) { return; }	// �ʏ��ԈȊO

	// �ϐ���錾
	D3DXVECTOR3 posPlayer = GetVec3Position();	// �v���C���[�ʒu
	D3DXVECTOR3 rotPlayer = GetVec3Rotation();	// �v���C���[����

	// �J�E���^�[��������
	m_nCounterState = 0;

	// �m�b�N�o�b�N�ړ��ʂ�ݒ�
	m_move.x = KNOCK_SIDE * vecKnock.x;
	m_move.y = KNOCK_UP;
	m_move.z = KNOCK_SIDE * vecKnock.z;

	// �m�b�N�o�b�N�����Ɍ�����ݒ�
	rotPlayer.y = atan2f(vecKnock.x, vecKnock.z);	// ������ь������v�Z
	SetVec3Rotation(rotPlayer);	// ������ݒ�

	// �󒆏�Ԃɂ���
	m_bJump = true;

	// �m�b�N��Ԃ�ݒ�
	SetState(STATE_KNOCK);

	// �T�E���h�̍Đ�
	CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_HIT);	// �q�b�g��

#endif
}
