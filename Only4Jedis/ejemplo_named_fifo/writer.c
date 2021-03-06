#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>

#define FIFO_NAME "myfifo"
#define BUFFER_SIZE 300

int main(void)
{
   
    char outputBuffer[BUFFER_SIZE];
	uint32_t bytesWrote;
	int32_t returnCode, fd;
    
    // en su mayoria los errores son negativos 
    // en este caso error -1 indica que ya existe la cola
    //lo ignoramos porque puede ser otro usuario la pudo haber creado y solo necesito usarla
    // por defecto el umask esta 0022
    // por ello debemos cambiarlo 
    if ( (returnCode = mknod(FIFO_NAME, S_IFIFO | 0666, 0) ) < -1 )
    {
        printf("Error creating named fifo: %d\n", returnCode);
        exit(1);
    }

    // en este caso si consideramos el error <0
    /* Open named fifo. Blocks until other process opens it */
	printf("waiting for readers...\n");
	if ( (fd = open(FIFO_NAME, O_WRONLY) ) < 0 )
    {
        printf("Error opening named fifo file: %d\n", fd);
        exit(1);
    }
    
    /* open syscalls returned without error -> other process attached to named fifo */
	printf("got a reader--type some stuff\n");

    /* Loop forever */
	while (1)
	{
        /* Get some text from console */
		fgets(outputBuffer, BUFFER_SIZE, stdin);
        
        /* Write buffer to named fifo. Strlen - 1 to avoid sending \n char */
		if ((bytesWrote = write(fd, outputBuffer, strlen(outputBuffer)-1)) == -1)
        {
			perror("write");
        }
        else
        {
			printf("writer: wrote %d bytes\n", bytesWrote);
        }
	}
	return 0;
}
