#pragma once
#include "Interface/IBuilder.h"

/// Dll 매크로
#ifdef FOOTGRAPHICSENGINE_EXPORTS
#define GRAPHICSENGINE_DECLSPEC __declspec(dllexport)
#else
#define GRAPHICSENGINE_DECLSPEC __declspec(dllimport)
#endif

namespace GraphicsEngineSpace
{
	class IDXObject;
	class IBuilder;
	// 팩토리가 오브젝트 생성에 필요한 디바이스와 DC를 가지고 빌더를 가지고 생성한다.

	/**
	 * \brief 빌더를 통해서 각종 오브젝트를 만드는 팩토리 함수..
	 *
	 * 내부적으로 필요한 함수만 dllexport 한다.
	 * 2022.06.21(화) Dev.BlackFoot
	 */
	class Factory
	{
	private:
		static std::shared_ptr<Factory> instance;

	public:
		// 템플릿은 dll로 뽑을 수 없다. 다만 헤더에 존재하기 때문에 이걸 그대로 사용할 수 있지 않을까.

		// 특정 상황마다 추가되는 args들이 달라지기 때문에.. 해당하는 내용을 한번에 처리할 수 있는 가변 인자 템플릿을 사용.
			// 사실상 DiffuseMap과 NormalMap텍스쳐 관련 함수를 Builder에서 만들도록 하기 위한 함수이다.
		template <typename T>
		std::shared_ptr<IDXObject> CreateDXObject(std::shared_ptr<IBuilder> _builder, std::string _objName);

		GRAPHICSENGINE_DECLSPEC static std::shared_ptr<Factory> GetInstance()
		{
			if (instance == nullptr)
			{
				instance = std::make_shared<Factory>();
			}

			return instance;
		}
		GRAPHICSENGINE_DECLSPEC void DeleteFactory();
	};

	// 특정 상황에 맞는 오브젝트를 상속 받는 친구가 만들어 주려고 하기 위해 가변 인자 템플릿을 사용.

	/**
	 * \brief IDXObject를 상속받는 오브젝트를 만들고 내용을 추가하는 함수
	 * \tparam T 만들어질 오브젝트의 종류
	 * \tparam Types 가변인자로 사용될 타입들
	 * \param _builder 오브젝트에 정보를 넣어줄 빌더
	 * \param _objName 오브젝트의 이름
	 * \param args 각종 인자들, 여기서는 디퓨즈맵과 노말맵 등..
	 * \return 
	 */
	template <typename T>
	std::shared_ptr<IDXObject> Factory::CreateDXObject(std::shared_ptr<IBuilder> _builder, std::string _objName)
	{
		// 사실상 위와 같다.
		// 우선 오브젝트를 만들고
		std::shared_ptr<IDXObject> _tmpObj = std::make_shared<T>();

		// 빌더를 통해 내용을 추가해주는데, 해당 함수가 가변인자를 가진다.
		_tmpObj = _builder->BuildDXObject(_tmpObj, _objName);

		return _tmpObj;
	}
}
