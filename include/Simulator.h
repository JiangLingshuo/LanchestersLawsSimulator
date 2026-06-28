#pragma once

#include "GlfwWindow.h"
#include "ImGuiLayer.h"

enum class Phase
{
	ReadingInput,
	Simulating,
	Printing,
	COUNT
};

class Simulator
{
private:
	Phase m_CurrentPhase;

	GlfwWindow m_Window;
	ImGuiLayer m_ImGuiLayer;

	bool Initialize();
	void MainLoop();
	bool ShouldQuit();
	void Update();
	void Render();
	void Shutdown();

public:
	Simulator();
	~Simulator();

	// External Callers
	void Run();
};

