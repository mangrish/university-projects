/* proj.c
 *
 * As it stands, this program reads a scene definition from stdin in
 * accordance with the project specification, as well as reading in each as
 * many objects as included in the scene. Each object is assumed to be in its
 * own OFF file.
 */


#include "proj2.h"

int main(int argc, char* argv[])
{
    Scene*          scene = NULL;
		


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


		if (argc == 4)
		{	focal_length = atoi(argv[1]);
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


/* read_scene_defn()
 *
 * Reads a scene definition, currently line by line from stdin; 
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
						scene->ambient_light = 0;

            if (!strcmp(input_word, AMBIENT_IDENTIFIER)) {

                sscanf(input_line, "%s %f", input_word, &scene->ambient_light);
            }
        }
    }
}


/* get_polyhedron()
 *
 * Reads a polyhedron from an OFF file specified in the scene definition.
 */

Polyhedron* get_polyhedron(const char* input_line)
{
    Polyhedron*     polyhedron = NULL;
    char            input_word[MAX_LENGTH_INPUT_LINE] = {0};
    char            off_file_name[MAX_LENGTH_INPUT_LINE] = {0};
    int             num_args = 0;
		int 						i;
		

    if ((num_args = sscanf(input_line, "%s %s %f %f %f %f %f %f %f %f %f",
        input_word, off_file_name, &scale_x, &scale_y, &scale_z, &rot_x, &rot_y, &rot_z,
				&trans_x, &trans_y, &trans_z)) != 11) 
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


/* get_lightsource()
 *
 * Reads a description of a light source from the scene description file.
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


void print_ppm(void )
{

	int i,j;

	printf("P3\n");
	printf("%i %i\n", pixel_width, pixel_height);
	printf("255\n" );

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



Pixel** initialise_array( void)
{
	int i, j;
	Pixel** pixel = calloc(pixel_width, sizeof(Pixel*));
	for (i=0; i<= pixel_height-1; i++)
	{
		pixel[i] = calloc(pixel_height,sizeof(Pixel));
		for (j=0; j <= pixel_height-1; j++)
		{
		  pixel[i][j].colour.red = 0;
			pixel[i][j].colour.green = 0;
			pixel[i][j].colour.blue =0;
			pixel[i][j].colour.alpha = 0;
			pixel[i][j].z = 100000;
		}
	}
	return pixel;
}


Bucket** initialise_edge_table(void)
{
	Bucket** bucket = calloc(pixel_height, sizeof(Bucket*));

	return bucket;
}



void usage(char *progname)
{
	  fprintf( stderr, "Usage: %s [#focal point] [#pixel width] [#pixel height] < filename.off \n", progname );

		  exit( EXIT_FAILURE );
}



void transform(Point *point)
{

	float xrotationMatrix[4][4];
  float yrotationMatrix[4][4];
  float zrotationMatrix[4][4];


	point->x_ord = point->x_ord * scale_x;
	point->y_ord = point->y_ord * scale_y;
	point->z_ord = point->z_ord * scale_z;
/*-----------------------------------------------*/			
	xrotationMatrix[0][0] = 1;
        xrotationMatrix[0][1] = 0;
        xrotationMatrix[0][2] = 0;
        xrotationMatrix[0][3] = 0;
        xrotationMatrix[1][0] = 0;
        xrotationMatrix[1][1] = (float) (cos(rot_x));
        xrotationMatrix[1][2] = (float) (sin(rot_x));
        xrotationMatrix[1][3] = 0;
        xrotationMatrix[2][0] = 0;
        xrotationMatrix[2][1] = -(float) (sin(rot_x));;
        xrotationMatrix[2][2] = (float) (cos(rot_x));
        xrotationMatrix[2][3] = 0;
        xrotationMatrix[3][0] = 0;
        xrotationMatrix[3][1] = 0;
        xrotationMatrix[3][2] = 0;
        xrotationMatrix[3][3] = 1;

	yrotationMatrix[0][0] = (float) (cos(rot_y));
        yrotationMatrix[0][1] = 0;
        yrotationMatrix[0][2] = -(float) (sin(rot_y));
        yrotationMatrix[0][3] = 0;
        yrotationMatrix[1][0] = 0;
        yrotationMatrix[1][1] = 1;
        yrotationMatrix[1][2] = 0;
        yrotationMatrix[1][3] = 0;
        yrotationMatrix[2][0] = (float) (sin(rot_y));
        yrotationMatrix[2][1] = 0;
        yrotationMatrix[2][2] = (float) (cos(rot_y));
        yrotationMatrix[2][3] = 0;
        yrotationMatrix[3][0] = 0;
        yrotationMatrix[3][1] = 0;
        yrotationMatrix[3][2] = 0;
        yrotationMatrix[3][3] = 1;

	zrotationMatrix[0][0] = (float) (cos(rot_z));
        zrotationMatrix[0][1] = (float) (sin(rot_z));
        zrotationMatrix[0][2] = 0;
        zrotationMatrix[0][3] = 0;
        zrotationMatrix[1][0] = -(float) (sin(rot_z));
        zrotationMatrix[1][1] = (float) (cos(rot_z));
        zrotationMatrix[1][2] = 0;
        zrotationMatrix[1][3] = 0;
        zrotationMatrix[2][0] = 0;
        zrotationMatrix[2][1] = 0;
        zrotationMatrix[2][2] = 1;
        zrotationMatrix[2][3] = 0;
        zrotationMatrix[3][0] = 0;
        zrotationMatrix[3][1] = 0;
        zrotationMatrix[3][2] = 0;
        zrotationMatrix[3][3] = 1;


	rotate(point, xrotationMatrix);
  rotate(point, yrotationMatrix);
  rotate(point, zrotationMatrix);

/*------------------------------------------------*/
	point->x_ord = point->x_ord + trans_x;
	point->y_ord = point->y_ord + trans_y;
	point->z_ord = point->z_ord + trans_z;

	return;
}



void rotate(Point* point, float transMatrix[4][4])
{
	int j,k;
	float acc;
	float xyzVector[4];
	float xyzVector2[4];

  	xyzVector[0] = point->x_ord;
    xyzVector[1] = point->y_ord;
    xyzVector[2] = point->z_ord;
    xyzVector[3] = 1;

		for ( j = 0 ; j < 4 ; j++)
    {
			acc = 0;
      for ( k = 0 ; k < 4 ; k++)
			{
				acc += xyzVector[k] * transMatrix[k][j];
			}
			xyzVector2[j] = acc;
		}
		
		point->x_ord = xyzVector2[0];
    point->y_ord = xyzVector2[1];
    point->z_ord = xyzVector2[2];
}



void build_edge_table(Scene* scene)
{
	int 					i, j, k, l;
	float 				x1, x2, y1, y2, z1, z2;
	float					y_max, x_min, delta_x;
	float					z_min, z_max, zy_min;
	int 					y_min;
	Bucket*				new_edge_node;
	Polyhedron* 	polyhedron;
	Light_Source_List*	light_source_list;
	float					ambient_light;

	light_source_list = scene->light_sources;
	ambient_light = scene->ambient_light;

	for (polyhedron = scene->polyhedrons->first; polyhedron; polyhedron = polyhedron->next)
	{
		for (i = 0; i<= polyhedron->num_vertices-1; i++)
		{
			perspective_projection(&polyhedron->vertices[i]);
		}
		
		for (j = 0; j<= polyhedron->num_faces-1; j++)
		{
			calculate_normal(polyhedron->faces[j])
		}
		for (j = 0; j<= polyhedron->num_faces-1; j++)
		{

			k = 1;
			for (l = 0 ; l <= polyhedron->faces[j].num_vertices-1; l++)
			{
				if (l==(polyhedron->faces[j].num_vertices-1))
				{
					k = 0;
				}
				x1 = polyhedron->vertices[polyhedron->faces[j].vertices[l]].x_ord;
				y1 = polyhedron->vertices[polyhedron->faces[j].vertices[l]].y_ord;
				z1 = polyhedron->vertices[polyhedron->faces[j].vertices[l]].z_ord;

				x2 = polyhedron->vertices[polyhedron->faces[j].vertices[k]].x_ord;
				y2 = polyhedron->vertices[polyhedron->faces[j].vertices[k]].y_ord;
				z2 = polyhedron->vertices[polyhedron->faces[j].vertices[k]].z_ord;
				
				if (y1 != y2)
				{
					assign_values(x1,x2, y1, y2, z1, z2, &y_min, &y_max, &x_min, &delta_x, &z_min, &z_max, &zy_min,);
					find_normals(polyhedron->faces&normal_min, &normal_max);
					new_edge_node = create_new_edge_node(y_max, x_min, delta_x, z_min, z_max, zy_min, normal_min, normal_max);
					edge_table[y_min] = insert_new_node(edge_table[y_min], new_edge_node);
				}
				k++;
				
			}
			scanline(polyhedron->faces[j], light_source_list, ambient_light);
		}
	}
	return ;
}





void perspective_projection(Point *point)
{

	point->x_ord = (0.5 * pixel_width) + ((point->x_ord * focal_length)/  point->z_ord) ;
	point->y_ord=  (0.5 * pixel_height) +((point->y_ord * focal_length)/  point->z_ord) ;

	return ;
}





void assign_values(float x1, float x2, float y1, float y2, float z1, float z2, int *y_min, float *y_max, float *x_min, float *delta_x, float *z_min, float *z_max, float *zy_min)
{
	float fraction;
	float min_y;

	if (y2 > y1) { 
		*y_max = y2;
		min_y = y1;
		*x_min =x1;
		*z_min = -z1;
		*z_max = -z2;
		*zy_min = y1;
	}
	else { 
		*y_max = y1;
		min_y = y2;
		*x_min =x2;
		*z_min = -z2;
		*z_max = -z1;
		*zy_min = y2;
	}

	fraction =  min_y - ((int) min_y);
	if (fraction >= 0.5) {
		*y_min = (int) min_y + 1;
	}
	else {
		*y_min = (int) min_y;
	}
	

	if (x2 ==x1) {
		*delta_x = 0;
	}
	else {
		*delta_x = (x2 - x1) / (y2 -y1);
	}

	return;
}




Bucket* create_new_edge_node(float y_max,float x_min, float delta_x, float z_min, float z_max, float zy_min)
{
	Bucket* edge_table_node = malloc(sizeof(Bucket));

	if (edge_table_node != NULL)
	{
		edge_table_node->y_max = y_max;
		edge_table_node->x_min = x_min;
		edge_table_node->delta_x = delta_x;
		edge_table_node->z = z_min;
		edge_table_node->z_min =	z_min; 
		edge_table_node->z_max = z_max;
		edge_table_node->zy_min = zy_min;
		edge_table_node->normal = normal_min;
		edge_table_node->normal_min = normal_min;
		edge_table_node->normal_max = normal_max;
		edge_table_node->next = NULL;
	}

	return edge_table_node;
}





Bucket* insert_new_node(Bucket* head, Bucket* new_node)
{
	Bucket *prv_ptr = NULL;
	Bucket *curr_ptr = head;
	Bucket *new_ptr = new_node;
	
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




void scanline( Polygon polygon, Light_Source_List* light_source_list, float ambient_light)
{
	int scanline = 0;
	Bucket* aet = NULL;
	Bucket* new_ptr;


	while (edge_table[scanline] == NULL )
	{
		scanline++;
	}

		while ( aet !=NULL || edge_table[scanline] != NULL )
		{
			while (edge_table[scanline] != NULL)
			{
				new_ptr = edge_table[scanline];
				edge_table[scanline] = edge_table[scanline]->next;
				new_ptr->next = NULL;
				aet = add_aet_entry(aet,new_ptr, scanline);
			}
			while (delete_old_aet_nodes(&aet, scanline)== 0 && aet != NULL);
			fill_scanline(aet, scanline,polygon, light_source_list, ambient_light);	
			scanline++;
			aet = update_aet(aet);
		}

	return ;
}




void fill_scanline(Bucket* aet, int scanline, Polygon polygon, Light_Source_list* light_source_list, float ambientlight)
{

	int parity = TRUE;
	int	x_left, x_right;
	int j; 
	Bucket *curr_ptr = aet;
	Bucket  *prv_ptr = NULL;
	float z; 
	Vector normal;

	while (curr_ptr != NULL )
	{
		if (parity == FALSE)
		{

			if ( (prv_ptr->x_min - ((int) prv_ptr->x_min)) >= 0.5) 
			{
				x_left = (int) prv_ptr->x_min + 1;
			}
			else
			{
				x_left = (int) prv_ptr->x_min;
			}
			if ( (curr_ptr->x_min - ((int) curr_ptr->x_min)) >=0.5)
			{
				x_right = (int) curr_ptr->x_min + 1;
			}
			else
			{
				x_right = (int) curr_ptr->x_min;
			}
			z = prv_ptr->z;
			normal = nomralise(normal);

			for (j = x_left; j <= x_right-1; j++)
			{
				if ( z < display[j][scanline].z)
				{
					fill_pixel( j, scanline, z, polygon,light_source_list, ambient_light, normal)
				}					
				z +=  (curr_ptr->z - prv_ptr->z)/(curr_ptr->x_min - prv_ptr->x_min );
				normal = calc_normal(prv_ptr->nomral, curr_ptr->normal, Prv_ptr->x_min, curr_ptr->x_min, j)
			}
			parity = TRUE; 
		}
		else
		{
			prv_ptr = curr_ptr;
			curr_ptr= curr_ptr->next;
			normal = prv_ptr->normal;
			parity = FALSE;
		}

	}
	return;
}


void fill_pixel(int j,int  scanline,float z, Polygon polygon, Light_Source_List* light_source_list,float ambient_light, Vector normal)
{
	float red=0;
	float green = 0;
	float blue = 0;
	Light_Source* light_source;

	light_source = light_source_list->first;

	while (!light_source)
	{	
		L.a = light_source->x_loc;
		L.b = light_source->y_loc;
		L.c = light_source->z_loc;

		L = normalise(L);

		dot_product = normal.a * L.a + normal.b * L.b +  normal.c * L.c;

		if (dot_product > 0)
		{
			red += polygon.colour.red * light_source->intensity * dot_product;
			resultGreen += polygon.colour.green  light_source->intensity * dot_product;
			resultBlue += polygon.colour.blue * light_source->intensity * dot_product;
		}
		light_source = light_source->next;
	}

	red += ambient_light * polygon.refl.red
	green += ambient_light * polygon.refl.green;
	blue += ambient_light * polygon.refl.blue;


	if (red > 255)
	{
		display[j][scanline].colour.red = 255;
	}
	else
	{
		display[j][scanline].colour.red = (int) red;
	}

	if (green > 255)
	{
		display[j][scanline].colour.green = 255;
	}
	else
	{
		display[j][scanline].colour.green =(int) green;
	}

	if (blue > 255)
	{
		display[j][scanline].colour.blue = 255;
	}	
	else
	{
		display[j][scanline].colour.blue = (int) blue;
	}

	display[j][scanline].z = z;
	
	return;
}




Bucket* add_aet_entry( Bucket* head, Bucket* newn, int scanline)
{
	Bucket *prv_ptr = NULL;
	Bucket *curr_ptr = head;
	Bucket *new_ptr = newn;

	while (curr_ptr != NULL && new_ptr->x_min > curr_ptr->x_min)
	{
		prv_ptr = curr_ptr;
		curr_ptr= curr_ptr->next;
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



int delete_old_aet_nodes(Bucket* *aet, int scanline)
{
	Bucket* prv_ptr ;
	Bucket* curr_ptr ;
	Bucket* tmp_ptr = NULL;

	if ( (*aet)->y_max <= scanline )
	{
			tmp_ptr = *aet;
			*aet= (*aet)->next;
			free(tmp_ptr);
			return 0;
	}
	else 
	{
		prv_ptr = *aet;
		curr_ptr = (*aet)->next;

		while (curr_ptr != NULL && curr_ptr->y_max >= scanline)
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



Bucket* update_aet(Bucket* aet)
{
	Bucket* head = aet;
	Bucket* curr_ptr = head;
	while ( curr_ptr != NULL)
	{
		curr_ptr->x_min = curr_ptr->x_min + curr_ptr->delta_x;
		curr_ptr->z += (curr_ptr->z_max - curr_ptr->z_min)/(curr_ptr->y_max - curr_ptr->zy_min);
		curr_ptr= curr_ptr->next;
	}
	return head;
}



Vector normalise(Vector normal)
{
	Vector newNormal;
	float length;

	length = sqrt(pow(normal.a,2.0)
		+ pow(normal.b,2.0)
		+ pow(normal.c,2.0));

	newNormal.a = normal.a/length;
	newNormal.b = normal.b/length;
	newNormal.c = normal.c/length;

	return newNormal;
}

void updateNormals(int y)
{
	Bucket *this;

	this = aET;

	while( this != NULL )
	{
		/* find the next normal */
		this->normal = interpolateNormal(this->normalMin,	
		    this->normalMax, this->ymin, this->ymax, y);
		this = this->next;
	}
}

/*
 * interpolateNormal() - Interpolates a normal vector.
 */
Vector interpolateNormal(Vector nMin, Vector nMax, int x1, int x2, int x)
{
	Vector norm;

	norm.a = nMin.a + (x - x1)*(nMax.a - nMin.a)/(x2 - x1);
	norm.b = nMin.b + (x - x1)*(nMax.b - nMin.b)/(x2 - x1);
	norm.c = nMin.c + (x - x1)*(nMax.c - nMin.c)/(x2 - x1);

	return norm;
}


Vector findNormalVertex(int thisVertex, Polygon* polygon)
{
	Vector normal;
	int i, j;

	normal.a = 0;
	normal.b = 0;
	normal.c = 0;

	/* use brute force to find the adjacent faces */
	for (i = 0 ; i < polyhedron->num_faces; i++)
	{
	    	for (j = 0 ; j < polyhedron->faces[i].num_vertices;j++)
	    	{
	    		if (thisVertex == polyhedron->faces[i].vertices[j])
	    		{
				/* add the normal */
	    			normal.a += polyhedron->faces[i].normal.a;
	    			normal.b += polyhedron->faces[i].normal.b;
	    			normal.c += polyhedron->faces[i].normal.c;
				break;
	    		}
	    	}
	}    

	return normal;
}	

