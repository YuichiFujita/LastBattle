//============================================================
//
//	�v���C����w�b�_�[ [playControl.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _PLAY_CONTROL_H_
#define _PLAY_CONTROL_H_

//************************************************************
//	�O���錾
//************************************************************
class CAnim2D;	// �A�j���[�V����2D�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �v���C����N���X
class CPlayControl
{
public:
	// �����
	enum EControl
	{
		CONTROL_NONE = 0,	// ����Ȃ�
		CONTROL_RIDE,		// �R�摀��
		CONTROL_RUSH,		// �A���U������
		CONTROL_MAX			// ���̗񋓌^�̑���
	};

	// ��ԗ�
	enum EState
	{
		STATE_NONE = 0,		// �������Ȃ�
		STATE_SCALEIN,		// �\���J�n
		STATE_DISP,			// �\��
		STATE_FADEOUT,		// �\���I��
		STATE_MAX			// ���̗񋓌^�̑���
	};

	// �\����
	enum EDisp
	{
		DISP_NORMAL = 0,	// �Œ�\��
		DISP_BLINK,			// �_�ŕ\��
		DISP_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CPlayControl();

	// �f�X�g���N�^
	~CPlayControl();

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��

	void SetDisp	// �\���ݒ�
	( // ����
		EControl contType,	// �\������
		EDisp dispType		// �\���`��
	);
	void SetHide(const bool bFlash = true);	// ��\���ݒ�
	bool IsDisp(void);	// �\���擾

	// �ÓI�����o�֐�
	static CPlayControl *Create	// ����
	( // ����
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rSize	// �傫��
	);
	static void Release(CPlayControl *&prPlayControl);	// �j��

private:
	// �����o�ϐ�
	CAnim2D *m_pControl;		// ����̏��
	D3DXVECTOR3 m_originSize;	// ����̌��̑傫��
	EControl m_contType;		// �\������
	EDisp m_dispType;			// �\���`��
	EState m_state;		// ���
	int m_nCounter;		// ��ԊǗ��J�E���^�[
	float m_fSinScale;	// �g�k����
	float m_fScale;		// �g�嗦
};

#endif	// _PLAY_CONTROL_H_
