////////////////////////////////////////////////////////////////////////////////
//	Filename: modelclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "modelclass.h"

ModelClass::ModelClass()
{
	m_vertexBuffer = 0;

	m_indexBuffer = 0;

	m_model = 0;
	vertexCount = 0;
	textureCount = 0;
	normalCount = 0;
	faceCount = 0;
}


ModelClass::ModelClass(const ModelClass& other)
{
}


ModelClass::~ModelClass()
{
	CloseHandle(hThread);
}


int ModelClass::Get_FaceCount() {
	return faceCount;
}

DWORD WINAPI ModelInitialize(LPVOID pParam) {
	ModelClass* thisModel = (ModelClass*)pParam;

	bool result;



	// Read in the name of the model file.
	thisModel->GetModelFilename(thisModel->modelFilename);

	result = thisModel->ReadFileCounts(thisModel->modelFilename, thisModel->vertexCount, thisModel->textureCount, thisModel->normalCount, thisModel->faceCount);
	if (!result) { return false; }


	result = thisModel->LoadDataStructures(thisModel->device, thisModel->modelFilename, thisModel->vertexCount, thisModel->textureCount, thisModel->normalCount, thisModel->faceCount);
	//	result = LoadDataStructuresFatch(device, modelFilename, vertexCount, textureCount, normalCount, faceCount);

	if (!result) { return false; }


	result = thisModel->LoadTexture(thisModel->device, thisModel->textureFilename);
	if (!result)
	{
		return false;
	}

	thisModel->Header->Next = thisModel->Tail;
	thisModel->Header->Pre = thisModel->Tail;
	thisModel->Tail->Pre = thisModel->Header;
	thisModel->Tail->Next = thisModel->Header;
	thisModel->Mark = thisModel->Header;

	return 0;
}

bool ModelClass::InitializeByThread(ID3D11Device* device, char* modelFilename, WCHAR* textureFilename)
{

	this->device = device;
	this->modelFilename = modelFilename;
	this->textureFilename = textureFilename;

	hThread = CreateThread(0, 1, ModelInitialize, (LPVOID)this, 0, &ModelThreadID);

	return true;
}


bool ModelClass::Initialize(ID3D11Device* device, char* modelFilename, WCHAR* textureFilename)
{
	bool result;



	// Read in the name of the model file.
	GetModelFilename(modelFilename);

	result = ReadFileCounts(modelFilename, vertexCount, textureCount, normalCount, faceCount);
	if (!result) { return false; }


	result = LoadDataStructures(device, modelFilename, vertexCount, textureCount, normalCount, faceCount);
	//	result = LoadDataStructuresFatch(device, modelFilename, vertexCount, textureCount, normalCount, faceCount);

	if (!result) { return false; }


	result = LoadTexture(device, textureFilename);
	if (!result)
	{
		return false;
	}
	
	Header->Next = Tail;
	Header->Pre = Tail;
	Tail->Pre = Header;
	Tail->Next = Header;
	Mark = Header;
	return true;
}


void ModelClass::Shutdown()
{

	//	Release the vertex and index buffers. 
	ShutdownBuffers();

	//	Release the vertex and index buffers.
	ShutdownBuffers();

	//	Release the model data.
	ReleaseModel();
	return;
}

void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	WaitForSingleObject(hThread, INFINITE);
	//	Put the vertex and index buffers on the graphics pipeline to prepare them for drawing. 
	RenderBuffers(deviceContext);

	return;
}

int ModelClass::GetIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView* ModelClass::GetTexture()
{
	return m_Texture->GetTexture();
}

ID3D11ShaderResourceView* ModelClass::GetTexture1()
{
	return m_Texture1->GetTexture();
}

ID3D11ShaderResourceView* ModelClass::GetTexture2()
{
	return m_Texture2->GetTexture();
}

ID3D11ShaderResourceView* ModelClass::GetTexture3()
{
	return m_Texture3->GetTexture();
}


void ModelClass::ShutdownBuffers()
{
	//	Release the index buffer.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release(); m_indexBuffer = 0;

	}

	//	Release the vertex buffer.
	if (m_vertexBuffer)
	{

		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}


void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;

	unsigned int offset;


	//	Set vertex buffer stride and offset. 
	stride = sizeof(VertexType);

	offset = 0;

	//	Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	//	Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//	Set the type of primitive that should be rendered from this vertex buffer, in this case triangles. 
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//deviceContext->IASetPrimitiveTopology()
	return;
}

