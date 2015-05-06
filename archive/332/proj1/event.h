/*
 *	Function prototypes for the "core" of the simulator.
 *
 *	Author: Gary Eddy, 1996
 *	Revision History: Paolo Busetta, March 1997 - added comments.
 */

#include "disk.h"

void	init_event(int files, char **filenames);
int	simulation(void (*schedule)(int, int *, int *), 
		   void (*request)(int, int, ReadOrWrite));
