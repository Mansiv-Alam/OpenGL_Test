#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <windows.h>
#include <iostream>
#include <Shader.h>
#include <stb_image.h>
#include <math.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Timing

float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

// Camera 

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float yaw = -90.0f, pitch = 0.0f;
float lastX = SCR_WIDTH/2.0f, lastY = SCR_HEIGHT / 2.0f;
float fov = 45.0f;

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

    float cameraSpeed = 5.0f * deltaTime;
    // WASD camera movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraPos += cameraSpeed * cameraFront;;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraPos -= cameraSpeed * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        cameraPos += glm::vec3(0.0f, 1.0f, 0.0f) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        cameraPos += glm::vec3(0.0f, -1.0f, 0.0f) * cameraSpeed;;
    }
}

// Mouse Input
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    // No jitter based off where the mouse cursor came from
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    // Find the offset from the last cursor position
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    // change the offset using a sensitivity
    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    // Change the pitch and yaw using the offset
    yaw += xoffset;
    pitch += yoffset;

    // Clamp the pitch so the camera doesn't turn upside down (90 degrees = parallel vectors = 0 cross product = broken camera)
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){

    // Change the fov depending on the vertical scroll
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}

int main()
{
    HWND consoleWindow = GetConsoleWindow();
    //ShowWindow(consoleWindow, SW_HIDE); // hides the console

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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Testing", NULL, NULL);
    if (window == NULL)
    {
        //std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    
    // 
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        //std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    // Making verticies (z coordinate is 0)
    float vertices[] = {
        // Position             // Normal Vectors // Textures Coordinates
        -0.5f, -0.5f, -0.5f,    0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,     0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f,      0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f,      0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f,     0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f,     0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f,      0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f,       0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f,       0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f,      0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f,     0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f,      -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f,     -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,    -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,    -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f,     -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f,      -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f,       1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f,      1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,     1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,     1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f,      1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f,       1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,     0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f,      0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f,      0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f,     0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, -0.5f,     0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f,      0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f,       0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f,       0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f,      0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f,     0.0f, 1.0f, 0.0f, 0.0f, 1.0f
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
    glm::vec3 cubePositions[] = { // cube positions in the world space
        glm::vec3( 0.0f, 0.0f, 0.0f),
        glm::vec3( 2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f, 2.0f, -2.5f),
        glm::vec3( 1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)
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
    unsigned int texture, texture2;
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
    stbi_set_flip_vertically_on_load(true); // tells stb_image.h to flip the loaded texture on the y axis to correspond to OpenGL coordinate system

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

    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // Wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    // filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load images and create mipmaps 
    data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);


    // Make a Vertex Array object
    unsigned int VAO; // Create the object
    glGenVertexArrays(1, &VAO); // Store the reference to the Object as an ID
    glBindVertexArray(VAO); // Binds the Object as the current active object in the OpenGL context

    // Light VAO
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

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

    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Vertex Coordinates attributes
    //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    //glEnableVertexAttribArray(1);
    
    // Use the .vs and .fs extensions for the vector shader and fragment shader files
    Shader shader("vShader.vs", "fShader.fs");
    Shader lightCubeShader("vShader.vs", "lightShader.fs");
    

    // Set viewport and resize callback
    glViewport(0, 0, 800, 600);

    // Alternative: glUniform1i(glGetUniformLocation(shader.ID, "texture1"), 0);

    // Use glm to translate a vector
    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    glm::mat4 trans = glm::mat4(1.0f); // Initialize identity Matrix
    trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f)); // identity matrix multiplied with the transformation matrix
    vec = trans * vec; // Multiply the vector and tranformation matrix together
    std::cout << vec.x << vec.y << vec.z << std::endl;

    // Rotate and scale the vector 
    //vec = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    trans = glm::mat4(1.0f); // Initialize identity Matrix
    trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0)); // glm::radians converts degrees to radians
    trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5)); // Scales the vector using a uniform scale
    vec = trans * vec; // Multiply the vector and tranformation matrix together
    std::cout << vec.x << vec.y << vec.z << std::endl;

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 1.0f)); // Rotates or changes the position in the world space
    
    //glm::mat4 view = glm::mat4(1.0f);
    //view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f)); // distance from the camera


    // Orthographic projection 
    //glm::mat4 proj = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
    // Perspective projection
    glm::mat4 proj = glm::perspective(glm::radians(fov), (float)SCR_WIDTH/ (float)SCR_HEIGHT, 0.1f, 100.0f); // fov dependent on the user, aspect ratio (width/height), near distance/plane, far distance/plane



    // Colours under different light colours
    glm::vec3 lightColor(0.33f, 0.42f, 0.18f);
    glm::vec3 objectColor(1.0f, 0.5f, 0.31f);

    // Light Source Position
    glm::vec3 lightPos(1.2f, 1.0f, 0.8f);

    
    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); // Wireframe mode
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Default

        float red = 70.0 / 255.0;
        float green = 70.0 / 255.0;
        float blue =  70.0 / 255.0;

        // Clear the screen with a specified color
        glClearColor(red, green, blue, 1.0f); // State-setting function
        glClear(GL_COLOR_BUFFER_BIT); // State-using function

        shader.use();
        shader.setInt("texture2", 1); // use the shader class to set the uniforms 
        shader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        shader.setVec3("lightPos", lightPos);

        /*
        glActiveTexture(GL_TEXTURE0); // Activates the texture unit (useful for multiple textures, 0 on default, minimum of 16 texture units)
        glBindTexture(GL_TEXTURE_2D, texture);

        glActiveTexture(GL_TEXTURE1); // Use the second texture
        glBindTexture(GL_TEXTURE_2D, texture2);
        */
        glEnable(GL_DEPTH_TEST); // enable depth testing (calculates which pixels should be on top depending on the stored z values)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the depth buffer for each render iteration

        // Draw Polygon
        //float timeValue = glfwGetTime();
        //float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        //int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColour"); // gets the location of the uniform in the shader
        //glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f); // Can be different suffixes for function overloading f: floats, i: ints, ui: unsigned ints

        vec = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        trans = glm::mat4(1.0f); 
        trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0)); // Rotates the texture over time using glfwGetTime()
        trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5)); // (scales first then rotates despite writing the rotate code first)
        //vec = trans * vec;

        //model = glm::mat4(1.0f);
        //model = glm::rotate(model, (float)(glfwGetTime() * 0.5f), glm::vec3(0.5f, 1.0f, 1.0f));

        glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget); // Reverse direction (direction from the origin to the camera)

        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection)); // Cross product, points to the positive x axis
        //glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight); // Cross product of the right vector to get the up vector

        glm::mat4 view;
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp); // Creates a view matrix for the camera system

        glm::mat4 proj = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f); // fov dependent on the user, aspect ratio (width/height), near distance/plane, far distance/plane

        glm::mat4 model = glm::mat4(1.0f);
        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("projection", proj);

        glBindVertexArray(lightVAO); // Tells OpenGL which vertex data and attribute setup to use
        glDrawArrays(GL_TRIANGLES, 0, 36);

        lightCubeShader.use();
        lightCubeShader.setMat4("projection", proj);
        lightCubeShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);

        model = glm::scale(model, glm::vec3(0.4f)); // a smaller cube
        lightCubeShader.setMat4("model", model);

        glBindVertexArray(lightVAO); // make a different vao for the lighting cube usually
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // Draw the elements, draw 6 vertices,indices are of type unsigned int, EBO has an offset of 0

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