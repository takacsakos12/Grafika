#include "pong.h"
#include <GL/gl.h>
#include <math.h>
#include <stdio.h>

void init_pong(Pong* pong, int width, int height)
{
    pong->width = width;
    pong->height = height;
    init_pad(&(pong->left_pad), 0, height, RED_THEME);
    init_pad(&(pong->right_pad), width - 50, height, GREEN_THEME);
    init_ball(&(pong->ball), width / 2, height / 2);
    pong->left_score = 0;
    pong->right_score = 0;
}

void update_pong(Pong* pong, double time)
{
    Ball* b = &(pong->ball);

    update_pad(&(pong->left_pad), time);
    update_pad(&(pong->right_pad), time);
    update_ball(&(pong->ball), time);
    bounce_ball(pong);

    if (b->x + b->radius < pong->left_pad.x) {
    pong->right_score++;
    init_ball(&(pong->ball), pong->width/2, pong->height/2);
    }

    if (b->x - b->radius > pong->right_pad.x + pong->right_pad.width) {
    pong->left_score++;
        init_ball(&(pong->ball), pong->width/2, pong->height/2);
}
}

static void draw_score_lines(const Pong* pong)
{
    int i;

    glColor3f(1.0, 1.0, 1.0);
    glLineWidth(3);

    // bal jatekos
    for (i = 0; i < pong->left_score; i++) {
        float x = 20 + i * 10;

        glBegin(GL_LINES);
        glVertex2f(x, 20);
        glVertex2f(x, 50);
        glEnd();
    }

    // jobb jatekos
    for (i = 0; i < pong->right_score; i++) {
        float x = pong->width - 20 - i * 10;

        glBegin(GL_LINES);
        glVertex2f(x, 20);
        glVertex2f(x, 50);
        glEnd();
    }
}

void render_pong(Pong* pong)
{
    render_pad(&(pong->left_pad));
    render_pad(&(pong->right_pad));
    render_ball(&(pong->ball));

    draw_score_lines(pong);
}

void set_left_pad_position(Pong* pong, float position)
{
    set_pad_position(&(pong->left_pad), position);
}

void set_left_pad_speed(Pong* pong, float speed)
{
    set_pad_speed(&(pong->left_pad), speed);
}

void set_right_pad_position(Pong* pong, float position)
{
    set_pad_position(&(pong->right_pad), position);
}

void set_right_pad_speed(Pong* pong, float speed)
{
    set_pad_speed(&(pong->right_pad), speed);
}

void set_ball_position(Pong* pong, float x, float y)
{
    pong->ball.x = x;
    pong->ball.y = y;
}

void set_ball_radius(Pong* pong, float radius)
{
    if (radius < 5) {
        radius = 5;
    }
    if (radius > 200) {
        radius = 200;
    }

    pong->ball.radius = radius;
}


void bounce_ball(Pong* pong)
{
    Ball* b = &pong->ball;
    Pad* left = &pong->left_pad;
    Pad* right = &pong->right_pad;

    if (b->x - b->radius <= left->x + left->width &&
        b->y >= left->y &&
        b->y <= left->y + left->height)
    {
        b->x = left->x + left->width + b->radius;
        b->speed_x *= -1;
        b->rotation_speed = fabs(b->speed_y)* 0.5f;
    }

    if (b->x + b->radius >= right->x &&
        b->y >= right->y &&
        b->y <= right->y + right->height)
    {
        b->x = right->x - b->radius;
        b->speed_x *= -1;
        b->rotation_speed = fabs(b->speed_y)* 0.5f;
    }
    if (b->y - b->radius < 0) {
        b->y = b->radius;
        b->speed_y *= -1;
        b->rotation_speed *= -1;
    }

    if (b->y + b->radius > pong->height) {
        b->y = pong->height - b->radius;
        b->speed_y *= -1;
        b->rotation_speed *= -1;
    }
}


