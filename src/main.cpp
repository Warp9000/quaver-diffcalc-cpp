#include "Qua.h"
#include "DifficultyProcessor.h"
#include <chrono>

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <file>\n", argv[0]);
        return 1;
    }

    auto start = std::chrono::high_resolution_clock::now();
    string file = argv[1];
    Qua qua = Qua::ParseFile(file);
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = end - start;
    printf("Time elapsed (parse): %lldms\n", std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count());

    start = std::chrono::high_resolution_clock::now();
    DifficultyProcessor processor = DifficultyProcessor(&qua, ModIdentifier::NoMod, true);
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    printf("Time elapsed (diff): %lldms\n", std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count());

    printf("difficulty: %f\n", processor.OverallDifficulty);

    return 0;
}