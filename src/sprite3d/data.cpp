#include <config.h>

#include "sprite3d/data.hpp"

#include <iostream>
#include <physfs.h>
#include <string.h>
#include <sstream>
#include <stdexcept>
#include "util.hpp"
#include "globals.hpp"
#include "display/texture_manager.hpp"
#include "display/texture.hpp"

namespace sprite3d {

static const int FORMAT_VERSION = 2;

static inline float read_float(PHYSFS_file* file)
{
  uint32_t int_result;
  if(PHYSFS_readULE32(file, &int_result) == 0) {
    std::ostringstream msg;
    msg << "Problem reading float value: " << PHYSFS_getLastError();
    throw std::runtime_error(msg.str());
  }

  // is this platform independent?
  return * ( reinterpret_cast<float*> (&int_result) );
}

static inline uint16_t read_uint16_t(PHYSFS_file* file)
{
  uint16_t result;
  if(PHYSFS_readULE16(file, &result) == 0) {
    std::ostringstream msg;
    msg << "Problem reading uint16 value: " << PHYSFS_getLastError();
    throw std::runtime_error(msg.str());
  }
  return result;
}

static inline std::string read_string(PHYSFS_file* file, size_t size)
{
  char buffer[size+1];
  if(PHYSFS_read(file, buffer, size, 1) != 1) {
    std::ostringstream msg;
    msg << "Problem reading string value: " << PHYSFS_getLastError();
    throw std::runtime_error(msg.str());
  }
  buffer[size] = 0;

  return buffer;
}

Data::Data(const std::string& filename)
{
  PHYSFS_file* file = PHYSFS_openRead(filename.c_str());
  if(!file) {
    std::ostringstream msg;
    msg << "Couldn't open '" << filename << "': "
        << PHYSFS_getLastError();
    throw std::runtime_error(msg.str());
  }

  try {
    std::string magic = read_string(file, 4);
    if(magic != "W3DS")
      throw std::runtime_error("Not a windstille 3d sprite file");
    uint16_t format_version = read_uint16_t(file);
    if(format_version > FORMAT_VERSION)
      throw std::runtime_error("sprite file format too new");
    if(format_version < FORMAT_VERSION)
      throw std::runtime_error("sprite file format too old");

    uint16_t mesh_count = read_uint16_t(file);
    if(mesh_count == 0)
      throw std::runtime_error("Sprite3D contains no meshs");
    uint16_t attachment_point_count = read_uint16_t(file);
    uint16_t action_count = read_uint16_t(file);
    if(action_count == 0)
      throw std::runtime_error("Sprite3D contains no actions");

    // read meshs
    meshs.resize(mesh_count);
    for(std::vector<Mesh>::iterator i = meshs.begin(); i != meshs.end(); ++i) {
      Mesh& mesh = *i;

      std::string texturename = read_string(file, 64);
      texturename = dirname(filename) + basename(texturename);
      mesh.triangle_count = read_uint16_t(file);
      mesh.vertex_count   = read_uint16_t(file);

      mesh.texture = texture_manager->get(texturename);

      // read triangles
      mesh.vertex_indices.reserve(mesh.triangle_count * 3);
      for(uint16_t v = 0; v < mesh.triangle_count * 3; ++v) {
        mesh.vertex_indices.push_back(read_uint16_t(file));
      }
      
      mesh.normals.reserve(mesh.triangle_count * 3);
      for(uint16_t n = 0; n < mesh.triangle_count * 3; ++n) {
        mesh.normals.push_back(read_float(file));
      }

      mesh.tex_coords.reserve(mesh.vertex_count * 2);
      for(uint16_t v = 0; v < mesh.vertex_count * 2; ++v) {
        mesh.tex_coords.push_back(read_float(file));
      }
    }

    // read attachment points
    attachment_points.reserve(attachment_point_count);
    for(uint16_t a = 0; a < attachment_point_count; ++a) {
      attachment_points.push_back(read_string(file, 64));
    }

    // read actions
    actions.resize(action_count);
    for(std::vector<Action>::iterator i = actions.begin(); i != actions.end(); ++i) {
      Action& action = *i;

      action.name = read_string(file, 64);
      action.speed = read_float(file);
      uint16_t marker_count = read_uint16_t(file);
      uint16_t frame_count  = read_uint16_t(file);

      // read markers
      action.markers.resize(marker_count);
      for(uint16_t m = 0; m < action.markers.size(); ++m) {
        Marker& marker = action.markers[m];
        marker.name  = read_string(file, 64);
        marker.frame = read_uint16_t(file);
      }

      // read frames
      action.frames.resize(frame_count);
      for(uint16_t f = 0; f < action.frames.size(); ++f) {
        ActionFrame& frame = action.frames[f];
        
        frame.meshs.resize(mesh_count);
        for(uint16_t m = 0; m < mesh_count; ++m) {
          MeshVertices& mesh = frame.meshs[m];

          mesh.vertices.resize(meshs[m].vertex_count * 3);
          for(uint16_t v = 0; v < meshs[m].vertex_count * 3; ++v) {
            mesh.vertices[v] = read_float(file);
          }
        }

        frame.attachment_points.resize(attachment_point_count);
        for(uint16_t a = 0; a < attachment_point_count; ++a) {
          AttachmentPointPosition& point = frame.attachment_points[a];

          point.pos.x = read_float(file);
          point.pos.y = read_float(file);
          point.pos.z = read_float(file);

          point.quat.w = -read_float(file);
          point.quat.x =  read_float(file);
          point.quat.y =  read_float(file);
          point.quat.z =  read_float(file);
          point.quat.normalize();
        }
      }
    }
  } catch(std::exception& e) {
    PHYSFS_close(file);
    std::ostringstream msg;
    msg << "Problem while reading '" << filename << "': " << e.what();
    throw std::runtime_error(msg.str());
  }
  PHYSFS_close(file);
}

Data::~Data()
{
}

const Action&
Data::get_action(const std::string& name) const
{
  for(std::vector<Action>::const_iterator action = actions.begin(); action != actions.end(); ++action) {
    if(action->name == name)
      return *action;
  }

  if (actions.empty())
    {
      std::ostringstream msg;
      msg << "No action with name '" << name << "' defined";
      throw std::runtime_error(msg.str());
    }
  else
    {
      std::cout << "No action with name '" << name << "' defined" << std::endl;
      return actions.front();
    }
}

const Marker&
Data::get_marker(const Action* action, const std::string& name) const
{
  for(uint16_t m = 0; m < action->markers.size(); ++m) {
    if(action->markers[m].name == name)
      return action->markers[m];
  }
  std::ostringstream msg;
  msg << "No marker with name '" << name << "' defined in action '"
      << action->name << "'";
  throw std::runtime_error(msg.str());
}

uint16_t
Data::get_attachment_point_id(const std::string& name) const
{
  for(uint16_t a = 0; a < attachment_points.size(); ++a) {
    if(attachment_points[a] == name)
      return a;
  }

  std::ostringstream msg;
  msg << "No Attachment Point with name '" << name << "' defined";
  throw std::runtime_error(msg.str());
}

} // namespace sprite3d

/* EOF */