bool ModelClass::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	bool result;


	// Create the texture object.
	m_Texture = new TextureClass;

	if (!m_Texture)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture->Initialize(device, filename);
	if (!result)

	{
		return false;

	}

	return true;
}



void ModelClass::ReleaseTexture()
{

	//	Release the texture object.
	if (m_Texture)
	{
		m_Texture->Shutdown(); delete m_Texture; m_Texture = 0;
	}

	return;
}


bool ModelClass::LoadModel(char* filename)
{
	ifstream fin;
	char input;

	int i;


	//	Open the model file.
	fin.open(filename);

	//	If it could not open the file then exit. 
	if (fin.fail())
	{
		return false;
	}

	//	Read up to the value of vertex count.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	//	Read in the vertex count.
	fin >> m_vertexCount;

	//	Set the number of indices to be the same as the vertex count.
	m_indexCount = m_vertexCount;

	//	Create the model using the vertex count that was read in.
	m_model = new ModelType[m_vertexCount];
	if (!m_model)
	{

		return false;
	}

	//	Read up to the beginning of the data.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);

	}
	fin.get(input);

	fin.get(input);

	//	Read in the vertex data. 
	for (i = 0; i < m_vertexCount; i++)
	{
		fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		fin >> m_model[i].tu >> m_model[i].tv;
		fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
	}

	//	Close the model file.
	fin.close();

	return true;
}

void ModelClass::ReleaseModel()
{
	if (m_model)

	{
		delete[] m_model;
		m_model = 0;
	}

	return;
}






void ModelClass::GetModelFilename(char* filename) {
	bool done;
	ifstream fin;


	// Loop until we have a file name. 
	done = false;
	while (!done) {   // Ask the user for the filename.   
		// Read in the filename. 
		// Attempt to open the file.  
		fin.open(filename);
		if (fin.good()) {    // If the file exists and there are no problems then exit since we have the file name.   
			done = true;
		}
		else {    // If the file does not exist or there was an issue opening it then notify the user and repeat the process. 
			fin.clear();

		}
	}
	return;
}


bool ModelClass::ReadFileCounts(char* filename, int& vertexCount, int& textureCount, int& normalCount, int& faceCount) {
	ifstream fin;
	char input;


	// Initialize the counts.
	// Open the file.  
	fin.open(filename);
	// Check if it was successful in opening the file. 
	if (fin.fail() == true) { return false; }
	// Read from the file and continue to read until the end of the file is reached. 
	fin.get(input);
	while (!fin.eof()) {   // If the line starts with 'v' then count either the vertex, the texture coordinates, or the normal vector. 
		if (input == 'v') {
			fin.get(input);
			if (input == ' ') {
				vertexCount++;
			}
			if (input == 't') {
				textureCount++;
			}
			if (input == 'n') {
				normalCount++;
			}
		}
		// If the line starts with 'f' then increment the face count.  
		if (input == 'f') {
			fin.get(input);
			if (input == ' ') {
				faceCount++;
			}
		}      // Otherwise read in the remainder of the line.  
		while (input != '\n') {
			fin.get(input);
		}
		// Start reading the beginning of the next line.  
		fin.get(input);
	}
	// Close the file. 
	fin.close();
	return true;
}


