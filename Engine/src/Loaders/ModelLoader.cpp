#include "ModelLoader.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

std::vector<Mesh> ModelLoader::LoadMeshes(const std::string& path, ID3D11Device *device)
{
	Logger::Debug("Preparing to load mesh from: '" + path + "'...");

	std::vector<Mesh> returnedMeshes;
	Assimp::Importer importer;

	Logger::Debug("Reading data from file...");
	const auto scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);
	if (scene == nullptr)
		return returnedMeshes;

	const auto meshes = scene->mMeshes;
	for (auto i = 0; i < scene->mNumMeshes; i++)
	{
		auto mesh = meshes[i];
		std::vector<Vertex> vertices;
		std::vector<DWORD> indices;

		for (auto j = 0; j < mesh->mNumVertices; j++)
		{
			Vertex vertex;

			vertex.Position.x = mesh->mVertices[j].x;
			vertex.Position.y = mesh->mVertices[j].y;
			vertex.Position.z = mesh->mVertices[j].z;

			if (mesh->mTextureCoords[0])
			{
				vertex.TextureCoord.x = static_cast<float>(mesh->mTextureCoords[0][j].x);
				vertex.TextureCoord.y = static_cast<float>(mesh->mTextureCoords[0][j].y);
			}

			vertices.emplace_back(vertex);
		}

		for (auto j = 0; j < mesh->mNumFaces; j++)
		{
			const auto face = mesh->mFaces[j];

			for (auto k = 0; k < face.mNumIndices; k++)
			{
				indices.emplace_back(face.mIndices[k]);
			}
		}

		Mesh newMesh;
		newMesh.Name = mesh->mName.C_Str();
		auto hr = newMesh.RenderIndexBuffer.Initialize(device, indices.data(), indices.size());
		if (FAILED(hr))
			continue;
		hr = newMesh.RenderVertexBuffer.Initialize(device, vertices.data(), vertices.size());
		if (FAILED(hr))
			continue;
		returnedMeshes.emplace_back(newMesh);

		Logger::Debug("Mesh '" + newMesh.Name + "' added into the model!");
	}

	return returnedMeshes;
}
