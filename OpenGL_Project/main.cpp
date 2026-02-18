#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <windows.h>
#include <iostream>
#include <Shader.h>
#include <stb_image.h>

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

    // Making verticies (z coordinate is 0)
    float vertices[] = {
        // Position         // Colour           // Textures Coordinates
        0.5f, 0.4f, 0.0f, 0.0f, 1.0f, 0.0f,     1.0f, 1.0f,
        0.5f, -0.4f, 0.0f, 1.0f, 0.0f, 0.0f,    1.0f, 0.0f,
        -0.5f, -0.4f, 0.0f, 0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
        -0.5f, 0.4f, 0.0f, 1.0f, 0.0f, 0.0f,     0.0f, 1.0f,
        //0.0f, 0.8f, 0.0f, 0.0f, 1.0f, 0.0f,     0.0f, 1.0f,
        //0.0f, -0.8f, 0.0f, 0.0f, 0.0f, 1.0f,
    };
    unsigned int indices[] = { // Indexed drawing to not write the same vertices repeatedly (allows the reuse of vertices)

        0, 1, 3, // first triangle
        1, 2, 3, // second triangle
        //0, 3, 4, // third triangle
        //1, 2, 5 // fourth triangle

    };
    // Texture coordinates for sampling (retrieving texture colour based of texture coordinates)
    float texCoords[] = {
        0.0f, 0.0f, // lower-left corner
        1.0f, 0.0f, // lower-right corner
        0.5f, 1.0f // top-center corner
    };
    // Multiple types of texture wrapping, GL_REPEAT (default) repeats the texture image, GL_MIRRORED_REPEAT mirrors and repeats the texture image, 
    // GL_CLAMP_TO_EDGE clamps the coordinates between 0 and 1 (stretches the texture image), GL_CLAMP_TO_BORDER: coordinates outside the range is given 
    // user specified border colour

    float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    // Mipmaps are used to render far away textures with less resolution, they are pre-generated smaller versions of the texture, each 1/2 the size of the previous level
    // Each mipmap can also have the nearest vs interpolation methods used to determine which mipmap to use 
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // Bilinear filter inside mipmap A then Bilinear filter inside mipmap B then Linear blend between mipmap A and B
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Generate a texture object
    unsigned int texture;
    glGenTextures(1, &texture); // args 1: how many textures we want to generate
    glBindTexture(GL_TEXTURE_2D, texture);

    // tells OpenGL how to interpret the textures, args 1: texture target
    // args 2: which axises to configure the texture to, args 3: how the texture should be wrapped
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    // GL_NEAREST: finds the nearest texture pixel from the original texture image, GL_LINEAR interpolates the pixel depending on the surrounding texture pixels in a given coordinate
    // Magnifying and minifying operations (upscaling or downscaling) can use either filitering method
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels; // these variables will be automatically filled by stb_image.h (nrChannels = # of colour channels)
    unsigned char* data = stbi_load("wall.jpg", &width, &height, &nrChannels, 0);

    if (data) {
        // args 1: texture target, args 2: mipmap level for the texture, args 3: format to store the texture, args 4/5 = width and height (already set from stb_image.h)
        // args 6: border, must be 0 (legacy stuff), args 7/8 = format and data type of the source image, args 9 = actual image data
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data); // frees up memory allocated when loading the image in stbi_load()

    // Make a Vertex Array object
    unsigned int VAO; // Create the object
    glGenVertexArrays(1, &VAO); // Store the reference to the Object as an ID
    glBindVertexArray(VAO); // Binds the Object as the current active object in the OpenGL context

    // Make a Elemenet buffer object
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // EBO = GL_ELEMENT_ARRAYBUFFER type
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // store the data of indices into the ebo


    // Make a VBO (Vertex Buffer Object) to send all the vertex data at once
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // VBO buffer type is an array buffer (binds the array data to the GL_ARRAY_BUFFER target)
    // Copies the vertex data in to the current bound buffer, 
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // GL_ARRAY_BUFFER = target buffer, sizeof(vertices) = size of the data, vertices = data to transfer, GL_STATIC_DRAW = how to use the data 
    // GL_STREAM_DRAW = data is set once and used a few times, GL_STATIC_DRAW = data is set once and used many times, GL_DYNAMIC_DRAW = data is changed alot and used many times

    // arg 1 = location/index in the vertex shader (layout (location = 0)), arg 2 = number of components per vertex attribute (vec3 in this case), arg 3 = data type of each component, 
    // arg 4 = should the input be normalized (turned to 0,1 or -1,1 for ints, not needed for floats), arg 5 = stride (the byte offset between vector attributes, works as 0 for tightly packed attributes),
    // arg 6 = pointer = offset in the VBO where this attribute starts (0 for us, requires a void cast for the pointer)

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // tells OpenGL how to interpret the VBO data
    glEnableVertexAttribArray(0); // Tells the VAO that attribute pointer 0 is enabled

    // Colour attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Vertex Coordinates attributes
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Write the vector shader (default: 16 4-component vertex attributes available)
    // vecn: the default vector of n floats. 
    // bvecn : a vector of n booleans. 
    // ivecn : a vector of n integers. 
    // uvecn : a vector of n unsigned integers. 
    // dvecn : a vector of n double components
    //  rgba for colors or stpq for texture coordinates, swizzling (vec2 someVec; vec4 differentVec = someVec.xyxx;)
    
    // Use the .vs and .fs extensions for the vector shader and fragment shader files
    Shader shader("vShader.vs", "fShader.fs");
    

    // Set viewport and resize callback
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        

        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); // Wireframe mode
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Default

        float red = 121.0 / 255.0;
        float green = 175.0 / 255.0;
        float blue =  199.0 / 255.0;

        // Clear the screen with a specified color
        glClearColor(red, green, blue, 1.0f); // State-setting function
        glClear(GL_COLOR_BUFFER_BIT); // State-using function

        glBindTexture(GL_TEXTURE_2D, texture);

        // Draw Polygon 
        shader.use();
        //float timeValue = glfwGetTime();
        //float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        //int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColour"); // gets the location of the uniform in the shader
        //glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f); // Can be different suffixes for function overloading f: floats, i: ints, ui: unsigned ints

        glBindVertexArray(VAO); // Tells OpenGL which vertex data and attribute setup to use
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // Draw the elements, draw 6 vertices,indices are of type unsigned int, EBO has an offset of 0

        processInput(window);

        // Swap buffers and poll for events
        glfwSwapBuffers(window); // Puts the drawn screen onto the main screen (back buffer > front buffer)
        glfwPollEvents(); // Checks for window events / user input
    }
    
    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}