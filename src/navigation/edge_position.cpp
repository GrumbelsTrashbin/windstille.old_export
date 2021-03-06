/*
**  Windstille - A Sci-Fi Action-Adventure Game
**  Copyright (C) 2007 Ingo Ruhnke <grumbel@gmx.de>
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

#include "navigation/edge_position.hpp"

#include <math.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "navigation/edge.hpp"
#include "navigation/node.hpp"
#include "display/display.hpp"

EdgePosition::EdgePosition() :
  edge(0),
  pos(0.0f)
{
}

EdgePosition::EdgePosition(Edge* edge_, float pos_) :
  edge(edge_),
  pos(pos_)
{  
}

void
EdgePosition::set_pos(Edge* edge_, float pos_)
{
  edge = edge_;
  pos     = pos_;
}

void
EdgePosition::advance(float& adv, Node*& next_node)
{
  Vector2f p1 = edge->get_node1()->get_pos();
  Vector2f p2 = edge->get_node2()->get_pos();
  
  float length = glm::length(p2 - p1);
  
  // convert from world co to [0,1] range
  float adv_01 = adv / length;

  if (adv_01 > 0)
  {
    pos += adv_01;
    if (pos > 1.0f) {
      adv = (pos - 1.0f) * length;
      pos = 1.0f;
      next_node = edge->get_node2();
    } else {
      adv = 0;
    }
  }
  else
  {
    pos += adv_01;
    if (pos < 0.0f) {
      adv = pos * length;
      pos = 0;
      next_node = edge->get_node1();
    } else {
      adv = 0;
    }
  }
}

void
EdgePosition::advance(Vector2f& adv, Node*& next_node)
{
  // FIXME: This might be optimizable
  Vector2f p1 = edge->get_node1()->get_pos();
  Vector2f p2 = edge->get_node2()->get_pos();
  
  Vector2f edge_v = p2 - p1;

  Vector2f proj = glm::proj(adv, edge_v);

  float angle = atan2f(edge_v.y, edge_v.x) - atan2f(proj.y, proj.x);

  // Check if we are going forward or backward
  float advf;
  if (angle > M_PI/2 || angle < -M_PI/2)
    advf = -glm::length(proj);
  else
    advf = glm::length(proj);

  // Move forward
  advance(advf, next_node);
  
  // Calculate the rest Vector2f
  // Calculate the rest Vector2f
  if (advf == 0.0f)
    adv = Vector2f(0,0);
  else
    adv -= (proj * ((glm::length(proj) - advf)/glm::length(proj)));
}

Vector2f
EdgePosition::get_pos() const
{
  Vector2f p1 = edge->get_node1()->get_pos();
  Vector2f p2 = edge->get_node2()->get_pos();

  return p1 + pos*(p2 - p1);
}

/* EOF */
