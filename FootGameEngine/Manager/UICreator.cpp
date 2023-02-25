#include "GamePch.h"
#include "UICreator.h"

#include "SceneManager.h"
#include "UIManager.h"
#include "GraphicsManager.h"

namespace GameEngineSpace
{
	void UICreator::CreateCanvasAll()
	{
		// 사용할 메인 캔버스를 가져옵니다.
		mainCanvas = UIManager::GetInstance()->GetMainCanvas();

		// 아래의 모든 UI를 생성합니다.
		CreateTitleCanvas();
		CreateSettingCanvas();
		CreateInMenuCanvas();
		CreateGameHUD();
		CreateCombinationCanvas();
		CreateTestSceneCanvas();
	}

	void UICreator::CreateTitleCanvas()
	{
		auto canvas = mainCanvas->GetCanvasUI("TitleCanvas");

		auto buttonTexture = GameEngineSpace::GraphicsManager::GetInstance()->LoadTexture(L"Resources/UI/Button2.png");

		auto backGround = GameEngineSpace::GraphicsManager::GetInstance()->LoadTexture(L"Resources/UI/titleTest.png");
		//==================(SettingBackground)===============================
		auto background = canvas->CreateSpriteUI("Background");
		background->SetTexture(backGround);
		background->SetPosition(Vector3{ 0.0f, 0.0f, 0.8f });
		background->SetWidth(1980.f);
		background->SetHeight(1180.f);

		//==================(Setting Buttons)===============================
		auto startButton = canvas->CreateButtonUI("StartButton");
		startButton->SetPosition(Vector3{ 0, 0, 0.7f });
		startButton->SetDefaultTexture(buttonTexture);
		startButton->SetWidth(300.f);
		startButton->SetHeight(80.f);

		auto startext = canvas->CreateTextUI("StartText");
		startext->SetPosition(Vector3{ 22.f, 0, 0.5f });
		std::wstring startString = L"Start";
		startext->SetText(startString);
		startext->SetFontSize(30.f);

		auto continueButton = canvas->CreateButtonUI("ContinueButton");
		continueButton->SetPosition(Vector3{ 0, 120, 0.7f });
		continueButton->SetDefaultTexture(buttonTexture);
		continueButton->SetWidth(300.f);
		continueButton->SetHeight(80.f);

		auto continuetext = canvas->CreateTextUI("ContinueText");
		std::wstring continuetextString = L"Continue";
		continuetext->SetPosition(Vector3{ 37.f, 117.f, 0.5f });
		continuetext->SetText(continuetextString);
		continuetext->SetFontSize(30.f);

		auto creditButton = canvas->CreateButtonUI("CreditButton");
		creditButton->SetPosition(Vector3{ 0.f, 240.f , 0.7f });
		creditButton->SetDefaultTexture(buttonTexture);
		creditButton->SetWidth(300.f);
		creditButton->SetHeight(80.f);

		auto credittext = canvas->CreateTextUI("CreditText");
		std::wstring CreditTextString = L"Credit";
		credittext->SetPosition(Vector3{ 27.f, 240.f, 0.5f });
		credittext->SetText(CreditTextString);
		credittext->SetFontSize(30.f);

		auto settingButton = canvas->CreateButtonUI("SettingButton");
		settingButton->SetPosition(Vector3{ 0.f, 360.f, 0.7f });
		settingButton->SetDefaultTexture(buttonTexture);
		settingButton->SetWidth(300.f);
		settingButton->SetHeight(80.f);

		auto settingtext = canvas->CreateTextUI("SettingText");
		std::wstring SettingtextTextString = L"Setting";
		settingtext->SetPosition(Vector3{ 36.f, 357.f, 0.5f });
		settingtext->SetText(SettingtextTextString);
		settingtext->SetFontSize(30.f);

		auto exitButton = canvas->CreateButtonUI("ExitButton");
		exitButton->SetPosition(Vector3{ 0.f, 480.f, 0.7f });
		exitButton->SetDefaultTexture(buttonTexture);
		exitButton->SetWidth(300.f);
		exitButton->SetHeight(80.f);

		auto exitText = canvas->CreateTextUI("ExitText");
		std::wstring exitTextString = L"Exit";
		exitText->SetPosition(Vector3{ 13.f, 473, 0.5f });
		exitText->SetText(exitTextString);
		exitText->SetFontSize(30.f);

		//==================(Setting SetClickEvents)===============================
		startButton->SetClickEvent([&]
		{
			//LobbyScene, DemoScene
			SceneManager::GetInstance()->LoadScene(TEXT("LobbyScene"));
			mainCanvas->GetCanvasUI("TitleCanvas")->SetEnable(false);
		});

		settingButton->SetClickEvent([&]
		{
			mainCanvas->GetCanvasUI("SettingCanvase")->SetEnable(true);

			mainCanvas->GetCanvasUI("TitleCanvas")->GetButtonUI("StartButton")->SetEnable(false);
			mainCanvas->GetCanvasUI("TitleCanvas")->GetTextUI("StartText")->SetEnable(false);

			mainCanvas->GetCanvasUI("TitleCanvas")->GetButtonUI("ContinueButton")->SetEnable(false);
			mainCanvas->GetCanvasUI("TitleCanvas")->GetTextUI("ContinueText")->SetEnable(false);

			mainCanvas->GetCanvasUI("TitleCanvas")->GetButtonUI("SettingButton")->SetEnable(false);
			mainCanvas->GetCanvasUI("TitleCanvas")->GetTextUI("SettingText")->SetEnable(false);

			mainCanvas->GetCanvasUI("TitleCanvas")->GetButtonUI("CreditButton")->SetEnable(false);
			mainCanvas->GetCanvasUI("TitleCanvas")->GetTextUI("CreditText")->SetEnable(false);

			mainCanvas->GetCanvasUI("TitleCanvas")->GetButtonUI("ExitButton")->SetEnable(false);
			mainCanvas->GetCanvasUI("TitleCanvas")->GetTextUI("ExitText")->SetEnable(false);

		});
		
		exitButton->SetClickEvent([&]
		{
			PostQuitMessage(0);
		});



		canvas->SetEnable(false);
	}

	void UICreator::CreateDialogCanvas()
	{
		auto canvas = mainCanvas->GetCanvasUI("DialogCanvas");

		auto dialogTexture = GameEngineSpace::GraphicsManager::GetInstance()->LoadTexture(L"Resources/UI/Dialog.png");

		auto buttonTexture = GameEngineSpace::GraphicsManager::GetInstance()->LoadTexture(L"Resources/UI/Button2.png");

		//==================(CharactersDialog & SelectDialog )===================================
		auto charactersDialog = canvas->CreateSpriteUI("CharactersDialog");
		charactersDialog->SetTexture(dialogTexture);
		charactersDialog->SetPosition(Vector3{ -327.f, -244.f, 0.4f });
		charactersDialog->SetWidth(535.f);
		charactersDialog->SetHeight(335.f);
		charactersDialog->SetEnable(false);

		auto selectDialogButtons1 = canvas->CreateButtonUI("SelectDialogButtons1");
		selectDialogButtons1->SetPosition(Vector3{ 415.f, -79.f, 0.4f });
		selectDialogButtons1->SetDefaultTexture(buttonTexture);
		selectDialogButtons1->SetWidth(580.f);
		selectDialogButtons1->SetHeight(60.f);
		selectDialogButtons1->SetEnable(false);

		auto selectDialogButtons2 = canvas->CreateButtonUI("SelectDialogButtons2");
		selectDialogButtons2->SetPosition(Vector3{ 415.f, 38.f, 0.4f });
		selectDialogButtons2->SetDefaultTexture(buttonTexture);
		selectDialogButtons2->SetWidth(580.f);
		selectDialogButtons2->SetHeight(60.f);
		selectDialogButtons2->SetEnable(false);
	}

	//  나중에 없어지는 UICanvas입니다
	void UICreator::CreateTestSceneCanvas()
	{
		auto canvas = mainCanvas->GetCanvasUI("SceneTestCanvas");
		auto buttonTexture = GameEngineSpace::GraphicsManager::GetInstance()->LoadTexture(L"Resources/UI/Button2.png");

		//==================(ClickEvents)===============================
		static enum ClickCount
		{
			Click0,
			Click1,
			Click2,
		};
		static ClickCount nowScene = ClickCount::Click0;

		//==================(TestSceneChangeButton)===============================
		auto testSceneButton = canvas->CreateButtonUI("TestSceneChangeButton");
		testSceneButton->SetPosition(Vector3{ -813.f, -145.f, 0.4f });
		testSceneButton->SetDefaultTexture(buttonTexture);
		testSceneButton->SetWidth(130.f);
		testSceneButton->SetHeight(60.f);
		testSceneButton->SetEnable(true);

		//==================(TestCombinationButton)===============================
		auto testButton = canvas->CreateButtonUI("TestButton");
		testButton->SetPosition(Vector3{ -813.f, -220.f, 0.4f });
		testButton->SetDefaultTexture(buttonTexture);
		testButton->SetWidth(130.f);
		testButton->SetHeight(60.f);
		testButton->SetEnable(false);

		//==================(TestCombinationButtonClickEvents)===============================
		/*testButton->SetClickEvent([&]
		{
			SceneManager::GetInstance()->LoadScene(TEXT("BarOperationScene"));

			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("CombinationBackGround")->SetEnable(true);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("CombinationBoxBackGround")->SetEnable(true);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("StuffResultTextrue")->SetEnable(true);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("ReQuest")->SetEnable(true);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("Customer1")->SetEnable(true);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("CharactersDialog")->SetEnable(true);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("CashierMachine")->SetEnable(true);

			mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("RumButton")->SetEnable(true);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("VodkaButton")->SetEnable(true);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("GinButton")->SetEnable(true);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("LiquorButton")->SetEnable(true);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("JuiceButton")->SetEnable(true);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("SodaButton")->SetEnable(true);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("FruitButton")->SetEnable(true);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("StuffButton")->SetEnable(true);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("StuffButton2")->SetEnable(true);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("StuffButton3")->SetEnable(true);

			mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("BeerMaChineButton")->SetEnable(true);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("CocktailrecipeButton")->SetEnable(true);

			mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("MixButton")->SetEnable(true);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("ReSetButton")->SetEnable(true);

			mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("SoldOutButton")->SetEnable(true);

			mainCanvas->GetCanvasUI("CombinationCanvas")->GetTextUI("ReQuestText")->SetEnable(true);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetTextUI("GoldText")->SetEnable(true);

			mainCanvas->GetCanvasUI("SceneTestCanvas")->GetButtonUI("TestButton")->SetEnable(false);
		});*/

		//==================(ClickEvents)===============================
		testSceneButton->SetClickEvent([&]
		{
			///*mainCanvas->GetCanvasUI("CombinationCanvas")->SetEnable(false);*/
			//switch (nowScene)
			//{
			//case Click0:
			//	nowScene = ClickCount::Click1;
			//	SceneManager::GetInstance()->LoadScene(TEXT("TutorialScene"));
			//	mainCanvas->GetCanvasUI("SceneTestCanvas")->GetButtonUI("TestButton")->SetEnable(false);
			//	break;
			//case Click1:
			//	nowScene = ClickCount::Click2;
			//	SceneManager::GetInstance()->LoadScene(TEXT("Room1Scene"));
			//	mainCanvas->GetCanvasUI("SceneTestCanvas")->GetButtonUI("TestButton")->SetEnable(false);
			//	break;
			//case Click2:
			//	nowScene = ClickCount::Click0;
			//	SceneManager::GetInstance()->LoadScene(TEXT("LobbyScene"));
			//	mainCanvas->GetCanvasUI("SceneTestCanvas")->GetButtonUI("TestButton")->SetEnable(true);
			//	break;
			//default:
			//	break;
			//}
			SceneManager::GetInstance()->LoadScene(TEXT("LobbyScene"));
		});

		canvas->SetEnable(false);
	}

