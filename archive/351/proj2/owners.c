# include "/local/ingres/files/eqdef.h"
# 1 "tmp.c"
# 1 "owners.h" 1
# 1 "/usr/include/stdio.h" 1 3
#pragma ident	"@(#)stdio.h	1.69	98/07/13 SMI"	
# 1 "/usr/include/sys/feature_tests.h" 1 3
#pragma ident	"@(#)feature_tests.h	1.17	97/12/04 SMI"
# 1 "/usr/include/sys/isa_defs.h" 1 3
#pragma ident	"@(#)isa_defs.h	1.16	99/05/25 SMI"
# 218 "/usr/include/sys/isa_defs.h" 3
# 316 "/usr/include/sys/isa_defs.h" 3
# 15 "/usr/include/sys/feature_tests.h" 2 3
# 22 "/usr/include/stdio.h" 2 3
# 1 "/usr/include/sys/va_list.h" 1 3
#pragma ident	"@(#)va_list.h	1.11	97/11/22 SMI"
typedef void *__va_list;
# 23 "/usr/include/stdio.h" 2 3
# 1 "/usr/include/stdio_tag.h" 1 3
#pragma ident	"@(#)stdio_tag.h	1.3	98/04/20 SMI"
typedef struct __FILE  __FILE;
# 24 "/usr/include/stdio.h" 2 3
# 1 "/usr/include/stdio_impl.h" 1 3
#pragma ident	"@(#)stdio_impl.h	1.8	99/06/30 SMI"
typedef int	ssize_t;		 
# 36 "/usr/include/stdio_impl.h" 3
struct __FILE 	 
{
	ssize_t		_cnt;	 
	unsigned char	*_ptr;	 
	unsigned char	*_base;	 
	unsigned char	_flag;	 
	unsigned char	_file;	 
	unsigned	__orientation:2;  
	unsigned	__ionolock:1;	 
	unsigned	__filler:5;
};
# 25 "/usr/include/stdio.h" 2 3
typedef	__FILE FILE;
typedef unsigned int	size_t;		 
typedef	long long	__longlong_t;
typedef long		off_t;
typedef __longlong_t	off64_t;
typedef long		fpos_t;
typedef __longlong_t	fpos64_t;
extern FILE	__iob[20 ];
extern unsigned char	 _sibuf[], _sobuf[];
# 227 "/usr/include/stdio.h" 3
extern unsigned char	*_bufendtab[];
extern FILE		*_lastbuf;
# 257 "/usr/include/stdio.h" 3
extern int	remove(const char *);
extern int	rename(const char *, const char *);
extern FILE	*tmpfile(void);
extern char	*tmpnam(char *);
extern int	fclose(FILE *);
extern int	fflush(FILE *);
extern FILE	*fopen(const char *, const char *);
extern FILE	*freopen(const char *, const char *, FILE *);
extern void	setbuf(FILE *, char *);
extern void setbuffer(FILE *, char *, size_t);
extern int setlinebuf(FILE *);
extern int	setvbuf(FILE *, char *, int, size_t);
extern int	fprintf(FILE *, const char *, ...);
extern int	fscanf(FILE *, const char *, ...);
extern int	printf(const char *, ...);
extern int	scanf(const char *, ...);
extern int	snprintf(char *, size_t, const char *, ...);
extern int	sprintf(char *, const char *, ...);
extern int	sscanf(const char *, const char *, ...);
extern int	vfprintf(FILE *, const char *, __va_list);
extern int	vprintf(const char *, __va_list);
extern int	vsnprintf(char *, size_t, const char *, __va_list);
extern int	vsprintf(char *, const char *, __va_list);
extern int	fgetc(FILE *);
extern char	*fgets(char *, int, FILE *);
extern int	fputc(int, FILE *);
extern int	fputs(const char *, FILE *);
extern int	getc(FILE *);
extern int	getchar(void);
extern char	*gets(char *);
extern int	putc(int, FILE *);
extern int	putchar(int);
extern int	puts(const char *);
extern int	ungetc(int, FILE *);
extern size_t	fread(void *, size_t, size_t, FILE *);
extern size_t	fwrite(const void *, size_t, size_t, FILE *);
extern int	fgetpos(FILE *, fpos_t *);
extern int	fseek(FILE *, long, int);
extern int	fsetpos(FILE *, const fpos_t *);
extern long	ftell(FILE *);
extern void	rewind(FILE *);
extern void	clearerr(FILE *);
extern int	feof(FILE *);
extern int	ferror(FILE *);
extern void	perror(const char *);
extern int	__filbuf(FILE *);
extern int	__flsbuf(int, FILE *);
extern FILE	*fdopen(int, const char *);
extern char	*ctermid(char *);
extern int	fileno(FILE *);
# 358 "/usr/include/stdio.h" 3
extern FILE	*popen(const char *, const char *);
extern char	*cuserid(char *);
extern char	*tempnam(const char *, const char *);
extern int	getopt(int, char *const *, const char *);
extern int	getsubopt(char **, char *const *, char **);
extern char	*optarg;
extern int	optind, opterr, optopt;
extern int	getw(FILE *);
extern int	putw(int, FILE *);
extern int	pclose(FILE *);
extern int	fseeko(FILE *, off_t, int);
extern off_t	ftello(FILE *);
extern FILE	*fopen64(const char *, const char *);
extern FILE	*freopen64(const char *, const char *, FILE *);
extern FILE	*tmpfile64(void);
extern int	fgetpos64(FILE *, fpos64_t *);
extern int	fsetpos64(FILE *, const fpos64_t *);
extern int	fseeko64(FILE *, off64_t, int);
extern off64_t	ftello64(FILE *);
# 515 "/usr/include/stdio.h" 3
# 567 "/usr/include/stdio.h" 3
# 16 "owners.h" 2
# 1 "/usr/include/stdlib.h" 1 3
#pragma ident	"@(#)stdlib.h	1.44	98/01/22 SMI"	
typedef	struct {
	int	quot;
	int	rem;
} div_t;
typedef struct {
	long	quot;
	long	rem;
} ldiv_t;
typedef struct {
	long long	quot;
	long long	rem;
} lldiv_t;
typedef long	uid_t;			 
typedef long	wchar_t;
# 101 "/usr/include/stdlib.h" 3
# 117 "/usr/include/stdlib.h" 3
extern unsigned char	__ctype[];
extern double atof(const char *);
extern int atoi(const char *);
extern long int atol(const char *);
extern double strtod(const char *, char **);
extern long int strtol(const char *, char **, int);
extern unsigned long int strtoul(const char *, char **, int);
extern int rand(void);
extern void srand(unsigned int);
extern void *calloc(size_t, size_t);
extern void free(void *);
extern void *malloc(size_t);
extern void *realloc(void *, size_t);
extern void abort(void);
extern int atexit(void (*)(void));
extern void exit(int);
extern void _exithandle(void);
extern char *getenv(const char *);
extern int system(const char *);
extern void *bsearch(const void *, const void *, size_t, size_t,
	int (*)(const void *, const void *));
