//============================================================
//
//	�V�[���w�b�_�[ [scene.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _SCENE_H_
#define _SCENE_H_

//************************************************************
//	�O���錾
//************************************************************
class CStage;	// �X�e�[�W�N���X
class CPlayer;	// �v���C���[�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �V�[���N���X
class CScene
{
public:
	// ���[�h��
	enum EMode
	{
		MODE_GAME = 0,	// �Q�[�����
		MODE_MAX		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	explicit CScene(const EMode mode);

	// �f�X�g���N�^
	virtual ~CScene();

	// �������z�֐�
	virtual void Draw(void)	= 0;	// �`��

	// ���z�֐�
	virtual HRESULT Init(void);	// ������
	virtual void Uninit(void);	// �I��
	virtual void Update(void);	// �X�V

	// �ÓI�����o�֐�
	static CScene *Create(EMode mode);		// ����
	static void Release(CScene *&prScene);	// �j��

	static CStage  *GetStage(void);		// �X�e�[�W�擾
	static CPlayer *GetPlayer(void);	// �v���C���[�擾

	// �����o�֐�
	void SetMode(const EMode mode);	// ���[�h�ݒ�
	EMode GetMode(void) const;		// ���[�h�擾

private:
	// �ÓI�����o�ϐ�
	static CStage  *m_pStage;	// �X�e�[�W�̏��
	static CPlayer *m_pPlayer;	// �v���C���[�̏��

	// �����o�ϐ�
	EMode m_mode;	// ���[�h
};

#endif	// _SCENE_H_