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
            : m_lastTime(std::chrono::system_clock::now())
        {}

        void Reset()
        {
            m_lastTime = std::chrono::system_clock::now();
        }

        float GrabDeltaTime()
        {
            auto newTime = std::chrono::system_clock::now();
            auto timePassed = std::chrono::duration_cast<std::chrono::milliseconds>(newTime - m_lastTime);
            m_lastTime = newTime;
            return 0.001f * float(timePassed.count());
        };

    private:
        std::chrono::system_clock::time_point m_lastTime;
    };

    class App
    {
    public:
        int Run();

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
    };
} // namespace Eugenix


