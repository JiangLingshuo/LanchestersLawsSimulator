#pragma once

struct GLFWwindow;

class GlfwWindow
{
public:
    bool Initialize(const char* title, int baseWidth, int baseHeight);
    void Shutdown();

    bool ShouldClose() const;
    GLFWwindow* GetNativeWindow() const;
    const char* GetGlslVersion() const;
    float GetContentScale() const;

private:
    void ConfigureOpenGL();

    GLFWwindow* m_Window = nullptr;
    const char* m_GlslVersion = "#version 130";
    float m_ContentScale = 1.0f;
};
