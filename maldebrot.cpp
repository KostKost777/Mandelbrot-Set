#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "raylib.h"

// g++ maldebrot.cpp -o mald -lraylib -lX11 -lpthread -ldl -lrt -lm

const int HEIGHT = 600;
const int WIDTH  = 800;

int main()
{
    InitWindow(WIDTH, HEIGHT, "MALDEBROT");

    int   N_max    = 256;
    float r2_max   = 4;
    float x_offset = 0;
    float y_offset = 0;
    float zoom     = 1;

    int aaaa = 0;

    while (!WindowShouldClose()) 
    {
        if (IsKeyDown(KEY_ESCAPE))       break;
        if (IsKeyDown(KEY_LEFT))         x_offset -= 10 * zoom;
        if (IsKeyDown(KEY_RIGHT))        x_offset += 10 * zoom;
        if (IsKeyDown(KEY_UP))           y_offset -= 10 * zoom;
        if (IsKeyDown(KEY_DOWN))         y_offset += 10 * zoom;
        if (IsKeyDown(KEY_PAGE_UP))      zoom     /= 1.1;
        if (IsKeyDown(KEY_PAGE_DOWN))    zoom     *= 1.1;

        BeginDrawing();
        ClearBackground(BLACK);

        clock_t start_time = clock();

        for (int i_y = 0; i_y < HEIGHT; ++i_y)
        {   
            for (int i_x = 0; i_x < WIDTH; ++i_x)
            {

                float x_0 = -0.5 + ((((float)i_x -  WIDTH / 2)  * zoom) + x_offset) / WIDTH  * 2.5;
                float y_0 =        ((((float)i_y - HEIGHT / 2)  * zoom) + y_offset) / HEIGHT * 2.5;  

                float X = 0;
                float Y = 0;
                
                struct Color color = BLACK;

                for (int N = 0; N < N_max; N++)
                {
                    float x2 = X * X;
                    float y2 = Y * Y;
                    float xy = X * Y;

                    float r2 = x2 + y2;

                    if (r2 >= r2_max)
                    {
                        color = WHITE;
                        break;
                    }

                    X = x2 - y2 + x_0;
                    Y = xy + xy + y_0;
                }

                DrawPixel(i_x, i_y, color);
            }
        }

        DrawFPS(10, 10);

        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}