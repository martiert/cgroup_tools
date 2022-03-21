#include <fmt/core.h>

#include <unistd.h>
#include <string>

namespace {

void allocate(size_t blockSize, size_t allocations)
{
    std::size_t total_allocated = 0;

    for (size_t i = 0; i < allocations; ++i) {
        char * p = new char[blockSize];

        if (!p) {
            fmt::print("Failed to allocate {} bytes", blockSize);
            return;
        }

        for (size_t j = 0; j < blockSize; j += 1024)
            p[j] = 0;

        total_allocated += blockSize;
        fmt::print("#{}: address = 0x{:p}; total = {} ({} MiB)\n",
                i, p, total_allocated, total_allocated / (1024 * 1024));
    }
}

}


int main(int argc, char ** argv)
{
    std::size_t blockSize = std::stoul(argv[1]);
    std::size_t allocations = std::stoul(argv[2]);

    allocate(blockSize, allocations);

    fmt::print("All memory allocated\n");
    pause();
}
