/* Created by Maxime Sevigny, 10/03/2026: Added function to handle setup / stop ImGui + DisplayOverlay()
	Modified by
		
*/

/*
* This file contains function used to help with the display
* All the ImGui functions should only be compiled when using ImGui
*/

#pragma once

#include "Util.h"

#if IS_USING_IMGUI

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"
#include <stdio.h>

inline void DisplayOverlay(float elapsedMillisec, float elapsed_second)
{
	ImGui::Begin("Performance");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", elapsedMillisec, 1.0 / elapsed_second);
	ImGui::End();
}

inline void SetupImGuiContext(SDL_Window* window, SDL_Renderer* renderer)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	ImGui::StyleColorsDark();

	ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer3_Init(renderer);
}

inline void SetupImGuiFrame()
{
	ImGui_ImplSDLRenderer3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();
}

inline void StopImGui()
{
	ImGui_ImplSDLRenderer3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();
}

#endif // IS_USING_IMGUI