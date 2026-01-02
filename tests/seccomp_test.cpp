#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "sandbox/seccomp.h"

int main() {
    std::cout << "seccomp_test: starting" << std::endl;
    // Create a minimal whitelist file that allows some syscalls but not execve
    const char* wf = "/tmp/native_node_seccomp_whitelist.conf";
    FILE* f = fopen(wf, "w");
    if (!f) {
        std::cerr << "failed to create whitelist file" << std::endl;
        return 2;
    }
    fprintf(f, "read\nwrite\nopen\nclose\nexit\nexit_group\n");
    fclose(f);

    pid_t pid = fork();
    if (pid < 0) {
        std::cerr << "fork failed: " << strerror(errno) << std::endl;
        return 2;
    }
    if (pid == 0) {
        // Child: apply seccomp and then attempt execve (should fail with EPERM/EACCES)
        if (!sandbox::load_seccomp_whitelist(wf)) {
            std::cerr << "child: failed to load seccomp whitelist" << std::endl;
            _exit(2);
        }
        char* const argv[] = { (char*)"/bin/true", nullptr };
        execv("/bin/true", argv);
        // execv returned: expected error
        if (errno == EPERM || errno == EACCES) {
            std::cout << "child: execv blocked by seccomp as expected (errno=" << errno << ")" << std::endl;
            _exit(0);
        }
        std::cerr << "child: execv returned unexpected errno: " << errno << std::endl;
        _exit(2);
    }

    int status = 0;
    if (waitpid(pid, &status, 0) < 0) {
        std::cerr << "waitpid failed" << std::endl;
        return 2;
    }
    if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
        std::cout << "seccomp_test: succeeded" << std::endl;
        return 0;
    }
    std::cerr << "seccomp_test: child failed (status=" << status << ")" << std::endl;
    return 2;
}
