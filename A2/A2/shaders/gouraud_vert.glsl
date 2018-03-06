attribute vec3 position; // Given vertex position in object space
attribute vec3 normal; // Given vertex normal in object space
attribute vec3 worldPosition; // Given vertex position in world space

uniform mat4 projection, modelview, normalMat; // Given scene transformation matrices
uniform vec3 eyePos;	// Given position of the camera/eye/viewer

// These will be given to the fragment shader and interpolated automatically
varying vec3 normalInterp; // Normal
varying vec3 vertPos; // Vertex position
varying vec3 viewVec; // Vector from the eye to the vertex
varying vec4 color;

uniform float Ka;   // Ambient reflection coefficient
uniform float Kd;   // Diffuse reflection coefficient
uniform float Ks;   // Specular reflection coefficient
uniform float shininessVal; // Shininess

// Material color
uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform vec3 lightPos; // Light position in camera space


void main(){
  // Your solution should go here.
  // Only the ambient colour calculations have been provided as an example.
  vec4 vertPos4 = modelview * vec4(position, 1.0);
  gl_Position = projection * vertPos4;
  
  vec3 ambient, diffuse, specular;
  vec3 norm = normalize(mat3(normalMat) * normal);
  vec3 lightdir = normalize(vec3(lightPos - vertPos4.xyz));
  vec3 view = normalize(-vertPos4.xyz);
  vec3 reflec = reflect(-lightdir, norm);
  //color = vec4(ambientColor, Ka); // Commented out to allow for simpler code
  
  // Ambient Lighting
  ambient = ambientColor * Ka;
  
  // Diffuse Lighting
  float dotprod = max(dot(lightdir, norm), 0.0);
  diffuse = diffuseColor * Kd * dotprod;
  
  // Specular Lighting
  specular = specularColor * Ks * pow(max(dot(reflec, view), 0.0), shininessVal);
  
  color = vec4(ambient + diffuse + specular, 1.0);
}