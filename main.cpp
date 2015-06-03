#include "renderer.h"
#include "model.h"

int main(int argc, char*argv[])
{
    Model m;
    Renderer r;

    m.Load("data.txt");
    m.Process();

    r.SetModel(m);
    r.Run();

    return 0;
}
