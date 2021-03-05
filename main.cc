#include "engine.hh"
#include "parser.hh"
#include "scene.hh"

#include <iostream>
#include <sstream>

#define M_PI 3.14159265358979323846 /* pi */

int main(int argc, char* argv[])
{
    if (argc != 7)
    {
        std::cerr << "Usage: " << argv[0]
                  << "file.scene outputfile.ppm width height aliasing_level "
                  << "reflection_max_depth\n";
        return EXIT_FAILURE;
    }
    unsigned int width;
    std::stringstream ss_width(argv[3]);
    ss_width >> width;

    unsigned int height;
    std::stringstream ss_height(argv[4]);
    ss_height >> height;

    unsigned int aliasing_level;
    std::stringstream ss_aliasing(argv[5]);
    ss_aliasing >> aliasing_level;

    unsigned int reflection_max_depth;
    std::stringstream ss_reflection(argv[6]);
    ss_reflection >> reflection_max_depth;

    parse::Parser parser;
    try
    {
        scene::Scene scene = parser.parse_scene(argv[1]);
        rendering::Engine::render(argv[2],
                                  width,
                                  height,
                                  scene,
                                  aliasing_level,
                                  reflection_max_depth);
    }
    catch (const parse::ParseError& e)
    {
        std::string str = e.what();
        std::cerr << str << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}