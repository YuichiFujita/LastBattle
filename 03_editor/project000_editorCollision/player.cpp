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
#include "camera.h"
#include "fade.h"
#include "stage.h"
#include "shadow.h"
#include "field.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char *SETUP_TXT = "data\\TXT\\setup.txt";	// �Z�b�g�A�b�v�e�L�X�g

	const int	PRIORITY	= 3;		// �v���C���[�̗D�揇��
	const float	JUMP		= 18.0f;	// �W�����v�㏸��
	const float	GRAVITY		= 0.85f;	// �d��
	const float	RADIUS		= 20.0f;	// ���a
	const float	HEIGHT		= 80.0f;	// �c��
	const float	REV_ROTA	= 0.15f;	// �����ύX�̕␳�W��
	const float	MOVE_REV	= 0.5f;		// �ړ��ʂ̕␳�W��
	const float	JUMP_REV	= 0.16f;	// �󒆂̈ړ��ʂ̌����W��
	const float	LAND_REV	= 0.16f;	// �n��̈ړ��ʂ̌����W��
	const float	STICK_REV	= 0.00015f;	// �ړ�����X�e�B�b�N�̌X���ʂ̕␳�W��
	const float	ADD_ALPHA	= 0.03f;	// �����x�̉��Z��

	const D3DXVECTOR3 SHADOW_SIZE = D3DXVECTOR3(80.0f, 0.0f, 80.0f);	// �e�̑傫��
}

