#pragma once

	class Scene_2D : public Scene {
	private:

	public:
		Scene_2D();
		~Scene_2D();

		virtual void Update() override;
		virtual void PreRender() override;
		virtual void Render() override;
		virtual void PostRender() override;
		virtual void GUIRender() override;
};