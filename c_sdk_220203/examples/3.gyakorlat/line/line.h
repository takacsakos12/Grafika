#ifndef LINE_H
#define LINE_H

typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} Color;

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    Point start;
    Point end;
    Color color;
} Line;

Line Line_create(Point start, Point end, Color color);

void Line_setStart(Line* line, Point start);
void Line_setEnd(Line* line, Point end);
void Line_setColor(Line* line, Color color);

Point Line_getStart(const Line* line);
Point Line_getEnd(const Line* line);
Color Line_getColor(const Line* line);

// Debug / kiírás
void Line_print(const Line* line);

#endif