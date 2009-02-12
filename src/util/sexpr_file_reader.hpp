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

#ifndef HEADER_WINDSTILLE_SEXPR_FILE_READER_HPP
#define HEADER_WINDSTILLE_SEXPR_FILE_READER_HPP

#include "file_reader.hpp"
#include "lisp/lisp.hpp"

/** */
class SExprFileReader : public FileReader
{
private:
public:
  SExprFileReader(const lisp::Lisp* root, const lisp::Lisp* lisp);
  SExprFileReader(const lisp::Lisp* lisp, bool delete_sexpr = false);
};

#endif

/* EOF */