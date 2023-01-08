#pragma once
#include "Define/FootEngineTypedef.h"

// �׷��Ƚ� ���� ����� ���� ���
#include "../../FootGraphicsEngine/GraphicsEngineFramework.h"

namespace GameEngineSpace
{
	using namespace GraphicsEngineSpace;

	/**
	 * \brief ������ �׷��Ƚ� ������ �ʱ�ȭ �ϰ�, ������ �ִ� �Ŵ��� Ŭ����
	 *
	 * �׷��Ƚ� ������ ����� �ʿ��� �� �ش� Ŭ������ �Ű��� �̿��Ѵ�. => ���������� ȭ�鿡 �׸��� �׸��� ģ��..
	 *
	 * 2022.08.02 Dev.BlackFoot
	 */
	class GraphicsManager final
	{
	private:
		static std::shared_ptr<GraphicsManager> instance;

		// ������
		std::shared_ptr<IRenderer> renderer;
		HMODULE graphicsDLL;

		// �� �������� ���� ť..
			// ���� ������ DirLit �ϳ���
	public:
		GraphicsManager() = default;
		~GraphicsManager() = default;
		GraphicsManager(const GraphicsManager& other) = delete;
		GraphicsManager& operator= (const GraphicsManager& other) = delete;
		GraphicsManager(const GraphicsManager&& other) = delete;
		GraphicsManager& operator= (const GraphicsManager&& other) = delete;

	private:
		// ���� ť�� �ִ� ��� �����͸� �׸��ϴ�.
		//void RenderPrefab();

	public:
		bool Init(HWND hWnd, uint32 screenWidth, uint32 screenHeight);
		bool LateInit();
		void Update();
		void Release();

		// �׷��Ƚ� �Ŵ����� ������.. �������� �����ִ°ɷ� �غ���..
		std::shared_ptr<IRenderer> GetRenderer() { return renderer; }

		// ���� �����ݽô�..

		// �� ������
		void PassDirectionalLight(SimpleMath::Vector3 color, SimpleMath::Vector3 direction, float power, SimpleMath::Matrix lightViewProj);
		void PassPointLight(SimpleMath::Vector3 color, SimpleMath::Vector3 position, float power, float range, SimpleMath::Matrix lightViewProj);
		void PassSpotLight(SimpleMath::Vector3 color, SimpleMath::Vector3 direction, float power, float halfAngle, float range, SimpleMath::Matrix lightViewProj);

		void CreateUITest(HWND hWnd);
		void UIRender(float tick);

		std::shared_ptr<Texture> LoadTexture(std::wstring path);

	public:
		static std::shared_ptr<GraphicsManager> GetInstance();

	};
}