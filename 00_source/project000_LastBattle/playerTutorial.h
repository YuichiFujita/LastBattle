//============================================================
//
//	�v���C���[�`���[�g���A���w�b�_�[ [playerTutorial.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _PLAYER_TUTORIAL_H_
#define _PLAYER_TUTORIAL_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "player.h"

//************************************************************
//	�N���X��`
//************************************************************
// �v���C���[�`���[�g���A���N���X
class CPlayerTutorial : public CPlayer
{
public:
	// �R���X�g���N�^
	CPlayerTutorial();

	// �f�X�g���N�^
	~CPlayerTutorial() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(CShader *pShader = nullptr) override;	// �`��

private:
	// �I�[�o�[���C�h�֐�
	void UpdateNormal(int *pLowMotion, int *pUpMotion) override;	// �ʏ��Ԏ��̍X�V
};

#endif	// _PLAYER_TUTORIAL_H_
