#pragma once

struct GLFWwindow;
struct Data;

class Gui
{
public:
    Gui(const Data& data);
    ~Gui();
    void run();

private:
    const Data& mData;
    GLFWwindow* m_glfwWindow = nullptr;

    void render();
};
