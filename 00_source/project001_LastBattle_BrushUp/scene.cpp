//============================================================
//
//	�V�[������ [scene.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "scene.h"
#include "manager.h"
#include "renderer.h"
#include "light.h"
#include "camera.h"
#include "sceneTitle.h"
#include "sceneTutorial.h"
#include "sceneGame.h"
#include "sceneResult.h"
#include "sceneRanking.h"

#include "stage.h"
#include "player.h"
#include "enemy.h"

// TODO�F����
#include "object2D.h"
#include "font.h"
#include "object2D.h"
#include "font2D.h"

// TODO�F�X�L�����b�V��
#include "objectSkinMesh3D.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	// �{�X�̏��
	namespace boss
	{
		const D3DXVECTOR3 SPAWN_POS = D3DXVECTOR3(0.0f, 0.0f, 600.0f);	// �X�|�[���ʒu
		const D3DXVECTOR3 SPAWN_ROT = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �X�|�[������
	}
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CStage *CScene::m_pStage = nullptr;	// �X�e�[�W�̏��

//************************************************************
//	�e�N���X [CScene] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CScene::CScene(const EMode mode) : m_mode(mode)
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CScene::~CScene()
{

}

//============================================================
//	����������
//============================================================
HRESULT CScene::Init(void)
{
	// �X�e�[�W�̐���
	m_pStage = CStage::Create(m_mode);
	if (m_pStage == nullptr)
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	if (m_mode == MODE_GAME)
	{ // �Q�[����ʂ̏ꍇ

		// �{�X�̐���
		CEnemy::Create
		( // ����
			CEnemy::TYPE_BOSS_DRAGON,	// ���
			boss::SPAWN_POS,			// �ʒu
			boss::SPAWN_ROT				// ����
		);
	}

	// �v���C���[�̐���
	CPlayer::Create(m_mode);

#if 1
#if 0
	// �t�H���g2D�̐���
	CFont2D *pCharA = CFont2D::Create
	( // ����
		GET_MANAGER->GetFont()->RegistName("���ς����C�J").pFontChar,
		L'h',
		D3DXVECTOR3(150.0f, 150.0f, 0.0f)
	);

	// �t�H���g2D�̐���
	CFont2D *pCharB = CFont2D::Create
	( // ����
		GET_MANAGER->GetFont()->RegistName("���ς����C�J").pFontChar,
		L'i',
		D3DXVECTOR3(pCharA->GetVec3Position().x + pCharA->GetNext() * 0.17f, 150.0f, 0.0f)
	);

	// �t�H���g2D�̐���
	CFont2D *pCharC = CFont2D::Create
	( // ����
		GET_MANAGER->GetFont()->RegistName("���ς����C�J").pFontChar,
		L'j',
		D3DXVECTOR3(pCharB->GetVec3Position().x + pCharB->GetNext() * 0.17f, 150.0f, 0.0f)
	);

	// �t�H���g2D�̐���
	CFont2D *pCharD = CFont2D::Create
	( // ����
		GET_MANAGER->GetFont()->RegistName("���ς����C�J").pFontChar,
		L'k',
		D3DXVECTOR3(pCharC->GetVec3Position().x + pCharC->GetNext() * 0.17f, 150.0f, 0.0f)
	);
#else
#if 1
	for (int nHeight = 0; nHeight < 8; nHeight++)
	{
		float fNext = 150.0f;
		for (int nWidth = 0; nWidth < 16; nWidth++)
		{
			// �t�H���g2D�̐���
			CFont2D *p = CFont2D::Create
			( // ����
				GET_MANAGER->GetFont()->RegistName("���ς����C�J").pFontChar,
				L'a' + nWidth + (nHeight * 16),
				D3DXVECTOR3(fNext - ((float)GET_MANAGER->GetFont()->RegistChar(L'a' + nWidth + (nHeight * 16), "���ς����C�J").glyph.gmptGlyphOrigin.x * 0.17f), 150.0f + (nHeight * 100.0f), 0.0f)
			);
			if (p == nullptr)
			{ // �����Ɏ��s�����ꍇ

				// ���s��Ԃ�
				assert(false);
				return E_FAIL;
			}

			// �D�揇�ʂ̐ݒ�
			p->SetPriority(7);

			// ���x���̐ݒ�
			p->SetLabel(CObject::LABEL_EFFECT);

			// ���̈ʒu�܂ł̋󔒗ʂ����Z
			fNext = (p->GetVec3Position().x/* - p->GetVec3Sizing().x * 0.5f*/ + p->GetNext() * 0.17f);
		}
	}
#else
#if 1
	for (int i = 0; i < 300; i++)
	{
		// �t�H���g2D�̐���
		CFont2D *p = CFont2D::Create
		( // ����
			GET_MANAGER->GetFont()->RegistName("Times New Roman").pFontChar,
			L'a' + i,
			D3DXVECTOR3(1255.0f - ((i % 25) * 50.0f), 25.0f + ((i / 25) * 50.0f), 0.0f),	// �ʒu
			1.0f,		// �傫��
			VEC3_ZERO,	// ����
			XCOL_WHITE	// �F
		);
		if (p == nullptr)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �����e�N�X�`��������
		//p->BindTexture(GET_MANAGER->GetFont()->RegistChar(L'��' + i, "���ς����C�J").nTexID);
		//p->BindTexture(GET_MANAGER->GetFont()->RegistChar(L'a' + i, "Times New Roman").nTexID);

		// �D�揇�ʂ̐ݒ�
		p->SetPriority(7);

		// ���x���̐ݒ�
		p->SetLabel(CObject::LABEL_EFFECT);
	}
#else
	for (int i = 0; i < 300; i++)
	{
		// �I�u�W�F�N�g2D�̐���
		CObject2D *p = CObject2D::Create
		( // ����
			D3DXVECTOR3(1255.0f - ((i % 25) * 50.0f), 25.0f + ((i / 25) * 50.0f), 0.0f),	// �ʒu
			D3DXVECTOR3(50.0f, 50.0f, 0.0f),	// �傫��
			VEC3_ZERO,	// ����
			XCOL_WHITE	// �F
		);
		if (p == nullptr)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �����e�N�X�`��������
		//p->BindTexture(GET_MANAGER->GetFont()->RegistChar(L'��' + i, "���ς����C�J").nTexID);
		p->BindTexture(GET_MANAGER->GetFont()->RegistChar(L'a' + i, "Times New Roman").nTexID);

		// �D�揇�ʂ̐ݒ�
		p->SetPriority(7);

		// ���x���̐ݒ�
		p->SetLabel(CObject::LABEL_EFFECT);
	}

#if 0
	// �I�u�W�F�N�g2D�̐���
	CObject2D *p = CObject2D::Create
	( // ����
		D3DXVECTOR3(1255.0f, 25.0f, 0.0f),	// �ʒu
		D3DXVECTOR3(50.0f, 50.0f, 0.0f),	// �傫��
		VEC3_ZERO,	// ����
		XCOL_BLACK	// �F
	);
	if (p == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �����e�N�X�`��������
	//p->BindTexture(GET_MANAGER->GetFont()->RegistChar(L'��', "���ς����C�J").nTexID);
	p->BindTexture(GET_MANAGER->GetFont()->RegistChar(L'��', "���ς����C�J", true).nTexID);

	// �D�揇�ʂ̐ݒ�
	p->SetPriority(7);

	// ���x���̐ݒ�
	p->SetLabel(CObject::LABEL_EFFECT);
#endif
#endif
#endif
#endif
#endif

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CScene::Uninit(void)
{
	// �X�e�[�W�̔j��
	SAFE_REF_RELEASE(m_pStage);
}

//============================================================
//	�X�V����
//============================================================
void CScene::Update(void)
{
	// �|�C���^��錾
	CManager	*pManager	= GET_MANAGER;				// �}�l�[�W���[
	CLight		*pLight		= pManager->GetLight();		// ���C�g
	CCamera		*pCamera	= pManager->GetCamera();	// �J����
	CRenderer	*pRenderer	= pManager->GetRenderer();	// �����_���[

	// �X�e�[�W�̍X�V
	assert(m_pStage != nullptr);
	m_pStage->Update();

	// ���C�g�̍X�V
	assert(pLight != nullptr);
	pLight->Update();

	// �J�����̍X�V
	assert(pCamera != nullptr);
	pCamera->Update();

	// �����_���[�̍X�V
	assert(pRenderer != nullptr);
	pRenderer->Update();
}

//============================================================
//	��������
//============================================================
CScene *CScene::Create(EMode mode)
{
	// �|�C���^��錾
	CScene *pScene = nullptr;	// �V�[�����

	// �V�[���̐���
	switch (mode)
	{ // ���[�h���Ƃ̏���
	case MODE_TITLE:
		pScene = new CSceneTitle(mode);
		break;

	case MODE_TUTORIAL:
		pScene = new CSceneTutorial(mode);
		break;

	case MODE_GAME:
		pScene = new CSceneGame(mode);
		break;

	case MODE_RESULT:
		pScene = new CSceneResult(mode);
		break;

	case MODE_RANKING:
		pScene = new CSceneRanking(mode);
		break;

	default:	// ��O����
		assert(false);
		break;
	}

	if (pScene == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �m�ۂ����A�h���X��Ԃ�
		return pScene;
	}
}

//============================================================
//	�j������
//============================================================
void CScene::Release(CScene *&prScene)
{
	// �V�[���̏I��
	assert(prScene != nullptr);
	prScene->Uninit();

	// �������J��
	SAFE_DELETE(prScene);
}

//============================================================
//	�X�e�[�W�擾����
//============================================================
CStage *CScene::GetStage(void)
{
	// �C���X�^���X���g�p
	assert(m_pStage != nullptr);

	// �X�e�[�W�̃|�C���^��Ԃ�
	return m_pStage;
}

//============================================================
//	�v���C���[�擾����
//============================================================
CPlayer *CScene::GetPlayer(void)
{
	CListManager<CPlayer> *pListManager = CPlayer::GetList();	// �v���C���[���X�g�}�l�[�W���[
	if (pListManager == nullptr)		 { return nullptr; }	// ���X�g���g�p�̏ꍇ������
	if (pListManager->GetNumAll() != 1)	 { return nullptr; }	// �v���C���[��1�l�ł͂Ȃ��ꍇ������
	CPlayer *pPlayer = pListManager->GetList().front();			// �v���C���[�̏��

	// �v���C���[�̃|�C���^��Ԃ�
	return pPlayer;
}

//============================================================
//	�{�X�擾����
//============================================================
CEnemy *CScene::GetBoss(void)
{
	CListManager<CEnemy> *pListManager = CEnemy::GetList();	// �G���X�g�}�l�[�W���[
	if (pListManager == nullptr) { return nullptr; }		// ���X�g���g�p

	std::list<CEnemy*> listEnemy = pListManager->GetList();	// �G���X�g
	for (CEnemy *pEnemy : listEnemy)
	{ // �v�f�����J��Ԃ�

		if (pEnemy->GetType() == CEnemy::TYPE_BOSS_DRAGON)
		{ // �G���{�X�̏ꍇ

			// �G�̃|�C���^��Ԃ�
			return pEnemy;
		}
	}

	return nullptr;
}

//============================================================
//	���[�h�̐ݒ菈��
//============================================================
void CScene::SetMode(const EMode mode)
{
	// �����̃��[�h��ݒ�
	m_mode = mode;
}

//============================================================
//	���[�h�擾����
//============================================================
CScene::EMode CScene::GetMode(void) const
{
	// ���݂̃��[�h��Ԃ�
	return m_mode;
}
