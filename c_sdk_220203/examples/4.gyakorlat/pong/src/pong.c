#include "pong.h"

void init_pong(Pong* pong, int width, int height)
{
    pong->width = width;
    pong->height = height;
    init_pad(&(pong->left_pad), 0, height, RED_THEME);
    init_pad(&(pong->right_pad), width - 50, height, GREEN_THEME);
    init_ball(&(pong->ball), width / 2, height / 2);
}

void update_pong(Pong* pong, double time)
{
    update_pad(&(pong->left_pad), time);
    update_pad(&(pong->right_pad), time);
    update_ball(&(pong->ball), time);
    bounce_ball(pong);
}

void render_pong(Pong* pong)
{
    render_pad(&(pong->left_pad));
    render_pad(&(pong->right_pad));
    render_ball(&(pong->ball));
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