//************************************************************
//	�q�N���X [CPlayer] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CPlayer::CPlayer() : CObjectChara(CObject::LABEL_PLAYER, CObject::DIM_3D, PRIORITY),
	m_pShadow		(nullptr),		// �e�̏��
	m_oldPos		(VEC3_ZERO),	// �ߋ��ʒu
	m_move			(VEC3_ZERO),	// �ړ���
	m_destRot		(VEC3_ZERO),	// �ڕW����
	m_state			(STATE_NONE),	// ���
	m_bJump			(false),		// �W�����v��
	m_nCounterState	(0)				// ��ԊǗ��J�E���^�[
{
	// �����o�ϐ����N���A
	m_modelFileName.clear();	// ���f���t�@�C����
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
	m_modelFileName.clear();		// ���f���t�@�C����
	m_pShadow		= nullptr;		// �e�̏��
	m_oldPos		= VEC3_ZERO;	// �ߋ��ʒu
	m_move			= VEC3_ZERO;	// �ړ���
	m_destRot		= VEC3_ZERO;	// �ڕW����
	m_state			= STATE_NORMAL;	// ���
	m_bJump			= true;			// �W�����v��
	m_nCounterState	= 0;			// ��ԊǗ��J�E���^�[

	// �I�u�W�F�N�g�L�����N�^�[�̏�����
	if (FAILED(CObjectChara::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �Z�b�g�A�b�v�̓Ǎ�
	LoadSetup();

	// ���f�����̐ݒ�
	SetModelInfo();

	// �e�̐���
	m_pShadow = CShadow::Create(CShadow::TEXTURE_NORMAL, SHADOW_SIZE, this);
	if (m_pShadow == nullptr)
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CPlayer::Uninit(void)
{
	// ���f���t�@�C�������N���A
	m_modelFileName.clear();

	// �e�̏I��
	m_pShadow->DeleteObjectParent();	// �e�I�u�W�F�N�g���폜
	SAFE_UNINIT(m_pShadow);

	// �I�u�W�F�N�g�L�����N�^�[�̏I��
	CObjectChara::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CPlayer::Update(void)
{
	// �ϐ���錾
	int nCurMotion = 0;	// ���݂̃��[�V����

	// �I�u�W�F�N�g�L�����N�^�[�̍X�V
	CObjectChara::Update();

	// �ߋ��ʒu�̍X�V
	UpdateOldPosition();

	// �d�͂̍X�V
	UpdateGravity();

	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_NONE:
	case STATE_DEATH:
		break;

	case STATE_NORMAL:

		// �ʏ��Ԃ̍X�V
		UpdateNormal(&nCurMotion);

		break;

	default:
		assert(false);
		break;
	}

	// �e�̍X�V
	m_pShadow->Update();

	// ���[�V�����̍X�V
	UpdateMotion(nCurMotion);
}

//============================================================
//	�`�揈��
//============================================================
void CPlayer::Draw(void)
{
	// �I�u�W�F�N�g�L�����N�^�[�̕`��
	CObjectChara::Draw();
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
//	��������
//============================================================
CPlayer *CPlayer::Create(void)
{
	// �v���C���[�̐���
	CPlayer *pPlayer = new CPlayer;
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
//	���[�V�����̍X�V����
//============================================================
void CPlayer::UpdateMotion(const int nMotion)
{
	if (nMotion <= NONE_IDX || nMotion >= GetMotionNumType()) { assert(false); return; }	// ���[�V���������ݒ�
	if (IsDeath()) { return; }	// ���S���Ă���

	if (IsMotionLoop())
	{ // ���[�v���郂�[�V�����̏ꍇ

		if (GetMotionType() != nMotion)
		{ // ���݂̃��[�V�������Đ����̃��[�V�����ƈ�v���Ȃ��ꍇ

			// ���݂̃��[�V�����̐ݒ�
			SetMotion(nMotion);
		}
	}
	else
	{ // ���[�v���Ȃ����[�V�����̏ꍇ

		switch (GetMotionType())
		{ // ���[�V�������Ƃ̏���
		case MOTION_JUMP:	// �W�����v���[�V�����F���[�vOFF
			break;

		case MOTION_LAND:	// ���n���[�V�����F���[�vOFF

			if (nMotion != MOTION_IDOL)
			{ // �ҋ@���[�V�����ȊO�̏ꍇ

				// ���݂̃��[�V�����̐ݒ�
				SetMotion(nMotion);
			}

			break;
		}
	}

	switch (GetMotionType())
	{ // ���[�V�������Ƃ̏���
	case MOTION_IDOL:	// �ҋ@���[�V�����F���[�vON
	case MOTION_MOVE:	// �ړ����[�V�����F���[�vON
	case MOTION_JUMP:	// �W�����v���[�V�����F���[�vOFF
		break;

	case MOTION_LAND:	// ���n���[�V�����F���[�vOFF

		if (IsMotionFinish())
		{ // ���[�V�������I�����Ă����ꍇ

			// ���݂̃��[�V�����̐ݒ�
			SetMotion(nMotion);
		}

		break;
	}
}

//============================================================
//	�ʏ��Ԏ��̍X�V����
//============================================================
void CPlayer::UpdateNormal(int *pMotion)
{
	// �ϐ���錾
	D3DXVECTOR3 posPlayer = GetVec3Position();	// �v���C���[�ʒu
	D3DXVECTOR3 rotPlayer = GetVec3Rotation();	// �v���C���[����

	// �|�C���^��錾
	CStage *pStage = CScene::GetStage();				// �X�e�[�W���
	if (pStage == nullptr) { assert(false); return; }	// �X�e�[�W��g�p��

	// �ړ�����E�ڕW�����ݒ�
	UpdateMove(pMotion);

	// �W�����v����
	UpdateJump(pMotion);

	// �ʒu�X�V
	UpdatePosition(&posPlayer);

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
//	�ړ�����E�ڕW�����̍X�V����
//============================================================
void CPlayer::UpdateMove(int *pMotion)
{
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

		// �ړ����[�V������ݒ�
		*pMotion = MOTION_MOVE;
	}
}

//============================================================
//	�W�����v����̍X�V����
//============================================================
void CPlayer::UpdateJump(int *pMotion)
{
	if (m_bJump) { return; }	// �W�����v���̏ꍇ������

	if (GET_INPUTPAD->IsTrigger(CInputPad::KEY_A))
	{
		// ��ړ��ʂ�^����
		m_move.y += JUMP;

		// �W�����v���ɂ���
		m_bJump = true;

		// �W�����v���[�V������ݒ�
		*pMotion = MOTION_JUMP;
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
	m_bJump = true;

	// �n�ʁE�����ʒu�̒��n����
	if (CScene::GetStage()->LandFieldPosition(*pPos, m_move)
	||  CScene::GetStage()->LandLimitPosition(*pPos, m_move, 0.0f))
	{ // �v���C���[�����n���Ă����ꍇ

		// �W�����v�t���O��OFF�ɂ���
		m_bJump = false;
	}

	if (!m_bJump)
	{ // �󒆂ɂ��Ȃ��ꍇ

		if (GetMotionType() == MOTION_JUMP)
		{ // ���[�V�������W�����v���̏ꍇ

			// ���n���[�V�������w��
			SetMotion(MOTION_LAND);
		}
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
//	�Z�b�g�A�b�v����
//============================================================
void CPlayer::LoadSetup(void)
{
	// �ϐ���錾
	CMotion::SMotionInfo info;		// �|�[�Y�̑���p
	D3DXVECTOR3 pos = VEC3_ZERO;	// �ʒu�̑���p
	D3DXVECTOR3 rot = VEC3_ZERO;	// �����̑���p
	int nModel		= 0;	// ���f���C���f�b�N�X�̑���p
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
	pFile = fopen(SETUP_TXT, "r");

	if (pFile != nullptr)
	{ // �t�@�C�����J�����ꍇ

		do
		{ // �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			nEnd = fscanf(pFile, "%s", &aString[0]);	// �e�L�X�g��ǂݍ��݂������� EOF ��Ԃ�

			// �L�����N�^�[�̐ݒ�
			if (strcmp(&aString[0], "MODEL_FILENAME") == 0)
			{ // �ǂݍ��񂾕����� MODEL_FILENAME �̏ꍇ

				// �t�@�C�����當�����ǂݍ���
				fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
				fscanf(pFile, "%s", &aString[0]);	// ���f���̃t�@�C���p�X��ǂݍ���

				// �t�@�C���p�X��ݒ�
				m_modelFileName[nModel] = &aString[0];

				// ���f���C���f�b�N�X�����Z
				nModel++;
			}

			// �L�����N�^�[�̐ݒ�
			else if (strcmp(&aString[0], "CHARACTERSET") == 0)
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

						auto pModel = m_modelFileName.find(nID);
						if (pModel != m_modelFileName.end())
						{ // ���f�������������ꍇ

							// �p�[�c���̐ݒ�
							SetPartsInfo(nID, nParentID, pos, rot, pModel->second.c_str());
						}
						else { assert(false); }	// ���f���Ȃ�
					}
				} while (strcmp(&aString[0], "END_CHARACTERSET") != 0);		// �ǂݍ��񂾕����� END_CHARACTERSET �ł͂Ȃ��ꍇ���[�v
			}

			// ���[�V�����̐ݒ�
			else if (strcmp(&aString[0], "MOTIONSET") == 0)
			{ // �ǂݍ��񂾕����� MOTIONSET �̏ꍇ

				// ���݂̃|�[�Y�ԍ���������
				nNowPose = 0;

				// �|�[�Y����p�̕ϐ���������
				memset(&info, 0, sizeof(info));

				// �L�����Z���t���[�����Ȃ��ɂ���
				info.nCancelFrame = NONE_IDX;

				// �U���������������
				info.collLeft.nMin  = NONE_IDX;
				info.collLeft.nMax  = NONE_IDX;
				info.collRight.nMin = NONE_IDX;
				info.collRight.nMax = NONE_IDX;

				// ����\����OFF�ɂ���
				info.bWeaponDisp = false;

				do
				{ // �ǂݍ��񂾕����� END_MOTIONSET �ł͂Ȃ��ꍇ���[�v

					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "WEAPON") == 0)
					{ // �ǂݍ��񂾕����� WEAPON �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &nWeapon);		// ����\����ON/OFF��ǂݍ���

						// �ǂݍ��񂾒l��bool�^�ɕϊ�
						info.bWeaponDisp = (nWeapon == 0) ? false : true;
					}
					else if (strcmp(&aString[0], "LOOP") == 0)
					{ // �ǂݍ��񂾕����� LOOP �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &nLoop);		// ���[�v��ON/OFF��ǂݍ���

						// �ǂݍ��񂾒l��bool�^�ɕϊ�
						info.bLoop = (nLoop == 0) ? false : true;
					}
					else if (strcmp(&aString[0], "NUM_KEY") == 0)
					{ // �ǂݍ��񂾕����� NUM_KEY �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &info.nNumKey);	// �L�[�̑�����ǂݍ���
					}
					else if (strcmp(&aString[0], "CANCEL") == 0)
					{ // �ǂݍ��񂾕����� CANCEL �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &info.nCancelFrame);	// �L�����Z���\�t���[����ǂݍ���
					}
					else if (strcmp(&aString[0], "LEFT_COLL") == 0)
					{ // �ǂݍ��񂾕����� LEFT_COLL �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &info.collLeft.nMin);	// ������o���J�n�t���[����ǂݍ���
						fscanf(pFile, "%d", &info.collLeft.nMax);	// ����������I���t���[����ǂݍ���
					}
					else if (strcmp(&aString[0], "RIGHT_COLL") == 0)
					{ // �ǂݍ��񂾕����� RIGHT_COLL �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &info.collRight.nMin);	// ������o���J�n�t���[����ǂݍ���
						fscanf(pFile, "%d", &info.collRight.nMax);	// ����������I���t���[����ǂݍ���
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

								fscanf(pFile, "%s", &aString[0]);						// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &info.aKeyInfo[nNowPose].nFrame);	// �L�[���؂�ւ��܂ł̃t���[������ǂݍ���
							}
							else if (strcmp(&aString[0], "MOVE") == 0)
							{ // �ǂݍ��񂾕����� MOVE �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);						// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].move.x);	// �L�[���؂�ւ��܂ł̈ړ��ʂ�ǂݍ���
								fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].move.y);	// �L�[���؂�ւ��܂ł̈ړ��ʂ�ǂݍ���
								fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].move.z);	// �L�[���؂�ւ��܂ł̈ړ��ʂ�ǂݍ���
							}
							else if (strcmp(&aString[0], "KEY") == 0)
							{ // �ǂݍ��񂾕����� KEY �̏ꍇ

								do
								{ // �ǂݍ��񂾕����� END_KEY �ł͂Ȃ��ꍇ���[�v

									// �t�@�C�����當�����ǂݍ���
									fscanf(pFile, "%s", &aString[0]);

									if (strcmp(&aString[0], "POS") == 0)
									{ // �ǂݍ��񂾕����� POS �̏ꍇ

										fscanf(pFile, "%s", &aString[0]);									// = ��ǂݍ��� (�s�v)
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].pos.x);	// X�ʒu��ǂݍ���
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].pos.y);	// Y�ʒu��ǂݍ���
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].pos.z);	// Z�ʒu��ǂݍ���

										// �ǂݍ��񂾈ʒu�Ƀp�[�c�̏����ʒu�����Z
										info.aKeyInfo[nNowPose].aKey[nNowKey].pos += GetPartsPosition(nNowKey);
									}
									else if (strcmp(&aString[0], "ROT") == 0)
									{ // �ǂݍ��񂾕����� ROT �̏ꍇ

										fscanf(pFile, "%s", &aString[0]);									// = ��ǂݍ��� (�s�v)
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].rot.x);	// X������ǂݍ���
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].rot.y);	// Y������ǂݍ���
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].rot.z);	// Z������ǂݍ���

										// �ǂݍ��񂾌����Ƀp�[�c�̏������������Z
										info.aKeyInfo[nNowPose].aKey[nNowKey].rot += GetPartsRotation(nNowKey);

										// ���������𐳋K��
										useful::Vec3NormalizeRot(info.aKeyInfo[nNowPose].aKey[nNowKey].rot);
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
				SetMotionInfo(info);
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