	void UICreator::CreateCombinationCanvas()
	{
		auto canvas = mainCanvas->GetCanvasUI("CombinationCanvas");

		auto canvasTexture = GameEngineSpace::GraphicsManager::GetInstance()->LoadTexture(L"Resources/UI/titleTest.png");

		auto buttonTexture = GameEngineSpace::GraphicsManager::GetInstance()->LoadTexture(L"Resources/UI/Button2.png");

		auto mixbuttonTexture = GameEngineSpace::GraphicsManager::GetInstance()->LoadTexture(L"Resources/UI/CackTailMix.png");

		auto reSetbuttonTexture = GameEngineSpace::GraphicsManager::GetInstance()->LoadTexture(L"Resources/UI/ReSet.png");

		auto beerMaChineTexture = GameEngineSpace::GraphicsManager::GetInstance()->LoadTexture(L"Resources/UI/BeerMaChine.png");

		auto cocktailrecipeTexture = GameEngineSpace::GraphicsManager::GetInstance()->LoadTexture(L"Resources/UI/Cocktailrecipe.png");

		auto combinationBoxBgTexture = GameEngineSpace::GraphicsManager::GetInstance()->LoadTexture(L"Resources/UI/CombinationBox.png");

		auto combinationBoxTexture = GameEngineSpace::GraphicsManager::GetInstance()->LoadTexture(L"Resources/UI/CombinationBack.png");

		auto customer1Texture = GameEngineSpace::GraphicsManager::GetInstance()->LoadTexture(L"Resources/UI/Character1.png");

		auto customer2Texture = GameEngineSpace::GraphicsManager::GetInstance()->LoadTexture(L"Resources/UI/Character1.png");

		auto customer3Texture = GameEngineSpace::GraphicsManager::GetInstance()->LoadTexture(L"Resources/UI/Character1.png");

		auto reQuestTextureBox = GameEngineSpace::GraphicsManager::GetInstance()->LoadTexture(L"Resources/UI/ReQuest.png");

		auto cashiermachineTexture = GameEngineSpace::GraphicsManager::GetInstance()->LoadTexture(L"Resources/UI/Cashiermachine.png");

		auto dialogTexture = GameEngineSpace::GraphicsManager::GetInstance()->LoadTexture(L"Resources/UI/Dialog.png");

		auto rumTexture = GameEngineSpace::GraphicsManager::GetInstance()->LoadTexture(L"Resources/UI/RumFIller.png");
		auto vodkaTexture = GameEngineSpace::GraphicsManager::GetInstance()->LoadTexture(L"Resources/UI/VodkaFiller.png");
		auto ginTexture = GameEngineSpace::GraphicsManager::GetInstance()->LoadTexture(L"Resources/UI/GinFiller.png");
		auto liquorTexture = GameEngineSpace::GraphicsManager::GetInstance()->LoadTexture(L"Resources/UI/LiquorFiller.png");
		auto juiceTexture = GameEngineSpace::GraphicsManager::GetInstance()->LoadTexture(L"Resources/UI/JuiceFiller.png");
		auto sodaTexture = GameEngineSpace::GraphicsManager::GetInstance()->LoadTexture(L"Resources/UI/SodaFiller.png");
		auto fruitTexture = GameEngineSpace::GraphicsManager::GetInstance()->LoadTexture(L"Resources/UI/Fruit.png");

		//==================(CombinationBackGround)===============================
		auto combinationBackGround = canvas->CreateSpriteUI("CombinationBackGround");
		combinationBackGround->SetTexture(canvasTexture);
		combinationBackGround->SetPosition(Vector3{ 0.f, 0.f, 0.5f });
		combinationBackGround->SetWidth(1923.f);
		combinationBackGround->SetHeight(1080.f);
		combinationBackGround->SetEnable(false);

		//==================(characters Textrue)===================================
		auto customer1 = canvas->CreateSpriteUI("Customer1");
		customer1->SetTexture(customer1Texture);
		customer1->SetPosition(Vector3{ 0.f, 0.f, 0.4f });
		customer1->SetWidth(250.f);
		customer1->SetHeight(250.f);
		customer1->SetEnable(false);

		auto customer2 = canvas->CreateSpriteUI("Customer2");
		customer2->SetTexture(customer2Texture);
		customer2->SetPosition(Vector3{ 0.f, 0.f, 0.4f });
		customer2->SetWidth(200.f);
		customer2->SetHeight(200.f);
		customer2->SetEnable(false);

		auto customer3 = canvas->CreateSpriteUI("Customer3");
		customer3->SetTexture(customer3Texture);
		customer3->SetPosition(Vector3{ 0.f, 0.f, 0.4f });
		customer3->SetWidth(200.f);
		customer3->SetHeight(200.f);
		customer3->SetEnable(false);

		//==================(CharactersDialog & SelectDialog )===================================
		auto charactersDialog = canvas->CreateSpriteUI("CharactersDialog");
		charactersDialog->SetTexture(dialogTexture);
		charactersDialog->SetPosition(Vector3{ -327.f, -244.f, 0.4f });
		charactersDialog->SetWidth(535.f);
		charactersDialog->SetHeight(335.f);
		charactersDialog->SetEnable(false);

		auto selectDialogButtons1 = canvas->CreateButtonUI("SelectDialogButtons1");
		selectDialogButtons1->SetPosition(Vector3{ 415.f, -79.f, 0.4f });
		selectDialogButtons1->SetDefaultTexture(buttonTexture);
		selectDialogButtons1->SetWidth(580.f);
		selectDialogButtons1->SetHeight(60.f);
		selectDialogButtons1->SetEnable(false);

		auto selectDialogButtons2 = canvas->CreateButtonUI("SelectDialogButtons2");
		selectDialogButtons2->SetPosition(Vector3{ 415.f, 38.f, 0.4f });
		selectDialogButtons2->SetDefaultTexture(buttonTexture);
		selectDialogButtons2->SetWidth(580.f);
		selectDialogButtons2->SetHeight(60.f);
		selectDialogButtons2->SetEnable(false);

		//==================(CombinationBoxBackGround)===================================
		auto combinationBoxBackGround = canvas->CreateSpriteUI("CombinationBoxBackGround");
		combinationBoxBackGround->SetTexture(combinationBoxBgTexture);
		combinationBoxBackGround->SetPosition(Vector3{ 0.f,335.f, 0.5f });
		combinationBoxBackGround->SetWidth(1923.f);
		combinationBoxBackGround->SetHeight(427.f);
		combinationBoxBackGround->SetEnable(false);

		//==================(stuffButtons)===================================
		auto rumButton = canvas->CreateButtonUI("RumButton");
		rumButton->SetPosition(Vector3{ -898.f, 254.f, 0.4f });
		rumButton->SetDefaultTexture(rumTexture);
		rumButton->SetWidth(100.f);
		rumButton->SetHeight(90.f);
		rumButton->SetEnable(false);

		auto rumSprite = canvas->CreateSpriteUI("RumTextrue");
		rumSprite->SetTexture(rumTexture);
		rumSprite->SetPosition(Vector3{ -205.f, 215.f, 0.3f });
		rumSprite->SetWidth(100.f);
		rumSprite->SetHeight(100.f);
		rumSprite->SetEnable(false);

		auto rumSprite2 = canvas->CreateSpriteUI("RumTextrue2");
		rumSprite2->SetTexture(rumTexture);
		rumSprite2->SetPosition(Vector3{ -7.f, 215.f, 0.3f });
		rumSprite2->SetWidth(100.f);
		rumSprite2->SetHeight(100.f);
		rumSprite2->SetEnable(false);

		auto rumSprite3 = canvas->CreateSpriteUI("RumTextrue3");
		rumSprite3->SetTexture(rumTexture);
		rumSprite3->SetPosition(Vector3{ 180.f, 215.f, 0.3f });
		rumSprite3->SetWidth(100.f);
		rumSprite3->SetHeight(100.f);
		rumSprite3->SetEnable(false);

		auto vodkaButton = canvas->CreateButtonUI("VodkaButton");
		vodkaButton->SetPosition(Vector3{ -898.f, 375.f, 0.4f });
		vodkaButton->SetDefaultTexture(vodkaTexture);
		vodkaButton->SetWidth(124.f);
		vodkaButton->SetHeight(80.f);
		vodkaButton->SetEnable(false);

		auto vodkaSprite = canvas->CreateSpriteUI("VodkaTextrue");
		vodkaSprite->SetTexture(vodkaTexture);
		vodkaSprite->SetPosition(Vector3{ -205.f, 215.f, 0.3f });
		vodkaSprite->SetWidth(100.f);
		vodkaSprite->SetHeight(100.f);
		vodkaSprite->SetEnable(false);

		auto vodkaSprite2 = canvas->CreateSpriteUI("VodkaTextrue2");
		vodkaSprite2->SetTexture(vodkaTexture);
		vodkaSprite2->SetPosition(Vector3{ -7.f, 215.f, 0.3f });
		vodkaSprite2->SetWidth(100.f);
		vodkaSprite2->SetHeight(100.f);
		vodkaSprite2->SetEnable(false);

		auto vodkaSprite3 = canvas->CreateSpriteUI("VodkaTextrue3");
		vodkaSprite3->SetTexture(vodkaTexture);
		vodkaSprite3->SetPosition(Vector3{ 180.f, 215.f, 0.3f });
		vodkaSprite3->SetWidth(100.f);
		vodkaSprite3->SetHeight(100.f);
		vodkaSprite3->SetEnable(false);

		auto ginButton = canvas->CreateButtonUI("GinButton");
		ginButton->SetPosition(Vector3{ -898.f, 495.f, 0.4f });
		ginButton->SetDefaultTexture(ginTexture);
		ginButton->SetWidth(115.f);
		ginButton->SetHeight(80.f);
		ginButton->SetEnable(false);

		auto ginSprite = canvas->CreateSpriteUI("GinTextrue");
		ginSprite->SetTexture(ginTexture);
		ginSprite->SetPosition(Vector3{ -205.f, 215.f, 0.3f });
		ginSprite->SetWidth(100.f);
		ginSprite->SetHeight(100.f);
		ginSprite->SetEnable(false);

		auto ginSprite2 = canvas->CreateSpriteUI("GinTextrue2");
		ginSprite2->SetTexture(ginTexture);
		ginSprite2->SetPosition(Vector3{ -7.f, 215.f, 0.3f });
		ginSprite2->SetWidth(100.f);
		ginSprite2->SetHeight(100.f);
		ginSprite2->SetEnable(false);

		auto ginSprite3 = canvas->CreateSpriteUI("GinTextrue3");
		ginSprite3->SetTexture(ginTexture);
		ginSprite3->SetPosition(Vector3{ 180.f, 215.f, 0.3f });
		ginSprite3->SetWidth(100.f);
		ginSprite3->SetHeight(100.f);
		ginSprite3->SetEnable(false);

		auto liquorButton = canvas->CreateButtonUI("LiquorButton");
		liquorButton->SetPosition(Vector3{ -707.f, 320.f, 0.4f });
		liquorButton->SetDefaultTexture(liquorTexture);
		liquorButton->SetWidth(130.f);
		liquorButton->SetHeight(60.f);
		liquorButton->SetEnable(false);

		auto liquorSprite = canvas->CreateSpriteUI("LiquorTextrue");
		liquorSprite->SetTexture(liquorTexture);
		liquorSprite->SetPosition(Vector3{ -205.f, 215.f, 0.3f });
		liquorSprite->SetWidth(100.f);
		liquorSprite->SetHeight(100.f);
		liquorSprite->SetEnable(false);

		auto liquorSprite2 = canvas->CreateSpriteUI("LiquorTextrue2");
		liquorSprite2->SetTexture(liquorTexture);
		liquorSprite2->SetPosition(Vector3{ -7.f, 215.f, 0.3f });
		liquorSprite2->SetWidth(100.f);
		liquorSprite2->SetHeight(100.f);
		liquorSprite2->SetEnable(false);

		auto liquorSprite3 = canvas->CreateSpriteUI("LiquorTextrue3");
		liquorSprite3->SetTexture(liquorTexture);
		liquorSprite3->SetPosition(Vector3{ 180.f, 215.f, 0.3f });
		liquorSprite3->SetWidth(100.f);
		liquorSprite3->SetHeight(100.f);
		liquorSprite3->SetEnable(false);

		auto juiceButton = canvas->CreateButtonUI("JuiceButton");
		juiceButton->SetPosition(Vector3{ -492.f, 320.f, 0.4f });
		juiceButton->SetDefaultTexture(juiceTexture);
		juiceButton->SetWidth(130.f);
		juiceButton->SetHeight(60.f);
		juiceButton->SetEnable(false);

		auto juiceSprite = canvas->CreateSpriteUI("JuiceTextrue");
		juiceSprite->SetTexture(juiceTexture);
		juiceSprite->SetPosition(Vector3{ -205.f, 215.f, 0.3f });
		juiceSprite->SetWidth(100.f);
		juiceSprite->SetHeight(100.f);
		juiceSprite->SetEnable(false);

		auto juiceSprite2 = canvas->CreateSpriteUI("JuiceTextrue2");
		juiceSprite2->SetTexture(juiceTexture);
		juiceSprite2->SetPosition(Vector3{ -7.f, 215.f, 0.3f });
		juiceSprite2->SetWidth(100.f);
		juiceSprite2->SetHeight(100.f);
		juiceSprite2->SetEnable(false);

		auto juiceSprite3 = canvas->CreateSpriteUI("JuiceTextrue3");
		juiceSprite3->SetTexture(juiceTexture);
		juiceSprite3->SetPosition(Vector3{ 180.f, 215.f, 0.3f });
		juiceSprite3->SetWidth(100.f);
		juiceSprite3->SetHeight(100.f);
		juiceSprite3->SetEnable(false);

		auto sodaButton = canvas->CreateButtonUI("SodaButton");
		sodaButton->SetPosition(Vector3{ -706.f, 467.f, 0.4f });
		sodaButton->SetDefaultTexture(sodaTexture);
		sodaButton->SetWidth(130.f);
		sodaButton->SetHeight(60.f);
		sodaButton->SetEnable(false);

		auto sodaSprite = canvas->CreateSpriteUI("SodaTextrue");
		sodaSprite->SetTexture(sodaTexture);
		sodaSprite->SetPosition(Vector3{ -205.f, 215.f, 0.3f });
		sodaSprite->SetWidth(100.f);
		sodaSprite->SetHeight(100.f);
		sodaSprite->SetEnable(false);

		auto sodaSprite2 = canvas->CreateSpriteUI("SodaTextrue2");
		sodaSprite2->SetTexture(sodaTexture);
		sodaSprite2->SetPosition(Vector3{ -7.f, 215.f, 0.3f });
		sodaSprite2->SetWidth(100.f);
		sodaSprite2->SetHeight(100.f);
		sodaSprite2->SetEnable(false);

		auto sodaSprite3 = canvas->CreateSpriteUI("SodaTextrue3");
		sodaSprite3->SetTexture(sodaTexture);
		sodaSprite3->SetPosition(Vector3{ 180.f, 215.f, 0.3f });
		sodaSprite3->SetWidth(100.f);
		sodaSprite3->SetHeight(100.f);
		sodaSprite3->SetEnable(false);

		auto fruitButton = canvas->CreateButtonUI("FruitButton");
		fruitButton->SetPosition(Vector3{ -492.f, 464.f, 0.4f });
		fruitButton->SetDefaultTexture(fruitTexture);
		fruitButton->SetWidth(130.f);
		fruitButton->SetHeight(60.f);
		fruitButton->SetEnable(false);

		auto fruitSprite = canvas->CreateSpriteUI("FruitTextrue");
		fruitSprite->SetTexture(fruitTexture);
		fruitSprite->SetPosition(Vector3{ -205.f, 215.f, 0.3f });
		fruitSprite->SetWidth(100.f);
		fruitSprite->SetHeight(100.f);
		fruitSprite->SetEnable(false);

		auto fruitSprite2 = canvas->CreateSpriteUI("FruitTextrue2");
		fruitSprite2->SetTexture(fruitTexture);
		fruitSprite2->SetPosition(Vector3{ -7.f, 215.f, 0.3f });
		fruitSprite2->SetWidth(100.f);
		fruitSprite2->SetHeight(100.f);
		fruitSprite2->SetEnable(false);

		auto fruitSprite3 = canvas->CreateSpriteUI("FruitTextrue3");
		fruitSprite3->SetTexture(fruitTexture);
		fruitSprite3->SetPosition(Vector3{ 180.f, 215.f, 0.3f });
		fruitSprite3->SetWidth(100.f);
		fruitSprite3->SetHeight(100.f);
		fruitSprite3->SetEnable(false);

		auto stuffButton = canvas->CreateButtonUI("StuffButton");
		stuffButton->SetPosition(Vector3{ -205.f, 215.f, 0.4f });
		stuffButton->SetDefaultTexture(combinationBoxTexture);
		stuffButton->SetWidth(100.f);
		stuffButton->SetHeight(100.f);
		stuffButton->SetEnable(false);

		auto stuffButton2 = canvas->CreateButtonUI("StuffButton2");
		stuffButton2->SetPosition(Vector3{ -7.f, 215.f, 0.4f });
		stuffButton2->SetDefaultTexture(combinationBoxTexture);
		stuffButton2->SetWidth(100.f);
		stuffButton2->SetHeight(100.f);
		stuffButton2->SetEnable(false);

		auto stuffButton3 = canvas->CreateButtonUI("StuffButton3");
		stuffButton3->SetPosition(Vector3{ 180.f, 215.f, 0.4f });
		stuffButton3->SetDefaultTexture(combinationBoxTexture);
		stuffButton3->SetWidth(100.f);
		stuffButton3->SetHeight(100.f);
		stuffButton3->SetEnable(false);

		//==================(stuffResult Textrue)===================================
		auto stuffResultTextrue = canvas->CreateSpriteUI("StuffResultTextrue");
		stuffResultTextrue->SetTexture(combinationBoxTexture);
		stuffResultTextrue->SetPosition(Vector3{ -7.f, 378.f, 0.4f });
		stuffResultTextrue->SetWidth(100.f);
		stuffResultTextrue->SetHeight(100.f);
		stuffResultTextrue->SetEnable(false);

		//==================(Mix & ReSet Button)===================================
		auto mixButton = canvas->CreateButtonUI("MixButton");
		mixButton->SetPosition(Vector3{ 105.f, 485.f, 0.4f });
		mixButton->SetDefaultTexture(mixbuttonTexture);
		mixButton->SetWidth(130.f);
		mixButton->SetHeight(60.f);
		mixButton->SetEnable(false);

		auto reSetButton = canvas->CreateButtonUI("ReSetButton");
		reSetButton->SetPosition(Vector3{ -125.f, 485.f, 0.4f });
		reSetButton->SetDefaultTexture(reSetbuttonTexture);
		reSetButton->SetWidth(130.f);
		reSetButton->SetHeight(60.f);
		reSetButton->SetEnable(false);

		//==================(ReQuest)===================================
		auto reQuestSprite = canvas->CreateSpriteUI("ReQuest");
		reQuestSprite->SetTexture(reQuestTextureBox);
		reQuestSprite->SetPosition(Vector3{ 810.f, -391.f, 0.4f });
		reQuestSprite->SetWidth(300.f);
		reQuestSprite->SetHeight(300.f);
		reQuestSprite->SetEnable(false);

		auto reQuestText = canvas->CreateTextUI("ReQuestText");
		std::wstring reQuestTextString = L"ReQuest ";
		reQuestText->SetPosition(Vector3{ 841.f, -466.f, 0.4f });
		reQuestText->SetText(reQuestTextString);
		reQuestText->SetFontSize(30.f);
		reQuestText->SetEnable(false);

		//==================(CashierMachine)===================================
		auto cashiermachineSprite = canvas->CreateSpriteUI("CashierMachine");
		cashiermachineSprite->SetTexture(cashiermachineTexture);
		cashiermachineSprite->SetPosition(Vector3{ 490.f, 337.f, 0.4f });
		cashiermachineSprite->SetWidth(335.f);
		cashiermachineSprite->SetHeight(431.f);
		cashiermachineSprite->SetEnable(false);

		auto goldText = canvas->CreateTextUI("GoldText");
		std::wstring goldTextString = L"Gold";
		goldText->SetPosition(Vector3{ 730.f, 373.f, 0.4f });
		goldText->SetText(goldTextString);
		goldText->SetFontSize(30.f);
		goldText->SetEnable(false);

		//==================(BeerMaChine)===================================
		auto beerMaChineButton = canvas->CreateButtonUI("BeerMaChineButton");
		beerMaChineButton->SetPosition(Vector3{ 845.f, 104.f, 0.4f });
		beerMaChineButton->SetDefaultTexture(beerMaChineTexture);
		beerMaChineButton->SetWidth(229.f);
		beerMaChineButton->SetHeight(165.f);
		beerMaChineButton->SetEnable(false);

		//==================(CocktailrecipeButton)===================================
		auto cocktailrecipeButton = canvas->CreateButtonUI("CocktailrecipeButton");
		cocktailrecipeButton->SetPosition(Vector3{ -807.f, 132.f, 0.4f });
		cocktailrecipeButton->SetDefaultTexture(cocktailrecipeTexture);
		cocktailrecipeButton->SetWidth(307.f);
		cocktailrecipeButton->SetHeight(115.f);
		cocktailrecipeButton->SetEnable(false);


		//==================(SoldOutButton)===================================
		auto soldOutButton = canvas->CreateButtonUI("SoldOutButton");
		soldOutButton->SetPosition(Vector3{ 883.f, 496.f, 0.4f });
		soldOutButton->SetDefaultTexture(buttonTexture);
		soldOutButton->SetWidth(130.f);
		soldOutButton->SetHeight(60.f);
		soldOutButton->SetEnable(false);


		//==================(SoldOutButtonClickEvents)===============================
		static enum ClickCount
		{
			Click0,
			Click1,
			Click2,
		};
		static ClickCount nowTexture = ClickCount::Click0;

		soldOutButton->SetClickEvent([&]
		{
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("CombinationBackGround")->SetEnable(false);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("CombinationBoxBackGround")->SetEnable(false);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("StuffResultTextrue")->SetEnable(false);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("ReQuest")->SetEnable(false);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("CashierMachine")->SetEnable(false);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("Customer1")->SetEnable(false);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("CharactersDialog")->SetEnable(false);

			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("RumTextrue")->SetEnable(false);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("RumTextrue2")->SetEnable(false);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("RumTextrue3")->SetEnable(false);

			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("VodkaTextrue")->SetEnable(false);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("VodkaTextrue2")->SetEnable(false);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("VodkaTextrue3")->SetEnable(false);

			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("GinTextrue")->SetEnable(false);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("GinTextrue2")->SetEnable(false);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("GinTextrue3")->SetEnable(false);

			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("LiquorTextrue")->SetEnable(false);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("LiquorTextrue2")->SetEnable(false);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("LiquorTextrue3")->SetEnable(false);

			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("JuiceTextrue")->SetEnable(false);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("JuiceTextrue2")->SetEnable(false);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("JuiceTextrue3")->SetEnable(false);

			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("SodaTextrue")->SetEnable(false);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("SodaTextrue2")->SetEnable(false);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("SodaTextrue3")->SetEnable(false);

			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("FruitTextrue")->SetEnable(false);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("FruitTextrue2")->SetEnable(false);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("FruitTextrue3")->SetEnable(false);

			mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("RumButton")->SetEnable(false);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("VodkaButton")->SetEnable(false);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("GinButton")->SetEnable(false);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("LiquorButton")->SetEnable(false);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("JuiceButton")->SetEnable(false);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("SodaButton")->SetEnable(false);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("FruitButton")->SetEnable(false);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("StuffButton")->SetEnable(false);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("StuffButton2")->SetEnable(false);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("StuffButton3")->SetEnable(false);

			mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("BeerMaChineButton")->SetEnable(false);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("CocktailrecipeButton")->SetEnable(false);

			mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("MixButton")->SetEnable(false);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("ReSetButton")->SetEnable(false);

			mainCanvas->GetCanvasUI("CombinationCanvas")->GetTextUI("ReQuestText")->SetEnable(false);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetTextUI("GoldText")->SetEnable(false);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("SoldOutButton")->SetEnable(false);

			SceneManager::GetInstance()->LoadScene(TEXT("LobbyScene"));

		});

		rumButton->SetClickEvent([&]
		{
			// 풀 때도 반드시 SetButtonEnable(true)로만 풀 수 있으니 잘 쓸것.
			//mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("VodkaButton")->SetButtonEnable(false);

			switch (nowTexture)
			{
			case Click0:
				nowTexture = ClickCount::Click1;
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("RumTextrue")->SetEnable(true);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("VodkaTextrue")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("GinTextrue")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("LiquorTextrue")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("JuiceTextrue")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("SodaTextrue")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("FruitTextrue")->SetEnable(false);
				break;
			case Click1:
				nowTexture = ClickCount::Click2;
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("RumTextrue2")->SetEnable(true);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("VodkaTextrue2")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("GinTextrue2")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("LiquorTextrue2")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("JuiceTextrue2")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("SodaTextrue2")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("FruitTextrue2")->SetEnable(false);
				break;
			case Click2:
				nowTexture = ClickCount::Click0;
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("RumTextrue3")->SetEnable(true);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("VodkaTextrue3")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("GinTextrue3")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("LiquorTextrue3")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("JuiceTextrue3")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("SodaTextrue3")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("FruitTextrue3")->SetEnable(false);
				// 모든 버튼을 잠금
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("RumButton")->SetButtonEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("VodkaButton")->SetButtonEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("GinButton")->SetButtonEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("LiquorButton")->SetButtonEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("JuiceButton")->SetButtonEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("SodaButton")->SetButtonEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("FruitButton")->SetButtonEnable(false);
				break;
			default:
				break;
			}
		});

		vodkaButton->SetClickEvent([&]
		{

			switch (nowTexture)
			{
			case Click0:
				nowTexture = ClickCount::Click1;
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("VodkaTextrue")->SetEnable(true);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("RumTextrue")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("GinTextrue")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("LiquorTextrue")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("JuiceTextrue")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("SodaTextrue")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("FruitTextrue")->SetEnable(false);
				break;
			case Click1:
				nowTexture = ClickCount::Click2;
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("VodkaTextrue2")->SetEnable(true);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("RumTextrue2")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("GinTextrue2")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("LiquorTextrue2")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("JuiceTextrue2")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("SodaTextrue2")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("FruitTextrue2")->SetEnable(false);
				break;
			case Click2:
				nowTexture = ClickCount::Click0;
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("VodkaTextrue3")->SetEnable(true);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("RumTextrue3")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("GinTextrue3")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("LiquorTextrue3")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("JuiceTextrue3")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("SodaTextrue3")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("FruitTextrue3")->SetEnable(false);
				// 모든 버튼을 잠금
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("RumButton")->SetButtonEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("VodkaButton")->SetButtonEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("GinButton")->SetButtonEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("LiquorButton")->SetButtonEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("JuiceButton")->SetButtonEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("SodaButton")->SetButtonEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("FruitButton")->SetButtonEnable(false);
				break;
			default:
				break;
			}
			
		});

		ginButton->SetClickEvent([&]
		{

			switch (nowTexture)
			{
			case Click0:
				nowTexture = ClickCount::Click1;
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("GinTextrue")->SetEnable(true);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("RumTextrue")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("VodkaTextrue")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("LiquorTextrue")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("JuiceTextrue")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("SodaTextrue")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("FruitTextrue")->SetEnable(false);
				break;
			case Click1:
				nowTexture = ClickCount::Click2;
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("GinTextrue2")->SetEnable(true);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("RumTextrue2")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("VodkaTextrue2")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("LiquorTextrue2")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("JuiceTextrue2")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("SodaTextrue2")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("FruitTextrue2")->SetEnable(false);
				break;
			case Click2:
				nowTexture = ClickCount::Click0;
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("GinTextrue3")->SetEnable(true);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("RumTextrue3")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("VodkaTextrue3")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("LiquorTextrue3")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("JuiceTextrue3")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("SodaTextrue3")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("FruitTextrue3")->SetEnable(false);
				// 모든 버튼을 잠금
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("RumButton")->SetButtonEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("VodkaButton")->SetButtonEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("GinButton")->SetButtonEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("LiquorButton")->SetButtonEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("JuiceButton")->SetButtonEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("SodaButton")->SetButtonEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("FruitButton")->SetButtonEnable(false);
				break;
			default:
				break;
			}
		});

		liquorButton->SetClickEvent([&]
		{

			switch (nowTexture)
			{
			case Click0:
				nowTexture = ClickCount::Click1;
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("LiquorTextrue")->SetEnable(true);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("RumTextrue")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("GinTextrue")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("VodkaTextrue")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("JuiceTextrue")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("SodaTextrue")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("FruitTextrue")->SetEnable(false);
				break;
			case Click1:
				nowTexture = ClickCount::Click2;
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("LiquorTextrue2")->SetEnable(true);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("RumTextrue2")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("GinTextrue2")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("VodkaTextrue2")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("JuiceTextrue2")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("SodaTextrue2")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("FruitTextrue2")->SetEnable(false);
				break;
			case Click2:
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("LiquorTextrue3")->SetEnable(true);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("RumTextrue3")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("GinTextrue3")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("VodkaTextrue3")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("JuiceTextrue3")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("SodaTextrue3")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("FruitTextrue3")->SetEnable(false);
				// 모든 버튼을 잠금
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("RumButton")->SetButtonEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("VodkaButton")->SetButtonEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("GinButton")->SetButtonEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("LiquorButton")->SetButtonEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("JuiceButton")->SetButtonEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("SodaButton")->SetButtonEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("FruitButton")->SetButtonEnable(false);
				break;
			default:
				break;
			}
		});

		juiceButton->SetClickEvent([&]
		{

			switch (nowTexture)
			{
			case Click0:
				nowTexture = ClickCount::Click1;
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("JuiceTextrue")->SetEnable(true);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("RumTextrue")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("VodkaTextrue")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("GinTextrue")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("LiquorTextrue")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("SodaTextrue")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("FruitTextrue")->SetEnable(false);
				break;
			case Click1:
				nowTexture = ClickCount::Click2;
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("JuiceTextrue2")->SetEnable(true);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("RumTextrue2")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("VodkaTextrue2")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("GinTextrue2")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("LiquorTextrue2")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("SodaTextrue2")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("FruitTextrue2")->SetEnable(false);
				break;
			case Click2:
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("JuiceTextrue3")->SetEnable(true);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("RumTextrue3")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("VodkaTextrue3")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("GinTextrue3")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("LiquorTextrue3")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("SodaTextrue3")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("FruitTextrue3")->SetEnable(false);
				// 모든 버튼을 잠금
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("RumButton")->SetButtonEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("VodkaButton")->SetButtonEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("GinButton")->SetButtonEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("LiquorButton")->SetButtonEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("JuiceButton")->SetButtonEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("SodaButton")->SetButtonEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("FruitButton")->SetButtonEnable(false);
				break;
			default:
				break;
			}
		});

		sodaButton->SetClickEvent([&]
		{
			switch (nowTexture)
			{
			case Click0:
				nowTexture = ClickCount::Click1;
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("SodaTextrue")->SetEnable(true);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("RumTextrue")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("VodkaTextrue")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("GinTextrue")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("LiquorTextrue2")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("JuiceTextrue")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("FruitTextrue")->SetEnable(false);
				break;
			case Click1:
				nowTexture = ClickCount::Click2;
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("SodaTextrue2")->SetEnable(true);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("RumTextrue2")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("VodkaTextrue2")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("GinTextrue2")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("LiquorTextrue2")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("JuiceTextrue2")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("FruitTextrue2")->SetEnable(false);
				break;
			case Click2:
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("SodaTextrue3")->SetEnable(true);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("RumTextrue3")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("VodkaTextrue3")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("GinTextrue3")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("LiquorTextrue3")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("JuiceTextrue3")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("FruitTextrue3")->SetEnable(false);
				// 모든 버튼을 잠금
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("RumButton")->SetButtonEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("VodkaButton")->SetButtonEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("GinButton")->SetButtonEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("LiquorButton")->SetButtonEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("JuiceButton")->SetButtonEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("SodaButton")->SetButtonEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("FruitButton")->SetButtonEnable(false);
				break;
			default:
				break;
			}
		});

		fruitButton->SetClickEvent([&]
		{

			switch (nowTexture)
			{
			case Click0:
				nowTexture = ClickCount::Click1;
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("FruitTextrue")->SetEnable(true);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("RumTextrue")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("VodkaTextrue")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("GinTextrue")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("LiquorTextrue")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("JuiceTextrue")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("SodaTextrue")->SetEnable(false);
				break;
			case Click1:
				nowTexture = ClickCount::Click2;
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("FruitTextrue2")->SetEnable(true);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("RumTextrue2")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("VodkaTextrue2")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("GinTextrue2")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("LiquorTextrue2")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("JuiceTextrue2")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("SodaTextrue2")->SetEnable(false);
				break;
			case Click2:
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("FruitTextrue3")->SetEnable(true);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("RumTextrue3")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("VodkaTextrue3")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("GinTextrue3")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("LiquorTextrue3")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("JuiceTextrue3")->SetEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("SodaTextrue3")->SetEnable(false);
				// 모든 버튼을 잠금
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("RumButton")->SetButtonEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("VodkaButton")->SetButtonEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("GinButton")->SetButtonEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("LiquorButton")->SetButtonEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("JuiceButton")->SetButtonEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("SodaButton")->SetButtonEnable(false);
				mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("FruitButton")->SetButtonEnable(false);
				break;
			default:
				break;
			}
		});

		reSetButton->SetClickEvent([&]
		{
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("RumButton")->SetButtonEnable(true);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("VodkaButton")->SetButtonEnable(true);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("GinButton")->SetButtonEnable(true);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("LiquorButton")->SetButtonEnable(true);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("JuiceButton")->SetButtonEnable(true);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("SodaButton")->SetButtonEnable(true);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("FruitButton")->SetButtonEnable(true);

			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("RumTextrue")->SetEnable(false);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("RumTextrue2")->SetEnable(false);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("RumTextrue3")->SetEnable(false);

			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("VodkaTextrue")->SetEnable(false);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("VodkaTextrue2")->SetEnable(false);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("VodkaTextrue3")->SetEnable(false);

			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("GinTextrue")->SetEnable(false);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("GinTextrue2")->SetEnable(false);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("GinTextrue3")->SetEnable(false);

			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("LiquorTextrue")->SetEnable(false);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("LiquorTextrue2")->SetEnable(false);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("LiquorTextrue3")->SetEnable(false);

			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("JuiceTextrue")->SetEnable(false);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("JuiceTextrue2")->SetEnable(false);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("JuiceTextrue3")->SetEnable(false);

			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("SodaTextrue")->SetEnable(false);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("SodaTextrue2")->SetEnable(false);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("SodaTextrue3")->SetEnable(false);

			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("FruitTextrue")->SetEnable(false);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("FruitTextrue2")->SetEnable(false);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("FruitTextrue3")->SetEnable(false);

			//클릭 카운터 초기화
			nowTexture = ClickCount::Click0;
		});

		canvas->SetEnable(false);
	}

