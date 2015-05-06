/******************************************************************************/
/*----------------------------------------------------------------------------*/
/*
**  Program : proj2.c: 
**  For     : 433 -380: Graphics and Computation;
**            PROJECT 
**  Written : Mark Angrish, Student number: 74592
**            October 2000;
**  Input   : From stdin:reads a scene definition from stdin in
** 			  accordance with the project specification, as well as reading in each as
**			  many objects as included in the scene.Each object is assumed to be in its
**			  own OFF file. 
**  Output  : To stdout: ppm format.
*/
/*----------------------------------------------------------------------------*/
/******************************************************************************/

#include "proj2.h"

int main(int argc, char* argv[])
{
	Scene*	scene = NULL;

    /* Allocate initial containers for our scene; 
     */

    if (!(scene = calloc(1, sizeof(Scene)))) {
        printf("Cannot allocate storage for type `%s'.\n", "Scene");
    }

    if (!(scene->polyhedrons = calloc(1, sizeof(Polyhedron_List)))) {
        printf("Cannot allocate storage for type `%s'.\n", "Polyhedron_List");
    }

    if (!(scene->light_sources = calloc(1, sizeof(Light_Source_List)))) {
        printf("Cannot allocate storage for type `%s'.\n", "Light_Source_List");
    }

	if (argc == 4){
		focal_length = atoi(argv[1]);
		pixel_width = atoi(argv[2]);
		pixel_height = atoi(argv[3]);
	}

	else
	{
		usage(argv[0]);
	}

	display = initialise_array();
	edge_table = initialise_edge_table();

    /* Here we read in lines from the scene definition, currently expected to
     * be piped in from stdin.
     */

    read_scene_defn(scene);

	build_edge_table(scene); 

    print_ppm();

    return 0;
}


/*==============================================================================
*---read_scene_defn(): Reads a scene definition, currently line by line from 
*--------------------  stdin;
*===============================================================================
*/

void read_scene_defn(Scene* scene)
{
	Polyhedron*     polyhedron = NULL;
    Light_Source*   light_source = NULL;
    char            input_line[MAX_LENGTH_INPUT_LINE] = {0};
    char            input_word[MAX_LENGTH_INPUT_LINE] = {0};


    while (gets(input_line)) {


        /* Check that the line read is not a comment line or a blank line.
         */

        if (input_line && (input_line[0] != COMMENT_CHAR)) {

            /* The first word of each scene definition line is a command that
             * identifies the line's type - either "off" for a new object,
             * "light" for a distant light source or "ambient" to define the
             * scene's ambient light level.
             */

            sscanf(input_line, "%s", input_word);

            if (!strcmp(input_word, OBJECT_IDENTIFIER)) {


                if (!scene->polyhedrons->first) {
                    scene->polyhedrons->first = get_polyhedron(input_line);
                    polyhedron = scene->polyhedrons->first;
                } else {
                    polyhedron->next = get_polyhedron(input_line);
                    polyhedron = polyhedron->next;
                }
            }

            if (!strcmp(input_word, LIGHT_IDENTIFIER)) {


                if (!scene->light_sources->first) {
                    scene->light_sources->first = get_light_source(input_line);
                    light_source = scene->light_sources->first;
                } else {
                    light_source->next = get_light_source(input_line);
                    light_source = light_source->next;
                }
            }

            if (!strcmp(input_word, AMBIENT_IDENTIFIER)) {

                sscanf(input_line, "%s %f", input_word, &scene->ambient_light);
            }
        }
    }
}


/*==============================================================================
*---get_polyhedron():	Reads a polyhedron from an OFF file specified in the 
*-------------------	scene definition.
*===============================================================================
*/

