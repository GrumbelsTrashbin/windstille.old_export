//  $Id: tilemap_paint_tool.hxx,v 1.1 2003/09/23 19:10:05 grumbel Exp $
// 
//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
// 
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#ifndef HEADER_TILEMAP_PAINT_TOOL_HXX
#define HEADER_TILEMAP_PAINT_TOOL_HXX

#include "tilemap_tool.hxx"

/** */
class TileMapPaintTool : public TileMapTool
{
private:
  bool painting;
  
public:
  TileMapPaintTool(EditorTileMap* t);
  virtual ~TileMapPaintTool();

  void draw();
  void on_mouse_down(const CL_InputEvent& event);
  void on_mouse_move(const CL_InputEvent& event);
  void on_mouse_up  (const CL_InputEvent& event);
};

#endif

/* EOF */