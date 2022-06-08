#pragma once

#include <math/Matrix4.h>
#include <math/Vector3.h>

class Affin {

public:
	//メンバ関数
	Affin();
	~Affin();

	Matrix4 Initialize();
	Matrix4 Scale(Vector3 scale);
	Matrix4 Rotation(Vector3 rotation, int X_1_Y_2_Z_3_XYZ_6);
	Matrix4 Move(Vector3 move);

private://メンバ変数
	

public://メンバ変数


};