extern void qsort(void *, size_t, size_t,
	int (*)(const void *, const void *));
extern int abs(int);
extern div_t div(int, int);
extern long int labs(long);
extern ldiv_t ldiv(long, long);
extern int mbtowc(wchar_t *, const char *, size_t);
extern int mblen(const char *, size_t);
extern int wctomb(char *, wchar_t);
extern size_t mbstowcs(wchar_t *, const char *, size_t);
extern size_t wcstombs(char *, const wchar_t *, size_t);
extern double drand48(void);
extern double erand48(unsigned short *);
extern long jrand48(unsigned short *);
extern void lcong48(unsigned short *);
extern long lrand48(void);
extern long mrand48(void);
extern long nrand48(unsigned short *);
extern unsigned short *seed48(unsigned short *);
extern void srand48(long);
extern int putenv(char *);
extern void setkey(const char *);
extern void swab(const char *, char *, int);
extern int	mkstemp(char *);
extern int	mkstemp64(char *);
extern long a64l(const char *);
extern char *ecvt(double, int, int *, int *);
extern char *fcvt(double, int, int *, int *);
extern char *gcvt(double, int, char *);
extern int getsubopt(char **, char *const *, char **);
extern int  grantpt(int);
extern char *initstate(unsigned, char *, size_t);
extern char *l64a(long);
extern char *mktemp(char *);
extern char *ptsname(int);
extern long random(void);
extern char *realpath(const char *, char *);
extern char *setstate(const char *);
extern void srandom(unsigned);
extern int ttyslot(void);
extern int  unlockpt(int);
extern void *valloc(size_t);
extern int dup2(int, int);
extern char *qecvt(long double, int, int *, int *);
extern char *qfcvt(long double, int, int *, int *);
extern char *qgcvt(long double, int, char *);
extern char *getcwd(char *, size_t);
extern const char *getexecname(void);
extern char *getlogin(void);
extern int getopt(int, char *const *, const char *);
extern char *optarg;
extern int optind, opterr, optopt;
extern char *getpass(const char *);
extern char *getpassphrase(const char *);
extern int getpw(uid_t, char *);
extern int isatty(int);
extern void *memalign(size_t, size_t);
extern char *ttyname(int);
extern long long atoll(const char *);
extern long long llabs(long long);
extern lldiv_t lldiv(long long, long long);
extern char *lltostr(long long, char *);
extern long long strtoll(const char *, char **, int);
extern unsigned long long strtoull(const char *, char **, int);
extern char *ulltostr(unsigned long long, char *);
# 380 "/usr/include/stdlib.h" 3
# 17 "owners.h" 2
# 1 "/usr/include/strings.h" 1 3
#pragma ident	"@(#)strings.h	1.3	96/03/12 SMI"
# 1 "/usr/include/sys/types.h" 1 3
#pragma ident	"@(#)types.h	1.63	98/04/28 SMI"
# 1 "/usr/include/sys/machtypes.h" 1 3
#pragma ident	"@(#)machtypes.h	1.12	96/08/01 SMI"
typedef struct  _physadr_t { int r[1]; } *physadr_t;
typedef	struct	_label_t { long val[2]; } label_t;
typedef	unsigned char	lock_t;		 
# 24 "/usr/include/sys/types.h" 2 3
# 1 "/usr/include/sys/int_types.h" 1 3
#pragma ident	"@(#)int_types.h	1.6	97/08/20 SMI"
typedef char			int8_t;
typedef short			int16_t;
typedef int			int32_t;
typedef	long long		int64_t;
typedef unsigned char		uint8_t;
typedef unsigned short		uint16_t;
typedef unsigned int		uint32_t;
typedef unsigned long long	uint64_t;
typedef int64_t			intmax_t;
typedef uint64_t		uintmax_t;
typedef	int			intptr_t;
typedef	unsigned int		uintptr_t;
typedef char			int_least8_t;
typedef short			int_least16_t;
typedef int			int_least32_t;
typedef long long		int_least64_t;
typedef unsigned char		uint_least8_t;
typedef unsigned short		uint_least16_t;
typedef unsigned int		uint_least32_t;
typedef unsigned long long	uint_least64_t;
# 36 "/usr/include/sys/types.h" 2 3
typedef	long long		longlong_t;
typedef	unsigned long long	u_longlong_t;
# 65 "/usr/include/sys/types.h" 3
typedef long		t_scalar_t;	 
typedef unsigned long	t_uscalar_t;
typedef	unsigned char	uchar_t;
typedef	unsigned short	ushort_t;
typedef	unsigned int	uint_t;
typedef	unsigned long	ulong_t;
typedef	char		*caddr_t;	 
typedef	long		daddr_t;	 
typedef	short		cnt_t;		 
typedef	ulong_t		paddr_t;	 
typedef	int	ptrdiff_t;		 
typedef	ulong_t		pfn_t;		 
typedef	ulong_t		pgcnt_t;	 
typedef	long		spgcnt_t;	 
typedef	uchar_t		use_t;		 
typedef	short		sysid_t;
typedef	short		index_t;
typedef void		*timeout_id_t;	 
typedef void		*bufcall_id_t;	 
# 143 "/usr/include/sys/types.h" 3
typedef ulong_t		ino_t;		 
typedef long		blkcnt_t;	 
typedef ulong_t		fsblkcnt_t;	 
typedef ulong_t		fsfilcnt_t;	 
typedef u_longlong_t	ino64_t;	 
typedef longlong_t	blkcnt64_t;	 
typedef u_longlong_t	fsblkcnt64_t;	 
typedef u_longlong_t	fsfilcnt64_t;	 
typedef	long		blksize_t;	 
typedef enum { B_FALSE, B_TRUE } boolean_t;
typedef int64_t		pad64_t;
typedef	uint64_t	upad64_t;
# 202 "/usr/include/sys/types.h" 3
typedef	longlong_t	offset_t;
typedef	u_longlong_t	u_offset_t;
typedef u_longlong_t	len_t;
typedef	longlong_t	diskaddr_t;
# 222 "/usr/include/sys/types.h" 3
typedef union {
	offset_t	_f;	 
	struct {
		int32_t	_u;	 
		int32_t	_l;	 
	} _p;
} lloff_t;
# 242 "/usr/include/sys/types.h" 3
typedef union {
	diskaddr_t	_f;	 
	struct {
		int32_t	_u;	 
		int32_t	_l;	 
	} _p;
} lldaddr_t;
typedef uint_t k_fltset_t;	 
typedef	long		id_t;		 
typedef uint_t		useconds_t;	 
typedef long	suseconds_t;	 
typedef ulong_t	major_t;	 
typedef ulong_t	minor_t;	 
typedef short	pri_t;
typedef	ushort_t o_mode_t;		 
typedef short	o_dev_t;		 
typedef	ushort_t o_uid_t;		 
typedef	o_uid_t	o_gid_t;		 
typedef	short	o_nlink_t;		 
typedef short	o_pid_t;		 
typedef ushort_t o_ino_t;		 
typedef	int	key_t;			 
typedef	ulong_t	mode_t;			 
typedef	uid_t	gid_t;			 
typedef	unsigned int	pthread_t;	 
typedef	unsigned int	pthread_key_t;	 
typedef	struct	_pthread_mutex {		 
	struct {
		uint8_t		__pthread_mutex_flag[4];
		uint32_t 	__pthread_mutex_type;
	} __pthread_mutex_flags;
	union {
		struct {
			uint8_t	__pthread_mutex_pad[8];
		} __pthread_mutex_lock64;
		upad64_t __pthread_mutex_owner64;
	} __pthread_mutex_lock;
	upad64_t __pthread_mutex_data;
} pthread_mutex_t;
typedef	struct	_pthread_cond {		 
	struct {
		uint8_t		__pthread_cond_flag[4];
		uint32_t 	__pthread_cond_type;
	} __pthread_cond_flags;
	upad64_t __pthread_cond_data;
} pthread_cond_t;
typedef	struct _pthread_rwlock {	 
	int32_t		__pthread_rwlock_readers;
	uint16_t	__pthread_rwlock_type;
	uint16_t	__pthread_rwlock_magic;
	upad64_t	__pthread_rwlock_pad1[3];
	upad64_t	__pthread_rwlock_pad2[2];
	upad64_t	__pthread_rwlock_pad3[2];
} pthread_rwlock_t;
typedef struct _pthread_attr {
	void	*__pthread_attrp;
} pthread_attr_t;
typedef struct _pthread_mutexattr {
	void	*__pthread_mutexattrp;
} pthread_mutexattr_t;
typedef struct _pthread_condattr {
	void	*__pthread_condattrp;
} pthread_condattr_t;
typedef	struct	_once {
	upad64_t	__pthread_once_pad[4];
} pthread_once_t;
typedef struct _pthread_rwlockattr {
	void	*__pthread_rwlockattrp;
} pthread_rwlockattr_t;
typedef ulong_t	dev_t;			 
typedef	ulong_t	nlink_t;		 
typedef	long	pid_t;			 
typedef	long		time_t;	 
typedef	long		clock_t;  
typedef	int	clockid_t;	 
typedef	int	timer_t;	 
typedef	unsigned char	unchar;
typedef	unsigned short	ushort;
typedef	unsigned int	uint;
typedef	unsigned long	ulong;
# 501 "/usr/include/sys/types.h" 3
typedef unsigned char	u_char;
typedef unsigned short	u_short;
typedef unsigned int	u_int;
typedef unsigned long	u_long;
typedef struct _quad { int val[2]; } quad_t;	 
typedef quad_t		quad;			 
# 1 "/usr/include/sys/select.h" 1 3
#pragma ident	"@(#)select.h	1.16	98/04/27 SMI"	
# 1 "/usr/include/sys/time.h" 1 3
#pragma ident	"@(#)time.h	2.62	98/02/04 SMI"	
struct timeval {
	time_t		tv_sec;		 
	suseconds_t	tv_usec;	 
};
# 74 "/usr/include/sys/time.h" 3
struct timezone {
	int	tz_minuteswest;	 
	int	tz_dsttime;	 
};
# 1 "/usr/include/sys/types.h" 1 3
# 559 "/usr/include/sys/types.h" 3
# 100 "/usr/include/sys/time.h" 2 3
struct	itimerval {
	struct	timeval it_interval;	 
	struct	timeval it_value;	 
};
# 181 "/usr/include/sys/time.h" 3
typedef	longlong_t	hrtime_t;
# 310 "/usr/include/sys/time.h" 3
int adjtime(struct timeval *, struct timeval *);
int getitimer(int, struct itimerval *);
int utimes(const char *, const struct timeval *);
int setitimer(int, struct itimerval *, struct itimerval *);
int settimeofday(struct timeval *, void *);
hrtime_t	gethrtime(void);
hrtime_t	gethrvtime(void);
int gettimeofday(struct timeval *, void *);
# 1 "/usr/include/time.h" 1 3
#pragma ident	"@(#)time.h	1.36	98/03/27 SMI"	
# 1 "/usr/include/sys/types.h" 1 3
# 559 "/usr/include/sys/types.h" 3
# 19 "/usr/include/time.h" 2 3
struct	tm {	 
	int	tm_sec;
	int	tm_min;
	int	tm_hour;
	int	tm_mday;
	int	tm_mon;
	int	tm_year;
	int	tm_wday;
	int	tm_yday;
	int	tm_isdst;
};
extern clock_t clock(void);
extern double difftime(time_t, time_t);
extern time_t mktime(struct tm *);
extern time_t time(time_t *);
extern char *asctime(const struct tm *);
extern char *ctime(const time_t *);
extern struct tm *gmtime(const time_t *);
extern struct tm *localtime(const time_t *);
extern size_t strftime(char *, size_t, const char *, const struct tm *);
extern char *strptime(const char *, const char *, struct tm *);
# 1 "/usr/include/sys/time_impl.h" 1 3
#pragma ident	"@(#)time_impl.h	1.3	98/02/13 SMI"
typedef struct  timespec {		 
	time_t		tv_sec;		 
	long		tv_nsec;	 
} timespec_t;
# 58 "/usr/include/sys/time_impl.h" 3
typedef struct timespec timestruc_t;	 
typedef struct itimerspec {		 
	struct timespec	it_interval;	 
	struct timespec	it_value;	 
} itimerspec_t;
# 100 "/usr/include/sys/time_impl.h" 3
# 103 "/usr/include/time.h" 2 3
union sigval {
	int	sival_int;	 
	void	*sival_ptr;	 
};
struct sigevent {
	int		sigev_notify;	 
	int		sigev_signo;	 
	union sigval	sigev_value;	 
	void		(*sigev_notify_function)(union sigval);
	pthread_attr_t	*sigev_notify_attributes;
	int		__sigev_pad2;
};
extern int clock_getres(clockid_t, struct timespec *);
extern int clock_gettime(clockid_t, struct timespec *);
extern int clock_settime(clockid_t, const struct timespec *);
extern int timer_create(clockid_t, struct sigevent *, timer_t *);
extern int timer_delete(timer_t);
extern int timer_getoverrun(timer_t);
extern int timer_gettime(timer_t, struct itimerspec *);
extern int timer_settime(timer_t, int, const struct itimerspec *,
		struct itimerspec *);
