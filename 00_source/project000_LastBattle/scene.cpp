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

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CStage	*CScene::m_pStage	= nullptr;	// �X�e�[�W

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

	// �v���C���[�̐���
	CPlayer::Create(m_mode);

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

		// �V�[���̏�����
		if (FAILED(pScene->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �v���C���[�̔j��
			SAFE_DELETE(pScene);
			return nullptr;
		}

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
