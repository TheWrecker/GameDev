
#include "../external/ImGui/imgui.h"
#ifdef _WINDOWS
#include "../external/ImGui/imgui_impl_win32.h"
#endif // _WINDOWS
#include "../external/ImGui/imgui_impl_dx11.h"

#include "util_funcs.h"
#include "../input/keyboard.h"
#include "../input/mouse.h"
#include "../gameplay/items/item_container.h"
#include "../entities/player.h"
#include "../scene/elements/renderable_frustrum.h"
#include "../scene/elements/sun.h"
#include "../scene/compartments/segment.h"
#include "../scene/world.h"
#include "../scene/scene.h"
#include "render/render_dev.h"
#include "render/aggregator.h"
#include "../core/platform.h"
#include "../core/sys_ticker.h"
#include "presenter.h"
#include "../gameplay/physics_engine.h"
#include "../gameplay/world_engine.h"

#include "overlay.h"

std::vector<std::string> InteractionModeNames = {
	"Default",
	"Dig Mode",
	"Placement Mode"
};

Overlay::Overlay(Presenter* parent)
	:presenter(parent), show(false)
{
}

Overlay::~Overlay()
{
#ifdef _WINDOWS
	ImGui_ImplWin32_Shutdown();
#endif // _WINDOWS
	ImGui_ImplDX11_Shutdown();
	ImGui::DestroyContext();
}

void Overlay::Show()
{
	show = true;
}

void Overlay::Hide()
{
	show = false;
}

void Overlay::ToggleShow()
{
	show = !show;
}

bool Overlay::Initialize()
{
	#ifdef _WINDOWS
		RetAssert(ImGui_ImplWin32_Init(presenter->QueryService<Platform*>("platform")->GetWindowHandle()));
	#endif // _WINDOWS

	RetAssert(ImGui_ImplDX11_Init(presenter->GetDevice(), presenter->GetContext()));

	ticker = presenter->QueryService<SystemTicker*>("ticker");
	mouse = presenter->QueryService<Mouse*>("mouse");
	keyboard = presenter->QueryService<Keyboard*>("keyboard");

	if (!ticker || !mouse || !keyboard)
		return false;

	physics_engine = presenter->QueryService<PhysicsEngine*>("physics_engine");
	world_engine = presenter->QueryService<WorldEngine*>("world_engine");

	if (!world_engine || !physics_engine)
		return false;

	aggregator = presenter->GetAggregator();
	scene = presenter->QueryService<Scene*>("scene");


	if (!aggregator || !scene)
		return false;

	sun = scene->GetSun();
	player = scene->GetPlayer();

	return sun && player;
}