Polyhedron* get_polyhedron(const char* input_line)
{
    Polyhedron*     polyhedron = NULL;
    char            input_word[MAX_LENGTH_INPUT_LINE] = {0};
    char            off_file_name[MAX_LENGTH_INPUT_LINE] = {0};
    int             num_args = 0;
	int				i;
		

    if ((num_args = sscanf(input_line, "%s %s %f %f %f %f %f %f %f %f %f",
        input_word, off_file_name, &scale_x, &scale_y, &scale_z, &rot_x, 
		&rot_y, &rot_z, &trans_x, &trans_y, &trans_z)) != 11)
	{
		printf("Syntax error in input.\n");
        return NULL;
	}

	rot_x = rot_x * (PI/180);
	rot_y = rot_y * (PI/180);
	rot_z = rot_z * (PI/180);

    /* Get the polyhedron's data; space for the polyhedron will be dynamically
     * allocated by read_off().
     */

    polyhedron = read_off(off_file_name);

	for (i = 0; i < polyhedron->num_vertices; i++) 
	{
		transform(&polyhedron->vertices[i]);
	}
	
    return polyhedron;
}


/*==============================================================================
*---get_lightsource(): Reads a description of a light source from the scene 
*--------------------  description file
*===============================================================================
*/

Light_Source* get_light_source(const char* input_line)
{
    Light_Source*   light_source = NULL;
    char            input_word[MAX_LENGTH_INPUT_LINE] = {0};


    if (!(light_source = calloc(1, sizeof(Light_Source)))) {
        printf("Cannot allocate storage for type `%s'.\n", "Light_Source");
    }

    sscanf(input_line, "%s %f %f %f %f",
            input_word,
            &light_source->intensity,
            &light_source->x_loc,
            &light_source->y_loc,
            &light_source->z_loc);

    return light_source;
}


/*==============================================================
*---print_ppm(): print the frame buffer to stdout in ppm format.
*===============================================================
*/

void print_ppm( void )
{
	int 	i, j;
	
	/*	header information for ppm format in ascii	*/
	printf("P3\n");
	printf("%i %i\n", pixel_width, pixel_height);
	printf("255\n");

	for (i=0; i<= pixel_width-1; i++)
	{
		for (j=pixel_height-1; j >= 0; j--)
		{
			printf("%i %i %i\t",display[j][i].colour.red,
			display[j][i].colour.green,
			display[j][i].colour.blue);
		}
	}
    
	return;
}


/*==============================================================================
*---initialise_array(): initialise the frame buffer to the specified pixel 
----------------------  height and width on the command line.
*===============================================================================
*/

Pixel** initialise_array( void )
{
	int 		i, j;
	Pixel**		pixel;
	  
	  
	pixel  = calloc(pixel_width, sizeof(Pixel*));		/* initialise a frame buffer */

	for (i = 0; i <= pixel_height-1; i++)
	{
		pixel[i] = calloc(pixel_height, sizeof(Pixel));
		for (j = 0; j <= pixel_height-1; j++)
		{
			pixel[i][j].colour.red = 0;			/*...and set its values for each pixel*/
			pixel[i][j].colour.green = 0;		/* to black....						  */
			pixel[i][j].colour.blue =0;
			pixel[i][j].colour.alpha = 0;
			pixel[i][j].z = 100000;				/* and a value for infinite depth	*/
		}
	}

	return pixel;
}


/*==============================================================================
*---initialise_edge_table(): initialise the frame buffer to the specified pixel
*--------------------------- hieght and width on the command line.
*===============================================================================
*/

Bucket** initialise_edge_table(void)
{
	Bucket** e_table = calloc(pixel_height, sizeof(Bucket*));

	return e_table;
}


/*=====================================================
*---usage(): print the usage of this project to stderr.
*======================================================
*/

void usage(char *progname)
{
	fprintf( stderr, "Usage: %s [#focal point] [#pixel width] [#pixel height] < [file] [stdin] \n", progname );

	exit( EXIT_FAILURE );
}


/*==============================================================================
*---transform(): firstly scales a vertice, then using rotation matricies for
*--------------  each axis, rotates the vertice, then translates a vertice 
*--------------  using the specified veriables in the input.
*===============================================================================
*/

