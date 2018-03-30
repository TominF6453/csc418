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
    
	// Normalized vectors for phong shading (from A2's phong shader)
	Vector3D n = ray.intersection.normal; // norm
	n.normalize();
	Vector3D l = pos-ray.intersection.point; // lightdir
	l.normalize();
	Vector3D v = -ray.dir; // view
	v.normalize();
	Vector3D r = 2.0*(l.dot(n)*n)-l; // reflec
	r.normalize();
	
	// Ambient Lighting
	Color Ia = (*ray.intersection.mat).ambient * col_ambient;
	
	// Diffuse Lighting
	float dotprod = fmax(0.0, l.dot(n));
	Color Id = dotprod * (*ray.intersection.mat).diffuse * col_diffuse;
	
	// Specular Lighting
	dotprod = fmax(0.0, r.dot(v));
	Color Is = pow(dotprod, (*ray.intersection.mat).specular_exp) * (*ray.intersection.mat).specular
					* col_specular;
	
	// Set color
	ray.col = Ia + Id + Is;
	
	// Custom renderings for part A
	// Diffuse and Ambient only 
	//ray.col = Ia + Id;
	// Scene Signature
	//ray.col = (*ray.intersection.mat).diffuse;
	
	// Clamp the color
	ray.col.clamp();
}

