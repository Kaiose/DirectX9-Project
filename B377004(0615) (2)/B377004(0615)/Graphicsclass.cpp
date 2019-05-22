#include "graphicsclass.h"



GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;


	for (int i = 0; i < SIZE; i++) {
		m_Model[i] = NULL;
		m_LightShader[i] = NULL;
	}
	for (int i = 0; i < TEXTSIZE; i++) {
		m_Text[i] = NULL;
	}

	for (int i = 0; i < NUMOFEFFECT; i++)
	{
		m_Effect[i] = NULL;
		m_FireShader[i] = NULL;
	}


	m_SkyDome = 0;
	m_SkyDomeShader = 0;
	m_Light = 0;
	m_Bitmap = 0;
	m_TextureShader = 0;
	mousePosX = 0;
	mousePosY = 0;
	screenWidth = 0, screenHeight = 0;

	ObjectCount = SIZE;
	PolygonCount = 0;
	ScreenSize = 0;

	m_Terrain = 0;


	m_TerrainShader = 0;
	m_ModelLight = 0;


	m_RefractionTexture = 0;
	m_ReflectionTexture = 0;
	m_ReflectionShader = 0;
	m_Water = 0;
	m_WaterShader = 0;

	m_SkyPlane = 0;
	m_SkyPlaneShader = 0;

	RotationAngle = 0.0f;

	m_ParticleShader = 0;
	m_ParticleSystem = 0;


	m_BillboardModel = 0;

	int InputKey = 0;

	int HurtFrame = FrameCount;

	bool ShipHurt = false;


	//for Specular
	m_S_Light = 0;

	m_Sound = 0;
	m_Timer = 0;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{

}

GraphicsClass::~GraphicsClass()
{
	fclose(p);
}



bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd, int& change)
{

	p = fopen("ColliderPoint.txt", "r");
	int index = 0;
	while (!feof(p)) {
		fscanf(p, "%f%f", &ColPoint[index].x, &ColPoint[index].y);
		fgetc(p);
		index++;
	}


	bool result;
	//D3DXMATRIX baseViewMatrix;
	// Create the Direct3D object.

	m_D3D = new D3DClass;
	if (!m_D3D)
	{
		return false;
	}
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
	mousePosX = 0; //screenHeight / 2;
	mousePosY = 0;// screenWidth / 2;
	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED,
		hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if (!m_Camera)
	{

		return false;
	}

	m_Camera->SetPosition(0.0f, 5.0f, -10.0f);
	m_Camera->GenerateBaseViewMatrix();
	m_Camera->GetBaseViewMatrix(baseViewMatrix);

	// Set the initial position of the camera.
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);

	m_AimTextureShader = new TextureShaderClass;
	m_AimTextureShader->Initialize(m_D3D->GetDevice(), hwnd);



	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader) { return false; }
	// Initialize the texture shader object. 
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}
	//	Create the model object.
	for (int i = 0; i < SIZE; i++) {
		m_Model[i] = new ModelClass;
		if (!m_Model[i])
		{
			return false;
		}
	}
	//	Initialize the model object.

