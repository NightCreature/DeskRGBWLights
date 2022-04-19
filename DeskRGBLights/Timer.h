#pragma once


//Tracks elapsed time
class Timer
{
public:
    Timer();

    void Update();
    const float GetElapsedTime() const { return m_elapsedTime; }
private:
    unsigned long m_timeStamp;
    float m_elapsedTime;
};

