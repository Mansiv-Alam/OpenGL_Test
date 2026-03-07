#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void math() {

    // Write the vector shader (default: 16 4-component vertex attributes available)
    // vecn: the default vector of n floats. 
    // bvecn : a vector of n booleans. 
    // ivecn : a vector of n integers. 
    // uvecn : a vector of n unsigned integers. 
    // dvecn : a vector of n double components
    //  rgba for colors or stpq for texture coordinates, swizzling (vec2 someVec; vec4 differentVec = someVec.xyxx;)
    /*const char* vertexShaderSource = "#version 330 core\n" // Use GLSL 3.3 (the shader language for OpenGL 3.3)
        "layout (location = 0) in vec3 aPos;\n" // declare an input variable as aPos which is a vec3 or 3d vector type  
        "layout (location = 1) in vec3 aColour;\n"
        "out vec3 ourColour;"
        "void main()\n"
        "{\n"
        " gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n " // Tells the gpu where to place the vector onto the screen, must cast to 4d vector, gl_Position is required in every vector shader
        " ourColour = aColour;\n"
        "}\0";*/

    int vec3One[] = { 1, 2, 3 };
    int vec3Two[] = { 3, 4, 5 };
    int vec3Result[3];
    vec3Result[0] = vec3One[0] + vec3Two[0];
    vec3Result[1] = vec3One[1] + vec3Two[1];
    vec3Result[2] = vec3One[2] + vec3Two[2];
    //std::cout << vec3Result[0] << "," << vec3Result[1] << "," << vec3Result[2];

    vec3Result[0] = vec3One[0] - vec3Two[0];
    vec3Result[1] = vec3One[1] - vec3Two[1];
    vec3Result[2] = vec3One[2] - vec3Two[2];
    //std::cout << vec3Result[0] << "," << vec3Result[1] << "," << vec3Result[2];

    // Dot Product Calculations
    float vec3NormA[] = { 0.6, -0.8, 0 }; // Normalized Vectors
    float vec3NormB[] = { 0, 1, 0 };
    float vec3Resultf[3], normDotProd;

    // Component-wise multiplication
    vec3Resultf[0] = vec3NormA[0] * vec3NormB[0];
    vec3Resultf[1] = vec3NormA[1] * vec3NormB[1];
    vec3Resultf[2] = vec3NormA[2] * vec3NormB[2];

    normDotProd = vec3Resultf[0] + vec3Resultf[1] + vec3Resultf[2];

    std::cout << normDotProd << std::endl;
    std::cout << (acos(normDotProd) * 180 / (acos(-1.0))) << std::endl; // Finds the angle between the two vectors and outputs them in degrees, (acos(-1.0)) = pi

    // Cross Product
    vec3One[0] = 1; vec3One[1] = 2; vec3One[2] = 3;
    vec3Two[0] = 4; vec3Two[1] = 5; vec3Two[2] = 6;

    vec3Result[0] = vec3One[1] * vec3Two[2] - vec3One[2] * vec3Two[1]; // Vy*Wz ​− Vz*Wy
    vec3Result[1] = vec3One[2] * vec3Two[0] - vec3One[0] * vec3Two[2]; // Vz*Wx ​− Vx*Wz
    vec3Result[2] = vec3One[0] * vec3Two[1] - vec3One[1] * vec3Two[0]; // Vx*Wy ​− Vy*Wx

    float crossProd = sqrtf(powf(vec3Result[0], 2) + powf(vec3Result[1], 2) + powf(vec3Result[2], 2));
    std::cout << vec3Result[0] << "," << vec3Result[1] << "," << vec3Result[2] << std::endl;
    std::cout << crossProd << std::endl;

    // Matrix Mulitplication
    int matrixA[2][2] = { {1, 2}, { 3, 4} };
    int matrixB[2][2] = { {5, 6}, { 7, 8} };
    int result[2][2];

    result[0][0] = matrixA[0][0] * matrixB[0][0] + matrixA[0][1] * matrixB[1][0];
    result[0][1] = matrixA[0][0] * matrixB[0][1] + matrixA[0][1] * matrixB[1][1];
    result[1][0] = matrixA[1][0] * matrixB[0][0] + matrixA[1][1] * matrixB[1][0];
    result[1][1] = matrixA[1][0] * matrixB[0][1] + matrixA[1][1] * matrixB[1][1];
    std::cout << result[0][0] << "," << result[0][1] << "\n" << result[1][0] << "," << result[1][1] << std::endl;

    // Scaling
    int vec4[] = { 3, 2, 1, 1 }; // w = Homogeneous coordinate, when it is 0 the vector becomes a direction vector (translations are not possible)
    float MatrixScale[4][4] = { {1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {0,0,0,1} }; // identity matrix
    float resultf[4];
    MatrixScale[0][0] = 3; // Non uniform scale (scaling factor is different for each axis)
    MatrixScale[1][1] = 2;
    MatrixScale[2][2] = 2;
    MatrixScale[3][3] = 1;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << MatrixScale[i][j] << ", ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    // Translation
    MatrixScale[0][3] = 2;
    MatrixScale[1][3] = 2;
    MatrixScale[2][3] = 2;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << MatrixScale[i][j] << ", ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    // Rotation Around the X axis (in radians) (90 is the angle you want to rotate the vector by on that axis)
    MatrixScale[0][3] = 0; MatrixScale[1][3] = 0; MatrixScale[2][3] = 0;
    MatrixScale[1][1] = cos(90); MatrixScale[1][2] = -sin(90);
    MatrixScale[2][1] = sin(90); MatrixScale[2][2] = cos(90);
    // Rotation Around the Y axis (in radians)
    MatrixScale[1][1] = 1; MatrixScale[1][2] = 0; MatrixScale[2][1] = 0; // Reset values
    MatrixScale[0][0] = cos(90); MatrixScale[0][2] = sin(90);
    MatrixScale[2][0] = -sin(90); MatrixScale[2][2] = cos(90);
    // Rotation Around the Z axis (in radians)
    MatrixScale[2][0] = 0; MatrixScale[2][2] = 1; MatrixScale[0][0] = 1; MatrixScale[0][2] = 0;// Reset values
    MatrixScale[0][0] = cos(90); MatrixScale[0][1] = -sin(90);
    MatrixScale[1][0] = sin(90); MatrixScale[1][1] = cos(90);
    // Careful of combining the rotation matrices due to Gimbal Locks, use Rodrigues' Rotation Matrix or Quaternions for complete avoidance
    // Also Matrix Multiplication is not commutative, so order matters (multiplying the translation then the scale would also scale the translation vector)
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << MatrixScale[i][j] << ", ";
        }
        std::cout << std::endl;
    }

    // i < N
    for (int i = 0; i < 4; i++) {
        resultf[i] = 0;
        for (int j = 0; j < 4; j++) {
            resultf[i] += MatrixScale[i][j] * vec4[j];
        }
    }
    std::cout << resultf[0] << "," << resultf[1] << "," << resultf[2] << "," << resultf[3] << std::endl;
}