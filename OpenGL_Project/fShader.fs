#version 330 core
out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
    //FragColor = mix (texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2f); // 0.2 = 20% linear interpolation
    FragColor = vec4(lightColor * objectColor, 1.0f);
}