#include "GlfwWindow.h"

#include <stdio.h>
#include <GLFW/glfw3.h>

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

bool GlfwWindow::Initialize(const char* title, int baseWidth, int baseHeight)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
    {
        return false;
    }

    ConfigureOpenGL();

    float contentScaleY = 1.0f;
    glfwGetMonitorContentScale(glfwGetPrimaryMonitor(), &m_ContentScale, &contentScaleY);

    m_Window = glfwCreateWindow(
        static_cast<int>(baseWidth * m_ContentScale),
        static_cast<int>(baseHeight * m_ContentScale),
        title,
        nullptr,
        nullptr);

    if (m_Window == nullptr)
    {
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(m_Window);
    glfwSwapInterval(1);

    return true;
}

void GlfwWindow::Shutdown()
{
    if (m_Window != nullptr)
    {
        glfwDestroyWindow(m_Window);
        m_Window = nullptr;
    }

    glfwTerminate();
}

bool GlfwWindow::ShouldClose() const
{
    return m_Window == nullptr || glfwWindowShouldClose(m_Window);
}

GLFWwindow* GlfwWindow::GetNativeWindow() const
{
    return m_Window;
}

const char* GlfwWindow::GetGlslVersion() const
{
    return m_GlslVersion;
}

float GlfwWindow::GetContentScale() const
{
    return m_ContentScale;
}

void GlfwWindow::ConfigureOpenGL()
{
#if defined(IMGUI_IMPL_OPENGL_ES2)
    m_GlslVersion = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(IMGUI_IMPL_OPENGL_ES3)
    m_GlslVersion = "#version 300 es";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    m_GlslVersion = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
    m_GlslVersion = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif
}
