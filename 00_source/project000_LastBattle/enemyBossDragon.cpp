//============================================================
//
//	�{�X�h���S������ [enemyBossDragon.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "enemyBossDragon.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "stage.h"
#include "camera.h"
#include "multiModel.h"
#include "sceneGame.h"
#include "gameManager.h"
#include "gauge2D.h"
#include "magicCircle.h"
#include "retentionManager.h"
#include "random.h"
#include "hitStop.h"
#include "flash.h"
#include "timerManager.h"
#include "cinemaScope.h"
#include "player.h"
#include "collision.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char *MODEL_FILE[] =	// ���f���t�@�C��
	{
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\00_waist.x",		// ��
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\01_body.x",		// ��
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\02_jaw.x",		// �{
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\03_head.x",		// ��
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\04_armUL.x",		// ����r
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\05_armUR.x",		// �E��r
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\06_armDL.x",		// �����r
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\07_armDR.x",		// �E���r
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\08_handL.x",		// ����
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\09_handR.x",		// �E��
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\10_legUL.x",		// ��������
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\11_legUR.x",		// �E������
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\12_legDL.x",		// ����
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\13_legDR.x",		// �E��
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\14_footL.x",		// ����
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\15_footR.x",		// �E��
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\16_wingL_00.x",	// ����00
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\17_wingR_00.x",	// �E��00
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\18_wingL_01.x",	// ����01
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\19_wingR_01.x",	// �E��01
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\20_wingL_02.x",	// ����02
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\21_wingR_02.x",	// �E��02
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\22_wingL_03.x",	// ����03
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\23_wingR_03.x",	// �E��03
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\24_tail_00.x",	// �K��00
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\25_tail_01.x",	// �K��01
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\26_tail_02.x",	// �K��02
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\27_tail_03.x",	// �K��03
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\28_tail_04.x",	// �K��04
		"data\\MODEL\\ENEMY\\BOSS_DRAGON\\29_tail_05.x",	// �K��05
	};

	const D3DXVECTOR3 DEATH_CAMERA_ROT[] =	// ���S���J��������
	{
		VEC3_ZERO,	// ����͌����ݒ�Ȃ�
		D3DXVECTOR3(2.11f, -0.76f, 0.0f),
		D3DXVECTOR3(1.61f, 0.49f, 0.0f),
	};

	const CCamera::SSwing LAND_SWING	= CCamera::SSwing(10.0f, 1.5f, 0.12f);	// ���n�̃J�����h��
	const CCamera::SSwing HOWL_SWING	= CCamera::SSwing(14.0f, 2.0f, 0.09f);	// ���K�̃J�����h��
	const CCamera::SSwing RIDE_SWING	= CCamera::SSwing(9.5f, 2.0f, 0.09f);	// ��яオ��O�̙��K�̃J�����h��
	const int	BLEND_FRAME		= 16;			// ���[�V�����̃u�����h�t���[��
	const int	LAND_MOTION_KEY	= 9;			// ���[�V�����̒��n�̏u�ԃL�[
	const int	HOWL_MOTION_KEY	= 13;			// ���[�V�����̙��K�̊J�n�L�[
	const int	HOWL_WAIT_FRAME	= 40;			// ���K�̗]�C�t���[��
	const int	ATK_WAIT_FRAME	= 60;			// �U���̗]�C�t���[��
	const int	STAN_ATK_WAIT	= -30;			// �X�^����̍U���̗]�C�v���J�n�t���[��
	const float	REV_ROTA		= 0.15f;		// �����ύX�̕␳�W��
	const float	SCALE_MAGIC		= 35.0f;		// ���@�w�̔��a�ϓ���
	const float	MOVE_MAGIC		= 30.0f;		// ���@�w�̏㉺�ړ���
	const float	MAGIC_CIRCLE_RADIUS	= 400.0f;	// ���@�w�̔��a
	const float	MAGIC_ALPHA_RADIUS	= 400.0f;	// ���@�w�̓������a
	const float	MAGIC_DELPOS_PLUSY	= 250.0f;	// ���@�w�̏����ʒu�̉��Z��Y
	const float	RIDE_FIND_RADIUS	= 250.0f;	// ���C�h���̃{�X���m���a

	const int	NUM_MULTI_FLASH		= 2;	// ���S��A������t���b�V���̉�
	const int	HITSTOP_DEATH_START	= 90;	// ���S���߂̃q�b�g�X�g�b�v�̒���
	const int	HITSTOP_DEATH_MULTI	= 55;	// ���S��A������q�b�g�X�g�b�v�̒���
	const float	FLASH_INITALPHA_DEATH_START	= 0.55f;	// ���S���߂̃t���b�V���J�n�����x
	const float	FLASH_SUBALPHA_DEATH_START	= 0.02f;	// ���S��A������t���b�V�������x���Z��
	const float	FLASH_INITALPHA_DEATH_MULTI	= 0.35f;	// ���S���߂̃t���b�V���J�n�����x
	const float	FLASH_SUBALPHA_DEATH_MULTI	= 0.02f;	// ���S��A������t���b�V�������x���Z��

	const float	DEATH_CANERA_DIS = 800.0f;	// ���S�J�����̋���
	const D3DXVECTOR3 DEATH_CAMERA_OFFSET	= D3DXVECTOR3(35.0f, 106.0f, 0.0f);		// ���S�J�����̈ʒu�I�t�Z�b�g
	const CCamera::SSwing DEATH_START_SWING	= CCamera::SSwing(10.0f, 1.5f, 0.12f);	// ���S���߂̃J�����h��
	const CCamera::SSwing DEATH_MULTI_SWING	= CCamera::SSwing(8.6f, 1.5f, 0.24f);	// ���S��A������J�����h��

	const float	RIDE_ROTATE_DIS  = 2400.0f;	// ���񎞂̃X�e�[�W�������痣��鋗��
	const float	RIDE_ROTATE_POSY = 1200.0f;	// ���񎞂�Y���W
	const float	RIDE_ROTATE_MOVE = 0.005f;	// ���񎞂̉�]���x

	const int	FLYUP_MOTION_KEY = 2;		// ��яオ�胂�[�V�����̙��K�̊J�n�L�[
	const int	ROTATE_FRAME     = 560;		// ���v����t���[��
	const float	STAN_CANERA_DIS  = 600.0f;	// �X�^���J�����̋���
	const D3DXVECTOR3 STAN_CAMERA_OFFSET	= D3DXVECTOR3(0.0f, 100.0f, 0.0f);	// �X�^���J�����̈ʒu�I�t�Z�b�g
	const D3DXVECTOR3 STAN_CAMERA_ROT		= D3DXVECTOR3(1.46f, 0.0f, 0.0f);	// �X�^���J�����̌���

	// �̗͂̏��
	namespace lifeInfo
	{
		const char *TEXTURE_FRAME = "data\\TEXTURE\\lifeframe001.png";	// �̗̓t���[���\���̃e�N�X�`���t�@�C��

		const D3DXVECTOR3	POS				= D3DXVECTOR3(760.0f, 650.0f, 0.0f);			// �ʒu
		const D3DXCOLOR		COL_FRONT		= D3DXCOLOR(0.77f, 0.19f, 0.94f, 1.0f);			// �\�Q�[�W�F
		const D3DXCOLOR		COL_BACK		= D3DXCOLOR(0.02f, 0.008f, 0.03f, 1.0f);		// ���Q�[�W�F
		const D3DXVECTOR3	SIZE_GAUGE		= D3DXVECTOR3(460.0f, 20.0f, 0.0f);				// �Q�[�W�傫��
		const D3DXVECTOR3	SIZE_FRAME		= SIZE_GAUGE + D3DXVECTOR3(16.5f, 16.5f, 0.0f);	// �t���[���傫��
		const int			CHANGE_FRAME	= 10;	// �\���l�ϓ��t���[��
	}
}

