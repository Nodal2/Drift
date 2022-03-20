#version 330 core

in vec3 v_position;
in vec3 v_normal;
in vec2 v_textureCoords;

out vec4 f_color;

struct Material
{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
};

struct DirectionalLight
{
    vec3 direction;

    // phong
    vec3 ambiant;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    vec3 position;

    // phong
    vec3 ambiant;
    vec3 diffuse;
    vec3 specular;

    // attenuation
    float constant;
    float linear;
    float quadratic;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;
    float angle;

    // phong
    vec3 ambiant;
    vec3 diffuse;
    vec3 specular;

    // attenuation
    float constant;
    float linear;
    float quadratic;

};

#define NB_POINT_LIGHTS 4

uniform Material u_material;
uniform vec3 u_cameraPosition;

// lights
uniform DirectionalLight u_directionalLight;
uniform PointLight u_pointLights[NB_POINT_LIGHTS];
uniform SpotLight u_spotLight;

vec3 computeDirectionalLight(DirectionalLight light, vec3 normal, vec3 cameraDirection)
{  

    vec3 lightDirection = normalize(-light.direction);
    vec3 ambiantColor = light.ambiant * vec3(texture(u_material.texture_diffuse1, v_textureCoords));

    // diffuse
    float diffuse = max(dot(normal, lightDirection), 0.0);
    vec3 diffuseColor = light.diffuse * diffuse * vec3(texture(u_material.texture_diffuse1, v_textureCoords));

    // specular
    vec3 reflectedLightDirection = reflect(-lightDirection, normal);
    float specular = pow(max(dot(reflectedLightDirection, cameraDirection), 0.0), u_material.shininess);
    vec3 specularColor = light.specular * specular * vec3(texture(u_material.texture_specular1, v_textureCoords));

    return ambiantColor + diffuseColor + specularColor;
}

vec3 computePointLight(PointLight light, vec3 normal, vec3 cameraDirection)
{
    vec3 lightDirection = normalize(light.position - v_position);
    vec3 ambiantColor = light.ambiant * vec3(texture(u_material.texture_diffuse1, v_textureCoords));

    // diffuse
    float diffuse = max(dot(lightDirection, normal), 0.0);
    vec3 diffuseColor = light.diffuse * diffuse * vec3(texture(u_material.texture_diffuse1, v_textureCoords));

    // specular
    vec3 reflectedLightDirection = reflect(-lightDirection, normal);
    float specular = pow(max(dot(reflectedLightDirection, cameraDirection), 0.0), u_material.shininess);
    vec3 specularColor = light.specular * specular * vec3(texture(u_material.texture_specular1, v_textureCoords));

    // attenuation
    float distance = length(light.position - v_position);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    return (ambiantColor + diffuseColor + specularColor) * attenuation;
}

vec3 computeSpotLight(SpotLight light, vec3 normal, vec3 cameraDirection)
{
    vec3 lightDirection = normalize(light.position - v_position);
    float angle = dot(lightDirection, normalize(-light.direction));

    vec3 ambiantColor = light.ambiant * vec3(texture(u_material.texture_diffuse1, v_textureCoords));
    

    if(angle >= light.angle)
    {
        float theta = dot(lightDirection, normalize(-light.direction));
        float epsilon = light.angle - cos(0.55);
        float intensity = clamp((theta - light.angle) / epsilon, 0.0, 1.0);

        // diffuse
        float diffuse = max(dot(lightDirection, normal), 0.0);
        vec3 diffuseColor = light.diffuse * diffuse * intensity * vec3(texture(u_material.texture_diffuse1, v_textureCoords));

        // specular
        vec3 reflectedLightDirection = reflect(-lightDirection, normal);
        float specular = pow(max(dot(cameraDirection, reflectedLightDirection), 0.0), u_material.shininess);
        vec3 specularColor = light.specular * specular * intensity * vec3(texture(u_material.texture_specular1, v_textureCoords));

        // attenuation
        float distance = length(light.position - v_position);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

        return (ambiantColor + diffuseColor + specularColor) * attenuation;
    }
    return ambiantColor;
}

void main()
{
    vec3 normal = normalize(v_normal);
    vec3 cameraDirection = normalize(u_cameraPosition - v_position);
    vec3 color;

    // compute directional light
    color = computeDirectionalLight(u_directionalLight, normal, cameraDirection);
    for(int i = 0; i < NB_POINT_LIGHTS; ++i) {
        //color += computePointLight(u_pointLights[i], normal, cameraDirection);
    }
        
    // compute spot light
    color += computeSpotLight(u_spotLight, normal, cameraDirection);

    f_color = vec4(color, 1.0);
}