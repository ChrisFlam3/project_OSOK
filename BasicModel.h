#ifndef BASICMODEL_H
#define BASICMODEL_H

#include "MeshGeometry.h"
#include "TextureMgr.h"
#include "Vertex.h"

class BasicModel
{
public:
	BasicModel(ID3D11Device* device, TextureMgr& texMgr, const std::string& modelFilename, const std::wstring& texturePath);
	~BasicModel();

	UINT SubsetCount;

	
	std::vector<Material> Mat;
	std::vector<ID3D11ShaderResourceView*> DiffuseMapSRV;
	std::vector<ID3D11ShaderResourceView*> NormalMapSRV;

	// Zachowaj kopi� danych siatki w pami�ci systemowej, aby mo�na je by�o odczyta�.   
	std::vector<Vertex> Vertices;
	std::vector<int> Indices;
	std::vector<MeshGeometry::Subset> Subsets;

	MeshGeometry ModelMesh;
};

struct BasicModelInstance
{
	BasicModel* Model;
	XMFLOAT4X4 World;
};
#endif