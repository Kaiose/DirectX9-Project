#pragma once
////////////////////////////////////////////////////////////////////////////////
//	Filename: cameraclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _CAMERACLASS_H_ 
#define _CAMERACLASS_H_

//////////////
// INCLUDES //
//////////////
#include <d3dx10math.h>
#include <math.h>

////////////////////////////////////////////////////////////////////////////////
//	Class name: CameraClass
////////////////////////////////////////////////////////////////////////////////
class CameraClass
{
public:
	CameraClass();
	CameraClass(const CameraClass&); 
	~CameraClass();
	D3DXMATRIX m_baseViewMatrix;
	void SetPosition(float x, float y, float z);
	void SetRotation(float, float, float);

	D3DXMATRIX GetReflectionViewMatrix();
	D3DXMATRIX m_reflectionViewMatrix;
	void Move(int KeyNum, double RotationAngle, float frameTime);

	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();
	D3DXVECTOR3 position;
	float GetAngle();
	void Render();
	void GetViewMatrix(D3DXMATRIX&);
	void SetViewMatrix(D3DXMATRIX& matrix);
	int move_X, move_Y, move_Z;
	bool posChange, angleChange;
	D3DXVECTOR3 rotate;
	float Speed;
	float vibration;
	
	void GenerateBaseViewMatrix();
	void GetBaseViewMatrix(D3DXMATRIX& viewMatrix);
	void RenderReflection(float height);
	float Angle;
	float GetAccelation();
	void SetAccelation(float Speed);
	float Accelation;
//private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;

	D3DXMATRIX m_viewMatrix;


	bool Hurt;
};

#endif