	void UICreator::CreateSettingCanvas()
	{
		//==================(Canvas & LoadTexture)============================
		auto canvas = mainCanvas->GetCanvasUI("SettingCanvase");

		auto settingBackgroundtexture = GameEngineSpace::GraphicsManager::GetInstance()->LoadTexture(L"Resources/UI/SettingUI2.png");

		auto buttonTexture = GameEngineSpace::GraphicsManager::GetInstance()->LoadTexture(L"Resources/UI/Button2.png");

		auto barBGTexture = GameEngineSpace::GraphicsManager::GetInstance()->LoadTexture(L"Resources/UI/BarBg3.png");

		auto barTexture = GameEngineSpace::GraphicsManager::GetInstance()->LoadTexture(L"Resources/UI/BarBg3.png");

		auto leftTexture = GameEngineSpace::GraphicsManager::GetInstance()->LoadTexture(L"Resources/UI/left.png");
		auto leftHover = GameEngineSpace::GraphicsManager::GetInstance()->LoadTexture(L"Resources/UI/leftHover.png");

		auto rightTexture = GameEngineSpace::GraphicsManager::GetInstance()->LoadTexture(L"Resources/UI/right.png");
		auto rightHover = GameEngineSpace::GraphicsManager::GetInstance()->LoadTexture(L"Resources/UI/rightHover.png");

		//==================(SettingBackground)===============================
		auto  SettingBackground = canvas->CreateSpriteUI("Setting");
		SettingBackground->SetTexture(settingBackgroundtexture);
		SettingBackground->SetPosition(Vector3{ 0.f, 0.f, 0.5f });
		SettingBackground->SetWidth(900.f);
		SettingBackground->SetHeight(600.f);

		//==================(GraphicsButton)======================================
		auto graphicsButton = canvas->CreateButtonUI("GraphicsButton");
		graphicsButton->SetPosition(Vector3{ -350.f, -240.f, 0.4f });
		graphicsButton->SetDefaultTexture(buttonTexture);
		graphicsButton->SetWidth(130.f);
		graphicsButton->SetHeight(60.f);

		auto graphicsText = canvas->CreateTextUI("GraphicsText");
		std::wstring graphicsTextString = L"Graphics";
		graphicsText->SetPosition(Vector3{ -317.f, -245.f, 0.4f });
		graphicsText->SetText(graphicsTextString);
		graphicsText->SetFontSize(25.f);

		auto sfXText = canvas->CreateTextUI("SFXText");
		std::wstring sfXTextString = L"SFX";
		sfXText->SetPosition(Vector3{ -235.f, -242.f, 0.4f });
		sfXText->SetText(sfXTextString);
		sfXText->SetEnable(false);
		sfXText->SetFontSize(19.f);

		auto sfxBarTexture = canvas->CreateSpriteUI("SFXBarTexture");
		sfxBarTexture->SetTexture(barTexture);
		sfxBarTexture->SetPosition(Vector3{ 73.f, -239.f, 0.4f });
		sfxBarTexture->SetWidth(557.f);
		sfxBarTexture->SetHeight(6.f);
		sfxBarTexture->SetEnable(false);

		auto windowSizeText = canvas->CreateTextUI("WindowSizeText");
		std::wstring windowSizeTextString = L"WindowSize";
		windowSizeText->SetPosition(Vector3{ -173.f, -117.f, 0.4f });
		windowSizeText->SetText(windowSizeTextString);
		windowSizeText->SetEnable(false);
		windowSizeText->SetFontSize(19.f);

		auto graphicsLeftButton = canvas->CreateButtonUI("GraphicsLeftButton");
		graphicsLeftButton->SetPosition(Vector3{ -240.f, -47.f, 0.4f });
		graphicsLeftButton->SetDefaultTexture(leftTexture);
		graphicsLeftButton->SetHoverTexture(leftHover);
		graphicsLeftButton->SetWidth(54.f);
		graphicsLeftButton->SetHeight(50.f);
		graphicsLeftButton->SetEnable(false);

		auto graphicsRightButton = canvas->CreateButtonUI("GraphicsRightButton");
		graphicsRightButton->SetPosition(Vector3{ 159.f, -47.f, 0.4f });
		graphicsRightButton->SetDefaultTexture(rightTexture);
		graphicsRightButton->SetHoverTexture(rightHover);
		graphicsRightButton->SetWidth(54.f);
		graphicsRightButton->SetHeight(50.f);
		graphicsRightButton->SetEnable(false);

		auto windowSize1Text = canvas->CreateTextUI("WindowSize1Text");
		std::wstring windowSize1TextString = L"1920 X 1080";
		windowSize1Text->SetPosition(Vector3{ -20.f, -46.f, 0.4f });
		windowSize1Text->SetText(windowSize1TextString);
		windowSize1Text->SetFontSize(19.f);
		windowSize1Text->SetEnable(false);

		auto windowSize2Text = canvas->CreateTextUI("WindowSize2Text");
		std::wstring windowSize2TextString = L"1600 X 900";
		windowSize2Text->SetPosition(Vector3{ -20.f, -46.f, 0.4f });
		windowSize2Text->SetText(windowSize2TextString);
		windowSize2Text->SetFontSize(19.f);
		windowSize2Text->SetEnable(false);

		auto windowSize3Text = canvas->CreateTextUI("WindowSize3Text");
		std::wstring windowSize3TextString = L"1280 X 720";
		windowSize3Text->SetPosition(Vector3{ -20.f, -46.f, 0.4f });
		windowSize3Text->SetText(windowSize3TextString);
		windowSize3Text->SetFontSize(19.f);
		windowSize3Text->SetEnable(false);

		//==================(MusicButton)======================================
		auto musicButton = canvas->CreateButtonUI("MusicButton");
		musicButton->SetPosition(Vector3{ -350.f, -140.f, 0.4f });
		musicButton->SetDefaultTexture(buttonTexture);
		musicButton->SetWidth(130.f);
		musicButton->SetHeight(60.f);

		auto musicText = canvas->CreateTextUI("MusicText");
		std::wstring musicTextString = L"Music";
		musicText->SetPosition(Vector3{ -336.f, -145.f, 0.4f });
		musicText->SetText(musicTextString);
		musicText->SetFontSize(25.f);

		auto fullVolumeText = canvas->CreateTextUI("FullVolumeText");
		std::wstring fullVolumeTextString = L"Full Volume";
		fullVolumeText->SetPosition(Vector3{ -153.f, -242.f, 0.4f });
		fullVolumeText->SetText(fullVolumeTextString);
		fullVolumeText->SetEnable(false);
		fullVolumeText->SetFontSize(19.f);

		auto fullVolumeTexture = canvas->CreateSpriteUI("FullVolumeTexture");
		fullVolumeTexture->SetTexture(barTexture);
		fullVolumeTexture->SetPosition(Vector3{ 23.f, -182.f, 0.4f });
		fullVolumeTexture->SetWidth(557.f);
		fullVolumeTexture->SetHeight(6.f);
		fullVolumeTexture->SetEnable(false);

		auto bgmVolumeText = canvas->CreateTextUI("BgmVolumeText");
		std::wstring bgmVolumeTextString = L"Bgm Volume";
		bgmVolumeText->SetPosition(Vector3{ -164.f, -122.f, 0.4f });
		bgmVolumeText->SetText(bgmVolumeTextString);
		bgmVolumeText->SetEnable(false);
		bgmVolumeText->SetFontSize(19.f);

		auto bgmVolumeTexture = canvas->CreateSpriteUI("BgmVolumeTexture");
		bgmVolumeTexture->SetTexture(barTexture);
		bgmVolumeTexture->SetPosition(Vector3{ 23.f, -62.f, 0.4f });
		bgmVolumeTexture->SetWidth(557.f);
		bgmVolumeTexture->SetHeight(6.f);
		bgmVolumeTexture->SetEnable(false);

		auto effectVolumeText = canvas->CreateTextUI("EffectVolume");
		std::wstring effectVolumeString = L"Effect Volume ";
		effectVolumeText->SetPosition(Vector3{ -128.f, -2.f, 0.4f });
		effectVolumeText->SetText(effectVolumeString);
		effectVolumeText->SetEnable(false);
		effectVolumeText->SetFontSize(19.f);

		auto effectVolumeTexture = canvas->CreateSpriteUI("EffectVolumeTexture");
		effectVolumeTexture->SetTexture(barTexture);
		effectVolumeTexture->SetPosition(Vector3{ 23.f, 58.f, 0.4f });
		effectVolumeTexture->SetWidth(557.f);
		effectVolumeTexture->SetHeight(6.f);
		effectVolumeTexture->SetEnable(false);

		//==================(EtcButton)======================================
		auto etcButton = canvas->CreateButtonUI("EtcButton");
		etcButton->SetPosition(Vector3{ -350.f, -37.f, 0.4f });
		etcButton->SetDefaultTexture(buttonTexture);
		etcButton->SetWidth(130.f);
		etcButton->SetHeight(60.f);

		auto etcText = canvas->CreateTextUI("EtcText");
		std::wstring etcTextString = L"Etc";
		etcText->SetPosition(Vector3{ -342.f, -43.f, 0.4f });
		etcText->SetText(etcTextString);
		etcText->SetFontSize(25.f);

		auto languageText = canvas->CreateTextUI("LanguageText");
		std::wstring languageTextString = L"Language";
		languageText->SetPosition(Vector3{ -170.f, -200.f, 0.4f });
		languageText->SetText(languageTextString);
		languageText->SetColor({ 0.f, 0.f, 0.f, 1.f });
		languageText->SetFontSize(19.f);
		languageText->SetEnable(true);

		auto languageLeftButton = canvas->CreateButtonUI("LanguageLeftButton");
		languageLeftButton->SetPosition(Vector3{ -123.f, -195.f, 0.4f });
		languageLeftButton->SetDefaultTexture(leftTexture);
		languageLeftButton->SetHoverTexture(leftHover);
		languageLeftButton->SetWidth(54.f);
		languageLeftButton->SetHeight(50.f);
		languageLeftButton->SetEnable(true);

		auto languageRightButton = canvas->CreateButtonUI("LanguageRightButton");
		languageRightButton->SetPosition(Vector3{ 86.f, -195.f, 0.4f });
		languageRightButton->SetDefaultTexture(rightTexture);
		languageRightButton->SetHoverTexture(rightHover);
		languageRightButton->SetWidth(54.f);
		languageRightButton->SetHeight(50.f);
		languageRightButton->SetEnable(true);

		auto koreanText = canvas->CreateTextUI("KoreanText");
		std::wstring koreanTextString = L"Korean";
		koreanText->SetPosition(Vector3{ -5.f, -197.f, 0.4f });
		koreanText->SetText(koreanTextString);
		koreanText->SetFontSize(19.f);
		koreanText->SetEnable(true);

		auto englishText = canvas->CreateTextUI("EnglishText");
		std::wstring englishTextString = L"English";
		englishText->SetPosition(Vector3{ -5.f, -197.f, 0.4f });
		englishText->SetText(englishTextString);
		englishText->SetFontSize(19.f);
		englishText->SetEnable(false);

		//==================(CloseButton)======================================
		auto closeButton = canvas->CreateButtonUI("CloseButtonButton");
		closeButton->SetPosition(Vector3{ 356.f, 247.f, 0.4f });
		closeButton->SetDefaultTexture(buttonTexture);
		closeButton->SetWidth(100.f);
		closeButton->SetHeight(50.f);


		//==================(ClickEvents)===============================
		static enum ClickCount
		{
			Click0,
			Click1,
			Click2,
			Click3,
			Click4,
		};
		static ClickCount nowWindowSize = ClickCount::Click0;

		/*static enum WindowSize
		{
			Click0,
			Click1,
			Click2,
			Click3,
			Click4,
		};
		static WindowSize nowWindowSize = WindowSize::Click0;*/

		graphicsButton->SetClickEvent([&]
		{
			mainCanvas->GetCanvasUI("SettingCanvase")->GetTextUI("SFXText")->SetEnable(true);
			mainCanvas->GetCanvasUI("SettingCanvase")->GetSpriteUI("SFXBarTexture")->SetEnable(true);
			mainCanvas->GetCanvasUI("SettingCanvase")->GetTextUI("WindowSizeText")->SetEnable(true);
			mainCanvas->GetCanvasUI("SettingCanvase")->GetTextUI("WindowSize1Text")->SetEnable(true);
			mainCanvas->GetCanvasUI("SettingCanvase")->GetButtonUI("GraphicsLeftButton")->SetEnable(true);
			mainCanvas->GetCanvasUI("SettingCanvase")->GetButtonUI("GraphicsRightButton")->SetEnable(true);

			mainCanvas->GetCanvasUI("SettingCanvase")->GetTextUI("FullVolumeText")->SetEnable(false);
			mainCanvas->GetCanvasUI("SettingCanvase")->GetSpriteUI("FullVolumeTexture")->SetEnable(false);
			mainCanvas->GetCanvasUI("SettingCanvase")->GetTextUI("BgmVolumeText")->SetEnable(false);
			mainCanvas->GetCanvasUI("SettingCanvase")->GetSpriteUI("BgmVolumeTexture")->SetEnable(false);
			mainCanvas->GetCanvasUI("SettingCanvase")->GetTextUI("EffectVolume")->SetEnable(false);
			mainCanvas->GetCanvasUI("SettingCanvase")->GetSpriteUI("EffectVolumeTexture")->SetEnable(false);

			mainCanvas->GetCanvasUI("SettingCanvase")->GetTextUI("LanguageText")->SetEnable(false);
			mainCanvas->GetCanvasUI("SettingCanvase")->GetButtonUI("LanguageLeftButton")->SetEnable(false);
			mainCanvas->GetCanvasUI("SettingCanvase")->GetButtonUI("LanguageRightButton")->SetEnable(false);
			mainCanvas->GetCanvasUI("SettingCanvase")->GetTextUI("KoreanText")->SetEnable(false);
			mainCanvas->GetCanvasUI("SettingCanvase")->GetTextUI("EnglishText")->SetEnable(false);
		});

		musicButton->SetClickEvent([&]
		{
			mainCanvas->GetCanvasUI("SettingCanvase")->GetTextUI("FullVolumeText")->SetEnable(true);
			mainCanvas->GetCanvasUI("SettingCanvase")->GetSpriteUI("FullVolumeTexture")->SetEnable(true);
			mainCanvas->GetCanvasUI("SettingCanvase")->GetTextUI("BgmVolumeText")->SetEnable(true);
			mainCanvas->GetCanvasUI("SettingCanvase")->GetSpriteUI("BgmVolumeTexture")->SetEnable(true);
			mainCanvas->GetCanvasUI("SettingCanvase")->GetTextUI("EffectVolume")->SetEnable(true);
			mainCanvas->GetCanvasUI("SettingCanvase")->GetSpriteUI("EffectVolumeTexture")->SetEnable(true);

			mainCanvas->GetCanvasUI("SettingCanvase")->GetTextUI("SFXText")->SetEnable(false);
			mainCanvas->GetCanvasUI("SettingCanvase")->GetSpriteUI("SFXBarTexture")->SetEnable(false);
			mainCanvas->GetCanvasUI("SettingCanvase")->GetTextUI("WindowSizeText")->SetEnable(false);
			mainCanvas->GetCanvasUI("SettingCanvase")->GetTextUI("WindowSize1Text")->SetEnable(false);
			mainCanvas->GetCanvasUI("SettingCanvase")->GetButtonUI("GraphicsLeftButton")->SetEnable(false);
			mainCanvas->GetCanvasUI("SettingCanvase")->GetButtonUI("GraphicsRightButton")->SetEnable(false);

			mainCanvas->GetCanvasUI("SettingCanvase")->GetTextUI("LanguageText")->SetEnable(false);
			mainCanvas->GetCanvasUI("SettingCanvase")->GetButtonUI("LanguageLeftButton")->SetEnable(false);
			mainCanvas->GetCanvasUI("SettingCanvase")->GetButtonUI("LanguageRightButton")->SetEnable(false);
			mainCanvas->GetCanvasUI("SettingCanvase")->GetTextUI("KoreanText")->SetEnable(false);
			mainCanvas->GetCanvasUI("SettingCanvase")->GetTextUI("EnglishText")->SetEnable(false);
		});

		languageLeftButton->SetClickEvent([&]
		{
			mainCanvas->GetCanvasUI("SettingCanvase")->GetTextUI("EnglishText")->SetEnable(true);
			mainCanvas->GetCanvasUI("SettingCanvase")->GetTextUI("KoreanText")->SetEnable(false);
		});

		languageRightButton->SetClickEvent([&]
		{
			mainCanvas->GetCanvasUI("SettingCanvase")->GetTextUI("KoreanText")->SetEnable(true);
			mainCanvas->GetCanvasUI("SettingCanvase")->GetTextUI("EnglishText")->SetEnable(false);
		});

		graphicsLeftButton->SetClickEvent([&]
		{
			// list 사용해서 만들어 보자 
			/*switch (nowWindowSize)
			{
			case Click0:
			{
				nowWindowSize = WindowSize::Click1;
				mainCanvas->GetCanvasUI("SettingCanvase")->GetTextUI("WindowSize2Text")->SetEnable(true);
				mainCanvas->GetCanvasUI("SettingCanvase")->GetTextUI("WindowSize1Text")->SetEnable(false);
				mainCanvas->GetCanvasUI("SettingCanvase")->GetTextUI("WindowSize3Text")->SetEnable(false);
				break;
			}
			case Click1:
				nowWindowSize = WindowSize::Click2;
				mainCanvas->GetCanvasUI("SettingCanvase")->GetTextUI("WindowSize3Text")->SetEnable(true);
				mainCanvas->GetCanvasUI("SettingCanvase")->GetTextUI("WindowSize2Text")->SetEnable(false);
				mainCanvas->GetCanvasUI("SettingCanvase")->GetTextUI("WindowSize1Text")->SetEnable(false);
				break;
			case Click2:
				nowWindowSize = WindowSize::Click3;
				mainCanvas->GetCanvasUI("SettingCanvase")->GetTextUI("WindowSize1Text")->SetEnable(true);
				mainCanvas->GetCanvasUI("SettingCanvase")->GetTextUI("WindowSize2Text")->SetEnable(false);
				mainCanvas->GetCanvasUI("SettingCanvase")->GetTextUI("WindowSize3Text")->SetEnable(false);
				break;
			case Click3:
				nowWindowSize = WindowSize::Click4;
				mainCanvas->GetCanvasUI("SettingCanvase")->GetTextUI("WindowSize2Text")->SetEnable(true);
				mainCanvas->GetCanvasUI("SettingCanvase")->GetTextUI("WindowSize1Text")->SetEnable(false);
				mainCanvas->GetCanvasUI("SettingCanvase")->GetTextUI("WindowSize3Text")->SetEnable(false);
				break;
			case Click4:
				nowWindowSize = WindowSize::Click4;
				mainCanvas->GetCanvasUI("SettingCanvase")->GetTextUI("WindowSize3Text")->SetEnable(true);
				mainCanvas->GetCanvasUI("SettingCanvase")->GetTextUI("WindowSize2Text")->SetEnable(false);
				mainCanvas->GetCanvasUI("SettingCanvase")->GetTextUI("WindowSize1Text")->SetEnable(false);
				break;
			default:
				break;
			}*/
		});

		graphicsRightButton->SetClickEvent([&]
		{
			/*switch (nowWindowSize)
			{
			case Click4:
			{
				nowWindowSize = WindowSize::Click3;
				mainCanvas->GetCanvasUI("SettingCanvase")->GetTextUI("WindowSize2Text")->SetEnable(true);
				mainCanvas->GetCanvasUI("SettingCanvase")->GetTextUI("WindowSize3Text")->SetEnable(false);
				mainCanvas->GetCanvasUI("SettingCanvase")->GetTextUI("WindowSize1Text")->SetEnable(false);
				break;
			}
			case Click3:
				nowWindowSize = WindowSize::Click2;
				mainCanvas->GetCanvasUI("SettingCanvase")->GetTextUI("WindowSize1Text")->SetEnable(true);
				mainCanvas->GetCanvasUI("SettingCanvase")->GetTextUI("WindowSize2Text")->SetEnable(false);
				mainCanvas->GetCanvasUI("SettingCanvase")->GetTextUI("WindowSize3Text")->SetEnable(false);
				break;
			case Click2:
				nowWindowSize = WindowSize::Click1;
				mainCanvas->GetCanvasUI("SettingCanvase")->GetTextUI("WindowSize3Text")->SetEnable(true);
				mainCanvas->GetCanvasUI("SettingCanvase")->GetTextUI("WindowSize1Text")->SetEnable(false);
				mainCanvas->GetCanvasUI("SettingCanvase")->GetTextUI("WindowSize2Text")->SetEnable(false);

				break;
			default:
				break;
			}*/
		});

		etcButton->SetClickEvent([&]
		{
			mainCanvas->GetCanvasUI("SettingCanvase")->GetTextUI("LanguageText")->SetEnable(true);
			mainCanvas->GetCanvasUI("SettingCanvase")->GetButtonUI("LanguageLeftButton")->SetEnable(true);
			mainCanvas->GetCanvasUI("SettingCanvase")->GetButtonUI("LanguageRightButton")->SetEnable(true);
			mainCanvas->GetCanvasUI("SettingCanvase")->GetTextUI("KoreanText")->SetEnable(true);

			mainCanvas->GetCanvasUI("SettingCanvase")->GetTextUI("FullVolumeText")->SetEnable(false);
			mainCanvas->GetCanvasUI("SettingCanvase")->GetSpriteUI("FullVolumeTexture")->SetEnable(false);
			mainCanvas->GetCanvasUI("SettingCanvase")->GetTextUI("BgmVolumeText")->SetEnable(false);
			mainCanvas->GetCanvasUI("SettingCanvase")->GetSpriteUI("BgmVolumeTexture")->SetEnable(false);
			mainCanvas->GetCanvasUI("SettingCanvase")->GetTextUI("EffectVolume")->SetEnable(false);
			mainCanvas->GetCanvasUI("SettingCanvase")->GetSpriteUI("EffectVolumeTexture")->SetEnable(false);

			mainCanvas->GetCanvasUI("SettingCanvase")->GetTextUI("SFXText")->SetEnable(false);
			mainCanvas->GetCanvasUI("SettingCanvase")->GetSpriteUI("SFXBarTexture")->SetEnable(false);
			mainCanvas->GetCanvasUI("SettingCanvase")->GetTextUI("WindowSizeText")->SetEnable(false);
			mainCanvas->GetCanvasUI("SettingCanvase")->GetTextUI("WindowSize1Text")->SetEnable(false);
			mainCanvas->GetCanvasUI("SettingCanvase")->GetButtonUI("GraphicsLeftButton")->SetEnable(false);
			mainCanvas->GetCanvasUI("SettingCanvase")->GetButtonUI("GraphicsRightButton")->SetEnable(false);

		});

		closeButton->SetClickEvent([&]
		{
			mainCanvas->GetCanvasUI("SettingCanvase")->GetTextUI("SFXText")->SetEnable(false);
			mainCanvas->GetCanvasUI("SettingCanvase")->GetSpriteUI("SFXBarTexture")->SetEnable(false);
			mainCanvas->GetCanvasUI("SettingCanvase")->GetTextUI("WindowSizeText")->SetEnable(false);
			mainCanvas->GetCanvasUI("SettingCanvase")->GetTextUI("WindowSize1Text")->SetEnable(false);
			mainCanvas->GetCanvasUI("SettingCanvase")->GetButtonUI("GraphicsLeftButton")->SetEnable(false);
			mainCanvas->GetCanvasUI("SettingCanvase")->GetButtonUI("GraphicsRightButton")->SetEnable(false);

			mainCanvas->GetCanvasUI("SettingCanvase")->GetTextUI("FullVolumeText")->SetEnable(false);
			mainCanvas->GetCanvasUI("SettingCanvase")->GetSpriteUI("FullVolumeTexture")->SetEnable(false);
			mainCanvas->GetCanvasUI("SettingCanvase")->GetTextUI("BgmVolumeText")->SetEnable(false);
			mainCanvas->GetCanvasUI("SettingCanvase")->GetSpriteUI("BgmVolumeTexture")->SetEnable(false);
			mainCanvas->GetCanvasUI("SettingCanvase")->GetTextUI("EffectVolume")->SetEnable(false);
			mainCanvas->GetCanvasUI("SettingCanvase")->GetSpriteUI("EffectVolumeTexture")->SetEnable(false);

			mainCanvas->GetCanvasUI("TitleCanvas")->GetButtonUI("StartButton")->SetEnable(true);
			mainCanvas->GetCanvasUI("TitleCanvas")->GetTextUI("StartText")->SetEnable(true);

			mainCanvas->GetCanvasUI("TitleCanvas")->GetButtonUI("ContinueButton")->SetEnable(true);
			mainCanvas->GetCanvasUI("TitleCanvas")->GetTextUI("ContinueText")->SetEnable(true);

			mainCanvas->GetCanvasUI("TitleCanvas")->GetButtonUI("SettingButton")->SetEnable(true);
			mainCanvas->GetCanvasUI("TitleCanvas")->GetTextUI("SettingText")->SetEnable(true);

			mainCanvas->GetCanvasUI("TitleCanvas")->GetButtonUI("CreditButton")->SetEnable(true);
			mainCanvas->GetCanvasUI("TitleCanvas")->GetTextUI("CreditText")->SetEnable(true);

			mainCanvas->GetCanvasUI("TitleCanvas")->GetButtonUI("ExitButton")->SetEnable(true);
			mainCanvas->GetCanvasUI("TitleCanvas")->GetTextUI("ExitText")->SetEnable(true);

			mainCanvas->GetCanvasUI("SettingCanvase")->SetEnable(false);
			mainCanvas->GetCanvasUI("InGameMenuCanvas")->SetEnable(false);
		});

		canvas->SetEnable(false);
	}