void transform(Point *point)
{
	float x_rotation[4][4];		/*the rotation matricies for x, y and z	*/
	float y_rotation[4][4];
	float z_rotation[4][4];


	point->x_ord = point->x_ord * scale_x;	/*	                    */	
	point->y_ord = point->y_ord * scale_y;	/*	scaling the vertice.*/
	point->z_ord = point->z_ord * scale_z;	/*	                    */

	/*-----------------------*/	
	/*---x roation matricie--*/
	/*-----------------------*/
	x_rotation[0][0] = 1;
	x_rotation[0][1] = 0;
	x_rotation[0][2] = 0;
	x_rotation[0][3] = 0;
	x_rotation[1][0] = 0;
	x_rotation[1][1] = (float) (cos(rot_x));
	x_rotation[1][2] = (float) (sin(rot_x));
	x_rotation[1][3] = 0;
	x_rotation[2][0] = 0;
	x_rotation[2][1] = -(float) (sin(rot_x));;
	x_rotation[2][2] = (float) (cos(rot_x));
	x_rotation[2][3] = 0;
	x_rotation[3][0] = 0;
	x_rotation[3][1] = 0;
	x_rotation[3][2] = 0;
	x_rotation[3][3] = 1;

	/*-----------------------*/	
	/*---y roation matricie--*/
	/*-----------------------*/
	y_rotation[0][0] = (float) (cos(rot_y));
	y_rotation[0][1] = 0;
	y_rotation[0][2] = -(float) (sin(rot_y));
	y_rotation[0][3] = 0;
	y_rotation[1][0] = 0;
	y_rotation[1][1] = 1;
	y_rotation[1][2] = 0;
	y_rotation[1][3] = 0;
	y_rotation[2][0] = (float) (sin(rot_y));
	y_rotation[2][1] = 0;
	y_rotation[2][2] = (float) (cos(rot_y));
	y_rotation[2][3] = 0;
	y_rotation[3][0] = 0;
	y_rotation[3][1] = 0;
	y_rotation[3][2] = 0;
	y_rotation[3][3] = 1;

	/*-----------------------*/	
	/*---z roation matricie--*/
	/*-----------------------*/
	z_rotation[0][0] = (float) (cos(rot_z));
	z_rotation[0][1] = (float) (sin(rot_z));
	z_rotation[0][2] = 0;
	z_rotation[0][3] = 0;
	z_rotation[1][0] = -(float) (sin(rot_z));
	z_rotation[1][1] = (float) (cos(rot_z));
	z_rotation[1][2] = 0;
	z_rotation[1][3] = 0;
	z_rotation[2][0] = 0;
	z_rotation[2][1] = 0;
	z_rotation[2][2] = 1;
	z_rotation[2][3] = 0;
	z_rotation[3][0] = 0;
	z_rotation[3][1] = 0;
	z_rotation[3][2] = 0;
	z_rotation[3][3] = 1;

	rotate(point, x_rotation);	/*	                                    */
	rotate(point, y_rotation);	/*	rotate the vertice around each axis	*/
	rotate(point, z_rotation);	/*	                                    */

	point->x_ord = point->x_ord + trans_x;	/*	                      */
	point->y_ord = point->y_ord + trans_y;	/*	translate the vertice */
	point->z_ord = point->z_ord + trans_z;	/*	                      */

	return;
}


/*==============================================================================
*---rotate(): using a given rotation matrix, the vertice is rotated about that
*-----------  axis.
*===============================================================================
*/

void rotate(Point* point, float rot_matrix[4][4])
{
	int 	j, k;
	float	acc;
	float	vector[4];
	float	xyz_acc[4];


	vector[0] = point->x_ord;	/* inialise a matrix with the values of a vector	*/
    vector[1] = point->y_ord;
    vector[2] = point->z_ord;
    vector[3] = 1;

	for ( j = 0 ; j < 4 ; j++)	/* for each part of the vector */
    {
		acc = 0;
		for ( k = 0 ; k < 4 ; k++)
		{
			acc += vector[k] * rot_matrix[k][j];
		}
		xyz_acc[j] = acc;	/* store it in an accumulator	*/
	}
		
	point->x_ord = xyz_acc[0];	/* then copy it back into the vertice	*/
    point->y_ord = xyz_acc[1];
    point->z_ord = xyz_acc[2];

	return;
}


