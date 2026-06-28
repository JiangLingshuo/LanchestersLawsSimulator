#pragma once

struct GLFWwindow;

class ImGuiLayer
{
public:
    bool Initialize(GLFWwindow* window, const char* glslVersion, float contentScale);
    bool BeginFrame(GLFWwindow* window);
    void DrawSimulationParameters();
    void EndFrame(GLFWwindow* window);
    void Shutdown();

private:
    float m_ClearColor[4] = { 0.45f, 0.55f, 0.60f, 1.00f };
    float m_MeleeRangeBlend = 0.5f;
    float m_TimeStep = 0.1f;
    float m_LinearCoefficient = 0.01f;
    float m_SquareCoefficient = 0.00001f;
    float m_CollapseThreshold = 0.001f;
    float m_RedInitialUnits = 1000000.0f;
    float m_BlueInitialUnits = 1000000.0f;
    float m_RedEffectiveness = 1.0f;
    float m_BlueEffectiveness = 1.0f;
};
