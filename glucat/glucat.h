#ifndef _GLUCAT_GLUCAT_H
#define _GLUCAT_GLUCAT_H
/***************************************************************************
    GluCat : Generic library of universal Clifford algebra templates
    glucat.h : Organize GluCat header files for applications
                             -------------------
    begin                : Sun 2001-12-09
    copyright            : (C) 2001-2007 by Paul C. Leopardi
 ***************************************************************************

    This library is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this library.  If not, see <http://www.gnu.org/licenses/>.

 ***************************************************************************
 This library is based on a prototype written by Arvind Raja and was
 licensed under the LGPL with permission of the author. See Arvind Raja,
 "Object-oriented implementations of Clifford algebras in C++: a prototype",
 in Ablamowicz, Lounesto and Parra (eds.)
 "Clifford algebras with numeric and symbolic computations", Birkhauser, 1996.
 ***************************************************************************
     Arvind Raja's original header comments and references follow.
 ***************************************************************************
// clifford algebra package,  Arvind.Raja@hut.fi
// ref: Press et.al. "Numerical Recipes in C", 2nd ed., C.U.P., 1992.
// ref: LEDA, v 3.0, Stefan N\"aher, Max-Planck-Institut f\"ur Informatik
// ref: Stroustrup B., "The C++ Programming Language", 2nd ed.,
//      Addison-Wesley, 1991.
// ref: R. Sedgewick, "Algorithms in C++", Addison-Wesley, 1992.
// ref: S. Meyers, "Effective C++ ", Addison-Wesley, 1992.
 ***************************************************************************/

#include <boost/config.hpp>
#include <boost/cstdint.hpp>
#include <boost/limits.hpp>
#include "glucat/portability.h"

// Declarations

#include <cmath>
#include "glucat/global.h"

#include <exception>
#include <stdexcept>
#include "glucat/errors.h"

#include <bitset>
#include "glucat/index_set.h"

// Add real equivalents to complex functions
#include "glucat/long_double.h"

// Improve IEEE NaN support
#include "glucat/scalar.h"

// Include <utility> to define pair<>
#include <utility>
#include <vector>
#include <fstream>
#include "glucat/clifford_algebra.h"

#include <map>
#ifdef _GLUCAT_USE_GNU_CXX_HASH_MAP
#include <ext/hash_map>
#endif
#include <string>
#include <complex>
#include "glucat/framed_multi.h"

#include <iostream>

#include <boost/numeric/ublas/fwd.hpp>

#include "glucat/matrix.h"

#include "glucat/generation.h"

#include "glucat/matrix_multi.h"

#endif  // _GLUCAT_GLUCAT_H