/*==============================================================================
*---build_edge_table(): for each scene calculates the normals of each face in 
*---------------------- the polyhedron and then for each vertice using a polygon
*---------------------- ID then does perspective projection on each point, then 
*---------------------- for each face in the polyhedron, does a scanline.
*===============================================================================
*/

void build_edge_table(Scene* scene)
{
	int 			i, j, k, l, total;
	float			x1, x2, y1, y2, z1, z2;
	int				y_min;
	int				vertice1, vertice2, vertice3;
	Vector			normal1, normal2;	
	Bucket*			new_edge_node;
	Polyhedron*		polyhedron;
	Identity*		curr_ptr;


	light_source_list	= scene->light_sources;
	ambient_light 		= scene->ambient_light;
	
	/* for each polyhedron in the scene */
	for (polyhedron = scene->polyhedrons->first; polyhedron; polyhedron = polyhedron->next)
	{
	  	/*set up the polygon_id table*/
		polygon_table = initialise_polygon_table(polyhedron->num_vertices);

		/* for each face in the polyhedron calculate the normal*/
		for (j = 0; j<= polyhedron->num_faces-1; j++)
		{
			vertice1 = polyhedron->faces[j].vertices[0];
			vertice2 = polyhedron->faces[j].vertices[1];
			vertice3 = polyhedron->faces[j].vertices[2];
		
			polyhedron->faces[j].ID = j;/* store the ID number for the face	*/
		/*calculate the normal of the face	*/
			polyhedron->faces[j].normal = 
				calc_normal(polyhedron->vertices[vertice1],
				polyhedron->vertices[vertice2], 
				polyhedron->vertices[vertice3]);

			/* for each vertice in the face, place the ID of the polygon	*/
			/* in the polygon ID table										*/ 
			for (k = 0; k <= polyhedron->faces[j].num_vertices-1; k++)
			{
				polygon_table[polyhedron->faces[j].vertices[k]] = 
					insert_poly_table(polygon_table[polyhedron->faces[j].vertices[k]], j);
			}
		}
		
		/* for each vertice, calculate its normal from the polygon table */
		for (i = 0; i <= polyhedron->num_vertices-1; i++)
		{
			curr_ptr = polygon_table[i];
			total = 0;
			while (curr_ptr != NULL)
			{
				total++;
				/*	summate the vertices	*/
				polyhedron->vertices[i].normal = 
				  summate_vectors(polyhedron->vertices[i].normal,
								 polyhedron->faces[curr_ptr->polyID].normal);
				curr_ptr = curr_ptr->next;
			}
			/* get the average	*/
			polyhedron->vertices[i].normal.a =  polyhedron->vertices[i].normal.a / total;
			polyhedron->vertices[i].normal.b =  polyhedron->vertices[i].normal.b / total;
			polyhedron->vertices[i].normal.c =  polyhedron->vertices[i].normal.c / total;
		}

		/* calculate the perspective projection	*/
		for (i = 0; i<= polyhedron->num_vertices-1; i++)
		{
			perspective_projection(&polyhedron->vertices[i]);
		}

		for (j = 0; j<= polyhedron->num_faces-1; j++)
		{

			k = 1;
			for (l = 0 ; l <= polyhedron->faces[j].num_vertices-1; l++)
			{
			  	/* if the vertice is the last in the off format, then use */
				/* the 1st vertice 										  */ 
				if (l==(polyhedron->faces[j].num_vertices-1)) {
					k = 0;
				}
				
				/* information for each alternate pair of vertices		*/
				vertice1 = polyhedron->faces[j].vertices[l];
				vertice2 = polyhedron->faces[j].vertices[k];

				x1 = polyhedron->vertices[vertice1].x_ord;
				y1 = polyhedron->vertices[vertice1].y_ord;
				z1 = polyhedron->vertices[vertice1].z_ord;
				normal1 = 	polyhedron->vertices[vertice1].normal;

				x2 = polyhedron->vertices[vertice2].x_ord;
				y2 = polyhedron->vertices[vertice2].y_ord;
				z2 = polyhedron->vertices[vertice2].z_ord;
				normal2 = 	polyhedron->vertices[vertice2].normal;

				if (y1 != y2)
				{
				  	/* assign the values to a new node	*/
					new_edge_node = assign_values(x1,x2, y1, y2, z1, z2, normal1, normal2, &y_min);
					/* insert the node into the edge table	*/
					edge_table[y_min] = insert_new_node(edge_table[y_min], new_edge_node);
				}
				k++;
			}
			/* do a scanline for the polygon	*/
			scanline(polyhedron->faces[j]);
		}
	}
	return ;
}


