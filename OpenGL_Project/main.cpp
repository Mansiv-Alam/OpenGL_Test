#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <windows.h>
#include <iostream>

// Callback to resize the viewport when the window size changes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// Key inputs 
void processInput(GLFWwindow* window)
{
    // Use the glfw get key function (returns GLFW_PRESS or GLFW_RELEASE)
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true); // Closes the window if escape is pressed 
    }
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

    // Making 3 verticies (z coordinate is 0)
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };

    // Binding object tests
    unsigned int VAO; // Create the object
    glGenVertexArrays(1, &VAO); // Store the reference to the Object as an ID
    glBindVertexArray(VAO); // Binds the Object as the current active object in the OpenGL context

    // Make a VBO (Vertex Buffer Object) to send all the vertex data at once
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // VBO buffer type is an array buffer (binds the array data to the GL_ARRAY_BUFFER target)
    // Copies the vertex data in to the current bound buffer, 
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // GL_ARRAY_BUFFER = target buffer, sizeof(vertices) = size of the data, vertices = data to transfer, GL_STATIC_DRAW = how to use the data 
    // GL_STREAM_DRAW = data is set once and used a few times, GL_STATIC_DRAW = data is set once and used many times, GL_DYNAMIC_DRAW = data is changed alot and used many times

    // Write the vector shader 
    const char* vertexShaderSource = "#version 330 core\n" // Use GLSL 3.3 (the shader language for OpenGL 3.3)
        "layout (location = 0) in vec3 aPos;\n" // declare an input variable as aPos which is a vec3 or 3d vector type  
        "void main()"
        "{\n"
        " gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n" // Tells the gpu where to place the vector onto the screen, must cast to 4d vector, gl_Position is required in every vector shader
        "}\0";
    // Create and compile a shader object 
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER); // Indicate the type of vector it is
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // argument 1 = shader to compile, argument 2 = number of strings passed through as source code, argument 3 = source code of the vector shader, 
    // argument 4 = length of the source code (null here because we want the function to read till the end of the source code)
    glCompileShader(vertexShader);

    // Set viewport and resize callback
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glBindVertexArray(0); // optional unbind (sets the object id of the window target to 0)

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        float red = 121.0 / 255.0;
        float green = 175.0 / 255.0;
        float blue =  199.0 / 255.0;

        // Clear the screen with a specified color
        glClearColor(red, green, blue, 1.0f); // State-setting function
        glClear(GL_COLOR_BUFFER_BIT); // State-using function

        processInput(window);

        // Swap buffers and poll for events
        glfwSwapBuffers(window); // Puts the drawn screen onto the main screen (back buffer > front buffer)
        glfwPollEvents(); // Checks for window events / user input
    }
    
    // Cleanup
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}