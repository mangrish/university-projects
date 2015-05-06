/* structs.h
 * Defines data structures for the rendering program
 */

#ifndef STRUCTS_H
#define STRUCTS_H

#define MAX_COLOUR_COMPONENT    255	
#define FALSE					0
#define TRUE					1
#define PI						3.1415927
#define PN						1000.00			/* phongs n value */
#define KS						0.1				/* the specular reflection coefficient */

#define round(x)			((int) (x+0.5))


typedef struct colour {
    int                 red;
    int                 green;
    int                 blue;
    int                 alpha;
} Colour;


typedef struct vector {			 
	float				a;
	float				b;
	float				c;
} Vector;

typedef struct point {
    float               x_ord;
    float               y_ord;
    float               z_ord;
	Vector				normal; /* normal of a vertice	*/
    struct point*       next;
} Point;

typedef struct polygon {
    int                 num_vertices;
    int*                vertices;
	int					ID;			/* the ID of a polygon for polygon_table */
	struct vector		normal;		/* the normal of a polygon */
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

typedef struct pixel {			/* the frame buffer */
	Colour			colour;		/* the RGB values or a pixel */
	float			z;			/* the z value used for z-buffering */
} Pixel;

typedef struct z_info {		/*	info for z-buffering	*/
	float			z;
	float			z_min;
	float			z_max;
	float			y_min;
} Z_info;

typedef struct normal_info {	/* info for calculating normals for phong */
	Vector			normal;
	Vector			normal_min;
	Vector			normal_max;
} Normal_info;

typedef struct bucket {			/*a bucket used in the AET and the ET	*/
	float			y_max;		/* has data for defining an edge, the 	*/
	float			x_min;		/* z-buffer, and for phong(vertex normals*/
	float			delta_x;
	Z_info			z_data;
	Normal_info		normal_data;
	struct bucket*	next;
} Bucket;

typedef struct identity {		/* polygon table used for each vertice */
	int					polyID; /* to find all faces that are connected*/
	struct identity*	next;	/* to it.							   */
} Identity;


#endif
