#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightPos;
uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
    //FragColor = mix (texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2f); // 0.2 = 20% linear interpolation
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // Calculate the normalized vectors
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0); // Calculate the angle using the dot product, max because >90 degrees diff = negative 
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diff) * objectColor; // Phong shading
    FragColor = vec4(result, 1.0f);

}