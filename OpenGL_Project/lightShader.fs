#version 330 core
out vec4 FragColor;

void main()
{
    //FragColor = mix (texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2f); // 0.2 = 20% linear interpolation
    FragColor = vec4(1.0f);
}