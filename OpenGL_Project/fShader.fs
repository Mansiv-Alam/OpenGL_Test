#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

struct Material {
vec3 ambient;
vec3 diffuse;
vec3 specular;
float shininess;
};

struct Light {
vec3 position;
vec3 ambient;
vec3 diffuse;
vec3 specular;
};

uniform Material material;  
uniform Light light;

uniform vec3 viewPos;

void main()
{
    //FragColor = mix (texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2f); // 0.2 = 20% linear interpolation
    vec3 ambient = material.ambient * light.ambient;

    // Calculate the normalized vectors
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0); // Calculate the angle using the dot product, max because >90 degrees diff = negative 
    vec3 diffuse = (diff * material.diffuse) * light.diffuse;

    // Calculate specular lighting
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm); // the lightDirection vector is pointing TOWARDS the light source but reflect expects it to point FROM the light source
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = (material.specular * spec) * light.specular;

    vec3 result = (ambient + diff + specular); // Phong shading
    FragColor = vec4(result, 1.0f);
}