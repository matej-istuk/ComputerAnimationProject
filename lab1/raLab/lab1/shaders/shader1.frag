#version 330 core
out vec4 FragColor;

in vec3 position;
in vec3 normal;
in vec2 uvCoord;

uniform vec4 eye_pos;
uniform vec4 light_pos;
uniform vec3 light_ambient;
uniform vec3 light_diffuse;
uniform vec3 light_specular;
uniform vec3 material_ambient;
uniform vec3 material_diffuse;
uniform vec3 material_specular;
uniform float material_reflective;

uniform sampler2D ourTexture;

float calculate_component_intensity(float lightIA, float lightID, float lightIS, float matAC, float matDC, float matSC, float matR, float nlAngleCos, float vrAnglecos) {
    return lightIA * matAC + lightID * matDC * nlAngleCos + lightIS * matSC * pow(vrAnglecos, matR);
}
void main()
{
    vec3 lightDir = normalize(light_pos.xyz / light_pos.w - position);
    vec3 viewDir = normalize(eye_pos.xyz / eye_pos.w - position);
    vec3 reflection = normalize(-lightDir - 2 * dot(-lightDir, normal) * normal);
    float nlAngleCos = max(dot(normal, lightDir), 0);
    float vrAngleCos = max(dot(viewDir, reflection), 0);

    vec4 textureDiffuse = texture(ourTexture, uvCoord);
    vec3 color = vec3(
        calculate_component_intensity(light_ambient.r, light_diffuse.r, light_specular.r, material_ambient.r, textureDiffuse.r, material_specular.r, material_reflective, nlAngleCos, vrAngleCos),
        calculate_component_intensity(light_ambient.g, light_diffuse.g, light_specular.g, material_ambient.g, textureDiffuse.g, material_specular.g, material_reflective, nlAngleCos, vrAngleCos),
        calculate_component_intensity(light_ambient.b, light_diffuse.b, light_specular.b, material_ambient.b, textureDiffuse.b, material_specular.b, material_reflective, nlAngleCos, vrAngleCos)
    );
    FragColor = vec4(color, 0.0);  
} 