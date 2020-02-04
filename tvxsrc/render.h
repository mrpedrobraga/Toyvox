#include "tvxerror.h"
#include "tvxutil.h"

#ifdef IRIS_RENDERER
  //#include "iris-render.h"
  #define pixel_shader_path "native_extensions/iris_renderer/main.frag"
  #define vertex_shader_path "native_extensions/iris_renderer/main.vert"
#else
  errorlog << "[Fatal] No renderer. Use IRIS_RENDERER for a quick preview.";
#endif

using Voxel = uint32_t;
