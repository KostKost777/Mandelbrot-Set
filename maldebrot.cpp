#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include "raylib.h"

// g++ maldebrot.cpp -o mald -lraylib -lX11 -lpthread -ldl -lrt -lm

const int HEIGHT = 300;
const int WIDTH  = 400;

int main()
{
    InitWindow(WIDTH, HEIGHT, "MALDEBROT");

    uint8_t  N_max      = 255;
    float x_offset      = 0;
    float y_offset      = 0;
    float dx            = 1.f / (float)WIDTH;
    float dy            = 1.f / (float)HEIGHT;
    float scale         = 2.5;

    float _01234567[8]  = {0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f};
    float r2_max[8]     = {4.f, 4.f, 4.f, 4.f, 4.f, 4.f, 4.f, 4.f};

    while (!WindowShouldClose()) 
    {
        if (IsKeyDown(KEY_ESCAPE))       break;

        if (IsKeyDown(KEY_LEFT))         x_offset -= 20 * dx * scale;
        if (IsKeyDown(KEY_RIGHT))        x_offset += 20 * dx * scale;
        if (IsKeyDown(KEY_UP))           y_offset -= 20 * dy * scale;
        if (IsKeyDown(KEY_DOWN))         y_offset += 20 * dy * scale;

        if (IsKeyDown(KEY_PAGE_UP))      scale    *=  1.1;
        if (IsKeyDown(KEY_PAGE_DOWN))    scale    /=  1.1;

        BeginDrawing();
        ClearBackground(BLACK);

        float coeff[8] = {}; for (int i = 0; i < 8; i++) coeff[i] = dx * scale * _01234567[i];

        float y_0 = (-HEIGHT / 2) * dy * scale + y_offset;

        for (int y_i = 0; y_i < HEIGHT; y_i++, y_0 += dy * scale)
        { 
            float x_0 = (-WIDTH / 2) * dx * scale + x_offset;
            
            for (int x_i = 0; x_i < WIDTH; x_i += 8, x_0 += 8 * dx * scale)
            {
                float x0[8] = {}; for (int i = 0; i < 8; i++) x0[i] = x_0;
                float y0[8] = {}; for (int i = 0; i < 8; i++) y0[i] = y_0;

               // printf("x_0: %g    y_0: %g\n", dx, y_0);

                //for (int i = 0; i < 8; ++i) printf("%f %f %f %f\n", x0[0], x0[1], x0[2], x0[3]);

                for (int i = 0; i < 8; i++) x0[i] += coeff[i];

                float X[8] = {}; for (int i = 0; i < 8; i++) X[i] = x0[i];
                float Y[8] = {}; for (int i = 0; i < 8; i++) Y[i] = y0[i];
                
                int cmp[8]     = {};
                uint8_t N[8]   = {};

                for (uint8_t N_i = 0; N_i < N_max; N_i++)
                {
                    float x2[8] = {}; for (int i = 0; i < 8; i++) x2[i] = X[i] * X[i];
                    float y2[8] = {}; for (int i = 0; i < 8; i++) y2[i] = Y[i] * Y[i];
                    float xy[8] = {}; for (int i = 0; i < 8; i++) xy[i] = X[i] * Y[i];

                    float r2[8] = {}; for (int i = 0; i < 8; i++) r2[i] = x2[i] + y2[i];

                    int cmp[8] = {};
                    for (int i = 0; i < 8; i++) r2[i] <= r2_max[i] ? cmp[i] = 1 : cmp[i] = 0;

                    int mask = 0;
                    for (int i = 0 ; i < 8; ++i) mask += cmp[i] << i;
                    if (!mask)
                        break;

                    for (int i = 0; i < 8; i++) X[i] = x2[i] - y2[i] + x0[i];
                    for (int i = 0; i < 8; i++) Y[i] = 2 * xy[i] + y0[i];

                    for (int i = 0; i < 8; ++i) N[i] += cmp[i];
                }

                //for (int i = 0; i < 8; ++i) printf("%d %d %d %d\n", N[0], N[1], N[2], N[3]);

                for (int i = 0; i < 8; ++i) DrawPixel(x_i + i, y_i, (Color){ N[i], N[i], N[i], 255});
                
            }
        }

        DrawFPS(10, 10);

        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}