bool ModelClass::LoadDataStructures(ID3D11Device* device, char* filename, int vertexCount, int textureCount, int normalCount, int faceCount)
{
	VertexTypeObj *verticesObj, *texcoordsObj, *normalsObj;
	FaceType *faces;
	ifstream fin;
	int vertexIndex, texcoordIndex, normalIndex, faceIndex, vIndex, tIndex, nIndex;
	char input, input2;
	ofstream fout;

	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	m_vertexCount = faceCount * 3;
	m_indexCount = m_vertexCount;
	m_model = new ModelType[m_vertexCount]; //모델을 이루는 점들의 집합
	if (!m_model)
	{

		return false;
	}
	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];

	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}
	// Initialize the four data structures. 
	verticesObj = new VertexTypeObj[vertexCount];
	if (!verticesObj) { return false; }
	texcoordsObj = new VertexTypeObj[textureCount];
	if (!texcoordsObj) { return false; }
	normalsObj = new VertexTypeObj[normalCount];
	if (!normalsObj) { return false; }
	faces = new FaceType[faceCount];  //면을 구성하는데 필요한 점들의 집합
	if (!faces) { return false; }
	// Initialize the indexes. 
	vertexIndex = 0;  texcoordIndex = 0;  normalIndex = 0;  faceIndex = 0;
	// Open the file. 
	fin.open(filename);
	// Check if it was successful in opening the file. 
	if (fin.fail() == true) { return false; }
	// Read in the vertices, texture coordinates, and normals into the data structures.  // Important: Also convert to left hand coordinate system since Maya uses right hand coordinate system. 
	fin.get(input);
	while (!fin.eof()) {
		if (input == 'v') {
			fin.get(input);
			// Read in the vertices.    
			if (input == ' ') {
				fin >> m_model[vertexIndex].x >> m_model[vertexIndex].y >> m_model[vertexIndex].z;
				// Invert the Z vertex to change to left hand system.    
				m_model[vertexIndex].z = m_model[vertexIndex].z * -1.0f;
				vertexIndex++;
			}
			// Read in the texture uv coordinates.   
			if (input == 't') {
				fin >> m_model[texcoordIndex].tu >> m_model[texcoordIndex].tv;
				// Invert the V texture coordinates to left hand system. 
				m_model[texcoordIndex].tv = 1.0f - m_model[texcoordIndex].tv;
				texcoordIndex++;
			}
			// Read in the normals.   
			if (input == 'n') {
				fin >> m_model[normalIndex].nx >> m_model[normalIndex].ny >> m_model[normalIndex].nz;
				// Invert the Z normal to change to left hand system.  
				m_model[normalIndex].nz = m_model[normalIndex].nz * -1.0f;
				normalIndex++;
			}
		} /// m_model 에 좌표 전부삽입
			// Read in the faces. 
		if (input == 'f') {
			fin.get(input);
			if (input == ' ')
			{     // Read the face data in backwards to convert it to a left hand system from right hand system.  
				fin >> faces[faceIndex].vIndex3 >> input2 >> faces[faceIndex].tIndex3 >> input2 >> faces[faceIndex].nIndex3
					>> faces[faceIndex].vIndex2 >> input2 >> faces[faceIndex].tIndex2 >> input2 >> faces[faceIndex].nIndex2
					>> faces[faceIndex].vIndex1 >> input2 >> faces[faceIndex].tIndex1 >> input2 >> faces[faceIndex].nIndex1;
				faceIndex++;
			}
		}
		// Read in the remainder of the line.  
		while (input != '\n') { fin.get(input); }
		// Start reading the beginning of the next line.   
		fin.get(input);
	}
	// Close the file. 
	fin.close();
	// Open the output file.
	 // Write out the file header that our model format uses.
	// Now loop through all the faces and output the three vertices for each face. 


	int j = 0;
	//	Load the vertex array and index array with data. 
	for (i = 0; i < faceIndex; i++)//vertexIndex
	{


		indices[j] = j;
		vIndex = faces[i].vIndex1 - 1;   tIndex = faces[i].tIndex1 - 1;   nIndex = faces[i].nIndex1 - 1;
		vertices[j].position = D3DXVECTOR3(m_model[vIndex].x, m_model[vIndex].y, m_model[vIndex].z);
		vertices[j].texture = D3DXVECTOR2(m_model[tIndex].tu, m_model[tIndex].tv);
		vertices[j].normal = D3DXVECTOR3(m_model[nIndex].nx, m_model[nIndex].ny, m_model[nIndex].nz);
		j++;
		indices[j] = j;
		vIndex = faces[i].vIndex2 - 1;   tIndex = faces[i].tIndex2 - 1;   nIndex = faces[i].nIndex2 - 1;
		vertices[j].position = D3DXVECTOR3(m_model[vIndex].x, m_model[vIndex].y, m_model[vIndex].z);
		vertices[j].texture = D3DXVECTOR2(m_model[tIndex].tu, m_model[tIndex].tv);
		vertices[j].normal = D3DXVECTOR3(m_model[nIndex].nx, m_model[nIndex].ny, m_model[nIndex].nz);
		j++;
		indices[j] = j;
		vIndex = faces[i].vIndex3 - 1;   tIndex = faces[i].tIndex3 - 1;   nIndex = faces[i].nIndex3 - 1;
		vertices[j].position = D3DXVECTOR3(m_model[vIndex].x, m_model[vIndex].y, m_model[vIndex].z);
		vertices[j].texture = D3DXVECTOR2(m_model[tIndex].tu, m_model[tIndex].tv);
		vertices[j].normal = D3DXVECTOR3(m_model[nIndex].nx, m_model[nIndex].ny, m_model[nIndex].nz);
		j++;


	}

	// Close the output file.
	// Release the four data structures.
	if (verticesObj) { delete[] verticesObj;   verticesObj = 0; }
	if (texcoordsObj) { delete[] texcoordsObj;   texcoordsObj = 0; }
	if (normalsObj) { delete[] normalsObj;   normalsObj = 0; }
	if (faces) { delete[] faces;   faces = 0; }





	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0; vertexBufferDesc.StructureByteStride = 0;


	//	Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	//	Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	//	Set up the description of the static index buffer. 
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0; indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	//	Give the subresource structure a pointer to the index data. 
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	//	Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer); if (FAILED(result))

	{
		return false;
	}

	//	Release the arrays now that the vertex and index buffers have been created and loaded. 
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

