#include <stdio.h>
#include "raylib.h"

const int HIEGHT = 600;
const int WIDTH  = 800;

int main()
{
    InitWindow(WIDTH, HIEGHT, "MALDEBROT");

    int N_max = 255;
    float r2_max = 2;
    bool is_in_set = false;
    float R = 100.f;

    while (!WindowShouldClose()) 
    {
        if (IsKeyPressed(KEY_ESCAPE)) {
            break;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        for (int i_y = 0; i_y < 600; ++i_y)
        {   

            for (int i_x = 0; i_x < 800; ++i_x)
            {
                float X = i_x - WIDTH  / 2;
                float Y = i_y - HIEGHT / 2;

                if (X * X + Y * Y <= R * R)
                DrawPixel(i_x, i_y, WHITE);
            }
        }

        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}