#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include "raylib.h"

// g++ maldebrot.cpp -o mald -lraylib -lX11 -lpthread -ldl -lrt -lm

const int HEIGHT = 600;
const int WIDTH  = 800;

int main()
{
    InitWindow(WIDTH, HEIGHT, "MALDEBROT");

    uint8_t  N_max      = 255;
    float x_offset      = 0;
    float y_offset      = 0;
    float dx            = 1.f / (float)WIDTH;
    float dy            = 1.f / (float)HEIGHT;
    float r2_max        = 4.f;
    float scale         = 2.5;
    float base_offset   = -250.f;

    while (!WindowShouldClose()) 
    {
        if (IsKeyDown(KEY_ESCAPE))       break;

        if (IsKeyDown(KEY_LEFT))         x_offset -= 20 * dx * scale;
        if (IsKeyDown(KEY_RIGHT))        x_offset += 20 * dx * scale;
        if (IsKeyDown(KEY_UP))           y_offset -= 20 * dy * scale;
        if (IsKeyDown(KEY_DOWN))         y_offset += 20 * dy * scale;

        if (IsKeyDown(KEY_PAGE_UP))      scale    /= 1.1;
        if (IsKeyDown(KEY_PAGE_DOWN))    scale    *= 1.1;

        BeginDrawing();
        ClearBackground(BLACK);

        float y_0 = (-HEIGHT / 2) * dy * scale + y_offset;

        for (int y_i = 0; y_i < HEIGHT; ++y_i, y_0 += dy * scale)
        {   
            float x_0 = (-WIDTH / 2 + base_offset) * dx * scale + x_offset;
            
            for (int x_i = 0; x_i < WIDTH; ++x_i, x_0 += dx * scale)
            {
                float X = 0;
                float Y = 0;

                uint8_t N = 0;
                for (; N < N_max; N++)
                {
                    float x2 = X * X;
                    float y2 = Y * Y;
                    float xy = X * Y;

                    float r2 = x2 + y2;

                    if (r2 >= r2_max) break;

                    X = x2 - y2 + x_0;
                    Y = xy + xy + y_0;
                }

                DrawPixel(x_i, y_i, (Color){0, (uint8_t)N , (uint8_t)(N * 0.5) , 150});
            }
        }

        DrawFPS(10, 10);

        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}