#include "engine.hh"
#include "parser.hh"
#include "scene.hh"

#include <iostream>
#include <sstream>

#define M_PI 3.14159265358979323846 /* pi */

int main(int argc, char* argv[])
{
    if (argc != 5)
    {
        std::cerr << "Usage: " << argv[0]
                  << "file.scene outputfile.ppn width height\n";
        return EXIT_FAILURE;
    }
    unsigned int width;
    std::stringstream ss_width(argv[3]);
    ss_width >> width;

    unsigned int height;
    std::stringstream ss_height(argv[3]);
    ss_height >> height;

    parse::Parser parser;
    const scene::Scene scene = parser.parse_scene(argv[1]);
    rendering::Engine::render(argv[2], width, height, scene);

    return EXIT_SUCCESS;
}