//============================================================
//
//	�t�H���g�w�b�_�[ [font.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _FONT_H_
#define _FONT_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "fontChar.h"

//************************************************************
//	�N���X��`
//************************************************************
// �t�H���g�N���X
class CFont
{
public:
	// �R���X�g���N�^
	CFont();

	// �f�X�g���N�^
	~CFont();

	// �L�[�\����
	struct SKey
	{
		// �R���X�g���N�^
		SKey(std::string FontName, bool Italic) :
		sFontName(FontName), bItalic(Italic) {}

		// ��r���Z�q�I�[�o�[���[�h
		bool operator<(const SKey& key) const
		{
			// �����܂Ƃ߂Ĕ�r
			return std::tie(sFontName, bItalic) < std::tie(key.sFontName, key.bItalic);
		}

		std::string sFontName;	// �t�H���g��
		bool bItalic;			// �C�^���b�N
	};

	// �t�H���g�\����
	struct SFont
	{
		// �R���X�g���N�^
		SFont() { memset(this, 0, sizeof(*this)); }

		CFontChar *pFontChar;	// �t�H���g�����C���X�^���X
		HFONT pFont;			// �t�H���g�ւ̃|�C���^
	};

	// �����o�֐�
	HRESULT Init(void);		// �t�H���g������
	void Uninit(void);		// �t�H���g�I��
	HRESULT LoadAll(void);	// �t�H���g�S�Ǎ�
	HRESULT Load(const std::string &rFilePass);	// �t�H���g�Ǎ�

	SFont Regist(const std::string &rFontName, bool bItalic = false);	// �t�H���g�o�^
	CFontChar::SChar RegistChar	// �t�H���g�����o�^ (���O)
	( // ����
		const wchar_t wcChar,			// �w�蕶��
		const std::string &rFontName,	// �t�H���g��
		bool bItalic = false			// �C�^���b�N
	);

	// �ÓI�����o�֐�
	static CFont *Create(void);			// ����
	static void Release(CFont *&pFont);	// �j��

private:
	// �����o�֐�
	HRESULT SearchFolderAll(const std::string &rFolderPath);	// �t�H���_�S����

	// �����o�ϐ�
	std::map<SKey, SFont> m_mapFont;		// �t�H���g�A�z�z��
	std::vector<std::string> m_vecFilePass;	// �Ǎ��ς݃t�@�C���p�X
};

#endif	// _FONT_H_
