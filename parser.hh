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
        : msg_(msg)
        , nb_line_(nb_line)
    {
    }

    virtual const char* what() const noexcept override
    {
        std::string msg = "Line: " + std::to_string(nb_line_) + ". " + msg_;
        return msg.c_str();
    }

  private:
    const std::string msg_;
    const unsigned int nb_line_;
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
    std::shared_ptr<scene::Light> parse_pointlight(const std::string& line);

    scene::Camera parse_camera(const std::string& line);
    unsigned int nb_line_;
    std::unordered_map<std::string, std::shared_ptr<scene::TextureMaterial>>
        textures_;
};
} // namespace parse