//fance , iball , table, character, //desk
	m_SkyDome = new SkyDomeClass;
	if (!m_SkyDome)
	{
		return false;
	}

	// Initialize the sky dome object.
	result = m_SkyDome->Initialize(m_D3D->GetDevice());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the sky dome object.", L"Error", MB_OK);
		return false;
	}

	// Create the sky dome shader object.
	m_SkyDomeShader = new SkyDomeShaderClass;
	if (!m_SkyDomeShader)
	{
		return false;
	}

	// Initialize the sky dome shader object.
	result = m_SkyDomeShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the sky dome shader object.", L"Error", MB_OK);
		return false;
	}







	////// box box box














	result = m_Model[0]->InitializeByThread(m_D3D->GetDevice(),
		"TriangulationModel/Ship_free_Triangle.obj"
		, L"Wood.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	result = m_Model[1]->InitializeByThread(m_D3D->GetDevice(),
		"BeconTriangle.obj"
		, L"TriangulationModel/OceanTexture.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	result = m_Model[2]->InitializeByThread(m_D3D->GetDevice(),
		"smallship2.obj", L"Wood.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	m_Model[2]->CurrentPositionX = 49.0f, m_Model[2]->CurrentPositionY = 3.5f,
	m_Model[2]->CurrentPositionZ = 215.0f, m_Model[2]->CurrentDgree = (float)D3DX_PI;
	m_Model[2]->CreateAllocatePosition(m_Model[2]->Header, 49.0f, 3.5f, 215.0f, (float)D3DX_PI);
	m_Model[2]->CreateAllocatePosition(m_Model[2]->Header, 49.0f, 3.5f, 112.0f, (float)D3DX_PI*2);
	
	result = m_Model[3]->InitializeByThread(m_D3D->GetDevice(),
		"boat.obj", L"wood1_COLOR.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	m_Model[3]->CurrentPositionX = 154.0f, m_Model[3]->CurrentPositionY = 3.0f,
	m_Model[3]->CurrentPositionZ = 90.0f, m_Model[3]->CurrentDgree = (float)D3DX_PI;
	m_Model[3]->CreateAllocatePosition(m_Model[3]->Header, 154.0f, 3.0f, 90.0f, (float)D3DX_PI);
	m_Model[3]->CreateAllocatePosition(m_Model[3]->Header, 84.0f, 3.0f, 90.0f, (float)D3DX_PI*2);

	result = m_Model[4]->InitializeByThread(m_D3D->GetDevice(),
		"dock.obj", L"dock.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	result = m_Model[5]->InitializeByThread(m_D3D->GetDevice(),
		"dock.obj", L"dock.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	result = m_Model[6]->InitializeByThread(m_D3D->GetDevice(),
		"smallship4.obj", L"Wood3.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	result = m_Model[7]->InitializeByThread(m_D3D->GetDevice(),
		"Leaf1.obj", L"Leaf.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	result = m_Model[8]->InitializeByThread(m_D3D->GetDevice(),
		"treebody.obj", L"treebody.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	result = m_Model[9]->InitializeByThread(m_D3D->GetDevice(),
		"treebody.obj", L"treebody.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	result = m_Model[10]->InitializeByThread(m_D3D->GetDevice(),
		"Leaf1.obj", L"Leaf.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	result = m_Model[11]->InitializeByThread(m_D3D->GetDevice(),
		"treebody.obj", L"treebody.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	result = m_Model[12]->InitializeByThread(m_D3D->GetDevice(),
		"Leaf1.obj", L"Leaf.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	result = m_Model[13]->InitializeByThread(m_D3D->GetDevice(),
		"rocks3.obj", L"rock.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	result = m_Model[14]->InitializeByThread(m_D3D->GetDevice(),
		"rocks3.obj", L"rock.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	result = m_Model[15]->InitializeByThread(m_D3D->GetDevice(),
		"woodenbridge2.obj", L"wood4.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	result = m_Model[16]->InitializeByThread(m_D3D->GetDevice(),
		"BeconTriangle.obj"
		, L"TriangulationModel/OceanTexture.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	result = m_Model[17]->InitializeByThread(m_D3D->GetDevice(),
		"roof.obj", L"blue.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	result = m_Model[18]->InitializeByThread(m_D3D->GetDevice(),
		"housewall.obj", L"white.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	result = m_Model[19]->InitializeByThread(m_D3D->GetDevice(),
		"goose.obj", L"gull.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	m_Model[19]->CurrentPositionX = 60.0f, m_Model[19]->CurrentPositionY = 45.0f,
	m_Model[19]->CurrentPositionZ = 0.0f, m_Model[19]->CurrentDgree = (float)D3DX_PI*2.3f;
	m_Model[19]->CreateAllocatePosition(m_Model[19]->Header, 1560.f, 45.0f, 1500.0f, (float)D3DX_PI*2.3);
	m_Model[19]->CreateAllocatePosition(m_Model[19]->Header, 500.0f, 45.0f, -1060.0f, (float)D3DX_PI*2.3);
	m_Model[19]->CreateAllocatePosition(m_Model[19]->Header, -1000.0f, 45.0f, -1060.0f, (float)D3DX_PI*2.3);


	// Create the light shader object. 
	for (int i = 0; i < SIZE; i++) {
		m_LightShader[i] = new LightShaderClass;
		if (!m_LightShader[i])
		{
			return false;
		}

		// Initialize the light shader object. 
		result = m_LightShader[i]->Initialize(m_D3D->GetDevice(), hwnd);
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the light shader 1object.", L"Error", MB_OK);
			return false;
		}
	}



	m_Bitmap = new BitmapClass;
	if (!m_Bitmap) {
		return false;
	}
	m_Aim = new BitmapClass;


	// Initialize the bitmap object.  
	result = m_Bitmap->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"compass6.dds", UIWidth, UIHeight);
	if (!result) { MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);   return false; }

	result = m_Aim->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"aim2.dds", UIWidth/2, UIHeight/2);
	if (!result) { MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);   return false; }


	

	// Create the light object. 
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}

	// Initialize the light object. 
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(1.0f, -0.75f, 0.25f);


	m_ModelLight = new LightClass;
	if (!m_Light)
	{
		return false;
	}
	m_ModelLight->SetAmbientColor(0.05f, 0.05f, 0.05f, 1.0f);
	m_ModelLight->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_ModelLight->SetDirection(1.0f, -0.75f, 0.25f);




	for (int i = 0; i < TEXTSIZE; i++) {
		m_Text[i] = new TextClass;
	}


	// Initialize the text object.  
	result = m_Text[0]->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix, "", 1, 0, 0);
	if (!result) { MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);   return false; }

	result = m_Text[1]->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix, "", 0, 1, 0);
	if (!result) { MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);   return false; }

	result = m_Text[2]->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix, "", 0, 0, 1);
	if (!result) { MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);   return false; }
	result = m_Text[3]->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix, "", 0, 0, 1);
	if (!result) { MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);   return false; }
	//

	

	m_Terrain = new TerrainClass;
	if (!m_Terrain)
	{
		return false;
	}

	result = m_Terrain->Initialize(m_D3D->GetDevice(),
		"TriangulationModel/heightmap01.bmp", "TriangulationModel/cm.bmp",
		10.0f, L"TriangulationModel/dirt.dds",
		L"TriangulationModel/water01.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain object.", L"Error", MB_OK);
		return false;
	}


	m_TerrainShader = new TerrainShaderClass;
	if (!m_TerrainShader)
	{
		return false;
	}

	// Initialize the terrain shader object.
	result = m_TerrainShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain shader object.", L"Error", MB_OK);
		return false;
	}

	m_RefractionTexture = new RenderTextureClass;
	if (!m_RefractionTexture)
	{
		return false;
	}
	
	
	// Initialize the refraction render to texture object.
	result = m_RefractionTexture->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the refraction render to texture object.", L"Error", MB_OK);
		return false;
	}

	// Create the reflection render to texture object.
	m_ReflectionTexture = new RenderTextureClass;
	if (!m_ReflectionTexture)
	{
		return false;
	}

	// Initialize the reflection render to texture object.
	result = m_ReflectionTexture->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the reflection render to texture object.", L"Error", MB_OK);
		return false;
	}


	m_ReflectionShader = new ReflectionShaderClass;
	if (!m_ReflectionShader)
	{
		return false;
	}

	// Initialize the reflection shader object.
	result = m_ReflectionShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the reflection shader object.", L"Error", MB_OK);
		return false;
	}

	m_Water = new WaterClass;
	if (!m_Water)
	{
		return false;
	}

	// Initialize the water object.
	result = m_Water->Initialize(m_D3D->GetDevice(), L"normal1.dds", 3.75f, 110.0f);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the water object.", L"Error", MB_OK);
		return false;
	}

	// Create the water shader object.
	m_WaterShader = new WaterShaderClass;
	if (!m_WaterShader)
	{
		return false;
	}

	// Initialize the water shader object.
	result = m_WaterShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the water shader object.", L"Error", MB_OK);
		return false;
	}
	m_SkyPlane = new SkyPlaneClass;
	if (!m_SkyPlane)
	{
		return false;
	}

	// Initialize the sky plane object.
	result = m_SkyPlane->Initialize(m_D3D->GetDevice(), L"cloud001.dds", L"perturb001.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the sky plane object.", L"Error", MB_OK);
		return false;
	}


	// Create the sky plane shader object.
	m_SkyPlaneShader = new SkyPlaneShaderClass;
	if (!m_SkyPlaneShader)
	{
		return false;
	}

	// Initialize the sky plane shader object.
	result = m_SkyPlaneShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the sky plane shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the model object.
	for (int i = 0; i < NUMOFEFFECT; i++)
	{
		m_Effect[i] = new ModelClass;
		if (!m_Effect[i])
		{
			return false;
		}
	}

	// Initialize the Effect object.
	result = m_Effect[0]->InitializeEffect(m_D3D->GetDevice(), "currentofwater.obj", L"currentofwater.dds",
		L"waternoise4.dds", L"alpha01.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	result = m_Effect[1]->InitializeEffect(m_D3D->GetDevice(), "currentofwater.obj", L"currentofwater.dds",
		L"waternoise4.dds", L"alpha01.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	result = m_Effect[2]->InitializeEffect(m_D3D->GetDevice(), "Leaf3.obj", L"sakura.dds",
		L"noise01.dds", L"alpha01.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create the fire shader object.
	for (int i = 0; i < NUMOFEFFECT; i++)
	{
		m_FireShader[i] = new FireShaderClass;
		if (!m_FireShader[i])
		{
			return false;
		}

		// Initialize the fire shader object.
		result = m_FireShader[i]->Initialize(m_D3D->GetDevice(), hwnd);
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the fire shader object.", L"Error", MB_OK);
			return false;
		}
	}

	// Initialize the light shader object.
	m_S_LightShader = new SLightShaderClass;
	if (!m_S_LightShader)
	{
		return false;
	}
	result = m_S_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the  shader 2object.", L"Error", MB_OK);
		return false;
	}

	//initialize the S_light object
	m_S_Light = new LightClass;
	if (!m_S_Light)
	{
		return false;
	}
	m_S_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_S_Light->SetDiffuseColor(0.3f, 0.3f, 0.3f, 0.3f);
	m_S_Light->SetDirection(1.0f, -0.75f, 0.25f);
	m_S_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 0.5f);
	m_S_Light->SetSpecularPower(32.0f);


	m_ParticleShader = new ParticleShaderClass;
	if (!m_ParticleShader)
	{
		return false;
	}

	// Initialize the particle shader object.
	result = m_ParticleShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the particle shader object.", L"Error", MB_OK);
		return false;
	}

		// Create the particle system object.
		m_ParticleSystem = new ParticleSystemClass;
	if (!m_ParticleSystem)
	{
		return false;
	}

	// Initialize the particle system object.
	result = m_ParticleSystem->Initialize(m_D3D->GetDevice(), L"rain11.dds");
	if (!result)
	{
		return false;
	}
	
	m_BillboardModel = new BitmapClass;
	result = m_BillboardModel->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"Hurt.dds", screenWidth, screenHeight);

	m_TransparentShader = new TransparentShaderClass;

	result = m_TransparentShader->Initialize(m_D3D->GetDevice(), hwnd);
	
	for (int i = 0; i < ObjectCount; i++) {
		PolygonCount += m_Model[i]->Get_FaceCount();
	}

	PolygonCount += m_Terrain->GetIndexCount()/3;





	char polystr[100] = { " PolygonCount : " };
	char objectstr[100] = { " ObjectCount : " };
	char screenstr[100] = { " ScreenSIze : " };
	char buf[10];
	sprintf(buf, "%d", PolygonCount);
	strcat(polystr, buf);

	result = m_Text[4]->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix, polystr, 0, 0, 1);
	if (!result) { MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);   return false; }
	memset(buf, '\0', sizeof(char) * 10);
	sprintf(buf, "%d", ObjectCount);
	strcat(objectstr, buf);
	result = m_Text[5]->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix, objectstr, 0, 0, 1);
	if (!result) { MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);   return false; }
	memset(buf, '\0', sizeof(char) * 10);
	sprintf(buf, "%d*%d", screenWidth, screenHeight);
	strcat(screenstr, buf);
	result = m_Text[6]->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix, screenstr, 0, 0, 1);
	if (!result) { MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);   return false; }


	m_Sound = new SoundClass;
	if (!m_Sound) {
		return false;
	}
	m_Sound->Initialize(hwnd);


	
	return true;
}


