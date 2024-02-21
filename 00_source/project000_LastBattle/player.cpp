//============================================================
//
//	�v���C���[���� [player.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "player.h"
#include "manager.h"
#include "sceneGame.h"
#include "gameManager.h"
#include "renderer.h"
#include "sound.h"
#include "camera.h"
#include "texture.h"
#include "collision.h"
#include "fade.h"
#include "retentionManager.h"

#include "objectChara.h"
#include "multiModel.h"
#include "blur.h"
#include "sword.h"
#include "swordWaveManager.h"
#include "gauge2D.h"
#include "shadow.h"
#include "stage.h"
#include "field.h"
#include "enemy.h"

#include "effect3D.h"
#include "particle3D.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char *LOWER_MODEL_FILE[] =	// �����g���f���t�@�C��
	{
		"data\\MODEL\\PLAYER\\00_waist.x",	// ��
		"data\\MODEL\\PLAYER\\09_legUL.x",	// ��������
		"data\\MODEL\\PLAYER\\10_legUR.x",	// �E������
		"data\\MODEL\\PLAYER\\11_legDL.x",	// ����
		"data\\MODEL\\PLAYER\\12_legDR.x",	// �E��
		"data\\MODEL\\PLAYER\\13_footL.x",	// ����
		"data\\MODEL\\PLAYER\\14_footR.x",	// �E��
	};
	const char *UPPER_MODEL_FILE[] =	// �㔼�g���f���t�@�C��
	{
		"data\\MODEL\\PLAYER\\01_body.x",	// ��
		"data\\MODEL\\PLAYER\\02_head.x",	// ��
		"data\\MODEL\\PLAYER\\03_armUL.x",	// ����r
		"data\\MODEL\\PLAYER\\04_armUR.x",	// �E��r
		"data\\MODEL\\PLAYER\\05_armDL.x",	// �����r
		"data\\MODEL\\PLAYER\\06_armDR.x",	// �E���r
		"data\\MODEL\\PLAYER\\07_handL.x",	// ����
		"data\\MODEL\\PLAYER\\08_handR.x",	// �E��
	};

	const char *SETUP_TXT[] =	// �Z�b�g�A�b�v�e�L�X�g���΃p�X
	{
		"data\\TXT\\playerMotionLower.txt",	// �����g
		"data\\TXT\\playerMotionUpper.txt",	// �㔼�g
	};
	const char **MODEL_PASS[] =	// ���f���ő吔
	{
		&LOWER_MODEL_FILE[0],	// �����g
		&UPPER_MODEL_FILE[0],	// �㔼�g
	};
	const D3DXVECTOR3 SWORD_OFFSET[] =	// ���̃I�t�Z�b�g�ʒu
	{
		D3DXVECTOR3(-5.8f, 0.0f, 0.0f),	// �E��
		D3DXVECTOR3(5.8f, 0.0f, 0.0f),	// ����
	};
	const D3DXVECTOR3 SPAWN_POS = D3DXVECTOR3(0.0f, 0.0f, -600.0f);	// �X�|�[���ʒu
	const D3DXVECTOR3 SPAWN_ROT = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);	// �X�|�[������

	const int	PRIORITY	= 3;		// �v���C���[�̗D�揇��
	const float	INIT_JUMP	= 4.0f;		// �W�����v�ʂ̏����l
	const float	PULS_JUMP	= 4.15f;	// �W�����v���x�㏸��
	const float	GRAVITY		= 0.85f;	// �d��
	const float	RADIUS		= 20.0f;	// ���a
	const float	HEIGHT		= 80.0f;	// �c��
	const float	REV_ROTA	= 0.15f;	// �����ύX�̕␳�W��
	const float	MOVE_REV	= 0.5f;		// �ړ��ʂ̕␳�W��
	const float	DODGE_REV	= 0.12f;	// ����̈ړ��ʂ̌����W��
	const float	JUMP_REV	= 0.16f;	// �󒆂̈ړ��ʂ̌����W��
	const float	LAND_REV	= 0.16f;	// �n��̈ړ��ʂ̌����W��
	const float	STICK_REV	= 0.00015f;	// �ړ�����X�e�B�b�N�̌X���ʂ̕␳�W��
	const float	ADD_ALPHA	= 0.03f;	// �����x�̉��Z��
	const float	LOOK_RADIUS	= 500.0f;	// �G�̕�������������ۂ̓G���m���a
	const float	LOOK_REV	= 0.5f;		// �G�̕�������������ۂ̕␳�W��
	const int	WALK_SOUND	= 4;		// ���s�̌��ʉ��̃L�[�^�C�~���O

	const int	DAMAGE_FRAME	= 14;		// �_���[�W��Ԃ̈ێ��t���[��
	const int	INVULN_FRAME	= 120;		// ���G��Ԃ̈ێ��t���[��
	const float	INVULN_ALPHA	= 0.7f;		// ���G��Ԃ̊�b�����x
	const float	ADD_SINROT		= 0.2f;		// �����x���ӂ�ӂ킳����ۂ̃T�C���J�[�u�������Z��
	const float	MAX_ADD_ALPHA	= 0.25f;	// �����x�̍ő���Z��
	const float	DODGE_REV_MOVE	= 0.065f;	// ����ړ��ʂ̌��Z�W��
	const float	DODGE_MIN_MOVE	= 0.35f;	// ����ړ��ʂ̍ŏ��l
	const float	DODGE_SIDE_MOVE	= 3.2f;		// ����̉��ړ���
	const float	DODGE_JUMP_MOVE	= 9.0f;		// ����̏�ړ���
	const int	DODGE_WAIT_FRAME	= 100;	// ����̃t�[���^�C���t���[��
	const int	PRESS_JUMP_FRAME	= 10;	// �W�����v���x�㏸�̎�t���͎���
	const int	ATTACK_BUFFER_FRAME = 8;	// �U���̐�s���͉\�t���[��

	const D3DXVECTOR3 SHADOW_SIZE	= D3DXVECTOR3(80.0f, 0.0f, 80.0f);		// �e�̑傫��
	const char *TEXTURE_LIFEFRAME	= "data\\TEXTURE\\lifeframe002.png";	// �̗̓t���[���\���̃e�N�X�`���t�@�C��

	// �̗͂̏��
	namespace lifeInfo
	{
		const D3DXVECTOR3	POS			= D3DXVECTOR3(260.0f, 170.0f, 0.0f);		// �ʒu
		const D3DXVECTOR3	SIZE_GAUGE	= D3DXVECTOR3(200.0f, 20.0f, 0.0f);			// �Q�[�W�傫��
		const D3DXCOLOR		COL_FRONT	= D3DXCOLOR(0.93f, 0.92f, 0.25f, 1.0f);		// �\�Q�[�W�F
		const D3DXCOLOR		COL_BACK	= D3DXCOLOR(0.03f, 0.03f, 0.008f, 1.0f);	// ���Q�[�W�F
		const int	MAX_LIFE		= 100;	// �ő�\���l
		const int	CHANGE_FRAME	= 40;	// �\���l�ϓ��t���[��
	}

	// �u���[�̏��
	namespace blurInfo
	{
		const float	START_ALPHA	= 0.4f;	// �u���[�J�n�����x
		const int	MAX_LENGTH	= 12;	// �ێ��I�u�W�F�N�g�ő吔
	}
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CListManager<CPlayer> *CPlayer::m_pList = nullptr;			// �I�u�W�F�N�g���X�g
CPlayer::AFuncUpdateMotion CPlayer::m_aFuncUpdateMotion[] =	// ���[�V�����X�V�֐�
{
	&CPlayer::UpdateMotionLower,	// �����g���[�V�����̍X�V
	&CPlayer::UpdateMotionUpper,	// �㔼�g���[�V�����̍X�V
};

//************************************************************
//	�X�^�e�B�b�N�A�T�[�g
//************************************************************
static_assert(NUM_ARRAY(LOWER_MODEL_FILE) == CPlayer::L_MODEL_MAX, "ERROR : Model Count Mismatch");
static_assert(NUM_ARRAY(UPPER_MODEL_FILE) == CPlayer::U_MODEL_MAX, "ERROR : Model Count Mismatch");
static_assert(NUM_ARRAY(SETUP_TXT)    == CPlayer::BODY_MAX, "ERROR : Body Count Mismatch");
static_assert(NUM_ARRAY(SWORD_OFFSET) == player::NUM_SWORD, "ERROR : Body Count Mismatch");

//************************************************************
//	�q�N���X [CPlayer] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CPlayer::CPlayer() : CObjectDivChara(CObject::LABEL_PLAYER, CObject::DIM_3D, PRIORITY),
	m_pLife			(nullptr),		// �̗͂̏��
	m_pShadow		(nullptr),		// �e�̏��
	m_oldPos		(VEC3_ZERO),	// �ߋ��ʒu
	m_move			(VEC3_ZERO),	// �ړ���
	m_destRot		(VEC3_ZERO),	// �ڕW����
	m_state			(STATE_NONE),	// ���
	m_fSinAlpha		(0.0f),			// ��������
	m_nCounterState	(0)				// ��ԊǗ��J�E���^�[
{
	// �����o�ϐ����N���A
	memset(&m_apSowrd,	0, sizeof(m_apSowrd));	// ���̏��
	memset(&m_apBlur,	0, sizeof(m_apBlur));	// �u���[�̏��
	memset(&m_jump,		0, sizeof(m_jump));		// �W�����v�̏��
	memset(&m_dodge,	0, sizeof(m_dodge));	// ����̏��
	memset(&m_attack,	0, sizeof(m_attack));	// �U���̏��
}

//============================================================
//	�f�X�g���N�^
//============================================================
CPlayer::~CPlayer()
{

}

