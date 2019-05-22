#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

#include <windows.h>
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "lightshaderclass.h"
#include "lightclass.h"
#include "bitmapclass.h"
#include "textclass.h" 
#include "textureshaderclass.h"
#include "skydomeclass.h"
#include "skydomeshaderclass.h"
#include "refractionshaderclass.h"


#include "TerrainClass.h"
#include "TerrainShaderClass.h"


#include "rendertextureclass.h"
#include "reflectionshaderclass.h"
#include "waterclass.h"
#include "watershaderclass.h"

#include "SkyPlaneClass.h"
#include "SkyPlaneShaderClass.h"

#include "particleshaderclass.h"
#include "particlesystemclass.h"

#include "fireshaderclass.h"

#include "transparentshaderclass.h"

#include "s_lightshaderclass.h"

#include "soundclass.h"

/////////////
// GLOBALS //
/////////////

const bool FULL_SCREEN = false;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;
const bool VSYNC_ENABLED = true;
#define SIZE 20
#define NUMOFEFFECT 3
#define TEXTSIZE 7
#define FrameCount 50
#define CIRCLE_COLLIDER 1
#define NUMOFSPECILAR 1
////////////////////////////////////////////////////////////////////////////////
//	Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&); ~GraphicsClass();

	bool Initialize(int, int, HWND, int & change);
	void Shutdown();
	bool Frame(int, int, float,int,int,int);
	double RotationAngle;
public:
	bool Render(float,float,double);
	int mousePosX, mousePosY;
	int screenWidth, screenHeight;
	D3DXMATRIX baseViewMatrix;
	int ObjectCount;
	int PolygonCount;
	int ScreenSize;
	void RenderRefractionToTexture();
	void RenderReflectionToTexture();
	const int UIWidth = 200;
	const int UIHeight = 200;

	TerrainClass* m_Terrain;
	TerrainShaderClass* m_TerrainShader;
	LightClass* m_ModelLight;


	RenderTextureClass *m_RefractionTexture, *m_ReflectionTexture;
	ReflectionShaderClass* m_ReflectionShader;
	WaterClass* m_Water;
	WaterShaderClass* m_WaterShader;

	ParticleShaderClass* m_ParticleShader;
	ParticleSystemClass* m_ParticleSystem;
private:
	D3DClass * m_D3D;
	CameraClass* m_Camera;
	ModelClass* m_Model[SIZE];	
	LightShaderClass* m_LightShader[SIZE]; 
	LightClass* m_Light;
	TextureShaderClass* m_TextureShader;
	TextureShaderClass* m_AimTextureShader;
	BitmapClass* m_Bitmap;
	BitmapClass* m_Aim;
	TextClass* m_Text[TEXTSIZE];
	TextClass* m_Text2;
	TextClass* m_Text3[SIZE];
	SkyDomeClass* m_SkyDome;
	SkyDomeShaderClass* m_SkyDomeShader;

	SkyPlaneClass *m_SkyPlane;
	SkyPlaneShaderClass* m_SkyPlaneShader;

	ModelClass* CircleCollider[CIRCLE_COLLIDER];
	LightShaderClass* m_ColliderShader;
	// only for Effect
private:
	FireShaderClass* m_FireShader[NUMOFEFFECT];
	ModelClass* m_Effect[NUMOFEFFECT];

	BitmapClass *m_BillboardModel;


	TransparentShaderClass* m_TransparentShader;

	int InputKey;
	int HurtFrame;
	bool ShipHurt;

	float tempX;
	float tempY;
	FILE* p;
	
	D3DXVECTOR2 ColPoint[139];

private:
	LightClass* m_S_Light;
	SLightShaderClass* m_S_LightShader;


	SoundClass* m_Sound;
	float m_Timer;
};

#endif