void GraphicsClass::Shutdown()
{
	if (m_TransparentShader)
	{
		m_TransparentShader->Shutdown();
		delete m_TransparentShader;
		m_TransparentShader = 0;
	}

	if (m_BillboardModel)
	{
		m_BillboardModel->Shutdown();
		delete m_BillboardModel;
		m_BillboardModel = 0;
	}
	if (m_ParticleSystem)
	{
		m_ParticleSystem->Shutdown();
		delete m_ParticleSystem;
		m_ParticleSystem = 0;
	}

	// Release the particle shader object.
	if (m_ParticleShader)
	{
		m_ParticleShader->Shutdown();
		delete m_ParticleShader;
		m_ParticleShader = 0;
	}
	if (m_WaterShader)
	{
		m_WaterShader->Shutdown();
		delete m_WaterShader;
		m_WaterShader = 0;
	}

	// Release the water object.
	if (m_Water)
	{
		m_Water->Shutdown();
		delete m_Water;
		m_Water = 0;
	}

	// Release the reflection shader object.
	if (m_ReflectionShader)
	{
		m_ReflectionShader->Shutdown();
		delete m_ReflectionShader;
		m_ReflectionShader = 0;
	}

	// Release the reflection render to texture object.
	if (m_ReflectionTexture)
	{
		m_ReflectionTexture->Shutdown();
		delete m_ReflectionTexture;
		m_ReflectionTexture = 0;
	}

	// Release the refraction render to texture object.
	if (m_RefractionTexture)
	{
		m_RefractionTexture->Shutdown();
		delete m_RefractionTexture;
		m_RefractionTexture = 0;
	}

	if (m_ModelLight)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the terrain shader object.
	if (m_TerrainShader)
	{
		m_TerrainShader->Shutdown();
		delete m_TerrainShader;
		m_TerrainShader = 0;
	}
	// Release the terrain object.
	if (m_Terrain)
	{
		m_Terrain->Shutdown();
		delete m_Terrain;
		m_Terrain = 0;
	}
	if (m_SkyDomeShader)
	{
		m_SkyDomeShader->Shutdown();
		delete m_SkyDomeShader;
		m_SkyDomeShader = 0;
	}

	// Release the sky dome object.
	if (m_SkyDome)
	{
		m_SkyDome->Shutdown();
		delete m_SkyDome;
		m_SkyDome = 0;
	}
	if (m_Bitmap) { m_Bitmap->Shutdown();   delete m_Bitmap;   m_Bitmap = 0; }

	if (m_Aim) { m_Aim->Shutdown();   delete m_Aim;   m_Aim = 0; }

	// Release the light object. 
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}
	if (m_TextureShader) {
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	if (m_AimTextureShader) {
		m_AimTextureShader->Shutdown();
		delete m_AimTextureShader;
		m_AimTextureShader = 0;
	}

	// Release the light shader object. 
	for (int i = 0; i < TEXTSIZE; i++) {
		//if (m_Text[i] == NULL) continue;
		m_Text[i]->Shutdown();
		delete m_Text[i];
		m_Text[i] = 0;
	}
	for (int i = 0; i < SIZE; i++) {

		if (m_LightShader[i])
		{
			if (m_LightShader[i] == NULL) continue;
			m_LightShader[i]->Shutdown();
			delete m_LightShader[i];
			m_LightShader[i] = 0;

		}
		//	Release the model object.
		if (m_Model[i])
		{
			if (m_Model[i] == NULL) continue;
			m_Model[i]->Shutdown();
			delete m_Model[i];
			m_Model[i] = 0;
		}
	}

	//Release Effects
	for (int i = 0; i < NUMOFEFFECT; i++)
	{
		if (m_Effect[i])
		{
			m_Effect[i]->Shutdown();
			delete m_Effect[i];
			m_Effect[i] = 0;
		}
		if (m_FireShader[i])
		{
			m_FireShader[i]->Shutdown();
			delete m_FireShader[i];
			m_FireShader[i] = 0;
		}
	}

	//	Release the camera object. 
	if (m_Camera)
	{
		delete m_Camera; m_Camera = 0;
	}


	if (m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}
	if (m_SkyPlaneShader)
	{
		m_SkyPlaneShader->Shutdown();
		delete m_SkyPlaneShader;
		m_SkyPlaneShader = 0;
	}

	// Release the sky plane object.
	if (m_SkyPlane)
	{
		m_SkyPlane->Shutdown();
		delete m_SkyPlane;
		m_SkyPlane = 0;
	}


	//Release the Specular Object
	if (m_S_Light)
		{
			delete m_S_Light;
			m_S_Light = 0;
		}
	
	if (m_S_LightShader)
	{
		m_S_LightShader->Shutdown();
		delete m_S_LightShader;
		m_S_LightShader = 0;
	}

	return;
}