	void UICreator::CreateInMenuCanvas()
	{
		auto canvas = mainCanvas->GetCanvasUI("InGameMenuCanvas");

		auto canvasTexture = GameEngineSpace::GraphicsManager::GetInstance()->LoadTexture(L"Resources/UI/MenuBack.png");

		auto inventoryButtonTexture = GameEngineSpace::GraphicsManager::GetInstance()->LoadTexture(L"Resources/UI/Inventory.png");

		auto cotrolButtonTexture = GameEngineSpace::GraphicsManager::GetInstance()->LoadTexture(L"Resources/UI/Control.png");

		auto settingButtonTexture = GameEngineSpace::GraphicsManager::GetInstance()->LoadTexture(L"Resources/UI/Setting.png");

		auto settingBackgroundtexture = GameEngineSpace::GraphicsManager::GetInstance()->LoadTexture(L"Resources/UI/SettingUI2.png");

		//==================(InGameMenuBackGround)===============================
		auto  menuBackground = canvas->CreateSpriteUI("Menu");
		menuBackground->SetTexture(canvasTexture);
		menuBackground->SetPosition(Vector3{ 0.f, -465.f, 0.5f });
		menuBackground->SetWidth(1980.f);
		menuBackground->SetHeight(180.f);
		menuBackground->SetEnable(true);

		//==================(InGameInventoryButton)===============================
		auto inventoryButton = canvas->CreateButtonUI("InventoryButton");
		inventoryButton->SetPosition(Vector3{ -485.f, -460.f, 0.4f });
		inventoryButton->SetDefaultTexture(inventoryButtonTexture);
		inventoryButton->SetWidth(120.f);
		inventoryButton->SetHeight(120.f);
		inventoryButton->SetEnable(true);

		//==================(InGameControlButton)===============================
		auto cotrolButton = canvas->CreateButtonUI("ControlButton");
		cotrolButton->SetPosition(Vector3{ 36.f, -460.f, 0.4f });
		cotrolButton->SetDefaultTexture(cotrolButtonTexture);
		cotrolButton->SetWidth(120.f);
		cotrolButton->SetHeight(120.f);
		cotrolButton->SetEnable(true);

		//==================(InGameSettingButton)===============================
		auto settingButton = canvas->CreateButtonUI("SettingButton");
		settingButton->SetPosition(Vector3{ 579.f, -452.f, 0.4f });
		settingButton->SetDefaultTexture(settingButtonTexture);
		settingButton->SetWidth(120.f);
		settingButton->SetHeight(120.f);
		settingButton->SetEnable(true);


		//==================(InGame SetClickEvents)===============================

		settingButton->SetClickEvent([&]
		{
			mainCanvas->GetCanvasUI("SettingCanvase")->SetEnable(true);
		});

		inventoryButton->SetClickEvent([&]
		{


		});

		cotrolButton->SetClickEvent([&]
		{


		});

		canvas->SetEnable(false);
	}