//============================================================
//	����������
//============================================================
HRESULT CPlayer::Init(void)
{
	// �����o�ϐ���������
	memset(&m_apSowrd,	0, sizeof(m_apSowrd));	// ���̏��
	memset(&m_apBlur,	0, sizeof(m_apBlur));	// �u���[�̏��
	memset(&m_jump,		0, sizeof(m_jump));		// �W�����v�̏��
	memset(&m_dodge,	0, sizeof(m_dodge));	// ����̏��
	memset(&m_attack,	0, sizeof(m_attack));	// �U���̏��
	m_pLife			= nullptr;		// �̗͂̏��
	m_pShadow		= nullptr;		// �e�̏��
	m_oldPos		= VEC3_ZERO;	// �ߋ��ʒu
	m_move			= VEC3_ZERO;	// �ړ���
	m_destRot		= VEC3_ZERO;	// �ڕW����
	m_state			= STATE_NONE;	// ���
	m_fSinAlpha		= 0.0f;			// ��������
	m_nCounterState	= 0;			// ��ԊǗ��J�E���^�[

	// �I�u�W�F�N�g�����L�����N�^�[�̏�����
	if (FAILED(CObjectDivChara::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	for (int nCntSword = 0; nCntSword < player::NUM_SWORD; nCntSword++)
	{ // ���̐����J��Ԃ�

		// ���̐���
		m_apSowrd[nCntSword] = CSword::Create(nullptr, SWORD_OFFSET[nCntSword]);
		if (m_apSowrd[nCntSword] == nullptr)
		{ // ��g�p���̏ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}
	}

	// �Z�b�g�A�b�v�̓Ǎ�
	LoadSetup(BODY_LOWER, MODEL_PASS[BODY_LOWER]);
	LoadSetup(BODY_UPPER, MODEL_PASS[BODY_UPPER]);

	// ���f�����̐ݒ�
	SetModelInfo();

	// �㔼�g�̐e�C���f�b�N�X�̐ݒ�
	SetUpperParentID(L_MODEL_WAIST);

	for (int nCntSword = 0; nCntSword < player::NUM_SWORD; nCntSword++)
	{ // ���̐����J��Ԃ�

		// �e�I�u�W�F�N�g (������) �̐ݒ�
		m_apSowrd[nCntSword]->SetParentObject(GetMultiModel(BODY_UPPER, U_MODEL_HANDL + nCntSword));
	}

	for (int nCntBlur = 0; nCntBlur < BODY_MAX; nCntBlur++)
	{ // �u���[�̐����J��Ԃ�

		// �u���[�̏��
		D3DXMATERIAL mat = material::GlowCyan();	// �u���[�}�e���A��
		m_apBlur[nCntBlur] = CBlur::Create
		( // ����
			GetObjectChara((CObjectDivChara::EBody)nCntBlur),	// �e�I�u�W�F�N�g
			mat,					// �u���[�}�e���A��
			blurInfo::START_ALPHA,	// �u���[�J�n�����x
			blurInfo::MAX_LENGTH	// �ێ��I�u�W�F�N�g�ő吔
		);
		if (m_apBlur[nCntBlur] == nullptr)
		{ // ��g�p���̏ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}
	}

	// �̗͂̐���
	m_pLife = CGauge2D::Create
	( // ����
		lifeInfo::MAX_LIFE,		// �ő�\���l
		lifeInfo::CHANGE_FRAME,	// �\���l�ϓ��t���[��
		lifeInfo::POS,			// �ʒu
		lifeInfo::SIZE_GAUGE,	// �Q�[�W�傫��
		lifeInfo::COL_FRONT,	// �\�Q�[�W�F
		lifeInfo::COL_BACK,		// ���Q�[�W�F
		true,
		TEXTURE_LIFEFRAME,
		lifeInfo::SIZE_GAUGE + D3DXVECTOR3(16.5f, 16.5f, 0.0f)
	);

	// �e�̐���
	m_pShadow = CShadow::Create(CShadow::TEXTURE_NORMAL, SHADOW_SIZE, this);
	if (m_pShadow == nullptr)
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �����`���OFF�ɂ���
	SetEnableDraw(false);

	if (m_pList == nullptr)
	{ // ���X�g�}�l�[�W���[�����݂��Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̐���
		m_pList = CListManager<CPlayer>::Create();
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
void CPlayer::Uninit(void)
{
	for (int nCntSword = 0; nCntSword < player::NUM_SWORD; nCntSword++)
	{ // ���̐����J��Ԃ�

		// ���̏I��
		SAFE_UNINIT(m_apSowrd[nCntSword]);
	}

	// �e�̏I��
	m_pShadow->DeleteObjectParent();	// �e�I�u�W�F�N�g���폜
	SAFE_UNINIT(m_pShadow);

	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DeleteList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

	// �I�u�W�F�N�g�����L�����N�^�[�̏I��
	CObjectDivChara::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CPlayer::Update(void)
{
	// �ϐ���錾
	ELowerMotion curLowMotion = L_MOTION_IDOL;	// ���݂̉����g���[�V����
	EUpperMotion curUpMotion  = U_MOTION_IDOL;	// ���݂̏㔼�g���[�V����

	// �I�u�W�F�N�g�����L�����N�^�[�̍X�V
	CObjectDivChara::Update();

	// �ߋ��ʒu�̍X�V
	UpdateOldPosition();

	// �d�͂̍X�V
	UpdateGravity();

	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_NONE:
		break;

	case STATE_SPAWN:

		// �X�|�[����Ԏ��̍X�V
		UpdateSpawn();

		break;

	case STATE_NORMAL:

		// �ʏ��Ԃ̍X�V
		UpdateNormal((int*)&curLowMotion, (int*)&curUpMotion);

		break;

	case STATE_DAMAGE:

		// �ʏ��Ԃ̍X�V
		UpdateDamage((int*)&curLowMotion, (int*)&curUpMotion);

		break;

	case STATE_INVULN:

		// ���G��Ԃ̍X�V
		UpdateInvuln((int*)&curLowMotion, (int*)&curUpMotion);

		break;

	case STATE_DEATH:

		// ���S���[�V�����ɂ���
		curLowMotion = L_MOTION_DEATH;
		curUpMotion  = U_MOTION_DEATH;

		break;

	default:
		assert(false);
		break;
	}

	bool bColl[] =	// ����ON/OFF��
	{
		IsLeftWeaponCollision(BODY_UPPER),	// ����̕���̍U������
		IsRightWeaponCollision(BODY_UPPER),	// �E��̕���̍U������
	};
	for (int nCntSword = 0; nCntSword < player::NUM_SWORD; nCntSword++)
	{ // ���̐����J��Ԃ�

		// ���̍U�������ݒ�
		m_apSowrd[nCntSword]->SetEnableAttack(bColl[nCntSword]);

		// ���̍X�V
		m_apSowrd[nCntSword]->Update();
	}

	// �e�̍X�V
	m_pShadow->Update();

	// ���[�V�����̍X�V
	UpdateMotion(curLowMotion, curUpMotion);
}

//============================================================
//	�`�揈��
//============================================================
void CPlayer::Draw(CShader *pShader)
{
	CToonShader	*pToonShader = CToonShader::GetInstance();	// �g�D�[���V�F�[�_�[���
	if (pToonShader->IsEffectOK())
	{ // �G�t�F�N�g���g�p�\�ȏꍇ

		// �I�u�W�F�N�g�����L�����N�^�[�̕`��
		CObjectDivChara::Draw(pToonShader);

		for (int nCntSword = 0; nCntSword < player::NUM_SWORD; nCntSword++)
		{ // ���̐����J��Ԃ�

			// ���̕`��
			m_apSowrd[nCntSword]->Draw(pToonShader);
		}
	}
	else
	{ // �G�t�F�N�g���g�p�s�\�ȏꍇ

		// �G�t�F�N�g�G���[
		assert(false);

		// �I�u�W�F�N�g�����L�����N�^�[�̕`��
		CObjectDivChara::Draw(pShader);

		for (int nCntSword = 0; nCntSword < player::NUM_SWORD; nCntSword++)
		{ // ���̐����J��Ԃ�

			// ���̕`��
			m_apSowrd[nCntSword]->Draw(pShader);
		}
	}
}

//============================================================
//	��Ԃ̐ݒ菈��
//============================================================
void CPlayer::SetState(const int nState)
{
	if (nState > NONE_IDX && nState < STATE_MAX)
	{ // �͈͓��̏ꍇ

		// �����̏�Ԃ�ݒ�
		m_state = (EState)nState;
	}
	else { assert(false); }
}

//============================================================
//	��Ԏ擾����
//============================================================
int CPlayer::GetState(void) const
{
	// ��Ԃ�Ԃ�
	return m_state;
}

//============================================================
//	���a�擾����
//============================================================
float CPlayer::GetRadius(void) const
{
	// ���a��Ԃ�
	return RADIUS;
}

//============================================================
//	�c���擾����
//============================================================
float CPlayer::GetHeight(void) const
{
	// �c����Ԃ�
	return HEIGHT;
}

//============================================================
//	�X�V�󋵂̐ݒ菈��
//============================================================
void CPlayer::SetEnableUpdate(const bool bUpdate)
{
	// �����̍X�V�󋵂�ݒ�
	CObject::SetEnableUpdate(bUpdate);		// ���g
	m_pShadow->SetEnableUpdate(bUpdate);	// �e
}

//============================================================
//	�`��󋵂̐ݒ菈��
//============================================================
void CPlayer::SetEnableDraw(const bool bDraw)
{
	// �����̕`��󋵂�ݒ�
	CObject::SetEnableDraw(bDraw);		// ���g
	m_pShadow->SetEnableDraw(bDraw);	// �e
}

//============================================================
//	�}�g���b�N�X�v�Z���ʂ̎擾����
//============================================================
D3DXMATRIX CPlayer::CalcMtxWorld(void) const
{
	// �ϐ���錾
	D3DXMATRIX  mtxRot, mtxTrans, mtxWorld;		// �v�Z�p�}�g���b�N�X
	D3DXVECTOR3 posPlayer = GetVec3Position();	// �v���C���[�ʒu
	D3DXVECTOR3 rotPlayer = GetVec3Rotation();	// �v���C���[����

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rotPlayer.y, rotPlayer.x, rotPlayer.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, posPlayer.x, posPlayer.y, posPlayer.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X��Ԃ�
	return mtxWorld;
}

//============================================================
//	��������
//============================================================
CPlayer *CPlayer::Create(CScene::EMode mode)
{
	// �|�C���^��錾
	CPlayer *pPlayer = nullptr;	// �v���C���[���

	// �v���C���[�̐���
	switch (mode)
	{ // ���[�h���Ƃ̏���
	case CScene::MODE_TITLE:
	case CScene::MODE_RESULT:
	case CScene::MODE_RANKING:

		break;

	case CScene::MODE_TUTORIAL:
	case CScene::MODE_GAME:

		// �������m��
		pPlayer = new CPlayer;	// �v���C���[

		break;

	default:	// ��O����
		assert(false);
		break;
	}

	if (pPlayer == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �v���C���[�̏�����
		if (FAILED(pPlayer->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �v���C���[�̔j��
			SAFE_DELETE(pPlayer);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pPlayer;
	}
}

//============================================================
//	���X�g�擾����
//============================================================
CListManager<CPlayer> *CPlayer::GetList(void)
{
	// �I�u�W�F�N�g���X�g��Ԃ�
	return m_pList;
}

//============================================================
//	UI�`��̐ݒ菈��
//============================================================
void CPlayer::SetEnableDrawUI(const bool bDraw)
{
	// UI�I�u�W�F�N�g�ɕ`��󋵂𔽉f
	m_pLife->SetEnableDraw(bDraw);	// �̗�
}

//============================================================
//	�q�b�g����
//============================================================
void CPlayer::Hit(const int nDamage)
{
	if (IsDeath())				 { return; }	// ���S�ς�
	if (m_state != STATE_NORMAL) { return; }	// �ʏ��ԈȊO
	if (m_pLife->GetNum() <= 0)	 { return; }	// �̗͂Ȃ�

	// �ϐ���錾
	D3DXVECTOR3 posPlayer = GetVec3Position();	// �v���C���[�ʒu
	D3DXVECTOR3 rotPlayer = GetVec3Rotation();	// �v���C���[����

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

		if (GET_MANAGER->GetMode() == CScene::MODE_GAME)
		{ // �Q�[����ʂ̏ꍇ

			// ���U���g��ʂɑJ�ڂ�����
			CSceneGame::GetGameManager()->TransitionResult(CRetentionManager::WIN_FAILED);
		}
	}
}

//============================================================
//	�m�b�N�o�b�N�q�b�g����
//============================================================
void CPlayer::HitKnockBack(const int /*nDamage*/, const D3DXVECTOR3 & /*vecKnock*/)
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

//============================================================
//	�ʏ��Ԃ̏���������
//============================================================
void CPlayer::InitNormal(void)
{
	// �ϐ���錾
	D3DXVECTOR3 posPlayer = GetVec3Position();	// �v���C���[�ʒu

	// �|�C���^��錾
	CStage *pStage = CScene::GetStage();				// �X�e�[�W���
	if (pStage == nullptr) { assert(false); return; }	// �X�e�[�W��g�p��

	// �ʏ��Ԃɂ���
	SetState(STATE_NORMAL);

	// �J�E���^�[��������
	m_nCounterState = 0;	// ��ԊǗ��J�E���^�[

	// �ړ��ʂ�������
	m_move = VEC3_ZERO;

	// �����`���ON�ɂ���
	SetEnableDraw(true);

	// �����x��s�����ɍĐݒ�
	SetAlpha(1.0f);

	// �}�e���A�����Đݒ�
	ResetMaterial();

	// �ҋ@���[�V������ݒ�
	SetMotion(BODY_LOWER, L_MOTION_IDOL);
	SetMotion(BODY_UPPER, U_MOTION_IDOL);

	// ���n����
	UpdateLanding(&posPlayer);

	// �X�e�[�W�͈͊O�̕␳
	pStage->LimitPosition(posPlayer, RADIUS);

	// �ʒu�𔽉f
	SetVec3Position(posPlayer);

	for (int nCntSword = 0; nCntSword < player::NUM_SWORD; nCntSword++)
	{ // ���̐����J��Ԃ�

		// ���̎����`���OFF�ɂ���
		m_apSowrd[nCntSword]->SetState(CSword::STATE_NONE);
	}
}

//============================================================
//	�X�|�[���̐ݒ菈��
//============================================================
void CPlayer::SetSpawn(void)
{
	// �ϐ���錾
	D3DXVECTOR3 setPos = SPAWN_POS;	// �ʒu�ݒ�p
	D3DXVECTOR3 setRot = SPAWN_ROT;	// �����ݒ�p

	// �X�|�[����Ԃɂ���
	SetState(STATE_SPAWN);

	// �J�E���^�[��������
	m_nCounterState = 0;	// ��ԊǗ��J�E���^�[

	// ���n����
	UpdateLanding(&setPos);

	// �ʒu��ݒ�
	SetVec3Position(setPos);
	m_oldPos = setPos;

	// ������ݒ�
	SetVec3Rotation(setRot);
	m_destRot = setRot;

	// �ړ��ʂ�������
	m_move = VEC3_ZERO;

	// �����`���ON�ɂ���
	SetEnableDraw(true);

	// �}�e���A�����Đݒ�
	ResetMaterial();

	// �o�����[�V������ݒ�
	SetMotion(BODY_LOWER, L_MOTION_SPAWN);
	SetMotion(BODY_UPPER, U_MOTION_SPAWN);

	// �J�������v���C���[���ڏ�Ԃɐݒ�
	GET_MANAGER->GetCamera()->SetState(CCamera::STATE_LOOK_PLAYER);
	GET_MANAGER->GetCamera()->SetDestLookPlayer();	// �J�����ڕW�ʒu�̏�����
}

//============================================================
//	���G�̐ݒ菈��
//============================================================
void CPlayer::SetInvuln(void)
{
	// ���G��Ԃɂ���
	SetState(STATE_INVULN);

	// �J�E���^�[��������
	m_nCounterState = 0;	// ��ԊǗ��J�E���^�[

	// ����������������
	m_fSinAlpha = 0.0f;

	// �`����ĊJ
	SetEnableDraw(true);

	// �}�e���A�����Đݒ�
	ResetMaterial();

	// ���G�̊�b�����x��ݒ�
	SetAlpha(INVULN_ALPHA);
}

//============================================================
//	���[�V�����̐ݒ菈��
//============================================================
void CPlayer::SetMotion
(
	const EBody bodyID,		// �g�̃C���f�b�N�X
	const int nType,		// ���[�V�������
	const int nBlendFrame	// �u�����h�t���[��
)
{
	if (bodyID > NONE_IDX && bodyID < BODY_MAX)
	{ // ���K�C���f�b�N�X�̏ꍇ

		// �����C���f�b�N�X�̃��[�V������ݒ�
		CObjectDivChara::SetMotion(bodyID, nType, nBlendFrame);

		for (int nCntSword = 0; nCntSword < player::NUM_SWORD; nCntSword++)
		{ // ���̐����J��Ԃ�

			// ���̏�Ԃ�ݒ�
			m_apSowrd[nCntSword]->SetState((IsWeaponDisp(BODY_UPPER)) ? CSword::STATE_NORMAL : CSword::STATE_VANISH);
		}
	}
	else { assert(false); }	// �C���f�b�N�X�G���[
}

//============================================================
//	���[�V�����̍X�V����
//============================================================
void CPlayer::UpdateMotion(const int nLowMotion, const int nUpMotion)
{
	if (nLowMotion <= NONE_IDX || nLowMotion >= L_MOTION_MAX) { assert(false); return; }	// �����g�̃��[�V���������ݒ�
	if (nUpMotion  <= NONE_IDX || nUpMotion  >= L_MOTION_MAX) { assert(false); return; }	// �㔼�g�̃��[�V���������ݒ�
	if (IsDeath()) { return; }	// ���S���Ă���

	const int aMotion[] = { nLowMotion, nUpMotion };	// ���[�V�������
	for (int nCntBody = 0; nCntBody < BODY_MAX; nCntBody++)
	{ // ���������g�̂̐����J��Ԃ�

		// �e���g���Ƃ̃��[�V�����X�V
		(this->*(m_aFuncUpdateMotion[nCntBody]))(aMotion[nCntBody]);
	}
}

//============================================================
//	�����g���[�V�����̍X�V����
//============================================================
void CPlayer::UpdateMotionLower(const int nMotion)
{
	if (IsMotionLoop(BODY_LOWER))
	{ // ���[�v���郂�[�V�������̏ꍇ

		if (GetMotionType(BODY_LOWER) != nMotion)
		{ // ���݂̃��[�V�������Đ����̃��[�V�����ƈ�v���Ȃ��ꍇ

			// ���݂̃��[�V�����̐ݒ�
			SetMotion(BODY_LOWER, nMotion);
		}
	}
	else
	{ // ���[�v���Ȃ����[�V�������̏ꍇ

		switch (GetMotionType(BODY_LOWER))
		{ // ���[�V�������Ƃ̏���
		case L_MOTION_LAND:	// ���n���[�V�����F���[�vOFF

			if (nMotion != L_MOTION_IDOL)
			{ // �ҋ@���[�V�����ȊO�̏ꍇ

				// ���݂̃��[�V�����̐ݒ�
				SetMotion(BODY_LOWER, nMotion);
			}

			break;

		// TODO�F�U�����[�V�����ǉ������炱���ɂ��L�q
		case L_MOTION_ATTACK_00:		// �U�����[�V������i�K�ځF���[�vOFF
		case L_MOTION_ATTACK_01:		// �U�����[�V������i�K�ځF���[�vOFF
		case L_MOTION_ATTACK_02:		// �U�����[�V�����O�i�K�ځF���[�vOFF
		case L_MOTION_JUMP_ATTACK_00:	// �󒆍U�����[�V������i�K�ځF���[�vOFF
		case L_MOTION_JUMP_ATTACK_01:	// �󒆍U�����[�V������i�K�ځF���[�vOFF

			if (IsMotionCancel(BODY_LOWER))
			{ // �L�����Z���ł���ꍇ

				if (nMotion != L_MOTION_IDOL)
				{ // �ҋ@���[�V�����ȊO�̏ꍇ

					// ���݂̃��[�V�����̐ݒ�
					SetMotion(BODY_LOWER, nMotion);
				}
			}

			break;
		}
	}

	switch (GetMotionType(BODY_LOWER))
	{ // ���[�V�������Ƃ̏���
	case L_MOTION_SPAWN:	// �o�ꃂ�[�V�����F���[�vOFF
	case L_MOTION_IDOL:		// �ҋ@���[�V�����F���[�vON
		break;

	case L_MOTION_MOVE:	// �ړ����[�V�����F���[�vON

		if (GetMotionKey(BODY_LOWER) % WALK_SOUND == 0 && GetMotionKeyCounter(BODY_LOWER) == 0)
		{ // ���������^�C�~���O�̏ꍇ

			// ���s���̍Đ�
			PLAY_SOUND(CSound::LABEL_SE_WALK_BUILD);
		}

		break;
	
	case L_MOTION_ATTACK_00:	// �U�����[�V������i�K�ځF���[�vOFF
	case L_MOTION_ATTACK_01:	// �U�����[�V������i�K�ځF���[�vOFF

		if (IsMotionCombo(BODY_LOWER))
		{ // ���[�V�����R���{���ł���ꍇ

			if (m_attack.bInput)
			{ // �U������s���͂���Ă���ꍇ

				// ���̍U�����[�V������ݒ�
				SetMotion(BODY_LOWER, GetMotionType(BODY_LOWER) + 1);

				// ���͔��f�����Z
				if (m_attack.Add())	// �S���[�V�����ɔ��f�������s���͂�������
				{ // �S���[�V�����ɔ��f�ς݂̏ꍇ

					// L�X�e�B�b�N�̌����Ƀv���C���[����������
					SetLStickRotation();
				}

				// �����𔲂���
				break;
			}
		}

		if (IsMotionFinish(BODY_LOWER))
		{ // ���[�V�������I�������ꍇ

			// ���݂̃��[�V�����̐ݒ�
			SetMotion(BODY_LOWER, nMotion);
		}

		break;

	case L_MOTION_ATTACK_02:	// �U�����[�V�����O�i�K�ځF���[�vOFF

		if (IsMotionFinish(BODY_LOWER))
		{ // ���[�V�������I�����Ă����ꍇ

			// ���݂̃��[�V�����̐ݒ�
			SetMotion(BODY_LOWER, nMotion);
		}

		break;
	
	case L_MOTION_JUMP:	// �W�����v���[�V�����F���[�vOFF
		break;

	case L_MOTION_LAND:	// �W�����v���[�V�����F���[�vOFF

		if (IsMotionFinish(BODY_LOWER))
		{ // ���[�V�������I�����Ă����ꍇ

			// ���݂̃��[�V�����̐ݒ�
			SetMotion(BODY_LOWER, nMotion);
		}

		break;

	case L_MOTION_JUMP_ATTACK_00:	// �󒆍U�����[�V������i�K�ځF���[�vOFF

		if (IsMotionCombo(BODY_LOWER))
		{ // ���[�V�����R���{���ł���ꍇ

			if (m_attack.bInput)
			{ // �U������s���͂���Ă���ꍇ

				// ���̍U�����[�V������ݒ�
				SetMotion(BODY_LOWER, GetMotionType(BODY_LOWER) + 1);

				// ���͔��f�����Z
				if (m_attack.Add())	// �S���[�V�����ɔ��f�������s���͂�������
				{ // �S���[�V�����ɔ��f�ς݂̏ꍇ

					// L�X�e�B�b�N�̌����Ƀv���C���[����������
					SetLStickRotation();
				}

				// �����𔲂���
				break;
			}
		}

		if (IsMotionFinish(BODY_LOWER))
		{ // ���[�V�������I�������ꍇ

			// ���݂̃��[�V�����̐ݒ�
			SetMotion(BODY_LOWER, nMotion);
		}

		break;

	case L_MOTION_JUMP_ATTACK_01:	// �󒆍U�����[�V������i�K�ځF���[�vOFF

		if (IsMotionFinish(BODY_LOWER))
		{ // ���[�V�������I�����Ă����ꍇ

			// ���݂̃��[�V�����̐ݒ�
			SetMotion(BODY_LOWER, nMotion);
		}

		break;

	case L_MOTION_DEATH:	// ���S���[�V�����F���[�vON
		break;

	default:	// ��O����
		assert(false);
		break;
	}
}

//============================================================
//	�㔼�g���[�V�����̍X�V����
//============================================================
void CPlayer::UpdateMotionUpper(const int nMotion)
{
	if (IsMotionLoop(BODY_UPPER))
	{ // ���[�v���郂�[�V�������̏ꍇ

		if (GetMotionType(BODY_UPPER) != nMotion)
		{ // ���݂̃��[�V�������Đ����̃��[�V�����ƈ�v���Ȃ��ꍇ

			// ���݂̃��[�V�����̐ݒ�
			SetMotion(BODY_UPPER, nMotion);
		}
	}
	else
	{ // ���[�v���Ȃ����[�V�������̏ꍇ

		switch (GetMotionType(BODY_UPPER))
		{ // ���[�V�������Ƃ̏���
		case U_MOTION_LAND:	// ���n���[�V�����F���[�vOFF

			if (nMotion != U_MOTION_IDOL)
			{ // �ҋ@���[�V�����ȊO�̏ꍇ

				// ���݂̃��[�V�����̐ݒ�
				SetMotion(BODY_UPPER, nMotion);
			}

			break;

		// TODO�F�U�����[�V�����ǉ������炱���ɂ��L�q
		case U_MOTION_ATTACK_00:		// �U�����[�V������i�K�ځF���[�vOFF
		case U_MOTION_ATTACK_01:		// �U�����[�V������i�K�ځF���[�vOFF
		case U_MOTION_ATTACK_02:		// �U�����[�V�����O�i�K�ځF���[�vOFF
		case U_MOTION_JUMP_ATTACK_00:	// �󒆍U�����[�V������i�K�ځF���[�vOFF
		case U_MOTION_JUMP_ATTACK_01:	// �󒆍U�����[�V������i�K�ځF���[�vOFF

			if (IsMotionCancel(BODY_UPPER))
			{ // �L�����Z���ł���ꍇ

				if (nMotion != U_MOTION_IDOL)
				{ // �ҋ@���[�V�����ȊO�̏ꍇ

					// ���݂̃��[�V�����̐ݒ�
					SetMotion(BODY_UPPER, nMotion);
				}
			}

			break;
		}
	}

	switch (GetMotionType(BODY_UPPER))
	{ // ���[�V�������Ƃ̏���
	case U_MOTION_SPAWN:	// �o�ꃂ�[�V�����F���[�vOFF
	case U_MOTION_IDOL:		// �ҋ@���[�V�����F���[�vON
	case U_MOTION_MOVE:		// �ړ����[�V�����F���[�vON
		break;
	
	case U_MOTION_ATTACK_00:	// �U�����[�V������i�K�ځF���[�vOFF
	case U_MOTION_ATTACK_01:	// �U�����[�V������i�K�ځF���[�vOFF

		if (IsMotionCombo(BODY_UPPER))
		{ // ���[�V�����R���{���ł���ꍇ

			if (m_attack.bInput)
			{ // �U������s���͂���Ă���ꍇ

				// ���݂̃��[�V�����̐ݒ�
				SetMotion(BODY_UPPER, GetMotionType(BODY_UPPER) + 1);

				// ���͔��f�����Z
				if (m_attack.Add())	// �S���[�V�����ɔ��f�������s���͂�������
				{ // �S���[�V�����ɔ��f�ς݂̏ꍇ

					// L�X�e�B�b�N�̌����Ƀv���C���[����������
					SetLStickRotation();
				}

				// �����𔲂���
				break;
			}
		}

		if (IsMotionFinish(BODY_UPPER))
		{ // ���[�V�������I�������ꍇ

			// ���݂̃��[�V�����̐ݒ�
			SetMotion(BODY_UPPER, nMotion);
		}
	
		break;

	case U_MOTION_ATTACK_02:	// �U�����[�V�����O�i�K�ځF���[�vOFF

		if (IsMotionFinish(BODY_UPPER))
		{ // ���[�V�������I�����Ă����ꍇ

			// ���݂̃��[�V�����̐ݒ�
			SetMotion(BODY_UPPER, nMotion);
		}

		break;
	
	case U_MOTION_JUMP:	// �W�����v���[�V�����F���[�vOFF
		break;

	case U_MOTION_LAND:	// �W�����v���[�V�����F���[�vOFF

		if (IsMotionFinish(BODY_UPPER))
		{ // ���[�V�������I�����Ă����ꍇ

			// ���݂̃��[�V�����̐ݒ�
			SetMotion(BODY_UPPER, nMotion);
		}

		break;

	case U_MOTION_JUMP_ATTACK_00:	// �󒆍U�����[�V������i�K�ځF���[�vOFF

		if (IsMotionCombo(BODY_UPPER))
		{ // ���[�V�����R���{���ł���ꍇ

			if (m_attack.bInput)
			{ // �U������s���͂���Ă���ꍇ

				// ���݂̃��[�V�����̐ݒ�
				SetMotion(BODY_UPPER, GetMotionType(BODY_UPPER) + 1);

				// ���͔��f�����Z
				if (m_attack.Add())	// �S���[�V�����ɔ��f�������s���͂�������
				{ // �S���[�V�����ɔ��f�ς݂̏ꍇ

					// L�X�e�B�b�N�̌����Ƀv���C���[����������
					SetLStickRotation();
				}

				if (m_move.y < 0.0f)
				{ // �c�ړ��ʂ��}�C�i�X�̏ꍇ

					// ��ړ��ʂ��v���X�ɂ���
					m_move.y = 10.0f;
				}
				else
				{ // �c�ړ��ʂ��v���X�̏ꍇ

					// ��ړ��ʂ�������
					m_move.y += 4.0f;
				}

				// �����𔲂���
				break;
			}
		}

		if (IsMotionFinish(BODY_UPPER))
		{ // ���[�V�������I�������ꍇ

			// ���݂̃��[�V�����̐ݒ�
			SetMotion(BODY_UPPER, nMotion);
		}

		break;

	case U_MOTION_JUMP_ATTACK_01:	// �󒆍U�����[�V������i�K�ځF���[�vOFF

		if (IsMotionFinish(BODY_UPPER))
		{ // ���[�V�������I�����Ă����ꍇ

			// ���݂̃��[�V�����̐ݒ�
			SetMotion(BODY_UPPER, nMotion);
		}

		break;

	case U_MOTION_DEATH:	// ���S���[�V�����F���[�vON
		break;

	default:	// ��O����
		assert(false);
		break;
	}
}

//============================================================
//	�ڕW������G�ɂ��鏈��
//============================================================
void CPlayer::SetDestLookEnemy
(
	const D3DXVECTOR3& rPosPlayer,	// �v���C���[�ʒu
	const D3DXVECTOR3& rRotPlayer,	// �v���C���[����
	const float fRate				// �����␳�W��
)
{
	CEnemy *pEnemy = nullptr;	// ������␳����G

	// �����̕␳������G���w��
	{
		CListManager<CEnemy> *pList = CEnemy::GetList();	// ���X�g�}�l�[�W���[
		if (pList == nullptr)		 { return; }	// ���X�g���g�p
		if (pList->GetNumAll() <= 0) { return; }	// �G�����݂��Ȃ�

		float fMinLength = 0.0f;	// ��ԃv���C���[�ɋ߂�����
		std::list<CEnemy*> list = pList->GetList();	// �G���X�g
		for (auto enemy : list)
		{ // ���X�g�̂��ׂĂ��J��Ԃ�

			// XZ���ʂ̉~�̓����蔻��
			float fLength = 0.0f;	// �v���C���[����̋���
			bool bHit = collision::Circle2D
			( // ����
				rPosPlayer,					// ����ʒu
				enemy->GetVec3Position(),	// ����ڕW�ʒu
				LOOK_RADIUS,	// ���蔼�a
				0.0f,			// ����ڕW���a
				&fLength		// ����ڕW�Ƃ̋���
			);
			if (bHit)
			{ // ������̏ꍇ

				if (pEnemy == nullptr)
				{ // �߂��G���ݒ肳��Ă��Ȃ��ꍇ

					// ��������ۑ�
					pEnemy = enemy;			// ��ԃv���C���[�ɋ߂��G
					fMinLength = fLength;	// ��ԃv���C���[�ɋ߂�����
				}
				else
				{ // �߂��G���ݒ肳��Ă���ꍇ

					if (fLength < fMinLength)
					{ // �ݒ蒆�̓G��肳��ɋ߂��ꍇ

						// ����ۑ�
						pEnemy = enemy;			// ��ԃv���C���[�ɋ߂��G
						fMinLength = fLength;	// ��ԃv���C���[�ɋ߂�����
					}
				}
			}
		}
	}

	// �G�̕�������������
	{
		if (pEnemy == nullptr) { return; }	// �G�w��Ȃ��ꍇ������

		D3DXVECTOR3 posEnemy = pEnemy->GetVec3Position();	// �G�ʒu
		float fDestRotY, fDiffRotY;	// �ڕW�E��������

		// �ڕW������ݒ�
		fDestRotY = atan2f(rPosPlayer.x - posEnemy.x, rPosPlayer.z - posEnemy.z);
		useful::NormalizeRot(fDestRotY);	// �������K��

		// ����������ݒ�
		fDiffRotY = fDestRotY - rRotPlayer.y;
		useful::NormalizeRot(fDiffRotY);	// �������K��

		// ���ۂ̖ڕW������ݒ�
		m_destRot.y += fDiffRotY * fRate;
	}
}

//============================================================
//	L�X�e�B�b�N�̌����Ƀv���C���[���������鏈��
//============================================================
void CPlayer::SetLStickRotation(void)
{
	CCamera *pCamera = GET_MANAGER->GetCamera();	// �J����
	CInputPad *pPad = GET_INPUTPAD;	// �p�b�h
	D3DXVECTOR3 vec = VEC3_ZERO;	// �x�N�g��

	float fLTilt = pPad->GetPressLStickTilt();	// �X�e�B�b�N�̌X����
	if (pad::DEAD_ZONE < fLTilt)
	{ // �f�b�h�]�[���ȏ�̏ꍇ

		// �X�e�B�b�N�X�������̃x�N�g�����쐬
		vec.x = sinf(pPad->GetPressLStickRot() + pCamera->GetVec3Rotation().y + HALF_PI);
		vec.z = cosf(pPad->GetPressLStickRot() + pCamera->GetVec3Rotation().y + HALF_PI);

		// �X�e�B�b�N�X�������������ɐݒ�
		m_destRot.y = atan2f(-vec.x, -vec.z);	// �ڕW����
		SetVec3Rotation(m_destRot);				// ����
	}
}

//============================================================
//	�U���󋵂̎擾����
//============================================================
bool CPlayer::IsAttack(void) const
{
	// �ϐ���錾
	EUpperMotion curMotion = (EUpperMotion)GetMotionType(BODY_UPPER);	// ���݂̃��[�V����

	// TODO�F�U���ǉ�������L�q

	// �U���󋵂�ݒ�
	bool bAttack = (curMotion == U_MOTION_ATTACK_00
				 || curMotion == U_MOTION_ATTACK_01
				 || curMotion == U_MOTION_ATTACK_02
				 || curMotion == U_MOTION_JUMP_ATTACK_00
				 || curMotion == U_MOTION_JUMP_ATTACK_01);

	// �U���󋵂�Ԃ�
	return bAttack;
}

//============================================================
//	�X�|�[����Ԏ��̍X�V����
//============================================================
void CPlayer::UpdateSpawn(void)
{
	// �ʃ��[�V�����̎w�肪����Ă���ꍇ�G���[
	assert(GetMotionType(BODY_LOWER) == L_MOTION_SPAWN);

	if (IsMotionCancel(BODY_LOWER))
	{ // ���[�V�������L�����Z���ł���ꍇ

		// �������Ȃ���Ԃɂ���
		SetState(STATE_NONE);
	}
}

//============================================================
//	�ʏ��Ԏ��̍X�V����
//============================================================
void CPlayer::UpdateNormal(int *pLowMotion, int *pUpMotion)
{
	// �ϐ���錾
	D3DXVECTOR3 posPlayer = GetVec3Position();	// �v���C���[�ʒu
	D3DXVECTOR3 rotPlayer = GetVec3Rotation();	// �v���C���[����

	// �|�C���^��錾
	CStage *pStage = CScene::GetStage();				// �X�e�[�W���
	if (pStage == nullptr) { assert(false); return; }	// �X�e�[�W��g�p��

	// �U������
	UpdateAttack(posPlayer, rotPlayer);

	// ��𑀍�
	UpdateDodge();

	// �ړ�����E�ڕW�����ݒ�
	UpdateMove(pLowMotion, pUpMotion);

	// �W�����v����
	UpdateJump(pLowMotion, pUpMotion);

	// �ʒu�X�V
	UpdatePosition(&posPlayer);

	// �G�Ƃ̓����蔻��̍X�V
	UpdateCollEnemy(&posPlayer);

	// ���n����
	UpdateLanding(&posPlayer);

	// �����X�V
	UpdateRotation(&rotPlayer);

	// �X�e�[�W�͈͊O�̕␳
	pStage->LimitPosition(posPlayer, RADIUS);

	// �ʒu�𔽉f
	SetVec3Position(posPlayer);

	// �����𔽉f
	SetVec3Rotation(rotPlayer);
}

//============================================================
//	�_���[�W��Ԏ��̍X�V����
//============================================================
void CPlayer::UpdateDamage(int *pLowMotion, int *pUpMotion)
{
	// �g�̂̐F��Ԃ�����
	SetAllMaterial(material::DamageRed());

	// �ʏ��Ԃ̍X�V
	UpdateNormal(pLowMotion, pUpMotion);

	// �J�E���^�[�����Z
	m_nCounterState++;
	if (m_nCounterState > DAMAGE_FRAME)
	{ // �_���[�W��Ԃ̏I���t���[���ɂȂ����ꍇ

		// �J�E���^�[��������
		m_nCounterState = 0;

		// �g�̂̐F�����ɖ߂�
		ResetMaterial();

		// ���G��Ԃɂ���
		SetInvuln();
	}
}

//============================================================
//	���G��Ԏ��̍X�V����
//============================================================
void CPlayer::UpdateInvuln(int *pLowMotion, int *pUpMotion)
{
	// �ϐ���錾
	float fAddAlpha = 0.0f;	// �����x�̉��Z��

	// �T�C���J�[�u��������]
	m_fSinAlpha += ADD_SINROT;
	useful::NormalizeRot(m_fSinAlpha);	// �������K��

	// �����x���Z�ʂ����߂�
	fAddAlpha = (MAX_ADD_ALPHA / 2.0f) * (sinf(m_fSinAlpha) - 1.0f);

	// �����x��ݒ�
	SetAlpha(INVULN_ALPHA + fAddAlpha);

	// �ʏ��Ԃ̍X�V
	UpdateNormal(pLowMotion, pUpMotion);

	// �J�E���^�[�����Z
	m_nCounterState++;
	if (m_nCounterState > INVULN_FRAME)
	{ // ���G��Ԃ̏I���t���[���ɂȂ����ꍇ

		// �J�E���^�[��������
		m_nCounterState = 0;

		// �s�����ɂ���
		SetAlpha(1.0f);

		// �ʏ��Ԃ�ݒ�
		SetState(STATE_NORMAL);
	}
}

//============================================================
//	�U������̍X�V����
//============================================================
void CPlayer::UpdateAttack
(
	const D3DXVECTOR3& rPos,	// �v���C���[�ʒu
	const D3DXVECTOR3& rRot		// �v���C���[����
)
{
	if (m_dodge.bDodge)	{ return; }	// ��𒆂̏ꍇ������

	if (GET_INPUTPAD->IsTrigger(CInputPad::KEY_X))
	{
		if (!m_jump.bJump)
		{ // �W�����v���Ă��Ȃ��ꍇ

			// �n��U������̍X�V
			UpdateLandAttack();
		}
		else
		{ // �W�����v���Ă���ꍇ

			// �󒆍U������̍X�V
			UpdateSkyAttack();
		}

		// �ڕW������G�����ɂ���
		SetDestLookEnemy
		( // ����
			rPos,		// �v���C���[�ʒu
			rRot,		// �v���C���[����
			LOOK_REV	// �����␳�W��
		);
	}
}

//============================================================
//	�n��U������̍X�V����
//============================================================
void CPlayer::UpdateLandAttack(void)
{
	if (!IsAttack())
	{ // �U�����ł͂Ȃ��ꍇ

		// �U�����[�V�������w��
		SetMotion(BODY_LOWER, L_MOTION_ATTACK_00);
		SetMotion(BODY_UPPER, U_MOTION_ATTACK_00);
	}
	else
	{ // �U�����̏ꍇ

		if (GetMotionType(BODY_UPPER) != U_MOTION_ATTACK_02)	// TODO�F��ԍŌ�̍U���ɂ���
		{ // �ŏI�U�����[�V�����ł͂Ȃ��ꍇ

			// �R���{�\�܂ł̎c��t���[�����v�Z
			int nWholeFrame = GetMotionComboFrame(BODY_UPPER) - GetMotionWholeCounter(BODY_UPPER);
			if (nWholeFrame < ATTACK_BUFFER_FRAME)
			{ // ��s���͂��\�ȏꍇ

				// ��s���͂��󂯕t����
				m_attack.bInput = true;
			}
		}
	}
}

//============================================================
//	�󒆍U������̍X�V����
//============================================================
void CPlayer::UpdateSkyAttack(void)
{
	if (!IsAttack())
	{ // �U�����ł͂Ȃ��ꍇ

		// �U�����[�V�������w��
		SetMotion(BODY_LOWER, L_MOTION_JUMP_ATTACK_00);
		SetMotion(BODY_UPPER, U_MOTION_JUMP_ATTACK_00);

		if (m_move.y < 0.0f)
		{ // �c�ړ��ʂ��}�C�i�X�̏ꍇ

			// ��ړ��ʂ��v���X�ɂ���
			m_move.y = 10.0f;
		}
		else
		{ // �c�ړ��ʂ��v���X�̏ꍇ

			// ��ړ��ʂ�������
			m_move.y += 4.0f;
		}
	}
	else
	{ // �U�����̏ꍇ

		if (GetMotionType(BODY_UPPER) != U_MOTION_JUMP_ATTACK_01)	// TODO�F��ԍŌ�̍U���ɂ���
		{ // �ŏI�U�����[�V�����ł͂Ȃ��ꍇ

			// �R���{�\�܂ł̎c��t���[�����v�Z
			int nWholeFrame = GetMotionComboFrame(BODY_UPPER) - GetMotionWholeCounter(BODY_UPPER);
			if (nWholeFrame < ATTACK_BUFFER_FRAME)
			{ // ��s���͂��\�ȏꍇ

				// ��s���͂��󂯕t����
				m_attack.bInput = true;
			}
		}
	}
}

//============================================================
//	��𑀍�̍X�V����
//============================================================
void CPlayer::UpdateDodge(void)
{
	CInputPad *pPad  = GET_INPUTPAD;				// �p�b�h
	CCamera *pCamera = GET_MANAGER->GetCamera();	// �J����

	if (m_dodge.bDodge)
	{ // ��𒆂̏ꍇ

		// �������Ɉړ��ʂ�^����
		m_move.x += sinf(m_dodge.fRot) * m_dodge.fMove;
		m_move.z += cosf(m_dodge.fRot) * m_dodge.fMove;

		// �ړ��ʂ����Z
		m_dodge.fMove += (0.0f - m_dodge.fMove) * DODGE_REV_MOVE;
		if (m_dodge.fMove <= DODGE_MIN_MOVE)
		{ // �ړ��ʂ�������؂����ꍇ

			// ����t���O��OFF�ɂ���
			m_dodge.bDodge = false;
		}
	}
	else
	{ // ��𒆂ł͂Ȃ��ꍇ

		if (IsAttack() && !IsMotionCancel(BODY_LOWER)) { return; }	// �U���������[�V�������L�����Z���ł��Ȃ��ꍇ������
		if (m_jump.bJump) { return; }	// �W�����v���̏ꍇ������

		if (m_dodge.nWaitCounter > 0)
		{ // �N�[���^�C�����c���Ă���ꍇ

			// �N�[���^�C�������Z
			m_dodge.nWaitCounter--;
		}

		if (pPad->IsTrigger(CInputPad::KEY_B))
		{
			if (m_dodge.nWaitCounter <= 0)
			{ // �N�[���^�C�����Ȃ��ꍇ

				if (pad::DEAD_ZONE < pPad->GetPressLStickTilt())
				{ // �X�e�B�b�N�̌X���ʂ��f�b�h�]�[���ȏ�̏ꍇ

					// ����ړ��ʂ�ݒ�
					m_dodge.fMove = DODGE_SIDE_MOVE;

					// ��������ݒ�
					m_dodge.fRot = pPad->GetPressLStickRot() + pCamera->GetVec3Rotation().y + HALF_PI;

					// �X�e�B�b�N���͕����Ɉړ��ʂ�^����
					m_move.x += sinf(m_dodge.fRot) * m_dodge.fMove;
					m_move.z += cosf(m_dodge.fRot) * m_dodge.fMove;

					// ��ړ��ʂ�^����
					m_move.y += DODGE_JUMP_MOVE;

					// �ڕW������ݒ�
					m_destRot.y = atan2f(-m_move.x, -m_move.z);

					// �W�����v���ɂ���
					m_jump.bJump = true;

					// ��𒆂ɂ���
					m_dodge.bDodge = true;

					// �N�[���^�C����ݒ�
					m_dodge.nWaitCounter = DODGE_WAIT_FRAME;
				}
			}
		}
	}
}

//============================================================
//	�ړ�����E�ڕW�����̍X�V����
//============================================================
void CPlayer::UpdateMove(int *pLowMotion, int *pUpMotion)
{
	if (IsAttack() && !IsMotionCancel(BODY_LOWER)) { return; }	// �U���������[�V�������L�����Z���ł��Ȃ��ꍇ������
	if (m_dodge.bDodge)	{ return; }	// ��𒆂̏ꍇ������

	CInputPad *pPad  = GET_INPUTPAD;				// �p�b�h
	CCamera *pCamera = GET_MANAGER->GetCamera();	// �J����

	float fLTilt = pPad->GetPressLStickTilt();	// �X�e�B�b�N�̌X����
	if (pad::DEAD_ZONE < fLTilt)
	{ // �f�b�h�]�[���ȏ�̏ꍇ

		// �ϐ���錾
		float fMove = fLTilt * STICK_REV;	// �v���C���[�ړ���

		// �ړ��ʂ��X�V
		m_move.x += sinf(pPad->GetPressLStickRot() + pCamera->GetVec3Rotation().y + HALF_PI) * fMove * MOVE_REV;
		m_move.z += cosf(pPad->GetPressLStickRot() + pCamera->GetVec3Rotation().y + HALF_PI) * fMove * MOVE_REV;

		// �ڕW������ݒ�
		m_destRot.y = atan2f(-m_move.x, -m_move.z);

		// �㉺�Ɉړ����[�V������ݒ�
		*pLowMotion = L_MOTION_MOVE;
		*pUpMotion  = U_MOTION_MOVE;
	}
}

//============================================================
//	�W�����v����̍X�V����
//============================================================
void CPlayer::UpdateJump(int *pLowMotion, int *pUpMotion)
{
	if (m_dodge.bDodge)	{ return; }	// ��𒆂̏ꍇ������
	if (IsAttack() && !IsMotionCancel(BODY_LOWER)) { return; }	// �U���������[�V�������L�����Z���ł��Ȃ��ꍇ������

	if (GET_INPUTPAD->IsTrigger(CInputPad::KEY_A))
	{
		if (!m_jump.bJump)
		{ // �W�����v���Ă��Ȃ��ꍇ

			// ��ړ��ʂ�^����
			m_move.y += INIT_JUMP;

			// �W�����v���ɂ���
			m_jump.bJump = true;

			// �v���X���͒��ɂ���
			m_jump.bInputPress = true;

			// �㉺�ɃW�����v���[�V������ݒ�
			*pLowMotion = L_MOTION_JUMP;
			*pUpMotion  = U_MOTION_JUMP;
		}
	}

	if (m_jump.bJump && m_jump.bInputPress)
	{ // �v���X���͂���������Ă��Ȃ��ꍇ

		if (GET_INPUTPAD->IsPress(CInputPad::KEY_A))
		{ // �v���X���̏ꍇ

			if (m_jump.nPressCounter < PRESS_JUMP_FRAME)
			{ // ���͉\���ԓ��̏ꍇ

				// �W�����v���x�㏸�ʂ̕␳�W��
				float fRate = easeing::InQuint((PRESS_JUMP_FRAME - m_jump.nPressCounter), 0, PRESS_JUMP_FRAME);

				// ��ړ��ʂ�^����
				m_move.y += GRAVITY + PULS_JUMP * fRate;

				// ���͎��ԃJ�E���^�[�����Z
				m_jump.nPressCounter++;
			}
		}
		else
		{ // ���͂��Ȃ��Ȃ����ꍇ

			// ���͎��ԃJ�E���^�[��������
			m_jump.nPressCounter = 0;

			// �v���X���͂�����
			m_jump.bInputPress = false;
		}
	}
}

//============================================================
//	�ߋ��ʒu�̍X�V����
//============================================================
void CPlayer::UpdateOldPosition(void)
{
	// �ߋ��ʒu���X�V
	m_oldPos = GetVec3Position();
}

//============================================================
//	�d�͂̍X�V����
//============================================================
void CPlayer::UpdateGravity(void)
{
	// �d�͂����Z
	m_move.y -= GRAVITY;
}

//============================================================
//	���n�󋵂̍X�V����
//============================================================
void CPlayer::UpdateLanding(D3DXVECTOR3 *pPos)
{
	// �W�����v���Ă����Ԃɂ���
	m_jump.bJump = true;

	// �n�ʁE�����ʒu�̒��n����
	if (CScene::GetStage()->LandFieldPosition(*pPos, m_move)
	||  CScene::GetStage()->LandLimitPosition(*pPos, m_move, 0.0f))
	{ // �v���C���[�����n���Ă����ꍇ

		// �W�����v�t���O��OFF�ɂ���
		m_jump.bJump = false;

		// ����t���O��OFF�ɂ���
		m_dodge.bDodge = false;
	}

	if (!m_jump.bJump)
	{ // �󒆂ɂ��Ȃ��ꍇ

		if (GetMotionType(BODY_LOWER) == L_MOTION_JUMP
		&&  GetMotionType(BODY_UPPER) == U_MOTION_JUMP)
		{ // �㉺���[�V�������W�����v���̏ꍇ

			// ���n���[�V�������w��
			SetMotion(BODY_LOWER, L_MOTION_LAND);
			SetMotion(BODY_UPPER, U_MOTION_LAND);
		}
	}
}

//============================================================
//	�G�Ƃ̓����蔻��̍X�V����
//============================================================
void CPlayer::UpdateCollEnemy(D3DXVECTOR3 *pPos)
{
	CListManager<CEnemy> *pList = CEnemy::GetList();	// ���X�g�}�l�[�W���[
	if (pList == nullptr)		 { return; }	// ���X�g���g�p
	if (pList->GetNumAll() <= 0) { return; }	// �G�����݂��Ȃ�

	D3DXVECTOR3 heightPlayer = D3DXVECTOR3(0.0f, HEIGHT, 0.0f);	// �v���C���[�̏c��
	D3DXVECTOR3 posPlayerCent = *pPos + heightPlayer * 0.5f;	// �v���C���[�̒��S�ʒu
	std::list<CEnemy*> list = pList->GetList();	// �G���X�g
	for (auto enemy : list)
	{ // ���X�g�̂��ׂĂ��J��Ԃ�

		float fRadiusEnemy = enemy->GetStatusInfo().fCollRadius;	// �G�̔��a
		int   nHeadID = enemy->GetHeadModelID();					// �G�̓����f���C���f�b�N�X
		int   nWaistID = enemy->GetWaistModelID();					// �G�̍����f���C���f�b�N�X
		D3DXMATRIX  mtxEnemyHead = enemy->GetMultiModel(nHeadID)->GetMtxWorld();	// �G�̓��̃��[���h�}�g���b�N�X
		D3DXVECTOR3 posEnemyHead = useful::GetMatrixPosition(mtxEnemyHead);			// �G�̓��̈ʒu
		D3DXMATRIX  mtxEnemyWaist = enemy->GetMultiModel(nWaistID)->GetMtxWorld();	// �G�̍��̃��[���h�}�g���b�N�X
		D3DXVECTOR3 posEnemyWaist = useful::GetMatrixPosition(mtxEnemyWaist);		// �G�̍��̈ʒu
		D3DXVECTOR3 posEnemyOrigin = enemy->GetVec3Position();						// �G�̌��_�ʒu

		float fHeightEnemy = posEnemyHead.y - posEnemyOrigin.y;	// �G�̏c��
		D3DXVECTOR3 posEnemyCent = D3DXVECTOR3	// �G�̒��S�ʒu
		(
			posEnemyWaist.x,
			posEnemyOrigin.y + fHeightEnemy * 0.5f,
			posEnemyWaist.z
		);

		// �G�Ƃ̉����߂�����
		collision::ResponseCapsule3D
		( // ����
			&posPlayerCent,	// ����ʒu
			posEnemyCent,	// ����ڕW�ʒu
			RADIUS,			// ���蔼�a
			fRadiusEnemy,	// ����ڕW���a
			fHeightEnemy	// ����ڕW�c��
		);

		// �v���C���[�̒��S�ʒu����c�������Z
		*pPos = posPlayerCent - heightPlayer * 0.5f;
	}
}

//============================================================
//	�ʒu�̍X�V����
//============================================================
void CPlayer::UpdatePosition(D3DXVECTOR3 *pPos)
{
	// �ʒu���ړ�
	*pPos += m_move;

	// �ړ��ʂ�����
	if (m_dodge.bDodge)
	{ // ��𒆂̏ꍇ

		m_move.x += (0.0f - m_move.x) * DODGE_REV;
		m_move.z += (0.0f - m_move.z) * DODGE_REV;
	}
	else
	{ // ��𒆂ł͂Ȃ��ꍇ

		if (m_jump.bJump)
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
}

//============================================================
//	�����̍X�V����
//============================================================
void CPlayer::UpdateRotation(D3DXVECTOR3 *pRot)
{
	// �ϐ���錾
	float fDiffRot = 0.0f;	// ��������

	// �ڕW�����̐��K��
	useful::NormalizeRot(m_destRot.y);

	// �ڕW�����܂ł̍������v�Z
	fDiffRot = m_destRot.y - pRot->y;

	// ���������̐��K��
	useful::NormalizeRot(fDiffRot);

	// �����̍X�V
	pRot->y += fDiffRot * REV_ROTA;

	// �����̐��K��
	useful::NormalizeRot(pRot->y);
}

//============================================================
//	�t�F�[�h�A�E�g��Ԏ��̍X�V����
//============================================================
bool CPlayer::UpdateFadeOut(const float fAdd)
{
	// �ϐ���錾
	bool bEndAlpha = false;		// ������
	float fAlpha = GetAlpha();	// �����x

	// �v���C���[���g�̕`����ĊJ
	CObject::SetEnableDraw(true);

	// �����x���グ��
	fAlpha += fAdd;

	float fMaxAlpha = GetMaxAlpha();
	if (fAlpha >= fMaxAlpha)
	{ // �����x���オ��؂����ꍇ

		// �����x��␳
		fAlpha = fMaxAlpha;

		// �s�����ɂȂ�؂�����Ԃɂ���
		bEndAlpha = true;
	}

	// �����x��ݒ�
	SetAlpha(fAlpha);

	// �����󋵂�Ԃ�
	return bEndAlpha;
}

//============================================================
//	�t�F�[�h�C����Ԏ��̍X�V����
//============================================================
bool CPlayer::UpdateFadeIn(const float fSub)
{
	// �ϐ���錾
	bool bEndAlpha = false;		// ������
	float fAlpha = GetAlpha();	// �����x

	// �����x��������
	fAlpha -= fSub;

	if (fAlpha <= 0.0f)
	{ // �����x��������؂����ꍇ

		// �����x��␳
		fAlpha = 0.0f;

		// �����ɂȂ�؂�����Ԃɂ���
		bEndAlpha = true;

		// �v���C���[���g�̕`����~
		CObject::SetEnableDraw(false);
	}

	// �����x��ݒ�
	SetAlpha(fAlpha);

	// �����󋵂�Ԃ�
	return bEndAlpha;
}

//============================================================
//	�Z�b�g�A�b�v����
//============================================================
void CPlayer::LoadSetup(const EBody bodyID, const char **ppModelPass)
{
	// �ϐ���錾
	CSwordWaveManager::STiming waveTiming;	// �g���X�|�[���̑���p
	CMotion::SMotionInfo keyInfo;	// �L�[�̑���p
	D3DXVECTOR3 pos = VEC3_ZERO;	// �ʒu�̑���p
	D3DXVECTOR3 rot = VEC3_ZERO;	// �����̑���p
	int nSword		= NONE_IDX;		// ���C���f�b�N�X
	int nID			= 0;	// �C���f�b�N�X�̑���p
	int nParentID	= 0;	// �e�C���f�b�N�X�̑���p
	int nNowPose	= 0;	// ���݂̃|�[�Y�ԍ�
	int nNowKey		= 0;	// ���݂̃L�[�ԍ�
	int nWeapon		= 0;	// ����\����ON/OFF�̕ϊ��p
	int nLoop		= 0;	// ���[�v��ON/OFF�̕ϊ��p
	int nEnd		= 0;	// �e�L�X�g�ǂݍ��ݏI���̊m�F�p

	// �ϐ��z���錾
	char aString[MAX_STRING];	// �e�L�X�g�̕�����̑���p

	// �|�C���^��錾
	FILE *pFile;	// �t�@�C���|�C���^

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(SETUP_TXT[bodyID], "r");

	if (pFile != nullptr)
	{ // �t�@�C�����J�����ꍇ

		do
		{ // �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			nEnd = fscanf(pFile, "%s", &aString[0]);	// �e�L�X�g��ǂݍ��݂������� EOF ��Ԃ�

			// �L�����N�^�[�̐ݒ�
			if (strcmp(&aString[0], "CHARACTERSET") == 0)
			{ // �ǂݍ��񂾕����� CHARACTERSET �̏ꍇ

				do
				{ // �ǂݍ��񂾕����� END_CHARACTERSET �ł͂Ȃ��ꍇ���[�v

					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "PARTSSET") == 0)
					{ // �ǂݍ��񂾕����� PARTSSET �̏ꍇ

						do
						{ // �ǂݍ��񂾕����� END_PARTSSET �ł͂Ȃ��ꍇ���[�v

							// �t�@�C�����當�����ǂݍ���
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "INDEX") == 0)
							{ // �ǂݍ��񂾕����� INDEX �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &nID);			// ���f���̃C���f�b�N�X��ǂݍ���
							}
							else if (strcmp(&aString[0], "PARENT") == 0)
							{ // �ǂݍ��񂾕����� PARENT �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &nParentID);	// ���f���̐e�̃C���f�b�N�X��ǂݍ���
							}
							else if (strcmp(&aString[0], "POS") == 0)
							{ // �ǂݍ��񂾕����� POS �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &pos.x);		// X���W��ǂݍ���
								fscanf(pFile, "%f", &pos.y);		// Y���W��ǂݍ���
								fscanf(pFile, "%f", &pos.z);		// Z���W��ǂݍ���
							}
							else if (strcmp(&aString[0], "ROT") == 0)
							{ // �ǂݍ��񂾕����� ROT �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &rot.x);		// X������ǂݍ���
								fscanf(pFile, "%f", &rot.y);		// Y������ǂݍ���
								fscanf(pFile, "%f", &rot.z);		// Z������ǂݍ���
							}
						} while (strcmp(&aString[0], "END_PARTSSET") != 0);	// �ǂݍ��񂾕����� END_PARTSSET �ł͂Ȃ��ꍇ���[�v

						// �p�[�c���̐ݒ�
						SetPartsInfo(bodyID, nID, nParentID, pos, rot, ppModelPass[nID]);
					}
				} while (strcmp(&aString[0], "END_CHARACTERSET") != 0);		// �ǂݍ��񂾕����� END_CHARACTERSET �ł͂Ȃ��ꍇ���[�v
			}

			// ���[�V�����̐ݒ�
			else if (strcmp(&aString[0], "MOTIONSET") == 0)
			{ // �ǂݍ��񂾕����� MOTIONSET �̏ꍇ

				// ���݂̃|�[�Y�ԍ���������
				nNowPose = 0;

				// �|�[�Y����p�̕ϐ���������
				memset(&keyInfo, 0, sizeof(keyInfo));

				// �L�����Z���E�R���{�t���[�����Ȃ��ɂ���
				keyInfo.nCancelFrame = NONE_IDX;
				keyInfo.nComboFrame  = NONE_IDX;

				// �U���������������
				keyInfo.collLeft.nMin  = NONE_IDX;
				keyInfo.collLeft.nMax  = NONE_IDX;
				keyInfo.collRight.nMin = NONE_IDX;
				keyInfo.collRight.nMax = NONE_IDX;

				// ����\����OFF�ɂ���
				keyInfo.bWeaponDisp = false;

				do
				{ // �ǂݍ��񂾕����� END_MOTIONSET �ł͂Ȃ��ꍇ���[�v

					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "WEAPON") == 0)
					{ // �ǂݍ��񂾕����� WEAPON �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &nWeapon);		// ����\����ON/OFF��ǂݍ���

						// �ǂݍ��񂾒l��bool�^�ɕϊ�
						keyInfo.bWeaponDisp = (nWeapon == 0) ? false : true;
					}
					else if (strcmp(&aString[0], "LOOP") == 0)
					{ // �ǂݍ��񂾕����� LOOP �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &nLoop);		// ���[�v��ON/OFF��ǂݍ���

						// �ǂݍ��񂾒l��bool�^�ɕϊ�
						keyInfo.bLoop = (nLoop == 0) ? false : true;
					}
					else if (strcmp(&aString[0], "NUM_KEY") == 0)
					{ // �ǂݍ��񂾕����� NUM_KEY �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);		// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &keyInfo.nNumKey);	// �L�[�̑�����ǂݍ���
					}
					else if (strcmp(&aString[0], "CANCEL") == 0)
					{ // �ǂݍ��񂾕����� CANCEL �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &keyInfo.nCancelFrame);	// �L�����Z���\�t���[����ǂݍ���
					}
					else if (strcmp(&aString[0], "COMBO") == 0)
					{ // �ǂݍ��񂾕����� COMBO �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &keyInfo.nComboFrame);	// �R���{�\�t���[����ǂݍ���
					}
					else if (strcmp(&aString[0], "LEFT_COLL") == 0)
					{ // �ǂݍ��񂾕����� LEFT_COLL �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &keyInfo.collLeft.nMin);	// ������o���J�n�t���[����ǂݍ���
						fscanf(pFile, "%d", &keyInfo.collLeft.nMax);	// ����������I���t���[����ǂݍ���
					}
					else if (strcmp(&aString[0], "RIGHT_COLL") == 0)
					{ // �ǂݍ��񂾕����� RIGHT_COLL �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &keyInfo.collRight.nMin);	// ������o���J�n�t���[����ǂݍ���
						fscanf(pFile, "%d", &keyInfo.collRight.nMax);	// ����������I���t���[����ǂݍ���
					}
					else if (strcmp(&aString[0], "WAVE") == 0)
					{ // �ǂݍ��񂾕����� WAVE �̏ꍇ

						do
						{ // �ǂݍ��񂾕����� END_WAVE �ł͂Ȃ��ꍇ���[�v

							// �t�@�C�����當�����ǂݍ���
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "SWORD") == 0)
							{ // �ǂݍ��񂾕����� SWORD �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &nSword);		// ���C���f�b�N�X��ǂݍ���
							}
							else if (strcmp(&aString[0], "FRAME") == 0)
							{ // �ǂݍ��񂾕����� FRAME �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &waveTiming.nFrame);	// �����t���[����ǂݍ���
							}
							else if (strcmp(&aString[0], "OFFSETPOS") == 0)
							{ // �ǂݍ��񂾕����� OFFSETPOS �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &waveTiming.posOffset.x);	// X�ʒu�I�t�Z�b�g��ǂݍ���
								fscanf(pFile, "%f", &waveTiming.posOffset.y);	// Y�ʒu�I�t�Z�b�g��ǂݍ���
								fscanf(pFile, "%f", &waveTiming.posOffset.z);	// Z�ʒu�I�t�Z�b�g��ǂݍ���
							}
							else if (strcmp(&aString[0], "OFFSETROT") == 0)
							{ // �ǂݍ��񂾕����� OFFSETROT �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &waveTiming.rotOffset.x);	// X�����I�t�Z�b�g��ǂݍ���
								fscanf(pFile, "%f", &waveTiming.rotOffset.y);	// Y�����I�t�Z�b�g��ǂݍ���
								fscanf(pFile, "%f", &waveTiming.rotOffset.z);	// Z�����I�t�Z�b�g��ǂݍ���
							}
						} while (strcmp(&aString[0], "END_WAVE") != 0);	// �ǂݍ��񂾕����� END_WAVE �ł͂Ȃ��ꍇ���[�v

						if (nSword > NONE_IDX && nSword < player::NUM_SWORD)
						{ // ���C���f�b�N�X���͈͓��̏ꍇ

							// ���̔g���^�C�~���O��ǉ�
							waveTiming.nMotion = GetMotionNumType(bodyID);	// ���݂̃��[�V������ݒ�
							m_apSowrd[nSword]->GetWaveManager()->AddTiming(waveTiming);
						}
						else { assert(false); }	// �͈͊O
					}
					else if (strcmp(&aString[0], "KEYSET") == 0)
					{ // �ǂݍ��񂾕����� KEYSET �̏ꍇ

						// ���݂̃L�[�ԍ���������
						nNowKey = 0;

						do
						{ // �ǂݍ��񂾕����� END_KEYSET �ł͂Ȃ��ꍇ���[�v

							// �t�@�C�����當�����ǂݍ���
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "FRAME") == 0)
							{ // �ǂݍ��񂾕����� FRAME �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);							// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &keyInfo.aKeyInfo[nNowPose].nFrame);	// �L�[���؂�ւ��܂ł̃t���[������ǂݍ���
							}
							else if (strcmp(&aString[0], "MOVE") == 0)
							{ // �ǂݍ��񂾕����� MOVE �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);							// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &keyInfo.aKeyInfo[nNowPose].move.x);	// �L�[���؂�ւ��܂ł̈ړ��ʂ�ǂݍ���
								fscanf(pFile, "%f", &keyInfo.aKeyInfo[nNowPose].move.y);	// �L�[���؂�ւ��܂ł̈ړ��ʂ�ǂݍ���
								fscanf(pFile, "%f", &keyInfo.aKeyInfo[nNowPose].move.z);	// �L�[���؂�ւ��܂ł̈ړ��ʂ�ǂݍ���
							}
							else if (strcmp(&aString[0], "KEY") == 0)
							{ // �ǂݍ��񂾕����� KEY �̏ꍇ

								do
								{ // �ǂݍ��񂾕����� END_KEY �ł͂Ȃ��ꍇ���[�v

									// �t�@�C�����當�����ǂݍ���
									fscanf(pFile, "%s", &aString[0]);

									if (strcmp(&aString[0], "POS") == 0)
									{ // �ǂݍ��񂾕����� POS �̏ꍇ

										fscanf(pFile, "%s", &aString[0]);										// = ��ǂݍ��� (�s�v)
										fscanf(pFile, "%f", &keyInfo.aKeyInfo[nNowPose].aKey[nNowKey].pos.x);	// X�ʒu��ǂݍ���
										fscanf(pFile, "%f", &keyInfo.aKeyInfo[nNowPose].aKey[nNowKey].pos.y);	// Y�ʒu��ǂݍ���
										fscanf(pFile, "%f", &keyInfo.aKeyInfo[nNowPose].aKey[nNowKey].pos.z);	// Z�ʒu��ǂݍ���

										// �ǂݍ��񂾈ʒu�Ƀp�[�c�̏����ʒu�����Z
										keyInfo.aKeyInfo[nNowPose].aKey[nNowKey].pos += GetPartsPosition(bodyID, nNowKey);
									}
									else if (strcmp(&aString[0], "ROT") == 0)
									{ // �ǂݍ��񂾕����� ROT �̏ꍇ

										fscanf(pFile, "%s", &aString[0]);										// = ��ǂݍ��� (�s�v)
										fscanf(pFile, "%f", &keyInfo.aKeyInfo[nNowPose].aKey[nNowKey].rot.x);	// X������ǂݍ���
										fscanf(pFile, "%f", &keyInfo.aKeyInfo[nNowPose].aKey[nNowKey].rot.y);	// Y������ǂݍ���
										fscanf(pFile, "%f", &keyInfo.aKeyInfo[nNowPose].aKey[nNowKey].rot.z);	// Z������ǂݍ���

										// �ǂݍ��񂾌����Ƀp�[�c�̏������������Z
										keyInfo.aKeyInfo[nNowPose].aKey[nNowKey].rot += GetPartsRotation(bodyID, nNowKey);

										// ���������𐳋K��
										useful::NormalizeRot(keyInfo.aKeyInfo[nNowPose].aKey[nNowKey].rot.x);
										useful::NormalizeRot(keyInfo.aKeyInfo[nNowPose].aKey[nNowKey].rot.y);
										useful::NormalizeRot(keyInfo.aKeyInfo[nNowPose].aKey[nNowKey].rot.z);
									}

								} while (strcmp(&aString[0], "END_KEY") != 0);	// �ǂݍ��񂾕����� END_KEY �ł͂Ȃ��ꍇ���[�v

								// ���݂̃L�[�ԍ������Z
								nNowKey++;
							}
						} while (strcmp(&aString[0], "END_KEYSET") != 0);	// �ǂݍ��񂾕����� END_KEYSET �ł͂Ȃ��ꍇ���[�v

						// ���݂̃|�[�Y�ԍ������Z
						nNowPose++;
					}
				} while (strcmp(&aString[0], "END_MOTIONSET") != 0);	// �ǂݍ��񂾕����� END_MOTIONSET �ł͂Ȃ��ꍇ���[�v

				// ���[�V�������̐ݒ�
				SetMotionInfo(bodyID, keyInfo);
			}
		} while (nEnd != EOF);	// �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v
		
		// �t�@�C�������
		fclose(pFile);
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(nullptr, "�v���C���[�Z�b�g�A�b�v�t�@�C���̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);
	}
}
