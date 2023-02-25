#pragma once


namespace ClientSpace
{
	using GraphicsEngineSpace::Canvas;
	using GraphicsEngineSpace::UIBase;

	class SettingUIController : public ComponentBase
	{

	public:
		SettingUIController(std::weak_ptr<GameObject> obj);
		~SettingUIController();

	private:
		// 내부적으로 그래픽스 매니저 캐싱
		//std::shared_ptr<GraphicsManager> graphicsInstance;
		std::shared_ptr<Canvas> mainCanvas;

	public:
		virtual void Awake() override;
		virtual void Start() override;

		virtual void Update(float tick) override;
		virtual void LateUpdate(float tick) override;

	private:

	};

}