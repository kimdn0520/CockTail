#pragma once
#include "GraphicsCore/RasterizerState.h"
#include "Resources/Mesh.h"
#include "Resources/Texture.h"
#include "Define/YAMLBinaryLayout.h"

#ifdef FOOTGRAPHICSENGINE_EXPORTS
#define GRAPHICSENGINE_DECLSPEC __declspec(dllexport)
#else
#define GRAPHICSENGINE_DECLSPEC __declspec(dllimport)
#endif

namespace GraphicsEngineSpace
{
	class IDXObject;

	class UnityScene;

	// 게임 엔진에서 사용 가능하도록 DLL Export를 해준다.
	class GRAPHICSENGINE_DECLSPEC ResourceManager
	{
	private:
		static std::shared_ptr<ResourceManager> instance;

	public:
		ResourceManager()
		{}
		~ResourceManager() {};

		static std::shared_ptr<ResourceManager> GetInstance();

	private:
		std::unordered_map<std::string, std::shared_ptr<IDXObject>> prefab;

	private:
		std::string meshPath;
		std::string materialPath;
		std::string AnimationPath;
		std::string modelPath;
		std::string prefabPath;

	public:
		// 내부에서 들고 있을 기본 메시들
		std::unordered_map<std::string, std::shared_ptr<Mesh>> basicMeshes;

	public:
		std::shared_ptr<Mesh> LoadMesh(std::string name);

		std::shared_ptr<Texture> LoadTexture(std::wstring path);

		//std::shared_ptr<NavMesh> LoadNavMesh(std::string name);

	public:
		// 메시를 만들어서 넘겨준다.
		template <typename T>
		const std::shared_ptr<Mesh> CreateMesh(std::vector<T> vertices, std::vector<std::vector<unsigned int>> indices, std::string name, D3D_PRIMITIVE_TOPOLOGY topology, ComPtr<ID3D11RasterizerState> rasterizerState);
		
		void Initialize();

		void LoadCubeMesh();			// CUBE_MESH = 0;

		void LoadSphereMesh();			// SPHERE_MESH = 1;

		void LoadAxisMesh();			// AXIS_MESH = 2;

		void LoadGridMesh();			// GRID_MESH = 3;

		void LoadScreenMesh();			// SCREEN_MESH = 4;

		void LoadDebugScreenMesh();		// DEBUG_SCREEN_MESH = 5;

		void LoadWireCubeMesh();		// WIRECUBE_MESH = 6;

		void LoadPlaneMesh();			// PLANE_MESH = 7;

		void LoadPrefab();

		std::shared_ptr<YAMLBinaryData::Scene> LoadUnityScene(std::string path);
	};

	template<typename T>
	inline const std::shared_ptr<Mesh> ResourceManager::CreateMesh(std::vector<T> vertices, std::vector<std::vector<unsigned int>> indices, std::string name, D3D_PRIMITIVE_TOPOLOGY topology, ComPtr<ID3D11RasterizerState> rasterizerState)
	{
		// TODO : unique가 맞는가?
		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();

		mesh->SetName(name);

		mesh->SetPrimitiveTopology(topology);
		
		mesh->SetRenderState(rasterizerState);

		mesh->CreateVertexBuffer(vertices);
		for (int i = 0; i < indices.size(); i++)
		{
			mesh->CreateIndexBuffer(indices[i]);
		}
		mesh->SetStride(sizeof(T));

		return mesh;
	}
}