extern int nanosleep(const struct timespec *, struct timespec *);
extern void tzset(void);
extern char *tzname[2];
extern long _sysconf(int);	 
extern long timezone;
extern int daylight;
extern int cftime(char *, char *, const time_t *);
extern int ascftime(char *, const char *, const struct tm *);
extern long altzone;
extern struct tm *getdate(const char *);
extern int getdate_err;
# 222 "/usr/include/time.h" 3
# 331 "/usr/include/time.h" 3
# 405 "/usr/include/sys/time.h" 2 3
# 1 "/usr/include/sys/select.h" 1 3
# 107 "/usr/include/sys/select.h" 3
# 418 "/usr/include/sys/time.h" 2 3
# 17 "/usr/include/sys/select.h" 2 3
typedef	long	fd_mask;
typedef	long	fds_mask;
typedef	struct fd_set {
	long	fds_bits[((( 1024  )+((  (sizeof (fds_mask) * 8 )  )-1))/(  (sizeof (fds_mask) * 8 )  )) ];
} fd_set;
extern int select(int, fd_set *, fd_set *, fd_set *, struct timeval *);
# 539 "/usr/include/sys/types.h" 2 3
# 11 "/usr/include/strings.h" 2 3
# 1 "/usr/include/string.h" 1 3
#pragma ident	"@(#)string.h	1.22	97/12/22 SMI"	
extern void *memcpy(void *, const void *, size_t);
extern void *memmove(void *, const void *, size_t);
extern char *strcpy(char *, const char *);
extern char *strncpy(char *, const char *, size_t);
extern char *strcat(char *, const char *);
extern char *strncat(char *, const char *, size_t);
extern int memcmp(const void *, const void *, size_t);
extern int strcmp(const char *, const char *);
extern int strcoll(const char *, const char *);
extern int strncmp(const char *, const char *, size_t);
extern size_t strxfrm(char *, const char *, size_t);
extern void *memchr(const void *, int, size_t);
extern char *strchr(const char *, int);
extern size_t strcspn(const char *, const char *);
extern char *strpbrk(const char *, const char *);
extern char *strrchr(const char *, int);
extern size_t strspn(const char *, const char *);
extern char *strstr(const char *, const char *);
extern char *strtok(char *, const char *);
extern void *memset(void *, int, size_t);
extern char *strerror(int);
extern size_t strlen(const char *);
extern void *memccpy(void *, const void *, int, size_t);
extern char *strsignal(int);
extern int ffs(int);
extern int strcasecmp(const char *, const char *);
extern int strncasecmp(const char *, const char *, size_t);
extern char *strdup(const char *);
# 143 "/usr/include/string.h" 3
# 15 "/usr/include/strings.h" 2 3
extern int bcmp(const void *, const void *, size_t);
extern void bcopy(const void *, void *, size_t);
extern void bzero(void *, size_t);
extern char *index(const char *, int);
extern char *rindex(const char *, int);
# 62 "/usr/include/strings.h" 3
# 18 "owners.h" 2
# 1 "/usr/include/sys/socket.h" 1 3
#pragma ident	"@(#)socket.h	1.48	98/06/19 SMI"	
# 1 "/usr/include/sys/uio.h" 1 3
#pragma ident	"@(#)uio.h	1.29	97/06/29 SMI"	
typedef struct iovec {
	caddr_t	iov_base;
	long	iov_len;
} iovec_t;
# 58 "/usr/include/sys/uio.h" 3
typedef enum uio_seg { UIO_USERSPACE, UIO_SYSSPACE, UIO_USERISPACE } uio_seg_t;
typedef struct uio {
	iovec_t	*uio_iov;	 
	int	uio_iovcnt;	 
	lloff_t	_uio_offset;	 
	uio_seg_t uio_segflg;	 
	short	uio_fmode;	 
	lldaddr_t _uio_limit;	 
	ssize_t	uio_resid;	 
} uio_t;
typedef enum uio_rw { UIO_READ, UIO_WRITE } uio_rw_t;
# 108 "/usr/include/sys/uio.h" 3
extern ssize_t readv(int, const struct iovec *, int);
extern ssize_t writev(int, const struct iovec *, int);
# 33 "/usr/include/sys/socket.h" 2 3
# 1 "/usr/include/sys/netconfig.h" 1 3
#pragma ident	"@(#)netconfig.h	1.15	97/03/10 SMI"	
struct  netconfig {
	char		*nc_netid;	 
	unsigned int	nc_semantics;	 
	unsigned int	nc_flag;	 
	char		*nc_protofmly;	 
	char		*nc_proto;	 
	char		*nc_device;	 
	unsigned int	nc_nlookups;	 
	char		**nc_lookups;	 
	unsigned int	nc_unused[8];	 
};
typedef struct {
	struct netconfig **nc_head;
	struct netconfig **nc_curr;
} NCONF_HANDLE;
extern void		*setnetconfig(void);
extern int		endnetconfig(void *);
extern struct netconfig	*getnetconfig(void *);
extern struct netconfig	*getnetconfigent(char *);
extern void		freenetconfigent(struct netconfig *);
extern void		*setnetpath(void);
extern int		endnetpath(void *);
extern struct netconfig *getnetpath(void *);
# 113 "/usr/include/sys/netconfig.h" 3
# 37 "/usr/include/sys/socket.h" 2 3
typedef uint16_t	sa_family_t;
typedef	uint32_t	socklen_t;
typedef	void		*Psocklen_t;
struct	linger {
	int	l_onoff;		 
	int	l_linger;		 
};
struct sockaddr {
	sa_family_t	sa_family;	 
	char		sa_data[14];	 
};
struct msghdr {
	void		*msg_name;		 
	socklen_t	msg_namelen;		 
	struct iovec	*msg_iov;		 
	int		msg_iovlen;		 
	caddr_t		msg_accrights;	 
	int		msg_accrightslen;
};
# 290 "/usr/include/sys/socket.h" 3
struct cmsghdr {
	socklen_t	cmsg_len;	 
	int		cmsg_level;	 
	int		cmsg_type;	 
};
# 325 "/usr/include/sys/socket.h" 3
# 349 "/usr/include/sys/socket.h" 3
# 372 "/usr/include/sys/socket.h" 3
extern int accept(int, struct sockaddr *, Psocklen_t);
extern int bind(int, const struct sockaddr *, socklen_t);
extern int connect(int, const struct sockaddr *, socklen_t);
extern int getpeername(int, struct sockaddr *, Psocklen_t);
extern int getsockname(int, struct sockaddr *, Psocklen_t);
extern int getsockopt(int, int, int, void *, Psocklen_t);
extern int listen(int, int);	 
extern int socketpair(int, int, int, int *);
extern ssize_t recv(int, void *, size_t, int);
extern ssize_t recvfrom(int, void *, size_t, int, struct sockaddr *,
	Psocklen_t);
