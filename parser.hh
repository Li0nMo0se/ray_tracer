#pragma once

#include "scene.hh"
#include <exception>
#include <string>
#include <unordered_map>

namespace parse
{
class ParseError : public std::exception
{
  public:
    ParseError(const std::string& msg, const unsigned int nb_line)
        : msg_("Line " + std::to_string(nb_line) + ": " + msg)
    {
    }

    virtual const char* what() const noexcept { return msg_.c_str(); }

  private:
    const std::string msg_;
};

class Parser final
{
  public:
    Parser() = default;
    ~Parser() = default;

    Parser(const Parser&) = delete;
    Parser operator=(const Parser&) = delete;

    scene::Scene parse_scene(const std::string filename);

  private:
    space::Vector3 parse_vector(const std::string& vector);
    void parse_texture(const std::string& line);

    std::shared_ptr<scene::Object> parse_sphere(const std::string& line);
    std::shared_ptr<scene::Object> parse_plan(const std::string& line);
    std::shared_ptr<scene::Object> parse_triangle(const std::string& line);
    std::shared_ptr<scene::Object> parse_metaball(const std::string& line);

    std::shared_ptr<scene::Light> parse_pointlight(const std::string& line);

    std::shared_ptr<scene::TextureMaterial> get_texture(std::stringstream& ss);

    scene::Camera parse_camera(const std::string& line);
    unsigned int nb_line_;
    std::unordered_map<std::string, std::shared_ptr<scene::TextureMaterial>>
        textures_;
};
} // namespace parse