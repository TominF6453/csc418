/***********************************************************
	
	Starter code for Assignment 3

	Implementations of functions in raytracer.h, 
	and the main function which specifies the scene to be rendered.	

***********************************************************/


#include "raytracer.h"
#include <cmath>
#include <iostream>
#include <cstdlib>

void Raytracer::traverseScene(Scene& scene, Ray3D& ray)  {
	for (size_t i = 0; i < scene.size(); ++i) {
		SceneNode* node = scene[i];

		if (node->obj->intersect(ray, node->worldToModel, node->modelToWorld)) {
			ray.intersection.mat = node->mat;
		}
	}
}

void Raytracer::computeTransforms(Scene& scene) {
	// right now this method might seem redundant. But if you decide to implement 
	// scene graph this is where you would propagate transformations to child nodes
		
	for (size_t i = 0; i < scene.size(); ++i) {
		SceneNode* node = scene[i];

		node->modelToWorld = node->trans;
		node->worldToModel = node->invtrans; 
	}
}

void Raytracer::computeShadow(Ray3D& ray, LightSource* light, Scene& scene) {
	// Helper function to compute shadows
	
	// Get normal colour from ray
	light->shade(ray);
	
	// Possible shadow position, generate ray from intersection point to light
	Point3D origin = ray.intersection.point;
	Vector3D dir = light->get_position() - origin;
	dir.normalize();
	
	// Move origin slightly off intersection such that it doesn't intersect with itself
	Ray3D shadowRay(origin + 0.01*dir,dir); 
	
	// Traverse ray to get any intersections between intersection point and the light source
	traverseScene(scene, shadowRay);
	
	// If ray intersected with object, the point is in shadow, if refractive: no shadow
	if (!shadowRay.intersection.none && !shadowRay.intersection.mat->refractive) {
		ray.col = 0.2*ray.col;
	}
}

void Raytracer::computeShading(Ray3D& ray, LightList& light_list, Scene& scene) {
	Color final_col(0.0,0.0,0.0);
	for (size_t  i = 0; i < light_list.size(); ++i) {
		LightSource* light = light_list[i];
		
		// Each lightSource provides its own shading function.
		// Implement shadows here if needed.
		//light->shade(ray);  OLD
		computeShadow(ray, light, scene);
		final_col = final_col + (1.0f/light_list.size()) * ray.col;
	}
	ray.col = final_col;
}

Color Raytracer::shadeRay(Ray3D& ray, Scene& scene, LightList& light_list, int k_bounce = 0) {
	Color col(0.0, 0.0, 0.0); 
	traverseScene(scene, ray); 

	// Don't bother shading if the ray didn't hit 
	// anything.
	if (!ray.intersection.none) {
		computeShading(ray, light_list, scene); 
		col = ray.col;  
		
		// You'll want to call shadeRay recursively (with a different ray, 
		// of course) here to implement reflection/refraction effects. 
		// Generate new ray and recursively shadeRay up to MAX_BOUNCE
        if(k_bounce < MAX_BOUNCE){
            
            //normal at the intersection
            Vector3D ray_norm = ray.intersection.normal;
            //direction of incidence
            Vector3D I_dir = ray.dir;
            
            //if refractive then calculate the refraction.
            if(ray.intersection.mat->refractive){                
                //direction of refraction, using snells law
                double cos_idx = ray_norm.dot(I_dir);
                double idx = ray.intersection.mat->index_of_refraction;
                Vector3D dir = idx * I_dir - (-cos_idx + idx*cos_idx) * ray_norm;
                
                //create new ray
                Ray3D new_ray;
                new_ray.dir = dir;
                new_ray.dir.normalize();
                new_ray.origin = ray.intersection.point + 0.01 * new_ray.dir;
                
                //shade ray
                Color refrCol = shadeRay(new_ray, scene, light_list, k_bounce+1);
                col = col + refrCol;
            } 
            //for reflection
            Ray3D ray_new;        
            ray_new.dir = ray.dir - (2 * (ray.intersection.normal.dot(ray.dir)) * ray.intersection.normal);
            ray_new.dir.normalize();
		    // Avoid intersecting with original object
		    ray_new.origin = ray.intersection.point + 0.01*ray_new.dir;
            Color new_col = shadeRay(ray_new, scene, light_list, k_bounce+1);
		    // Add new color with a small scalar multiple
		    col = col + 0.2*new_col; 
		
        }
		col.clamp();
	}

	return col; 
}	

void Raytracer::render(Camera& camera, Scene& scene, LightList& light_list, Image& image) {
    computeTransforms(scene);

    Matrix4x4 viewToWorld;
    double factor = (double(image.height)/2)/tan(camera.fov*M_PI/360.0);

    viewToWorld = camera.initInvViewMatrix();

    // Construct a ray for each pixel.
    for (int i = 0; i < image.height; i++) {
        for (int j = 0; j < image.width; j++) {
            // Sets up ray origin and direction in view space, 
            // image plane is at z = -1.
            Point3D origin(0, 0, 0);
            Point3D imagePlane;
            Color final_col(0.0, 0.0, 0.0);
            // 4x Supersampling AA - 4 rays per pixel, averaged out
            for (float i_frag = i; i_frag < i + 1; i_frag += 0.5f) {
                for (float j_frag = j; j_frag < j + 1; j_frag += 0.5f) {
                    // 4 rays per pixel, by adding 0.5f to the planar directions
                    imagePlane[0] = (-double(image.width)/2 + 0.5 + j_frag)/factor;
                    imagePlane[1] = (-double(image.height)/2 + 0.5 + i_frag)/factor;
                    imagePlane[2] = -1;
                    
                    // 2x Depth of Field
					for(int d=0; d<2; d++){
						//choose random number b/w -0.019 to 0.019;
						float r = ((float(rand())) / float(RAND_MAX)) * 0.028 - 0.019;
						//generate new cam eye position based on the random number
						Point3D eye = camera.eye;
						eye[0] += r; 
						r = ((float(rand())) / float(RAND_MAX)) * 0.028 - 0.019;
						eye[1] += r;
						r = ((float(rand())) / float(RAND_MAX)) * 0.028 - 0.019;
						eye[2] += r;
						//make new camera, with new eye, same view and up
						Camera cam(eye, camera.view, camera.up, camera.fov);
						Matrix4x4 vTW = cam.initInvViewMatrix();
						//generate new ray
						Point3D w_origin = vTW * imagePlane;
						Vector3D w_dir = w_origin - cam.eye;
						w_dir.normalize();
						Ray3D ray(w_origin, w_dir); 
						//apply shading
						Color col = shadeRay(ray, scene, light_list);
						// Apply the coefficient and add to the final colour (0.25 since 4 rays)
						col = 0.5 * 0.25 * col;
						final_col = final_col + col;
					}
                }
            }
            
            image.setColorAtPixel(i, j, final_col);            
        }
    }
}

