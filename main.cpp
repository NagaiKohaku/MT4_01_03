#include <Novice.h>

#include "Vector3.h"

#include "cmath"

const char kWindowTitle[] = "LE2C_18_ナガイコハク_MT4_01_03_Quaternion";

//Vector3の外積
Vector3 Cross(Vector3 v1, Vector3 v2) {
	return { v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x };
}

//クオータニオン
struct Quaternion {
	Vector3 v;
	float w;
};

//積法
Quaternion Multiply(Quaternion lns, Quaternion rhs) {
	Quaternion result;

	result.w = lns.w * rhs.w - Dot(lns.v, rhs.v);

	result.v = Cross(lns.v, rhs.v) + lns.v * rhs.w + rhs.v * lns.w;

	return result;
}

//乗法単位元
Quaternion IdentityQuaternion() {
	Quaternion result;

	result.w = 1.0f;

	result.v = Vector3(0.0f, 0.0f, 0.0f);

	return result;
}

//共役
Quaternion Conjugate(Quaternion quaternion) {
	Quaternion result;

	result.w = quaternion.w;

	result.v = Vector3(-quaternion.v.x, -quaternion.v.y, -quaternion.v.z);

	return result;
}

//ノルム
float Norm(Quaternion quaternion) {

	float sqW = quaternion.w * quaternion.w;
	float sqX = quaternion.v.x * quaternion.v.x;
	float sqY = quaternion.v.y * quaternion.v.y;
	float sqZ = quaternion.v.z * quaternion.v.z;

	return std::sqrtf(sqW + sqX + sqY + sqZ);
}

//正規化
Quaternion Normalize(Quaternion quaternion) {
	float mag = Norm(quaternion);

	if (mag != 0.0f) {
		return Quaternion({ quaternion.v.x / mag,quaternion.v.y / mag,quaternion.v.z / mag }, quaternion.w / mag);
	}

	return quaternion;
}

//逆Quaternion
Quaternion Inverse(Quaternion quaternion) {
	Quaternion result;

	Quaternion conjugate = Conjugate(quaternion);

	result.v.x = conjugate.v.x / (Norm(quaternion) * Norm(quaternion));
	result.v.y = conjugate.v.y / (Norm(quaternion) * Norm(quaternion));
	result.v.z = conjugate.v.z / (Norm(quaternion) * Norm(quaternion));
	result.w = conjugate.w / (Norm(quaternion) * Norm(quaternion));

	return result;
}


static const int kRowHeight = 20;
static const int kColumnWidth = 60;

//Quaternionの出力関数
void QuaternionScreenPrintf(int x, int y, const Quaternion& quaternion, const char* label) {

	Novice::ScreenPrintf(x, y, "%.2f", quaternion.v.x);

	Novice::ScreenPrintf(x + kColumnWidth * 1, y, "%.2f", quaternion.v.y);

	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%.2f", quaternion.v.z);

	Novice::ScreenPrintf(x + kColumnWidth * 3, y, "%.2f", quaternion.w);

	Novice::ScreenPrintf(x + kColumnWidth * 5 - kRowHeight, y, "%s", ":");

	Novice::ScreenPrintf(x + kColumnWidth * 5, y, "%s", label);
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	const int kWindowWidth = 1280;
	const int kWindowHeight = 720;
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	Quaternion q1 = { {2.0f,3.0f,4.0f},1.0f };
	Quaternion q2 = { {1.0f,3.0f,5.0f},2.0f };

	Quaternion identity = IdentityQuaternion();
	Quaternion conj = Conjugate(q1);
	Quaternion inv = Inverse(q1);
	Quaternion normal = Normalize(q1);
	Quaternion mul1 = Multiply(q1, q2);
	Quaternion mul2 = Multiply(q2, q1);
	float norm = Norm(q1);

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		QuaternionScreenPrintf(0, kRowHeight * 0, identity, "Identity");
		QuaternionScreenPrintf(0, kRowHeight * 1, conj, "Conjugate");
		QuaternionScreenPrintf(0, kRowHeight * 2, inv, "Inverse");
		QuaternionScreenPrintf(0, kRowHeight * 3, normal, "Normalize");
		QuaternionScreenPrintf(0, kRowHeight * 4, mul1, "Multiply(q1,q2)");
		QuaternionScreenPrintf(0, kRowHeight * 5, mul2, "Multiply(q2,q1)");

		Novice::ScreenPrintf(0, kRowHeight * 6, "%.2f", norm);
		Novice::ScreenPrintf(kColumnWidth * 5 - kRowHeight, kRowHeight * 6, "%s", ":");
		Novice::ScreenPrintf(kColumnWidth * 5, kRowHeight * 6, "%s", "Norm");

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
