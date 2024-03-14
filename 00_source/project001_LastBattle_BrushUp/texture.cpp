//============================================================
//
//	�e�N�X�`������ [texture.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "texture.h"
#include "manager.h"
#include "renderer.h"

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
int CTexture::m_nNumAll = 0;	// �e�N�X�`���̑���

//************************************************************
//	�e�N���X [CTexture] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CTexture::CTexture()
{
	// �e�N�X�`���A�z�z����N���A
	m_mapTexture.clear();
}

//============================================================
//	�f�X�g���N�^
//============================================================
CTexture::~CTexture()
{

}

//============================================================
//	�e�N�X�`����������
//============================================================
HRESULT CTexture::Load(void)
{
	// �e�N�X�`���A�z�z���������
	m_mapTexture.clear();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�e�N�X�`���j������
//============================================================
void CTexture::Unload(void)
{
	for (auto& rMap : m_mapTexture)
	{ // �e�N�X�`���̗v�f�����J��Ԃ�

		// �e�N�X�`���̔j��
		SAFE_RELEASE(rMap.second.textureData.pTexture);
	}

	// �e�N�X�`���A�z�z����N���A
	m_mapTexture.clear();
}

//============================================================
//	�e�N�X�`���o�^���� (����)
//============================================================
int CTexture::Regist(const SInfo info)
{
	// �ϐ���錾
	HRESULT  hr;			// �ُ�I���̊m�F�p
	SMapInfo tempMapInfo;	// �}�b�v���
	int nID = m_nNumAll;	// �e�N�X�`���Ǎ��ԍ�

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// �f�o�C�X�̃|�C���^

	// ��̃e�N�X�`���𐶐�
	hr = D3DXCreateTexture
	( // ����
		pDevice,		// Direct3D�f�o�C�X
		info.Width,		// �e�N�X�`������
		info.Height,	// �e�N�X�`���c��
		info.MipLevels,	// �~�b�v�}�b�v���x��
		info.Usage,		// �����E�m�ۃI�v�V����
		info.Format,	// �s�N�Z���t�H�[�}�b�g
		info.Pool,		// �i�[������
		&tempMapInfo.textureData.pTexture	// �e�N�X�`���ւ̃|�C���^
	);
	if (FAILED(hr))
	{ // �e�N�X�`���̐����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return NONE_IDX;
	}

	// �t�@�C���p�X����ۑ�
	tempMapInfo.sFilePassName = NONE_STRING;	// �Ǎ��ł͂Ȃ��̂Ńp�X����

	// �e�N�X�`������ۑ�
	m_mapTexture.insert(std::make_pair(m_nNumAll, tempMapInfo));

	// �e�N�X�`���̑��������Z
	m_nNumAll++;

	// �ǂݍ��񂾃e�N�X�`���̔z��ԍ���Ԃ�
	return nID;
}

//============================================================
//	�e�N�X�`���o�^���� (�p�X)
//============================================================
int CTexture::Regist(const char *pFileName)
{
	// �ϐ���錾
	SMapInfo tempMapInfo;	// �}�b�v���
	int nID = m_nNumAll;	// �e�N�X�`���Ǎ��ԍ�

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// �f�o�C�X�̃|�C���^

	if (pFileName != nullptr)
	{ // �|�C���^���g�p����Ă���ꍇ

		int nCntTexture = 0;	// �e�N�X�`���ԍ�
		for (const auto& rMap : m_mapTexture)
		{ // �e�N�X�`���̗v�f�����J��Ԃ�

			if (rMap.second.sFilePassName.compare(pFileName) == 0)
			{ // �����񂪈�v�����ꍇ

				// ���łɓǂݍ���ł���e�N�X�`���̔z��ԍ���Ԃ�
				return nCntTexture;
			}

			// ���̃e�N�X�`���ԍ��ɂ���
			nCntTexture++;
		}

		// �e�N�X�`���̓ǂݍ���
		if (FAILED(D3DXCreateTextureFromFile(pDevice, pFileName, &tempMapInfo.textureData.pTexture)))
		{ // �e�N�X�`���̓ǂݍ��݂Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return NONE_IDX;
		}

		// �t�@�C���p�X����ۑ�
		tempMapInfo.sFilePassName = pFileName;

		// �e�N�X�`������ۑ�
		m_mapTexture.insert(std::make_pair(m_nNumAll, tempMapInfo));

		// �e�N�X�`���̑��������Z
		m_nNumAll++;

		// �ǂݍ��񂾃e�N�X�`���̔z��ԍ���Ԃ�
		return nID;
	}
	else
	{ // �|�C���^���g�p����Ă��Ȃ��ꍇ

		// �e�N�X�`����g�p��Ԃ�
		return NONE_IDX;
	}
}

//============================================================
//	�e�N�X�`�����̎擾����
//============================================================
CTexture::STexture CTexture::GetInfo(const int nID)
{
	int nArray = (int)m_mapTexture.size();	// �z��v�f��
	if (nID > NONE_IDX && nID < nArray)
	{ // �e�N�X�`��������ꍇ

		// �����̃e�N�X�`������Ԃ�
		return m_mapTexture.find(nID)->second.textureData;
	}
	else
	{ // �e�N�X�`�����Ȃ��ꍇ

		// �C���f�b�N�X�G���[
		assert(false);

		if (nArray > 0)
		{ // �e�N�X�`������������Ă���ꍇ

			// �擪�e�N�X�`����Ԃ�
			return m_mapTexture.find(0)->second.textureData;
		}
		else
		{ // �e�N�X�`��������Ȃ��ꍇ

			// ��̃e�N�X�`������Ԃ�
			STexture tempTexture;
			memset(&tempTexture, 0, sizeof(tempTexture));
			return tempTexture;
		}
	}
}

//============================================================
//	�e�N�X�`���|�C���^�̎擾����
//============================================================
LPDIRECT3DTEXTURE9 CTexture::GetPtr(const int nID)
{
	if (nID >= 0 && nID < m_nNumAll)
	{ // �����̃C���f�b�N�X���͈͓��̏ꍇ

		if (nID > NONE_IDX && nID < (int)m_mapTexture.size())
		{ // �e�N�X�`��������ꍇ

			// �����̃e�N�X�`���|�C���^��Ԃ�
			return m_mapTexture.find(nID)->second.textureData.pTexture;
		}
		else
		{ // �e�N�X�`�����Ȃ��ꍇ

			// nullptr��Ԃ�
			assert(false);
			return nullptr;
		}
	}
	else if (nID == NONE_IDX)
	{ // �����̃C���f�b�N�X�� -1�̏ꍇ

		// nullptr��Ԃ�
		return nullptr;
	}
	else
	{ // �����̃C���f�b�N�X���g�p�s�̏ꍇ

		// nullptr��Ԃ�
		assert(false);
		return nullptr;
	}
}

//============================================================
//	��������
//============================================================
CTexture *CTexture::Create(void)
{
	// �e�N�X�`���̐���
	CTexture *pTexture = new CTexture;
	if (pTexture == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �e�N�X�`���̓Ǎ�
		if (FAILED(pTexture->Load()))
		{ // �e�N�X�`���Ǎ��Ɏ��s�����ꍇ

			// �e�N�X�`���̔j��
			SAFE_DELETE(pTexture);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pTexture;
	}
}

//============================================================
//	�j������
//============================================================
void CTexture::Release(CTexture *&prTexture)
{
	// �e�N�X�`���̔j��
	assert(prTexture != nullptr);
	prTexture->Unload();

	// �������J��
	SAFE_DELETE(prTexture);
}
