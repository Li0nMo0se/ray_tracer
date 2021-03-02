#include "parser.hh"
#include "camera.hh"
#include "point_light.hh"
#include "sphere.hh"
#include "uniform_texture.hh"
#include <fstream>
#include <sstream>

#include <iostream>

namespace parse
{
space::Vector3 Parser::parse_vector(const std::string& vector)
{
    float x, y, z;
    // Not accurate
    sscanf(vector.c_str(), "(%f,%f,%f)", &x, &y, &z);
    return space::Vector3({x, y, z});
}
/* Parse a line which describe the camera */
scene::Camera Parser::parse_camera(const std::string& line)
{
    std::stringstream ss(line);
    std::string tmp;
    ss >> tmp;
    if (tmp != "Camera")
        throw ParseError("Was excepting a Camera keyword.", nb_line_);
    ss >> tmp;
    const space::Vector3 origin = parse_vector(tmp);
    ss >> tmp;
    const space::Vector3 y_axis = parse_vector(tmp);
    ss >> tmp;
    const space::Vector3 z_axis = parse_vector(tmp);
    float z_min;
    ss >> z_min;
    float alpha;
    ss >> alpha;
    // convert to radian
    alpha = alpha * M_PI / 180.f;
    float beta;
    ss >> beta;
    // convert to radian
    beta = beta * M_PI / 180.f;

    return scene::Camera(origin, y_axis, z_axis, z_min, alpha, beta);
}

void Parser::parse_texture(const std::string& line)
{
    std::stringstream ss(line);
    std::string texture_type;
    ss >> texture_type; // UniformTexture, ...
    std::string texture_name;
    ss >> texture_name;

    std::string color_str;
    ss >> color_str;
    color::Color3 color = parse_vector(color_str);

    float kd, ks, ns;
    ss >> kd >> ks >> ns;

    if ("UniformTexture")
    {
        textures_.insert(
            {texture_name,
             std::make_shared<scene::UniformTexture>(color, kd, ks, ns)});
    }
}

std::shared_ptr<scene::Object> Parser::parse_sphere(const std::string& line)
{
    std::stringstream ss(line);
    std::string tmp;
    ss >> tmp; // Sphere

    std::string origin_str;
    ss >> origin_str;
    space::Vector3 origin = parse_vector(origin_str);

    float radius;
    ss >> radius;

    std::string texture_name;
    ss >> texture_name;
    auto it = textures_.find(texture_name);
    if (it == textures_.end())
        throw ParseError("No such texture " + texture_name, nb_line_);

    std::shared_ptr<scene::TextureMaterial> texture = it->second;
    return std::make_shared<scene::Sphere>(origin, radius, texture);
}

std::shared_ptr<scene::Light> Parser::parse_pointlight(const std::string& line)
{
    std::stringstream ss(line);
    std::string tmp;
    ss >> tmp; // PointLight

    std::string origin_str;
    ss >> origin_str;
    space::Vector3 origin = parse_vector(origin_str);

    float intensity;
    ss >> intensity;
    return std::make_shared<scene::PointLight>(origin, intensity);
}

scene::Scene Parser::parse_scene(const std::string filename)
{
    nb_line_ = 0;
    std::ifstream in(filename);
    if (!in)
    {
        throw std::invalid_argument(
            "Cannot parse the scene: no such file: " + filename + ".");
    }

    // File is valid
    std::string line;
    while (std::getline(in, line))
    {
        if (!(line.empty() || line[0] == '#'))
            break;
        nb_line_++;
    }
    scene::Camera camera = parse_camera(line);
    scene::Scene scene(camera);

    while (std::getline(in, line))
    {
        if (!(line.empty() || line[0] == '#'))
        {
            std::stringstream ss(line);
            std::string curr_token;
            ss >> curr_token;
            if (curr_token == "UniformTexture")
                parse_texture(line);
            else if (curr_token == "Sphere")
            {
                std::shared_ptr<scene::Object> sphere = parse_sphere(line);
                scene.add_object(sphere);
            }
            else if (curr_token == "PointLight")
            {
                std::shared_ptr<scene::Light> light = parse_pointlight(line);
                scene.add_light(light);
            }
            else
                throw ParseError("Undefined structure", nb_line_);
        }
        nb_line_++;
    }

    return scene;
}
} // namespace parse