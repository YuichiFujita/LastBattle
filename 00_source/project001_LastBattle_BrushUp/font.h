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
	SFont RegistName		// �t�H���g�o�^ (���O)
	( // ����
		std::string sFontName,	// �t�H���g��
		bool bItalic = false	// �C�^���b�N
	);
	CFontChar::SChar RegistChar	// �t�H���g�����o�^ (���O)
	( // ����
		const UINT uChar,		// �w�蕶��
		std::string sFontName,	// �t�H���g��
		bool bItalic = false	// �C�^���b�N
	);

	// �ÓI�����o�֐�
	static CFont *Create(void);			// ����
	static void Release(CFont *&pFont);	// �j��

private:
	// �����o�֐�
	HRESULT SearchFolderAll(std::string sFolderPath);	// �t�H���_�S����

	// �����o�ϐ�
	std::map<SKey, SFont> m_mapFont;	// �t�H���g�A�z�z��
};

#endif	// _FONT_H_
