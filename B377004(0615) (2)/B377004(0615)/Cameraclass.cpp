////////////////////////////////////////////////////////////////////////////////
//	Filename: cameraclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "cameraclass.h"

CameraClass::CameraClass()
{
	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;

	position.x = 0;
	position.y = 0;
	position.z = 0;
	Speed = 0.0f;
	vibration = 0.0;
	Accelation = 0.0f;
	Hurt = false;
}


D3DXMATRIX CameraClass::GetReflectionViewMatrix()
{
	return m_reflectionViewMatrix;
}
CameraClass::CameraClass(const CameraClass& other)
{
}


CameraClass::~CameraClass()
{
}

void CameraClass::Move(int KeyNum, double RotationAngle, float frameTime) {
	if (!Hurt) {
		switch (KeyNum) {
		case 1:
			/*m_positionZ += (float)sin(D3DX_PI*0.5 + RotationAngle);
			m_positionX -= (float)cos(D3DX_PI*0.5 + RotationAngle);*/
			Speed += 1;
			if ((Speed / frameTime) > 15)
				Speed = frameTime * 15;

			m_positionX += sinf(Angle) / (frameTime / (Speed / frameTime));
			m_positionZ += cosf(Angle) / (frameTime / (Speed / frameTime));
			vibration = 0.0f;
			SetAccelation(Speed);
			break;
		case 2:
			m_positionZ += (float)sin(double(Angle));
			m_positionX -= (float)cos(double(Angle));

			break;
		case 3:
			m_positionZ -= (float)sin(D3DX_PI*0.5 + double(Angle));
			m_positionX += (float)cos(D3DX_PI*0.5 + double(Angle));

			break;
		case 4:

			m_positionZ -= (float)sin(double(Angle));
			m_positionX += (float)cos(double(Angle));
			break;

		case 5:
			m_positionY += 1;
			break;

		case 7:
			m_positionY -= 1;
			break;
		case 0:
			vibration += 0.01;
			m_positionY += sinf(vibration * 10.0f)*powf(0.5f, vibration) / frameTime;
			Speed = 0.0f;
			SetAccelation(Speed);
		}
	}
	else {
		m_positionX -= sinf(Angle) / (frameTime / (Speed / frameTime));
		m_positionZ -= cosf(Angle) / (frameTime / (Speed / frameTime));
		
	}
}

float CameraClass::GetAccelation() {
	return Accelation;
}

void CameraClass::SetAccelation(float Speed) {
	 Accelation = Speed;
}


void CameraClass::SetPosition(float x, float y, float z)
{

	
		m_positionX += (float)cos(double(Angle));
	//m_positionY += (double)sin(double(Angle));
	m_positionX += x;
	m_positionY += y;
	m_positionZ += z-10;
	return;
}


void CameraClass::SetRotation(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;

	Angle = m_rotationY * 3.0174532925f;
	return;
}

D3DXVECTOR3 CameraClass::GetPosition()
{
	return D3DXVECTOR3(m_positionX, m_positionY, m_positionZ);
}


D3DXVECTOR3 CameraClass::GetRotation()
{
	return D3DXVECTOR3(m_rotationX, m_rotationY, m_rotationZ);
}
float CameraClass::GetAngle()
{
	return Angle;
}

void CameraClass::Render()
{
	D3DXVECTOR3 up, lookAt;
	float yaw, pitch, roll;
	D3DXMATRIX rotationMatrix;

	//	Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f; 
	up.z = 0.0f;
	//
	//if (position.x != m_positionX || position.y != m_positionY ||
	//	position.z != m_positionZ) {
	//	position.x = m_positionX;
	//	position.y = m_positionY;
	//	position.z = m_positionZ;

	//	D3DXMatrixTranslation(&rotationMatrix, -m_positionX, -m_positionY, -m_positionZ);

	//}
	//	Setup the position of the camera in the world.
	
	position.x = m_positionX;
	position.y = m_positionY;
	position.z = m_positionZ;

	//	Setup where the camera is looking by default.
	lookAt.x = 0.0f;// -m_positionX;
	lookAt.y = 0.0f;// -m_positionY;
	lookAt.z = 1.0f;// -m_positionZ;

	//	Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = m_rotationX * 0.0174532925f;
	yaw = m_rotationY * 3.0174532925f;
	roll = m_rotationZ * 0.0174532925f;

	//	Create the rotation matrix from the yaw, pitch, and roll values.
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);
	//회전행렬을 구함.



	//	Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	//lookat * rotation
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);
	// lookAt 및 up 벡터를 회전 행렬로 변형하여 뷰가 원점에서 올바르게 회전되도록 합니다.

	//	Translate the rotated camera position to the location of the viewer.
	lookAt = position + lookAt;
	// 회전 된 카메라 위치를 뷰어 위치로 변환합니다.


	//lookAt.x = -1.0f;
	//lookAt.y = 0.0f;
	//lookAt.z = 0.0f;


	//	Finally create the view matrix from the three updated vectors.
	D3DXMatrixLookAtLH(&m_viewMatrix, &position, &lookAt, &up);

	// 마지막으로 세 개의 업데이트 된 벡터에서 뷰 행렬을 만듭니다.

	return;
}

void CameraClass::GetViewMatrix(D3DXMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
	return;
}

void CameraClass::SetViewMatrix(D3DXMATRIX& matrix) {
	m_viewMatrix *= matrix;
	return;
}





void CameraClass::GenerateBaseViewMatrix()
{
	D3DXVECTOR3 up, position, lookAt;
	float yaw, pitch, roll;
	D3DXMATRIX rotationMatrix;


	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Setup the position of the camera in the world.
	position.x = m_positionX;
	position.y = m_positionY;
	position.z = m_positionZ;

	// Setup where the camera is looking by default.
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = m_rotationX * 0.0174532925f;
	yaw = m_rotationY * 0.0174532925f;
	roll = m_rotationZ * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAt = position + lookAt;

	// Finally create the view matrix from the three updated vectors.
	D3DXMatrixLookAtLH(&m_baseViewMatrix, &position, &lookAt, &up);

	return;
}



void CameraClass::GetBaseViewMatrix(D3DXMATRIX& viewMatrix)
{
	viewMatrix = m_baseViewMatrix;
	return;
}

void CameraClass::RenderReflection(float height)
{
	D3DXVECTOR3 up, position, lookAt;
	float yaw, pitch, roll;
	D3DXMATRIX rotationMatrix;


	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Setup the position of the camera in the world.  For planar reflection invert the Y position of the camera.
	position.x = m_positionX;
	position.y = -m_positionY + (height * 2.0f);
	position.z = m_positionZ;

	// Setup where the camera is looking by default.
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.  Invert the X rotation for reflection.
	pitch = -m_rotationX * 0.0174532925f;
	yaw = m_rotationY * 0.0174532925f;
	roll = m_rotationZ * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAt = position + lookAt;

	// Finally create the reflection view matrix from the three updated vectors.
	D3DXMatrixLookAtLH(&m_reflectionViewMatrix, &position, &lookAt, &up);

	return;
}


