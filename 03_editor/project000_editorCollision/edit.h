//============================================================
//
//	�G�f�B�b�g�w�b�_�[ [edit.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _EDIT_H_
#define _EDIT_H_

//************************************************************
//	�N���X��`
//************************************************************
// �G�f�B�b�g�N���X
class CEdit
{
public:
	// ���[�h��
	enum EMode
	{
		MODE_PLAY = 0,	// �v���C���[�h
		MODE_COLL_SET,	// ����ݒ胂�[�h
		MODE_MAX		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CEdit();

	// �f�X�g���N�^
	~CEdit();

	// ���z�֐�
	virtual HRESULT Init(void);	// ������
	virtual void Uninit(void);	// �I��
	virtual void Update(void);	// �X�V
	virtual void Draw(void);	// �`��
	virtual void DrawEditControl(void);	// �G�f�B�b�g����\��
	virtual void DrawEditData(void);	// �G�f�B�b�g���\��

	// �����o�֐�
	EMode GetMode(void);	// �G�f�B�b�g���[�h�擾

	// �ÓI�����o�֐�
	static CEdit *Create(const EMode mode);	// ����
	static void Release(CEdit *&prEdit);	// �j��
	static void NextMode(CEdit *&prEdit);	// ���̃��[�h�ݒ�

private:
	// �����o�ϐ�
	EMode m_mode;	// �G�f�B�b�g���[�h
};

#endif	// _EDIT_H_