bool GraphicsClass::Frame(int fps, int cpu, float frameTime, int KeyNum, int mouseX, int mouseY)
{
	bool result;
	m_Timer += 1.0f;
	static float rotation1 = 0.0f;
	float move_X = 0, move_Y = 0, move_Z = 0;
	float rotate_X = 0, rotate_Y = 0, rotate_Z = 0;

	InputKey = KeyNum;
	switch (KeyNum) {
	case 1:
		move_Z = 1 / frameTime;
		break;
	case 2:
		move_X = -1 / frameTime;
		break;
	case 3:
		move_Z = -1 / frameTime;
		break;
	case 4:
		move_X = 1 / frameTime;
		break;
	case 5:
		move_Y = 1 / frameTime;
		break;
	case 6:
		rotate_X = -1000 / frameTime;
		break;
	case 7:
		move_Y = -1 / frameTime;
		break;

	case 8:
		rotate_X = 1000 / frameTime;
		break;
	}

	float ratioY = 0.1f;
	float ratioX = 0.001f;
	D3DXVECTOR3 Rotation = m_Camera->GetRotation();
	float distanceX = (mousePosX - mouseX);
	float distanceY = (mousePosY - mouseY);
	if (distanceX != 0 || distanceY != 0) {
		mousePosX = mouseX;
		mousePosY = mouseY;

		m_Camera->SetRotation(Rotation.x - distanceY*ratioY, Rotation.y - distanceX*ratioX, Rotation.z);

	}


	float CameraRotation = m_Camera->GetAngle();

	double Speed = CameraRotation * 0.001;
	RotationAngle += Speed*frameTime;
	if (RotationAngle > (double)CameraRotation)
		RotationAngle = (double)CameraRotation;
	if (RotationAngle < (double)CameraRotation)
		RotationAngle = (double)CameraRotation;

	//if (RotationAngle > 0.14)
	//	RotationAngle = 1.7;
	//if (RotationAngle < -0.14)
	//	RotationAngle = -1.7;
	//현재 0 도이다. 배가 오른쪽으로 도는건 +
	// 왼쪽으로 돌면 -;
	if (move_X != 0 || move_Y != 0 || move_Z != 0 || KeyNum == 0) {
		//m_Camera->SetPosition(move_X, move_Y, move_Z);
		m_Camera->Move(KeyNum,RotationAngle, frameTime);
	}


	// Update the rotation variable each frame. 
	rotation1 += (float)D3DX_PI * 0.005f;
	if (rotation1 > 360.0f)
	{
		rotation1 -= 360.0f;
	}

	float Accelation = m_Camera->GetAccelation();
	m_Text[0]->SetAccelation(Accelation, m_D3D->GetDeviceContext());
	result = m_Text[3]->SetFps(fps, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}
	// Set the cpu usage.
	result = m_Text[3]->SetCpu(cpu, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}



	m_Water->Frame();
	RenderRefractionToTexture();
	RenderReflectionToTexture();
	m_SkyPlane->Frame();
	m_ParticleSystem->Frame(frameTime, m_D3D->GetDeviceContext());

	// Render the graphics scene.
	result = Render(rotation1,frameTime,RotationAngle);
	if (!result)
	{
		return false;
	}

	return true;
}