void Overlay::Draw()
{
	if (!show) return;

	//setup new frame
	ImGui_ImplDX11_NewFrame();
#ifdef _WINDOWS
	ImGui_ImplWin32_NewFrame();
#endif // _WINDOWS
	ImGui::NewFrame();

	//GUI stuff
	if (ImGui::Begin("Developement"))
	{
		/*
		if (ImGui::CollapsingHeader("Scene"))
		{
			static int currentTextureIndex = 0; // Here we store our selection data as an index.
			if (ImGui::BeginCombo("Object Texture", Elements::Texture::names.at(Render::Developement::currentTextureID).c_str()))
			{
				for (unsigned int n = 0; n < Elements::Texture::names.size(); n++)
				{
					const bool is_selected = (currentTextureIndex == n);
					if (ImGui::Selectable(Elements::Texture::names.at(n).c_str(), is_selected))
					{
						currentTextureIndex = n;
						Render::Developement::currentTextureID = n;
					}

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			ImGui::Separator();
			ImGui::ColorPicker4("##picker1", (float*)&Render::Developement::ambientColor, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview);
			ImGui::Separator();
			ImGui::ColorPicker4("##picker2", (float*)&Render::Developement::lightColor, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview);
			ImGui::Separator();
			ImGui::DragFloat3("Light Direction", (float*)&Render::Developement::lightDirection, 0.01f, -1.0f, 1.0f);
		}


		if (ImGui::CollapsingHeader("Graphics"))
		{
			if (ImGui::Button("Toggle Fullscreen"))
				Visuals::ToggleFullscreen();
		}

		if (ImGui::CollapsingHeader("Camera"))
		{
			ImGui::SliderFloat("Sensitivity", &Scene::camera->mouseSensitivity, 1.0f, 100.0f);
			ImGui::SliderFloat("Move Speed", &Scene::camera->moveSpeed, 1.0f, 500.0f);
			ImGui::SliderFloat("Rotation Speed", &Scene::camera->rotationSpeed, 0.01f, 1.0f);
		}

		if (ImGui::CollapsingHeader("Profiler"))
		{
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "Total Hardware Usage");
			ImGui::Text("CPU Load: %u%%", Profiler::cpuLoad);
			ImGui::Text("RAM Load: %u%%", Profiler::ramLoad);
			ImGui::SameLine();
			ImGui::Text("- %u / %u MBs",
				(Profiler::physicalMemoryTotal - Profiler::physicalMemoryAvailable) / MegaByte,
				Profiler::physicalMemoryTotal / MegaByte);
			ImGui::Separator();

			ImGui::TextColored(ImVec4(1, 1, 0, 1), "Storm Field Usage");
			ImGui::Text("CPU Consumption: %u%%", Profiler::cpuConsumption);
			ImGui::Text("Physical Memory: %u / %u MBs", Profiler::physicalMemoryUsed / MegaByte,
				Profiler::physicalMemoryTotal / MegaByte);
			ImGui::Text("Virtual Memory: %u / %u MBs", Profiler::virtualMemoryUsed / MegaByte,
				Profiler::virtualMemoryTotal / MegaByte);
			ImGui::Separator();

			ImGui::TextColored(ImVec4(1, 1, 0, 1), "Performance");
			ImGui::Text("Last Tick Duration: %fms", Profiler::lastIterationDuration * 1000);
			ImGui::Text("Ticks Per Second: %u", Profiler::ips);
		}
		*/

		if (ImGui::CollapsingHeader("Scene"))
		{
			ImGui::DragFloat4("Light Color", (float*)&sun->light_info.color, 0.01f, 0.0f, 1.0f);
			ImGui::Separator();
			ImGui::DragFloat4("Ambient Color", (float*)&sun->light_info.ambient, 0.01f, 0.0f, 1.0f);
			ImGui::Separator();
			ImGui::DragFloat3("Light Direction", (float*)&sun->light_info.direction, 0.01f, -1.0f, 1.0f);

			if (ImGui::Button("Show current Frustrum"))
				scene->renderable_frustrum->UpdateToCamera();
		}

		if (ImGui::CollapsingHeader("Objects"))
		{
			static DirectX::XMFLOAT3 _obj_rotation = aggregator->render_dev->object->Rotation();
			ImGui::DragFloat3("Object Rotation", (float*)&_obj_rotation, 1.0f, 0.0f, 360.0f);
			aggregator->render_dev->object->SetRotation(_obj_rotation.x/57.2958f, _obj_rotation.y/ 57.2958f, _obj_rotation.z/ 57.2958f);

			static DirectX::XMFLOAT3 _obj_scale = aggregator->render_dev->object->Scale();
			ImGui::DragFloat3("Object Scale", (float*)&_obj_scale, 0.1f, 0.01f, 100.0f);
			aggregator->render_dev->object->SetScale(_obj_scale.x, _obj_scale.y, _obj_scale.z);

			static DirectX::XMFLOAT3 _obj_position = aggregator->render_dev->object->Position();
			ImGui::DragFloat3("Object Position", (float*)&_obj_position, 0.5f, -50.01f, 50.0f);
			aggregator->render_dev->object->SetPosition(_obj_position.x, _obj_position.y, _obj_position.z);

			static DirectX::XMFLOAT3 _pln_rotation = aggregator->render_dev->plane->Rotation();
			ImGui::DragFloat3("Plane Rotation", (float*)&_pln_rotation, 1.0f, 0.0f, 360.0f);
			aggregator->render_dev->plane->SetRotation(_pln_rotation.x / 57.2958f, _pln_rotation.y / 57.2958f, _pln_rotation.z / 57.2958f);

			static DirectX::XMFLOAT3 _pln_scale = aggregator->render_dev->plane->Scale();
			ImGui::DragFloat3("Plane Scale", (float*)&_pln_scale, 0.1f, 0.01f, 100.0f);
			aggregator->render_dev->plane->SetScale(_pln_scale.x, _pln_scale.y, _pln_scale.z);

			static DirectX::XMFLOAT3 _pln_position = aggregator->render_dev->plane->Position();
			ImGui::DragFloat3("Plane Position", (float*)&_pln_position, 0.5f, -50.01f, 50.0f);
			aggregator->render_dev->plane->SetPosition(_pln_position.x, _pln_position.y, _pln_position.z);
		}

		if (ImGui::CollapsingHeader("World"))
		{
			ImGui::Checkbox("Physics Enabled", &physics_engine->enabled);
			ImGui::Checkbox("WorldEngine Enabled", &world_engine->enabled);
		}

		if (ImGui::CollapsingHeader("Player"))
		{
			ImGui::Text("Position: x:%f - y:%f - z:%f", player->position.x, player->position.y, player->position.z);
			ImGui::Text("Direction: x:%f - y:%f - z:%f", player->rotation.x, player->rotation.y, player->rotation.z);

			ImGui::Separator();

			ImGui::TextColored(ImVec4(1, 1, 0, 1), "Mode: %s", InteractionModeNames[static_cast<unsigned int>(player->interaction_mode)].c_str());

			ImGui::Separator();

			ImGui::TextColored(ImVec4(1, 1, 0, 1), "Inventory:");
			unsigned int _i = 0;
			for (auto& _item : player->inventory->container)
			{
				if (_i == player->selected_slot)
				{
					ImGui::TextColored(ImVec4(0, 1, 0, 1), "Slot %u: %s - %u / %u",
						_i, _item.GetName().c_str(), _item.GetCurrentCount(), _item.GetCapacity());
				}
				else
				{
					ImGui::Text("Slot %u: %s - %u / %u", _i, _item.GetName().c_str(), _item.GetCurrentCount(), _item.GetCapacity());
				}
				_i++;
			}

		}

		ImGui::Separator();

		ImGui::TextColored(ImVec4(1, 1, 0, 1), "Performance");
		ImGui::Text("Last Tick Duration: %fms", ticker->GetLastTickDuration() * 1000.0f);
		ImGui::Text("Ticks Per Second: %u", ticker->GetTickPerSecond());
		ImGui::Text("Average Tick Duration: %fms", ticker->GetAverageTickDuration() * 1000.0f);

		ImGui::Separator();

		if (ImGui::Button("Toggle Fullscreen"))
			presenter->ToggleFullscreen();

		ImGui::Separator();

		if (ImGui::Button("Exit"))
			PostQuitMessage(0);

	}

	ImGui::End();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void Overlay::Update()
{
}
