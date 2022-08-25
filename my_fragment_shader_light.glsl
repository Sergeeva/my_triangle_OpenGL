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
uniform vec3 camPos;

void main(){
     //FragColor = texture(Tex1, TexCoord);//*vec4(ourColor, 0.0f);
     //FragColor = vec4(ourColor, 0.0f);

     // Ambient (фоновое)
    float ambientStrength = 0.2f;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse (направленное)
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    //Specular (зеркальные блики)
    float specularStrength = 0.5f; 
    vec3 viewDir = normalize(camPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    int spec_power = 16;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), spec_power);
    vec3 specular = specularStrength * spec * lightColor;


    vec3 result = (ambient + diffuse + specular);// * objectColor;
    FragColor = texture(Tex1, TexCoord)*vec4(result, 1.0f);

}
