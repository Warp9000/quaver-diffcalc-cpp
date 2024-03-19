#include "Qua.h"
#include "DifficultyProcessor.h"

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <file>\n", argv[0]);
        return 1;
    }

    string file = argv[1];
    Qua qua = Qua::ParseFile(file);

    qua.Save("t2.qua");

    DifficultyProcessor processor = DifficultyProcessor(&qua, ModIdentifier::NoMod, true);

    printf("%f\n", processor.OverallDifficulty);

    return 0;
}