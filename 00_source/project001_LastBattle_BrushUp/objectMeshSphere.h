//============================================================
//
//	�I�u�W�F�N�g���b�V���X�t�B�A�w�b�_�[ [objectMeshSphere.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _OBJECT_MESHSPHERE_H_
#define _OBJECT_MESHSPHERE_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object.h"

//************************************************************
//	�O���錾
//************************************************************
class CObjectMeshDome;	// �I�u�W�F�N�g���b�V���h�[��

//************************************************************
//	�N���X��`
//************************************************************
// �I�u�W�F�N�g���b�V���X�t�B�A�N���X
class CObjectMeshSphere : public CObject
{
public:
	// ������
	enum EDome
	{
		DOME_TOP = 0,	// �㔼��
		DOME_BOTTOM,	// ������
		DOME_MAX		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	explicit CObjectMeshSphere(const CObject::ELabel label = LABEL_NONE, const CObject::EDim dimension = DIM_3D, const int nPriority = object::DEFAULT_PRIO);

	// �f�X�g���N�^
	~CObjectMeshSphere() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(CShader *pShader = nullptr) override;			// �`��
	void BindTexture(const int nTextureID) override;		// �e�N�X�`������ (�C���f�b�N�X)
	void BindTexture(const char *pTexturePass) override;	// �e�N�X�`������ (�p�X)
	void SetVec3Position(const D3DXVECTOR3& rPos) override;	// �ʒu�ݒ�
	D3DXVECTOR3 GetVec3Position(void) const override;		// �ʒu�擾
	void SetVec3Rotation(const D3DXVECTOR3& rRot) override;	// �����ݒ�
	D3DXVECTOR3 GetVec3Rotation(void) const override;		// �����擾
	void SetColor(const D3DXCOLOR& rCol) override;	// �F�ݒ�
	D3DXCOLOR GetColor(void) const override;		// �F�擾
	void SetRadius(const float fRadius) override;	// ���a�ݒ�
	float GetRadius(void) const override;			// ���a�擾
	void SetPriority(const int nPrio) override;		// �D�揇�ʐݒ�

	// �ÓI�����o�֐�
	static CObjectMeshSphere *Create	// ����
	( // ����
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot,	// ����
		const D3DXCOLOR& rCol,		// �F
		const POSGRID2& rPart,		// ������
		const POSGRID2& rTexPart,	// �e�N�X�`��������
		const float fRadius			// ���a
	);

	// �����o�֐�
	HRESULT SetPattern(const POSGRID2& rPart);		// �������ݒ�
	POSGRID2 GetPattern(void) const;				// �������擾
	void SetTexPattern(const POSGRID2& rTexPart);	// �e�N�X�`���������ݒ�
	POSGRID2 GetTexPattern(void) const;				// �e�N�X�`���������擾

private:
	// �I�[�o�[���C�h�֐�
	void Release(void) override;	// �j��

	// �����o�ϐ�
	CObjectMeshDome *m_apDome[DOME_MAX];	// �����̏��
	POSGRID2 m_part;	// ������
	POSGRID2 m_texPart;	// �e�N�X�`��������
};

#endif	// _OBJECT_MESHSPHERE_H_
