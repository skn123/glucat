#ifndef _GLUCAT_CLIFFORD_ALGEBRA_IMP_H
#define _GLUCAT_CLIFFORD_ALGEBRA_IMP_H
/***************************************************************************
    GluCat : Generic library of universal Clifford algebra templates
    clifford_algebra_imp.h : Implement common Clifford algebra functions
                             -------------------
    begin                : Sun 2001-12-09
    copyright            : (C) 2001-2012 by Paul C. Leopardi
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
     See also Arvind Raja's original header comments in glucat.h
 ***************************************************************************/

// References for algorithms:
// [AS]:
// Milton Abramowicz and Irene A. Stegun, "Handbook of mathematical functions",
// Dover 1972, first published 1965.
// [CHKL]:
// Sheung Hun Cheng, Nicholas J. Higham, Charles S. Kenney and Alan J. Laub,
// "Approximating the Logarithm of a Matrix to Specified Accuracy", 1999.
// ftp://ftp.ma.man.ac.uk/pub/narep/narep353.ps.gz
// [GL]:
// Gene H. Golub and Charles F. van Loan,
// "Matrix Computations", 3rd ed., Johns Hopkins UP, 1996.
// [GW]:
// C.F. Gerald and P.O. Wheatley, "Applied Numerical Analysis",
// 6th Edition, Addison-Wesley, 1999.
// [H]:
// Nicholas J. Higham
// "The Scaling and Squaring Method for the Matrix Exponential Revisited",
// SIAM Journal on Matrix Analysis and Applications,
// Vol. 26,  Issue 4 (2005), pp. 1179-1193.
// [Z]:
// Doron Zeilberger, "PADE" (Maple code), 2002.
// http://www.math.rutgers.edu/~zeilberg/tokhniot/PADE

#include <glucat/clifford_algebra.h>

#include <glucat/scalar.h>

namespace glucat
{
  template< typename Scalar_T, typename Index_Set_T, typename Multivector_T>
  const std::string
  clifford_algebra<Scalar_T,Index_Set_T,Multivector_T>::
  classname()
  { return "clifford_algebra"; }

  /// Test for inequality of multivectors
  template
  <
    template<typename, const index_t, const index_t> class Multivector,
    template<typename, const index_t, const index_t> class RHS,
    typename Scalar_T, const index_t LO, const index_t HI
  >
  inline
  bool
  operator!= (const Multivector<Scalar_T,LO,HI>& lhs, const RHS<Scalar_T,LO,HI>& rhs)
  { return !(lhs == rhs); }

  /// Test for inequality of multivector and scalar
  template< template<typename, const index_t, const index_t> class Multivector,
            typename Scalar_T, const index_t LO, const index_t HI >
  inline
  bool
  operator!= (const Multivector<Scalar_T,LO,HI>& lhs, const Scalar_T& scr)
  { return !(lhs == scr); }

  /// Test for inequality of scalar and multivector
  template< template<typename, const index_t, const index_t> class Multivector,
            typename Scalar_T, const index_t LO, const index_t HI >
  inline
  bool
  operator!= (const Scalar_T& scr, const Multivector<Scalar_T,LO,HI>& rhs)
  { return !(rhs == scr); }

  /// Geometric sum of multivector and scalar
  template< template<typename, const index_t, const index_t> class Multivector,
            typename Scalar_T, const index_t LO, const index_t HI >
  inline
  const Multivector<Scalar_T,LO,HI>
  operator+ (const Multivector<Scalar_T,LO,HI>& lhs, const Scalar_T& scr)
  {
    Multivector<Scalar_T,LO,HI> result = lhs;
    return result += scr;
  }

  /// Geometric sum of scalar and multivector
  template< template<typename, const index_t, const index_t> class Multivector,
            typename Scalar_T, const index_t LO, const index_t HI >
  inline
  const Multivector<Scalar_T,LO,HI>
  operator+ (const Scalar_T& scr, const Multivector<Scalar_T,LO,HI>& rhs)
  {
    return rhs + scr;
  }

  /// Geometric sum
  template
  <
    template<typename, const index_t, const index_t> class Multivector,
    template<typename, const index_t, const index_t> class RHS,
    typename Scalar_T, const index_t LO, const index_t HI
  >
  inline
  const Multivector<Scalar_T,LO,HI>
  operator+ (const Multivector<Scalar_T,LO,HI>& lhs, const RHS<Scalar_T,LO,HI>& rhs)
  {
    Multivector<Scalar_T,LO,HI> result = lhs;
    return result += rhs;
  }

  /// Geometric difference of multivector and scalar
  template< template<typename, const index_t, const index_t> class Multivector,
            typename Scalar_T, const index_t LO, const index_t HI >
  inline
  const Multivector<Scalar_T,LO,HI>
  operator- (const Multivector<Scalar_T,LO,HI>& lhs, const Scalar_T& scr)
  {
    Multivector<Scalar_T,LO,HI> result = lhs;
    return result += -scr;
  }

