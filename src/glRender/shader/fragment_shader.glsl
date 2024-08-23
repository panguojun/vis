#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D texture1;

void main()
{
    vec4 color = texture(texture1, TexCoords);
    // 这里可以添加模糊计算逻辑
    FragColor = color; // 目前只是返回原色
}