/*==============================================================================
*---perspective_projection(): calculate the perpective projection of a vertice.
*===============================================================================
*/

void perspective_projection(Point *point)
{
	point->x_ord = (0.5 * pixel_width) + ((point->x_ord * focal_length)/  point->z_ord) ;
	point->y_ord=  (0.5 * pixel_height) +((point->y_ord * focal_length)/  point->z_ord) ;

	return ;
}


/*==============================================================================
*---assign_values(): create a new bucket with the corresponding information 
*------------------- about the data required to create an edge.
*===============================================================================
*/

Bucket* assign_values(float x1, float x2, float y1, float y2, float z1, float z2,Vector n1, Vector n2, int *y_min)
{
	Bucket*		edge_table_node;	/* the new node	*/


	edge_table_node  = malloc(sizeof(Bucket));

	if (y2 > y1) 
	{
	  	/* information for the edge of a polygon	*/
		edge_table_node->y_max = y2;		
		*y_min = round(y1);
		edge_table_node->x_min = x1;
		/*information for the z buffer data			*/
		edge_table_node->z_data.z = -z1;
		edge_table_node->z_data.z_min = -z1;
		edge_table_node->z_data.z_max = -z2;
		edge_table_node->z_data.y_min = y1;
		/* information the normal of the edge		*/
		edge_table_node->normal_data.normal = n1;
		edge_table_node->normal_data.normal_min = n1; 
		edge_table_node->normal_data.normal_max = n2;
	}
	else  
	{
	  	/* likewise for the alternative				*/
		edge_table_node->y_max = y1;
		*y_min = round(y2);
		edge_table_node->x_min = x2;
		edge_table_node->z_data.z = -z2;
		edge_table_node->z_data.z_min = -z2;
		edge_table_node->z_data.z_max = -z1;
		edge_table_node->z_data.y_min = y2;
		edge_table_node->normal_data.normal = n2; 
		edge_table_node->normal_data.normal_min = n2; 
		edge_table_node->normal_data.normal_max = n1;
	}
	
	/* the increment value for the scanline process	*/
	if (x2 ==x1) {
		edge_table_node->delta_x = 0;
	}
	else {
		edge_table_node->delta_x = (x2 - x1) / (y2 -y1);
	}
	
	edge_table_node->next = NULL;

	return edge_table_node;
}


/*==============================================================================
*---insert_new_node(): inserts a new node into the edge table or the active
*--------------------  edge table, in sorted order of the x_min then the increment.
*===============================================================================
*/

Bucket* insert_new_node(Bucket* head, Bucket* new_node)
{
	Bucket	*prv_ptr	= NULL;
	Bucket	*curr_ptr	= head;
	Bucket	*new_ptr	= new_node;
	
	while ((curr_ptr != NULL) && (new_ptr->x_min > curr_ptr->x_min))
	{
		prv_ptr = curr_ptr;
		curr_ptr = curr_ptr->next;
	}

	if (curr_ptr != NULL)
	{
		if (new_ptr->x_min == curr_ptr->x_min)
		{
			if (new_ptr->delta_x >= curr_ptr->delta_x)
			{
				prv_ptr = curr_ptr;
				curr_ptr = curr_ptr->next;
			}
		}
	}

	if (prv_ptr == NULL)
	{
		new_ptr->next = head;
		head = new_ptr;
	}

	else
	{
		prv_ptr->next = new_ptr;
		new_ptr->next = curr_ptr;
	}
	
	return head;
}


