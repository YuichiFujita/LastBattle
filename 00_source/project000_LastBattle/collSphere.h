//============================================================
//
//	�~����w�b�_�[ [collSphere.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _COLL_SPHERE_H_
#define _COLL_SPHERE_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include <vector>

//************************************************************
//	�O���錾
//************************************************************
class CObject;	// �I�u�W�F�N�g�N���X

#if _DEBUG
class CDebugCollSphere;	// ���̔���f�o�b�O�\���N���X
#endif	// _DEBUG

//************************************************************
//	�N���X��`
//************************************************************
// �~����N���X
class CCollSphere
{
public:
	// �R���X�g���N�^
	CCollSphere();

	// �f�X�g���N�^
	~CCollSphere();

	// ������\����
	struct SInfo
	{
		D3DXVECTOR3 offset;	// ����ʒu�I�t�Z�b�g
		float fRadius;		// ���蔼�a

#if _DEBUG
		CDebugCollSphere *pVisual;	// ����\��
#endif	// _DEBUG
	};

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	D3DXVECTOR3 CalcWorldPosition(const int nID) const;	// ����ʒu�v�Z
	void SetInfo(const SInfo& rInfo, const int nID);	// ������ݒ�
	SInfo GetInfo(const int nID) const;					// ������擾
	void SetVector(const std::vector<SInfo>& rVector);	// �z��ݒ�
	std::vector<SInfo> GetVector(void) const;			// �z��擾

	void AddColl	// ����ǉ�
	( // ����
		const D3DXVECTOR3& rOffset,	// ����ʒu�I�t�Z�b�g
		const float fRadius			// ���蔼�a
	);
	void SubColl(const int nID);	// ����폜

	// �ÓI�����o�֐�
	static CCollSphere *Create(CObject *pParent);		// ����
	static void Release(CCollSphere *&prCollSphere);	// �j��

	static void SetVisual(const bool bVisual)	{ m_bVisualColl = bVisual; }	// �����ڕ\���ݒ�
	static bool IsVisual(void)					{ return m_bVisualColl; }		// �����ڕ\���擾

private:
	// �ÓI�����o�ϐ�
	static bool m_bVisualColl;	// ���茩���ڕ\���t���O

	// �����o�ϐ�
	CObject *m_pParent;	// �e�I�u�W�F�N�g
	std::vector<SInfo> m_coll;	// ������
};

#endif	// _COLL_SPHERE_H_