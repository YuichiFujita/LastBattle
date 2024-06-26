//============================================================
//
//	便利関数処理 [useful.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "useful.h"

//************************************************************
//	通常関数
//************************************************************
//	便利関数空間
//************************************************************
//============================================================
//	数値の桁数ごとの分解
//============================================================
//	スコア表示の際などに使用
//============================================================
void useful::DivideDigitNum
(
	int *pNumDivide,	// 分解結果の格納配列
	const int nNum,		// 分解する数値
	const int nMaxDigit	// 分解する数字の桁数
)
{
	// 変数を宣言
	int nDigit = 1;	// 各桁の数値の計算用

	for (int nCntDigit = 0; nCntDigit < nMaxDigit; nCntDigit++)
	{ // 桁の最大数分繰り返す

		// 桁数分乗算する
		nDigit *= 10;
	}

	for (int nCntTex = nMaxDigit - 1; nCntTex >= 0; nCntTex--, pNumDivide++)
	{ // 桁数の最大から桁数の最小まで繰り返す

		// 桁数ごとの数値を求める
		*pNumDivide = (nNum % nDigit) / (nDigit / 10);

		// 除算して次の桁数に合わせる
		nDigit /= 10;
	}
}

//============================================================
//	法線の正規化
//============================================================
void useful::NormalizeNormal
(
	const D3DXVECTOR3& rPosLeft,	// 左位置
	const D3DXVECTOR3& rPosCenter,	// 中心位置
	const D3DXVECTOR3& rPosRight,	// 右位置
	D3DXVECTOR3& rNor				// 法線
)
{
	// 変数を宣言
	D3DXVECTOR3 vecLine[2];	// 辺ベクトル

	// 辺ベクトルを求める
	vecLine[0] = rPosLeft - rPosCenter;
	vecLine[1] = rPosRight - rPosCenter;

	// 法線を計算
	rNor.x = (vecLine[0].y * vecLine[1].z) - (vecLine[0].z * vecLine[1].y);
	rNor.y = (vecLine[0].z * vecLine[1].x) - (vecLine[0].x * vecLine[1].z);
	rNor.z = (vecLine[0].x * vecLine[1].y) - (vecLine[0].y * vecLine[1].x);

	// 法線を正規化
	D3DXVec3Normalize(&rNor, &rNor);
}

//============================================================
//	経過時間・X移動量・重力から放物線の位置を求める処理
//============================================================
D3DXVECTOR2 useful::CalcPosParabola
(
	const float fGravity,	// 重力
	const float fMoveX,		// X移動量
	const float fDestPosX,	// 最大X座標
	const float fDestPosY,	// 最大Y座標
	const float fTime,		// 経過時間
	float *pMaxTime,		// 最大経過時間
	float *pMaxPosY			// 最大到達Y座標
)
{
	const float fMaxTime	= fDestPosX / fMoveX;	// 最大経過時間
	const float fHalfTime	= fMaxTime * 0.5f;		// 半分の最大経過時間
	const float fMoveY		= -0.5f * fGravity * fMaxTime;	// Y移動量
	const float fMaxPosY	= 0.5f * fGravity * (fHalfTime * fHalfTime) + fMoveY * fHalfTime;	// 最大到達Y座標

	if (pMaxTime != nullptr)
	{
		// 最大経過時間を保存
		*pMaxTime = fMaxTime;
	}
	if (pMaxPosY != nullptr)
	{
		// 最大到達Y座標を保存
		*pMaxPosY = fMaxPosY;
	}

	// 現在時間の位置を求める
	D3DXVECTOR2 posTime = VEC2_ZERO;
	posTime.x = fMoveX * fTime;
	posTime.y = (0.5f * fGravity * (fTime * fTime) + fMoveY * fTime) * (fDestPosY / fMaxPosY);

	// 現在時間の位置を返す
	return posTime;
}