//************************************************************
//	�}�N����`
//************************************************************
// �����_���U����ON/OFF
#if 0
#define RANDOM_ATTACK_ON	// �����_���U��
#define ATTACK (CEnemyAttack::ATTACK_02)
#endif

//************************************************************
//	�X�^�e�B�b�N�A�T�[�g
//************************************************************
static_assert(NUM_ARRAY(MODEL_FILE) == CEnemyBossDragon::MODEL_MAX,	"ERROR : Model Count Mismatch");
static_assert(NUM_ARRAY(DEATH_CAMERA_ROT) == NUM_MULTI_FLASH + 1,	"ERROR : Array Count Mismatch");

//************************************************************
//	�q�N���X [CEnemyBossDragon] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEnemyBossDragon::CEnemyBossDragon(const EType type) : CEnemy(type),
	m_pLife				(nullptr),			// �̗͂̏��
	m_pAttack			(nullptr),			// �U���̏��
	m_pMagicCircle		(nullptr),			// ���@�w�̏��
	m_action			(ACT_NONE),			// �s��
	m_nCounterSameAct	(0),				// �����s���̘A����
	m_nCounterAttack	(0),				// �U���Ǘ��J�E���^�[
	m_nCounterRotate	(0),				// ���C�h����J�E���^�[
	m_nCounterFlash		(0),				// �t���b�V���Ǘ��J�E���^�[
	m_oldAtk	(CEnemyAttack::ATTACK_00),	// �O��̍U��
	m_curAtk	(CEnemyAttack::ATTACK_00)	// ����̍U��
{
	// �����o�ϐ����N���A
	memset(&m_teleport, 0, sizeof(m_teleport));	// �e���|�[�g�̏��
	memset(&m_ride, 0, sizeof(m_ride));			// ���C�h�̏��
}

//============================================================
//	�f�X�g���N�^
//============================================================
CEnemyBossDragon::~CEnemyBossDragon()
{

}

