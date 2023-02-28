
#include "../external/ImGui/imgui.h"
#ifdef _WINDOWS
#include "../external/ImGui/imgui_impl_win32.h"
#endif // _WINDOWS
#include "../external/ImGui/imgui_impl_dx11.h"

#include "util_funcs.h"
#include "../entities/sun.h"
#include "render/diffuse_lighting.h"
#include "render/aggregator.h"
#include "scene.h"
#include "../core/platform.h"
#include "../core/sys_ticker.h"
#include "presenter.h"

#include "overlay.h"

Overlay::Overlay(Presenter* parent)
	:presenter(parent), show(false)
{
#ifdef _WINDOWS
	RetAssert(ImGui_ImplWin32_Init(parent->QueryService<Platform*>("platform")->GetWindowHandle()));
#endif // _WINDOWS
	RetAssert(ImGui_ImplDX11_Init(parent->GetDevice(), parent->GetContext()));
	ticker = parent->QueryService<SystemTicker*>("ticker");
	scene = parent->GetActiveScene();
	aggregator = scene->GetAggregator();
	sun = scene->GetSun();
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
		}

		if (ImGui::CollapsingHeader("Objects"))
		{
			static DirectX::XMFLOAT3 _rotation = {0.0f, 0.0f, 0.0f};
			ImGui::DragFloat3("Object Rotation", (float*)&_rotation, 1.0f, 0.0f, 360.0f);
			aggregator->render_lighting_diffuse->object->SetRotation(_rotation.x/57.2958f, _rotation.y/ 57.2958f, _rotation.z/ 57.2958f);
		}

		ImGui::Separator();

		ImGui::TextColored(ImVec4(1, 1, 0, 1), "Performance");
		ImGui::Text("Last Tick Duration: %fms", ticker->GetLastTickDuration() * 1000);
		ImGui::Text("Ticks Per Second: %u", ticker->GetTickPerSecond());

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
