#include "../_base.h"
#include <signal.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CHILD_SUCCESSFUL 0
#define CHILD_UNSUCCESSFUL 1
#define CHILD_SIGNALED 2
#define ABNORMALLY_EXITED 3

int waitForProcess(int pid, int timeout, int wait);
#ifdef __cplusplus
}
#endif