  /// Geometric difference of scalar and multivector
  template< template<typename, const index_t, const index_t> class Multivector,
            typename Scalar_T, const index_t LO, const index_t HI >
  inline
  const Multivector<Scalar_T,LO,HI>
  operator- (const Scalar_T& scr, const Multivector<Scalar_T,LO,HI>& rhs)
  { return -rhs + scr; }

  /// Geometric difference
  template
  <
    template<typename, const index_t, const index_t> class Multivector,
    template<typename, const index_t, const index_t> class RHS,
    typename Scalar_T, const index_t LO, const index_t HI
  >
  inline
  const Multivector<Scalar_T,LO,HI>
  operator- (const Multivector<Scalar_T,LO,HI>& lhs, const RHS<Scalar_T,LO,HI>& rhs)
  {
    Multivector<Scalar_T,LO,HI> result = lhs;
    return result -= rhs;
  }

  /// Product of multivector and scalar
  template< template<typename, const index_t, const index_t> class Multivector,
            typename Scalar_T, const index_t LO, const index_t HI >
  inline
  const Multivector<Scalar_T,LO,HI>
  operator* (const Multivector<Scalar_T,LO,HI>& lhs, const Scalar_T& scr)
  {
    Multivector<Scalar_T,LO,HI> result = lhs;
    return result *= scr;
  }

  /// Product of scalar and multivector
  template< template<typename, const index_t, const index_t> class Multivector,
            typename Scalar_T, const index_t LO, const index_t HI >
  inline
  const Multivector<Scalar_T,LO,HI>
  operator* (const Scalar_T& scr, const Multivector<Scalar_T,LO,HI>& rhs)
  { // Note: this assumes that scalar commutes with multivector.
    // This excludes Clifford algebras over non-commuting rings.
    return rhs * scr;
  }

  /// Geometric product
  template
  <
    template<typename, const index_t, const index_t> class Multivector,
    template<typename, const index_t, const index_t> class RHS,
    typename Scalar_T, const index_t LO, const index_t HI
  >
  inline
  const Multivector<Scalar_T,LO,HI>
  operator* (const Multivector<Scalar_T,LO,HI>& lhs, const RHS<Scalar_T,LO,HI>& rhs)
  {
    typedef Multivector<Scalar_T,LO,HI> multivector_t;
    return lhs * multivector_t(rhs);
  }

  /// Outer product
  template
  <
    template<typename, const index_t, const index_t> class Multivector,
    template<typename, const index_t, const index_t> class RHS,
    typename Scalar_T, const index_t LO, const index_t HI
  >
  inline
  const Multivector<Scalar_T,LO,HI>
  operator^ (const Multivector<Scalar_T,LO,HI>& lhs, const RHS<Scalar_T,LO,HI>& rhs)
  {
    typedef Multivector<Scalar_T,LO,HI> multivector_t;
    return lhs ^ multivector_t(rhs);
  }

  /// Inner product
  template
  <
    template<typename, const index_t, const index_t> class Multivector,
    template<typename, const index_t, const index_t> class RHS,
    typename Scalar_T, const index_t LO, const index_t HI
  >
  inline
  const Multivector<Scalar_T,LO,HI>
  operator& (const Multivector<Scalar_T,LO,HI>& lhs, const RHS<Scalar_T,LO,HI>& rhs)
  {
    typedef Multivector<Scalar_T,LO,HI> multivector_t;
    return lhs & multivector_t(rhs);
  }

  /// Left contraction
  template
  <
    template<typename, const index_t, const index_t> class Multivector,
    template<typename, const index_t, const index_t> class RHS,
    typename Scalar_T, const index_t LO, const index_t HI
  >
  inline
  const Multivector<Scalar_T,LO,HI>
  operator% (const Multivector<Scalar_T,LO,HI>& lhs, const RHS<Scalar_T,LO,HI>& rhs)
  {
    typedef Multivector<Scalar_T,LO,HI> multivector_t;
    return lhs % multivector_t(rhs);
  }

  /// Hestenes scalar product
  template
  <
    template<typename, const index_t, const index_t> class Multivector,
    template<typename, const index_t, const index_t> class RHS,
    typename Scalar_T, const index_t LO, const index_t HI
  >
  inline
  Scalar_T
  star (const Multivector<Scalar_T,LO,HI>& lhs, const RHS<Scalar_T,LO,HI>& rhs)
  {
    typedef Multivector<Scalar_T,LO,HI> multivector_t;
    return star(lhs, multivector_t(rhs));
  }

  /// Quotient of multivector and scalar
  template< template<typename, const index_t, const index_t> class Multivector,
            typename Scalar_T, const index_t LO, const index_t HI >
  inline
  const Multivector<Scalar_T,LO,HI>
  operator/ (const Multivector<Scalar_T,LO,HI>& lhs, const Scalar_T& scr)
  {
    Multivector<Scalar_T,LO,HI> result = lhs;
    return result /= scr;
  }

