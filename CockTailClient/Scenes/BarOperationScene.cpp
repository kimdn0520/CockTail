#include "pch.h"
#include "BarOperationScene.h"

#include "InGameUIController.h"
#include "Util/JsonReader.h"

namespace ClientSpace
{
	BarOperationScene::BarOperationScene(tstring sceneName)
		:SceneBase(sceneName)
	{
	}

	BarOperationScene::~BarOperationScene()
	{
	}

	void BarOperationScene::BuildScene()
	{
		std::unique_ptr<JsonReader> jsonReader = std::make_unique<JsonReader>("Resources/Table/ItemDataTable.json");

		std::vector<JsonObject> jsonObjects = jsonReader->GetJsonObjects();

		for (auto jsonObject : jsonObjects)
		{
			std::string itemName = jsonObject.GetString("ItemName");
			int idx = jsonObject.GetInt("Index");
			int itemType = jsonObject.GetInt("ItemType");
			int itemCombineNUm = jsonObject.GetInt("ItemCombineNum");
	
		}



		CreateUI();
	}

	std::shared_ptr<GameObject> ClientSpace::BarOperationScene::CreateUI()
	{
		std::shared_ptr<GameObject> ui = CreateEmpty();
		ui->AddComponent<InGameUIController>();
		return ui;
	}
}