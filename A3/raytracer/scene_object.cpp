/***********************************************************
	
	Starter code for Assignment 3
	
	Implements scene_object.h

***********************************************************/

#include <cmath>
#include "scene_object.h"

bool UnitSquare::intersect(Ray3D& ray, const Matrix4x4& worldToModel,
		const Matrix4x4& modelToWorld) {
	// TODO: implement intersection code for UnitSquare, which is
	// defined on the xy-plane, with vertices (0.5, 0.5, 0), 
	// (-0.5, 0.5, 0), (-0.5, -0.5, 0), (0.5, -0.5, 0), and normal
	// (0, 0, 1).
	//
	// Your goal here is to fill ray.intersection with correct values
	// should an intersection occur.  This includes intersection.point, 
	// intersection.normal, intersection.none, intersection.t_value.   
	//
	// HINT: Remember to first transform the ray into object space  
	// to simplify the intersection test.
	
	Vector3D n(0.0,0.0,1.0); // Square normal
	
	// Cast ray to object space
	Vector3D raydir = worldToModel * ray.dir;
	Point3D rayorigin = worldToModel * ray.origin;
	float t_val = -(rayorigin[2]/raydir[2]);
	if (t_val <= 0) { // Ray moving away from square
		return false; 
	}
	
	// Check x and y values
	float x = rayorigin[0] + t_val * raydir[0];
	float y = rayorigin[1] + t_val * raydir[1];
	Point3D intersect(x,y,0.0);
	
	if ((x <= 0.5 && x >= -0.5) && (y <= 0.5 && y >= -0.5)){
		// Values fall within the square, intersection
		// Update ray
		if (ray.intersection.none || t_val < ray.intersection.t_value) {
			// If the ray hasn't intersected, or this intersection is closer, update
			ray.intersection.none = false;
			ray.intersection.t_value = t_val;
			ray.intersection.normal = worldToModel.transpose()*n;
			ray.intersection.normal.normalize();
			ray.intersection.point = modelToWorld * intersect;
			return true;
		}
	}
	return false;
}

bool UnitSphere::intersect(Ray3D& ray, const Matrix4x4& worldToModel,
		const Matrix4x4& modelToWorld) {
	// TODO: implement intersection code for UnitSphere, which is centred 
	// on the origin.  
	//
	// Your goal here is to fill ray.intersection with correct values
	// should an intersection occur.  This includes intersection.point, 
	// intersection.normal, intersection.none, intersection.t_value.   
	//
	// HINT: Remember to first transform the ray into object space  
	// to simplify the intersection test.
	
	// Cast ray to object space
	Vector3D raydir = worldToModel * ray.dir;
	Point3D rayorigin = worldToModel * ray.origin;
	
	Point3D c(0.0,0.0,0.0); // Center of sphere is origin
	float r = 1.0; // Radius of unit sphere is 1
	
	// Quadratic form to find intersection ( At^2 + Bt + C = 0 )
	float A = raydir.dot(raydir);
	float B = 2*(raydir.dot(rayorigin-c));
	float C = (rayorigin-c).dot(rayorigin-c) - r; // r^2 is 1, unit sphere.
	
	// Solve using quadratic formula
	float D = B*B - 4*A*C; // discriminant of the quadratic
	if (D < 0) {
		// No intersection
		return false;
	}
	float t0 = (-B + sqrt(D))/(2.0*A); // t values for both + and -
	float t1 = (-B - sqrt(D))/(2.0*A);
	
	// Find correct t value via minimum (first intersection)
	float t_val;
	if (t0 < 0 && t1 < 0) {
		return false; // Neither are intersections
	} else if (t0 < 0) {
		t_val = t1;
	} else if (t1 < 0) {
		t_val = t0;
	} else {
		t_val = fmin(t0,t1);
	}
	
	// Check point values
	float x = rayorigin[0] + t_val * raydir[0];
	float y = rayorigin[1] + t_val * raydir[1];
	float z = rayorigin[2] + t_val * raydir[2];
	Point3D intersect(x,y,z);
	Vector3D n(x,y,z);
	n.normalize();
	
	// Update ray
	if (ray.intersection.none || t_val < ray.intersection.t_value) {
		// If the ray hasn't intersected, or this intersection is closer, update
		ray.intersection.none = false;
		ray.intersection.t_value = t_val;
		ray.intersection.normal = worldToModel.transpose()*n;
		ray.intersection.normal.normalize();
		ray.intersection.point = modelToWorld * intersect;
		return true;
	}
	return false;
}