bool ModelClass::LoadDataStructuresFatch(ID3D11Device* device, char* filename, int vertexCount, int textureCount, int normalCount, int faceCount) {
	VertexTypeObj *verticesObj, *texcoordsObj, *normalsObj;
	RectFace *faces;
	ifstream fin;
	int vertexIndex, texcoordIndex, normalIndex, faceIndex, vIndex, tIndex, nIndex;
	char input, input2;
	ofstream fout;

	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	m_vertexCount = faceCount * 4;
	m_indexCount = m_vertexCount;
	m_model = new ModelType[m_vertexCount]; //모델을 이루는 점들의 집합
	if (!m_model)
	{

		return false;
	}
	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];

	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}
	// Initialize the four data structures. 
	verticesObj = new VertexTypeObj[vertexCount];
	if (!verticesObj) { return false; }
	texcoordsObj = new VertexTypeObj[textureCount];
	if (!texcoordsObj) { return false; }
	normalsObj = new VertexTypeObj[normalCount];
	if (!normalsObj) { return false; }
	faces = new RectFace[faceCount];  //면을 구성하는데 필요한 점들의 집합
	if (!faces) { return false; }
	// Initialize the indexes. 
	vertexIndex = 0;  texcoordIndex = 0;  normalIndex = 0;  faceIndex = 0;
	// Open the file. 
	fin.open(filename);
	// Check if it was successful in opening the file. 
	if (fin.fail() == true) { return false; }
	// Read in the vertices, texture coordinates, and normals into the data structures.  // Important: Also convert to left hand coordinate system since Maya uses right hand coordinate system. 
	fin.get(input);
	while (!fin.eof()) {
		if (input == 'v') {
			fin.get(input);
			// Read in the vertices.    
			if (input == ' ') {
				fin >> m_model[vertexIndex].x >> m_model[vertexIndex].y >> m_model[vertexIndex].z;
				// Invert the Z vertex to change to left hand system.    
				m_model[vertexIndex].z = m_model[vertexIndex].z * -1.0f;
				vertexIndex++;
			}
			// Read in the texture uv coordinates.   
			if (input == 't') {
				fin >> m_model[texcoordIndex].tu >> m_model[texcoordIndex].tv;
				// Invert the V texture coordinates to left hand system. 
				m_model[texcoordIndex].tv = 1.0f - m_model[texcoordIndex].tv;
				texcoordIndex++;
			}
			// Read in the normals.   
			if (input == 'n') {
				fin >> m_model[normalIndex].nx >> m_model[normalIndex].ny >> m_model[normalIndex].nz;
				// Invert the Z normal to change to left hand system.  
				m_model[normalIndex].nz = m_model[normalIndex].nz * -1.0f;
				normalIndex++;
			}
		} /// m_model 에 좌표 전부삽입
		  // Read in the faces. 
		if (input == 'f') {
			fin.get(input);
			if (input == ' ')
			{     // Read the face data in backwards to convert it to a left hand system from right hand system.  
				fin >> faces[faceIndex].vIndex4 >> input2 >> faces[faceIndex].tIndex4 >> input2 >> faces[faceIndex].nIndex4
					>> faces[faceIndex].vIndex3 >> input2 >> faces[faceIndex].tIndex3 >> input2 >> faces[faceIndex].nIndex3
					>> faces[faceIndex].vIndex2 >> input2 >> faces[faceIndex].tIndex2 >> input2 >> faces[faceIndex].nIndex2
					>> faces[faceIndex].vIndex1 >> input2 >> faces[faceIndex].tIndex1 >> input2 >> faces[faceIndex].nIndex1;
				faceIndex++;
			}
		}
		// Read in the remainder of the line.  
		while (input != '\n') { fin.get(input); }
		// Start reading the beginning of the next line.   
		fin.get(input);
	}
	// Close the file. 
	fin.close();
	// Open the output file.
	// Write out the file header that our model format uses.
	// Now loop through all the faces and output the three vertices for each face. 


	int j = 0;
	//	Load the vertex array and index array with data. 
	for (i = 0; i < faceIndex; i++)//vertexIndex
	{

		indices[j] = j;
		vIndex = faces[i].vIndex4 - 1;   tIndex = faces[i].tIndex4 - 1;   nIndex = faces[i].nIndex4 - 1;
		vertices[j].position = D3DXVECTOR3(m_model[vIndex].x, m_model[vIndex].y, m_model[vIndex].z);
		vertices[j].texture = D3DXVECTOR2(m_model[tIndex].tu, m_model[tIndex].tv);
		vertices[j].normal = D3DXVECTOR3(m_model[nIndex].nx, m_model[nIndex].ny, m_model[nIndex].nz);
		j++;


		indices[j] = j;
		vIndex = faces[i].vIndex3 - 1;   tIndex = faces[i].tIndex3 - 1;   nIndex = faces[i].nIndex3 - 1;
		vertices[j].position = D3DXVECTOR3(m_model[vIndex].x, m_model[vIndex].y, m_model[vIndex].z);
		vertices[j].texture = D3DXVECTOR2(m_model[tIndex].tu, m_model[tIndex].tv);
		vertices[j].normal = D3DXVECTOR3(m_model[nIndex].nx, m_model[nIndex].ny, m_model[nIndex].nz);
		j++;

		indices[j] = j;
		vIndex = faces[i].vIndex2 - 1;   tIndex = faces[i].tIndex2 - 1;   nIndex = faces[i].nIndex2 - 1;
		vertices[j].position = D3DXVECTOR3(m_model[vIndex].x, m_model[vIndex].y, m_model[vIndex].z);
		vertices[j].texture = D3DXVECTOR2(m_model[tIndex].tu, m_model[tIndex].tv);
		vertices[j].normal = D3DXVECTOR3(m_model[nIndex].nx, m_model[nIndex].ny, m_model[nIndex].nz);
		j++;

		indices[j] = j;
		vIndex = faces[i].vIndex1 - 1;   tIndex = faces[i].tIndex1 - 1;   nIndex = faces[i].nIndex1 - 1;
		vertices[j].position = D3DXVECTOR3(m_model[vIndex].x, m_model[vIndex].y, m_model[vIndex].z);
		vertices[j].texture = D3DXVECTOR2(m_model[tIndex].tu, m_model[tIndex].tv);
		vertices[j].normal = D3DXVECTOR3(m_model[nIndex].nx, m_model[nIndex].ny, m_model[nIndex].nz);
		j++;

	}

	// Close the output file.
	// Release the four data structures.
	if (verticesObj) { delete[] verticesObj;   verticesObj = 0; }
	if (texcoordsObj) { delete[] texcoordsObj;   texcoordsObj = 0; }
	if (normalsObj) { delete[] normalsObj;   normalsObj = 0; }
	if (faces) { delete[] faces;   faces = 0; }





	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0; vertexBufferDesc.StructureByteStride = 0;


	//	Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	//	Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	//	Set up the description of the static index buffer. 
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0; indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	//	Give the subresource structure a pointer to the index data. 
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	//	Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer); if (FAILED(result))

	{
		return false;
	}

	//	Release the arrays now that the vertex and index buffers have been created and loaded. 
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

