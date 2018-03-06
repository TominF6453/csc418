// Fragment shader template for the bonus question

precision mediump float; // It is required to set a floating point precision in all fragment shaders.

// Interpolated values from vertex shader
// NOTE: You may need to edit this section to add additional variables
varying vec3 normalInterp; // Normal
varying vec3 vertPos; // Vertex position
varying vec3 viewVec; // Interpolated view vector

// uniform values remain the same across the scene
// NOTE: You may need to edit this section to add additional variables
uniform float Ka;   // Ambient reflection coefficient
uniform float Kd;   // Diffuse reflection coefficient
uniform float Ks;   // Specular reflection coefficient
uniform float shininessVal; // Shininess

// Material color
uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;

uniform vec3 lightPos; // Light position in camera space

uniform sampler2D uSampler;	// 2D sampler for the earth texture

void main() {
  // Your solution should go here.
  // Only the ambient colour calculations have been provided as an example.
  //gl_FragColor = vec4(ambientColor, 1.0);
  
  // Cross-hatching should be similar to halftone, with different calculations for patterns and thickness.
  
  // Light values
  vec3 ambient, diffuse; 
  vec3 norm = normalize(normalInterp);
  vec3 lightdir = normalize(vec3(lightPos - vertPos));

  // Ambient Lighting (the line colour)
  ambient = ambientColor * Ka;
  
  // Diffuse Lighting (the bright colour)
  float dotprod = max(dot(lightdir,norm), 0.0);
  diffuse = diffuseColor * Kd * dotprod;
  
  // No specular lighting
  
  // Cross-hatch specific 
}
