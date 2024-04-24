//============================================================
//
//	�^�C�}�[UI�w�b�_�[ [timerUI.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _TIMER_UI_H_
#define _TIMER_UI_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "timerManager.h"

//************************************************************
//	�O���錾
//************************************************************
class CObject2D;	// �I�u�W�F�N�g2D�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �^�C�}�[UI�N���X
class CTimerUI : public CTimerManager
{
public:
	// �e�N�X�`����
	enum ETexture
	{
		TEXTURE_BG = 0,	// �w�i�e�N�X�`��
		TEXTURE_MAX		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CTimerUI();

	// �f�X�g���N�^
	~CTimerUI() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void SetPosition(const D3DXVECTOR3& rPos) override;	// �ʒu�ݒ�
	void SetPriority(const int nPriority) override;		// �D�揇�ʐݒ�
	void SetEnableUpdate(const bool bUpdate) override;	// �X�V�󋵐ݒ�
	void SetEnableDraw(const bool bDraw) override;		// �`��󋵐ݒ�

	// �ÓI�����o�֐�
	static CTimerUI *Create	// ����
	( // ����
		const ETime time,				// �ݒ�^�C��
		const long nTime,				// ��������
		const D3DXVECTOR3& rPos,		// �ʒu
		const D3DXVECTOR3& rSizeValue,	// �����̑傫��
		const D3DXVECTOR3& rSizePart,	// ��؂�̑傫��
		const D3DXVECTOR3& rSpaceValue,	// �����̋�
		const D3DXVECTOR3& rSpacePart,	// ��؂�̋�
		const D3DXVECTOR3& rOffsetBG,	// �w�i�̃I�t�Z�b�g
		const D3DXVECTOR3& rSizeBG		// �w�i�̑傫��
	);
	static void Release(CTimerUI *&prTimerUI);	// �j��

	// �����o�֐�
	void SetOffset(const D3DXVECTOR3& rOffset);	// �I�t�Z�b�g�ݒ�
	void SetSizingBG(const D3DXVECTOR3& rSize);	// �w�i�傫���ݒ�

private:
	// �����o�֐�
	void SetPositionRelative(void);	// ���Έʒu�ݒ�

	// �����o�ϐ�
	CObject2D *m_pBG;		// �w�i�̏��
	D3DXVECTOR3 m_offsetBG;	// �w�i�̃I�t�Z�b�g
};

#endif	// _TIMER_UI_H_