  /// Quotient of scalar and multivector
  template< template<typename, const index_t, const index_t> class Multivector,
            typename Scalar_T, const index_t LO, const index_t HI >
  inline
  const Multivector<Scalar_T,LO,HI>
  operator/ (const Scalar_T& scr, const Multivector<Scalar_T,LO,HI>& rhs)
  {
    Multivector<Scalar_T,LO,HI> result = scr;
    return result /= rhs;
  }

  /// Geometric quotient
  template
  <
    template<typename, const index_t, const index_t> class Multivector,
    template<typename, const index_t, const index_t> class RHS,
    typename Scalar_T, const index_t LO, const index_t HI
  >
  inline
  const Multivector<Scalar_T,LO,HI>
  operator/ (const Multivector<Scalar_T,LO,HI>& lhs, const RHS<Scalar_T,LO,HI>& rhs)
  {
    typedef Multivector<Scalar_T,LO,HI> multivector_t;
    return lhs / multivector_t(rhs);
  }

  /// Transformation via twisted adjoint action
  template
  <
    template<typename, const index_t, const index_t> class Multivector,
    template<typename, const index_t, const index_t> class RHS,
    typename Scalar_T, const index_t LO, const index_t HI
  >
  inline
  const Multivector<Scalar_T,LO,HI>
  operator| (const Multivector<Scalar_T,LO,HI>& lhs, const RHS<Scalar_T,LO,HI>& rhs)
  {
    typedef Multivector<Scalar_T,LO,HI> multivector_t;
    return lhs | multivector_t(rhs);
  }

  /// Geometric multiplicative inverse
  template< template<typename, const index_t, const index_t> class Multivector,
            typename Scalar_T, const index_t LO, const index_t HI >
  inline
  const Multivector<Scalar_T,LO,HI>
  inv(const Multivector<Scalar_T,LO,HI>& val)
  { return val.inv(); }

  /// Integer power of multivector
  template< template<typename, const index_t, const index_t> class Multivector,
            typename Scalar_T, const index_t LO, const index_t HI >
  const Multivector<Scalar_T,LO,HI>
  pow(const Multivector<Scalar_T,LO,HI>& lhs, int rhs)
  {
    typedef Multivector<Scalar_T,LO,HI> multivector_t;
    typedef numeric_traits<Scalar_T> traits_t;

    multivector_t a;
    if (rhs < 0)
    {
      if (lhs == Scalar_T(0))
        return traits_t::NaN();
      rhs = -rhs;
      a = lhs.inv();
    }
    else
      a = lhs;
    multivector_t result = Scalar_T(1);
    for (;
        rhs != 0;
        rhs >>= 1, a *= a)
      if (rhs & 1)
        result *= a;
    return result;
  }

  /// Multivector power of multivector
  template
  <
    template<typename, const index_t, const index_t> class Multivector,
    template<typename, const index_t, const index_t> class RHS,
    typename Scalar_T, const index_t LO, const index_t HI
  >
  inline
  const Multivector<Scalar_T,LO,HI>
  pow(const Multivector<Scalar_T,LO,HI>& lhs, const RHS<Scalar_T,LO,HI>& rhs)
  {
    typedef numeric_traits<Scalar_T> traits_t;

    if (lhs == Scalar_T(0))
    {
      const Scalar_T m = rhs.scalar();
      if (rhs == m)
        return (m < 0)
               ? traits_t::NaN()
               : (m == 0)
                 ? Scalar_T(1)
                 : Scalar_T(0);
      else
        return Scalar_T(0);
    }
    return exp(log(lhs) * rhs);
  }

  /// Outer product power of multivector
  template< template<typename, const index_t, const index_t> class Multivector,
            typename Scalar_T, const index_t LO, const index_t HI >
  const Multivector<Scalar_T,LO,HI>
  outer_pow(const Multivector<Scalar_T,LO,HI>& lhs, int rhs)
  { return lhs.outer_pow(rhs); }

  /// Scalar part
  template< template<typename, const index_t, const index_t> class Multivector,
            typename Scalar_T, const index_t LO, const index_t HI >
  inline
  Scalar_T
  scalar(const Multivector<Scalar_T,LO,HI>& val)
  { return val.scalar(); }

  /// Real part: synonym for scalar part
  template< template<typename, const index_t, const index_t> class Multivector,
            typename Scalar_T, const index_t LO, const index_t HI >
  inline
  Scalar_T
  real(const Multivector<Scalar_T,LO,HI>& val)
  { return val.scalar(); }

  /// Imaginary part: deprecated (always 0)
  template
  <
    template<typename, const index_t, const index_t> class Multivector,
    typename Scalar_T, const index_t LO, const index_t HI
  >
  inline
  Scalar_T
  imag(const Multivector<Scalar_T,LO,HI>& val)
  { return Scalar_T(0); }

