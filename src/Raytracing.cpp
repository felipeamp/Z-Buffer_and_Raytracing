// Raytracing.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include <cmath>
#include <limits>
#include <memory>
#include <iostream>
#include <vector>
#include <gl\glut.h>

#include "box.h"
#include "camera.h"
#include "light.h"
#include "obj.h"
#include "ray.h"
#include "rectangle.h"
#include "scene.h"
#include "sphere.h"
#include "vec3.h"


const int WIDTH = 800;
const int HEIGHT = 800;
const int MAX_DEPTH = 3;
const int SHININESS = 32;

Scene scene = Scene();

///////////////////////////////////////////////////////
//          BASIC COLOR AND RAYTRACING FUNCTIONS     //
///////////////////////////////////////////////////////

void calculate_intersections(Ray& ray) {
    for (size_t object_index = 0; object_index < scene.objects.size(); ++object_index){
        if (object_index != ray.prev_object_index) {
            float distance_to_intersection = scene.objects[object_index]->intersection(ray);
            if (distance_to_intersection > 0.0 && distance_to_intersection < ray.length) {
				ray.length = distance_to_intersection;
                ray.object_index = object_index;
            }
        }
    }
}

Ray reflect_ray(Ray ray) {
    Vec3 intersection = ray.origin + ray.direction * ray.length;
    auto object = scene.objects[ray.object_index];
    Vec3 object_normal = object->normal(intersection);
    Vec3 reflected_direction = 
    	 ray.direction - object_normal * 2.0 * Vec3::dot(ray.direction, object_normal);
    return Ray(intersection,
    		   reflected_direction,
    		   ray.depth + 1);
}

Vec3 ray_color(Ray &ray) {
    auto obj = scene.objects[ray.object_index];
    Vec3 color = Vec3(0.0, 0.0, 0.0);
    color += scene.ambient_color * obj->diffuse_coef * obj->color;
    for (size_t light_index = 0; light_index < scene.lights.size(); ++light_index) {
        Light light = scene.lights[light_index];
        // Search for occlusion
        Vec3 intersection = ray.origin + ray.direction * ray.length;
        Ray intersection_to_light = Ray(intersection, light.position - intersection);
        bool has_occlusion = false;
        for (size_t object_index = 0; object_index < scene.objects.size(); ++object_index) {
            auto curr_obj = scene.objects[object_index];
            if (obj != curr_obj) {
				float distance_to_intersection = curr_obj->intersection(intersection_to_light);
                if (distance_to_intersection > 0.0) {
                    has_occlusion = true;
                    break;
                }
            }
        }
        if (has_occlusion) {
        	continue;
        } else {
			Vec3 normal = obj->normal(intersection);
		    float cosine = Vec3::dot(intersection_to_light.direction, normal);
		    if (cosine > 0.0) {
		    	// has diffuse and specular colors
		        color += light.color * obj->color * cosine * obj->diffuse_coef;

			    Vec3 vector_r = (normal * 2 * Vec3::dot(intersection_to_light.direction, normal))
			    	- intersection_to_light.direction;
			    Vec3 vector_v = -ray.direction;

			    color += light.color * obj->specular_coef * obj->color
			    	* std::pow(Vec3::dot(vector_r, vector_v), SHININESS);
		    }
        }
    }
    return color;
}

Vec3 trace_ray(Ray& ray) {
    calculate_intersections(ray);
    if (ray.length < std::numeric_limits<float>::max()) {
        Vec3 color = ray_color(ray);
        if (ray.depth <= MAX_DEPTH) {
        	float obj_reflection_coef = scene.objects[ray.object_index]->reflection_coef;
            if (obj_reflection_coef > 0.0) {
                Ray reflected_ray = reflect_ray(ray);
                reflected_ray.prev_object_index = ray.object_index;
                Vec3 reflected_color = trace_ray(reflected_ray);
                color *= 1 - obj_reflection_coef;
                color += reflected_color * obj_reflection_coef;
            }
        }
        return color;
    } else {
		// background
    	return scene.ambient_color;
    }
}


