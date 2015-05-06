/*
 *	Function prototypes for the disk simulator.
 *
 *	Author: Gary Eddy, 1996
 *	Revision History: Paolo Busetta, March 1997 - updated prototypes.
 */

#ifndef __DISK_H
#define __DISK_H

typedef enum {Read,Write} ReadOrWrite;

int	disk_idle();
void	print_results(int end_time);

void	fcfs_init(int startup, int pertrack, int disk_size, 
		int cache_size, double penalty, int disk_head);
void	cscan_init(int startup, int pertrack, int disk_size, 
		int cache_size, double penalty, int disk_head);
void	vscan_init(int startup, int pertrack, int disk_size, 
		int cache_size, double penalty, int disk_head);
void	sstf_init(int startup, int pertrack, int disk_size, 
		int cache_size, double penalty, int disk_head);

void	fcfs_request(int proc, int track, ReadOrWrite type);
void	fcfs_schedule(int current_time, int *delay, int *nextproc);

void	cscan_request(int proc, int track, ReadOrWrite type);
void	cscan_schedule(int current_time, int *delay, int *nextproc);

void	vscan_request(int proc, int track, ReadOrWrite type);
void	vscan_schedule(int current_time, int *delay, int *nextproc);

void	sstf_request(int proc, int track, ReadOrWrite type);
void	sstf_schedule(int current_time, int *delay, int *nextproc);

#endif
