#pragma once

namespace GraphicsEngineSpace
{
	enum class RESOURCE_TYPE
	{
		NONE,
		MATERIAL,
		MESH,
		TEXTURE,

		END
	};

	enum
	{
		RESOURCE_TYPE_COUNT = static_cast<uint8>(RESOURCE_TYPE::END)
	};

	class Resource
	{
	protected:
		friend class ResourceManager;

	public:
		Resource(RESOURCE_TYPE type)
			: resourceType(type)
		{
			static uint32 idGenerator = 0;

			id = idGenerator;

			idGenerator++;

			name = "";
		}
		virtual ~Resource() {}

	public:
		RESOURCE_TYPE GetType() { return resourceType; }

		void SetName(const std::string& _name) { name = _name; }

		const std::string& GetName() { return name; }

		virtual void Load(const std::wstring& path) {}

	protected:
		RESOURCE_TYPE resourceType = RESOURCE_TYPE::NONE;

		std::string name;

	protected:
		uint64 id = 0;
	};
}