// Intersection code for new geometry: cylinder.
bool UnitCylinder::intersect(Ray3D& ray, const Matrix4x4& worldToModel,
		const Matrix4x4& modelToWorld) {
	// Intersection for unit cylinder with a height and radius = 1,
	// aligned along the z axis (x^2 + y^2 = r^2, for inf cylinder),
	// centered on the origin such that the top and bottom circles have
	// their centers at (0,0,0.5) and (0,0,-0.5) respectively.
	//
	// Your goal here is to fill ray.intersection with correct values
	// should an intersection occur.  This includes intersection.point, 
	// intersection.normal, intersection.none, intersection.t_value.   
	//
	// HINT: Remember to first transform the ray into object space  
	// to simplify the intersection test.
	
	// Cast ray to object space
	Vector3D raydir = worldToModel * ray.dir;
	Point3D rayorigin = worldToModel * ray.origin;
	
	Point3D c(0.0,0.0,0.0); // Center of sphere is origin
	float r = 1.0; // Radius of unit sphere is 1
	
	// Quadratic form to find intersection ( At^2 + Bt + C = 0 )
	float A = raydir[0]*raydir[0] + raydir[1]*raydir[1];
	float B = 2 * (rayorigin[0]*raydir[0] + rayorigin[1]*raydir[1]);
	float C = rayorigin[0]*rayorigin[0] + rayorigin[1]*rayorigin[1] - r;
	
	// Solve using quadratic formula
	float D = B*B - 4*A*C; // discriminant of the quadratic
	if (D < 0) {
		// No intersection
		return false;
	}
	float t0 = (-B + sqrt(D))/(2.0*A); // t values for both + and -
	float t1 = (-B - sqrt(D))/(2.0*A);
	
	// Find correct t value via minimum (first intersection)
	// Since this is the infinite cylinder intersection, we will have a
	// separate t_val to compare to later with the top and bottom caps.
	float t_val1;
	if (t0 < 0 && t1 < 0) {
		return false; // Neither are intersections
	} else if (t0 < 0) {
		t_val1 = t1;
	} else if (t1 < 0) {
		t_val1 = t0;
	} else {
		t_val1 = fmin(t0,t1);
	}
	// Get intersection data for the cylinder.
	float x = rayorigin[0] + t_val1 * raydir[0];
	float y = rayorigin[1] + t_val1 * raydir[1];
	float z = rayorigin[2] + t_val1 * raydir[2];
	Point3D intersect1(x,y,z);
	Vector3D n1(rayorigin[0] + t_val1*raydir[0], rayorigin[1] + t_val1*raydir[1], 0);
	n1.normalize();
	
	// Check if ray intersects with the either of the caps.
	// Caps are on z-plane, very similar to unitSquare calculation...
	t0 = (-0.5 - rayorigin[2])/raydir[2];
	t1 = (0.5 - rayorigin[2])/raydir[2];
	
	// Find correct t value via minimum (first intersection)
	// This is the z-planar circle intersections. Separate t_val to
	// compare to cylinder intersection later.
	float t_val2 = fmin(t0,t1);
	// Get intersection data for the circles.
	x = rayorigin[0] + t_val2 * raydir[0];
	y = rayorigin[1] + t_val2 * raydir[1];
	z = rayorigin[2] + t_val2 * raydir[2];
	Point3D intersect2(x,y,z);
	Vector3D n2(0,0,1);
	if (t_val2 == t0) { // If intersection is with bottom circle, normal has negative z.
		n2[2] = -1;
	}
	
	// Check intersections with either of the circles.
	if (intersect2[0]*intersect2[0] + intersect2[1]*intersect2[1] <= 1 && t_val2 > 0) {
		// Intersection is within circle radius.
		// Update ray
		if (ray.intersection.none || t_val2 < ray.intersection.t_value) {
			// If the ray hasn't intersected, or this intersection is closer, update
			ray.intersection.none = false;
			ray.intersection.t_value = t_val2;
			ray.intersection.normal = worldToModel.transpose()*n2;
			ray.intersection.normal.normalize();
			ray.intersection.point = modelToWorld * intersect2;
			return true;
		}
	}
	
	// If it doesn't intersect with the circles, check if it intersects with the side.
	if (intersect1[2] < 0.5 && intersect1[2] > -0.5) {
		// Intersection is within the z boundaries.
		// Update ray
		if (ray.intersection.none || t_val1 < ray.intersection.t_value) {
			// If the ray hasn't intersected, or this intersection is closer, update
			ray.intersection.none = false;
			ray.intersection.t_value = t_val1;
			ray.intersection.normal = worldToModel.transpose()*n1;
			ray.intersection.normal.normalize();
			ray.intersection.point = modelToWorld * intersect1;
			return true;
		}
	}
	
	return false;
}