/*==============================================================================
*---scanline(): for the given polygon, scans 1 line at a time and fills the
*-------------  value of each pixel WITHIN the lines of that polygon
*===============================================================================
*/

void scanline( Polygon polygon)
{
	int		scanline	= 0;
	Bucket* aet 		= NULL;	/*active edge table	*/
	Bucket* new_ptr;


	/* skip over any empty entries for the polygon in the edge table	*/
	while (edge_table[scanline] == NULL )
	{
		scanline++;
	}

	/* while the active edge table is not null or the edge table		*/
	while ( aet !=NULL || edge_table[scanline] != NULL )
	{
	  	/* insert the node into the edge table	*/
		while (edge_table[scanline] != NULL)
		{
			new_ptr = edge_table[scanline];
			edge_table[scanline] = edge_table[scanline]->next;
			new_ptr->next = NULL;
			aet = insert_new_node(aet,new_ptr);
		}
		/* delete any nodes that are not used any more	*/
		while (delete_old_aet_nodes(&aet, scanline)== 0 && aet != NULL);
		fill_scanline(aet, scanline,polygon ); /* fill the pixels at the scanline	*/	
		scanline++;	/* increment the scanline	*/
		aet = update_aet(aet, scanline); /* update the active edge table	*/
	}

	return ;
}


/*==============================================================================
*---fill_scanline(): fills the scanline with the correct pixel values, within
*------------------  the lines.
*===============================================================================
*/

void fill_scanline(Bucket* aet, int scanline, Polygon polygon)
{
	int		parity 		= TRUE;	/* parity to fill the polygon when this is odd	*/
	int		x_left, x_right;
	int 	j; 
	Bucket	*curr_ptr 	= aet;
	Bucket	*prv_ptr 	= NULL;
	float 	z; 
	Vector	normal;


	while (curr_ptr != NULL )	/* while the aet is not NULL	*/
	{
		if (parity == FALSE)	/*..and while the parity is odd	*/
		{
			x_left = round(prv_ptr->x_min); /* the left bound of the polygon */
			x_right = round(curr_ptr->x_min);/* the right bound				 */
			z = prv_ptr->z_data.z;			/* the z_buffer value			 */
			normal = prv_ptr->normal_data.normal;/* the normal of the edge	 */

			/* while in between the bounds		*/
			for (j = x_left; j <= x_right-1; j++)
			{
			  	/* if z is closer then then the z already in the buffer..*/
				if ( z < display[j][scanline].z)
				{
					fill_pixel( j ,scanline, z, polygon, normal); /*..fill the pixel	*/
			 	}
				/* update the z value	*/
				z +=  (curr_ptr->z_data.z - prv_ptr->z_data.z)/(curr_ptr->x_min - prv_ptr->x_min );
				/* update the normal value	*/
				normal = interpolate_normal(curr_ptr->normal_data.normal, prv_ptr->normal_data.normal,curr_ptr->x_min, prv_ptr->x_min, j);
			}
			parity = TRUE;/* cahnge the parity so the next line is not filled */ 
		}

		else
		{
			prv_ptr = curr_ptr;	/* change the parity and get the new edges	*/
			curr_ptr= curr_ptr->next;
			parity = FALSE;
		}
	}

	return;
}


/*==============================================================================
*---delete_old_aet_nodes(): deletes the nodes out of the active edge table
*===============================================================================
*/

