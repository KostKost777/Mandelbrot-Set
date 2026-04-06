#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <immintrin.h>
#include "raylib.h"

// g++ -O3 maldebrot.cpp -o mald -lraylib -lX11 -lpthread -ldl -lrt -lm

const int HEIGHT = 600;
const int WIDTH  = 800;

int main()
{
    InitWindow(WIDTH, HEIGHT, "MALDEBROT");

    uint8_t   N_max     = 255;
    float x_offset      = 0;
    float y_offset      = 0;
    float dx            = 1.f / (float)WIDTH;
    float dy            = 1.f / (float)HEIGHT;
    float scale         = 2.5;
    float base_offset   = -250.f;

    __m256 cnt_01234567  = _mm256_setr_ps  (0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f);
    __m256 r2_max        = _mm256_set1_ps (4.f);
    __m256 cnt_2         = _mm256_set1_ps (2.f);
    __m256 cnt_1         = _mm256_set1_ps (1.f);

    while (!WindowShouldClose()) 
    {
        if (IsKeyDown(KEY_ESCAPE))       break;

        if (IsKeyDown(KEY_LEFT))         x_offset -= 20 * dx * scale;
        if (IsKeyDown(KEY_LEFT))         x_offset -= 20 * dx * scale;
        if (IsKeyDown(KEY_RIGHT))        x_offset += 20 * dx * scale;
        if (IsKeyDown(KEY_UP))           y_offset -= 20 * dy * scale;
        if (IsKeyDown(KEY_DOWN))         y_offset += 20 * dy * scale;

        if (IsKeyDown(KEY_PAGE_UP))      scale    *=  1.1;
        if (IsKeyDown(KEY_PAGE_DOWN))    scale    /=  1.1;

        BeginDrawing();
        ClearBackground(BLACK);

        __m256 coeff = _mm256_mul_ps(_mm256_set1_ps(dx * scale), cnt_01234567);

        float y_0 = (-HEIGHT / 2) * dy * scale + y_offset;

        for (int y_i = 0; y_i < HEIGHT; y_i++, y_0 += dy * scale)
        {   
            float x_0 = (-WIDTH  / 2 + base_offset) * dx * scale + x_offset;
            
            for (int x_i = 0; x_i < WIDTH; x_i += 8, x_0 += 8 * dx * scale)
            {
                __m256 x0 = _mm256_add_ps(_mm256_set1_ps(x_0), coeff);
                __m256 y0 = _mm256_set1_ps(y_0);

                __m256 X = x0;
                __m256 Y = y0;

                __m256 cmp = _mm256_setzero_ps();
                __m256 N   = _mm256_setzero_ps();

                for (uint8_t N_i = 0; N_i < N_max; N_i++)
                {
                    __m256 x2 = _mm256_mul_ps(X, X);
                    __m256 y2 = _mm256_mul_ps(Y, Y);
                    __m256 xy = _mm256_mul_ps(X, Y);

                    __m256 r2 = _mm256_add_ps(x2, y2);

                    cmp = _mm256_cmp_ps(r2, r2_max, _CMP_LE_OS);

                    int mask = _mm256_movemask_ps(cmp);
                    if (!mask) break;

                    X = _mm256_sub_ps(_mm256_add_ps(x2, x0), y2);
                    Y = _mm256_fmadd_ps(cnt_2, xy, y0);

                    cmp = _mm256_and_ps(cmp, cnt_1);
                    N = _mm256_add_ps(N, cmp);
                }
                
                for (int i = 0; i < 8; ++i) 
                    DrawPixel(x_i + i, y_i, (Color){0, (uint8_t)N[i] , (uint8_t)(N[i] * 0.5) , 150});
            }
        }

        DrawFPS(10, 10);

        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}