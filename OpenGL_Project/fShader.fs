#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightPos;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 viewPos;

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

    // Calculate specular lighting
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm); // the lightDirection vector is pointing TOWARDS the light source but reflect expects it to point FROM the light source
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diff + specular) * objectColor; // Phong shading
    FragColor = vec4(result, 1.0f);

}