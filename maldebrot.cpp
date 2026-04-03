#include <stdio.h>
#include "raylib.h"

int main()
{
    InitWindow(800, 600, "MALDEBROT");

    int N_max = 255;
    float r2_max = 2;
    bool is_in_set = false;

    while (!WindowShouldClose()) 
    {
        if (IsKeyPressed(KEY_ESCAPE)) {
            break;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        for (int i_y = 0; i_y < 600; ++i_y)
        {   

            float ci = -1.2 + (float)i_y / 600 * 2.4;

            for (int i_x = 0; i_x < 800; ++i_x)
            {
                float cr = -2.0 + (float)i_x / 800 * 3.0;  

                float X = 0;
                float Y = 0;
                
                struct Color color = WHITE;

                for (int N = 0; N < N_max; N++)
                {
                    float x2 = X * X;
                    float y2 = Y * Y;
                    float xy = X * Y;

                    float r2 = x2 + y2;

                    if (r2 >= r2_max) 
                        color = BLACK;
                        break;

                    X = x2 - y2 + ci;
                    Y = xy + xy + cr;
                }

                DrawPixel(i_x, i_y, color);
            }
        }

        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}