//-----------------------------------------------------------------------------
// Eugenix Framework
// Copyright (C) Evgeny Strelkovsky, 2024
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <iostream>
#include <sstream>


#include <SDL.h>

#include "App.h"
#include "Platform/Runtime.h"

const int DEFAULT_WIDTH = 1024, DEFAULT_HEIGHT = 768;

using namespace Eugenix;

std::string App::loadStringFromFile(const std::string& filename)
{
    std::ifstream in;
    // Open a stream to the file.
    in.open(filename.c_str());
    if (!in)
    {
        std::cerr << filename + " is not a valid file." << std::endl;
        return "";
    }
    std::stringstream buffer;
    // Read the stream in a buffer.
    buffer << in.rdbuf();
    // Create a string based on the content of the buffer.
    std::string line = buffer.str();
    in.close();
    return line;
}

int App::Run()
{
    if (!Runtime::Init(DisplaySetup()))
        return 1;

    if (!OnInit())
        return 1;

    auto status = InternalMainLoop();

    OnShutdown();

    Runtime::Term();

    return status;
}

int App::InternalMainLoop()
{
    _chronometer.Reset();

    while (Runtime::RunFrame())
    {
        const float deltaTime = _chronometer.GrabDeltaTime();        
        OnUpdate(deltaTime);
        OnRender();

        Runtime::CommitFrame();
    }

    return 0;
}