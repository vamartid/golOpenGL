#version 330 core
in vec2 TexCoords;
out vec4 FragColor;
uniform sampler2D textTexture;
uniform vec3 textColor;
void main()
{
    vec4 sampled = texture(textTexture, TexCoords);
    // Discard fully transparent
    if (sampled.a < 0.1)
        discard;
    FragColor = vec4(textColor, sampled.a);
}