  /// Pure part
  template< template<typename, const index_t, const index_t> class Multivector,
            typename Scalar_T, const index_t LO, const index_t HI >
  inline
  const Multivector<Scalar_T,LO,HI>
  pure(const Multivector<Scalar_T,LO,HI>& val)
  { return val - val.scalar(); }

  /// Even part
  template< template<typename, const index_t, const index_t> class Multivector,
            typename Scalar_T, const index_t LO, const index_t HI >
  inline
  const Multivector<Scalar_T,LO,HI>
  even(const Multivector<Scalar_T,LO,HI>& val)
  { return val.even(); }

  /// Odd part
  template< template<typename, const index_t, const index_t> class Multivector,
            typename Scalar_T, const index_t LO, const index_t HI >
  inline
  const Multivector<Scalar_T,LO,HI>
  odd(const Multivector<Scalar_T,LO,HI>& val)
  { return val.odd(); }

  /// Vector part of multivector, as a vector_t with respect to frame()
  template< template<typename, const index_t, const index_t> class Multivector,
            typename Scalar_T, const index_t LO, const index_t HI >
  inline
  const std::vector<Scalar_T>
  vector_part(const Multivector<Scalar_T,LO,HI>& val)
  { return val.vector_part(); }

  /// Main involution, each {i} is replaced by -{i} in each term, eg. {1} ->-{1}
  template< template<typename, const index_t, const index_t> class Multivector,
            typename Scalar_T, const index_t LO, const index_t HI >
  inline
  const Multivector<Scalar_T,LO,HI>
  involute(const Multivector<Scalar_T,LO,HI>& val)
  { return val.involute(); }

  /// Reversion, eg. {1}*{2} -> {2}*{1}
  template< template<typename, const index_t, const index_t> class Multivector,
            typename Scalar_T, const index_t LO, const index_t HI >
  inline
  const Multivector<Scalar_T,LO,HI>
  reverse(const Multivector<Scalar_T,LO,HI>& val)
  { return val.reverse(); }

  /// Conjugation, rev o invo == invo o rev
  template< template<typename, const index_t, const index_t> class Multivector,
            typename Scalar_T, const index_t LO, const index_t HI >
  inline
  const Multivector<Scalar_T,LO,HI>
  conj(const Multivector<Scalar_T,LO,HI>& val)
  { return val.conj(); }

  /// Scalar_T quadratic form == (rev(x)*x)(0)
  template< template<typename, const index_t, const index_t> class Multivector,
            typename Scalar_T, const index_t LO, const index_t HI >
  inline
  Scalar_T
  quad(const Multivector<Scalar_T,LO,HI>& val)
  { return val.quad(); }

  /// Scalar_T norm == sum of norm of coordinates
  template< template<typename, const index_t, const index_t> class Multivector,
            typename Scalar_T, const index_t LO, const index_t HI >
  inline
  Scalar_T
  norm(const Multivector<Scalar_T,LO,HI>& val)
  { return val.norm(); }

  /// Absolute value == sqrt(norm)
  template< template<typename, const index_t, const index_t> class Multivector,
            typename Scalar_T, const index_t LO, const index_t HI >
  inline
  Scalar_T
  abs(const Multivector<Scalar_T,LO,HI>& val)
  { return numeric_traits<Scalar_T>::sqrt(val.norm()); }

  /// Maximum of absolute values of components of multivector: multivector infinity norm
  template< template<typename, const index_t, const index_t> class Multivector,
            typename Scalar_T, const index_t LO, const index_t HI >
  inline
  Scalar_T
  max_abs(const Multivector<Scalar_T,LO,HI>& val)
  { return val.max_abs(); }

  /// Square root of -1 which commutes with all members of the frame of the given multivector
  template< template<typename, const index_t, const index_t> class Multivector,
            typename Scalar_T, const index_t LO, const index_t HI >
  const Multivector<Scalar_T,LO,HI>
  complexifier(const Multivector<Scalar_T,LO,HI>& val)
  {
    typedef Multivector<Scalar_T,LO,HI> multivector_t;
    typedef numeric_traits<Scalar_T> traits_t;
    typedef typename multivector_t::index_set_t index_set_t;

    index_set_t frm = val.frame();
    index_t incp[] = {0, 2, 1, 0};
    index_t incq[] = {1, 0, 0, 0};
    index_t bott = pos_mod((frm.count_pos() - frm.count_neg()), 4);
    for (index_t
        k = 0;
        k != incp[bott];
        k++)
      for (index_t
          idx = 1;
          idx != HI+1;
          ++idx)
        if (!frm[idx])
        {
          frm.set(idx);
          break;
        }
    for (index_t
        k = 0;
        k != incq[bott];
        k++)
      for (index_t
          idx = -1;
          idx != LO-1;
          --idx)
        if (!frm[idx])
        {
          frm.set(idx);
          break;
        }
    index_t new_bott = pos_mod(frm.count_pos() - frm.count_neg(), 4);

    if ((incp[new_bott] == 0) && (incq[new_bott] == 0))
      return multivector_t(frm, Scalar_T(1));
    else
      // Return IEEE NaN or -Inf
      return traits_t::NaN();
  }

