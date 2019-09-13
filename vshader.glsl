#version 150

in vec4 vPosition;
in vec4 vNormal;
out vec4 color;

uniform mat4 model_view_matrix;
uniform mat4 projection_matrix;
uniform mat4 ctm;

uniform vec4 light_position;
uniform int isShadow;

uniform vec4 AmbientProduct;
uniform vec4 DiffuseProduct;
uniform vec4 SpecularProduct;

uniform float shininess;

uniform float attenuation_constant;
uniform float attenuation_linear;
uniform float attenuation_quadratic;

void main()
{
  if(isShadow == 1)
  {
    color = vec4(0, 0, 0, 1);

    vec4 ctmvPosition = ctm * vPosition;

    float vx = ctmvPosition.x;
    float vy = ctmvPosition.y;
    float vz = ctmvPosition.z;

    float lx = light_position.x;
    float ly = light_position.y;
    float lz = light_position.z;

    float x = lx - ly * (lx - vx) / (ly - vy);
    float y = 0.01;
    float z = lz - ly * (lz - vz) / (ly - vy);
    gl_Position = projection_matrix * model_view_matrix * vec4(x, y, z, 1);
  }
  else
  {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;

    vec4 vPos = ctm * vPosition;

    ambient = AmbientProduct;
    vec4 N = normalize(model_view_matrix * vNormal);
    vec4 L_temp = model_view_matrix * (light_position - vPos);
    vec4 L = normalize(L_temp);
    diffuse = max(dot(L,N), 0.0) * DiffuseProduct;
    vec4 EyePoint = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 V = normalize(EyePoint - (model_view_matrix * vPos));
    vec4 H = normalize(L + V);
    specular = pow(max(dot(N, H), 0.0), shininess) * SpecularProduct;
    float distance = length(L_temp);
    float attenuation = 1/(attenuation_constant + (attenuation_linear * distance) + (attenuation_quadratic * distance * distance));

    color = ambient + (attenuation * (diffuse + specular));

    gl_Position = projection_matrix * model_view_matrix * ctm * vPosition;
  }
}