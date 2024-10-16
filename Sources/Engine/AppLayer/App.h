//-----------------------------------------------------------------------------
// Eugenix Framework
// Copyright (C) Evgeny Strelkovsky, 2024
//-----------------------------------------------------------------------------

#pragma once

#include <chrono>

#include <SDL.h>

namespace Eugenix
{
    class CChronometer
    {
    public:
        CChronometer()
        {
            Reset();
        }

        void Reset()
        {
            _lastTime = std::chrono::system_clock::now();
        }

        float GrabDeltaTime()
        {
            auto newTime = std::chrono::system_clock::now();
            auto timePassed = std::chrono::duration_cast<std::chrono::milliseconds>(newTime - _lastTime);
            _lastTime = newTime;
            return 0.001f * float(timePassed.count());
        };

    private:
        std::chrono::system_clock::time_point _lastTime;
    };

    struct ApplicationConfig
    {
        struct App
        {
            bool enableConsole = true;
        } app;

        struct Window
        {
            int width = 640;
            int height = 480;
            const char* title = "Test";
            bool fullscreen = false;
        } window;

        struct Graphics
        {
            float maxFPS = 60.0f;
        } graphics;

        struct Audio
        {

        } audio;

        struct Resources
        {
            const char* coreFolder = "";
        } resources;
    };

    class App
    {
    public:
        int Run(const ApplicationConfig& config);

    protected:
        virtual bool OnInit() { return true; }
        virtual void OnShutdown() {}
        virtual void OnRender() {}
        virtual void OnUpdate(float deltaTime) {}

        std::string loadStringFromFile(const std::string& filename);

    private:
        int InternalMainLoop();

        SDL_Window* _window{ nullptr };
        SDL_GLContext _glContext;

        CChronometer _chronometer;

        ApplicationConfig _config;
    };
} // namespace Eugenix


