#version 330 core

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform vec3 viewPos;

out vec4 FragColor;

void main()
{   
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 lightDir = normalize(vec3(2.0, 0.0, 4.0) - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64.0);

    vec3 ambient = vec3(0.2, 0.2, 0.2) * vec3(texture(texture_diffuse1, TexCoords));
    vec3 diffuse = vec3(0.2, 0.2, 0.2) * diff * vec3(texture(texture_diffuse1, TexCoords));
    vec3 specular = vec3(1.0, 1.0, 1.0) * spec * vec3(texture(texture_specular1, TexCoords));

    vec3 light = ambient + diffuse + specular;
    FragColor = vec4(light, 1.0);
}