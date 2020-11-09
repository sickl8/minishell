#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#define wr(x) write(0, x, strlen(x))
// int main( int argc, const char* argv[] ) 
// {   
//     char arr[50];
// 	int i;
// 	int stdin_bak;
	
// 	stdin_bak = dup(0);
// 	pipe(pfd);
// 	dup2(pfd[0], 0);
// 	while (1)
// 	{
// 		i = read(0, arr, 50);
// 		printf("i = %d\n", i);
// 		if (i == 0)
// 		{
// 			close(0);
// 			dup2(stdin_bak,0);
// 			close(stdin_bak);
// 			stdin_bak = dup(0);
// 		}
// 	}
// 	return 0;
// }
#include "libft/libft.h"

int		main()
{
	char str[10000];
	struct termios termInfo, save;
    int c;
	int rdret;
	int bk = dup(0);

    c = tcgetattr(0,&termInfo);


    if(c == -1 ){
	    perror("tcgetattr");
	    exit(1);
    }
	//termInfo.c_lflag &= ~ECHO;
	//tcsetattr(0, 0, &termInfo);
	while (1)
	{
		memset(str, 0, 100);
		// wr("\033[s");
		wr("prompt: ");
		wr("\033[s");
		rdret = read(0, str, 10000);
		if (rdret == 0 || !ft_strchr(str, '\n'))
		{
			close(0);
			dup2(bk,0);
			close(bk);
			bk = dup(0);
			/**/
			//wr("\033[1A");
			//wr("www");
			sleep(1);
			int i = 0;
			while (i < rdret){
				wr("\b");
				i++;
			}
			i = -1;
			//while (++i < rdret)
				wr("\033[5C");
			while (1);
		}
		else
		{
			str[rdret - 1] = '\0';
			if (!strcmp(str, "reset"))
			{
				wr("\033[1A");
			}
		}
		// sleep(1);
		// wr("\033[1A");
		// sleep(1);
		// wr("\033[1D");
		// sleep(1);
		// if (!strcmp(str, "U"))
		// 	wr("\033[1A");
		// else if (!strcmp(str, "D"))
		// 	wr("\033[1B");
		// else if (!strcmp(str, "R"))
		// 	wr("\033[1C");
		// else if (!strcmp(str, "L"))
		// 	wr("\033[1D");
		// else
		// 	wr("\033[u");
		// {
		// 	termInfo.c_lflag |= ECHO;
		// 	tcsetattr(0, 0, &termInfo);
		// }
		//wr("\n");
	}
}


// red orange yellow lime green turquoise cyan blue pink purple 