//============================================================
//	経過時間・X移動量・重力から放物線の移動量を求める処理
//============================================================
D3DXVECTOR2 useful::CalcMoveParabola
(
	const float fGravity,	// 重力
	const float fMoveX,		// X移動量
	const float fDestPosX,	// 最大X座標
	const float fDestPosY,	// 最大Y座標
	const float fTime,		// 経過時間
	float *pMaxTime,		// 最大経過時間
	float *pMaxPosY			// 最大到達Y座標
)
{
	const float fMaxTime	= fDestPosX / fMoveX;	// 最大経過時間
	const float fHalfTime	= fMaxTime * 0.5f;		// 半分の最大経過時間
	const float fMoveY		= -0.5f * fGravity * fMaxTime;	// Y移動量
	const float fMaxPosY	= 0.5f * fGravity * (fHalfTime * fHalfTime) + fMoveY * fHalfTime;	// 最大到達Y座標

	if (pMaxTime != nullptr)
	{
		// 最大経過時間を保存
		*pMaxTime = fMaxTime;
	}
	if (pMaxPosY != nullptr)
	{
		// 最大到達Y座標を保存
		*pMaxPosY = fMaxPosY;
	}

	// 現在時間の移動量を求める
	D3DXVECTOR2 moveTime = VEC2_ZERO;
	moveTime.x = fMoveX;
	moveTime.y = (fGravity * fTime + fMoveY) * (fDestPosY / fMaxPosY);

	// 現在時間の移動量を返す
	return moveTime;
}

//============================================================
//	向きの正規化
//============================================================
void useful::NormalizeRot(float& rRot)
{
	if (rRot > D3DX_PI)
	{ // 向きが 3.14 を超えた場合

		// 向きの正規化
		rRot -= D3DX_PI * 2;
	}
	else if (rRot < -D3DX_PI)
	{ // 向きが -3.14 を超えた場合

		// 向きの正規化
		rRot += D3DX_PI * 2;
	}
}

//============================================================
//	三軸向きの正規化
//============================================================
void useful::Vec3NormalizeRot(D3DXVECTOR3& rRot)
{
	// 全ての向きを正規化
	NormalizeRot(rRot.x);
	NormalizeRot(rRot.y);
	NormalizeRot(rRot.z);
}

//============================================================
//	ベクトルの向き変換
//============================================================
void useful::VecToRot(const D3DXVECTOR3 vec, float *pPhi, float *pTheta)
{
	// 方位角の計算
	*pPhi = atan2f(vec.y, vec.x);

	// 仰角の計算
	*pTheta = atan2f(-vec.z, sqrtf((vec.x * vec.x) + (vec.y * vec.y)));
}

//============================================================
//	向きのベクトル変換
//============================================================
void useful::RotToVec(const float &rPhi, const float &rTheta, D3DXVECTOR3 *pVec)
{
	// ベクトルの計算
	pVec->x = sinf(rTheta) * cosf(rPhi);
	pVec->y = sinf(rTheta) * sinf(rPhi);
	pVec->z = cosf(rTheta);
}

//============================================================
//	ランダム向き取得
//============================================================
float useful::RandomRot(void)
{
	// ランダム向きを返す
	return (float)(rand() % 629 - 314) * 0.01f;
}

//============================================================
//	マトリックスのワールド座標取得
//============================================================
D3DXVECTOR3 useful::GetMtxWorldPosition(const D3DXMATRIX &rMtx)
{
	// マトリックスのワールド座標を返す
	return D3DXVECTOR3(rMtx._41, rMtx._42, rMtx._43);
}

//************************************************************
//	マテリアル空間
//************************************************************
//============================================================
//	白マテリアル取得
//============================================================
D3DXMATERIAL material::White(void)
{
	// 変数を宣言
	D3DXMATERIAL mat;	// 設定用マテリアル

	// マテリアルのメモリクリア
	ZeroMemory(&mat, sizeof(mat));

	// マテリアルの拡散光を白にする
	mat.MatD3D.Diffuse = XCOL_WHITE;

	// マテリアルを返す
	return mat;
}

//============================================================
//	赤マテリアル取得
//============================================================
D3DXMATERIAL material::Red(void)
{
	// 変数を宣言
	D3DXMATERIAL mat;	// 設定用マテリアル

	// マテリアルのメモリクリア
	ZeroMemory(&mat, sizeof(mat));

	// マテリアルの拡散光を赤にする
	mat.MatD3D.Diffuse = XCOL_RED;

	// マテリアルを返す
	return mat;
}

