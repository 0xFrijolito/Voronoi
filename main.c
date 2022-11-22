#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define HEIGTH 1200
#define WIDTH 1200

static uint32_t image[HEIGTH][WIDTH];

typedef uint32_t Color;
typedef struct point {
    float x;
    float y;
} Point;  

float square_distance (Point p1, Point p2) {
    float dx = p1.x - p2.x;
    float dy = p1.y - p2.y;

    return sqrt(dx*dx + dy*dy);
} 

float random (float min, float max) {
    return ((max - min) * ((float)rand() / RAND_MAX)) + min;
}

void safe_ppm_file (const char* filepath) {
    // Open file 📂
    FILE *fp = fopen(filepath, "wb");
    
    // Write header 📃
    fprintf(fp, "P6\n%d %d 255\n", WIDTH, HEIGTH);
    
    // Write body
    for (size_t y=0 ; y<HEIGTH ; y++) {
        for (size_t x=0 ; x<WIDTH ; x++) {
            // Get color
            uint32_t color = image[y][x];

            // Obtain pixel
            uint8_t pixel[3];
            pixel[0] = (color & 0x0000FF) >> 0;  // 🔴
            pixel[1] = (color & 0x00FF00) >> 8;  // 🟢
            pixel[2] = (color & 0xFF0000) >> 16; // 🔵
            
            // save pixel
            fwrite(pixel, sizeof(pixel), 1, fp);
        }
    }
    
    // close file 🔐
    fclose(fp);
}
 
void render_voronoi (Point points[], Color colors[], size_t n) {
    for (size_t y=0 ; y<HEIGTH ; y++) {
        for (size_t x=0 ; x<WIDTH ; x++) {
            float minDistance = INFINITY;
            Point actualPoint = {(float)x, (float)y}; 

            for (size_t i=0 ; i<n ; i++) {
                float distance = square_distance(points[i], actualPoint);
                if (minDistance > distance) {
                    minDistance = distance;
                    image[y][x] = colors[i];
                }
            }
        }
    }
}

int main (void) {
    srand(time(NULL));

    const int n = 8;
    Point points[n];
    Color colors[n];

    Color baseColor = 0xFF000000;

    for (size_t i=0 ; i<n ; i++) {
        uint32_t colorGap = 0xFF / n;
        colors[i] =  baseColor +
            ((colorGap << 8) * i) + 
            ((colorGap << 8) * i) + 
            ((colorGap << 8) * i);

        points[i].x = random(0, WIDTH);
        points[i].y = random(0, HEIGTH);
    }

    render_voronoi(points, colors, n);
    safe_ppm_file("output.ppm");
}