void SceneNode::rotate(char axis, double angle) {
	Matrix4x4 rotation;
	double toRadian = 2*M_PI/360.0;
	int i;
	
	for (i = 0; i < 2; i++) {
		switch(axis) {
			case 'x':
				rotation[0][0] = 1;
				rotation[1][1] = cos(angle*toRadian);
				rotation[1][2] = -sin(angle*toRadian);
				rotation[2][1] = sin(angle*toRadian);
				rotation[2][2] = cos(angle*toRadian);
				rotation[3][3] = 1;
			break;
			case 'y':
				rotation[0][0] = cos(angle*toRadian);
				rotation[0][2] = sin(angle*toRadian);
				rotation[1][1] = 1;
				rotation[2][0] = -sin(angle*toRadian);
				rotation[2][2] = cos(angle*toRadian);
				rotation[3][3] = 1;
			break;
			case 'z':
				rotation[0][0] = cos(angle*toRadian);
				rotation[0][1] = -sin(angle*toRadian);
				rotation[1][0] = sin(angle*toRadian);
				rotation[1][1] = cos(angle*toRadian);
				rotation[2][2] = 1;
				rotation[3][3] = 1;
			break;
		}
		if (i == 0) {
			this->trans = this->trans*rotation; 	
			angle = -angle;
		} 
		else {
			this->invtrans = rotation*this->invtrans; 
		}	
	}
}

void SceneNode::translate(Vector3D trans) {
	Matrix4x4 translation;
	
	translation[0][3] = trans[0];
	translation[1][3] = trans[1];
	translation[2][3] = trans[2];
	this->trans = this->trans*translation; 	
	translation[0][3] = -trans[0];
	translation[1][3] = -trans[1];
	translation[2][3] = -trans[2];
	this->invtrans = translation*this->invtrans; 
}

void SceneNode::scale(Point3D origin, double factor[3] ) {
	Matrix4x4 scale;
	
	scale[0][0] = factor[0];
	scale[0][3] = origin[0] - factor[0] * origin[0];
	scale[1][1] = factor[1];
	scale[1][3] = origin[1] - factor[1] * origin[1];
	scale[2][2] = factor[2];
	scale[2][3] = origin[2] - factor[2] * origin[2];
	this->trans = this->trans*scale; 	
	scale[0][0] = 1/factor[0];
	scale[0][3] = origin[0] - 1/factor[0] * origin[0];
	scale[1][1] = 1/factor[1];
	scale[1][3] = origin[1] - 1/factor[1] * origin[1];
	scale[2][2] = 1/factor[2];
	scale[2][3] = origin[2] - 1/factor[2] * origin[2];
	this->invtrans = scale*this->invtrans; 
}


