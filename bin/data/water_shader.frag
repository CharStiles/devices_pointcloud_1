#version 150

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;
uniform sampler2D dudvMap;
uniform sampler2D normalMap;
uniform float moveFactor;
uniform vec3 cameraPos;
uniform vec3 lightColor;
uniform float texScale = 10;

in vec2 textureCoords;
in vec4 clipSpace;
in vec3 toCameraVector;
in vec3 fromLightVector;
in vec4 modelPos;
in vec4 fogCol;

out vec4 fragColor;

float waveStrength = 0.004;
const float shineDamper = 60.0;
const float reflectivity = 0.6;

void main()
{
    vec2 ndc = ((clipSpace.xy/clipSpace.w)/2.0) + 0.5; // Nnormalized device coords
    
    vec2 reflectionCoords = ndc;
    vec2 refractionCoords = vec2(ndc.x,-ndc.y);
    //vec2 textureCoords = modelPos.xy;
    // vec2 distort1 = ((texture(dudv,vec2( mod(ndc.x + moveFactor,1.) , ndc.y ) ).xy * 2.)-1.) *waveStrength;
    // vec2 distort2 = ((texture(dudv,vec2( mod(-ndc.x + moveFactor,1.) , mod(ndc.y+ moveFactor,1.) )).xy * 2.)-1.) *waveStrength;
    // vec2 distort = distort1 + distort2;
    
    vec2 modTexCoords = textureCoords * texScale;
    float moveModY = modTexCoords.y + moveFactor;
    float moveModX = modTexCoords.x + moveFactor; // these are to remidy still water at times

    vec2 distortedTexCoords = texture(dudvMap, vec2(moveModX, modTexCoords.y)).rg*0.1;
	distortedTexCoords = modTexCoords + vec2(distortedTexCoords.x, moveModY);
	vec2 distort = (texture(dudvMap, distortedTexCoords).rg * 2.0 - 1.0) * waveStrength;

    reflectionCoords += distort;
    refractionCoords += distort;

    vec4 reflectionColor = texture(reflectionTexture,mod(reflectionCoords,1));
    vec4 refractionColor = texture(refractionTexture,mod(refractionCoords,1));


    vec4 normalColor = texture(normalMap,distortedTexCoords);
    // so the normal off the map is b, r and g are the tex positions in the map i think
    vec3 normal = vec3(normalColor.r *2 -1 , normalColor.b * 3.,normalColor.g*2-1);
    normal = normalize(normal);

    vec3 viewVector = normalize(toCameraVector);
    float refactiveFactor = dot (viewVector,normal);
    refactiveFactor = pow(refactiveFactor,0.5);

    //specular lighting
    vec3 reflectedLight = reflect(normalize(fromLightVector), normal);
	float specular = max(dot(reflectedLight, viewVector), 0.0);
	specular = pow(specular, shineDamper);
	vec3 specularHighlights = lightColor * specular * reflectivity;

    vec4 notBlue =  mix(reflectionColor, refractionColor, refactiveFactor) - (normalColor.bgra*0.05);
	fragColor = (mix(normalColor,notBlue,(0.8 + (0.2*refactiveFactor))) + vec4(specularHighlights,0.0)) * fogCol;
}
