//============================================================
//
//	������ [sword.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "sword.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "collision.h"
#include "swordWaveManager.h"
#include "orbit.h"
#include "enemy.h"
#include "collSphere.h"
#include "scene.h"
#include "player.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char *MODEL_FILE[] =	// ���f���t�@�C��
	{
		"data\\MODEL\\PLAYER\\15_sword.x",	// ��
	};
	const char *SETUP_TXT	= "data\\TXT\\sword.txt";	// �Z�b�g�A�b�v�e�L�X�g���΃p�X
	const float	SUB_ALPHA	= 0.05f;	// �����x�̌��Z��
	const int	DMG_HIT		= 10;		// �q�b�g���̃_���[�W��
	const int	ORBIT_PART	= 18;		// ������

	const COrbit::SOffset ORBIT_OFFSET = COrbit::SOffset	// �I�t�Z�b�g���
	(
		D3DXVECTOR3(0.0f, 0.0f, -10.0f),
		D3DXVECTOR3(0.0f, 0.0f, -65.0f),
		D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.5f)
	);
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CSword::SCollInfo CSword::m_collInfo = {};	// ������

//************************************************************
//	�X�^�e�B�b�N�A�T�[�g
//************************************************************
static_assert(NUM_ARRAY(MODEL_FILE) == CSword::MODEL_MAX, "ERROR : Model Count Mismatch");

//************************************************************
//	�q�N���X [CSword] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CSword::CSword() : CMultiModel(LABEL_NONE, DIM_3D, object::DEFAULT_PRIO),
	m_pWaveManager	(nullptr),		// ���g�������}�l�[�W���[
	m_state			(STATE_NONE),	// ���
	m_bAttack		(false),		// �U����
	m_nCounterState	(0)				// ��ԊǗ��J�E���^�[
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CSword::~CSword()
{

}