int delete_old_aet_nodes(Bucket* *aet, int scanline)
{
	Bucket* 	prv_ptr ;
	Bucket* 	curr_ptr ;
	Bucket* 	tmp_ptr 	= NULL;


	if ( round((*aet)->y_max) == scanline )/* if the y_max value is the same as the */
	{									   /* scanline, delete it					*/
		tmp_ptr = *aet;
		*aet= (*aet)->next;
		free(tmp_ptr);
		return 0;
	}
	else 
	{
		prv_ptr = *aet;
		curr_ptr = (*aet)->next;

		while (curr_ptr != NULL && round(curr_ptr->y_max) > scanline)
		{
			prv_ptr = curr_ptr;
			curr_ptr = curr_ptr->next;
		}

		if (curr_ptr != NULL)
		{
			tmp_ptr = curr_ptr;
			prv_ptr->next = curr_ptr->next;
			free(tmp_ptr);
			return 0;
		}
	}

	return -1;
}


/*==============================================================================
*---update_aet(): updates the edge table, with the new bounds for filling, and
*---------------  also for z buffering(depth) and the new normal value.
*===============================================================================
*/

Bucket* update_aet(Bucket* aet, int scanline)
{
	Bucket*		head		= aet;
	Bucket*		curr_ptr	= head;


	while ( curr_ptr != NULL)
	{
		curr_ptr->x_min += curr_ptr->delta_x;/* increment the edge with the inverse gradient*/
		/*update the new z value */
		curr_ptr->z_data.z += (curr_ptr->z_data.z_max - curr_ptr->z_data.z_min)/(curr_ptr->y_max - curr_ptr->z_data.y_min);
		/* update the new normal */
		curr_ptr->normal_data.normal = interpolate_normal(curr_ptr->normal_data.normal_max, curr_ptr->normal_data.normal_min,
													curr_ptr->y_max, curr_ptr->z_data.y_min, scanline);
		curr_ptr= curr_ptr->next;
	}
	return head;
}


/*==============================================================================
*---calc_normal(): calculates the normal given 3 points on the plane(polygon)
*===============================================================================
*/

Vector calc_normal(Point point1, Point point2, Point point3)
{
	Vector 		normal, A, B;
	float		x1, y1, z1;
	float		x2, y2, z2;
	float		x3, y3, z3;

	x1 =	point1.x_ord;	/* the 1st vertice	*/
	y1 =	point1.y_ord;
	z1 = 	point1.z_ord;
	
	x2 =	point2.x_ord;	/* the 2nd vertice	*/
	y2 =	point2.y_ord;
	z2 =	point2.z_ord;

	x3 =	point3.x_ord;	/* the 3rd vertice	*/
	y3 =	point3.y_ord;
	z3 = 	point3.z_ord;

	A.a = x1 - x2;	/* calculate the vectors	*/
	A.b = y1 - y2;
	A.c = z1 - z2;

	B.a = x2 - x3;
	B.b = y2 - y3;
	B.c = z2 - z3;

	normal.a = A.b * B.c - A.c * B.b; /* calculate the normal */
	normal.b = A.c * B.a - A.a * B.c;
	normal.c = A.a * B.b - A.b * B.a;

	normal = normalise(normal);/* normalise the normal(unit vector)*/

	return normal;
}


/*==============================================================================
*---normalise(): normalise a vector (calculate the unit vector).
*===============================================================================
*/

Vector normalise(Vector normal)
{
	float	length; /* the length of the vector */

	length = sqrt(pow(normal.a,2.0) + pow(normal.b,2.0) + pow(normal.c,2.0));

	normal.a = normal.a / length;
	normal.b = normal.b / length;
	normal.c = normal.c / length;

	return normal;
}


/*==============================================================================
*---fill_pixel(): fills the pixel with the correct value according to ambient 
*---------------- light, lambertian shading, and specular reflection.
*===============================================================================
*/

