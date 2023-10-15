#include "gui.h"
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <cmath>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl2.h>

#include <glfw/glfw3.h>
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "OpenGL32.lib")

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

static void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

const ImVec4 ClearColor = ImColor(0.1f, 0.1f, 0.1f, 1.0f);

Gui::Gui(const Data& data) : mData(data) {
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) exit(1);

    m_glfwWindow = glfwCreateWindow(640, 480, "Solver", nullptr, nullptr);
    if (m_glfwWindow == nullptr) exit(1);

    glfwMakeContextCurrent(m_glfwWindow);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(m_glfwWindow, true);
    ImGui_ImplOpenGL2_Init();
}

Gui::~Gui() {
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(m_glfwWindow);
    glfwTerminate();
}

void Gui::run() {
    while (!glfwWindowShouldClose(m_glfwWindow)) {
        glfwPollEvents();

        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        render();
       
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(m_glfwWindow, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(ClearColor.x, ClearColor.y, ClearColor.z, ClearColor.w);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

        glfwMakeContextCurrent(m_glfwWindow);
        glfwSwapBuffers(m_glfwWindow);
    }
}

void Gui::render() {

    ImGui::Begin("Overlay");



    ImGui::End();
}
