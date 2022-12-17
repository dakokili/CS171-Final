#version 330 core
out vec4 FragColor;

void main()
{
    vec3 objectColor=vec3(0.6f,0.3f,0.0f);
    FragColor = vec4(objectColor, 1.0);
}