void ModelClass::CreateAllocatePosition(struct Position* Header, float x, float y, float z, float rotation)
{
	WaitForSingleObject(hThread,INFINITE);
	DP *Destination;
	
	Destination = new DP;
		
	Destination->DestinationPositionX = x;
	Destination->DestinationPositionY = y;
	Destination->DestinationPositionZ = z;
	Destination->Rotation = rotation;

	Destination->Next = Header->Next;
	Destination->Pre = Header;
	Header->Next->Pre = Destination;
	Header->Next = Destination;
	
}

void ModelClass::Move(struct Position** Mark, float Speed)
{	 
	WaitForSingleObject(hThread, INFINITE);
	if (Mark == NULL)
	{
		return;
	}

	else
	{
		if ((int)CurrentPositionX == (int)(*Mark)->Next->DestinationPositionX&& (int)CurrentPositionY == (int)(*Mark)->Next->DestinationPositionY
			&& (int)CurrentPositionZ == (int)(*Mark)->Next->DestinationPositionZ&& (*Mark)->Next != NULL)
		{
			*(Mark) = (*Mark)->Next;
		}
		
		if ((*Mark)->Next == Tail||(*Mark)->Next == Header)
		{
			*Mark = (*Mark)->Next;
		}

		if (CurrentPositionX < (*Mark)->Next->DestinationPositionX)
		{
			CurrentPositionX += Speed;
		}

		else if (CurrentPositionX >(*Mark)->Next->DestinationPositionX)
		{
			CurrentPositionX -= Speed;
		}

		
		if (CurrentPositionY < (*Mark)->Next->DestinationPositionY)
		{
			CurrentPositionY += Speed;
		}

		else if (CurrentPositionY >(*Mark)->Next->DestinationPositionY)
		{
			CurrentPositionY -= Speed;
		}
		
		if (CurrentPositionZ < (*Mark)->Next->DestinationPositionZ)
		{
			CurrentPositionZ += Speed;
		}

		else if (CurrentPositionZ >(*Mark)->Next->DestinationPositionZ)
		{
			CurrentPositionZ -= Speed;
		}
		
		if (CurrentDgree < (*Mark)->Next->Rotation)
		{
			CurrentDgree += 0.05f;
		}

		else if (CurrentDgree > (*Mark)->Next->Rotation)
		{
			CurrentDgree -= 0.05f;
		}
	}
}

