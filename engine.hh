#pragma once

class Engine
{

private:
    
    // Size of a pixel in the real world
    // On x axis
    float unit_x_;
    // On y axis
    float unit_y_;

    // Position of the first pixel (top left) in the real world
    Vector3 top_left_pixel_;

    // Resolution of the 2D image
    unsigned int width_;
    unsigned int height_;
}