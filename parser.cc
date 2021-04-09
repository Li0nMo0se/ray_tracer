#include "parser.hh"
#include "camera.hh"
#include "metaball.hh"
#include "plan.hh"
#include "point_light.hh"
#include "raybox.hh"
#include "sphere.hh"
#include "triangle.hh"
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
    return space::Vector3(x, y, z);
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
    const color::Color3 color = parse_vector(color_str);

    float kd, ks, ns;
    ss >> kd >> ks >> ns;

    if (texture_type == "UniformTexture")
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
    const space::Vector3 origin = parse_vector(origin_str);

    float radius;
    ss >> radius;

    const std::shared_ptr<scene::TextureMaterial> texture = get_texture(ss);
    return std::make_shared<scene::Sphere>(origin, radius, texture);
}

std::shared_ptr<scene::Object> Parser::parse_plan(const std::string& line)
{
    std::stringstream ss(line);
    std::string tmp;
    ss >> tmp; // Plan

    std::string origin_str;
    ss >> origin_str;
    const space::Vector3 origin = parse_vector(origin_str);

    std::string normal_str;
    ss >> normal_str;
    const space::Vector3 normal = parse_vector(normal_str);

    const std::shared_ptr<scene::TextureMaterial> texture = get_texture(ss);

    return std::make_shared<scene::Plan>(origin, normal, texture);
}

std::shared_ptr<scene::Object> Parser::parse_triangle(const std::string& line)
{
    std::stringstream ss(line);
    std::string tmp;
    ss >> tmp; // Triangle

    std::string A_str;
    ss >> A_str;
    const space::Vector3 A = parse_vector(A_str);

    std::string B_str;
    ss >> B_str;
    const space::Vector3 B = parse_vector(B_str);

    std::string C_str;
    ss >> C_str;
    const space::Vector3 C = parse_vector(C_str);

    const std::shared_ptr<scene::TextureMaterial> texture = get_texture(ss);

    return std::make_shared<scene::Triangle>(A, B, C, texture);
}

std::shared_ptr<scene::Object> Parser::parse_raybox(const std::string& line)
{

    std::stringstream ss(line);
    std::string tmp;
    ss >> tmp; // Raybox

    std::string lower_bound_str;
    ss >> lower_bound_str;
    const space::Vector3 lower_bound = parse_vector(lower_bound_str);

    std::string higher_bound_str;
    ss >> higher_bound_str;
    const space::Vector3 higher_bound = parse_vector(higher_bound_str);

    return std::make_shared<scene::RayBox>(lower_bound,
                                           higher_bound,
                                           get_texture(ss));
}

std::shared_ptr<scene::TextureMaterial>
Parser::get_texture(std::stringstream& ss)
{
    std::string texture_name;
    ss >> texture_name;
    auto it = textures_.find(texture_name);
    if (it == textures_.end())
        throw ParseError("No such texture " + texture_name, nb_line_);
    return it->second;
}

std::shared_ptr<scene::Object> Parser::parse_metaball(const std::string& line)
{
    std::stringstream ss(line);
    std::string tmp;
    ss >> tmp; // Metaball

    float step;
    ss >> step;

    const std::shared_ptr<scene::TextureMaterial> texture = get_texture(ss);

    std::string potential_point;
    std::vector<space::Point3> potentials;
    while (ss >> potential_point)
    {
        const space::Point3 potential = parse_vector(potential_point);
        potentials.push_back(potential);
    }

    return std::make_shared<scene::Metaball>(potentials, step, texture);
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
    nb_line_ = 1;
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
        nb_line_++;
        if (!(line.empty() || line[0] == '#'))
            break;
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
                scene.add_object(parse_sphere(line));
            else if (curr_token == "PointLight")
                scene.add_light(parse_pointlight(line));
            else if (curr_token == "Plan")
                scene.add_object(parse_plan(line));
            else if (curr_token == "Triangle")
                scene.add_object(parse_triangle(line));
            else if (curr_token == "Metaball")
                scene.add_object(parse_metaball(line));
            else if (curr_token == "Raybox")
                scene.add_object(parse_raybox(line));
            else
                throw ParseError("Undefined structure: " + curr_token,
                                 nb_line_);
        }
        nb_line_++;
    }

    return scene;
}
} // namespace parse