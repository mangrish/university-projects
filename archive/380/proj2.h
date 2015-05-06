/* proj.h
 */

#include "structs2.h"
#include "read_off.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*=======================================*/
/*---GLOBAL VARIABLES--------------------*/	
/*=======================================*/

int					focal_length;
int					pixel_width;
int					pixel_height;

float				scale_x, scale_y, scale_z;	/*	scaling		*/
float				rot_x, rot_y, rot_z;		/*	rotation	*/
float				trans_x, trans_y, trans_z;	/*	translation	*/

Pixel**				display;		/* the frame buffer */
Bucket**			edge_table;		/* the edge table	*/
Identity**			polygon_table;	/* the polyfon table*/

float				ambient_light;		/* ambient light 		*/
Light_Source_List*	light_source_list;	/* light sources list	*/


/*=======================================*/
/*---FUNCTION PROTOTYPES-----------------*/	
/*=======================================*/

int main(int argc, char** argv);

void read_scene_defn(Scene*);
void print_ppm(void);
Pixel** initialise_array( void);
Bucket** initialise_edge_table(void);
void usage(char *progname);
void build_edge_table(Scene*); 
void scanline(Polygon);

Polyhedron* get_polyhedron(const char*);
Light_Source* get_light_source(const char*);

void transform(Point*); 
void perspective_projection(Point*);
Bucket* assign_values(float, float, float, float, float, float,Vector, Vector, int *);
Bucket* insert_new_node(Bucket*, Bucket*);

int delete_old_aet_nodes(Bucket**, int); 
void fill_scanline(Bucket*, int, Polygon );
Bucket* update_aet(Bucket*, int);

void rotate(Point*, float transMatrix[4][4]);
Vector calc_normal( Point, Point, Point);
Vector normalise(Vector);

void fill_pixel(int ,int ,float , Polygon, Vector);

Identity** initialise_polygon_table(int);
Identity* insert_poly_table(Identity*, int); 

Vector summate_vectors(Vector, Vector);
Vector interpolate_normal(Vector, Vector, int, int, int);

