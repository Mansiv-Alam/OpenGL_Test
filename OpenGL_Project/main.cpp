#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Windows.h>
#include <iostream>

// Callback to resize the viewport when the window size changes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


int main()
{
    HWND consoleWindow = GetConsoleWindow();
    ShowWindow(consoleWindow, SW_HIDE); // hides the console

    // Initialize GLFW
    if (!glfwInit())
    {
        //std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    // Configure GLFW to use OpenGL 3.3 core profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // 3.x
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // x.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a window
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Testing", NULL, NULL);
    if (window == NULL)
    {
        //std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        //std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    // Binding object tests
    unsigned int VAO; // Create the object
    glGenVertexArrays(1, &VAO); // Store the reference to the Object as an ID
    glBindVertexArray(VAO); // Binds the Object as the current active object in the OpenGL context

    // Set viewport and resize callback
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glBindVertexArray(0); // optional unbind (sets the object id of the window target to 0)

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        float red = 121.0 / 255.0;
        float green = 175.0 / 255.0;
        float blue =  199.0 / 255.0;

        // Clear the screen with a specified color
        glClearColor(red, green, blue, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);



        // Swap buffers and poll for events
        glfwSwapBuffers(window); // Puts the drawn screen onto the main screen (back buffer > front buffer)
        glfwPollEvents(); // Checks for window events / user input
    }
    
    // Cleanup
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}