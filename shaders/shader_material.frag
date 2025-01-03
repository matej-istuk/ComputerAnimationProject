#version 330 core
out vec4 FragColor;

in vec3 position;
in vec3 normal;

const int maxLights = 8;

uniform vec4 eye_pos;
uniform int lights_count;
uniform vec4 lights_pos[maxLights];
uniform vec3 lights_ambient[maxLights];
uniform vec3 lights_diffuse[maxLights];
uniform vec3 lights_specular[maxLights];
uniform vec3 material_ambient;
uniform vec3 material_diffuse;
uniform vec3 material_specular;
uniform float material_reflective;

float calculate_component_intensity(float lightIA, float lightID, float lightIS, float matAC, float matDC, float matSC, float matR, float nlAngleCos, float vrAnglecos) {
    return lightIA * matAC + lightID * matDC * nlAngleCos + lightIS * matSC * pow(vrAnglecos, matR);
}

vec3 calculate_color(int i) {
    vec3 lightDir = normalize(lights_pos[i].xyz / lights_pos[i].w - position);
    vec3 viewDir = normalize(eye_pos.xyz / eye_pos.w - position);
    vec3 reflection = normalize(-lightDir - 2 * dot(-lightDir, normal) * normal);
    float nlAngleCos = max(dot(normal, lightDir), 0);
    float vrAngleCos = max(dot(viewDir, reflection), 0);


    return vec3(
        calculate_component_intensity(lights_ambient[i].r, lights_diffuse[i].r, lights_specular[i].r, material_ambient.r, material_diffuse.r, material_specular.r, material_reflective, nlAngleCos, vrAngleCos),
        calculate_component_intensity(lights_ambient[i].g, lights_diffuse[i].g, lights_specular[i].g, material_ambient.g, material_diffuse.g, material_specular.g, material_reflective, nlAngleCos, vrAngleCos),
        calculate_component_intensity(lights_ambient[i].b, lights_diffuse[i].b, lights_specular[i].b, material_ambient.b, material_diffuse.b, material_specular.b, material_reflective, nlAngleCos, vrAngleCos)
    );
}
void main()
{
    vec3 color = vec3(0);
    for (int i = 0; i < lights_count && i < maxLights; i++) {
        color += calculate_color(i);
    }
    color.r = min(color.r, 1);
    color.g = min(color.g, 1);
    color.g = min(color.g, 1);

    FragColor = vec4(color, 0.0);
} 