///////////////////////////////////////////////////////
//                   MAIN CODE                       //
///////////////////////////////////////////////////////

// Makes scene
void init_config() {
	scene.ambient_color = Vec3(0.1, 0.1, 0.1);

	scene.camera = Camera(Vec3(100.0, 40.0, 40.0), 	// eye
						  Vec3(0.0, 0.0, 0.0),		// center
						  Vec3(0.0, 1.0, 0.0),		// up
						  90.0,						// fov
						  30.0,						// near
						  230.0,					// far
						  WIDTH,					// width
						  HEIGHT);					// height

    Light light = Light(Vec3(1.0, 1.0, 1.0),		// color
    					Vec3(60.0, 120.0, 40.0));	// position
    scene.lights.push_back(light);

	// Sphere
	auto sphere_p = std::make_shared<Sphere>(Vec3(0.0, 20.0, 0.0),	// center
											25.0,					// radius
											Vec3(0.0, 0.0, 1.0),	// color
											1.0,					// diffuse coeficient
											0.0,					// reflection coeficient
											1.0);					// specular coeficient
	scene.objects.push_back(sphere_p);

	// Floor
	auto rect_1_p = std::make_shared<Rectangle>(Vec3(-80.0, -45.0, 50.0),	// corner_1
												Vec3(-80.0, -45.0, -50.0),  // corner_2
												Vec3(50.0, -45.0, -50.0),   // corner_3
												Vec3(50.0, -45.0, 50.0),    // corner_4
			  									Vec3(0.7, 0.7, 0.0),		// color
			  									0.2,						// diffuse coeficient
			  									0.0,						// reflection coeficient
			  									0.2); 						// specular coeficient
    scene.objects.push_back(rect_1_p);

	// Wall
	auto rect_2_p = std::make_shared<Rectangle>(Vec3(-80.0, -50.0, -50.0),	// corner_1
												Vec3(-80.0, 50.0, -50.0),   // corner_2
												Vec3(50.0, 50.0, -50.0),	// corner_3
												Vec3(50.0, -50.0, -50.0),   // corner_4
			  									Vec3(1.0, 0.0, 0.0),		// color
			  									0.3,						// diffuse coeficient
			  									0.0,						// reflection coeficient
			  									0.3); 						// specular coeficient
    scene.objects.push_back(rect_2_p);

	// Box
	auto box_1 = std::make_shared<Box>(Vec3(25.0, -10.0, 0.0),		// corner_1
									   Vec3(50.0, 15.0, -25.0),		// corner_2
									   Vec3(0.0, 1.0, 0.0),			// color
									   1.0,							// diffuse coeficient
									   0.0,							// reflection coeficient
									   1.0); 						// specular coeficient
	scene.objects.push_back(box_1);
}

