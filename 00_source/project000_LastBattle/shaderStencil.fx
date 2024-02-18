//============================================================
//
//	�X�e���V���V�F�[�_�[�̃G�t�F�N�g�t�@�C�� [shaderStencil.fx]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�O���[�o���ϐ��錾
//************************************************************
float4x4 g_mtxWorld	: WORLD;		// ���[���h�}�g���b�N�X
float4x4 g_mtxView	: VIEW;			// �r���[�}�g���b�N�X
float4x4 g_mtxProj	: PROJECTION;	// �v���W�F�N�V�����}�g���b�N�X
texture  g_textureStencil;			// �X�e���V���e�N�X�`��

int g_nRefValue;	// �Q�ƒl
int g_nComparison;	// ��r���Z�q
int g_nOperation;	// ���l����

//************************************************************
//	�T���v���[�錾
//************************************************************
sampler texObject : register(s0);	// �I�u�W�F�N�g�e�N�X�`��
sampler texStencil =	// �X�e���V���e�N�X�`��
sampler_state			// �T���v���[�X�e�[�g
{
	// �Ώۃe�N�X�`��
	Texture = <g_textureStencil>;	// �X�e���V���e�N�X�`��

	// �g�k�E�~�b�v�}�b�v
	Filter = MIN_MAG_MIP_LINEAR;	// ���`�⊮

	// �e�N�X�`���A�h���b�V���O���[�h
	AddressU = Clamp;	// U�����̒[���������΂�
	AddressV = Clamp;	// V�����̒[���������΂�
};

//************************************************************
//	�\���̒�`
//************************************************************
// ���_�V�F�[�_�[�o�͏��
struct VS_OUTPUT
{
	float4 pos : POSITION;	// ���_���W
	float2 tex : TEXCOORD0;	// �e�N�Z�����W
};

//************************************************************
//	�}�N����`
//************************************************************
#define MIN_REF (0)		// �ŏ��Q�ƒl
#define MAX_REF (9)		// �ő�Q�ƒl
#define NUM_REF (10)	// ���v�Q�ƒl

//************************************************************
//	�O���[�o���֐�
//************************************************************
//============================================================
//	���_���W�̎ˉe�ϊ�
//============================================================
float4 TransVertex
(
	in float4	inPos		: POSITION,		// ���_���W
	in float4x4	inMtxWorld	: WORLD,		// ���[���h�}�g���b�N�X
	in float4x4	inMtxView	: VIEW,			// �r���[�}�g���b�N�X
	in float4x4	inMtxProj	: PROJECTION	// �v���W�F�N�V�����}�g���b�N�X
)
{
	// ���_���W���ˉe�ϊ�
	inPos = mul(inPos, inMtxWorld);
	inPos = mul(inPos, inMtxView);
	inPos = mul(inPos, inMtxProj);

	// �ϊ���̒��_���W��Ԃ�
	return inPos;
}

//============================================================
//	���_�V�F�[�_�[
//============================================================
void VS
(
	in	float4		inPos : POSITION,	// ���_���W
	in	float2		inTex : TEXCOORD0,	// �e�N�Z�����W
	out	VS_OUTPUT	outVertex			// ���_���
)
{
	// ���[�J�����_���W���ˉe�ϊ����ݒ�
	outVertex.pos = TransVertex(inPos, g_mtxWorld, g_mtxView, g_mtxProj);

	// �e�N�Z�����W��ݒ�
	outVertex.tex = inTex;
}

//============================================================
//	�s�N�Z���V�F�[�_�[
//============================================================
void PS
(
	in	VS_OUTPUT	inVertex,		// ���_���
	out	float4		outCol : COLOR0	// �s�N�Z���F
)
{
	bool bVsSuccess = false;	// ��r���ʃt���O
	int nPixcelValue = 0;		// �s�N�Z���̐ݒ�ςݎQ�ƒl

	// �s�N�Z���̐F����������
	outCol = float4(1.0f, 0.0f, 0.0f, 1.0f);

	// �s�N�Z���ɐݒ肳��Ă���Q�ƒl���擾
	nPixcelValue = (int)(((float)NUM_REF / 1.0f) * tex2D(texStencil, inVertex.tex).r);

	// ��r���Z�q���Ƃ̏���
	if (g_nComparison == 0)
	{ // �K�����s����ꍇ

		// ���s������
		bVsSuccess = false;
	}
	else if (g_nComparison == 1)
	{ //  <�Ő�������ꍇ

		if (nPixcelValue < g_nRefValue)
		{
			// ����������
			bVsSuccess = true;
		}
	}
	else if (g_nComparison == 2)
	{ // <=�Ő�������ꍇ

		if (nPixcelValue <= g_nRefValue)
		{
			// ����������
			bVsSuccess = true;
		}
	}
	else if (g_nComparison == 3)
	{ // ==�Ő�������ꍇ

		if (nPixcelValue == g_nRefValue)
		{
			// ����������
			bVsSuccess = true;
		}
	}
	else if (g_nComparison == 4)
	{ // !=�Ő�������ꍇ

		if (nPixcelValue != g_nRefValue)
		{
			// ����������
			bVsSuccess = true;
		}
	}
	else if (g_nComparison == 5)
	{ //  >�Ő�������ꍇ

		if (nPixcelValue > g_nRefValue)
		{
			// ����������
			bVsSuccess = true;
		}
	}
	else if (g_nComparison == 6)
	{ // >=�Ő�������ꍇ

		if (nPixcelValue >= g_nRefValue)
		{
			// ����������
			bVsSuccess = true;
		}
	}
	else if (g_nComparison == 7)
	{ // �K����������ꍇ

		// ����������
		bVsSuccess = true;
	}

	if (!bVsSuccess)
	{ // ��r���ʂ����s�̏ꍇ

		// �`��𒆎~����
		//discard;
	}
	else
	{ // ��r���ʂ������̏ꍇ

		// ���l���삲�Ƃ̏���
		float fSetValue = 0.0f;	// �ݒ肷��s�N�Z���Q�ƒl
		if (g_nOperation == 0)
		{ // �Q�ƒl�ɒu������ꍇ
		
			fSetValue = ((1.0f / (float)NUM_REF) * g_nRefValue);
		}
		else if (g_nOperation == 1)
		{ // �C���N�������g����ꍇ

			// �s�N�Z���̐ݒ�ςݎQ�ƒl�����Z
			fSetValue = nPixcelValue;
			fSetValue += (1.0f / (float)NUM_REF);
			saturate(fSetValue);
		}
		else if (g_nOperation == 2)
		{ // �f�N�������g����ꍇ

			// �s�N�Z���̐ݒ�ςݎQ�ƒl�����Z
			fSetValue  = nPixcelValue;
			fSetValue -= (1.0f / (float)NUM_REF);
			saturate(fSetValue);
		}

		// �s�N�Z���̐F����ݒ�
		outCol = float4(fSetValue, fSetValue, fSetValue, 1.0f);
	}
}

//============================================================
//	�e�N�j�b�N�֐�
//============================================================
technique TShader
{
	pass P0
	{
		VertexShader = compile vs_1_1 VS();
		PixelShader  = compile ps_2_0 PS();
	}
}
