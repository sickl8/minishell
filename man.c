/*
	malloc:
		void			*malloc(size_t bytes);
	free:
		void			free(void* adr);
	write:
		int				write(int fd, void* buf, size_t len);
	open:
		int				open(const char *path, int oflag, ...);
	read:
		int				read(int fd, void* buf, size_t len);
	close:
		int				close(int fd);
	fork:
		pid_t			fork(void);
	wait:
		pid_t			wait(int *stat_loc);
	waitpid:
		pid_t			waitpid(pid_t pid, int *stat_loc, int options);
	wait3:
		pid_t			wait3(int *stat_loc, int options, struct rusage *rusage);
	wait4:
		pid_t			wait4(pid_t pid, int *stat_loc, int options, struct rusage *rusage);
	signal:
		sig_t			signal(int sig, sig_t func);
	kill:
		int				kill(pid_t pid, int sig);
	exit:
		void			exit(int status);
	getcwd:
		char			*getcwd(char *buf, size_t size);
	chdir:
		int				chdir(const char *path);
	fstat:
		int				fstat(int fildes, struct stat *buf);
	stat:
		int				stat(const char *restrict path, struct stat *restrict buf);
	lstat:
		int				lstat(const char *restrict path, struct stat *restrict buf);
	execve:
		int				execve(const char *path, char *const argv[], char *const envp[]);
	dup:
		int				dup(int fildes);
	dup2:
		int				dup2(int fildes, int fildes2);
	pipe:
		int				pipe(int fildes[2]);
	opendir:
		DIR				*opendir(const char *filename);
	readdir:
		struct dirent	*readdir(DIR *dirp);
	closedir:
		int				closedir(DIR *dirp);		
	strerror:
		char			*strerror(int errnum);
	errno:
		int				errno;
*********************************************************************************************
	pid_t = int;
*********************************************************************************************
	DIR = typedef struct
	{
		int	__dd_fd;								// file descriptor associated with directory
		long	__dd_loc;							// offset in current buffer 
		long	__dd_size;							// amount of data returned 
		char	*__dd_buf;							// data buffer 
		int	__dd_len;								// size of data buffer 
		long	__dd_seek;							// magic cookie returned 
		__unused long	__padding;					// (__dd_rewind space left for bincompat) 
		int	__dd_flags;								//flags for readdir 
		__darwin_pthread_mutex_t __dd_lock;			// for thread locking 
		struct _telldir *__dd_td;					// telldir position recording
	}												DIR;
*********************************************************************************************
	struct dirent =
	{
		__uint64_t	d_ino;							// file number of entry
		__uint64_t	d_seekoff;						// seek offset (optional, used by servers)
		__uint16_t	d_reclen;						// length of this record
		__uint16_t	d_namlen;						// length of string in d_name
		__uint8_t	d_type;							// file type, see below
		char		d_name[__DARWIN_MAXPATHLEN];	// entry name (up to MAXPATHLEN bytes)
	}
*********************************************************************************************
*/