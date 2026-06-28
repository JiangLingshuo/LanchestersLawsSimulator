#include "ImGuiLayer.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>

bool ImGuiLayer::Initialize(GLFWwindow* window, const char* glslVersion, float contentScale)
{
    if (window == nullptr)
    {
        return false;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(contentScale);
    style.FontScaleDpi = contentScale;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
#ifdef __EMSCRIPTEN__
    ImGui_ImplGlfw_InstallEmscriptenCallbacks(window, "#canvas");
#endif
    ImGui_ImplOpenGL3_Init(glslVersion);

    return true;
}

bool ImGuiLayer::BeginFrame(GLFWwindow* window)
{
    glfwPollEvents();
    if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
    {
        ImGui_ImplGlfw_Sleep(10);
        return false;
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    return true;
}

void ImGuiLayer::DrawSimulationParameters()
{
    ImGuiIO& io = ImGui::GetIO();

    ImGui::SetNextWindowPos(ImVec2(24.0f, 24.0f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(520.0f, 440.0f), ImGuiCond_FirstUseEver);

    ImGui::Begin("Simulation Parameters");

    ImGui::Text("Combat rule blend");
    ImGui::SliderFloat("Melee <-> Range", &m_MeleeRangeBlend, 0.0f, 1.0f, "%.2f");
    ImGui::Text("Melee %.0f%%", (1.0f - m_MeleeRangeBlend) * 100.0f);
    ImGui::SameLine();
    ImGui::Text("Range %.0f%%", m_MeleeRangeBlend * 100.0f);

    ImGui::Separator();

    ImGui::Text("Fixed-step simulation");
    ImGui::SliderFloat("Time Step", &m_TimeStep, 0.001f, 1.0f, "%.3f");
    ImGui::SliderFloat("Collapse Threshold", &m_CollapseThreshold, 0.0f, 0.1f, "%.4f");

    ImGui::Separator();

    ImGui::Text("Attrition coefficients");
    ImGui::SliderFloat("Linear Coefficient", &m_LinearCoefficient, 0.0f, 0.1f, "%.5f");
    ImGui::SliderFloat("Square Coefficient", &m_SquareCoefficient, 0.0f, 0.001f, "%.7f");

    ImGui::Separator();

    ImGui::Text("Initial factions");
    ImGui::SliderFloat("Red Units", &m_RedInitialUnits, 1000.0f, 5000000.0f, "%.0f");
    ImGui::SliderFloat("Red Effectiveness", &m_RedEffectiveness, 0.0f, 10.0f, "%.2f");
    ImGui::SliderFloat("Blue Units", &m_BlueInitialUnits, 1000.0f, 5000000.0f, "%.0f");
    ImGui::SliderFloat("Blue Effectiveness", &m_BlueEffectiveness, 0.0f, 10.0f, "%.2f");

    ImGui::Separator();

    ImGui::ColorEdit3("Background", m_ClearColor);
    ImGui::Text("Frame %.3f ms (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

    ImGui::End();
}

void ImGuiLayer::EndFrame(GLFWwindow* window)
{
    ImGui::Render();

    int displayWidth = 0;
    int displayHeight = 0;
    glfwGetFramebufferSize(window, &displayWidth, &displayHeight);
    glViewport(0, 0, displayWidth, displayHeight);
    glClearColor(
        m_ClearColor[0] * m_ClearColor[3],
        m_ClearColor[1] * m_ClearColor[3],
        m_ClearColor[2] * m_ClearColor[3],
        m_ClearColor[3]);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
}

void ImGuiLayer::Shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
