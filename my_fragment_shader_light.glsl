#version 460 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;
in vec3 FragPos; 
in vec3 Normal;

uniform sampler2D Tex1;

uniform vec3 lightPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;

void main(){
     //FragColor = texture(Tex1, TexCoord);//*vec4(ourColor, 0.0f);
     //FragColor = vec4(ourColor, 0.0f);

     // Ambient
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse);// * objectColor;
    FragColor = texture(Tex1, TexCoord)*vec4(result, 1.0f);

}
