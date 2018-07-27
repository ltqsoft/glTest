#version 330 core
layout(location=0) out vec4 FragColor;
layout(location=1) out vec4 BrightColor;

in vec2 TexCoords;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float intensity;

void main()
{    
    FragColor = texture(texture1, TexCoords);

	///vec2 briCol2 = texture(texture1, TexCoords).rg * max(intensity, 0.0f);
	///BrightColor = vec4(briCol2, 1.0f, 1.0f);
	
	///vec3 briCol3 = texture(texture1, TexCoords).rgb / 3.0f;
	vec3 briCol3 = texture(texture1, TexCoords).rgb * max(intensity, 0.0f);
	BrightColor = vec4(briCol3, 1.0);
}
