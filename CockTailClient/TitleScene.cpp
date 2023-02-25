#include "pch.h"
#include "TitleScene.h"
#include "TitleMenuUIController.h"
#include "SettingUIController.h"
using namespace ClientSpace;



ClientSpace::TitleScene::TitleScene(tstring sceneName)
	:SceneBase(sceneName)
{
}

ClientSpace::TitleScene::~TitleScene()
{

}

void ClientSpace::TitleScene::BuildScene()
{
	CreateUI();
}

std::shared_ptr<GameObject> ClientSpace::TitleScene::CreateUI()
{
	std::shared_ptr<GameObject> ui = CreateEmpty();
	ui->AddComponent<TitleMenuUIController>();	// TitleMenu UI
	return ui;

}
