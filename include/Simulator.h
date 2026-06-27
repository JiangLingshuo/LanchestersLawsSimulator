#pragma once
#include <array>

#include "Military.h"

#define LOOP 2000
#define FACTION_COUNT 2
#define MILITARY_COUNT 1000

struct GLFWwindow;

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

	MilitaryData m_RedArmy;
	MilitaryData m_BlueArmy;

	GLFWwindow* m_Window;

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