bool GraphicsClass::Render(float rotation1,float frametime, double RotationAngle)
{


	D3DXMATRIX viewMatrix, projectionMatrix, baseViewMatrix,orthoMatrix, reflectionViewMatrix;;
	bool result;
	D3DXMATRIX localMatrix[SIZE];
	D3DXMATRIX worldMatrix[SIZE];

	D3DXVECTOR3 cameraPosition;
	for (int i = 0; i < SIZE; i++) {
		m_D3D->GetWorldMatrix(worldMatrix[i]);
		D3DXMatrixIdentity(&localMatrix[i]);
	}
	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	m_Camera->Render();
	m_Camera->RenderReflection(m_Water->GetWaterHeight());
	m_Camera->GetBaseViewMatrix(baseViewMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	reflectionViewMatrix = m_Camera->GetReflectionViewMatrix();


	


	////////////skydome






	m_D3D->GetWorldMatrix(worldMatrix[0]);
	cameraPosition = m_Camera->GetPosition();
	D3DXMatrixTranslation(&worldMatrix[0], cameraPosition.x, cameraPosition.y, cameraPosition.z);
	m_D3D->TurnOffCulling();
	m_D3D->TurnZBufferOff();
	m_SkyDome->Render(m_D3D->GetDeviceContext());
	m_SkyDomeShader->Render(m_D3D->GetDeviceContext(), m_SkyDome->GetIndexCount(), worldMatrix[0], viewMatrix, projectionMatrix,
		m_SkyDome->GetApexColor(), m_SkyDome->GetCenterColor());

	m_D3D->TurnOnCulling();
	D3DXMatrixIdentity(&localMatrix[0]);

	m_D3D->GetWorldMatrix(worldMatrix[0]);
	D3DXMatrixTranslation(&worldMatrix[0], 0, 10, 0);
	D3DXMatrixScaling(&localMatrix[0], 100.0f, 2.0f, 100.0f);
	worldMatrix[0] *= localMatrix[0];

	m_D3D->EnableSecondBlendState();

	// Render the sky plane using the sky plane shader.
	m_SkyPlane->Render(m_D3D->GetDeviceContext());
	m_SkyPlaneShader->Render(m_D3D->GetDeviceContext(), m_SkyPlane->GetIndexCount(), worldMatrix[0], viewMatrix, projectionMatrix,
		m_SkyPlane->GetCloudTexture(), m_SkyPlane->GetPerturbTexture(), m_SkyPlane->GetTranslation(), m_SkyPlane->GetScale(),
		m_SkyPlane->GetBrightness());


	m_D3D->TurnOffAlphaBlending();
	m_D3D->TurnZBufferOn();

	////////////skydome


	////////////////////////terrain////////////////
	m_D3D->GetWorldMatrix(worldMatrix[0]);
	m_Terrain->Render(m_D3D->GetDeviceContext());
	result = m_TerrainShader->Render(m_D3D->GetDeviceContext(), m_Terrain->GetIndexCount(), worldMatrix[0], viewMatrix, projectionMatrix,
		m_Terrain->GetColorTexture(), m_Terrain->GetNormalTexture(), m_Light->GetDiffuseColor(), m_Light->GetDirection(),
		2.0f);
	if (!result)
	{
		return false;
	}





	//////////////////////water ///////////////////
	D3DXMatrixScaling(&localMatrix[0], 2.0f, 1.0f, 2.0f);
	D3DXMatrixTranslation(&worldMatrix[0], 100.0f, m_Water->GetWaterHeight(), 80.0f);

	worldMatrix[0] *= localMatrix[0];


	//D3DXMatrixTranslation(&localMatrix[0], 1, 10, 1);
	//viewMatrix *= localMatrix[0];
	D3DXVECTOR3 temp = m_Camera->GetPosition();
	temp.y += 13;
	m_Water->Render(m_D3D->GetDeviceContext());
	m_WaterShader->Render(m_D3D->GetDeviceContext(), m_Water->GetIndexCount(), worldMatrix[0], viewMatrix, projectionMatrix, reflectionViewMatrix,
		m_RefractionTexture->GetShaderResourceView(), m_ReflectionTexture->GetShaderResourceView(), m_Water->GetTexture(),
		temp, m_Water->GetNormalMapTiling(), m_Water->GetWaterTranslation(), m_Water->GetReflectRefractScale(),
		m_Water->GetRefractionTint(), m_Light->GetDirection(), m_Water->GetSpecularShininess());


	////////////

	

	////////////
	D3DXMatrixIdentity(&localMatrix[0]);

	D3DXMatrixTranslation(&localMatrix[0],-200.0f, -230.0f, -2.0f);
	worldMatrix[0] *= localMatrix[0];
	D3DXMatrixRotationY(&localMatrix[0], (float)D3DX_PI);
	worldMatrix[0] *= localMatrix[0];
	

	D3DXMatrixRotationY(&localMatrix[0], RotationAngle);
	worldMatrix[0] *= localMatrix[0];


	D3DXMatrixScaling(&localMatrix[0], 0.01f, 0.01f, 0.01f);
	worldMatrix[0] *= localMatrix[0];

	D3DXMatrixTranslation(&localMatrix[0], cameraPosition.x, cameraPosition.y, cameraPosition.z);
	worldMatrix[0] *= localMatrix[0];

	D3DXMatrixRotationX(&localMatrix[1], (float)D3DX_PI*0.5);
	worldMatrix[1] *= localMatrix[1];
	D3DXMatrixScaling(&localMatrix[1], 0.05f, 0.05f, 0.05f);
	worldMatrix[1] *= localMatrix[1];
	D3DXMatrixTranslation(&localMatrix[1], 170.0f, 8.0f, 100.0f);
	worldMatrix[1] *= localMatrix[1];


	D3DXMatrixScaling(&localMatrix[2], 0.5f, 0.5f, 0.5f);
	worldMatrix[2] *= localMatrix[2];
	D3DXMatrixRotationY(&localMatrix[2], m_Model[2]->CurrentDgree);
	worldMatrix[2] *= localMatrix[2];
	m_Model[2]->Move(&m_Model[2]->Mark, 0.1f);
	D3DXMatrixTranslation(&localMatrix[2], m_Model[2]->CurrentPositionX, m_Model[2]->CurrentPositionY,
		m_Model[2]->CurrentPositionZ);
	worldMatrix[2] *= localMatrix[2];

	D3DXMatrixIdentity(&localMatrix[3]);
	D3DXMatrixScaling(&localMatrix[3], 1.0f, 1.0f, 1.0f);
	worldMatrix[3] *= localMatrix[3];
	D3DXMatrixRotationY(&localMatrix[3], m_Model[3]->CurrentDgree);
	worldMatrix[3] *= localMatrix[3];
	m_Model[3]->Move(&m_Model[3]->Mark, 0.1f);
	D3DXMatrixTranslation(&localMatrix[3], m_Model[3]->CurrentPositionX, m_Model[3]->CurrentPositionY, 
		m_Model[3]->CurrentPositionZ);
	worldMatrix[3] *= localMatrix[3];


	D3DXMatrixIdentity(&localMatrix[4]);
	D3DXMatrixScaling(&localMatrix[4], 2.0f, 2.0f, 2.0f);
	worldMatrix[4] *= localMatrix[4];
	D3DXMatrixRotationY(&localMatrix[4], (float)D3DX_PI/8);
	worldMatrix[4] *= localMatrix[4];
	D3DXMatrixTranslation(&localMatrix[4], 48.0f, 4.25f, 220.0f);
	worldMatrix[4] *= localMatrix[4];


	D3DXMatrixIdentity(&localMatrix[5]);
	D3DXMatrixScaling(&localMatrix[5], 2.0f, 2.0f, 2.0f);
	worldMatrix[5] *= localMatrix[5];
	D3DXMatrixRotationY(&localMatrix[5], (float)D3DX_PI / 2);
	worldMatrix[5] *= localMatrix[5];
	D3DXMatrixTranslation(&localMatrix[5], 154.0f, 4.25f, 100.0f);
	worldMatrix[5] *= localMatrix[5];

	D3DXMatrixIdentity(&localMatrix[6]);
	D3DXMatrixScaling(&localMatrix[6], 0.1, 0.1f, 0.1f);
	worldMatrix[6] *= localMatrix[6];
	D3DXMatrixRotationY(&localMatrix[6], (float)D3DX_PI / 3);
	worldMatrix[6] *= localMatrix[6];
	D3DXMatrixTranslation(&localMatrix[6], 70.0f, 1.5f, 165.0f);
	worldMatrix[6] *= localMatrix[6];

	D3DXMatrixIdentity(&localMatrix[7]);
	D3DXMatrixScaling(&localMatrix[7], 0.01f, 0.01f, 0.01f);
	worldMatrix[7] *= localMatrix[7];
	D3DXMatrixRotationY(&localMatrix[7], (float)D3DX_PI );
	worldMatrix[7] *= localMatrix[7];
	D3DXMatrixTranslation(&localMatrix[7], 30.0f, 25.0f, 90.0f);
	worldMatrix[7] *= localMatrix[7];

	D3DXMatrixIdentity(&localMatrix[8]);
	D3DXMatrixScaling(&localMatrix[8], 0.01f, 0.005f, 0.01f);
	worldMatrix[8] *= localMatrix[8];
	D3DXMatrixRotationY(&localMatrix[8], (float)D3DX_PI);
	worldMatrix[8] *= localMatrix[8];
	D3DXMatrixTranslation(&localMatrix[8], 40.0f, 22.0f, 94.0f);
	worldMatrix[8] *= localMatrix[8];

	D3DXMatrixIdentity(&localMatrix[9]);
	D3DXMatrixScaling(&localMatrix[9], 0.01f, 0.005f, 0.01f);
	worldMatrix[9] *= localMatrix[9];
	D3DXMatrixRotationY(&localMatrix[9], (float)D3DX_PI);
	worldMatrix[9] *= localMatrix[9];
	D3DXMatrixTranslation(&localMatrix[9], 42.0f, 22.0f, 94.0f);
	worldMatrix[9] *= localMatrix[9];

	D3DXMatrixIdentity(&localMatrix[10]);
	D3DXMatrixScaling(&localMatrix[10], 0.01f, 0.01f, 0.01f);
	worldMatrix[10] *= localMatrix[10];
	D3DXMatrixRotationY(&localMatrix[10], (float)D3DX_PI / 2);
	worldMatrix[10] *= localMatrix[10];
	D3DXMatrixTranslation(&localMatrix[10], 38.0f, 24.5f, 94.0f);
	worldMatrix[10] *= localMatrix[10];

	D3DXMatrixIdentity(&localMatrix[11]);
	D3DXMatrixScaling(&localMatrix[11], 0.01f, 0.005f, 0.01f);
	worldMatrix[11] *= localMatrix[11];
	D3DXMatrixRotationY(&localMatrix[11], (float)D3DX_PI / 2);
	worldMatrix[11] *= localMatrix[11];
	D3DXMatrixTranslation(&localMatrix[11], 45.0f, 22.0f, 94.0f);
	worldMatrix[11] *= localMatrix[11];

	D3DXMatrixIdentity(&localMatrix[12]);
	D3DXMatrixScaling(&localMatrix[12], 0.01f, 0.01f, 0.01f);
	worldMatrix[12] *= localMatrix[12];
	D3DXMatrixRotationY(&localMatrix[12], (float)D3DX_PI);
	worldMatrix[12] *= localMatrix[12];
	D3DXMatrixTranslation(&localMatrix[12], 41.5f, 23.0f, 77.5f);
	worldMatrix[12] *= localMatrix[12];


	D3DXMatrixIdentity(&localMatrix[13]);
	D3DXMatrixScaling(&localMatrix[13], 0.3f, 0.3f, 0.3f);
	worldMatrix[13] *= localMatrix[13];
	D3DXMatrixRotationY(&localMatrix[13], (float)D3DX_PI);
	worldMatrix[13] *= localMatrix[13];
	D3DXMatrixTranslation(&localMatrix[13], -238.0f, 2.75f, 2122.0f);
	worldMatrix[13] *= localMatrix[13];

	D3DXMatrixIdentity(&localMatrix[14]);
	D3DXMatrixScaling(&localMatrix[14], 0.3f, 0.3f, 0.3f);
	worldMatrix[14] *= localMatrix[14];
	D3DXMatrixRotationY(&localMatrix[14], (float)D3DX_PI);
	worldMatrix[14] *= localMatrix[14];
	D3DXMatrixTranslation(&localMatrix[14], -240.0f, 2.0f, 2127.0f);
	worldMatrix[14] *= localMatrix[14];

	D3DXMatrixIdentity(&localMatrix[15]);
	D3DXMatrixScaling(&localMatrix[15], 0.035f, 0.025f, 0.025f);
	worldMatrix[15] *= localMatrix[15];
	D3DXMatrixRotationY(&localMatrix[15], (float)D3DX_PI*0.3f);
	worldMatrix[15] *= localMatrix[15];
	D3DXMatrixTranslation(&localMatrix[15], 170.0f, 1.5f, 108.0f);
	worldMatrix[15] *= localMatrix[15];

	D3DXMatrixRotationX(&localMatrix[16], (float)D3DX_PI*0.5);
	worldMatrix[16] *= localMatrix[16];
	D3DXMatrixScaling(&localMatrix[16], 0.05f, 0.05f, 0.05f);
	worldMatrix[16] *= localMatrix[16];
	D3DXMatrixTranslation(&localMatrix[16], 30.0f, 5.0f, 220.0f);
	worldMatrix[16] *= localMatrix[16];

	D3DXMatrixRotationX(&localMatrix[17], (float)D3DX_PI*0.0125);
	worldMatrix[17] *= localMatrix[17];
	D3DXMatrixScaling(&localMatrix[17], 2.0f, 2.0f, 2.0f);
	worldMatrix[17] *= localMatrix[17];
	D3DXMatrixTranslation(&localMatrix[17], 165.0f, 9.5f, 160.0f);
	worldMatrix[17] *= localMatrix[17];

	D3DXMatrixRotationX(&localMatrix[18], (float)D3DX_PI*0.0125);
	worldMatrix[18] *= localMatrix[18];
	D3DXMatrixScaling(&localMatrix[18], 2.0f, 2.0f, 2.0f);
	worldMatrix[18] *= localMatrix[18];
	D3DXMatrixTranslation(&localMatrix[18], 165.0f, 10.0f, 162.0f);
	worldMatrix[18] *= localMatrix[18];

	D3DXMatrixRotationY(&localMatrix[19], m_Model[19]->CurrentDgree);
	worldMatrix[19] *= localMatrix[19];
	D3DXMatrixScaling(&localMatrix[19], 20.0f, 20.0f, 20.0f);
	worldMatrix[19] *= localMatrix[19];
	m_Model[19]->Move(&m_Model[19]->Mark,1.0f);
	D3DXMatrixTranslation(&localMatrix[19], m_Model[19]->CurrentPositionX, m_Model[19]->CurrentPositionY, m_Model[19]->CurrentPositionZ);
	worldMatrix[19] *= localMatrix[19];



	for (int i = 0; i < SIZE; i++) {
		if (i == 1) continue;
		m_Model[i]->Render(m_D3D->GetDeviceContext());

		result = m_LightShader[i]->Render(m_D3D->GetDeviceContext(), m_Model[i]->GetIndexCount(), worldMatrix[i],
			viewMatrix, projectionMatrix, m_Model[i]->GetTexture(), m_ModelLight->GetDirection(),
			m_ModelLight->GetAmbientColor(), m_ModelLight->GetDiffuseColor());
	}
	m_Model[1]->Render(m_D3D->GetDeviceContext());
	result = m_S_LightShader->Render(m_D3D->GetDeviceContext(), m_Model[1]->GetIndexCount(), worldMatrix[1], viewMatrix, projectionMatrix,
		m_Model[1]->GetTexture(), m_S_Light->GetDirection(), m_S_Light->GetAmbientColor(), m_S_Light->GetDiffuseColor(),
		m_Camera->GetPosition(), m_S_Light->GetSpecularColor(), m_S_Light->GetSpecularPower());
	if (!result)
	{
		return false;
	}
	// Present the rendered scene to the screen.

	// Render the model using the light shader.



	/////////////////////Effect////////////////////////////
	D3DXMATRIX localMatrixforEffect[NUMOFEFFECT];
	D3DXMATRIX worldMatrixforEffect[NUMOFEFFECT];
	D3DXVECTOR3 scrollSpeeds, scales;
	D3DXVECTOR2 distortion1, distortion2, distortion3;
	float distortionScale, distortionBias;
	static float frameTime = 0.0f;

	for (int i = 0; i < NUMOFEFFECT; i++)
	{
		m_D3D->GetWorldMatrix(worldMatrixforEffect[i]);
		D3DXMatrixIdentity(&localMatrixforEffect[i]);
	}

	// Increment the frame time counter.
	frameTime += 0.01f;
	if (frameTime > 1000.0f)
	{
		frameTime = 0.0f;
	}

	// Set the three scrolling speeds for the three different noise textures.
	scrollSpeeds = D3DXVECTOR3(1.3f, 2.1f, 2.3f);

	// Set the three scales which will be used to create the three different noise octave textures.
	scales = D3DXVECTOR3(1.0f, 2.0f, 3.0f);

	// Set the three different x and y distortion factors for the three different noise textures.
	distortion1 = D3DXVECTOR2(0.1f, 0.2f);
	distortion2 = D3DXVECTOR2(0.1f, 0.3f);
	distortion3 = D3DXVECTOR2(0.1f, 0.1f);

	// The the scale and bias of the texture coordinate sampling perturbation.
	distortionScale = 0.8f;
	distortionBias = 0.5f;

	//// Get the world, view, and projection matrices from the camera and d3d objects.
	//m_D3D->GetWorldMatrix(worldMatrixforEffect);
	// Turn on alpha blending for the fire transparency.
	m_D3D->TurnOnAlphaBlending();

	D3DXMatrixIdentity(&localMatrixforEffect[0]);
	D3DXMatrixScaling(&localMatrixforEffect[0], 0.1f, 0.1f, 0.2f);
	worldMatrixforEffect[0] *= localMatrixforEffect[0];
	D3DXMatrixRotationY(&localMatrixforEffect[0], m_Model[2]->CurrentDgree*(-1.0));
	worldMatrixforEffect[0] *= localMatrixforEffect[0];
	D3DXMatrixRotationX(&localMatrixforEffect[0], (float)D3DX_PI);
	worldMatrixforEffect[0] *= localMatrixforEffect[0];
	D3DXMatrixTranslation(&localMatrixforEffect[0], m_Model[2]->CurrentPositionX, m_Model[2]->CurrentPositionY + 0.3f , m_Model[2]->CurrentPositionZ);
	worldMatrixforEffect[0] *= localMatrixforEffect[0];

	// Put the square model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Effect[0]->Render(m_D3D->GetDeviceContext());

	// Render the square model using the fire shader.
	result = m_FireShader[0]->Render(m_D3D->GetDeviceContext(), m_Effect[0]->GetIndexCount(), worldMatrixforEffect[0], viewMatrix, projectionMatrix,
		m_Effect[0]->GetTexture1(), m_Effect[0]->GetTexture2(), m_Effect[0]->GetTexture3(), frameTime, -scrollSpeeds,
		scales, distortion1, distortion2, distortion3, distortionScale, distortionBias);
	if (!result)
	{
		return false;
	}

	D3DXMatrixIdentity(&localMatrixforEffect[1]);
	D3DXMatrixScaling(&localMatrixforEffect[1], 0.1f, 0.1f, 0.2f);
	worldMatrixforEffect[1] *= localMatrixforEffect[1];
	D3DXMatrixRotationY(&localMatrixforEffect[1], (m_Model[3]->CurrentDgree*(-1.0) - (float)D3DX_PI / 2));
	worldMatrixforEffect[1] *= localMatrixforEffect[1];
	D3DXMatrixRotationX(&localMatrixforEffect[1], (float)D3DX_PI);
	worldMatrixforEffect[1] *= localMatrixforEffect[1];
	D3DXMatrixTranslation(&localMatrixforEffect[1], m_Model[3]->CurrentPositionX, m_Model[3]->CurrentPositionY + 1.0f , m_Model[3]->CurrentPositionZ);
	worldMatrixforEffect[1] *= localMatrixforEffect[1];


	m_Effect[1]->Render(m_D3D->GetDeviceContext());

	// Render the square model using the fire shader.
	result = m_FireShader[1]->Render(m_D3D->GetDeviceContext(), m_Effect[1]->GetIndexCount(), worldMatrixforEffect[1], viewMatrix, projectionMatrix,
		m_Effect[1]->GetTexture1(), m_Effect[1]->GetTexture2(), m_Effect[1]->GetTexture3(), frameTime, -scrollSpeeds,
		scales, distortion1, distortion2, distortion3, distortionScale, distortionBias);
	if (!result)
	{
		return false;
	}
	
	D3DXMatrixIdentity(&localMatrixforEffect[2]);
	D3DXMatrixScaling(&localMatrixforEffect[2], 1.0f, 1.0f, 1.0f);
	worldMatrixforEffect[2] *= localMatrixforEffect[2];
	D3DXMatrixRotationY(&localMatrixforEffect[2], ((float)D3DX_PI));
	worldMatrixforEffect[2] *= localMatrixforEffect[2];
	D3DXMatrixRotationX(&localMatrixforEffect[2], (float)D3DX_PI);
	worldMatrixforEffect[2] *= localMatrixforEffect[2];
	D3DXMatrixTranslation(&localMatrixforEffect[2], 95.0f, 30.0f, 170.0f);
	worldMatrixforEffect[2] *= localMatrixforEffect[2];



	m_Effect[2]->Render(m_D3D->GetDeviceContext());

	// Render the square model using the fire shader.
	result = m_FireShader[2]->Render(m_D3D->GetDeviceContext(), m_Effect[2]->GetIndexCount(), worldMatrixforEffect[2], viewMatrix, projectionMatrix,
		m_Effect[2]->GetTexture1(), m_Effect[2]->GetTexture2(), m_Effect[2]->GetTexture3(), frameTime, -scrollSpeeds,
		scales, distortion1, distortion2, distortion3, distortionScale, distortionBias);
	if (!result)
	{
		return false;
	}





	D3DXMATRIX worldMatrixTEXT[TEXTSIZE];
	for (int i = 0; i < TEXTSIZE; i++) {
		m_D3D->GetWorldMatrix(worldMatrixTEXT[i]);
	}

	D3DXMatrixTranslation(&worldMatrixTEXT[0], 20.0f, -300.0f, 0.0f);
	D3DXMatrixTranslation(&worldMatrixTEXT[1], 250.0f, -300.0f, 0.0f);
	D3DXMatrixTranslation(&worldMatrixTEXT[2], 400.0f, -300.0f, 0.0f);
	D3DXMatrixTranslation(&worldMatrixTEXT[4], 60.0f, 60.0f, 0.0f);
	D3DXMatrixTranslation(&worldMatrixTEXT[5], 60.0f, 80.0f, 0.0f);
	D3DXMatrixTranslation(&worldMatrixTEXT[6], 250.0f, 60.0f, 0.0f);




	m_D3D->TurnZBufferOff();
	m_D3D->TurnOnAlphaBlending();
	for (int i = 0; i < TEXTSIZE; i++) {
		result = m_Text[i]->Render(m_D3D->GetDeviceContext(), worldMatrixTEXT[i], orthoMatrix);
		if (!result) { return false; }
	}
	m_D3D->TurnOffAlphaBlending();
	m_D3D->TurnZBufferOn();

	D3DXMatrixIdentity(&localMatrix[0]);
	//D3DXMatrixTranslation(&localMatrix[0], screenWidth/2, -240.0f, 0.0f);
	D3DXMatrixTranslation(&localMatrix[0], (screenWidth - UIWidth) / 2 , -(screenHeight-UIHeight)/2, 0.0f);
	D3DXMATRIX backgroundMatrix;
	D3DXMATRIX ParticleRainMatrix;
	D3DXMATRIX localParticleMatrix;
	D3DXVECTOR3 camerapos = m_Camera->GetPosition();

	D3DXMatrixIdentity(&localParticleMatrix);
	m_D3D->GetWorldMatrix(ParticleRainMatrix);
	D3DXMatrixTranslation(&localParticleMatrix, camerapos.x, camerapos.y, camerapos.z+5);
	ParticleRainMatrix *= localParticleMatrix;
	m_D3D->GetWorldMatrix(backgroundMatrix);
	
	m_D3D->TurnOffCulling();
	m_D3D->TurnOnAlphaBlending();


	// Put the particle system vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_ParticleSystem->Render(m_D3D->GetDeviceContext());

	// Render the model using the texture shader.
	result = m_ParticleShader->Render(m_D3D->GetDeviceContext(), m_ParticleSystem->GetIndexCount(), ParticleRainMatrix, viewMatrix, projectionMatrix,
		m_ParticleSystem->GetTexture());
	if (!result)
	{
		return false;
	}

	// Turn off alpha blending.
	m_D3D->TurnOffAlphaBlending();

	m_D3D->TurnOnCulling();

	for (int i = 0; i < 139; i++) {
		if (powf(ColPoint[i].x - cameraPosition.x, 2) + powf(ColPoint[i].y - cameraPosition.z, 2) <= 4) {
			ShipHurt = true;
		}
	}


	if (ShipHurt == true) {
		HurtFrame--;
		if (HurtFrame <= 0) {
			ShipHurt = false;
			HurtFrame = FrameCount;
		}
		m_D3D->TurnOnAlphaBlending();
		int blendAmount = 1.0f;
		m_BillboardModel->Render(m_D3D->GetDeviceContext(), 0, 0);
		result = m_TransparentShader->Render(m_D3D->GetDeviceContext(), m_BillboardModel->GetIndexCount(), backgroundMatrix, baseViewMatrix,
			orthoMatrix, m_BillboardModel->GetTexture(), blendAmount);

		m_D3D->TurnOffAlphaBlending();
		m_Camera->Hurt = ShipHurt;

	}
	backgroundMatrix *= localMatrix[0];
	

D3DXMatrixRotationZ(&localMatrix[0], RotationAngle);
	backgroundMatrix *= localMatrix[0];

	D3DXMatrixTranslation(&localMatrix[0], (screenWidth - UIWidth) / 2, (screenHeight - UIHeight) / 2, 0.0f);
	backgroundMatrix *= localMatrix[0];

	D3DXMatrixIdentity(&localMatrix[1]);
	D3DXMatrixTranslation(&localMatrix[1], (screenWidth - UIWidth/2) / 2, -(screenHeight - UIHeight/2) / 2, 0.0f);

	D3DXMATRIX AimMatrix;
	m_D3D->GetWorldMatrix(AimMatrix);
	AimMatrix *= localMatrix[1];



	m_D3D->TurnZBufferOff();
	m_D3D->TurnOnAlphaBlending();

	m_Aim->Render(m_D3D->GetDeviceContext(), 0, 0);
	m_AimTextureShader->Render(m_D3D->GetDeviceContext(), m_Aim->GetIndexCount(), AimMatrix, baseViewMatrix, orthoMatrix, m_Aim->GetTexture());

	m_Bitmap->Render(m_D3D->GetDeviceContext(), 0, 0);

	m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Bitmap->GetIndexCount(), backgroundMatrix, baseViewMatrix, orthoMatrix, m_Bitmap->GetTexture());
	m_D3D->TurnOffAlphaBlending();
	m_D3D->TurnZBufferOn();
	D3DXMatrixIdentity(&localMatrix[1]);
/*	D3DXMATRIX SpecularMatrix;
	m_D3D->GetWorldMatrix(SpecularMatrix);
	D3DXMatrixTranslation(&localMatrix[1], m_Camera->GetPosition().x , m_Camera->GetPosition().y, m_Camera->GetPosition().z);

	SpecularMatrix *= localMatrix[1];
	*/

	if (m_Timer > 300.0f)
	{
		m_Sound->PlayWaveFile();
		m_Timer = 0.0f;
	}
	m_D3D->EndScene();

	return true;
}

