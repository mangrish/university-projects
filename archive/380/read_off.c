/* read_off.c
 *
 * Parses OFF files; refer to the project specification for references for
 * details of this format.
 */


#include "read_off.h"


/* read_off()
 *
 * Parses a polyhedron from the specified OFF file.
 *
 * Note that apparently the OFF specification allows for comments ('#...') to
 * appear at any place, as well as for each 'line' to include line breaks.
 * This implementation will not handle all permutations of these allowances;
 * notably comments containing numbers are not handled except on face lines.
 * Various measures could be taken to improve further the robustness of this
 * parser, but most OFF files are fairly well behaved.
 *
 * This function contains debugging output that you should remove once you
 * understand how it works.
 */

Polyhedron* read_off(const char* off_file_name)
{
    Polyhedron*     polyhedron = NULL;
    FILE*           off_file = NULL;
    char            input_line[MAX_LENGTH_INPUT_LINE] = {0};
    char*           input_line_ptr = NULL;
    int             i = 0, j = 0;
    float           tmp_red = 0, tmp_green = 0, tmp_blue = 0, tmp_alpha = 0;


    if (!(polyhedron = calloc(1, sizeof(Polyhedron)))) {
        printf("read_off: Cannot allocate storage for type `%s'.\n",
                "Polyhedron");
    }

    if (!(off_file = fopen(off_file_name, "r"))) {
        printf("read_off: Cannot open OFF file `%s'.\n", off_file_name);
    }

    if (!(fgets(input_line, MAX_LENGTH_INPUT_LINE, off_file))) {
        printf("read_off: Error reading OFF header from file `%s'.\n",
                off_file_name);
    }

    get_line(off_file, input_line);

    sscanf(input_line, "%i %i %i",
            &polyhedron->num_vertices,
            &polyhedron->num_faces,
            &polyhedron->num_edges);

    /* Now that the dimensions of the polyhedron are known, data storage can
     * be allocated.
     */
    
    if (!(polyhedron->vertices =
            calloc(polyhedron->num_vertices, sizeof(Point)))) {
        printf("read_off: Cannot allocate storage for type `%s'.\n", "Point");
    }

    if (!(polyhedron->faces =
            calloc(polyhedron->num_faces, sizeof(Polygon)))) {
        printf("read_off: Cannot allocate storage for type `%s'.\n", "Point");
    }


    /* And now the data can be read in, vertices first and then faces.
     */

    for (i = 0; i < polyhedron->num_vertices; i++) {
        get_line(off_file, input_line);

        sscanf(input_line, "%f %f %f",
                &polyhedron->vertices[i].x_ord,
                &polyhedron->vertices[i].y_ord,
                &polyhedron->vertices[i].z_ord);
    }

    for (i = 0; i < polyhedron->num_faces; i++) {

        /* Read in the number of vertices for this face.
         */

        get_line(off_file, input_line);

        sscanf(input_line, "%i", &polyhedron->faces[i].num_vertices);

        /* Allocate space for the vertices.
         */

        if (!(polyhedron->faces[i].vertices =
                calloc(polyhedron->faces[i].num_vertices, sizeof(int)))) {
            printf("read_off: Cannot allocate storage for type `%s'.\n", "int");
        }

        /* Read in the numbers of the vertices for this face. At this point we
         * switch from processing 'input_line' to using 'input_line_ptr' -
         * this is necessary because sscanf does not "eat" its input in the
         * same way that fscanf does. This is inconvenient, and required the
         * creation of 'next_token()' below. The added complexity creates yet
         * another possible point of failure for this program; although this
         * code has been informally tested, you are strongly encouraged to
         * satisfy yourself that it is adequate for your project.
         */

        input_line_ptr = input_line;
        for (j = 0; j < polyhedron->faces[i].num_vertices; j++) {
            input_line_ptr = next_token(input_line_ptr);

            sscanf(input_line_ptr, "%i", &polyhedron->faces[i].vertices[j]);
        }

        /* Read in the optional face colour information; if there is no colour
         * information specified then none will be read.
         *
         * Note that this surface property is really the reflectivity, not the
         * colour that the face will appear when rendered - the colour is
         * calculated later from this property and will depend on lighting
         * conditions. You are expected to handle this during the rendering
         * phase.
         *
         * To decide whether the colour is specified in integers or floats,
         * look for a '.' in the rest of the line; if there aren't any, then
         * the colours are integers, otherwise they are floats.
         */

        input_line_ptr = next_token(input_line_ptr);
        j = 0;
        if (strchr(input_line_ptr, '.')) {
            j = sscanf(input_line_ptr, "%f %f %f %f",
                    &tmp_red, &tmp_green, &tmp_blue, &tmp_alpha);

            polyhedron->faces[i].refl.red = tmp_red * 255;
            polyhedron->faces[i].refl.green = tmp_green * 255;
            polyhedron->faces[i].refl.blue = tmp_blue * 255;
            polyhedron->faces[i].refl.alpha = tmp_alpha * 255;
        } else {
            j = sscanf(input_line_ptr, "%i %i %i %i",
                    &polyhedron->faces[i].refl.red,
                    &polyhedron->faces[i].refl.green,
                    &polyhedron->faces[i].refl.blue,
                    &polyhedron->faces[i].refl.alpha);
        }

        if (j < 3) {

            /* There was no useful colour information for this face, so a
             * default of white is assigned.
             */

            polyhedron->faces[i].refl.red = 255;
            polyhedron->faces[i].refl.green = 255;
            polyhedron->faces[i].refl.blue = 255;
            polyhedron->faces[i].refl.alpha = 255;
        }


    }

    return polyhedron;
}


/* This function reads a line from the given file, storing it in the provided
 * array of characters. Comments (identified by a '#' character) are removed.
 */

void get_line(FILE* input_file, char* input_line)
{
    fgets(input_line, MAX_LENGTH_INPUT_LINE, input_file);

    /* Comments are stripped from this line by replacing the first '#' with a
     * '\0' thus effectively truncating the string.
     */

    if (strchr(input_line, '#')) {
        *(strchr(input_line, '#')) = '\0';
    }
}


/* This function finds the next token (in this case a number) in the given
 * string.
 */

char* next_token(char* string)
{
    while (isspace(*(string++)));

    while (!isspace(*(string++)));

    return string;
}
