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

float checkDraw(float val, float dist, float dotprod, float upperlimit) {
	// Check whether or not a dark pixel should be drawn based on diffuse and position.
	if (mod(val, dist) == 0.0) {
		if (dotprod <= upperlimit) {
			return 1.0;
		}
	}
	return 0.0;
}

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
  float dist = 7.0; // Distance between lines
  vec2 pixel = floor(vec2(gl_FragCoord.xy));
  float a = 0.0; // Scalar for ambient colour
  // Stage 1, lightest shading
  float val = pixel.x - pixel.y; // Diagonal up-right, ( / )
  a = max(a, checkDraw(val, dist, dotprod, 0.8));
  // Stage 2, second tier shading
  val = pixel.x + pixel.y + 1.0; // Diagonal up-left, ( \ ), with offset
  a = max(a, checkDraw(val, dist, dotprod, 0.6));
  // Stage 3, dark shading
  val = pixel.x + 2.0; // Vertical line, ( | ), with offset
  a = max(a, checkDraw(val, dist, dotprod, 0.3));
  // Stage 4, darker shading
  val = pixel.x + 2.0*pixel.y + 3.0; // Angled line, with offset
  a = max(a, checkDraw(val, dist, dotprod, 0.15));
  // Stage 5, darkest shading
  val = pixel.y + 4.0; // Horizontal line, ( - ), with offset
  a = max(a, checkDraw(val, dist, dotprod, 0.05));
  
  gl_FragColor = vec4((ambientColor * a) + (diffuseColor * (1.0 - a)), 1.0);
}
