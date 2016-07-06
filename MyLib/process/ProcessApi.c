/** @FileApi.c
 *  @File system related helper API.
 *

 *  @author  ()
 *  @bug No know bugs.
 */

#include "ProcessApi.h"

/*
 *  @author  ()
 *  @bug No know bugs.
 *  @param1 = pid of child process
 *  @param2 = timeout if want to put limit of execution for child process
 *  @param3 = wait = 1 (wait for process in blocking call)
 = 0 (wait for process till timeout happen or child exited)
 *  @Returns	=  CHILD_SUCCESSFUL = If successful exit by child
 *				=  CHILD_SIGNALED	 = If child signaled after timeout
 */
int waitForProcess(int pid, int timeout, int wait) {
	printf("The pid of the child is: %d\n", pid);
	int waittime = 0;
	int stat = 0;
	int wpid = 0;

	if (wait == 1) {
		printf("Waiting for child to exit\n");
		wpid = waitpid(pid, &stat, 0);
		if (pid == -1) {
			perror("Child exited abnormally");
			return ABNORMALLY_EXITED;
		}
	}
	else if (wait == 0) {
		do {
			wpid = waitpid(pid, &stat, WNOHANG);
			if (wpid == 0) {
				if (waittime < timeout) {
					printf("Parent waiting %d second(s).\n", waittime);
					sleep(1);
					waittime++;
				}
				else {
					printf("Killing child process : %d\n", pid);
					kill(pid, SIGKILL);
					break;
				}
			}
		} while (wpid == 0 && waittime <= timeout);
	}
	else {
		perror("Pass wait argument as 0 OR 1\n");
		return -1;
	}

	if (WIFEXITED(stat)) {
		printf("Child exited, status = %d\n", WEXITSTATUS(stat));
		return CHILD_SUCCESSFUL;
	}
	else if (WIFSIGNALED(stat)) {
		printf("Child %d was terminated with a status of: %d \n", pid,
		        WTERMSIG(stat));
		return CHILD_SIGNALED;
	}
}
