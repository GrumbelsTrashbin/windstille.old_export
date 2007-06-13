/*  $Id$
**   __      __ __             ___        __   __ __   __
**  /  \    /  \__| ____    __| _/_______/  |_|__|  | |  |   ____
**  \   \/\/   /  |/    \  / __ |/  ___/\   __\  |  | |  | _/ __ \
**   \        /|  |   |  \/ /_/ |\___ \  |  | |  |  |_|  |_\  ___/
**    \__/\  / |__|___|  /\____ /____  > |__| |__|____/____/\___  >
**         \/          \/      \/    \/                         \/
**  Copyright (C) 2005 Ingo Ruhnke <grumbel@gmx.de>
**
**  This program is free software; you can redistribute it and/or
**  modify it under the terms of the GNU General Public License
**  as published by the Free Software Foundation; either version 2
**  of the License, or (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
** 
**  You should have received a copy of the GNU General Public License
**  along with this program; if not, write to the Free Software
**  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
**  02111-1307, USA.
*/

#include "sound/sound_manager.hpp"
#include "display/display.hpp"
#include "font/ttf_font.hpp"
#include "menu_component.hpp"

#include "menu_item.hpp"

namespace GUI {

MenuItem::MenuItem(MenuComponent* parent_, const std::string& label_)
  : parent(parent_),
    label(label_)
{}

void
MenuItem::draw(const Rectf& rect, bool is_active)
{
  Color font_color;
  TTFFont* font = parent->get_font();
  
  if (is_active) {
    Display::fill_rounded_rect(rect, 5.0f, Color(0.5f, 0.5f, 0.5f, 0.75f));
    Display::draw_rounded_rect(rect, 5.0f, Color(1.0f, 1.0f, 1.0f, 1.0f));
    font_color = Color(1.0f, 1.0f, 1.0f);
  } else {
    //Display::fill_rounded_rect(rect, 5.0f, Color(0.3f, 0.3f, 0.3f, 0.75f));
    //Display::draw_rounded_rect(rect, 5.0f, Color(1.0f, 1.0f, 1.0f, 0.75f));
    font_color = Color(0.75f, 0.75f, 0.75f, 1.0f);
  }

  font->draw(rect.left + font->get_height(), rect.top + font->get_height()/2.0f + rect.get_height()/2.0f - 2.0f,
             label, font_color);
      
}

void
MenuItem::update(float delta)
{
}

EnumMenuItem::EnumMenuItem(MenuComponent* parent_, 
                           const std::string& label_, int index_)
  : MenuItem(parent_, label_),
    index(index_)
{
}

void
EnumMenuItem::add_pair(int value, const std::string& label)
{
  EnumValue enum_value;
  enum_value.value = value;
  enum_value.label = label;
  labels.push_back(enum_value);
}

void
EnumMenuItem::incr()
{
  sound_manager->play("sounds/menu_click.wav");           

  index -= 1;
  if (index < 0)
    index = labels.size()-1;
  on_change(labels[index].value);
}

void
EnumMenuItem::decr()
{
  sound_manager->play("sounds/menu_click.wav");

  index += 1;
  if (index >= static_cast<int>(labels.size()))
    index = 0;
  on_change(labels[index].value);
}

void 
EnumMenuItem::draw(const Rectf& rect, bool is_active)
{
  MenuItem::draw(rect, is_active);
  TTFFont* font = parent->get_font();
  Color font_color;
  if (is_active)
    {
      font_color = Color(1.0f, 1.0f, 1.0f);
    }
  else
    {
      font_color = Color(0.75f, 0.75f, 0.75f, 1.0f);
    }

  font->draw(rect.right - font->get_height() - font->get_width(labels[index].label),
             rect.top + font->get_height()/2.0f + rect.get_height()/2.0f - 2.0f,
             labels[index].label,
             font_color);
}

void 
EnumMenuItem::update(float delta)
{
  MenuItem::update(delta);
}

SliderMenuItem::SliderMenuItem(MenuComponent* parent_, 
                               const std::string& label_,
                               int value_, int min_value_, int max_value_, int step_) 
  : MenuItem(parent_, label_),
    value(value_),
    min_value(min_value_),
    max_value(max_value_),
    step(step_)
{  
}

void
SliderMenuItem::decr()
{
  sound_manager->play("sounds/menu_click.wav");

  value += step;
  if (value > max_value)
    value = max_value;
  on_change(value);
}

void
SliderMenuItem::incr()
{
  sound_manager->play("sounds/menu_click.wav");

  value -= step;
  if (value < min_value)
    value = min_value;
  on_change(value);
}

void
SliderMenuItem::draw(const Rectf& rect, bool is_active)
{
  MenuItem::draw(rect, is_active);
  int total_width = 200;
  int width = total_width * value / (max_value - min_value);

  Color color;
  if (is_active)
    {
      color = Color(1.0f, 1.0f, 1.0f);
    }
  else
    {
      color = Color(0.75f, 0.75f, 0.75f, 1.0f);
    }

  Display::fill_rounded_rect(Rectf(Vector(rect.right - 4 - total_width, rect.top + 4),
                                   Sizef(width, rect.get_height() - 8)), 
                             5.0f,
                             Color(0.75f*color.r, 0.75f*color.g, 0.75f*color.b, color.a));


  Display::draw_rounded_rect(Rectf(Vector(rect.right - 4 - total_width, rect.top + 4),
                                   Sizef(total_width, rect.get_height() - 8)), 
                             5.0f,
                             color);
}

void
SliderMenuItem::update(float delta)
{
  MenuItem::update(delta);
}

ButtonMenuItem::ButtonMenuItem(MenuComponent* parent_, const std::string& label_)
  : MenuItem(parent_, label_)
{
}

void
ButtonMenuItem::click()
{
  sound_manager->play("sounds/menu_click.wav");

  on_click();
}

void
ButtonMenuItem::draw(const Rectf& rect, bool is_active)
{
  MenuItem::draw(rect, is_active);
}

void
ButtonMenuItem::update(float)
{
}

} // namespace GUI

/* EOF */