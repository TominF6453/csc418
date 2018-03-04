precision mediump float; // It is required to set a floating point precision in all fragment shaders.

// Interpolated values from vertex shader
varying vec3 normalInterp; // Normal
varying vec3 vertPos; // Vertex position
varying vec3 viewVec; // Interpolated view vector

// uniform values remain the same across the scene
uniform float Ka;   // Ambient reflection coefficient
uniform float Kd;   // Diffuse reflection coefficient
uniform float Ks;   // Specular reflection coefficient
uniform float shininessVal; // Shininess

// Material color
uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;

uniform vec3 lightPos; // Light position in camera space

void main() {
  // Your solution should go here
  // Only the ambient colour calculations have been provided as an example.
  // Cel shading is basically like Phong but with "stages" that are rounded to.
  
  // Light values
  vec3 ambient, diffuse, specular; 
  vec3 norm = normalize(normalInterp);
  vec3 lightdir = normalize(vec3(lightPos - vertPos));
  vec3 view = normalize(-vertPos);
  vec3 reflec = reflect(-lightdir, norm);
  
  // Ambient Lighting
  // Ambient lighting does not get rounded at all, lowest "stage"
  ambient = ambientColor * Ka;
  
  // Diffuse Lighting
  float dotprod = max(dot(lightdir,norm), 0.0);
  // 4 stages for diffuse lighting
  if (dotprod > 0.75) { dotprod = 1.0; } 
  else if (dotprod > 0.5) { dotprod = 0.75; }
  else if (dotprod > 0.25) { dotprod = 0.5; }
  else { dotprod = 0.0; }
  
  diffuse = diffuseColor * Kd * dotprod;
  
  // Specular Lighting
  float dotprod_spec = max(dot(reflec,view),0.0);
  // 2 stages for specular, on or off
  if (dotprod_spec > 0.95) { dotprod_spec = 1.0; }
  else { dotprod_spec = 0.0; }
  
  specular = specularColor * Ks * pow(dotprod_spec, shininessVal);

  gl_FragColor = vec4(ambient + diffuse + specular, 1.0);
}