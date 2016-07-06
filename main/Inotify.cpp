#include "Inotify.h"

using namespace std;
using namespace tthread;

#ifdef  __cplusplus
extern "C" {
#endif

static int globalCount;

int execute_app(struct inotify_event * event, inotifyFd InotifyInfo) {
	int ret = 0;
	char str[MAX_EXT_SIZE];
	int prevCheckSum = 0;
	if (!FindExt(event->name, str)) {
		if (strcmp("txt", str) == 0) {
			//cout << "Extention matched" << endl;
			char *buffer[1];
			char str[128] = { '\0' };
			int currentChSum = 0;
			strcpy(str, InotifyInfo.path);
			strcat(str, "/");
			strcat(str, event->name);
			ReadFile(str, buffer);
			//cout << "Process Ext : " << str << endl;

			// convert message to ndef format.
			currentChSum = CheckSum(buffer[0]);
			if (prevCheckSum != currentChSum) {
				prevCheckSum = currentChSum;
				pid_t pid, pid2;
				int status;
				char *env[] =
				        { "LD_LIBRARY_PATH=../extra/libndef/libndef", NULL };
				pid = fork();
				if (pid == 0) {
					cout << "Child run.sh started" << endl;
					chdir("../main");

					ret = system("./run.sh");
					if (ret == -1) {
						perror("unable to load ./run.sh");
					}
					//execlp("./snep-encode", "./snep-encode", str, "en-US", NULL);
					execle("./snep-encode", "./snep-encode", str, "en-US", NULL,
					        env);
					perror("unable to load ./snep-encode");
					exit(-1);
				}
				pid = waitpid(pid, &status, 0);
				if (pid == -1) {
					perror("waitpid fail for run.sh and snep-encode process.");
					exit(2);
				}

				if (!WIFEXITED(status)) {
					printf("snep-encode terminated abnormally");
					exit(3);
				}
				if (WEXITSTATUS(status) != 0) {
					printf("snep-encode failed");
					exit(3);
				}

				// send it to nfc through libllcp
				int timeout = 5;
				pid2 = fork();
				if (pid2 == 0) {
					cout << "Child snep-client started timeout " << timeout
					        << " Second" << endl;
					chdir("../MyLib/libllcp");
					execlp("./examples/snep-client/snep-client",
					        "./examples/snep-client/snep-client",
					        "../../main/receipt_nfc", NULL);
					perror("unable to load ./snep-client from libllcp");
					exit(0);
				}

				printf("Child PID : %d\n", pid2);
				int waittime = 0;
				int Stat = 0;
				int wpid = 0;
				do {
					wpid = waitpid(pid2, &Stat, WNOHANG);
					if (wpid == 0) {
						if (waittime < timeout) {
							printf("Parent waiting %d second(s).\n", waittime);
							sleep(1);
							waittime++;
						}
						else {
							printf("Killing child process : %d\n", pid2);
							errno = kill(pid2, SIGKILL);
							if (errno == ESRCH) {
								printf("child process already killed\n");
							}
							if (errno == EPERM) {
								printf("child process permission error\n");
							}
							wpid = waitpid(pid2, &Stat, WSTOPPED);
							if (wpid == pid2) {
								printf("Child process : %d resource cleaned\n",
								        wpid);
							}
							break;
						}
					}
				} while (wpid == 0 && waittime <= timeout);

				if (WIFEXITED(Stat)) {
					//printf("Child exited, status = %d\n", WEXITSTATUS(Stat));
					// In fail it exit with : 1 (WEXITSTATUS(Stat))
					// In success it exit with : 0   (WEXITSTATUS(Stat))
					if (WEXITSTATUS(Stat) == 1) {
						printf("Child work Unsuccessful\n");
						// NDEF message not transmitted but program came back normally.
						return CHILD_UNSUCCESSFUL;
					}
					else if (WEXITSTATUS(Stat) == 0) {
						printf("Child work Done\n");
						// Child transmitted NDEF successfully.
						return CHILD_SUCCESSFUL;
					}
				}
				else if (WIFSIGNALED(Stat)) {
					printf("Child %d was signaled with a status of: %d \n",
					        pid2, WTERMSIG(Stat));
					return CHILD_SIGNALED;
				}
			}
			return MESSAGE_REPEATED;
		}
		return EXT_NOT_MATCH;
	}
}

void InotifyLoop(void *arg) {
	int length = 0;
	int i = 0;
	char buffer[BUF_LEN];
	int ret = 0;
	int prevCheckSum = 0;
	inotifyFd InotifyInfo = *(inotifyFd *) arg;
	printf(" buffer : %s \n", InotifyInfo.path);

	/*	do it forever*/
	while (1) {
		i = 0;
		length = 0;
		memset(buffer, '\0', sizeof(buffer));

		length = read(InotifyInfo.fd, buffer, BUF_LEN);
		if (length < 0) {
			cerr << "read Event error" << endl;
		}

		cout << "Event Buffer Length : " << length << endl;
		while (i < length) {
			struct inotify_event *event = (struct inotify_event *) &buffer[i];
			if (event->len) {
				cout << "checking Event" << endl;
				if (event->mask & IN_CREATE) {
					cout << "IN create Event" << endl;
					if (event->mask & IN_ISDIR) {
						cout << "The directory " << event->name
						        << " was Created" << endl;
					}
					else {
						cout << "The file " << event->name
						        << " was Created with WD " << event->wd << endl;
					}
				}
				if (event->mask & IN_MODIFY) {
					cout << "IN modify Event" << endl;
					if (event->mask & IN_ISDIR) {
						cout << "The directory " << event->name
						        << " was modified" << endl;
					}
					else {
						cout << "The file " << event->name
						        << " was modified with WD " << event->wd
						        << endl;
						ret = execute_app(event, InotifyInfo);
						if (ret == CHILD_SUCCESSFUL) {
							cout << "NDEF send : Done" << endl;
							break;
						}
						if (ret == CHILD_SIGNALED || ret == CHILD_UNSUCCESSFUL) {
							cout << "NDEF send : Fail" << endl;
							globalCount++;
							if (globalCount < 5) {
								cout << "\nSending again ..." << endl;
								continue;
							}
							else {
								cout << "Not Able to Send Data" << endl;
								cout << "Mobile Not Detected" << endl;
								globalCount = 0;
							}
						}
						if (ret == EXT_NOT_MATCH) {
							cout << "Extension not matched" << endl;
							break;
						}
						if (ret == MESSAGE_REPEATED) {
							cout << "Message repeated" << endl;
							break;
						}
					}
				}
				if (event->mask & IN_MOVE) {
					cout << "IN move Event" << endl;
					if (event->mask & IN_ISDIR) {
						cout << "The directory " << event->name
						        << " was Move to/from" << endl;
					}
					else {
						cout << "The file " << event->name
						        << " was Move to/from " << event->wd << endl;
					}
				}
			}

			i += EVENT_SIZE + event->len;
		}
	}
}
#ifdef  __cplusplus
}
#endif

Inotify::Inotify() {
	_InotifyInfo.fd = 0;
	_InotifyInfo.wd = 0;
	_InotifyInfo.path = NULL;

}

void Inotify::mainThread(char **argv) {

	/*	 Initialize Inotify*/
	_InotifyInfo.path = argv[1];
	printf("%s : \n", _InotifyInfo.path);
	_InotifyInfo.fd = inotify_init();
	if (_InotifyInfo.fd < 0) {
		cerr << "Couldn't initialize inotify" << endl;
	}

	/*	add watch to starting directory*/
	_InotifyInfo.wd = inotify_add_watch(_InotifyInfo.fd, argv[1],
	IN_CREATE | IN_MODIFY | IN_DELETE | IN_OPEN | IN_MOVE | IN_CLOSE);
	if (_InotifyInfo.wd == -1) {
		cerr << "Couldn't add watch to " << argv[1] << endl;
	}
	else {
		cerr << "Watching:: " << argv[1] << endl;
	}
	/*	Start the child thread*/
	thread t(InotifyLoop, (void *) &_InotifyInfo);
	t.join();
	/*	Clean up*/
	inotify_rm_watch(_InotifyInfo.fd, _InotifyInfo.wd);
	close(_InotifyInfo.fd);
}

