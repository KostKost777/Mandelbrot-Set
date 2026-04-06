#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "raylib.h"

// g++ maldebrot.cpp -o mald -lraylib -lX11 -lpthread -ldl -lrt -lm

const int HEIGHT = 600;
const int WIDTH  = 800;

void mm128_set_ps(float dst[8], float x);
void mm128_add_ps(float dst[8], float src[8]);
void mm128_sub_ps(float dst[8], float src[8]);
void mm128_mul_ps(float dst[8], float src[8]);
void mm128_cpy_ps(float dst[8], float src[8]);
void mm128_cmp_ps(float a[8], float b[8], int cmp[8]);

void mm128_set_ps(float dst[8], float x)
{
    for (int i = 0; i < 8; i++) dst[i] = x;
}

void mm128_add_ps(float dst[8], float src[8])
{
    for (int i = 0; i < 8; i++) dst[i] += src[i];
}

void mm128_sub_ps(float dst[8], float src[8])
{
    for (int i = 0; i < 8; i++) dst[i] -= src[i];
}

void mm128_mul_ps(float dst[8], float src[8])
{
    for (int i = 0; i < 8; i++) dst[i] *= src[i];
}

void mm128_cpy_ps(float dst[8], float src[8])
{
    for (int i = 0; i < 8; i++) dst[i] = src[i];
}

void mm128_cmp_ps(float a[8], float b[8], int cmp[8])
{
    for (int i = 0; i < 8; i++) a[i] <= b[i] ? cmp[i] = 1 : cmp[i] = 0;
}

int main()
{
    InitWindow(WIDTH, HEIGHT, "MALDEBROT");

    int   N_max     = 256;
    float x_offset  = 0;
    float y_offset  = 0;
    float dx        = 1.f / (float)WIDTH;
    float dy        = 1.f / (float)HEIGHT;
    float zoom      = 2.5;
    float _0123[8]  = {0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f};
    float r2_max[8] = {4.f, 4.f, 4.f, 4.f, 4.f, 4.f, 4.f, 4.f};

    while (!WindowShouldClose()) 
    {
        if (IsKeyDown(KEY_ESCAPE))       break;

        if (IsKeyDown(KEY_LEFT))         x_offset -= 10 * dx * zoom;
        if (IsKeyDown(KEY_RIGHT))        x_offset += 10 * dx * zoom;
        if (IsKeyDown(KEY_UP))           y_offset -= 10 * dy * zoom;
        if (IsKeyDown(KEY_DOWN))         y_offset += 10 * dy * zoom;
        if (IsKeyDown(KEY_PAGE_UP))      zoom     *=  1.1;
        if (IsKeyDown(KEY_PAGE_DOWN))    zoom     /=  1.1;

        BeginDrawing();
        ClearBackground(BLACK);

        for (int y_i = 0; y_i < HEIGHT; y_i++)
        {   
            float coeff[8] = {}; for (int i = 0; i < 8; i++) coeff[i] = dx * zoom * _0123[i];

            for (int x_i = 0; x_i < WIDTH; x_i += 8)
            {

                float x_0 = (((float)x_i - WIDTH  / 2) * dx) * zoom + x_offset;
                float y_0 = (((float)y_i - HEIGHT / 2) * dy) * zoom + y_offset; 

                float x0[8] = {}; for (int i = 0; i < 8; i++) x0[i] = x_0;
                float y0[8] = {}; for (int i = 0; i < 8; i++) y0[i] = y_0;

               // printf("x_0: %g    y_0: %g\n", dx, y_0);

                //for (int i = 0; i < 8; ++i) printf("%f %f %f %f\n", x0[0], x0[1], x0[2], x0[3]);

                for (int i = 0; i < 8; i++) x0[i] += coeff[i];

                float X[8] = {}; for (int i = 0; i < 8; i++) X[i] = x0[i];
                float Y[8] = {}; for (int i = 0; i < 8; i++) Y[i] = y0[i];
                
                int cmp[8] = {};
                int N[8]   = {};

                for (int N_i = 0; N_i < N_max; N_i++)
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

                DrawPixel(x_i,     y_i, (Color){255 - N[0], 255 - N[0], 255 - N[0], 255});
                DrawPixel(x_i + 1, y_i, (Color){255 - N[1], 255 - N[1], 255 - N[1], 255});
                DrawPixel(x_i + 2, y_i, (Color){255 - N[2], 255 - N[2], 255 - N[2], 255});
                DrawPixel(x_i + 3, y_i, (Color){255 - N[3], 255 - N[3], 255 - N[3], 255});
                DrawPixel(x_i + 4, y_i, (Color){255 - N[4], 255 - N[4], 255 - N[4], 255});
                DrawPixel(x_i + 5, y_i, (Color){255 - N[5], 255 - N[5], 255 - N[5], 255});
                DrawPixel(x_i + 6, y_i, (Color){255 - N[6], 255 - N[6], 255 - N[6], 255});
                DrawPixel(x_i + 7, y_i, (Color){255 - N[7], 255 - N[7], 255 - N[7], 255});
            }
        }

        DrawFPS(10, 10);

        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}