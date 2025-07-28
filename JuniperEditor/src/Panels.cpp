#include <imgui/imgui.h>
#include "Panels.h"
#include "Utils.h"

namespace Juniper { namespace Panels {

	void ScenePanel(Scene& scene)
	{
		static Entity selectedEntity = NULL_ENTITY;

		ImGui::Begin("Scene");

		static Entity entityToRename = NULL_ENTITY;
		static char newTag[256] = {};
		static bool shouldFocusInput = false;

		// Right click to open context menu
		if (ImGui::BeginPopupContextWindow("SceneContext", ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems)) {
			if (ImGui::MenuItem("Create Entity")) {
				Entity newEntity = scene.CreateEntity();
				scene.AddComponent<TagComponent>(newEntity, TagComponent{ "New Entity" });

				entityToRename = newEntity;
				strncpy_s(newTag, sizeof(newTag), "New Entity", _TRUNCATE);
				shouldFocusInput = true; // rename on creation
			}
			ImGui::EndPopup();
		}

		// Entity list
		scene.GetView<TagComponent>().each([&](Entity entity, TagComponent& tag) {
			ImGui::PushID(static_cast<int>(entity));

			if (entityToRename == entity) {
				if (shouldFocusInput) {
					ImGui::SetKeyboardFocusHere();
					shouldFocusInput = false;
				}

				if (ImGui::InputText("##Rename", newTag, sizeof(newTag), ImGuiInputTextFlags_EnterReturnsTrue)) {
					tag.Tag = newTag;
					entityToRename = NULL_ENTITY;
				}

				// Cancel rename
				if (ImGui::IsKeyPressed(ImGuiKey_Escape) ||
					(!ImGui::IsItemActive() && !ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))) {
					entityToRename = NULL_ENTITY;
				}
			}
			else {
				if (ImGui::Selectable(tag.Tag.c_str(), selectedEntity == entity)) {
					selectedEntity = entity;
				}

				if (ImGui::BeginPopupContextItem()) {
					if (ImGui::MenuItem("Rename")) {
						entityToRename = entity;
						strncpy_s(newTag, sizeof(newTag), tag.Tag.c_str(), _TRUNCATE);
						shouldFocusInput = true;
					}
					if (ImGui::MenuItem("Delete")) {
						scene.DestroyEntity(entity);
						selectedEntity = NULL_ENTITY;
					}
					ImGui::EndPopup();
				}
			}

			ImGui::PopID();
			});

		ImGui::End();

		if (selectedEntity == NULL_ENTITY) return;

		ImGui::Begin("Entity");

		// Add component
		if (ImGui::Button("Add Component")) {
			ImGui::OpenPopup("AddComponentPopup");
		}

		if (ImGui::BeginPopup("AddComponentPopup")) {
			if (ImGui::MenuItem("Transform")) {
				scene.AddComponent<TransformComponent>(selectedEntity);
			}
			if (ImGui::MenuItem("Sprite")) {
				scene.AddComponent<SpriteComponent>(selectedEntity);
			}
			ImGui::EndPopup();
		}

		// Edit Transform
		if (scene.HasComponents<TransformComponent>(selectedEntity))
		{
			TransformComponent& transform = scene.Get<TransformComponent>(selectedEntity);
			if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
				ImGui::SliderFloat3("Translate", &transform.Translation.x, -10.0f, 10.0f);
				ImGui::SliderFloat3("Rotate", &transform.Rotation.x, 0.0f, 360.0f);
				ImGui::SliderFloat3("Scale", &transform.Scale.x, -10.0f, 10.0f);
			}
			if (ImGui::Button("Remove##Transform")) {
				scene.RemoveComponent<TransformComponent>(selectedEntity);
			}
		}

		// Edit Sprite
		if (scene.HasComponents<SpriteComponent>(selectedEntity))
		{
			SpriteComponent& sprite = scene.Get<SpriteComponent>(selectedEntity);
			if (ImGui::CollapsingHeader("Sprite", ImGuiTreeNodeFlags_DefaultOpen)) {
				ImGui::ColorEdit4("Color", &sprite.Color.r);
				if (ImGui::Button("Add Texture"))
				{
					std::string texturePath = Utils::OpenFileDialog();
					sprite.Texture = std::make_shared<Texture2D>(texturePath);
				}
			}
			if (ImGui::Button("Remove##Sprite")) {
				scene.RemoveComponent<SpriteComponent>(selectedEntity);
			}
		}
		// Edit Tilemap
		if (scene.HasComponents<TilemapComponent>(selectedEntity))
		{
			TilemapComponent& tilemap = scene.Get<TilemapComponent>(selectedEntity);
			if (ImGui::CollapsingHeader("Tilemap", ImGuiTreeNodeFlags_DefaultOpen)) {
				if (ImGui::Button("Add Tilemap"))
				{
					std::string tmxPath = Utils::OpenFileDialog();
					tilemap.Tilemap = std::make_shared<Tilemap>(tmxPath);
				}
			}
			if (ImGui::Button("Remove##Tilemap")) {
				scene.RemoveComponent<TilemapComponent>(selectedEntity);
			}
		}

		ImGui::End();
	}

	void StatsPanel()
	{
		ImGui::Begin("Render Stats");
		Stats stats = Renderer::GetStats();

		ImGui::Text("Draw calls: %i", stats.DrawCalls);
		ImGui::Text("Quad count %i", stats.QuadCount);
		ImGui::Text("Texture slots used: %i", stats.TextureSlotsUsed);

		Renderer::ResetStats();
		ImGui::End();
	}

}}