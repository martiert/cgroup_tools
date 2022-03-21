#include <sys/times.h>

#include <fmt/core.h>

#include <unistd.h>
#include <time.h>
#include <string.h>

#include <iostream>
#include <string>
#include <vector>
#include <thread>

namespace {

void errExit(std::string const & reason)
{
    fmt::print("{} Failed with reason: {}\n", reason, strerror(errno));
    exit(1);
}

void burn_cpu()
{
    timespec previous_rt;
    if (clock_gettime(CLOCK_REALTIME, &previous_rt) == -1)
        errExit("clock_gettime REALTIME");

    int previous_step = 0;
    int nloops = 0;
    pid_t tid = gettid();

    while (true) {
        ++nloops;

        timespec current_cpu;
        if (clock_gettime(CLOCK_THREAD_CPUTIME_ID, &current_cpu) == -1)
            errExit("clock_gettime CPU");

        int current_step = current_cpu.tv_sec +
            current_cpu.tv_nsec / 1000000000;

        timespec current_rt;
        if (clock_gettime(CLOCK_REALTIME, &current_rt) == -1)
            errExit("clock_gettime REALTIME");

        int elapsed = (current_rt.tv_sec - previous_rt.tv_sec) * 1000000 +
            (current_rt.tv_nsec - previous_rt.tv_nsec) / 1000;

        if (current_step > previous_step) {
            fmt::print("Thread: {} elapsed/cpu = {:.3} %CPU = {:.3}\n",
                    tid,
                    elapsed / 1000000.0,
                    1.0 / (elapsed / 1000000.0) * 100.0);
            std::flush(std::cout);
            previous_step = current_step;
            previous_rt = current_rt;
            nloops = 0;
        }
    }
}

}

int main(int argc, char ** argv)
{
    int n = std::stoi(argv[1]);
    std::vector<std::thread> threads;

    for (size_t i = 0; i < n; ++i) {
        threads.emplace_back([]() { burn_cpu(); });
    }

    pause();
}