DWORD WINAPI EffectInitialize(LPVOID pParam) {

	ModelClass* thisModel = (ModelClass*)pParam;

	bool result;

	// Read in the name of the model file.
	thisModel->GetModelFilename(thisModel->modelFilename);

	result = thisModel->ReadFileCounts(thisModel->modelFilename, thisModel->vertexCount, thisModel->textureCount, thisModel->normalCount, thisModel->faceCount);
	if (!result) { return false; }


	result = thisModel->LoadDataStructures(thisModel->device, thisModel->modelFilename, thisModel->vertexCount, thisModel->textureCount, thisModel->normalCount, thisModel->faceCount);
	//	result = LoadDataStructuresFatch(device, modelFilename, vertexCount, textureCount, normalCount, faceCount);

	if (!result) { return false; }

	result = thisModel->LoadTextures(thisModel->device, thisModel->textureFilename, thisModel->textureFilename2, thisModel->textureFilename3);
	if (!result)
	{
		return false;
	}



}

bool  ModelClass::InitializeEffect(ID3D11Device* device, char* modelFilename, WCHAR* textureFilename1, WCHAR* textureFilename2,
	WCHAR* textureFilename3)
{

	this->device = device;
	this->modelFilename = modelFilename;
	this->textureFilename = textureFilename1;
	this->textureFilename2 = textureFilename2;
	this->textureFilename3 = textureFilename3;

	hThread = CreateThread(NULL, 0, EffectInitialize, (LPVOID)this, 0, &EffectdwThreadID);

	return true;
}

bool  ModelClass::LoadTextures(ID3D11Device* device, WCHAR* textureFilename1, WCHAR* textureFilename2, WCHAR* textureFilename3)
{
	bool result;

	// Create the texture object.
	m_Texture1 = new TextureClass;
	if (!m_Texture1)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture1->Initialize(device, textureFilename1);
	if (!result)
	{
		return false;
	}

	m_Texture2 = new TextureClass;
	if (!m_Texture2)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture2->Initialize(device, textureFilename2);
	if (!result)
	{
		return false;
	}

	// Create the texture object.
	m_Texture3 = new TextureClass;
	if (!m_Texture3)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture3->Initialize(device, textureFilename3);
	if (!result)
	{
		return false;
	}

	return true;
}

bool ModelClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;


	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// Load the vertex array and index array with data.
	for (i = 0; i<m_vertexCount; i++)
	{
		vertices[i].position = D3DXVECTOR3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = D3DXVECTOR2(m_model[i].tu, m_model[i].tv);
		indices[i] = i;
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}