extern ssize_t recvmsg(int, struct msghdr *, int);
extern ssize_t send(int, const void *, size_t, int);
extern ssize_t sendmsg(int, const struct msghdr *, int);
extern ssize_t sendto(int, const void *, size_t, int, const struct sockaddr *,
	socklen_t);
extern int setsockopt(int, int, int, const void *, socklen_t);
extern int shutdown(int, int);
extern int socket(int, int, int);
# 421 "/usr/include/sys/socket.h" 3
# 19 "owners.h" 2
# 1 "dbname.h" 1
# 20 "owners.h" 2
typedef struct owners Owners_record; 
typedef struct hotels Hotels_record; 
typedef struct company Company;  
typedef struct output Output;  
struct owners {
		char parent_name[12 ];
		char parent_city[11 ];
		char child_name[22 ];
		char child_city[11 ];
		float fraction;
		Owners_record *next;
	};
struct hotels {
	char hotel_name[22 ];
	char hotel_city[10 ];
	Hotels_record *next;
	};
struct company {
	char company_name[12 ]; 
	char company_city[11 ];
	double percentage;
	Company *next;
	};
struct output {
	char hotel_name[22 ];
	char hotel_city[10 ];
	Company *company_head;
	Output *next;
	};
# include "/local/ingres/files/eqsqlca.h"
    extern IISQLCA sqlca;   /* SQL Communications Area */
