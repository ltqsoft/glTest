#version 420 core
out vec4 FragColor;

in vec2 TexCoords;

layout(binding = 0) uniform sampler2D screenTexture;
///uniform sampler2D screenTexture2;

void main()
{
    vec3 col = texture(screenTexture, TexCoords).rgb;
	///vec3 col = texture(screenTexture2, TexCoords).rgb;
    FragColor = vec4(col, 1.0);
}
