// This is the main program (i.e. the main thread)
#include "Inotify.h"

using namespace std;
int main(int argc, char *argv[]) {
	Inotify moniter;
	struct stat directoryStatus;
	if (argc < 2) {
		cout << "Pass Folder Path as Second Argument." << endl;
		exit(0);
	}
	else {
		cout << argv[1] << endl;
		if (stat(argv[1], &directoryStatus)
		        == 0&& S_ISDIR(directoryStatus.st_mode)) {
			cout << "Director found.\n" << endl;
		}
		else {
			cout << "No directory found.\n Directory Created.\n" << endl;
			createDirectory(argv[1], 0777);
		}
	}
	moniter.mainThread(argv);
}