void GraphicsClass::RenderRefractionToTexture()
{
	D3DXVECTOR4 clipPlane;
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix;


	// Setup a clipping plane based on the height of the water to clip everything above it to create a refraction.
	clipPlane = D3DXVECTOR4(0.0f, -1.0f, 0.0f, m_Water->GetWaterHeight() + 0.1f);

	// Set the render target to be the refraction render to texture.
	m_RefractionTexture->SetRenderTarget(m_D3D->GetDeviceContext());

	// Clear the refraction render to texture.
	m_RefractionTexture->ClearRenderTarget(m_D3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the matrices from the camera and d3d objects.
	m_D3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	// Render the terrain using the reflection shader and the refraction clip plane to produce the refraction effect.
	m_Terrain->Render(m_D3D->GetDeviceContext());
	m_ReflectionShader->Render(m_D3D->GetDeviceContext(), m_Terrain->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Terrain->GetColorTexture(), m_Terrain->GetNormalTexture(), m_Light->GetDiffuseColor(), m_Light->GetDirection(), 2.0f,
		clipPlane);

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_D3D->SetBackBufferRenderTarget();

	// Reset the viewport back to the original.
	m_D3D->ResetViewport();

	return;
}


void GraphicsClass::RenderReflectionToTexture()
{
	D3DXVECTOR4 clipPlane;
	D3DXMATRIX reflectionViewMatrix, worldMatrix, projectionMatrix;
	D3DXVECTOR3 cameraPosition;


	// Setup a clipping plane based on the height of the water to clip everything below it.
	clipPlane = D3DXVECTOR4(0.0f, 1.0f, 0.0f, -m_Water->GetWaterHeight());

	// Set the render target to be the reflection render to texture.
	m_ReflectionTexture->SetRenderTarget(m_D3D->GetDeviceContext());

	// Clear the reflection render to texture.
	m_ReflectionTexture->ClearRenderTarget(m_D3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	// Use the camera to render the reflection and create a reflection view matrix.
	m_Camera->RenderReflection(m_Water->GetWaterHeight());

	// Get the camera reflection view matrix instead of the normal view matrix.
	reflectionViewMatrix = m_Camera->GetReflectionViewMatrix();

	// Get the world and projection matrices from the d3d object.
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	// Get the position of the camera.
	cameraPosition = m_Camera->GetPosition();

	// Invert the Y coordinate of the camera around the water plane height for the reflected camera position.
	cameraPosition.y = -cameraPosition.y + (m_Water->GetWaterHeight() * 2.0f);

	// Translate the sky dome and sky plane to be centered around the reflected camera position.
	D3DXMatrixTranslation(&worldMatrix, cameraPosition.x, cameraPosition.y, cameraPosition.z);

	// Turn off back face culling and the Z buffer.
	m_D3D->TurnOffCulling();
	m_D3D->TurnZBufferOff();

	// Render the sky dome using the reflection view matrix.
	m_SkyDome->Render(m_D3D->GetDeviceContext());
	m_SkyDomeShader->Render(m_D3D->GetDeviceContext(), m_SkyDome->GetIndexCount(), worldMatrix, reflectionViewMatrix, projectionMatrix,
		m_SkyDome->GetApexColor(), m_SkyDome->GetCenterColor());

	// Enable back face culling.
	m_D3D->TurnOnCulling();

	// Enable additive blending so the clouds blend with the sky dome color.
	m_D3D->EnableSecondBlendState();

	// Render the sky plane using the sky plane shader.
	m_SkyPlane->Render(m_D3D->GetDeviceContext());
	m_SkyPlaneShader->Render(m_D3D->GetDeviceContext(), m_SkyPlane->GetIndexCount(), worldMatrix, reflectionViewMatrix, projectionMatrix,
		m_SkyPlane->GetCloudTexture(), m_SkyPlane->GetPerturbTexture(), m_SkyPlane->GetTranslation(), m_SkyPlane->GetScale(),
		m_SkyPlane->GetBrightness());

	// Turn off blending and enable the Z buffer again.
	m_D3D->TurnOffAlphaBlending();
	m_D3D->TurnZBufferOn();

	// Reset the world matrix.
	m_D3D->GetWorldMatrix(worldMatrix);

	// Render the terrain using the reflection view matrix and reflection clip plane.
	m_Terrain->Render(m_D3D->GetDeviceContext());
	m_ReflectionShader->Render(m_D3D->GetDeviceContext(), m_Terrain->GetIndexCount(), worldMatrix, reflectionViewMatrix, projectionMatrix,
		m_Terrain->GetColorTexture(), m_Terrain->GetNormalTexture(), m_Light->GetDiffuseColor(), m_Light->GetDirection(), 2.0f,
		clipPlane);

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_D3D->SetBackBufferRenderTarget();

	// Reset the viewport back to the original.
	m_D3D->ResetViewport();

	return;
}
