#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>

#ifndef BENCHMARK_MODE

    #include "raylib.h"
    #define BREAK_CASE (!WindowShouldClose())

#else

    const int TOTAL_FRAMES = 1000;
    #define BREAK_CASE (frame_counter < TOTAL_FRAMES)

#endif

// g++ -O3 maldebrot.cpp -o mald -lraylib -lX11 -lpthread -ldl -lrt -lm

const int HEIGHT = 600;
const int WIDTH  = 800;

static inline void mm256_set_ps (float dst[8], float x);
static inline void mm256_add_ps (float dst[8], float src[8]);
static inline void mm256_sub_ps (float dst[8], float src[8]);
static inline void mm256_mul_ps (float dst[8], float src[8]);
static inline void mm256_cpy_ps (float dst[8], float src[8]);
static inline void mm256_cmp_ps (float a[8], float b[8], int cmp[8]);

static inline void mm256_set_ps(float dst[8], float x)
{
    for (int i = 0; i < 8; i++) dst[i] = x;
}

static inline void mm256_add_ps(float dst[8], float src[8])
{
    for (int i = 0; i < 8; i++) dst[i] += src[i];
}

static inline void mm256_sub_ps(float dst[8], float src[8])
{
    for (int i = 0; i < 8; i++) dst[i] -= src[i];
}

static inline void mm256_mul_ps(float dst[8], float src[8])
{
    for (int i = 0; i < 8; i++) dst[i] *= src[i];
}

static inline void mm256_cpy_ps(float dst[8], float src[8])
{
    for (int i = 0; i < 8; i++) dst[i] = src[i];
}

static inline void mm256_cmp_ps(float a[8], float b[8], int cmp[8])
{
    for (int i = 0; i < 8; i++) a[i] <= b[i] ? cmp[i] = 1 : cmp[i] = 0;
}

int main()
{
    #ifndef BENCHMARK_MODE

        InitWindow(WIDTH, HEIGHT, "MALDEBROT");

    #else

        int frame_counter = 0;

    #endif

    uint8_t   N_max     = 255;
    float x_offset      = 0;
    float y_offset      = 0;
    float dx            = 1.f / (float)WIDTH;
    float dy            = 1.f / (float)HEIGHT;
    float scale         = 2.5;
    float base_offset   = -250.f;

    float _01234567[8]  = {0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f};
    float r2_max[8]     = {4.f, 4.f, 4.f, 4.f, 4.f, 4.f, 4.f, 4.f};

    while (BREAK_CASE) 
    {
        #ifndef  BENCHMARK_MODE

            clock_t start_time = clock();

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

        #endif

        float coeff[8] = {}; mm256_set_ps(coeff, dx * scale);

        mm256_mul_ps(coeff, _01234567);

        float y_0 = (-HEIGHT / 2) * dy * scale + y_offset;

        for (int y_i = 0; y_i < HEIGHT; y_i++, y_0 += dy * scale)
        {   

            float x_0 = (-WIDTH  / 2 + base_offset) * dx * scale + x_offset;
            

            for (int x_i = 0; x_i < WIDTH; x_i += 8, x_0 += 8 * dx * scale)
            {
                float x0[8] = {}; mm256_set_ps(x0, x_0);
                float y0[8] = {}; mm256_set_ps(y0, y_0);

                mm256_add_ps(x0, coeff);

                float X[8] = {}; mm256_cpy_ps(X, x0);
                float Y[8] = {}; mm256_cpy_ps(Y, y0);

                int cmp[8]              = {};
                volatile uint8_t N[8]   = {};

                for (uint8_t N_i = 0; N_i < N_max; N_i++)
                {
                    float x2[8] = {}; mm256_cpy_ps(x2, X);
                    float y2[8] = {}; mm256_cpy_ps(y2, Y);
                    float xy[8] = {}; mm256_cpy_ps(xy, X);

                    mm256_mul_ps(x2, x2);
                    mm256_mul_ps(y2, y2);
                    mm256_mul_ps(xy, Y);

                    float r2[8] = {}; mm256_cpy_ps(r2, x2);
                    
                    mm256_add_ps(r2, y2);

                    int cmp[8] = {};

                    mm256_cmp_ps(r2, r2_max, cmp);

                    int mask = 0;
                    for (int i = 0 ; i < 8; ++i) mask += cmp[i] << i;
                    if (!mask)
                        break;

                    mm256_cpy_ps(X, x2);
                    mm256_sub_ps(X, y2);
                    mm256_add_ps(X, x0);

                    mm256_cpy_ps(Y, xy);
                    mm256_add_ps(Y, xy);
                    mm256_add_ps(Y, y0);

                    for (int i = 0; i < 8; ++i) N[i] += cmp[i];
                }

                #ifndef BENCHMARK_MODE

                    for (int i = 0; i < 8; ++i) 
                        DrawPixel(x_i + i, y_i, (Color){0, (uint8_t)N[i], 
                                                           (uint8_t)(N[i] * 0.5) , 150});

                #endif
            }
        }

        #ifndef BENCHMARK_MODE

            EndDrawing();

            clock_t end_time = clock();
            double delta_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
            
            printf("\rFPS: %.1lf   ", 1 / delta_time);
            fflush(stdout);
            
        #else

            //printf("FRAMES: %d\n", frame_counter);
            frame_counter++;

        #endif
    }
    
    #ifndef BENCHMARK_MODE

        CloseWindow();

    #endif

    return 0;
}