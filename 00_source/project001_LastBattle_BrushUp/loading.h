//============================================================
//
//	���[�f�B���O�w�b�_�[ [loading.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _LOADING_H_
#define _LOADING_H_

//************************************************************
//	�O���錾
//************************************************************
class CAnim2D;	// �A�j���[�V����2D�N���X

//************************************************************
//	�N���X��`
//************************************************************
// ���[�f�B���O�N���X
class CLoading
{
public:
	// ���[�f�B���O��ԗ�
	enum ELoading
	{
		LOAD_NONE = 0,	// �������Ă��Ȃ����
		LOAD_UPDATE,	// ���[�h�X�V���
		LOAD_END,		// ���[�h�I�����
		LOAD_MAX		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CLoading();

	// �f�X�g���N�^
	~CLoading();

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��
	void Set(std::function<HRESULT(bool*)> funcLoad);	// ���[�h�J�n�ݒ�
	ELoading GetState(void) const { return m_state; }	// ���[�h��Ԏ擾

	// �ÓI�����o�֐�
	static CLoading *Create(void);				// ����
	static void Release(CLoading *&pLoading);	// �j��

private:
	// �����o�ϐ�
	std::thread m_funcLoad;	// �Ǎ������X���b�h
	CAnim2D *m_pLoad;		// ���[�h��ʏ��
	ELoading m_state;		// ���[�h���
	bool m_bEnd;			// ���[�h�I����
};

#endif	// _LOADING_H_