	void UICreator::CreateGameHUD()
	{
		auto canvas = mainCanvas->GetCanvasUI("InGameCanvas");

		auto character = GameEngineSpace::GraphicsManager::GetInstance()->LoadTexture(L"Resources/UI/Character1.png");

		auto boss = GameEngineSpace::GraphicsManager::GetInstance()->LoadTexture(L"Resources/UI/Boss.png");

		auto hpTexture = GameEngineSpace::GraphicsManager::GetInstance()->LoadTexture(L"Resources/UI/HP_Bar2.png");

		auto hpBackGroundTexture = GameEngineSpace::GraphicsManager::GetInstance()->LoadTexture(L"Resources/UI/HP_Bar_BG2.png");

		auto systemTexture = GameEngineSpace::GraphicsManager::GetInstance()->LoadTexture(L"Resources/UI/SystemMassage2.png");

		//==================(InGameHud: Character)===============================
		auto  characterTexture = canvas->CreateSpriteUI("Character");
		characterTexture->SetTexture(character);
		characterTexture->SetPosition(Vector3{ -887.f, 455.f, 0.4f });
		characterTexture->SetWidth(146.f);
		characterTexture->SetHeight(164.f);
		characterTexture->SetEnable(true);

		auto hpBar = canvas->CreateProgressBarUI("HpBar");
		hpBar->SetBackgroundTexture(hpBackGroundTexture);
		hpBar->SetBarTexture(hpTexture);
		hpBar->SetFillDirection(FillDir::RIGHT);
		hpBar->SetWidth(300.f);
		hpBar->SetHeight(34.f);
		hpBar->SetPosition(Vector3{ -716.f, 517.f, 0.4f });
		hpBar->SetBarTexRect(0.f, 0.f, 470.f, 71.f);
		hpBar->SetFillPercent(1.0f);
		hpBar->SetEnable(true);

		//==================(InGameHud: Boss & Monsters)===============================
		auto bossTexture = canvas->CreateSpriteUI("Boss");
		bossTexture->SetTexture(boss);
		bossTexture->SetPosition(Vector3{ -242.f, -481.f, 0.4f });
		bossTexture->SetWidth(146.f);
		bossTexture->SetHeight(116.f);
		bossTexture->SetEnable(false);

		auto bossHpBar = canvas->CreateProgressBarUI("bossHpBar");
		bossHpBar->SetBackgroundTexture(hpBackGroundTexture);
		bossHpBar->SetBarTexture(hpTexture);
		bossHpBar->SetFillDirection(FillDir::RIGHT);
		bossHpBar->SetWidth(727.f);
		bossHpBar->SetHeight(35.f);
		bossHpBar->SetPosition(Vector3{ 100.f, -435.f, 0.4f });
		bossHpBar->SetBarTexRect(0.f, 0.f, 470.f, 71.f);
		bossHpBar->SetFillPercent(1.0f);
		bossHpBar->SetEnable(false);

		//==================(InGameHud: SytemMassage)===============================
		auto  sytemMassageTexture = canvas->CreateSpriteUI("SytemMassage");
		sytemMassageTexture->SetTexture(systemTexture);
		sytemMassageTexture->SetPosition(Vector3{ 665.f, 350.f, 0.4f });
		sytemMassageTexture->SetWidth(590.f);
		sytemMassageTexture->SetHeight(225.f);

		auto sytemMassageText = canvas->CreateTextUI("SytemMassageText");
		std::wstring sytemMassageTextString = L"System Massage";
		sytemMassageText->SetPosition(Vector3{ 884.f, 433.f, 0.4f });
		sytemMassageText->SetText(sytemMassageTextString);
		sytemMassageText->SetFontSize(30.f);

		//==================(InGameHud: Money)===============================
		auto goldTexture = canvas->CreateSpriteUI("Gold");
		goldTexture->SetTexture(systemTexture);
		goldTexture->SetPosition(Vector3{ 888.f, 502.f, 0.4f });
		goldTexture->SetWidth(123.f);
		goldTexture->SetHeight(72.f);

		auto goldText = canvas->CreateTextUI("GoldText");
		std::wstring goldTextString = L"Gold";
		goldText->SetPosition(Vector3{ 904.f, 499.f, 0.4f });
		goldText->SetText(goldTextString);
		goldText->SetFontSize(30.f);
		canvas->SetEnable(false);
	}
}
