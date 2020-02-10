#pragma once

#include <string>
#include <iostream>
#include <fstream>

/* RENDERER: */
//use: #define <renderer_name>

#define IRIS_RENDERER

/* (Iris) RENDER MODE (Definitions):

IRIS_RENDER_COLOR: Default mode.
IRIS_RENDER_NORMALS: Render 3D normals instead of the model's colour.
IRIS_RENDER_NORMALS2D: Render screen-space normals.
IRIS_RENDER_DEPTH: Render monochromatically with brightness = depth.
*/
enum RenderMode {
  IRIS_RENDER_COLOR, IRIS_RENDER_NORMALS, IRIS_RENDER_NORMALS2D, IRIS_DEPTH
};


const char* pixel_shader_path;
const char* vertex_shader_path;
void define_shaders();
std::string load_file_as_string(std::string file);

#ifdef IRIS_RENDERER
  void define_shaders()
  {

    pixel_shader_path = "native-extensions/iris-renderer/colour.glsl";
    vertex_shader_path = "native-extensions/iris-renderer/vertex.glsl";
  }
#elif defined(CUSTOM_RENDERER)
  void define_shaders()
  {
    pixel_shader_path = "Edit fragment path";
    vertex_shader_path = "Edit vertex path";
  }
#else
  #warning "[Fatal] No renderer selected. To use the default renderer, define IRIS_RENDERER."
#endif

std::string load_file_as_string(std::string file)
{
    std::ifstream ifile;
    std::string result, line;

    ifile.open(file);
    if(!ifile) std::cerr << "[Warning!] Unable to open \"" << file << "\".\n";
    while(getline(ifile, line)) result = result + line + "\n";

    return result;
}
