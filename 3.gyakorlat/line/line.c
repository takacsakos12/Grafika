#include <stdio.h>
#include <stdbool.h>
#include <SDL2\SDL.h>
#include "line.h"

Line Line_create(Point start, Point end, Color color) {
    Line line;
    line.start = start;
    line.end = end;
    line.color = color;
    return line;
}

void Line_setStart(Line* line, Point start) {
    line->start = start;
}

void Line_setEnd(Line* line, Point end) {
    line->end = end;
}

void Line_setColor(Line* line, Color color) {
    line->color = color;
}

Point Line_getStart(const Line* line) {
    return line->start;
}

Point Line_getEnd(const Line* line) {
    return line->end;
}

Color Line_getColor(const Line* line) {
    return line->color;
}

void Line_print(const Line* line) {
    printf("Start: (%d, %d)\n", line->start.x, line->start.y);
    printf("End: (%d, %d)\n", line->end.x, line->end.y);
    printf("Color: R=%d G=%d B=%d\n",
           line->color.r,
           line->color.g,
           line->color.b);
}