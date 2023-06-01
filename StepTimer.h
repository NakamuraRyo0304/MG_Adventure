//
// StepTimer.h - A simple timer that provides elapsed time information
//

#pragma once

#include <cmath>
#include <exception>
#include <stdint.h>

namespace DX
{
    // Helper class for animation and simulation timing.
    class StepTimer
    {
    public:
        StepTimer() noexcept(false) :
            m_elapsedTicks(0),
            m_totalTicks(0),
            m_leftOverTicks(0),
            m_frameCount(0),
            m_framesPerSecond(0),
            m_framesThisSecond(0),
            m_qpcSecondCounter(0),
            m_isFixedTimeStep(false),
            m_targetElapsedTicks(TicksPerSecond / 60)
        {
            if (!QueryPerformanceFrequency(&m_qpcFrequency))
            {
                throw std::exception( "QueryPerformanceFrequency" );
            }

            if (!QueryPerformanceCounter(&m_qpcLastTime))
            {
                throw std::exception( "QueryPerformanceCounter" );
            }

            // Initialize max delta to 1/10 of a second.
            m_qpcMaxDelta = static_cast<uint64_t>(m_qpcFrequency.QuadPart / 10);
        }

        // Get elapsed time since the previous Update call.
        uint64_t GetElapsedTicks() const					{ return m_elapsedTicks; }
        double GetElapsedSeconds() const					{ return TicksToSeconds(m_elapsedTicks); }

        // Get total time since the start of the program.
        uint64_t GetTotalTicks() const						{ return m_totalTicks; }
        double GetTotalSeconds() const						{ return TicksToSeconds(m_totalTicks); }

        // Get total number of updates since start of the program.
        uint32_t GetFrameCount() const						{ return m_frameCount; }

        // Get the current framerate.
        uint32_t GetFramesPerSecond() const					{ return m_framesPerSecond; }

        // Set whether to use fixed or variable timestep mode.
        void SetFixedTimeStep(bool isFixedTimestep)			{ m_isFixedTimeStep = isFixedTimestep; }

        // Set how often to call Update when in fixed timestep mode.
        void SetTargetElapsedTicks(uint64_t targetElapsed)	{ m_targetElapsedTicks = targetElapsed; }
        void SetTargetElapsedSeconds(double targetElapsed)	{ m_targetElapsedTicks = SecondsToTicks(targetElapsed); }

        // Integer format represents time using 10,000,000 ticks per second.
        static const uint64_t TicksPerSecond = 10000000;

        static double TicksToSeconds(uint64_t ticks)		{ return static_cast<double>(ticks) / TicksPerSecond; }
        static uint64_t SecondsToTicks(double seconds)		{ return static_cast<uint64_t>(seconds * TicksPerSecond); }

        // After an intentional timing discontinuity (for instance a blocking IO operation)
        // call this to avoid having the fixed timestep logic attempt a set of catch-up
        // Update calls.

        void ResetElapsedTime()
        {
            if (!QueryPerformanceCounter(&m_qpcLastTime))
            {
                throw std::exception("QueryPerformanceCounter");
            }

            m_leftOverTicks = 0;
            m_framesPerSecond = 0;
            m_framesThisSecond = 0;
            m_qpcSecondCounter = 0;
        }

        // Update timer state, calling the specified Update function the appropriate number of times.
        template<typename TUpdate>
        void Tick(const TUpdate& update)
        {
            // Query the current time.
            LARGE_INTEGER currentTime;

            if (!QueryPerformanceCounter(&currentTime))
            {
                throw std::exception( "QueryPerformanceCounter" );
            }

            uint64_t timeDelta = static_cast<uint64_t>(currentTime.QuadPart - m_qpcLastTime.QuadPart);

            m_qpcLastTime = currentTime;
            m_qpcSecondCounter += timeDelta;

            // Clamp excessively large time deltas (e.g. after paused in the debugger).
            if (timeDelta > m_qpcMaxDelta)
            {
                timeDelta = m_qpcMaxDelta;
            }

            // Convert QPC units into a canonical tick format. This cannot overflow due to the previous clamp.
            timeDelta *= TicksPerSecond;
            timeDelta /= static_cast<uint64_t>(m_qpcFrequency.QuadPart);

            uint32_t lastFrameCount = m_frameCount;

            if (m_isFixedTimeStep)
            {
                // �^�C���X�e�b�v�X�V���W�b�N���C��
               // �A�v���̓��삪�ڕW�o�ߎ��Ԃɔ��ɋ߂��ꍇ�i1/4�~���b�ȓ��j�ɂ́A
               // �N�����v�����ŃN���b�N���^�[�Q�b�g�l�ɐ��m�Ɉ�v����悤�ɂ��܂��B
               // ����ɂ��A�����Ŗ��֌W�ȃG���[��h�����Ƃ��ł��܂�
               // �����Ԍo�߂ƂƂ��ɒ~�ς��Ă����B���̃N�����v���Ȃ���΁A
               //60fps��v�������Q�[����59.94��NTSC�f�B�X�v���C��vsync��L���ɂ��Ď��s�����ꍇ�A
               // �C�����ꂽ�A�b�v�f�[�g�́A�ŏI�I�ɂ�
               // �t���[���𗎂Ƃ��Ă��܂��قǏ����ȃG���[���~�ς���܂��B
               // �����ȃY�����[���ɂ��邱�ƂŁA�������~���ɐi�߂邱�Ƃ��ł��܂��B

                if (static_cast<uint64_t>(std::abs(static_cast<int64_t>(timeDelta - m_targetElapsedTicks))) < TicksPerSecond / 4000)
                {
                    timeDelta = m_targetElapsedTicks;
                }

                m_leftOverTicks += timeDelta;

                while (m_leftOverTicks >= m_targetElapsedTicks)
                {
                    m_elapsedTicks = m_targetElapsedTicks;
                    m_totalTicks += m_targetElapsedTicks;
                    m_leftOverTicks -= m_targetElapsedTicks;
                    m_frameCount++;

                    update();
                }
            }
            else
            {
                // Variable timestep update logic.
                m_elapsedTicks = timeDelta;
                m_totalTicks += timeDelta;
                m_leftOverTicks = 0;
                m_frameCount++;

                update();
            }

            // Track the current framerate.
            if (m_frameCount != lastFrameCount)
            {
                m_framesThisSecond++;
            }

            if (m_qpcSecondCounter >= static_cast<uint64_t>(m_qpcFrequency.QuadPart))
            {
                m_framesPerSecond = m_framesThisSecond;
                m_framesThisSecond = 0;
                m_qpcSecondCounter %= static_cast<uint64_t>(m_qpcFrequency.QuadPart);
            }
        }

    private:
        // Source timing data uses QPC units.
        LARGE_INTEGER m_qpcFrequency;
        LARGE_INTEGER m_qpcLastTime;
        uint64_t m_qpcMaxDelta;

        // Derived timing data uses a canonical tick format.
        uint64_t m_elapsedTicks;
        uint64_t m_totalTicks;
        uint64_t m_leftOverTicks;

        // Members for tracking the framerate.
        uint32_t m_frameCount;
        uint32_t m_framesPerSecond;
        uint32_t m_framesThisSecond;
        uint64_t m_qpcSecondCounter;

        // Members for configuring fixed timestep mode.
        bool m_isFixedTimeStep;
        uint64_t m_targetElapsedTicks;
    };
}