void fill_pixel(int j,int  scanline,float z, Polygon polygon, Vector N)
{
	float red		= 0;
	float green		= 0;
	float blue		= 0;
	Vector			 L, R, V;
	float			dot_productNL, dot_productRV;/* dot products for specular reflection */
	Light_Source* 	light_source;


	light_source = light_source_list->first;

	/* the vector of the viewer */
	V.a = - (j * z) / focal_length;
	V.b = - (scanline * z) / focal_length;
	V.c = - z;
	V = normalise(V);/* normalise the viewer normal */

	while (light_source != NULL) /* while the light source list isnt NULL */
	{	
	  	/* the light source vector */
		L.a = light_source->x_loc;
		L.b = light_source->y_loc;
		L.c = light_source->z_loc;
        L = normalise(L); /* normalise the light source vector */

		/* calculate the dot product of L.N */
		dot_productNL = ((N.a * L.a) + (N.b * L.b) +(N.c * L.c));
	
		/* the vector of the reflected light source (already normalised)*/
		R.a = 2 * N.a * dot_productNL - L.a;
		R.b = 2 * N.b * dot_productNL - L.b;
		R.c = 2 * N.c * dot_productNL - L.c;

		/* dot product of R.V */
		dot_productRV = ((R.a * V.a) + (R.b * V.b) +(R.c * V.c));

		if (dot_productNL > 0)
		{
		  	/* the specular reflection equation for each colour */
			red += light_source->intensity *  
			  	   ((polygon.refl.red * dot_productNL)  + (KS * pow(dot_productRV, PN)));
			green += light_source->intensity * 
			  		((polygon.refl.green * dot_productNL) + (KS * pow(dot_productRV, PN)));
			blue += light_source->intensity * 
			  		((polygon.refl.blue* dot_productNL) + (KS * pow(dot_productRV,PN)));
		}

		light_source = light_source->next;
	}

	/* the ambient light is then added */
	red += ambient_light * polygon.refl.red;
	green += ambient_light * polygon.refl.green;
	blue += ambient_light * polygon.refl.blue;

	/* if any of the colour values are greater than max colour then, assign max colour */
	if (red > 255) {
		display[j][scanline].colour.red = 255;
	}
	else {
		display[j][scanline].colour.red = (int) red;
	}

	if (green > 255) {
		display[j][scanline].colour.green = 255;
	}
	else {
		display[j][scanline].colour.green =(int) green;
	}

	if (blue > 255) {
		display[j][scanline].colour.blue = 255;
	}	
	else {
		display[j][scanline].colour.blue = (int) blue;
	}

	/* assign the depth of the pixel */
	display[j][scanline].z = z;
	
	return;
}


/*==============================================================================
*---initialise_polygon_table(): initialise the polygon ID  table.
*===============================================================================
*/

Identity** initialise_polygon_table(int num_vertices)
{
	Identity** poly_table = calloc(num_vertices, sizeof(Identity));

	return poly_table;
}


/*=============================================================================
*---insert_poly_table(): insert a new element to the head of the polygon table
*==============================================================================
*/

Identity* insert_poly_table(Identity* head, int identity)
{
	Identity*	newn; 


	newn = malloc(sizeof(Identity));
	newn->polyID = identity;
	newn->next = NULL;

	newn->next = head;
	head = newn;
	
	return head;
}


/*=============================================================================
*--- summate_vectors(): summates the given vectors and returns the first one
*==============================================================================
*/

Vector summate_vectors( Vector vertex_normal, Vector face_normal)
{

vertex_normal.a += face_normal.a;
vertex_normal.b += face_normal.b;
vertex_normal.c += face_normal.c;

return vertex_normal;

}


/*=============================================================================
*---interpolateNormal(): can calculate the next normal for either x or y
*==============================================================================
*/

Vector interpolate_normal(Vector n_max, Vector n_min, int axis2, int axis1, int axis)
{
	Vector normal;

	normal.a = n_min.a + ((axis - axis1)*(n_max.a - n_min.a))/(axis2 - axis1);
	normal.b = n_min.b + ((axis - axis1)*(n_max.b - n_min.b))/(axis2 - axis1);
	normal.c = n_min.c + ((axis - axis1)*(n_max.c - n_min.c))/(axis2 - axis1);

	return normal;
}

