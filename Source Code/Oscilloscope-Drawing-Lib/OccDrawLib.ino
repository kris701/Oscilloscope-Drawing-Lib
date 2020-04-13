#include <avr/pgmspace.h> 
#include "DrawLib.h"

unsigned long timer;
unsigned long currentTimer;
String FPS = "0";
int FPSCounter = 0;

Path TracePoint[] = { 
    {{1318, 306}, {1638, 1414} }, 
    { {1638, 1414},{1998, 273} }, 
    { {1638, 1414},{1634, 2988} }, 
    { {1638, 2433},{896, 2198} },
    { {1638, 2433},{2223, 2153} },
    { {1441, 3281},{1559, 3160} },
    { {1559, 3160}, {1687, 3166} },
    { {1687, 3166}, {1836, 3281} },
    { {1481, 3498}, {1490, 3505} },
    { {1785, 3489}, {1790, 3495} }
};

drawClass DrawingClass;

void setup() {
    Serial.begin(115200);
    DrawingClass.Config(DAC0, DAC1, 4096, 4096, 12);
    DrawingClass.SetQuality(DQ_High);

    timer = millis();
}

void loop() {
    for (int angle = 1; angle < 361; angle += 10)
    {
        FPSCounter++;
        DrawingClass.DrawRegtangle({ 0,0 }, 4095, 4095, 1, 0, false);

        DrawingClass.DrawPath(TracePoint, 10, {1000,0}, 0.5, angle);

        DrawingClass.DrawString(FPS, { 150, 3600 }, 0.5, 0);

        CheckTimer();
    }

    for (double i = 0.5; i < 1.5; i += 0.01)
    {
        FPSCounter++;
        DrawingClass.DrawRegtangle({ 0,0 }, 4095, 4095, 1, 0, false);

        DrawingClass.DrawPath(TracePoint, 10, { 1000,0 }, i, 0);

        DrawingClass.DrawString(FPS, { 150, 3600 }, 0.5, 0);

        CheckTimer();
    }

    for (double i = 1.5; i > 0.5; i -= 0.01)
    {
        FPSCounter++;
        DrawingClass.DrawRegtangle({ 0,0 }, 4095, 4095, 1, 0, false);

        DrawingClass.DrawPath(TracePoint, 10, { 1000,0 }, i, 0);

        DrawingClass.DrawString(FPS, { 150, 3600 }, 0.5, 0);

        CheckTimer();
    }
}

void CheckTimer()
{
    currentTimer = millis();
    if (currentTimer - timer >= 1000)
    {
        timer = currentTimer;
        FPS = String(FPSCounter);
        FPSCounter = 0;
    }
}
