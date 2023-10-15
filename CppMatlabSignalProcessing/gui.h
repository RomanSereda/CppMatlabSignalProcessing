#pragma once

struct GLFWwindow;

class Gui
{
public:
    Gui();
    ~Gui();
    void run();

private:
    GLFWwindow* m_glfwWindow = nullptr;
};