//============================================================
//	緑マテリアル取得
//============================================================
D3DXMATERIAL material::Green(void)
{
	// 変数を宣言
	D3DXMATERIAL mat;	// 設定用マテリアル

	// マテリアルのメモリクリア
	ZeroMemory(&mat, sizeof(mat));

	// マテリアルの拡散光を緑にする
	mat.MatD3D.Diffuse = XCOL_GREEN;

	// マテリアルを返す
	return mat;
}

//============================================================
//	青マテリアル取得
//============================================================
D3DXMATERIAL material::Blue(void)
{
	// 変数を宣言
	D3DXMATERIAL mat;	// 設定用マテリアル

	// マテリアルのメモリクリア
	ZeroMemory(&mat, sizeof(mat));

	// マテリアルの拡散光を青にする
	mat.MatD3D.Diffuse = XCOL_BLUE;

	// マテリアルを返す
	return mat;
}

//============================================================
//	黄マテリアル取得
//============================================================
D3DXMATERIAL material::Yellow(void)
{
	// 変数を宣言
	D3DXMATERIAL mat;	// 設定用マテリアル

	// マテリアルのメモリクリア
	ZeroMemory(&mat, sizeof(mat));

	// マテリアルの拡散光を黄にする
	mat.MatD3D.Diffuse = XCOL_YELLOW;

	// マテリアルを返す
	return mat;
}

//============================================================
//	ダメージ赤マテリアル取得
//============================================================
D3DXMATERIAL material::DamageRed(void)
{
	// 変数を宣言
	D3DXMATERIAL mat;	// 設定用マテリアル

	// マテリアルのメモリクリア
	ZeroMemory(&mat, sizeof(mat));

	// マテリアルの拡散光を設定
	mat.MatD3D.Diffuse = D3DXCOLOR(1.0f, 0.2f, 0.2f, 0.85f);

	// マテリアルの放射光を設定
	mat.MatD3D.Emissive = D3DXCOLOR(0.5f, 0.0f, 0.0f, 1.0f);

	// マテリアルを返す
	return mat;
}

//============================================================
//	発光緑マテリアル取得
//============================================================
D3DXMATERIAL material::GlowGreen(void)
{
	// 変数を宣言
	D3DXMATERIAL mat;	// 設定用マテリアル

	// マテリアルのメモリクリア
	ZeroMemory(&mat, sizeof(mat));

	// マテリアルの拡散光を緑にする
	mat.MatD3D.Diffuse = XCOL_GREEN;

	// マテリアルの放射光を緑にする
	mat.MatD3D.Emissive = XCOL_GREEN;

	// マテリアルを返す
	return mat;
}

//************************************************************
//	2整数座標の構造体メンバ関数
//************************************************************
//============================================================
//	演算子のオーバーロード (比較演算子 ==)
//============================================================
bool POSGRID2::operator==(const POSGRID2& grid) const
{
	if (grid.x == x && grid.y == y)
	{ // 方眼座標がすべて同じ場合

		// 真を返す
		return true;
	}

	// 偽を返す
	return false;
}

//============================================================
//	演算子のオーバーロード (比較演算子 !=)
//============================================================
bool POSGRID2::operator!=(const POSGRID2& grid) const
{
	if (grid.x != x || grid.y != y)
	{ // 方眼座標が一つでも違う場合

		// 真を返す
		return true;
	}

	// 偽を返す
	return false;
}

//************************************************************
//	2整数座標の構造体メンバ関数
//************************************************************
//============================================================
//	演算子のオーバーロード (比較演算子 ==)
//============================================================
bool POSGRID3::operator==(const POSGRID3& grid) const
{
	if (grid.x == x && grid.y == y && grid.z == z)
	{ // 方眼座標がすべて同じ場合

		// 真を返す
		return true;
	}

	// 偽を返す
	return false;
}

//============================================================
//	演算子のオーバーロード (比較演算子 !=)
//============================================================
bool POSGRID3::operator!=(const POSGRID3& grid) const
{
	if (grid.x != x || grid.y != y || grid.z != z)
	{ // 方眼座標が一つでも違う場合

		// 真を返す
		return true;
	}

	// 偽を返す
	return false;
}