  /// Square root of -1 which commutes with all members of the frame of the given multivector
  /// The name "elliptic" is now deprecated: use "complexifier" instead.
  template< template<typename, const index_t, const index_t> class Multivector,
            typename Scalar_T, const index_t LO, const index_t HI >
  inline
  const Multivector<Scalar_T,LO,HI>
  elliptic(const Multivector<Scalar_T,LO,HI>& val)
  { return complexifier(val); }

  /// Check that i is a valid complexifier for val
  template< template<typename, const index_t, const index_t> class Multivector,
            typename Scalar_T, const index_t LO, const index_t HI >
  inline
  static
  void
  check_complex(const Multivector<Scalar_T,LO,HI>& val,
                const Multivector<Scalar_T,LO,HI>& i, const bool prechecked = false)
  {
    if (!prechecked)
    {
      typedef Multivector<Scalar_T,LO,HI> multivector_t;
      typedef typename multivector_t::index_set_t index_set_t;
      typedef typename multivector_t::error_t error_t;

      const index_set_t i_frame = i.frame();
      // We need i to be a complexifier whose frame is large enough to represent val
      if (complexifier(i) != i ||
         (val.frame() | i_frame) != i_frame ||
          complexifier(val).frame().count() > i_frame.count())
        throw error_t("check_complex(val, i): i is not a valid complexifier for val");
    }
  }

  /// Square root of multivector with specified complexifier
  template< template<typename, const index_t, const index_t> class Multivector,
            typename Scalar_T, const index_t LO, const index_t HI >
  inline
  const Multivector<Scalar_T,LO,HI>
  sqrt(const Multivector<Scalar_T,LO,HI>& val, const Multivector<Scalar_T,LO,HI>& i, bool prechecked)
  { return sqrt(val, i, prechecked); }

  /// Square root of multivector
  template< template<typename, const index_t, const index_t> class Multivector,
            typename Scalar_T, const index_t LO, const index_t HI >
  inline
  const Multivector<Scalar_T,LO,HI>
  sqrt(const Multivector<Scalar_T,LO,HI>& val)
  { return sqrt(val, complexifier(val), true); }

  /// Exponential of multivector
  template< template<typename, const index_t, const index_t> class Multivector,
            typename Scalar_T, const index_t LO, const index_t HI >
  const Multivector<Scalar_T,LO,HI>
  clifford_exp(const Multivector<Scalar_T,LO,HI>& val)
  {
    // Scaling and squaring Pade' approximation of matrix exponential
    // Reference: [GL], Section 11.3, p572-576
    // Reference: [H]

    typedef numeric_traits<Scalar_T> traits_t;

    const Scalar_T scalar_val = scalar(val);
    const Scalar_T scalar_exp = traits_t::exp(scalar_val);
    if (traits_t::isNaN_or_isInf(scalar_exp))
      return traits_t::NaN();
    if (val == scalar_val)
      return scalar_exp;

    typedef Multivector<Scalar_T,LO,HI> multivector_t;
    multivector_t A = val - scalar_val;
    const Scalar_T pure_scale2 = A.norm();

    if (traits_t::isNaN_or_isInf(pure_scale2))
      return traits_t::NaN();
    if (pure_scale2 == Scalar_T(0))
      return scalar_exp;

    const int ilog2_scale =
      std::max(0, traits_t::to_int(ceil((log2(pure_scale2) + Scalar_T(A.frame().count()))/Scalar_T(2))) - 3);
    const Scalar_T i_scale = traits_t::pow(Scalar_T(2), ilog2_scale);
    if (traits_t::isNaN_or_isInf(i_scale))
      return traits_t::NaN();

    A /= i_scale;
    multivector_t pure_exp;
    {
      typedef std::numeric_limits<Scalar_T> limits_t;
      const int nbr_even_powers = 2*(limits_t::digits / 32) + 4;

      // Create an array of coefficients
      const int max_power = 2*nbr_even_powers + 1;
      static Scalar_T c[max_power+1];
      if (c[0] != Scalar_T(1))
      {
        c[0] = Scalar_T(1);
        for (int
            k = 0;
            k != max_power;
            ++k)
          c[k+1] = c[k]*(max_power-k) / ((2*max_power-k)*(k+1));
      }

      // Create an array of even powers
      multivector_t AA[nbr_even_powers];
      AA[0] = A * A;
      AA[1] = AA[0] * AA[0];
      for (int
        k = 2;
        k != nbr_even_powers;
        ++k)
        AA[k] = AA[k-2] * AA[1];

      // Use compensated summation to calculate U and AV
      multivector_t residual = 0;
      multivector_t U = c[0];
      for (int
          k = 0;
          k != nbr_even_powers;
          ++k)
      {
        const multivector_t& term = AA[k]*c[2*k + 2] - residual;
        const multivector_t& sum = U + term;
        residual = (sum - U) - term;
        U = sum;
      }
      residual = 0;
      multivector_t AV = c[1];
      for (int
          k = 0;
          k != nbr_even_powers;
          ++k)
      {
        const multivector_t& term = AA[k]*c[2*k + 3] - residual;
        const multivector_t& sum = AV + term;
        residual = (sum - AV) - term;
        AV = sum;
      }
      AV *= A;
      pure_exp = (U+AV) / (U-AV);
    }
    for (int
        k = 0;
        k != ilog2_scale;
        ++k)
      pure_exp *= pure_exp;
    return pure_exp * scalar_exp;
  }

