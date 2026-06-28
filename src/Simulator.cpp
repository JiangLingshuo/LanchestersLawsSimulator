#include "Simulator.h"

Simulator::Simulator()
{
    
}

Simulator::~Simulator()
{

} 


bool Simulator::Initialize()
{
    if (!m_Window.Initialize("Lanchester's Laws Simulator", 1280, 800))
    {
        return false;
    }

    return m_ImGuiLayer.Initialize(m_Window.GetNativeWindow(), m_Window.GetGlslVersion(), m_Window.GetContentScale());
}

void Simulator::MainLoop()
{

    Update();
	Render();


    //// Initialize with N units
    //size_t unitCount = 10000;
    //m_RedArmy.firePowers.resize(unitCount, 10.0f);
    //m_RedArmy.healths.resize(unitCount, 100.0f);

    //m_BlueArmy.firePowers.resize(unitCount, 12.0f);
    //m_BlueArmy.healths.resize(unitCount, 90.0f);

    //// Example: Apply damage to all units
    //for (size_t i = 0; i < unitCount; ++i) {
    //    m_RedArmy.healths[i] -= m_BlueArmy.firePowers[i] * 0.1f;
    //    m_BlueArmy.healths[i] -= m_RedArmy.firePowers[i] * 0.1f;
    //}
}

bool Simulator::ShouldQuit()
{
    return m_Window.ShouldClose();
}

void Simulator::Update()
{

}

void Simulator::Render()
{
    GLFWwindow* nativeWindow = m_Window.GetNativeWindow();
    if (m_ImGuiLayer.BeginFrame(nativeWindow))
    {
        m_ImGuiLayer.DrawSimulationParameters();
        m_ImGuiLayer.EndFrame(nativeWindow);
    }
}

void Simulator::Shutdown()
{
    m_ImGuiLayer.Shutdown();
    m_Window.Shutdown();
}

void Simulator::Run()
{
    if (!Initialize())
    {
        return;
    }
     
    // Simulate
    while (!ShouldQuit())
    {
        MainLoop();
    }

	Shutdown();
}