void draw_zbuffer() {
	std::cout << "Starting Z-Buffer rendering..." << std::endl;
	glClearColor(scene.ambient_color.vec[0], scene.ambient_color.vec[1], scene.ambient_color.vec[2], 1.0);
	glShadeModel(GL_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_DEPTH_TEST);// | GL_NORMALIZE);
	glDepthFunc(GL_LESS);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_LIGHTING);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(scene.camera.fov,
		scene.camera.width / scene.camera.height,
		scene.camera.near,
		scene.camera.far);

	//// Lights
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, scene.ambient_color.vec);
	float posicao_luz[4] = {
		scene.lights[0].position[0],
		scene.lights[0].position[1],
		scene.lights[0].position[2],
		1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, posicao_luz);
	glLightfv(GL_LIGHT0, GL_AMBIENT, scene.ambient_color.vec);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, scene.lights[0].color.vec);
	glLightfv(GL_LIGHT0, GL_SPECULAR, scene.lights[0].color.vec);
	glEnable(GL_LIGHT0);

	// Camera
	gluLookAt(scene.camera.eye[0], scene.camera.eye[1], scene.camera.eye[2],
		scene.camera.center[0], scene.camera.center[1], scene.camera.center[2],
		scene.camera.up[0], scene.camera.up[1], scene.camera.up[2]);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	// Objects
	float white[3] = {1.0, 1.0, 1.0};

	// Sphere
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	Sphere* sphere_p = dynamic_cast<Sphere*>(scene.objects[0].get());
	glTranslatef(sphere_p->center[0], sphere_p->center[1], sphere_p->center[2]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, sphere_p->color.vec);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, sphere_p->color.vec);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, SHININESS);
	glColor3f(sphere_p->color[0], sphere_p->color[1], sphere_p->color[2]);
	glutSolidSphere(sphere_p->radius, 64, 64);
	glPopMatrix();

	// Floor
	glPushMatrix();
	Rectangle* floor_p = dynamic_cast<Rectangle*>(scene.objects[1].get());
	glTranslatef(0.0f, floor_p->corners[1].vec[1], 0.0f);
	glRotated(-90.0, 1.0, 0.0, 0.0);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, floor_p->color.vec);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, floor_p->color.vec);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, SHININESS);
	glColor3f(floor_p->color[0], floor_p->color[1], floor_p->color[2]);
	glRectf(floor_p->corners[1].vec[0],
		floor_p->corners[1].vec[2],
		floor_p->corners[3].vec[0],
		floor_p->corners[3].vec[2]);
	glPopMatrix();

	// Wall
	glPushMatrix();
	Rectangle* wall_p = dynamic_cast<Rectangle*>(scene.objects[2].get());
	glTranslatef(0.0f, 0.0f, wall_p->corners[0].vec[2]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, wall_p->color.vec);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, wall_p->color.vec);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, SHININESS);
	glColor3f(wall_p->color[0], wall_p->color[1], wall_p->color[2]);
	glRectf(wall_p->corners[0].vec[0],
		wall_p->corners[0].vec[1],
		wall_p->corners[2].vec[0],
		wall_p->corners[2].vec[1]);
	glPopMatrix();

	// Box
	glPushMatrix();
	Box* box_p = dynamic_cast<Box*>(scene.objects[3].get());
	glTranslatef(0.5 * (box_p->x_max + box_p->x_min),
		0.5 * (box_p->y_max + box_p->y_min),
		0.5 * (box_p->z_max + box_p->z_min));
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, box_p->color.vec);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, box_p->color.vec);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, SHININESS);
	glColor3f(box_p->color[0], box_p->color[1], box_p->color[2]);
	glutSolidCube(box_p->x_max - box_p->x_min);
	glPopMatrix();

	glFlush();
	std::cout << "Z-Buffer rendering done!" << std::endl;
}


void draw_raytracing() {
	std::cout << "Starting Raytracing rendering..." << std::endl;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, WIDTH, 0.0, HEIGHT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_DEPTH);
	glDisable(GL_LIGHTING);


	for (int point_y = 0; point_y < scene.camera.height; ++point_y) {
		for (int point_x = 0; point_x < scene.camera.width; ++point_x) {
			Ray ray = scene.camera.ray(point_x, point_y);
			Vec3 color = trace_ray(ray);
			glBegin(GL_POINTS);
			glColor3f(color[0], color[1], color[2]);
			glVertex2f(point_x, point_y);
			glEnd();
			glFlush();
		}
	}
	std::cout << "Raytracing rendering done!" << std::endl;
}

void display() {
	draw_zbuffer();
	draw_raytracing();
	std::cout << "All done!" << std::endl << std::endl;
}


void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y) {
	if (key == 27U) { // ESC
		exit(0);
	}
}

void main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Raytracing - Felipe de A. Mello Pereira");
	init_config();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutMainLoop();
}

