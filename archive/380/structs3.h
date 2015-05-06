/* structs.h
 *
 * Defines data structures for the rendering program; you will almost
 * certainly want to change or extend these to suit your own renderer
 * implementation.
 */

#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct colour {
    int                 red;
    int                 green;
    int                 blue;
    int                 alpha;
} Colour;

#define MAX_COLOUR_COMPONENT    255
#define FALSE 0
#define TRUE 	1
#define PI 3.1415927

typedef struct vector {
	float									a;
	float									b;
	float									c;
} Vector;


typedef struct point {
    float               x_ord;
    float               y_ord;
    float               z_ord;
    struct point*       next;
} Point;

typedef struct polygon {
    int                 num_vertices;
    int*                vertices;
		struct vector				normal;
    struct colour       refl;
    struct polygon*     next;
} Polygon;

typedef struct polyhedron {
    int                 num_vertices;
    int                 num_faces;
    int                 num_edges;
    struct point*       vertices;
    struct polygon*     faces;
    struct polyhedron*  next;
} Polyhedron;

typedef struct polyhedron_list {
    struct polyhedron*  first;
} Polyhedron_List;

typedef struct light_source {
    float                   intensity;
    float                   x_loc;
    float                   y_loc;
    float                   z_loc;
    struct light_source*    next;
} Light_Source;

typedef struct light_source_list {
    struct light_source*    first;
} Light_Source_List;

typedef struct scene {
    Polyhedron_List*        polyhedrons;
    Light_Source_List*      light_sources;
    float                   ambient_light;
} Scene;

typedef struct pixel {
	Colour	 		colour;
	float				z;
} Pixel;

typedef struct bucket {
	float												y_max;
	float												x_min;
	float												delta_x;
	float												z;
	float												z_min;
	float												z_max;
	float												zy_min;
	struct bucket*							next;
}	Bucket;


#endif
