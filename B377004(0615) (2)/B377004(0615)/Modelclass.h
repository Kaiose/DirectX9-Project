////////////////////////////////////////////////////////////////////////////////
//	Filename: modelclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MODELCLASS_H_ 
#define _MODELCLASS_H_

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include <fstream>
#include <iostream>
using namespace std;
#define SIZE 19
///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "textureclass.h"


////////////////////////////////////////////////////////////////////////////////
//	Class name: ModelClass
////////////////////////////////////////////////////////////////////////////////
DWORD WINAPI ModelInitialize(LPVOID pParam);
DWORD WINAPI EffectInitialize(LPVOID pParam);
class ModelClass
{
private:
	 struct VertexType
	{

		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	};



	struct ModelType
	{
		float x, y, z;

		float tu, tv;
		float nx, ny, nz;
	};

public:
	HANDLE hThread;
	DWORD ModelThreadID;
	DWORD EffectdwThreadID;
	ID3D11Device* device;
	char* modelFilename;
	WCHAR* textureFilename;
	WCHAR* textureFilename2;
	WCHAR* textureFilename3;
	int Get_FaceCount();
	int vertexCount, textureCount, normalCount, faceCount;
	
	typedef struct VT {
		float x, y, z;
	}VertexTypeObj;

	typedef struct FT{
		int vIndex1, vIndex2, vIndex3;
		int tIndex1, tIndex2, tIndex3;
		int nIndex1, nIndex2, nIndex3;
	}FaceType;

	typedef struct FT4 {
		int vIndex1, vIndex2, vIndex3, vIndex4;
		int tIndex1, tIndex2, tIndex3, tIndex4;
		int nIndex1, nIndex2, nIndex3, nIndex4;
	}RectFace;

	typedef struct Position
	{
		float DestinationPositionX;
		float DestinationPositionY;
		float DestinationPositionZ;
		float Rotation;
		struct Position* Next;
		struct Position* Pre;
	}DP;

	ModelClass();

	ModelClass(const ModelClass&);
	~ModelClass();
	
	bool Initialize(ID3D11Device*, char*, WCHAR*);
	bool InitializeByThread(ID3D11Device* device, char* modelFilename, WCHAR* textureFilename);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
	void GetModelFilename(char*);

	DP *Header = new DP;
	DP *Tail = new DP;
	DP *Mark;
	float CurrentPositionX, CurrentPositionY, CurrentPositionZ, CurrentDgree;
	void Move(struct Position** Mark,float Speed);
	void CreateAllocatePosition(struct Position* Header, float x, float y, float z, float rotation);

	bool ReadFileCounts(char*, int&, int&, int&, int&);
	bool LoadDataStructures(ID3D11Device* device, char*, int, int, int, int);
	bool LoadDataStructuresFatch(ID3D11Device* device, char*, int, int, int, int);
	ID3D11ShaderResourceView* GetTexture();


	void ShutdownBuffers();

	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();
	
	bool LoadModel(char*);
	void ReleaseModel();



	ID3D11Buffer * m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	TextureClass* m_Texture;

	ModelType* m_model;
	//////only for Effect
public:
	bool  InitializeEffect(ID3D11Device*, char*, WCHAR*, WCHAR*, WCHAR*);
	TextureClass *m_Texture1, *m_Texture2, *m_Texture3;
	bool LoadTextures(ID3D11Device*, WCHAR*, WCHAR*, WCHAR*);
	bool InitializeBuffers(ID3D11Device* device);
	ID3D11ShaderResourceView* GetTexture1();
	ID3D11ShaderResourceView* GetTexture2();
	ID3D11ShaderResourceView* GetTexture3();


};

#endif
