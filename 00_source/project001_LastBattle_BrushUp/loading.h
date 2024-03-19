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
//	�C���N���[�h�t�@�C��
//************************************************************
#include <thread>

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

	// �������֐��|�C���^�^�G�C���A�X��`
	typedef HRESULT(*AFuncInit)(void);

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Set(AFuncInit *pFuncInit);	// ���[�h�ݒ�
	ELoading GetState(void) const;	// ���[�h��Ԏ擾

	// �ÓI�����o�֐�
	static CLoading *Create(void);				// ����
	static void Release(CLoading *&pLoading);	// �j��

	std::thread m_funcInit;	// �����������X���b�h

private:
	// �����o�ϐ�
	CAnim2D *m_pLoad;		// ���[�h��ʏ��
	ELoading m_stateLoad;	// ���[�h���
};

#endif	// _LOADING_H_