  /// Natural logarithm of multivector with specified complexifier
  template< template<typename, const index_t, const index_t> class Multivector,
            typename Scalar_T, const index_t LO, const index_t HI >
  inline
  const Multivector<Scalar_T,LO,HI>
  log(const Multivector<Scalar_T,LO,HI>& val, const Multivector<Scalar_T,LO,HI>& i, bool prechecked)
  { return log(val, i, prechecked); }

  /// Natural logarithm of multivector
  template< template<typename, const index_t, const index_t> class Multivector,
            typename Scalar_T, const index_t LO, const index_t HI >
  inline
  const Multivector<Scalar_T,LO,HI>
  log(const Multivector<Scalar_T,LO,HI>& val)
  { return log(val, complexifier(val), true); }

  /// Hyperbolic cosine of multivector
  template< template<typename, const index_t, const index_t> class Multivector,
            typename Scalar_T, const index_t LO, const index_t HI >
  inline
  const Multivector<Scalar_T,LO,HI>
  cosh(const Multivector<Scalar_T,LO,HI>& val)
  {
    typedef numeric_traits<Scalar_T> traits_t;
    if (val.isnan())
      return traits_t::NaN();

    const Scalar_T& s = scalar(val);
    if (val == s)
      return traits_t::cosh(s);
    return (exp(val)+exp(-val)) / Scalar_T(2);
  }

  /// Inverse hyperbolic cosine of multivector with specified complexifier
  // Reference: [AS], Section 4.6, p86-89
  template< template<typename, const index_t, const index_t> class Multivector,
            typename Scalar_T, const index_t LO, const index_t HI >
  inline
  const Multivector<Scalar_T,LO,HI>
  acosh(const Multivector<Scalar_T,LO,HI>& val, const Multivector<Scalar_T,LO,HI>& i, bool prechecked)
  {
    typedef numeric_traits<Scalar_T> traits_t;
    check_complex(val, i, prechecked);
    if (val.isnan())
      return traits_t::NaN();

    typedef Multivector<Scalar_T,LO,HI> multivector_t;
    const multivector_t radical = sqrt(val*val - Scalar_T(1), i, true);
    return (norm(val + radical) >= norm(val))
           ?  log(val + radical, i, true)
           : -log(val - radical, i, true);
  }

  /// Inverse hyperbolic cosine of multivector
  // Reference: [AS], Section 4.6, p86-89
  template< template<typename, const index_t, const index_t> class Multivector,
            typename Scalar_T, const index_t LO, const index_t HI >
  inline
  const Multivector<Scalar_T,LO,HI>
  acosh(const Multivector<Scalar_T,LO,HI>& val)
  { return acosh(val, complexifier(val), true); }

  /// Cosine of multivector with specified complexifier
  template< template<typename, const index_t, const index_t> class Multivector,
            typename Scalar_T, const index_t LO, const index_t HI >
  const Multivector<Scalar_T,LO,HI>
  cos(const Multivector<Scalar_T,LO,HI>& val, const Multivector<Scalar_T,LO,HI>& i, bool prechecked)
  {
    typedef numeric_traits<Scalar_T> traits_t;
    if (val.isnan())
      return traits_t::NaN();

    const Scalar_T& s = scalar(val);
    if (val == s)
      return traits_t::cos(s);

    check_complex(val, i, prechecked);

    typedef Multivector<Scalar_T,LO,HI> multivector_t;
    static const Scalar_T& twopi = Scalar_T(2) * traits_t::pi();
    const multivector_t& z = i *
      (val - s + traits_t::fmod(s, twopi));
    return (exp(z)+exp(-z)) / Scalar_T(2);
  }

  /// Cosine of multivector
  template< template<typename, const index_t, const index_t> class Multivector,
            typename Scalar_T, const index_t LO, const index_t HI >
  inline
  const Multivector<Scalar_T,LO,HI>
  cos(const Multivector<Scalar_T,LO,HI>& val)
  { return cos(val, complexifier(val), true); }

