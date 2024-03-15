//============================================================
//
//	���f���w�b�_�[ [model.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _MODEL_H_
#define _MODEL_H_

//************************************************************
//	�N���X��`
//************************************************************
// ���f���N���X
class CModel
{
public:
	// �R���X�g���N�^
	CModel();

	// �f�X�g���N�^
	~CModel();

	// ���f���\����
	struct SModel
	{
		LPD3DXMESH		pMesh;			// ���b�V�� (���_���) �ւ̃|�C���^
		LPD3DXBUFFER	pBuffMat;		// �}�e���A���ւ̃|�C���^
		DWORD			dwNumMat;		// �}�e���A���̐�
		D3DXVECTOR3		vtxMin;			// �ŏ��̒��_���W
		D3DXVECTOR3		vtxMax;			// �ő�̒��_���W
		D3DXVECTOR3		size;			// �傫��
		float			fRadius;		// ���a
		int				*pTextureID;	// �e�N�X�`���C���f�b�N�X
	};

	// �}�b�v���\����
	struct SMapInfo
	{
		SModel modelData;			// ���f�����
		std::string sFilePassName;	// �t�@�C���p�X��
	};

	// �����o�֐�
	int Regist(const char *pFileName);	// ���f���o�^
	SModel *GetInfo(const int nID);		// ���f�����擾

	// �ÓI�����o�֐�
	static CModel *Create(void);			// ����
	static void Release(CModel *&pModel);	// �j��

private:
	// �����o�֐�
	HRESULT Load(void);	// ���f������
	void Unload(void);	// ���f���j��
	HRESULT LoadXFileModel(SMapInfo *pMapInfo, const char *pFileName);	// x�t�@�C���̓Ǎ�
	HRESULT LoadTextureModel(SMapInfo *pMapInfo);	// �e�N�X�`���̓Ǎ�
	HRESULT SetCollisionModel(SMapInfo *pMapInfo);	// �����蔻��̍쐬
	HRESULT LoadAll(std::string sFolderPath);		// ���f���S�Ǎ�

	// �����o�ϐ�
	std::map<int, SMapInfo> m_mapModel;	// ���f���A�z�z��

	// �ÓI�����o�ϐ�
	static int m_nNumAll;	// ���f���̑���
};

#endif	// _MODEL_H_
