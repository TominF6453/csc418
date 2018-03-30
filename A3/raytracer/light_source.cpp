/***********************************************************
	
	Starter code for Assignment 3

	Implements light_source.h

***********************************************************/

#include <cmath>
#include "light_source.h"

void PointLight::shade(Ray3D& ray) {
	// TODO: implement this function to fill in values for ray.col 
	// using phong shading.  Make sure your vectors are normalized, and
	// clamp colour values to 1.0.
	//
	// It is assumed at this point that the intersection information in ray 
	// is available.  So be sure that traverseScene() is called on the ray 
	// before this function.  
    Vector3D norm = ray.intersection.normal;
    
    Vector3D rayToEyeDir = -ray.dir;
    
    //get light dir, from PointLight pos to intersection pt
    Vector3D lightDir = pos - ray.intersection.point;
    lightDir.normalize(); //normalize the light dir
    
    //calculate reflection dir
    Vector3D reflec = -lightDir.dot(normal);
    reflec.normalize;
    
    Material *intersMat = ray.intersection.mat;
    Color amb = (*intersMat).ambient * col_ambient;
    Color diff = (*intersMat).diffuse * (fmax(0.0, normal.dot(lightDir))) * col_diffuse;
    Color spec = (*intersMat).specular * (fmax(0.0, pow(rayToEyeDir.dot(reflec), (*intersMat).specular_exp))) * col_specular;
    
    ray.col = amb + diff + spec;
    ray.col.clamp();
}

