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

// HINT: Use the built-in variable gl_FragCoord to get the screen-space coordinates

void main() {
  // Your solution should go here.
  // Only the background color calculations have been provided as an example.
  // gl_FragColor = vec4(diffuseColor * Kd, 1.0);
  
  // Halftone requires a grid of dots coloured by the ambient colour, which are
  // decreased in size by the dot product for the diffuse calculation.
  
  // Light values
  vec3 ambient, diffuse; 
  vec3 norm = normalize(normalInterp);
  vec3 lightdir = normalize(vec3(lightPos - vertPos));

  // Ambient Lighting
  ambient = ambientColor * Ka;
  
  // Diffuse Lighting
  float dotprod = max(dot(lightdir,norm), 0.0);
  diffuse = diffuseColor * Kd * dotprod;
  
  // No specular lighting
  
  // Halftone specific grid creation
  // Get center pixel coordinate for circle
  vec2 pixel = floor(vec2(gl_FragCoord.xy));
  float thickness = 10.0; // Thickness of circles
  pixel = mod(pixel, vec2(thickness));
  // 
  float b = thickness / 2.0;
  float a = distance(pixel, vec2(b)) / (thickness * 0.65); // Larger a value = more diffuse colour
  if (a < 1.0 && a >= 0.03) { // Only modifying values >= 0.03 means small circles will still appear in bright areas
	  a += diffuse.r + diffuse.g + diffuse.b; // Scale circle size by diffuse color
  }
  // Exagerate circles for definition, clamp between 0 and 1 to avoid colour bleed
  float circles = clamp(pow(a,5.0), 0.0, 1.0); // Without clamping, render looks very strange
  
  // Base colour is purposely set to be the ambient colour. I wanted to be able to have
  // different coloured circles.
  gl_FragColor = vec4((ambient * (1.0 - circles)) + (diffuseColor * circles), 1.0);
}