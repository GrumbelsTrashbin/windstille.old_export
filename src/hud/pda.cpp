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

#include <sstream>

#include "input/input_manager.hpp"
#include "display/display.hpp"
#include "display/text_area.hpp"
#include "font/fonts.hpp"
#include "hud/pda.hpp"

DialogEntry::DialogEntry(const std::string& arg_character, const std::string& arg_text)
  : character(arg_character),
    text(arg_text)
{
}

ObjectiveEntry::ObjectiveEntry(const std::string& arg_name, const std::string& arg_text)
  : name(arg_name),
    text(arg_text),
    complete(false)
{
}

PDA::PDA()
  : pos(100.0f, 100.0f),
    background(),
    ui_area(),
    text_area(),
    dialogs(),
    objectives(),
    state(PDA_OBJECTIVES),
    old_state(PDA_NONE)
{ 
  background = Sprite(Pathname("images/pda/pda.sprite"));

  ui_area.reset(new TextArea(Rectf(pos + Vector2f(40.0f, 50.0f), 
                                   Sizef(315.0f, 435.0f)).grow(-12.0f), false));
  text_area.reset(new TextArea(Rectf(pos + Vector2f(40.0f, 50.0f) + Vector2f(0.0f, 56.0f),
                                     Sizef(315.0f, 380.0f)).grow(-12.0f), false));

  ui_area->set_font(Fonts::current()->vera12.get());
  text_area->set_font(Fonts::current()->vera12.get());
}

PDA::~PDA()
{
}

void
PDA::draw()
{
  // Darken the background a bit
  Display::fill_rect(Rect(0, 0, Display::get_width(), Display::get_height()), Color(0.0f, 0.0f, 0.0f, 0.25f));

  background.draw(pos);

  Rectf rect = text_area->get_rect().grow(8.0f);
  
  Display::fill_rounded_rect(rect, 16.0f, Color(0.1f, 0.1f, 0.2f, 0.8f));
  //Display::draw_rounded_rect(rect, 16.0f, Color(1.0f, 1.0f, 1.0f, 0.5f));

  ui_area->draw();
  text_area->draw();
}

void
PDA::update(float delta, const Controller& controller)
{
  const InputEventLst& events = controller.get_events();

  for(InputEventLst::const_iterator i = events.begin(); i != events.end(); ++i) 
  {
    if (i->type == BUTTON_EVENT)
    {
      if (i->axis.name == MENU_LEFT_BUTTON && i->button.down) 
      {
        state = static_cast<PDAState>(state + 1);
        if (state > PDA_DIALOGS)
          state = PDA_OBJECTIVES;
      }
      else if (i->axis.name == MENU_RIGHT_BUTTON && i->button.down) 
      {
        state = static_cast<PDAState>(state - 1);
        if (state < PDA_OBJECTIVES)
          state = PDA_DIALOGS;
      }
    }
  }
  
  if (text_area.get())
  {
    text_area->set_scroll_offset(text_area->get_scroll_offset() + 500.0f * controller.get_axis_state(Y2_AXIS) * delta);
  }
    
  if (old_state != state) 
  {
    old_state = state;

    switch (state) 
    {
      case PDA_NONE:
        break;

      case PDA_OBJECTIVES:
        generate_objectives();
        break;

      case PDA_DIALOGS:
        generate_dialogs();
        break;
    }
  }
  
  text_area->update(delta);
}

void
PDA::force_regeneration()
{
  old_state = PDA_NONE;
}

void
PDA::add_dialog(const std::string& character, const std::string& text)
{
  force_regeneration();
  dialogs.push_back(DialogEntry(character, text));
}

void
PDA::add_objective(const std::string& name, const std::string& text)
{
  force_regeneration();
  objectives.push_back(ObjectiveEntry(name, text));
}

void
PDA::objective_complete(const std::string& name)
{  
  force_regeneration();

  for (std::vector<ObjectiveEntry>::iterator i = objectives.begin(); i != objectives.end(); ++i) 
  {
    if (i->name == name)
      i->complete = true;
    return;
  }
}

bool
PDA::is_objective_given(const std::string& name)
{
  for (std::vector<ObjectiveEntry>::iterator i = objectives.begin(); i != objectives.end(); ++i) 
  {
    if (i->name == name)
      return true;
  }
  
  return false;
}

bool
PDA::is_objective_complete(const std::string& name)
{
  for (std::vector<ObjectiveEntry>::iterator i = objectives.begin(); i != objectives.end(); ++i) 
  {
    if (i->name == name && i->complete)
      return true;
  }
  
  return false;
}

void
PDA::generate_objectives()
{
  ui_area->set_text("<large>Personal Digital Assistant</large>\n"
                    "<b>objectives</b> - dialogs");
  
  std::ostringstream out;
  for (std::vector<ObjectiveEntry>::reverse_iterator i = objectives.rbegin(); i != objectives.rend(); ++i) 
  {
    out << i->name << ": ";

    if (i->complete)
      out << "complete\n";
    else
      out << "incomplete\n";

    out << i->text + "\n\n";
  }

  text_area->set_text(out.str());
}

void
PDA::generate_dialogs()
{
  ui_area->set_text("<large>Personal Digital Assistant</large>\n"
                    "objectives - <b>dialogs</b>");
  
  std::ostringstream out;
  for (std::vector<DialogEntry>::reverse_iterator i = dialogs.rbegin(); i != dialogs.rend(); ++i) 
  {
    if (i->character == "Jane")
      out << "<i>" << i->character << ":</i> " << i->text << '\n';
    else
      out << "<b>" << i->character << ":</b> " << i->text << '\n';
  }
  text_area->set_text(out.str());
}

/* EOF */
