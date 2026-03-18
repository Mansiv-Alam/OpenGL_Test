#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

struct Material {

    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 position; 
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    float cutOff;
    float outerCutOff;
};

uniform Material material;  
uniform Light light;
uniform vec3 viewPos;

void main()
{
    //FragColor = mix (texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2f); // 0.2 = 20% linear interpolation
    vec3 ambient = vec3(texture(material.diffuse, TexCoords)) * light.ambient;

    // Calculate the normalized vectors
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0); // Calculate the angle using the dot product, max because >90 degrees diff = negative 
    vec3 diffuse = (diff * vec3(texture(material.diffuse, TexCoords))) * light.diffuse;

    // Calculate specular lighting
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm); // the lightDirection vector is pointing TOWARDS the light source but reflect expects it to point FROM the light source
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = (vec3(texture(material.specular, TexCoords)) * spec) * light.specular;

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    diffuse *= intensity;
    specular *= intensity;

    // Light attenuation
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    //ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    vec3 result = (ambient + diffuse + specular); // Phong shading
    FragColor = vec4(result, 1.0f);
}