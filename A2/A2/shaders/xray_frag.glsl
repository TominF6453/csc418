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
  // Your solution should go here.
  
  // Xray shading, ignore light source. The lower the dot product is from the eye, 
  // the more visible the diffuse.
  
  // Light values
  vec3 xray; 
  vec3 norm = normalize(normalInterp);
  vec3 view = normalize(-vertPos);
  
  // Xray Lighting
  xray = diffuseColor * Kd * (1.0 - abs(dot(view, norm)));
  
  gl_FragColor = vec4(xray, 1.0);
}
