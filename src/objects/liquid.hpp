/*
**  Windstille - A Sci-Fi Action-Adventure Game
**  Copyright (C) 2005 Ingo Ruhnke <grumbel@gmx.de>
**
**  This program is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**  
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**  
**  You should have received a copy of the GNU General Public License
**  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef HEADER_WINDSTILLE_OBJECTS_LIQUID_HPP
#define HEADER_WINDSTILLE_OBJECTS_LIQUID_HPP

#include <vector>

#include "engine/entity.hpp"

class VertexArrayDrawable;

class Liquid : public Entity
{
private:
  TexturePtr texture;
  float t;

  std::vector<float> heightfield_store1;
  std::vector<float> heightfield_store2;

  std::vector<float>* heightfield1;
  std::vector<float>* heightfield2;

  boost::shared_ptr<VertexArrayDrawable> m_water_top;
  boost::shared_ptr<VertexArrayDrawable> m_water_body;

public:
  Liquid(const FileReader& props);
  ~Liquid();

  void update(float delta);

private:
  void update_scene_graph();

private:
  Liquid (const Liquid&);
  Liquid& operator= (const Liquid&);
};

#endif

/* EOF */
