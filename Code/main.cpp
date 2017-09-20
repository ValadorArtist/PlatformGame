#include <iostream>

#include "Application.h"
#include "Engine.h"


int main(int argc, char *argv[]) 
{
    Engine::Get().Load();

    size_t width = 640;
    size_t height = 420;
    Engine::Get().GetWindow()->SetSize(width, height);
    Application Application(true);

    Application.Run();
    return 0;
}
