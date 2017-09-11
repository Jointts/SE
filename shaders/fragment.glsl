#version 410

in vec2 TexCoords;
in vec3 NormalCoords;
in vec3 FragPos;
in vec3 VertexPos;

out vec4 color;

uniform float ambientStrength;
uniform vec3 ambientColor;
uniform vec3 lightColor;
uniform vec3 lightDirection;
uniform float lightStrength;
uniform sampler2D diffuse1;
uniform bool hit;

void applyAmbient(inout vec4 color, float ambientStrength, vec3 ambientColor);
void applyDirectionalLight(inout vec4 color, vec3 normalCoord, vec3 lightColor, vec3 lightDirection, float lightStrength);

void main()
{
    if(hit){
        vec3 viewDirection =
                       normalize(FragPos - vec3(VertexPos));

        if (dot(viewDirection, NormalCoords)
                       < mix(2.0, 3.0,
                       max(0.0, dot(NormalCoords, lightDirection))))
                    {
                       color = vec4(vec3(lightColor) * vec3(0.5, 0.5, 0.5), 1.0);
                    }
    }else{
        vec4 object_color = vec4(texture(diffuse1, TexCoords));
        applyAmbient(color, ambientStrength, ambientColor);
        applyDirectionalLight(color, NormalCoords, lightColor, lightDirection, lightStrength);
        color *= object_color;
    }
}

void applyAmbient(inout vec4 color, float ambientStrength, vec3 ambientColor){
    color += vec4(0.4, 0.4, 0.4, 1.0);
}

void applyDirectionalLight(inout vec4 color, vec3 normalCoord, vec3 lightColor, vec3 lightDirection, float lightStrength){
    vec3 norm = normalize(normalCoord);
    vec3 lightDir = normalize(lightDirection);
    float diff = max(dot(norm, lightDir), 0.0);
    color += vec4((diff * lightColor) * lightStrength, 1.0);
}