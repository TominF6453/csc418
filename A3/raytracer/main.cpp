/***********************************************************
	
	Starter code for Assignment 3

***********************************************************/

#include <cstdlib>
#include "raytracer.h"

int main(int argc, char* argv[])
{
	// Build your scene and setup your camera here, by calling 
	// functions from Raytracer.  The code here sets up an example
	// scene and renders it from two different view points, DO NOT
	// change this if you're just implementing part one of the 
	// assignment.  
	Raytracer raytracer;
	LightList light_list;
	Scene scene;   

	int width = 640;
	int height = 480;

	if (argc == 3) {
		width = atoi(argv[1]);
		height = atoi(argv[2]);
	}
	
	// Define materials for shading.
	Material gold(Color(0.3, 0.3, 0.3), Color(0.75164,0.60648,0.22648),
		Color(0.628281, 0.555802, 0.366065),
		51.2, false, 1.3);
	Material glass(Color(0.1, 0.0, 0.0), Color(0.0, 0.0, 0.0), Color(0.0, 0.0, 0.0), 51.2, true, 1.6);
	Material jade(Color(0, 0, 0), Color(0.54,0.89,0.63),
		Color(0.316228,0.316228,0.316228),
		12.8, false, 1.2);
	Material ruby(Color(0.2,0.2,0.2), Color(0.87843,0.06667,0.37255),
		Color(0.8,0.8,0.8), 51.2, false, 1.0);

	// Defines a point light source.
    //PointLight* pLight = new PointLight(Point3D(0,0,5), Color(0.9,0.9,0.9));
    //light_list.push_back(pLight);
    
    // Define the grid of light sources to simulate soft shadows.
    Color light_col(0.9,0.9,0.9);
    Point3D origin(0,0,5);
    PointLight* pLight;
    for (float xmod = -0.3f; xmod <= 0.3f; xmod += 0.3f) {
        for (float ymod = -0.3f; ymod <= 0.3f; ymod += 0.3f) {
            pLight = new PointLight(Point3D(origin[0] + xmod, origin[1] + ymod, origin[2]), light_col);
            light_list.push_back(pLight);
        }
    }

	// Add a unit square into the scene with material mat.
	SceneNode* sphere = new SceneNode(new UnitSphere(), &gold);
	scene.push_back(sphere);
	SceneNode* plane = new SceneNode(new UnitSquare(), &jade);
	scene.push_back(plane);
	SceneNode* sphere_glass = new SceneNode(new UnitSphere(), &glass);
	scene.push_back(sphere_glass);
	SceneNode* cylinder = new SceneNode(new UnitCylinder(), &ruby);
	scene.push_back(cylinder);

	// Apply some transformations to the sphere and unit square.
	double factor1[3] = { 1.0, 2.0, 1.0 };
	sphere->translate(Vector3D(0, 0, -5));
	sphere->rotate('x', -45);
	sphere->rotate('z', 45);
	sphere->scale(Point3D(0, 0, 0), factor1);

	double factor2[3] = { 6.0, 6.0, 6.0 };
	plane->translate(Vector3D(0, 0, -7));
	plane->rotate('z', 45);
	plane->scale(Point3D(0, 0, 0), factor2);
	
	// Apply transformations to new glass sphere
	sphere_glass->translate(Vector3D(3, 1, -3));
	
	// Apply transformations to ruby cylinder
	double factor3[3] = {1.0,1.0,2.0};
	cylinder->translate(Vector3D(-2.5, -1, -5));
	cylinder->rotate('y', 80);
	cylinder->rotate('x', 20);
	cylinder->scale(Point3D(0,0,0), factor3);

	// Render the scene, feel free to make the image smaller for
	// testing purposes.	
	Camera camera1(Point3D(0, 0, 1), Vector3D(0, 0, -1), Vector3D(0, 1, 0), 60.0);
	Image image1(width, height);
	raytracer.render(camera1, scene, light_list, image1); //render 3D scene to image
	image1.flushPixelBuffer("view1.bmp"); //save rendered image to file

	// Render it from a different point of view.
	Camera camera2(Point3D(4, 2, 1), Vector3D(-4, -2, -6), Vector3D(0, 1, 0), 60.0);
	Image image2(width, height);
	raytracer.render(camera2, scene, light_list, image2);
	image2.flushPixelBuffer("view2.bmp");

	// Free memory
	for (size_t i = 0; i < scene.size(); ++i) {
		delete scene[i];
	}

	for (size_t i = 0; i < light_list.size(); ++i) {
		delete light_list[i];
	}

	return 0;
}