/* # line 3478 "owners.sc" */	/* host code */
Owners_record *getowners( void);
Hotels_record *gethotels( void);
Output *build(Owners_record *, Hotels_record *);
# 26 "tmp.c" 2
void usage(char *);
Hotels_record *getopthotel(char *, char *);
void print_out(Output *);
int main(int argc, char *argv[])
{
	char *prog_name = argv[0];  
	char *hotel_name;  
	char *hotel_city;  
	Owners_record *owners;  
	Hotels_record *hotels;  
	Output *output;  
	if (argc != 1 && (argc != 4 || strcmp(argv[1],"-h") != 0)) 
		usage(prog_name);
	if (argc == 1) 
		hotels = gethotels(); 
	else {
		hotel_name = argv[2];
		hotel_city = argv[3];
		hotels = getopthotel(hotel_name, hotel_city);
	}
	owners = getowners();  
	output = build(owners, hotels); 
	print_out(output);  
	return 0;
}
void usage(char *prog_name)
{
	fprintf((&__iob[2]) , "Usage: %s [-h \"Hotel name\" \"Hotel city\"]\n", prog_name);
	exit(1 );
}
Hotels_record *getopthotel(char *hotel_name, char *hotel_city)
{
	Hotels_record *node;	
	node = malloc(sizeof(Hotels_record));
	strcpy(node->hotel_name, hotel_name);
	strcpy(node->hotel_city, hotel_city);
	node->next = 0 ;
	return node;
}
void print_out(Output *output)
{
	Output *curr_ptr ;
	Company *company_curr ;
	printf("\nReport on hotel owners\n");
	printf("======================\n");
	curr_ptr = output;
	while (curr_ptr != 0 ) {
		printf("%s,  %s",curr_ptr->hotel_name, curr_ptr->hotel_city);
		if (curr_ptr->company_head == 0 )
			printf("\n");
		else {
			printf(":\n");
			company_curr = curr_ptr->company_head;
			while (company_curr != 0 ) {
				printf("\t%s, %s (%.1f%\%)\n",company_curr->company_name, company_curr->company_city, 
							 company_curr->percentage);
				company_curr = company_curr->next;
			}
		}
		curr_ptr = curr_ptr->next;
	}
	printf("\n");
	return;
}