  /// Inverse cosine of multivector with specified complexifier
  // Reference: [AS], Section 4.4, p79-83
  template< template<typename, const index_t, const index_t> class Multivector,
            typename Scalar_T, const index_t LO, const index_t HI >
  inline
  const Multivector<Scalar_T,LO,HI>
  acos(const Multivector<Scalar_T,LO,HI>& val, const Multivector<Scalar_T,LO,HI>& i, bool prechecked)
  {
    typedef numeric_traits<Scalar_T> traits_t;
    if (val.isnan())
      return traits_t::NaN();

    const Scalar_T& realval = val.scalar();
    if (val == realval && traits_t::abs(realval) <= Scalar_T(1))
      return traits_t::acos(realval);

    check_complex(val, i, prechecked);
    return i * acosh(val, i, true);
  }

  /// Inverse cosine of multivector
  // Reference: [AS], Section 4.4, p79-83
  template< template<typename, const index_t, const index_t> class Multivector,
            typename Scalar_T, const index_t LO, const index_t HI >
  inline
  const Multivector<Scalar_T,LO,HI>
  acos(const Multivector<Scalar_T,LO,HI>& val)
  { return acos(val, complexifier(val), true); }

  /// Hyperbolic sine of multivector
  template< template<typename, const index_t, const index_t> class Multivector,
            typename Scalar_T, const index_t LO, const index_t HI >
  inline
  const Multivector<Scalar_T,LO,HI>
  sinh(const Multivector<Scalar_T,LO,HI>& val)
  {
    typedef numeric_traits<Scalar_T> traits_t;
    if (val.isnan())
      return traits_t::NaN();

    const Scalar_T& s = scalar(val);
    if (val == s)
      return traits_t::sinh(s);

    return (exp(val)-exp(-val)) / Scalar_T(2);
  }

  /// Inverse hyperbolic sine of multivector with specified complexifier
  // Reference: [AS], Section 4.6, p86-89
  template< template<typename, const index_t, const index_t> class Multivector,
            typename Scalar_T, const index_t LO, const index_t HI >
  inline
  const Multivector<Scalar_T,LO,HI>
  asinh(const Multivector<Scalar_T,LO,HI>& val, const Multivector<Scalar_T,LO,HI>& i, bool prechecked)
  {
    typedef numeric_traits<Scalar_T> traits_t;
    check_complex(val, i, prechecked);
    if (val.isnan())
      return traits_t::NaN();

    typedef Multivector<Scalar_T,LO,HI> multivector_t;
    const multivector_t radical = sqrt(val*val + Scalar_T(1), i, true);
    return (norm(val + radical) >= norm(val))
           ?  log( val + radical, i, true)
           : -log(-val + radical, i, true);
  }

  /// Inverse hyperbolic sine of multivector
  // Reference: [AS], Section 4.6, p86-89
  template< template<typename, const index_t, const index_t> class Multivector,
            typename Scalar_T, const index_t LO, const index_t HI >
  inline
  const Multivector<Scalar_T,LO,HI>
  asinh(const Multivector<Scalar_T,LO,HI>& val)
  { return asinh(val, complexifier(val), true); }

  /// Sine of multivector with specified complexifier
  template< template<typename, const index_t, const index_t> class Multivector,
            typename Scalar_T, const index_t LO, const index_t HI >
  const Multivector<Scalar_T,LO,HI>
  sin(const Multivector<Scalar_T,LO,HI>& val, const Multivector<Scalar_T,LO,HI>& i, bool prechecked)
  {
    typedef numeric_traits<Scalar_T> traits_t;
    if (val.isnan())
      return traits_t::NaN();

    const Scalar_T& s = scalar(val);
    if (val == s)
      return traits_t::sin(s);

    check_complex(val, i, prechecked);

    typedef Multivector<Scalar_T,LO,HI> multivector_t;
    static const Scalar_T& twopi = Scalar_T(2) * traits_t::pi();
    const multivector_t& z = i *
      (val - s + traits_t::fmod(s, twopi));
    return i * (exp(-z)-exp(z)) / Scalar_T(2);
  }

  /// Sine of multivector
  template< template<typename, const index_t, const index_t> class Multivector,
            typename Scalar_T, const index_t LO, const index_t HI >
  inline
  const Multivector<Scalar_T,LO,HI>
  sin(const Multivector<Scalar_T,LO,HI>& val)
  { return sin(val, complexifier(val), true); }

  /// Inverse sine of multivector with specified complexifier
  // Reference: [AS], Section 4.4, p79-83
  template< template<typename, const index_t, const index_t> class Multivector,
            typename Scalar_T, const index_t LO, const index_t HI >
  inline
  const Multivector<Scalar_T,LO,HI>
  asin(const Multivector<Scalar_T,LO,HI>& val, const Multivector<Scalar_T,LO,HI>& i, bool prechecked)
  {
    typedef numeric_traits<Scalar_T> traits_t;
    if (val.isnan())
      return traits_t::NaN();

    const Scalar_T& realval = val.scalar();
    if (val == realval && traits_t::abs(realval) <= Scalar_T(1))
      return traits_t::asin(realval);

    check_complex(val, i, prechecked);
    return -i * asinh(i * val, i, true);
  }