//============================================================
//	����������
//============================================================
HRESULT CEnemyBossDragon::Init(void)
{
	// �����o�ϐ���������
	memset(&m_teleport, 0, sizeof(m_teleport));		// �e���|�[�g�̏��
	memset(&m_ride, 0, sizeof(m_ride));				// ���C�h�̏��
	m_oldAtk			= CEnemyAttack::ATTACK_00;	// �O��̍U��
	m_curAtk			= CEnemyAttack::ATTACK_00;	// ����̍U��
	m_pLife				= nullptr;		// �̗͂̏��
	m_pAttack			= nullptr;		// �U���̏��
	m_pMagicCircle		= nullptr;		// ���@�w�̏��
	m_action			= ACT_NONE;		// �s��
	m_nCounterSameAct	= 0;			// �����s���̘A����
	m_nCounterAttack	= 0;			// �U���Ǘ��J�E���^�[
	m_nCounterRotate	= 0;			// ���C�h����J�E���^�[
	m_nCounterFlash		= 0;			// �t���b�V���Ǘ��J�E���^�[

	// �G�̏�����
	if (FAILED(CEnemy::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �̗͂̐���
	m_pLife = CGauge2D::Create
	( // ����
		GetStatusInfo().nMaxLife,	// �ő�\���l
		lifeInfo::CHANGE_FRAME,		// �\���l�ϓ��t���[��
		lifeInfo::POS,				// �ʒu
		lifeInfo::SIZE_GAUGE,		// �Q�[�W�傫��
		lifeInfo::COL_FRONT,		// �\�Q�[�W�F
		lifeInfo::COL_BACK,			// ���Q�[�W�F
		true,						// �g�`���
		lifeInfo::TEXTURE_FRAME,	// �t���[���e�N�X�`���p�X
		lifeInfo::SIZE_FRAME		// �g�傫��
	);

	// ���@�w�̐���
	m_pMagicCircle = CMagicCircle::Create(VEC3_ZERO, VEC3_ZERO, 0.0f, MAGIC_ALPHA_RADIUS);
	if (m_pMagicCircle == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �����`���OFF�ɂ���
	m_pMagicCircle->SetEnableDraw(false);

	// �Ȃɂ����Ȃ���Ԃɂ���
	SetState(STATE_NONE);

	// �����`���OFF�ɂ���
	SetEnableDraw(false);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CEnemyBossDragon::Uninit(void)
{
	// �U���̔j��
	SAFE_REF_RELEASE(m_pAttack);

	// �G�̏I��
	CEnemy::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CEnemyBossDragon::Update(void)
{
	// �G�̍X�V
	CEnemy::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CEnemyBossDragon::Draw(CShader *pShader)
{
	CDrawBossShader	*pDrawBossShader = CDrawBossShader::GetInstance();	// �{�X�`��V�F�[�_�[���
	if (pDrawBossShader->IsEffectOK())
	{ // �G�t�F�N�g���g�p�\�ȏꍇ

		// �G�̕`��
		CEnemy::Draw(pDrawBossShader);
	}
	else
	{ // �G�t�F�N�g���g�p�s�\�ȏꍇ

		// �G�t�F�N�g�G���[
		assert(false);

		// �G�̕`��
		CEnemy::Draw(pShader);
	}
}

//============================================================
//	�����f���̃C���f�b�N�X�擾����
//============================================================
int CEnemyBossDragon::GetWaistModelID(void) const
{
	// �����f���̃C���f�b�N�X��Ԃ�
	return MODEL_WAIST;
}

//============================================================
//	�����f���̃C���f�b�N�X�擾����
//============================================================
int CEnemyBossDragon::GetHeadModelID(void) const
{
	// �����f���̃C���f�b�N�X��Ԃ�
	return MODEL_HEAD;
}

//============================================================
//	���C�h�\���̎擾����
//============================================================
bool CEnemyBossDragon::IsRideOK(const D3DXVECTOR3& rPos) const
{
	// �X�^����Ԃł͂Ȃ��ꍇ���C�h�s��
	if (GetState() != STATE_STAN) { return false; }

	// XZ���ʂ̉~�̓����蔻��
	bool bRideOK = collision::Circle2D
	( // ����
		GetVec3Position(),	// ����ʒu
		rPos,				// ����ڕW�ʒu
		RIDE_FIND_RADIUS,	// ���蔼�a
		0.0f				// ����ڕW���a
	);

	// �~������Ȃ烉�C�h�\
	return bRideOK;
}

//============================================================
//	UI�`��̐ݒ菈��
//============================================================
void CEnemyBossDragon::SetEnableDrawUI(const bool bDraw)
{
	// UI�`��̐ݒ�
	CEnemy::SetEnableDrawUI(bDraw);

	// UI�I�u�W�F�N�g�ɕ`��󋵂𔽉f
	m_pLife->SetEnableDraw(bDraw);	// �̗�
}

//============================================================
//	�q�b�g����
//============================================================
bool CEnemyBossDragon::Hit(const int nDamage)
{
	if (IsDeath())						{ return false; }	// ���S�ς�
	if (m_action == ACT_MAGIC_FADEIN)	{ return false; }	// ���@�w�t�F�[�h�C����
	if (m_action == ACT_MAGIC_FADEOUT)	{ return false; }	// ���@�w�t�F�[�h�A�E�g��
	if (m_pLife->GetNum() <= 0)			{ return false; }	// �̗͂Ȃ�

	int nState = GetState();
	if (nState != STATE_NORMAL
	&&  nState != STATE_RIDE_ROTATE)
	{ // �ʏ�E���C�h�����ԈȊO�̏ꍇ������

		return false;
	}

	// �̗͂Ƀ_���[�W��^����
	m_pLife->AddNum(-nDamage);

	if (m_pLife->GetNum() > 0)
	{ // �̗͂��c���Ă���ꍇ

		// �_���[�W���󂯂�O�̏�Ԃ�ۑ�
		SetPrevState((EState)GetState());

		// �_���[�W��Ԃɂ���
		SetState(STATE_DAMAGE);
	}
	else
	{ // �̗͂��c���Ă��Ȃ��ꍇ

		// ���S��Ԃɂ���
		SetState(STATE_DEATH);
	}

	return true;
}

//============================================================
//	�m�b�N�o�b�N�q�b�g����
//============================================================
bool CEnemyBossDragon::HitKnockBack(const int /*nDamage*/, const D3DXVECTOR3 & /*vecKnock*/)
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
	PLAY_SOUND->Play(CSound::LABEL_SE_HIT);	// �q�b�g��

#endif

	return false;
}

//============================================================
//	�ʏ��Ԃ̏���������
//============================================================
void CEnemyBossDragon::InitNormal(void)
{
	// �ϐ���錾
	D3DXVECTOR3 posEnemy = GetVec3Position();	// �G�ʒu

	// �ʏ��Ԃ̏�����
	CEnemy::InitNormal();

	// �ʏ��Ԃɂ���
	SetState(STATE_NORMAL);

	// �����`���ON�ɂ���
	SetEnableDraw(true);

	// �����x��s�����ɍĐݒ�
	SetAlpha(1.0f);

	// �}�e���A�����Đݒ�
	ResetMaterial();

	// �ҋ@���[�V������ݒ�
	SetMotion(MOTION_IDOL);

	// ���n����
	UpdateLanding(&posEnemy);

	// �ʒu�͈͊O�̕␳
	LimitPosition(&posEnemy);

	// �ʒu�𔽉f
	SetVec3Position(posEnemy);
}

//============================================================
//	�؂蔲���p�̕`�揈��
//============================================================
void CEnemyBossDragon::DrawCrop(void)
{
	// �����`�悪OFF�̏ꍇ������
	if (!IsDraw()) { return; }

	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// �f�o�C�X���
	CMonoShader	*pMonoShader  = CMonoShader::GetInstance();	// �P�F�`��V�F�[�_�[���
	if (pMonoShader->IsEffectOK())
	{ // �G�t�F�N�g���g�p�\�ȏꍇ

		// �s�N�Z���`��F����ʃN���A�F�ɐݒ�
		pMonoShader->SetColor(GET_RENDERER->GetClearColor());

		// �X�e���V���e�X�g��L���ɂ���
		pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

		// ��r�Q�ƒl��ݒ肷��
		pDevice->SetRenderState(D3DRS_STENCILREF, 1);

		// �X�e���V���}�X�N���w�肷�� 
		pDevice->SetRenderState(D3DRS_STENCILMASK, 255);

		// �X�e���V����r�֐����w�肷��
		pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_GREATEREQUAL);

		// �X�e���V�����ʂɑ΂��Ă̔��f�ݒ�
		pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);	// Z�e�X�g�E�X�e���V���e�X�g����
		pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);		// Z�e�X�g�E�X�e���V���e�X�g���s
		pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);		// Z�e�X�g���s�E�X�e���V���e�X�g����

		// �G�̕`��
		CEnemy::Draw(pMonoShader);

		// �X�e���V���e�X�g�𖳌��ɂ���
		pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
	}
	else { assert(false); }
}

//============================================================
//	�e���|�[�g�̐ݒ菈��
//============================================================
void CEnemyBossDragon::SetTeleport
(
	const D3DXVECTOR3 &rPos,	// �e���|�[�g�ڕW�ʒu
	const D3DXVECTOR3 &rRot,	// �e���|�[�g�ڕW����
	const EMotion motion,		// �e���|�[�g�ヂ�[�V����
	const bool bLook			// �e���|�[�g��ɃJ�������������邩
)
{
	// �e���|�[�g��̈ʒu�E������ۑ�
	m_teleport.pos = rPos;
	m_teleport.rot = rRot;

	// ���@�w�̈ʒu�E���������݈ʒu�̑����ɐݒ�
	m_pMagicCircle->SetVec3Position(GetVec3Position());
	m_pMagicCircle->SetVec3Rotation(GetVec3Rotation());

	if (motion == MOTION_NONE
	||  motion == MOTION_IDOL
	||  motion == MOTION_FLY_IDOL)
	{ // �ݒ�ł��郂�[�V�����̏ꍇ

		// �e���|�[�g��̃��[�V������ݒ�
		m_teleport.motion = motion;
	}
	else
	{ // �ݒ�ł��Ȃ����[�V�����̏ꍇ

		// ���[�V�����w��Ȃ���ݒ�
		assert(false);
		m_teleport.motion = MOTION_NONE;
	}

	// �e���|�[�g��̃J�����␳��ݒ�
	m_teleport.bLook = bLook;

	// �e���|�[�g�̏�������Ԃɂ���
	m_teleport.state = TELEPORT_INIT;

	// ���@�w�ɂ��t�F�[�h�C���̍s�����Ƃ点��
	m_action = ACT_MAGIC_FADEIN;
}

//============================================================
//	�n�ʉ���̍s���ݒ菈��
//============================================================
void CEnemyBossDragon::SetActPunchGround(void)
{
	// �n�ʉ��胂�[�V������ݒ�
	SetMotion(MOTION_PUNCH_GROUND, BLEND_FRAME);

	// �n�ʂ�����s�����Ƃ点��
	m_action = ACT_PUNCH_GROUND;
}

//============================================================
//	���U���̍s���ݒ菈��
//============================================================
void CEnemyBossDragon::SetActFireAttack(void)
{
	// ���U�����[�V������ݒ�
	SetMotion(MOTION_FIRE_ATTACK, BLEND_FRAME);

	// ����f���s�����Ƃ点��
	m_action = ACT_FIRE_ATTACK;
}

//============================================================
//	�Ђ������U���̍s���ݒ菈��
//============================================================
void CEnemyBossDragon::SetActClawAttack(void)
{
	// �Ђ������U�����[�V������ݒ�
	SetMotion(MOTION_CLAW_ATTACK, BLEND_FRAME);

	// �Ђ������U���̍s�����Ƃ点��
	m_action = ACT_CLAW_ATTACK;
}

//============================================================
//	�����ۍU���̍s���ݒ菈��
//============================================================
void CEnemyBossDragon::SetActTailAttack(void)
{
	// �����ۍU�����[�V������ݒ�
	SetMotion(MOTION_TAIL_ATTACK, BLEND_FRAME);

	// �����ۍU���̍s�����Ƃ点��
	m_action = ACT_TAIL_ATTACK;
}

//============================================================
//	�󒆍U���̍s���ݒ菈��
//============================================================
void CEnemyBossDragon::SetActFlyAttack(void)
{
	// �󒆍U�����[�V������ݒ�
	SetMotion(MOTION_FLY_ATTACK, BLEND_FRAME);

	// �󒆂ōU������s�����Ƃ点��
	m_action = ACT_FLY_ATTACK;
}

//============================================================
//	�s���擾����
//============================================================
CEnemyBossDragon::EAction CEnemyBossDragon::GetAction(void)
{
	// ���݂̍s����Ԃ�
	return m_action;
}

//============================================================
//	���f���t�@�C���擾����
//============================================================
const char *CEnemyBossDragon::GetModelFileName(const int nModel) const
{
	if (nModel > NONE_IDX && nModel < MODEL_MAX)
	{ // �g�p�ł���C���f�b�N�X�̏ꍇ

		// �����̃C���f�b�N�X�̃��f����Ԃ�
		return MODEL_FILE[nModel];
	}
	else { assert(false); return NONE_STRING; }	// �͈͊O
}

//============================================================
//	���[�V�����̍X�V����
//============================================================
void CEnemyBossDragon::UpdateMotion(void)
{
	if (IsDeath()) { return; }	// ���S���Ă���

	switch (GetMotionType())
	{ // ���[�V�������Ƃ̏���
	case MOTION_HOWL:			// ���K���[�V����
	case MOTION_IDOL:			// �ҋ@���[�V����
	case MOTION_FLY_IDOL:		// �󒆑ҋ@���[�V����
	case MOTION_STAN:			// �X�^�����[�V����
	case MOTION_SHAKE_OFF:		// �󒆐U�艺�낵���[�V����
	case MOTION_HOWL_FLYUP:		// ���K��яオ�胂�[�V����
	case MOTION_DEATH:			// ���S���[�V����
		break;

	case MOTION_PUNCH_GROUND:	// �n�ʉ��胂�[�V����

		if (IsMotionFinish())
		{ // ���[�V�������I�����Ă����ꍇ

			// �ҋ@���[�V�����Ɉڍs
			SetMotion(MOTION_IDOL, BLEND_FRAME);
		}

		break;

	case MOTION_CLAW_ATTACK:	// �Ђ������U�����[�V����

		if (IsMotionFinish())
		{ // ���[�V�������I�����Ă����ꍇ

			// �ҋ@���[�V�����Ɉڍs
			SetMotion(MOTION_IDOL, BLEND_FRAME);
		}

		break;

	case MOTION_FIRE_ATTACK:	// ���U�����[�V����

		if (IsMotionFinish())
		{ // ���[�V�������I�����Ă����ꍇ

			// �ҋ@���[�V�����Ɉڍs
			SetMotion(MOTION_IDOL, BLEND_FRAME);
		}

		break;

	case MOTION_TAIL_ATTACK:	// �����ۍU�����[�V����

		if (IsMotionFinish())
		{ // ���[�V�������I�����Ă����ꍇ

			// �ҋ@���[�V�����Ɉڍs
			SetMotion(MOTION_IDOL, BLEND_FRAME);
		}

		break;

	case MOTION_FLY_ATTACK:		// �󒆍U�����[�V����

		if (IsMotionFinish())
		{ // ���[�V�������I�����Ă����ꍇ

			// �󒆑ҋ@���[�V�����Ɉڍs
			SetMotion(MOTION_FLY_IDOL, BLEND_FRAME);
		}

		break;

	case MOTION_FLY_RUSH:		// �󒆓ːi�U�����[�V����

		// TODO�F�g���悤�ɂȂ�����Ȃ�ɑJ�ڂ��邩�w��
		assert(false);

		break;

	case MOTION_COME_DOWN:		// ��󕜋A���[�V����

		if (IsMotionFinish())
		{ // ���[�V�������I�����Ă����ꍇ

			// �ҋ@���[�V�����Ɉڍs
			SetMotion(MOTION_IDOL, BLEND_FRAME);
		}

		break;

	default:	// ��O����
		assert(false);
		break;
	}
}

//============================================================
//	�X�|�[����Ԃ̐ݒ菈��
//============================================================
void CEnemyBossDragon::SetSpawn(void)
{
	// �ϐ���錾
	D3DXVECTOR3 posEnemy = GetVec3Position();	// �G�ʒu
	D3DXVECTOR3 rotEnemy = GetVec3Rotation();	// �G����

	// �X�|�[����Ԃ̐ݒ�
	CEnemy::SetSpawn();

	// �����x��s�����ɍĐݒ�
	SetAlpha(1.0f);

	// �����`���ON�ɂ���
	SetEnableDraw(true);

	// ���K���[�V������ݒ�
	SetMotion(MOTION_HOWL);

	// ���n����
	UpdateLanding(&posEnemy);

	// �ʒu�͈͊O�̕␳
	LimitPosition(&posEnemy);

	// �ʒu�𔽉f
	SetVec3Position(posEnemy);

	// �����𔽉f
	SetVec3Rotation(rotEnemy);
	SetDestRotation(rotEnemy);

	// �J�������{�X���ڏ�Ԃɐݒ�
	GET_MANAGER->GetCamera()->SetState(CCamera::STATE_LOOK_BOSS);
	GET_MANAGER->GetCamera()->SetDestLookBoss();	// �J�����ڕW�ʒu�̏�����
}

//============================================================
//	�X�^����Ԃ̐ݒ菈��
//============================================================
void CEnemyBossDragon::SetStan(void)
{
	// �X�^����Ԃ̐ݒ�
	CEnemy::SetStan();

	// �J�E���^�[��������
	m_nCounterAttack = 0;	// �U���Ǘ��J�E���^�[
	m_nCounterRotate = 0;	// ���C�h����J�E���^�[
	m_nCounterFlash  = 0;	// �t���b�V���Ǘ��J�E���^�[

	// �X�^�����[�V������ݒ�
	SetMotion(MOTION_STAN, BLEND_FRAME);
}

//============================================================
//	���C�h��яオ���Ԃ̐ݒ菈��
//============================================================
void CEnemyBossDragon::SetRideFlyUp(void)
{
	CCamera *pCamera = GET_MANAGER->GetCamera();	// �J�������

	// ���C�h��яオ���Ԃ̐ݒ�
	CEnemy::SetRideFlyUp();

	// �J�E���^�[��������
	m_nCounterAttack = 0;	// �U���Ǘ��J�E���^�[
	m_nCounterRotate = 0;	// ���C�h����J�E���^�[
	m_nCounterFlash  = 0;	// �t���b�V���Ǘ��J�E���^�[

	// ���K��яオ�胂�[�V������ݒ�
	SetMotion(MOTION_HOWL_FLYUP, BLEND_FRAME);

	// �J�������������Ȃ���Ԃɐݒ� (�Œ�J�����ɂ���)
	pCamera->SetState(CCamera::STATE_NONE);

	// �J�����̒����_���{�X�̒��S�ɐݒ�
	pCamera->SetPositionR(GetVec3Position() + STAN_CAMERA_OFFSET);

	// �J�����̌�����ݒ�
	pCamera->SetRotation(GetVec3Rotation() + STAN_CAMERA_ROT);

	// �J�����̋�����ݒ�
	pCamera->SetDistance(STAN_CANERA_DIS);
}

//============================================================
//	���C�h�����Ԃ̐ݒ菈��
//============================================================
void CEnemyBossDragon::SetRideRotate(void)
{
	// ���C�h�����Ԃ̐ݒ�
	CEnemy::SetRideRotate();

	// �J�E���^�[��������
	m_nCounterAttack = 0;	// �U���Ǘ��J�E���^�[
	m_nCounterFlash  = 0;	// �t���b�V���Ǘ��J�E���^�[
}

//============================================================
//	���C�h�I����Ԃ̐ݒ菈��
//============================================================
void CEnemyBossDragon::SetRideEnd(void)
{
	CStage *pStage = CScene::GetStage();				// �X�e�[�W���
	CPlayer *pPlayer = CScene::GetPlayer();				// �v���C���[���
	D3DXVECTOR3 posPlayer = pPlayer->GetVec3Position();	// �v���C���[�ʒu
	D3DXVECTOR3 posEnemy = GetVec3Position();			// �{�X�ʒu
	D3DXVECTOR3 rotEnemy = GetVec3Rotation();			// �{�X����

	// ���C�h�I����Ԃ̐ݒ�
	CEnemy::SetRideEnd();

	// �J�E���^�[��������
	m_nCounterAttack = 0;	// �U���Ǘ��J�E���^�[
	m_nCounterRotate = 0;	// ���C�h����J�E���^�[
	m_nCounterFlash  = 0;	// �t���b�V���Ǘ��J�E���^�[

	// ��󕜋A���[�V������ݒ�
	SetMotion(MOTION_COME_DOWN);

	// �{�X�̈ʒu�𒆉��ɐݒ�
	posEnemy = pStage->GetStageLimit().center;	// �X�e�[�W������ݒ�

	// ���n����E�X�e�[�W�͈͊O�̕␳
	UpdateLanding(&posEnemy);
	pStage->LimitPosition(posEnemy, GetStatusInfo().fRadius);

	// �ʒu�𔽉f
	SetVec3Position(posEnemy);

	// �������v���C���[�����ɂ���
	rotEnemy.y = atan2f(posEnemy.x - posPlayer.x, posEnemy.z - posPlayer.z);
	SetVec3Rotation(rotEnemy);
	SetDestRotation(rotEnemy);
}

//============================================================
//	���S��Ԃ̐ݒ菈��
//============================================================
void CEnemyBossDragon::SetDeath(void)
{
	// ���S��Ԃ̐ݒ�
	CEnemy::SetDeath();

	// �Q�[����ʂł͂Ȃ��ꍇ������
	if (GET_MANAGER->GetMode() != CScene::MODE_GAME) { return; }

	CGameManager *pGameManager = CSceneGame::GetGameManager();	// �Q�[���}�l�[�W���[�̏��
	CTimerManager *pTimer = CSceneGame::GetTimerManager();		// �^�C�}�[�}�l�[�W���[�̏��
	CCinemaScope *pScope = CSceneGame::GetCinemaScope();		// �V�l�}�X�R�[�v�̏��
	CHitStop *pHitStop	= CSceneGame::GetHitStop();	// �q�b�g�X�g�b�v���
	CFlash	*pFlash		= CSceneGame::GetFlash();	// �t���b�V�����
	CCamera	*pCamera	= GET_MANAGER->GetCamera();	// �J�������

	// �t���b�V���Ǘ��J�E���^�[��������
	m_nCounterFlash = 0;

	// �Q�[���L�����̐F��񃊃Z�b�g
	pGameManager->ResetColorGameChara();

	// �Q�[��UI�̎����`���OFF�ɂ���
	pGameManager->SetDrawGameUI(false);

	// �X�R�[�v�C��������
	pScope->SetScopeIn();

	// ���S���[�V������ݒ�
	SetMotion(MOTION_DEATH);

	// �q�b�g�X�g�b�v������
	pHitStop->SetStop(HITSTOP_DEATH_START);

	// �t���b�V����ݒ�
	pFlash->Set
	( // ����
		FLASH_INITALPHA_DEATH_START,	// �J�n�����x
		FLASH_SUBALPHA_DEATH_START		// �����x���Z��
	);

	// �J�����h���ݒ�
	pCamera->SetSwing(CCamera::TYPE_MAIN, DEATH_START_SWING);

	// �J�������������Ȃ���Ԃɐݒ� (�Œ�J�����ɂ���)
	pCamera->SetState(CCamera::STATE_NONE);

	// �Q�[�����I����Ԃɂ���
	CSceneGame::GetGameManager()->GameEnd();

	// �^�C�}�[�̌v���I��
	pTimer->End();
}

//============================================================
//	�X�|�[����Ԏ��̍X�V����
//============================================================
void CEnemyBossDragon::UpdateSpawn(void)
{
	// �ϐ���錾
	D3DXVECTOR3 posEnemy = GetVec3Position();	// �G�ʒu
	assert(GetMotionType() == MOTION_HOWL);		// ���[�V���������K������Ȃ�

	if (GetMotionKey() == LAND_MOTION_KEY && GetMotionKeyCounter() == 0)
	{ // ���[�V���������n�����^�C�~���O�̏ꍇ

		// ���n�̃J�����h��ݒ�
		GET_MANAGER->GetCamera()->SetSwing(CCamera::TYPE_MAIN, LAND_SWING);
	}
	else if (GetMotionKey() == HOWL_MOTION_KEY && GetMotionKeyCounter() == 0)
	{ // ���[�V�����������J���n�߂��^�C�~���O�̏ꍇ

		// ���K�̃J�����h��ݒ�
		GET_MANAGER->GetCamera()->SetSwing(CCamera::TYPE_MAIN, HOWL_SWING);

		// �h���S�����K�̍Đ�
		PLAY_SOUND(CSound::LABEL_SE_DRAGON_ROAR_L);
	}
	else if (IsMotionFinish())
	{ // ���[�V�������I�����Ă����ꍇ

		// �J�E���^�[�����Z
		m_nCounterAttack++;
		if (m_nCounterAttack > HOWL_WAIT_FRAME)
		{ // �]�C���I�������ꍇ

			// �J�E���^�[��������
			m_nCounterAttack = 0;

			// �Ȃɂ����Ȃ���Ԃɂ���
			SetState(STATE_NONE);

			// �ҋ@���[�V�����ɂ���
			SetMotion(MOTION_IDOL, BLEND_FRAME);
		}
	}

	// ���n����
	UpdateLanding(&posEnemy);

	// �ʒu�͈͊O�̕␳
	LimitPosition(&posEnemy);

	// �ʒu�𔽉f
	SetVec3Position(posEnemy);
}

//============================================================
//	�ʏ��Ԏ��̍X�V����
//============================================================
void CEnemyBossDragon::UpdateNormal(void)
{
	// �U���̍X�V
	UpdateAttack();

	// �s���̍X�V
	UpdateAction();
}

//============================================================
//	�X�^����Ԏ��̍X�V����
//============================================================
void CEnemyBossDragon::UpdateStan(void)
{
	// �X�^����Ԃ̍X�V
	CEnemy::UpdateStan();

	if (GetState() == STATE_NORMAL)
	{ // �e�N���X�X�V�ŃX�^�����I�������ꍇ

		// �U���܂ł̑ҋ@���Ԃ�ݒ�
		m_nCounterAttack = STAN_ATK_WAIT;

		// �ҋ@���[�V�����Ɉڍs
		SetMotion(MOTION_IDOL, BLEND_FRAME);
	}
}

//============================================================
//	���C�h��яオ���Ԏ��̍X�V����
//============================================================
void CEnemyBossDragon::UpdateRideFlyUp(void)
{
	// ���C�h��яオ���Ԃ̍X�V
	CEnemy::UpdateRideFlyUp();

	// �ʃ��[�V�����w��G���[
	assert(GetMotionType() == MOTION_HOWL_FLYUP);

	if (GetMotionKey() == FLYUP_MOTION_KEY && GetMotionKeyCounter() == 0)
	{ // ���[�V�����������J���n�߂��^�C�~���O�̏ꍇ

		// ��яオ��O�̙��K�̃J�����h��ݒ�
		GET_MANAGER->GetCamera()->SetSwing(CCamera::TYPE_MAIN, RIDE_SWING);

		// �h���S�����K�̍Đ�
		PLAY_SOUND(CSound::LABEL_SE_DRAGON_ROAR_S);
	}
	else if (IsMotionFinish())
	{ // ���[�V�������I�����Ă���ꍇ

		CCamera *pCamera = GET_MANAGER->GetCamera();	// �J�������

		// ���C�h����������
		m_ride.fDir = 0.0f;	// �{�X������������

		// �����s���[�V������ݒ�
		SetMotion(MOTION_FLY_IDOL);

		// �J�������R���Ԃɐݒ�
		pCamera->SetState(CCamera::STATE_RIDE);
		pCamera->SetDestRide();

		// ���C�h�����Ԃɂ���
		SetState(STATE_RIDE_ROTATE);
	}
}

//============================================================
//	���C�h�����Ԏ��̍X�V����
//============================================================
void CEnemyBossDragon::UpdateRideRotate(void)
{
	// ���C�h�����Ԏ��̍X�V
	CEnemy::UpdateRideRotate();

	CStage *pStage = CScene::GetStage();		// �X�e�[�W���
	D3DXVECTOR3 posEnemy = GetVec3Position();	// �G�ʒu
	D3DXVECTOR3 rotEnemy = GetVec3Rotation();	// �G����
	D3DXVECTOR3 posCenter = pStage->GetStageLimit().center;	// �X�e�[�W�����ʒu

	// �G�̈ʒu��ݒ�
	posEnemy.x = posCenter.x + sinf(m_ride.fDir) * RIDE_ROTATE_DIS;
	posEnemy.y = RIDE_ROTATE_POSY;
	posEnemy.z = posCenter.z + cosf(m_ride.fDir) * RIDE_ROTATE_DIS;

	// �G�̌�����i�s�����ɐݒ�
	rotEnemy.y = atan2f(posCenter.x - posEnemy.x, posCenter.z - posEnemy.z);
	rotEnemy.y += D3DX_PI * 0.5f;

	// �G�ʒu�̐ݒ��������]
	m_ride.fDir += RIDE_ROTATE_MOVE;
	useful::NormalizeRot(m_ride.fDir);

	// �ʒu�E�������f
	SetVec3Position(posEnemy);
	SetVec3Rotation(rotEnemy);

	if (CScene::GetPlayer()->GetState() != CPlayer::STATE_RIDE_END)
	{ // ���C�h���I�����Ă��Ȃ��ꍇ

		// �J�E���^�[�����Z
		m_nCounterRotate++;
		if (m_nCounterRotate > ROTATE_FRAME)
		{ // ���C�h�̍ő�o�ߎ��Ԃ̏ꍇ

			// ���C�h�I����Ԃɂ���
			CScene::GetPlayer()->SetRideEnd();

			// �󒆐U�艺�낵���[�V������ݒ�
			SetMotion(MOTION_SHAKE_OFF);

			// �h���S�����K�̍Đ�
			PLAY_SOUND(CSound::LABEL_SE_DRAGON_ROAR_S);
		}
	}
}

//============================================================
//	���C�h�I����Ԏ��̍X�V����
//============================================================
void CEnemyBossDragon::UpdateRideEnd(void)
{
	// ���C�h�I����Ԏ��̍X�V
	CEnemy::UpdateRideEnd();

	if (GetMotionType() != MOTION_COME_DOWN)
	{ // ���[�V��������󕜋A�ł͂Ȃ��Ȃ����ꍇ

		// �ʏ��Ԃ�ݒ�
		SetState(STATE_NORMAL);
	}
}

//============================================================
//	���S��Ԏ��̍X�V����
//============================================================
void CEnemyBossDragon::UpdateDeath(void)
{
	// �Q�[����ʂł͂Ȃ��ꍇ������
	if (GET_MANAGER->GetMode() != CScene::MODE_GAME) { return; }

	CHitStop *pHitStop	= CSceneGame::GetHitStop();	// �q�b�g�X�g�b�v���
	CFlash	*pFlash		= CSceneGame::GetFlash();	// �t���b�V�����
	CCamera	*pCamera	= GET_MANAGER->GetCamera();	// �J�������

	if (pFlash->GetState() == CFlash::FLASH_NONE)
	{ // �t���b�V�����I�������ꍇ

		if (m_nCounterFlash < NUM_MULTI_FLASH)
		{ // �ݒ肵���t���b�V���񐔂ɒB���Ă��Ȃ��ꍇ

			if (m_nCounterFlash == 0)
			{ // ����̃t���b�V�����I�������ꍇ

				// ���S���o�̐ݒ�
				SetDeathStaging();
			}

			// �t���b�V���������Z
			m_nCounterFlash++;

			// �q�b�g�X�g�b�v������
			pHitStop->SetStop(HITSTOP_DEATH_MULTI);

			// �t���b�V����ݒ�
			pFlash->Set
			( // ����
				FLASH_INITALPHA_DEATH_MULTI,	// �J�n�����x
				FLASH_SUBALPHA_DEATH_MULTI		// �����x���Z��
			);

			// �J�����̌�����ݒ�
			pCamera->SetRotation(DEATH_CAMERA_ROT[m_nCounterFlash]);

			// �J�����h���ݒ�
			pCamera->SetSwing(CCamera::TYPE_MAIN, DEATH_MULTI_SWING);
		}
		else
		{ // �ݒ肵���t���b�V���񐔂ɒB�����ꍇ

			if (IsMotionFinish())
			{ // ���[�V�������I�������ꍇ

				// ���U���g��ʂɑJ�ڂ�����
				CSceneGame::GetGameManager()->TransitionResult(CRetentionManager::WIN_CLEAR);
			}
		}
	}
}

//============================================================
//	�ʒu�͈͊O�̕␳����
//============================================================
void CEnemyBossDragon::LimitPosition(D3DXVECTOR3 *pPos)
{
	// �|�C���^��錾
	CStage *pStage = CScene::GetStage();				// �X�e�[�W���
	if (pStage == nullptr) { assert(false); return; }	// �X�e�[�W��g�p��

	if (!IsFly())
	{ // ��s���Ă��Ȃ��ꍇ

		// �X�e�[�W�͈͊O�̕␳
		pStage->LimitPosition(*pPos, GetStatusInfo().fRadius);
	}
}

//============================================================
//	�U���̑I������
//============================================================
void CEnemyBossDragon::SelectAttack(void)
{
	// �O��̍U����ۑ�
	m_oldAtk = m_curAtk;

	// �����_���������𐶐�
	CRandom<CEnemyAttack::EAttack> *pRandom = CRandom<CEnemyAttack::EAttack>::Create();
	if (pRandom == nullptr) { return; }		// �����~�X

	// �����\�ȍU����ݒ�
	if (m_pAttack == nullptr) { return; }	// �U���N���X����
	m_pAttack->SetRandomArray(pRandom, m_oldAtk, m_nCounterSameAct);

	// �U�����擾
	m_curAtk = pRandom->GetRandomNum();

	// �A���U���񐔂�ݒ�
	if (m_oldAtk == m_curAtk)
	{ // �O��̍U���Ɠ���̏ꍇ

		m_nCounterSameAct++;
	}
	else
	{ // �O��̍U���ƕʂ̏ꍇ

		m_nCounterSameAct = 0;
	}

	// �����_����������j��
	CRandom<CEnemyAttack::EAttack>::Release(pRandom);
}

//============================================================
//	�U���̍X�V����
//============================================================
void CEnemyBossDragon::UpdateAttack(void)
{
	if (m_pAttack == nullptr)
	{ // �U�������݂��Ȃ��ꍇ

		// �J�E���^�[�����Z
		m_nCounterAttack++;
		if (m_nCounterAttack > ATK_WAIT_FRAME)
		{ // �U���̗]�C���I�������ꍇ

			// �J�E���^�[��������
			m_nCounterAttack = 0;

			// �U���̐���
			m_pAttack = CEnemyAttack::Create
			( // ����
#ifndef RANDOM_ATTACK_ON
				m_curAtk, this
#else
				ATTACK, this
#endif
			);
			if (m_pAttack == nullptr)
			{ // �����Ɏ��s�����ꍇ

				// �֐��𔲂���
				assert(false);
				return;
			}
		}
	}
	else
	{ // �U�������݂���ꍇ

		// �U���̍X�V
		if (m_pAttack->Update())
		{ // �U�����I�����Ă���ꍇ

			// ���U���̑I��
			SelectAttack();

			// �U���̔j��
			SAFE_REF_RELEASE(m_pAttack);

			// �������Ȃ��s�����Ƃ点��
			m_action = ACT_NONE;
		}
	}
}

//============================================================
//	�s���̍X�V����
//============================================================
void CEnemyBossDragon::UpdateAction(void)
{
	// �ϐ���錾
	D3DXVECTOR3 posEnemy = GetVec3Position();		// �G�ʒu
	D3DXVECTOR3 rotEnemy = GetVec3Rotation();		// �G����
	D3DXVECTOR3 rotDestEnemy = GetDestRotation();	// �G�ڕW����

	// �ߋ��ʒu�̍X�V
	UpdateOldPosition();

	if (!IsFly())
	{ // ��s���Ă��Ȃ��ꍇ

		// �d�͂̍X�V
		UpdateGravity();
	}

	switch (m_action)
	{ // �s�����Ƃ̏���
	case ACT_NONE:	// �������Ȃ�
		break;

	case ACT_MAGIC_FADEIN:	// ���@�w�t�F�[�h�C��

		// ���@�w�t�F�[�h�C���s�����̍X�V
		UpdateMagicFadeIn(posEnemy);

		break;

	case ACT_MAGIC_FADEOUT:	// ���@�w�t�F�[�h�A�E�g

		// ���@�w�t�F�[�h�A�E�g�s�����̍X�V
		UpdateMagicFadeOut(&posEnemy, &rotEnemy, &rotDestEnemy);

		break;

	case ACT_PUNCH_GROUND:	// �n�ʉ���

		// �n�ʉ���̍s�����̍X�V
		UpdatePunchGround();

		break;

	case ACT_FIRE_ATTACK:	// ���U��

		// ���U���̍s�����̍X�V
		UpdateFireAttack();

		break;

	case ACT_CLAW_ATTACK:	// �Ђ������U��

		// �Ђ������U���̍s�����̍X�V
		UpdateClawAttack();

		break;

	case ACT_TAIL_ATTACK:	// �����ۍU��

		// �����ۍU���̍s�����̍X�V
		UpdateTailAttack();

		break;

	case ACT_FLY_ATTACK:	// �󒆍U��

		// �󒆍U���̍s�����̍X�V
		UpdatePunchGround();

		break;

	default:	// ��O����
		assert(false);
		break;
	}

	// �ʒu�X�V
	UpdatePosition(&posEnemy);

	// ���n����
	UpdateLanding(&posEnemy);

	// �����X�V
	UpdateRotation(&rotEnemy, &rotDestEnemy);

	// �ʒu�͈͊O�̕␳
	LimitPosition(&posEnemy);

	// �ʒu�𔽉f
	SetVec3Position(posEnemy);

	// �����𔽉f
	SetVec3Rotation(rotEnemy);

	// �ڕW�����𔽉f
	SetDestRotation(rotDestEnemy);
}

//============================================================
//	���S���o�̐ݒ菈��
//============================================================
void CEnemyBossDragon::SetDeathStaging(void)
{
	// �X�e�[�W�Ɏc�鉉�o�̎ז��ɂȂ�I�u�W�F�N�g��j��
	{
		// �j�����郉�x����v�f�ɒǉ�
		std::vector<ELabel> releaseLabel;	// �j�����x���z��
		releaseLabel.push_back(CObject::LABEL_EFFECT);		// �G�t�F�N�g
		releaseLabel.push_back(CObject::LABEL_PARTICLE);	// �p�[�e�B�N��
		releaseLabel.push_back(CObject::LABEL_FIRE);		// ��
		releaseLabel.push_back(CObject::LABEL_THUNDER);		// ��
		releaseLabel.push_back(CObject::LABEL_WAVE);		// �g��

		// �w�肵�����x���̃I�u�W�F�N�g�S�j��
		CObject::ReleaseAll(releaseLabel);

		// �j�����x���z����N���A
		releaseLabel.clear();
	}

	// �v���C���[�E�G�̉��o�ݒ�
	{
		// �v���C���[�̎����`���OFF�ɂ���
		CPlayer *pPlayer = CScene::GetPlayer();	// �v���C���[���
		pPlayer->SetEnableDraw(false);

		// ���E�O�Ղ̕\����OFF�ɂ���
		pPlayer->SetNoneTwinSword();

		// ���g�̈ʒu���X�e�[�W�����ɂ���
		CStage *pStage = CScene::GetStage();	// �X�e�[�W���
		D3DXVECTOR3 posEnemy = pStage->GetStageLimit().center;	// �{�X�ݒ�ʒu
		UpdateLanding(&posEnemy);	// ���n����
		LimitPosition(&posEnemy);	// �ʒu�͈͊O�̕␳
		SetVec3Position(posEnemy);	// �ʒu�𔽉f

		// ���g�̌�����������
		SetVec3Rotation(VEC3_ZERO);	// ����
		SetDestRotation(VEC3_ZERO);	// �ڕW����
	}

	// �J�������̐ݒ�
	{
		CCamera *pCamera = GET_MANAGER->GetCamera();	// �J�������

		// �J�����̒����_���{�X�̒��S�ɐݒ�
		pCamera->SetPositionR(GetVec3Position() + DEATH_CAMERA_OFFSET);

		// �J�����̋�����ݒ�
		pCamera->SetDistance(DEATH_CANERA_DIS);
	}
}

//============================================================
//	��s�t���O�擾����
//============================================================
bool CEnemyBossDragon::IsFly(void) const
{
	// �ϐ���錾
	EMotion curMotion = (EMotion)GetMotionType();	// ���݂̃��[�V����

	// TODO�F�󒆃��[�V�����ǉ�������L�q

	// ��s�󋵂�ݒ�
	bool bFly = (curMotion == MOTION_FLY_IDOL
			  || curMotion == MOTION_FLY_ATTACK
			  || curMotion == MOTION_FLY_RUSH
			  || m_teleport.motion == MOTION_FLY_IDOL
			  || m_teleport.motion == MOTION_FLY_ATTACK
			  || m_teleport.motion == MOTION_FLY_RUSH);

	// ��s�󋵂�Ԃ�
	return bFly;
}

//============================================================
//	���@�w�t�F�[�h�C���s�����̍X�V����
//============================================================
void CEnemyBossDragon::UpdateMagicFadeIn(const D3DXVECTOR3& rPos)
{
	// �ϐ���錾
	float fMagicPosY = m_pMagicCircle->GetVec3Position().y;	// ���@�w�̈ʒu

	switch (m_teleport.state)
	{ // �e���|�[�g��Ԃ��Ƃ̏���
	case TELEPORT_INIT:		// �e���|�[�g�̏�����
	{
		// ���@�w�̎����`���ON�ɂ���
		m_pMagicCircle->SetEnableDraw(true);

		// ���@�w�̈ʒu�𑫂̉��ɂ���
		fMagicPosY = rPos.y;

		// ���@�w�̏o����Ԃɂ���
		m_teleport.state = TELEPORT_APPEAR;

		// �����𔲂����ɂ��̂܂ܖ��@�w�̏o���Ɉڍs
	}
	case TELEPORT_APPEAR:	// ���@�w�̏o��
	{
		// �ϐ���錾
		float fMagicRadius = m_pMagicCircle->GetRadius();	// ���@�w�̔��a

		// ���@�w���g��
		fMagicRadius += SCALE_MAGIC;
		if (fMagicRadius >= MAGIC_CIRCLE_RADIUS)
		{ // �g�債�������ꍇ

			// ���a��␳
			fMagicRadius = MAGIC_CIRCLE_RADIUS;

			// ���@�w�̈ړ���Ԃɂ���
			m_teleport.state = TELEPORT_MOVE;
		}

		// ���@�w�̔��a�𔽉f
		m_pMagicCircle->SetRadius(fMagicRadius);

		break;
	}
	case TELEPORT_MOVE:	// ���@�w�̈ړ�
	{
		// �ϐ���錾
		float fDelPosY = GetMultiModel(MODEL_HEAD)->GetMtxWorld()._42 + MAGIC_DELPOS_PLUSY;	// ���@�w�̏����ʒu

		// ���@�w����ɓ�����
		fMagicPosY += MOVE_MAGIC;
		if (fMagicPosY >= fDelPosY)
		{ // �����ʒu�ɓ��B�����ꍇ

			// �ʒu��␳
			fMagicPosY = fDelPosY;

			// ���@�w�̏�����Ԃɂ���
			m_teleport.state = TELEPORT_DISAPPEAR;
		}

		break;
	}
	case TELEPORT_DISAPPEAR:	// ���@�w�̏���
	{
		// �ϐ���錾
		float fMagicRadius = m_pMagicCircle->GetRadius();	// ���@�w�̔��a

		// ���@�w���k��
		fMagicRadius -= SCALE_MAGIC;
		if (fMagicRadius <= 0.0f)
		{ // �k�����������ꍇ

			// ���a��␳
			fMagicRadius = 0.0f;

			// �e���|�[�g�̏�������Ԃɂ���
			m_teleport.state = TELEPORT_INIT;

			// ���@�w�ɂ��t�F�[�h�A�E�g�̍s�����Ƃ点��
			m_action = ACT_MAGIC_FADEOUT;
		}

		// ���@�w�̔��a�𔽉f
		m_pMagicCircle->SetRadius(fMagicRadius);

		break;
	}
	default:	// ��O����
		assert(false);
		break;
	}

	// ���@�w�̈ʒu�𔽉f
	m_pMagicCircle->SetVec3Position(D3DXVECTOR3(rPos.x, fMagicPosY, rPos.z));
}

//============================================================
//	���@�w�t�F�[�h�A�E�g�s�����̍X�V����
//============================================================
void CEnemyBossDragon::UpdateMagicFadeOut(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pRot, D3DXVECTOR3 *pDestRot)
{
	// �ϐ���錾
	float fMagicPosY = m_pMagicCircle->GetVec3Position().y;	// ���@�w�̈ʒu

	switch (m_teleport.state)
	{ // �e���|�[�g��Ԃ��Ƃ̏���
	case TELEPORT_INIT:		// �e���|�[�g�̏�����
	{
		if (m_teleport.motion > MOTION_NONE
		&&  m_teleport.motion < MOTION_MAX)
		{ // ���[�V�����w�肪����ꍇ

			// ���[�V������ݒ�
			SetMotion(m_teleport.motion, BLEND_FRAME);

			// �e���|�[�g�ヂ�[�V������������
			m_teleport.motion = MOTION_NONE;
		}

		// �{�X�̈ʒu�E�����E�ڕW�������e���|�[�g��ɕύX
		*pPos = m_teleport.pos;
		*pRot = m_teleport.rot;
		*pDestRot = m_teleport.rot;

		// �ʒu�͈͊O�̕␳
		LimitPosition(pPos);

		if (m_teleport.bLook)
		{ // �e���|�[�g������F����ꍇ

			// �e���|�[�g��̃{�X�����F������
			GET_MANAGER->GetCamera()->SetFollowLook(*pPos);
		}

		// ���@�w�̈ʒu�𓪂̏�ɂ���
		fMagicPosY  = GetMultiModel(MODEL_HEAD)->GetMtxWorld()._42 + MAGIC_DELPOS_PLUSY;
		fMagicPosY += pPos->y - GetOldPosition().y;	// �ߋ��ʒu���獡�t���[���ł��ꂽY���W�������Z

		// ���@�w�̏o����Ԃɂ���
		m_teleport.state = TELEPORT_APPEAR;

		// �����𔲂����ɂ��̂܂ܖ��@�w�̏o���Ɉڍs
	}
	case TELEPORT_APPEAR:	// ���@�w�̏o��
	{
		// �ϐ���錾
		float fMagicRadius = m_pMagicCircle->GetRadius();	// ���@�w�̔��a

		// ���@�w���g��
		fMagicRadius += SCALE_MAGIC;
		if (fMagicRadius >= MAGIC_CIRCLE_RADIUS)
		{ // �g�債�������ꍇ

			// ���a��␳
			fMagicRadius = MAGIC_CIRCLE_RADIUS;

			// ���@�w�̈ړ���Ԃɂ���
			m_teleport.state = TELEPORT_MOVE;
		}

		// ���@�w�̔��a�𔽉f
		m_pMagicCircle->SetRadius(fMagicRadius);

		break;
	}
	case TELEPORT_MOVE:	// ���@�w�̈ړ�
	{
		// �ϐ���錾
		float fDelPosY = GetVec3Position().y;	// ���@�w�̏����ʒu

		// ���@�w�����ɓ�����
		fMagicPosY -= MOVE_MAGIC;
		if (fMagicPosY <= fDelPosY)
		{ // �����ʒu�ɓ��B�����ꍇ

			// �ʒu��␳
			fMagicPosY = fDelPosY;

			// ���@�w�̏�����Ԃɂ���
			m_teleport.state = TELEPORT_DISAPPEAR;
		}

		break;
	}
	case TELEPORT_DISAPPEAR:	// ���@�w�̏���
	{
		// �ϐ���錾
		float fMagicRadius = m_pMagicCircle->GetRadius();	// ���@�w�̔��a

		// ���@�w���k��
		fMagicRadius -= SCALE_MAGIC;
		if (fMagicRadius <= 0.0f)
		{ // �k�����������ꍇ

			// ���a��␳
			fMagicRadius = 0.0f;

			// ���@�w�̎����`���OFF�ɂ���
			m_pMagicCircle->SetEnableDraw(false);

			// �������Ȃ��s�����Ƃ点��
			m_action = ACT_NONE;
		}

		// ���@�w�̔��a�𔽉f
		m_pMagicCircle->SetRadius(fMagicRadius);

		break;
	}
	default:	// ��O����
		assert(false);
		break;
	}

	// ���@�w�̈ʒu�𔽉f
	m_pMagicCircle->SetVec3Position(D3DXVECTOR3(pPos->x, fMagicPosY, pPos->z));
}

//============================================================
//	�n�ʉ���s�����̍X�V����
//============================================================
void CEnemyBossDragon::UpdatePunchGround(void)
{

}

//============================================================
//	���U���̍s�����̍X�V����
//============================================================
void CEnemyBossDragon::UpdateFireAttack(void)
{

}

//============================================================
//	�Ђ������U���̍s�����̍X�V����
//============================================================
void CEnemyBossDragon::UpdateClawAttack(void)
{

}

//============================================================
//	�����ۍU���̍s�����̍X�V����
//============================================================
void CEnemyBossDragon::UpdateTailAttack(void)
{

}

//============================================================
//	�󒆍U���̍s�����̍X�V����
//============================================================
void CEnemyBossDragon::UpdateFlyAttack(void)
{

}
