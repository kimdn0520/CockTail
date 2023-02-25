#include "pch.h"
#include "BarLobby.h"

ClientSpace::BarLobby::BarLobby(tstring sceneName)
{
}

ClientSpace::BarLobby::~BarLobby()
{
}

void ClientSpace::BarLobby::BuildScene()
{
    CreateUI();
}

std::shared_ptr<GameObject> ClientSpace::BarLobby::CreateUI()
{
    std::shared_ptr<GameObject> ui = CreateEmpty();
    return ui;
}