//============================================================
//	����������
//============================================================
HRESULT CSword::Init(void)
{
	// �����o�ϐ���������
	m_pWaveManager	= nullptr;		// ���g�������}�l�[�W���[
	m_state			= STATE_NONE;	// ���
	m_bAttack		= true;			// �U����
	m_nCounterState	= 0;			// ��ԊǗ��J�E���^�[

	// �}���`���f���̏�����
	if (FAILED(CMultiModel::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ���f����o�^�E����
	BindModel(MODEL_FILE[MODEL_SWORD]);

	// ���g�������}�l�[�W���[�̐���
	m_pWaveManager = CSwordWaveManager::Create();
	if (m_pWaveManager == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �O�Ղ̐���
	m_pOrbit = COrbit::Create
	( // ����
		GetPtrMtxWorld(),	// �e�}�g���b�N�X
		ORBIT_OFFSET,		// �I�t�Z�b�g���
		ORBIT_PART			// ������
	);
	if (m_pOrbit == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �O�Ղ�����
	m_pOrbit->SetState(COrbit::STATE_NONE);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CSword::Uninit(void)
{
	// ������̔j��
	SAFE_DEL_ARRAY(m_collInfo.pColl);

	// ���g�������}�l�[�W���[�̔j��
	SAFE_REF_RELEASE(m_pWaveManager);

	// �O�Ղ̏I��
	SAFE_UNINIT(m_pOrbit);

	// �}���`���f���̏I��
	CMultiModel::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CSword::Update(void)
{
	if (m_state == STATE_VANISH)
	{ // �������̏ꍇ

		// �ϐ���錾
		float fAlpha = GetAlpha();	// �����x

		// �����ɂ��Ă���
		fAlpha -= SUB_ALPHA;

		if (fAlpha <= 0.0f)
		{ // �����ɂȂ�؂����ꍇ

			// �������Ȃ���Ԃɂ���
			SetState(STATE_NONE);
		}

		// �����x�𔽉f
		SetAlpha(fAlpha);
	}

	if (m_bAttack)
	{ // �U�����肪ON�̏ꍇ

		if (m_pOrbit->GetState() != STATE_NORMAL)
		{ // �܂��\������Ă��Ȃ��ꍇ

			// �O�Ղ��o��
			m_pOrbit->SetState(COrbit::STATE_NORMAL);
		}
	}
	else
	{ // �U�����肪OFF�̏ꍇ

		// �O�Ղ�����
		m_pOrbit->SetState(COrbit::STATE_VANISH);
	}

	// TODO�F�����̂̔����OFF
#if 0
	// �G�Ƃ̓����蔻��
	CollisionEnemy();
#endif

	// ���g�������}�l�[�W���[�̍X�V
	m_pWaveManager->Update();

	// �O�Ղ̍X�V
	m_pOrbit->Update();

	// �}���`���f���̍X�V
	CMultiModel::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CSword::Draw(CShader *pShader)
{
	if (m_state != STATE_NONE)
	{ // �������Ȃ���ԈȊO�̏ꍇ

		// �}���`���f���̕`��
		CMultiModel::Draw(pShader);
	}
}

//============================================================
//	��Ԏ擾����
//============================================================
int CSword::GetState(void) const
{
	// ��Ԃ�Ԃ�
	return m_state;
}

//============================================================
//	��������
//============================================================
CSword *CSword::Create
(
	CObject *pParent,			// �e�I�u�W�F�N�g
	const D3DXVECTOR3 &rPos,	// �ʒu
	const D3DXVECTOR3 &rRot		// ����
)
{
	// ���̐���
	CSword *pSword = new CSword;
	if (pSword == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// ���̏�����
		if (FAILED(pSword->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ���̔j��
			SAFE_DELETE(pSword);
			return nullptr;
		}

		// �e�I�u�W�F�N�g��ݒ�
		pSword->SetParentObject(pParent);

		// �ʒu��ݒ�
		pSword->SetVec3Position(rPos);

		// ������ݒ�
		pSword->SetVec3Rotation(rRot);

		// �m�ۂ����A�h���X��Ԃ�
		return pSword;
	}
}

//============================================================
//	��Ԃ̐ݒ菈��
//============================================================
void CSword::SetState(const EState state)
{
	if (state == m_state && state != STATE_NORMAL)
	{ // �ݒ肷���Ԃ����݂̏�Ԋ��A�ݒ肷���Ԃ��ʏ��Ԃ̏ꍇ

		// �����𔲂���
		return;
	}

	if (m_state == STATE_NONE && state == STATE_VANISH)
	{ // ���łɏ����Ă���̂ɏ��������悤�Ƃ��Ă���ꍇ

		// �����𔲂���
		return;
	}

	// �����̏�Ԃ�ݒ�
	m_state = state;

	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_NONE:	// �������Ȃ����

		// �O�Ղ̎����`���OFF�ɂ���
		m_pOrbit->SetState(COrbit::STATE_NONE);

	case STATE_VANISH:	// �������
		break;

	case STATE_NORMAL:	// �ʏ���

		// �s�����ɂ���
		SetAlpha(1.0f);

		break;

	default:	// ��O���
		assert(false);
		break;
	}
}

//============================================================
//	�G�Ƃ̓����蔻��
//============================================================
void CSword::CollisionEnemy(void)
{
	CListManager<CEnemy> *pList = CEnemy::GetList();	// ���X�g�}�l�[�W���[
	if (pList == nullptr)		 { return; }	// ���X�g���g�p
	if (pList->GetNumAll() <= 0) { return; }	// �G�����݂��Ȃ�
	if (!m_bAttack)				 { return; }	// �U��OFF

	D3DXMATRIX mtxWorld = GetMtxWorld();		// ���[���h�}�g���b�N�X
	std::list<CEnemy*> list = pList->GetList();	// �G���X�g
	for (auto enemy : list)
	{ // ���X�g�̂��ׂĂ��J��Ԃ�

		int nNumParts = enemy->GetNumModel();	// �p�[�c��
		for (int nCntColl = 0; nCntColl < nNumParts; nCntColl++)
		{ // �p�[�c�����J��Ԃ�

			int nCntArray = 0;	// ������̗v�f�ԍ�
			CCollSphere *pColl = enemy->GetCollision(nCntColl);				// �~������
			std::vector<CCollSphere::SInfo> vector = pColl->GetVector();	// �~����z��
			for (auto coll : vector)
			{ // �z��̗v�f�����J��Ԃ�

				// �G�̔���ʒu���v�Z
				D3DXVECTOR3 posCollEnemy = pColl->CalcWorldPosition(nCntArray);

				// �v�f�ԍ�������
				nCntArray++;

				for (int i = 0; i < m_collInfo.nNumColl; i++)
				{ // ���萔���J��Ԃ�

					D3DXVECTOR3 posCollSword;		// ���̔���ʒu
					D3DXMATRIX mtxTrans, mtxColl;	// �}�g���b�N�X�v�Z�p

					// �I�t�Z�b�g�}�g���b�N�X���쐬
					D3DXMatrixTranslation
					( // ����
						&mtxTrans,
						m_collInfo.pColl[i].offset.x,
						m_collInfo.pColl[i].offset.y,
						m_collInfo.pColl[i].offset.z
					);

					// ���̃}�g���b�N�X�Ɗ|�����킹��
					D3DXMatrixMultiply(&mtxColl, &mtxTrans, &mtxWorld);

					// ����̔��蒆�S�ʒu��ݒ�
					posCollSword = useful::GetMatrixPosition(mtxColl);

					// �U���̓����蔻��
					bool bHit = collision::Circle3D
					( // ����
						posCollSword,	// ����ʒu
						posCollEnemy,	// ����ڕW�ʒu
						m_collInfo.pColl[i].fRadius,	// ���蔼�a
						coll.fRadius					// ����ڕW���a
					);
					if (bHit)
					{ // �U�������������ꍇ

						// �G�̃q�b�g����
						if (enemy->Hit(DMG_HIT))
						{ // �G�ɍU���ł����ꍇ

							// ���q�b�g���̍Đ�
							PLAY_SOUND(CSound::LABEL_SE_SWORD_HIT);
						}

						// �֐��𔲂���
						return;
					}
				}
			}
		}
	}
}

//============================================================
//	�Z�b�g�A�b�v����
//============================================================
void CSword::LoadSetup(void)
{
	// �ϐ���錾
	int nCurrentID = 0;	// ���݂̓ǂݍ��ݐ��̕ۑ��p
	int nEnd = 0;		// �e�L�X�g�ǂݍ��ݏI���̊m�F�p

	// �ϐ��z���錾
	char aString[MAX_STRING];	// �e�L�X�g�̕�����̑���p

	// �|�C���^��錾
	FILE *pFile;	// �t�@�C���|�C���^

	// ������̔j��
	SAFE_DEL_ARRAY(m_collInfo.pColl);

	// �ÓI�����o�ϐ��̏����N���A
	memset(&m_collInfo, 0, sizeof(m_collInfo));	// ������

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(SETUP_TXT, "r");

	if (pFile != nullptr)
	{ // �t�@�C�����J�����ꍇ

		do
		{ // �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			nEnd = fscanf(pFile, "%s", &aString[0]);	// �e�L�X�g��ǂݍ��݂������� EOF ��Ԃ�

			// ����̐ݒ�
			if (strcmp(&aString[0], "COLLISION_SET") == 0)
			{ // �ǂݍ��񂾕����� COLLISION_SET �̏ꍇ

				// ���݂̓ǂݍ��ݐ���������
				nCurrentID = 0;

				do
				{ // �ǂݍ��񂾕����� END_COLLISION_SET �ł͂Ȃ��ꍇ���[�v

					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "NUMCOLL") == 0)
					{ // �ǂݍ��񂾕����� NUMCOLL �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &m_collInfo.nNumColl);	// ���萔��ǂݍ���

						if (m_collInfo.nNumColl > 0)
						{ // ���肪����ꍇ

							if (m_collInfo.pColl == nullptr)
							{ // �����񂪎g�p����Ă��Ȃ��ꍇ

								// ������̓ǂݍ��ݐ����������m��
								m_collInfo.pColl = new SColl[m_collInfo.nNumColl];

								if (m_collInfo.pColl != nullptr)
								{ // �m�ۂɐ��������ꍇ

									// �������N���A
									memset(m_collInfo.pColl, 0, sizeof(SColl) * m_collInfo.nNumColl);
								}
								else { assert(false); }	// �m�ێ��s
							}
							else { assert(false); }	// �g�p��
						}
						else
						{ // �ǂݍ��ނ��̂��Ȃ��ꍇ

							// �����𔲂���
							break;
						}
					}
					else if (strcmp(&aString[0], "COLLSET") == 0)
					{ // �ǂݍ��񂾕����� COLLSET �̏ꍇ
			
						do
						{ // �ǂݍ��񂾕����� END_COLLSET �ł͂Ȃ��ꍇ���[�v
			
							// �t�@�C�����當�����ǂݍ���
							fscanf(pFile, "%s", &aString[0]);
			
							if (strcmp(&aString[0], "OFFSET") == 0)
							{ // �ǂݍ��񂾕����� OFFSET �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);								// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &m_collInfo.pColl[nCurrentID].offset.x);	// ����I�t�Z�b�gX��ǂݍ���
								fscanf(pFile, "%f", &m_collInfo.pColl[nCurrentID].offset.y);	// ����I�t�Z�b�gY��ǂݍ���
								fscanf(pFile, "%f", &m_collInfo.pColl[nCurrentID].offset.z);	// ����I�t�Z�b�gZ��ǂݍ���
							}
							else if (strcmp(&aString[0], "RADIUS") == 0)
							{ // �ǂݍ��񂾕����� RADIUS �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);							// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &m_collInfo.pColl[nCurrentID].fRadius);	// ���蔼�a��ǂݍ���
							}
						} while (strcmp(&aString[0], "END_COLLSET") != 0);	// �ǂݍ��񂾕����� END_COLLSET �ł͂Ȃ��ꍇ���[�v

						// �Ǎ������I�[�o�[
						assert(nCurrentID < m_collInfo.nNumColl);

						// ���݂̓ǂݍ��ݐ������Z
						nCurrentID++;
					}
				} while (strcmp(&aString[0], "END_COLLISION_SET") != 0);	// �ǂݍ��񂾕����� END_COLLISION_SET �ł͂Ȃ��ꍇ���[�v

				// �Ǎ������̕s��v
				assert(nCurrentID == m_collInfo.nNumColl);
			}
		} while (nEnd != EOF);	// �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v
		
		// �t�@�C�������
		fclose(pFile);
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(nullptr, "���Z�b�g�A�b�v�̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);
	}
}