  /// Inverse sine of multivector
  // Reference: [AS], Section 4.4, p79-83
  template< template<typename, const index_t, const index_t> class Multivector,
            typename Scalar_T, const index_t LO, const index_t HI >
  inline
  const Multivector<Scalar_T,LO,HI>
  asin(const Multivector<Scalar_T,LO,HI>& val)
  { return asin(val, complexifier(val), true); }

  /// Hyperbolic tangent of multivector
  template< template<typename, const index_t, const index_t> class Multivector,
            typename Scalar_T, const index_t LO, const index_t HI >
  inline
  const Multivector<Scalar_T,LO,HI>
  tanh(const Multivector<Scalar_T,LO,HI>& val)
  {
    typedef numeric_traits<Scalar_T> traits_t;
    if (val.isnan())
      return traits_t::NaN();

    const Scalar_T& s = scalar(val);
    if (val == s)
      return traits_t::tanh(s);

    return sinh(val) / cosh(val);
  }

  /// Inverse hyperbolic tangent of multivector with specified complexifier
  // Reference: [AS], Section 4.6, p86-89
  template< template<typename, const index_t, const index_t> class Multivector,
            typename Scalar_T, const index_t LO, const index_t HI >
  inline
  const Multivector<Scalar_T,LO,HI>
  atanh(const Multivector<Scalar_T,LO,HI>& val, const Multivector<Scalar_T,LO,HI>& i, bool prechecked)
  {
    typedef numeric_traits<Scalar_T> traits_t;
    check_complex(val, i, prechecked);
    return val.isnan()
        ? traits_t::NaN()
        : (norm(val + Scalar_T(1)) > norm(val - Scalar_T(1)))
          ? (log(val + Scalar_T(1), i, true) - log(-val + Scalar_T(1), i, true)) / Scalar_T(2)
          : log((val + Scalar_T(1)) / (-val + Scalar_T(1)), i, true) / Scalar_T(2);
  }

  /// Inverse hyperbolic tangent of multivector
  // Reference: [AS], Section 4.6, p86-89
  template< template<typename, const index_t, const index_t> class Multivector,
            typename Scalar_T, const index_t LO, const index_t HI >
  inline
  const Multivector<Scalar_T,LO,HI>
  atanh(const Multivector<Scalar_T,LO,HI>& val)
  { return atanh(val, complexifier(val), true); }

  /// Tangent of multivector with specified complexifier
  template< template<typename, const index_t, const index_t> class Multivector,
            typename Scalar_T, const index_t LO, const index_t HI >
  inline
  const Multivector<Scalar_T,LO,HI>
  tan(const Multivector<Scalar_T,LO,HI>& val, const Multivector<Scalar_T,LO,HI>& i, bool prechecked)
  {
    typedef numeric_traits<Scalar_T> traits_t;
    if (val.isnan())
      return traits_t::NaN();

    const Scalar_T& s = scalar(val);
    if (val == s)
      return traits_t::tan(s);

    check_complex(val, i, prechecked);
    return sin(val, i, true) / cos(val, i, true);
  }

  /// Tangent of multivector
  template< template<typename, const index_t, const index_t> class Multivector,
            typename Scalar_T, const index_t LO, const index_t HI >
  inline
  const Multivector<Scalar_T,LO,HI>
  tan(const Multivector<Scalar_T,LO,HI>& val)
  { return tan(val, complexifier(val), true); }

  /// Inverse tangent of multivector with specified complexifier
  // Reference: [AS], Section 4.4, p79-83
  template< template<typename, const index_t, const index_t> class Multivector,
            typename Scalar_T, const index_t LO, const index_t HI >
  inline
  const Multivector<Scalar_T,LO,HI>
  atan(const Multivector<Scalar_T,LO,HI>& val, const Multivector<Scalar_T,LO,HI>& i, bool prechecked)
  {
    typedef numeric_traits<Scalar_T> traits_t;
    if (val.isnan())
      return traits_t::NaN();

    const Scalar_T& s = scalar(val);
    if (val == s)
      return traits_t::atan(s);

    check_complex(val, i, prechecked);
    return -i * atanh(i * val, i, true);
  }

  /// Inverse tangent of multivector
  // Reference: [AS], Section 4.4, p79-83
  template< template<typename, const index_t, const index_t> class Multivector,
            typename Scalar_T, const index_t LO, const index_t HI >
  inline
  const Multivector<Scalar_T,LO,HI>
  atan(const Multivector<Scalar_T,LO,HI>& val)
  { return atan(val, complexifier(val), true); }

}
#endif  // _GLUCAT_CLIFFORD_ALGEBRA_IMP_H
