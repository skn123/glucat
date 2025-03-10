#ifndef _GLUCAT_FRAMED_MULTI_IMP_H
#define _GLUCAT_FRAMED_MULTI_IMP_H
/***************************************************************************
    GluCat : Generic library of universal Clifford algebra templates
    framed_multi_imp.h : Implement the coordinate map representation of a
    Clifford algebra element
                             -------------------
    begin                : Sun 2001-12-09
    copyright            : (C) 2001-2021 by Paul C. Leopardi
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

#include "glucat/framed_multi.h"

#include "glucat/scalar.h"
#include "glucat/random.h"
#include "glucat/generation.h"
#include "glucat/matrix.h"

#include <sstream>
#include <fstream>

namespace glucat
{
  /// Class name used in messages
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  auto
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  classname() -> const std::string
  { return "framed_multi"; }

#define _GLUCAT_HASH_N(x) (x)
#define _GLUCAT_HASH_SIZE_T(x) (typename multivector_t::hash_size_t)(x)

  /// Default constructor
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  framed_multi()
  : map_t(_GLUCAT_HASH_N(0))
  { }

  /// Private constructor using hash_size
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  framed_multi(const hash_size_t& hash_size)
  : map_t(_GLUCAT_HASH_N(hash_size()))
  { }

  /// Construct a multivector from a multivector with a different scalar type
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  template< typename Other_Scalar_T >
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  framed_multi(const framed_multi<Other_Scalar_T,LO,HI,Tune_P>& val)
  : map_t(_GLUCAT_HASH_N(val.size()))
  {
    for (auto& val_term : val)
      this->insert(term_t(val_term.first, numeric_traits<Scalar_T>::to_scalar_t(val_term.second)));
  }

  /// Construct a multivector, within a given frame, from a given multivector
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  template< typename Other_Scalar_T >
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  framed_multi(const framed_multi<Other_Scalar_T,LO,HI,Tune_P>& val,
               const index_set_t frm, const bool prechecked)
  : map_t(_GLUCAT_HASH_N(val.size()))
  {
    if (!prechecked && (val.frame() | frm) != frm)
      throw error_t("multivector_t(val,frm): cannot initialize with value outside of frame");
    for (auto& val_term : val)
      this->insert(term_t(val_term.first, numeric_traits<Scalar_T>::to_scalar_t(val_term.second)));
  }

  /// Construct a multivector, within a given frame, from a given multivector
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  framed_multi(const multivector_t& val,
               const index_set_t frm, const bool prechecked)
  : map_t(_GLUCAT_HASH_N(val.size()))
  {
    if (!prechecked && (val.frame() | frm) != frm)
      throw error_t("multivector_t(val,frm): cannot initialize with value outside of frame");
    for (auto& val_term : val)
      this->insert(val_term);
  }

  /// Construct a multivector from an index set and a scalar coordinate
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  framed_multi(const index_set_t ist, const Scalar_T& crd)
  : map_t(_GLUCAT_HASH_N(1))
  {
    if (crd != Scalar_T(0))
      this->insert(term_t(ist, crd));
  }

  /// Construct a multivector, within a given frame, from an index set and a scalar coordinate
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  framed_multi(const index_set_t ist, const Scalar_T& crd,
               const index_set_t frm, const bool prechecked)
  : map_t(_GLUCAT_HASH_N(1))
  {
    if (!prechecked && (ist | frm) != frm)
      throw error_t("multivector_t(ist,crd,frm): cannot initialize with value outside of frame");
    if (crd != Scalar_T(0))
      this->insert(term_t(ist, crd));
  }

  /// Construct a multivector from a scalar (within a frame, if given)
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  framed_multi(const Scalar_T& scr, const index_set_t frm)
  : map_t(_GLUCAT_HASH_N(1))
  {
    if (scr != Scalar_T(0))
      this->insert(term_t(index_set_t(), scr));
  }

  /// Construct a multivector from an int (within a frame, if given)
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  framed_multi(const int scr, const index_set_t frm)
  : map_t(_GLUCAT_HASH_N(1))
  {
    if (scr != Scalar_T(0))
      this->insert(term_t(index_set_t(), Scalar_T(scr)));
  }

  /// Construct a multivector, within a given frame, from a given vector
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  framed_multi(const vector_t& vec,
               const index_set_t frm, const bool prechecked)
  : map_t(_GLUCAT_HASH_N(vec.size()))
  {
    if (!prechecked && index_t(vec.size()) != frm.count())
      throw error_t("multivector_t(vec,frm): cannot initialize with vector not matching frame");
    auto idx = frm.min();
    const auto frm_end = frm.max()+1;
    for (auto& crd : vec)
    {
      *this += term_t(index_set_t(idx), crd);
      for (
        ++idx;
        idx != frm_end && !frm[idx];
        ++idx)
        ;
    }
  }

  /// Construct a multivector from a string: eg: "3+2{1,2}-6.1e-2{2,3}"
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  framed_multi(const std::string& str)
  : map_t(_GLUCAT_HASH_N(0))
  {
    std::istringstream ss(str);
    ss >> *this;
    if (!ss)
      throw error_t("multivector_t(str): could not parse string");
    // Peek to see if the end of the string has been reached.
    ss.peek();
    if (!ss.eof())
      throw error_t("multivector_t(str): could not parse entire string");
  }

  /// Construct a multivector, within a given frame, from a string: eg: "3+2{1,2}-6.1e-2{2,3}"
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  framed_multi(const std::string& str, const index_set_t frm, const bool prechecked)
  : map_t(_GLUCAT_HASH_N(0))
  {
    if (prechecked)
      *this = multivector_t(str);
    else
      *this = multivector_t(multivector_t(str), frm, false);
  }

  /// Construct a multivector from a matrix_multi_t
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  template< typename Other_Scalar_T >
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  framed_multi(const matrix_multi<Other_Scalar_T,LO,HI,Tune_P>& val)
  : map_t(_GLUCAT_HASH_N(1))
  {
    if (val == Other_Scalar_T(0))
      return;

    const auto dim = val.m_matrix.size1();
    using traits_t = numeric_traits<Scalar_T>;
    if (dim == 1)
    {
      this->insert(term_t(index_set_t(), traits_t::to_scalar_t(val.m_matrix(0, 0))));
      return;
    }
    if (dim >= Tune_P::inv_fast_dim_threshold)
      try
      {
        *this = (val.template fast_framed_multi<Scalar_T>()).truncated();
        return;
      }
      catch (const glucat_error& e)
      { }

    const auto val_norm = traits_t::to_scalar_t(val.norm());
    if (traits_t::isNaN_or_isInf(val_norm))
    {
      *this = traits_t::NaN();
      return;
    }
    const auto frm = val.frame();
    const auto algebra_dim = set_value_t(1) << frm.count();
    auto result = multivector_t(
      _GLUCAT_HASH_SIZE_T(std::min<size_t>(algebra_dim, matrix::nnz(val.m_matrix))));
    for (auto
        stv = set_value_t(0);
        stv != algebra_dim;
        stv++)
    {
      const auto ist = index_set_t(stv, frm, true);
      const auto crd =
        traits_t::to_scalar_t(matrix::inner<Other_Scalar_T>(val.basis_element(ist), val.m_matrix));
      if (crd != Scalar_T(0))
        result.insert(term_t(ist, crd));
    }
    *this = result.truncated();
  }

  /// Test for equality of multivectors
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  auto
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  operator==  (const multivector_t& rhs) const -> bool
  {
    if (this->size() != rhs.size())
      return false;
    const auto rhs_end = rhs.end();
    for (auto& this_term : *this)
    {
      const const_iterator& rhs_it = rhs.find(this_term.first);
      if (rhs_it == rhs_end || rhs_it->second != this_term.second)
        return false;
    }
    return true;
  }

  /// Test for equality of multivector and scalar
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  inline
  auto
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  operator==  (const Scalar_T& scr) const -> bool
  {
    switch (this->size())
    {
    case 0:
      return scr == Scalar_T(0);
    case 1:
      {
        const auto& this_it = this->begin();
        return this_it->first == index_set_t() && this_it->second == scr;
      }
    default:
      return false;
    }
  }

  /// Geometric sum of multivector and scalar
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  inline
  auto
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  operator+= (const Scalar_T& scr) -> multivector_t&
  {
    *this += term_t(index_set_t(), scr);
    return *this;
  }

  /// Geometric sum
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  inline
  auto
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  operator+= (const multivector_t& rhs) -> multivector_t&
  { // simply add terms
    for (auto& rhs_term : rhs)
      *this += rhs_term;
    return *this;
  }

  /// Geometric difference of multivector and scalar
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  inline
  auto
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  operator-= (const Scalar_T& scr) -> multivector_t&
  {
    *this += term_t(index_set_t(), -scr);
    return *this;
  }

  /// Geometric difference
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  inline
  auto
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  operator-= (const multivector_t& rhs) -> multivector_t&
  {
    for (auto& rhs_term : rhs)
      *this += term_t(rhs_term.first, -(rhs_term.second));
    return *this;
  }

  /// Unary -
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  inline
  auto
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  operator- () const -> const multivector_t
  { // multiply coordinates of all terms by -1
    auto result = *this;
    for (auto& result_term : result)
      result_term.second *= Scalar_T(-1);
    return result;
  }

  /// Product of multivector and scalar
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  auto
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  operator*= (const Scalar_T& scr) -> multivector_t&
  { // multiply coordinates of all terms by scalar
    using traits_t = numeric_traits<Scalar_T>;

    if (traits_t::isNaN_or_isInf(scr))
      return *this = traits_t::NaN();
    if (scr == Scalar_T(0))
      if (this->isnan())
        *this = traits_t::NaN();
      else
        this->clear();
    else
      for (auto& this_term : *this)
        this_term.second *= scr;
    return *this;
  }

  /// Geometric product
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  auto
  operator* (const framed_multi<Scalar_T,LO,HI,Tune_P>& lhs, const framed_multi<Scalar_T,LO,HI,Tune_P>& rhs) -> const framed_multi<Scalar_T,LO,HI,Tune_P>
  {
    using multivector_t = framed_multi<Scalar_T,LO,HI,Tune_P>;
    using traits_t = numeric_traits<Scalar_T>;

    if (lhs.isnan() || rhs.isnan())
      return traits_t::NaN();

    const double lhs_size = lhs.size();
    const double rhs_size = rhs.size();
    const auto our_frame = lhs.frame() | rhs.frame();
    const auto frm_count = our_frame.count();
    const auto algebra_dim = set_value_t(1) << frm_count;
    const auto direct_mult = lhs_size * rhs_size <= double(algebra_dim);
    if (direct_mult)
    { // If we have a sparse multiply, store the result directly
      auto result = multivector_t(
        _GLUCAT_HASH_SIZE_T(size_t(std::min(lhs_size * rhs_size, double(algebra_dim)))));
      for (auto& lhs_term : lhs)
        for (auto& rhs_term : rhs)
          result += lhs_term * rhs_term;
      return result;
    }
    else
    { // Past a certain threshold, the matrix algorithm is fastest
      using matrix_multi_t = typename multivector_t::matrix_multi_t;
      return matrix_multi_t(lhs, our_frame, true) *
             matrix_multi_t(rhs, our_frame, true);
    }
  }

  /// Geometric product
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  inline
  auto
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  operator*= (const multivector_t& rhs) -> multivector_t&
  { return *this = *this * rhs; }

  /// Outer product
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  auto
  operator^ (const framed_multi<Scalar_T,LO,HI,Tune_P>& lhs, const framed_multi<Scalar_T,LO,HI,Tune_P>& rhs) -> const framed_multi<Scalar_T,LO,HI,Tune_P>
  { // Arvind Raja's original reference:
    // "old clical, outerproduct(p,q:pterm):pterm in file compmod.pas"

    if (lhs.empty() || rhs.empty())
      return Scalar_T(0);

    using multivector_t = framed_multi<Scalar_T,LO,HI,Tune_P>;
    using index_set_t = typename multivector_t::index_set_t;
    using term_t = typename multivector_t::term_t;

    const auto empty_set = index_set_t();

    const double lhs_size = lhs.size();
    const double rhs_size = rhs.size();
    const auto lhs_frame = lhs.frame();
    const auto rhs_frame = rhs.frame();
    const auto our_frame = lhs_frame | rhs_frame;
    const auto algebra_dim = set_value_t(1) << our_frame.count();
    auto result = multivector_t(
      _GLUCAT_HASH_SIZE_T(size_t(std::min(lhs_size * rhs_size, double(algebra_dim)))));
    const auto lhs_end = lhs.end();
    const auto rhs_end = rhs.end();

    if (lhs_size * rhs_size > double(Tune_P::products_size_threshold))
    {
      for (auto
          result_stv = set_value_t(0);
          result_stv != algebra_dim;
          ++result_stv)
      {
        const auto result_ist = index_set_t(result_stv, our_frame, true);
        const auto lhs_result_frame = lhs_frame & result_ist;
        const auto lhs_result_dim = set_value_t(1) << lhs_result_frame.count();
        auto result_crd = Scalar_T(0);
        for (auto
            lhs_stv = set_value_t(0);
            lhs_stv != lhs_result_dim;
            ++lhs_stv)
        {
          const auto lhs_ist = index_set_t(lhs_stv, lhs_result_frame, true);
          const auto rhs_ist = result_ist ^ lhs_ist;
          if ((rhs_ist | rhs_frame) == rhs_frame)
          {
            const auto lhs_it = lhs.find(lhs_ist);
            if (lhs_it != lhs_end)
            {
              const auto rhs_it = rhs.find(rhs_ist);
              if (rhs_it != rhs_end)
                result_crd += crd_of_mult(*lhs_it, *rhs_it);
            }
          }
        }
        if (result_crd != Scalar_T(0))
          result.insert(term_t(result_ist, result_crd));
      }
      return result;
    }
    else
    {
      for (auto& lhs_term : lhs)
        for (auto& rhs_term : rhs)
          if ((lhs_term.first & rhs_term.first) == empty_set)
            result += lhs_term * rhs_term;
      return result;
    }
  }

  /// Outer product
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  inline
  auto
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  operator^= (const multivector_t& rhs) -> multivector_t&
  { return *this = *this ^ rhs; }

  /// Inner product
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  auto
  operator& (const framed_multi<Scalar_T,LO,HI,Tune_P>& lhs, const framed_multi<Scalar_T,LO,HI,Tune_P>& rhs) -> const framed_multi<Scalar_T,LO,HI,Tune_P>
  { // Arvind Raja's original reference:
    // "old clical, innerproduct(p,q:pterm):pterm in file compmod.pas"

    if (lhs.empty() || rhs.empty())
      return Scalar_T(0);

    using multivector_t = framed_multi<Scalar_T,LO,HI,Tune_P>;
    using index_set_t = typename multivector_t::index_set_t;
    using term_t = typename multivector_t::term_t;

    const auto lhs_end = lhs.end();
    const auto rhs_end = rhs.end();
    const double lhs_size = lhs.size();
    const double rhs_size = rhs.size();

    const auto lhs_frame = lhs.frame();
    const auto rhs_frame = rhs.frame();

    const auto our_frame = lhs_frame | rhs_frame;
    const auto algebra_dim = set_value_t(1) << our_frame.count();
    auto result = multivector_t(
      _GLUCAT_HASH_SIZE_T(size_t(std::min(lhs_size * rhs_size, double(algebra_dim)))));
    if (lhs_size * rhs_size > double(Tune_P::products_size_threshold))
    {
      for (auto
          result_stv = set_value_t(0);
          result_stv != algebra_dim;
          ++result_stv)
      {
        const auto result_ist = index_set_t(result_stv, our_frame, true);
        const auto comp_frame = our_frame & ~result_ist;
        const auto comp_dim = set_value_t(1) << comp_frame.count();
        auto result_crd = Scalar_T(0);
        for (auto
            comp_stv = set_value_t(1);
            comp_stv != comp_dim;
            ++comp_stv)
        {
          const auto comp_ist = index_set_t(comp_stv, comp_frame, true);
          const auto our_ist = result_ist ^ comp_ist;
          if ((our_ist | lhs_frame) == lhs_frame)
          {
            const auto lhs_it = lhs.find(our_ist);
            if (lhs_it != lhs_end)
            {
              const auto rhs_it = rhs.find(comp_ist);
              if (rhs_it != rhs_end)
                result_crd += crd_of_mult(*lhs_it, *rhs_it);
            }
          }
          if (result_stv != 0)
          {
            if ((our_ist | rhs_frame) == rhs_frame)
            {
              const auto rhs_it = rhs.find(our_ist);
              if (rhs_it != rhs_end)
              {
                const auto lhs_it = lhs.find(comp_ist);
                if (lhs_it != lhs_end)
                  result_crd += crd_of_mult(*lhs_it, *rhs_it);
              }
            }
          }
        }
        if (result_crd != Scalar_T(0))
          result.insert(term_t(result_ist, result_crd));
      }
    }
    else
    {
      const auto empty_set = index_set_t();
      for (auto& lhs_term : lhs)
      {
        const auto lhs_ist = lhs_term.first;
        if (lhs_ist != empty_set)
          for (auto& rhs_term : rhs)
          {
            const auto rhs_ist = rhs_term.first;
            if (rhs_ist != empty_set)
            {
              const auto our_ist = lhs_ist | rhs_ist;
              if ((lhs_ist == our_ist) || (rhs_ist == our_ist))
                result += lhs_term * rhs_term;
            }
          }
      }
    }
    return result;
  }

  /// Inner product
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  inline
  auto
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  operator&= (const multivector_t& rhs) -> multivector_t&
  { return *this = *this & rhs; }

  /// Left contraction
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  auto
  operator% (const framed_multi<Scalar_T,LO,HI,Tune_P>& lhs, const framed_multi<Scalar_T,LO,HI,Tune_P>& rhs) -> const framed_multi<Scalar_T,LO,HI,Tune_P>
  {
    // Reference: Leo Dorst, "Honing geometric algebra for its use in the computer sciences",
    // in Geometric Computing with Clifford Algebras, ed. G. Sommer,
    // Springer 2001, Chapter 6, pp. 127-152.
    // http://staff.science.uva.nl/~leo/clifford/index.html

    if (lhs.empty() || rhs.empty())
      return Scalar_T(0);

    using multivector_t = framed_multi<Scalar_T,LO,HI,Tune_P>;
    using index_set_t = typename multivector_t::index_set_t;
    using term_t = typename multivector_t::term_t;

    const auto lhs_end   = lhs.end();
    const auto rhs_end   = rhs.end();
    const double lhs_size = lhs.size();
    const double rhs_size = rhs.size();
    const auto lhs_frame = lhs.frame();
    const auto rhs_frame = rhs.frame();

    const auto our_frame = lhs_frame | rhs_frame;
    const auto algebra_dim = set_value_t(1) << our_frame.count();
    auto result = multivector_t(
      _GLUCAT_HASH_SIZE_T(size_t(std::min(lhs_size * rhs_size, double(algebra_dim)))));

    if (lhs_size * rhs_size > double(Tune_P::products_size_threshold))
    {
      for (auto
          result_stv = set_value_t(0);
          result_stv != algebra_dim;
          ++result_stv)
      {
        const auto result_ist = index_set_t(result_stv, our_frame, true);
        const auto comp_frame = lhs_frame & ~result_ist;
        const auto comp_dim = set_value_t(1) << comp_frame.count();
        auto result_crd = Scalar_T(0);
        for (auto
            comp_stv = set_value_t(0);
            comp_stv != comp_dim;
            ++comp_stv)
        {
          const auto comp_ist = index_set_t(comp_stv, comp_frame, true);
          const auto rhs_ist = result_ist ^ comp_ist;
          if ((rhs_ist | rhs_frame) == rhs_frame)
          {
            const auto rhs_it = rhs.find(rhs_ist);
            if (rhs_it != rhs_end)
            {
              const auto lhs_it = lhs.find(comp_ist);
              if (lhs_it != lhs_end)
                result_crd += crd_of_mult(*lhs_it, *rhs_it);
            }
          }
        }
        if (result_crd != Scalar_T(0))
          result.insert(term_t(result_ist, result_crd));
      }
    }
    else
    {
      for (auto& rhs_term : rhs)
      {
        const auto rhs_ist = rhs_term.first;
        for (auto& lhs_term : lhs)
        {
          const index_set_t lhs_ist = lhs_term.first;
          if ((lhs_ist | rhs_ist) == rhs_ist)
            result += lhs_term * rhs_term;
        }
      }
    }
    return result;
  }

  /// Left contraction
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  inline
  auto
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  operator%= (const multivector_t& rhs) -> multivector_t&
  { return *this = *this % rhs; }

  /// Hestenes scalar product
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  auto
  star(const framed_multi<Scalar_T,LO,HI,Tune_P>& lhs, const framed_multi<Scalar_T,LO,HI,Tune_P>& rhs) -> Scalar_T
  {
    auto result = Scalar_T(0);
    const auto small_star_large = lhs.size() < rhs.size();
    const auto* smallp =
      small_star_large
      ? &lhs
      : &rhs;
    const auto* largep =
      small_star_large
      ? &rhs
      : &lhs;

    for (auto& small_term : *smallp)
    {
      const auto small_ist = small_term.first;
      const auto large_crd = (*largep)[small_ist];
      if (large_crd != Scalar_T(0))
        result += small_ist.sign_of_square() * small_term.second * large_crd;
    }
    return result;
  }

  /// Quotient of multivector and scalar
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  auto
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  operator/= (const Scalar_T& scr) -> multivector_t&
  { // Divide coordinates of all terms by scr
    using traits_t = numeric_traits<Scalar_T>;

    if (traits_t::isNaN(scr))
      return *this = traits_t::NaN();
    if (traits_t::isInf(scr))
      if (this->isnan())
        *this = traits_t::NaN();
      else
        this->clear();
    else
      for (auto& this_term : *this)
        this_term.second /= scr;
    return *this;
  }

  /// Geometric quotient
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  inline
  auto
  operator/ (const framed_multi<Scalar_T,LO,HI,Tune_P>& lhs, const framed_multi<Scalar_T,LO,HI,Tune_P>& rhs) -> const framed_multi<Scalar_T,LO,HI,Tune_P>
  {
    using multivector_t = framed_multi<Scalar_T,LO,HI,Tune_P>;
    using traits_t = numeric_traits<Scalar_T>;
    using matrix_multi_t = typename multivector_t::matrix_multi_t;

    if (rhs == Scalar_T(0))
      return traits_t::NaN();

    const auto our_frame = lhs.frame() | rhs.frame();
    return matrix_multi_t(lhs, our_frame, true) / matrix_multi_t(rhs, our_frame, true);
  }

  /// Geometric quotient
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  inline
  auto
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  operator/= (const multivector_t& rhs) -> multivector_t&
  { return *this = *this / rhs; }

  /// Transformation via twisted adjoint action
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  inline
  auto
  operator| (const framed_multi<Scalar_T,LO,HI,Tune_P>& lhs, const framed_multi<Scalar_T,LO,HI,Tune_P>& rhs) -> const framed_multi<Scalar_T,LO,HI,Tune_P>
  {
    using multivector_t = framed_multi<Scalar_T,LO,HI,Tune_P>;
    using matrix_multi_t = typename multivector_t::matrix_multi_t;

    return matrix_multi_t(rhs) * matrix_multi_t(lhs) / matrix_multi_t(rhs.involute());
  }

  /// Transformation via twisted adjoint action
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  inline
  auto
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  operator|= (const multivector_t& rhs) -> multivector_t&
  { return *this = *this | rhs; }

  /// Clifford multiplicative inverse
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  inline
  auto
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  inv() const -> const multivector_t
  {
    auto result = matrix_multi_t(Scalar_T(1), this->frame());
    return result /= matrix_multi_t(*this);
  }

  /// Integer power of multivector: *this to the m
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  auto
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  pow(int m) const -> const multivector_t
  { return glucat::pow(*this, m); }

  /// Outer product power of multivector
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  auto
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  outer_pow(int m) const -> const multivector_t
  {
    if (m < 0)
      throw error_t("outer_pow(int): negative exponent");
    auto result = multivector_t(Scalar_T(1));
    auto a = *this;
    for (;
        m != 0;
        m >>= 1, a = a ^ a)
      if (m & 1)
        result ^= a;
    return result;
  }

  /// Frame of multivector: union of index sets of terms
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  inline
  auto
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  frame() const -> const index_set_t
  {
    auto result = index_set_t();
    for (auto& this_term : *this)
      result |= this_term.first;
    return result;
  }

  /// Grade of multivector: maximum of the grades of each term
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  inline
  auto
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  grade() const -> index_t
  {
    auto result = index_t(0);
    for (auto& this_term : *this)
      result = std::max(result, this_term.first.count());
    return result;
  }

  /// Subscripting: map from index set to scalar coordinate
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  inline
  auto
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  operator[] (const index_set_t ist) const -> Scalar_T
  {
    const auto& this_it = this->find(ist);
    if (this_it == this->end())
      return Scalar_T(0);
    else
      return this_it->second;
  }

  /// Grading: part where each term is a grade-vector
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  auto
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  operator() (index_t grade) const -> const multivector_t
  {
    if ((grade < 0) || (grade > HI-LO))
      return Scalar_T(0);
    else
    {
      auto result = multivector_t();
      for (auto& this_term : *this)
        if (this_term.first.count() == grade)
          result += this_term;
      return result;
    }
  }

  /// Scalar part
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  inline
  auto
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  scalar() const -> Scalar_T
  { return (*this)[index_set_t()]; }

  /// Pure part
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  inline
  auto
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  pure() const -> const multivector_t
  { return *this - this->scalar(); }

  /// Even part, sum of the even grade terms
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  auto
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  even() const -> const multivector_t
  { // even part of x, sum of the pure(count) with even count
    auto result = multivector_t();
    for (auto& this_term : *this)
      if ((this_term.first.count() % 2) == 0)
        result.insert(this_term);
    return result;
  }

  /// Odd part, sum of the odd grade terms
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  auto
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  odd() const -> const multivector_t
  { // even part of x, sum of the pure(count) with even count
    auto result = multivector_t();
    for (auto& this_term : *this)
      if ((this_term.first.count() % 2) == 1)
        result.insert(this_term);
    return result;
  }

  /// Vector part of multivector, as a vector_t
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  auto
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  vector_part() const -> const vector_t
  { return this->vector_part(this->frame(), true); }

  /// Vector part of multivector, as a vector_t
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  auto
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  vector_part(const index_set_t frm, const bool prechecked) const -> const vector_t
  {
    if (!prechecked && (this->frame() | frm) != frm)
      throw error_t("vector_part(frm): value is outside of requested frame");
    auto result = vector_t();
    result.reserve(frm.count());
    const auto frm_end = frm.max()+1;
    for (auto
        idx  = frm.min();
        idx != frm_end;
        ++idx)
      // Frame may contain indices which do not correspond to a grade 1 term but
      // frame cannot omit any index corresponding to a grade 1 term
      if (frm[idx])
        result.push_back((*this)[index_set_t(idx)]);
    return result;
  }

  /// Main involution, each {i} is replaced by -{i} in each term
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  auto
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  involute() const -> const multivector_t
  {
    auto result = *this;
    for (auto& result_term : result)
    { // for a k-vector u, involute(u) == (-1)^k * u
      if ((result_term.first.count() % 2) == 1)
        result_term.second *= Scalar_T(-1);
    }
    return result;
  }

  /// Reversion, order of {i} is reversed in each term
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  auto
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  reverse() const -> const multivector_t
  {
    auto result = *this;
    for (auto& result_term : result)
      // For a k-vector u, reverse(u) = { -u, k == 2,3 (mod 4)
      //                                {  u, k == 0,1 (mod 4)
      switch (result_term.first.count() % 4)
      {
      case 2:
      case 3:
        result_term.second *= Scalar_T(-1);
        break;
      default:
        break;
      }
    return result;
  }

  /// Conjugation, conj == reverse o involute == involute o reverse
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  auto
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  conj() const -> const multivector_t
  {
    auto result = *this;
    for (auto& result_term : result)
      // For a k-vector u, conj(u) = { -u, k == 1,2 (mod 4)
      //                             {  u, k == 0,3 (mod 4)
      switch (result_term.first.count() % 4)
      {
      case 1:
      case 2:
        result_term.second *= Scalar_T(-1);
        break;
      default:
        break;
      }
    return result;
  }

  /// Quadratic form := scalar part of rev(x)*x
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  auto
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  quad() const -> Scalar_T
  {
    // scalar(conj(x)*x) = 2*quad(even(x)) - quad(x)
    // ref: old clical: quadfunction(p:pter):pterm in file compmod.pas
    auto result = Scalar_T(0);
    for (auto& this_term : *this)
    {
      const auto sign =
        (this_term.first.count_neg() % 2)
        ? -Scalar_T(1)
        :  Scalar_T(1);
      result += sign * (this_term.second) * (this_term.second);
    }
    return result;
  }

  /// Norm squared := sum of norm squared of coordinates
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  auto
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  norm() const -> Scalar_T
  {
    using traits_t = numeric_traits<Scalar_T>;

    auto result = Scalar_T(0);
    for (auto& this_term : *this)
    {
      const auto abs_crd = traits_t::abs(this_term.second);
      result +=  abs_crd * abs_crd;
    }
    return result;
  }

  /// Maximum of absolute values of components of multivector: multivector infinity norm
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  auto
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  max_abs() const -> Scalar_T
  {
    using traits_t = numeric_traits<Scalar_T>;

    auto result = Scalar_T(0);
    for (auto& this_term : *this)
    {
      const auto abs_crd = traits_t::abs(this_term.second);
      if (abs_crd > result)
        result = abs_crd;
    }
    return result;
  }

  /// Random multivector within a frame
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  auto
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  random(const index_set_t frm, Scalar_T fill) -> const multivector_t
  {
    using multivector_t = framed_multi<Scalar_T,LO,HI,Tune_P>;
    using index_set_t = typename multivector_t::index_set_t;
    using term_t = typename multivector_t::term_t;

    using random_generator_t = random_generator<Scalar_T>;
    auto& generator = random_generator_t::generator();

    fill =
      (fill < Scalar_T(0))
      ? Scalar_T(0)
      : (fill > Scalar_T(1))
        ? Scalar_T(1)
        : fill;
    const auto algebra_dim = set_value_t(1) << frm.count();
    using traits_t = numeric_traits<Scalar_T>;
    const auto mean_abs = traits_t::sqrt(Scalar_T(double(algebra_dim)));
    auto result = multivector_t();
    for (auto
        stv = set_value_t(0);
        stv != algebra_dim;
        ++stv)
      if (generator.uniform() < fill)
      {
        const auto& result_crd = generator.normal() / mean_abs;
        result.insert(term_t(index_set_t(stv, frm, true), result_crd));
      }
    return result;
  }

  /// Write multivector to output
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  inline
  void
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  write(const std::string& msg) const
  { std::cout << msg << std::endl << "  " << (*this) << std::endl; }

  /// Write multivector to file
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  inline
  void
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  write(std::ofstream& ofile, const std::string& msg) const
  {
    if (!ofile)
      throw error_t("write(ofile,msg): cannot write to output file");
    ofile << msg << std::endl << "  " << (*this) << std::endl;
  }

  /// Sorted range for use with output
  template< typename Map_T,typename Sorted_Map_T >
  class sorted_range
  {
  public:
    using map_t = Map_T;
    using sorted_map_t = Sorted_Map_T;
    using sorted_iterator = typename Sorted_Map_T::const_iterator;

    sorted_range (Sorted_Map_T &sorted_val, const Map_T& val)
    {
      for (auto& val_term : val)
        sorted_val.insert(val_term);
      sorted_begin = sorted_val.begin();
      sorted_end   = sorted_val.end();
    }
    sorted_iterator sorted_begin;
    sorted_iterator sorted_end;
  };

  template< typename Sorted_Map_T >
  class sorted_range< Sorted_Map_T, Sorted_Map_T >
  {
  public:
    using map_t = Sorted_Map_T;
    using sorted_map_t = Sorted_Map_T;
    using sorted_iterator = typename Sorted_Map_T::const_iterator;

    sorted_range (Sorted_Map_T &sorted_val, const Sorted_Map_T& val)
    : sorted_begin( val.begin() ),
      sorted_end( val.end() )
    { }
    sorted_iterator sorted_begin;
    sorted_iterator sorted_end;
  };

  /// Write multivector to output
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  auto
  operator<< (std::ostream& os, const framed_multi<Scalar_T,LO,HI,Tune_P>& val) -> std::ostream&
  {
    using limits_t = std::numeric_limits<Scalar_T>;
    if (val.empty())
      os << 0;
    else if (val.isnan())
      os << limits_t::quiet_NaN();
    else if (val.isinf())
    {
      const Scalar_T& inf = limits_t::infinity();
      os << (scalar(val) < 0.0 ? -inf : inf);
    }
    else
    {
      using traits_t = numeric_traits<Scalar_T>;
      using multivector_t = framed_multi<Scalar_T,LO,HI,Tune_P>;
      Scalar_T truncation;
      switch (os.flags() & std::ios::floatfield)
      {
        case std::ios_base::scientific:
          truncation = Scalar_T(1) / traits_t::pow(Scalar_T(10), int(os.precision()) + 1);
          break;
        case std::ios_base::fixed:
          truncation = Scalar_T(1) / (traits_t::pow(Scalar_T(10), int(os.precision())) * val.max_abs());
          break;
        case std::ios_base::fixed | std::ios_base::scientific:
          truncation = multivector_t::default_truncation;
          break;
        default:
          truncation = Scalar_T(1) / traits_t::pow(Scalar_T(10), int(os.precision()));
          break;
      }
      auto truncated_val = val.truncated(truncation);
      if (truncated_val.empty())
        os << 0;
      else
      {
        using map_t = typename multivector_t::map_t;
        using sorted_map_t = typename multivector_t::sorted_map_t;
        auto sorted_val = sorted_map_t();
        const auto sorted_val_range = sorted_range< map_t, sorted_map_t >(sorted_val, truncated_val);
        auto sorted_it = sorted_val_range.sorted_begin;
        os << *sorted_it;
        for (++sorted_it;
            sorted_it != sorted_val_range.sorted_end;
            ++sorted_it)
        {
          const Scalar_T& scr = sorted_it->second;
          if (scr >= 0.0)
            os << '+';
          os << *sorted_it;
        }
      }
    }
    return os;
  }

  /// Write term to output
  template< typename Scalar_T, const index_t LO, const index_t HI >
  auto
  operator<< (std::ostream& os, const std::pair< const index_set<LO,HI>, Scalar_T >& term) -> std::ostream&
  {
    const auto second_as_double = numeric_traits<Scalar_T>::to_double(term.second);
    const auto use_double =
      (os.precision() <= std::numeric_limits<double>::digits10) ||
      (term.second == Scalar_T(second_as_double));
    if (term.first.count() == 0)
      if (use_double)
        os << second_as_double;
      else
        os << term.second;
    else if (term.second == Scalar_T(-1))
    {
      os << '-';
      os << term.first;
    }
    else if (term.second != Scalar_T(1))
    {
      if (use_double)
      {
        auto tol = std::pow(10.0,-os.precision());
        if ( std::fabs(second_as_double + 1.0) < tol )
          os << '-';
        else if ( std::fabs(second_as_double - 1.0) >= tol )
          os << second_as_double;
      }
      else
        os << term.second;
      os << term.first;
    }
    else
      os << term.first;
    return os;
  }

  /// Read multivector from input
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  auto
  operator>> (std::istream& s, framed_multi<Scalar_T,LO,HI,Tune_P> & val) -> std::istream&
  { // Input looks like 1.0-2.0{1,2}+3.2{3,4}.
    using multivector_t = framed_multi<Scalar_T,LO,HI,Tune_P>;
    // Parsing variables.
    auto local_val = multivector_t();
    auto c = 0;
    // Parsing control variables.
    auto negative = false;
    auto expect_term = true;
    // The multivector may begin with '+' or '-'. Check for this.
    c = s.peek();
    if (s.good() && (c == int('+') || c == int('-')))
    { // A '-' here negates the following term.
      negative = (c == int('-'));
      // Consume the '+' or '-'.
      s.get();
    }
    while (s.good())
    { // Parse a term.
      // A term consists of an optional scalar, followed by an optional index set.
      // At least one of the two must be present.
      // Default coordinate is Scalar_T(1).
      auto coordinate = Scalar_T(1);
      // Default index set is empty.
      auto ist = index_set<LO,HI>();
      // First, check for an opening brace.
      c = s.peek();
      if (s.good())
      { // If the character is not an opening brace,
        // a coordinate value is expected here.
        if (c != int('{'))
        { // Try to read a coordinate value.
          double coordinate_as_double;
          s >> coordinate_as_double;
          // Reading the coordinate may have resulted in an end of file condition.
          // This is not a failure.
          if (s)
            coordinate = Scalar_T(coordinate_as_double);
        }
      }
      else
      { // End of file here ends parsing while a term may still be expected.
        break;
      }
      // Coordinate is now Scalar_T(1) or a Scalar_T value.
      // Parse an optional index set.
      if (s.good())
      {
        c = s.peek();
        if (s.good() && c == int('{'))
        { // Try to read index set.
          s >> ist;
        }
      }
      // Reading the term may have resulted in an end of file condition.
      // This is not a failure.
      if (s)
      {
        // Immediately after parsing a term, another term is not expected.
        expect_term = false;
        if (coordinate != Scalar_T(0))
        {
          // Add the term to the local multivector.
          coordinate =
            negative
            ? -coordinate
            :  coordinate;
          using term_t = typename multivector_t::term_t;
          local_val += term_t(ist, coordinate);
        }
      }
      // Check if anything follows the current term.
      if (s.good())
      {
        c = s.peek();
        if (s.good())
        { // Only '+' and '-' are valid here.
          if (c == int('+') || c == int('-'))
          { // A '-' here negates the following term.
            negative = (c == int('-'));
            // Consume the '+' or '-'.
            s.get();
            // Immediately after '+' or '-',
            // expect another term.
            expect_term = true;
          }
          else
          { // Any other character here is a not failure,
            // but still ends the parsing of the multivector.
            break;
          }
        }
      }
    }
    // If a term is still expected, this is a failure.
    if (expect_term)
      s.clear(std::istream::failbit);
    // End of file is not a failure.
    if (s)
    { // The multivector has been successfully parsed.
      val = local_val;
    }
    return s;
  }

  /// Number of terms
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  auto
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  nbr_terms () const -> unsigned long
  { return this->size(); }

  /// Insert a term into a multivector, add terms with same index set.
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  inline
  auto
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  operator+= (const term_t& term) -> multivector_t&
  { // Do not insert terms with 0 coordinate
    if (term.second != Scalar_T(0))
    {
      const auto& this_it = this->find(term.first);
      if (this_it == this->end())
        this->insert(term);
      else if (this_it->second + term.second == Scalar_T(0))
        // Erase term if resulting coordinate is 0
        this->erase(this_it);
      else
        this_it->second += term.second;
    }
    return *this;
  }

  /// Check if a multivector contains any infinite values
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  auto
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  isinf() const -> bool
  {
    using traits_t = numeric_traits<Scalar_T>;

    if (std::numeric_limits<Scalar_T>::has_infinity)
      for (auto& this_term : *this)
        if (traits_t::isInf(this_term.second))
          return true;
    return false;
  }

  /// Check if a multivector contains any IEEE NaN values
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  auto
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  isnan() const -> bool
  {
    using traits_t = numeric_traits<Scalar_T>;

    if (std::numeric_limits<Scalar_T>::has_quiet_NaN)
      for (auto& this_term : *this)
        if (traits_t::isNaN(this_term.second))
          return true;
    return false;
  }

  /// Remove all terms with relative size smaller than limit
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  auto
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  truncated(const Scalar_T& limit) const -> const multivector_t
  {
    using traits_t = numeric_traits<Scalar_T>;

    if (this->isnan() || this->isinf())
      return *this;
    const auto truncation = traits_t::abs(limit);
    const auto top = max_abs();
    auto result = multivector_t();
    if (top != Scalar_T(0))
      for (auto& this_term : *this)
        if (traits_t::abs(this_term.second) > top * truncation)
          result.insert(this_term);
    return result;
  }

  /// Subalgebra isomorphism: fold each term within the given frame
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  auto
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  fold(const index_set_t frm) const -> multivector_t
  {
    if (frm.is_contiguous())
      return *this;
    else
    {
      auto result = multivector_t();
      for (auto& this_term : *this)
        result.insert(term_t(this_term.first.fold(frm), this_term.second));
      return result;
    }
  }

  /// Subalgebra isomorphism: unfold each term within the given frame
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  auto
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  unfold(const index_set_t frm) const -> multivector_t
  {
    if (frm.is_contiguous())
      return *this;
    else
    {
      auto result = multivector_t();
      for (auto& this_term : *this)
        result.insert(term_t(this_term.first.unfold(frm), this_term.second));
      return result;
    }
  }

  /// Subalgebra isomorphism: R_{p,q} to R_{p-4,q+4}
  // Reference: [L] 16.4 Periodicity of 8, p216
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  auto
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  centre_pm4_qp4(index_t& p, index_t& q) -> multivector_t&
  {
    // We add 4 to q by subtracting 4 from p
    if (q+4 > -LO)
      throw error_t("centre_pm4_qp4(p,q): LO is too high to represent this value");
    if (this->frame().max() > p-4)
    {
      using index_pair_t = typename index_set_t::index_pair_t;
      const auto pm3210 = index_set_t(index_pair_t(p-3,p), true);
      const auto qm4321 = index_set_t(index_pair_t(-q-4,-q-1), true);
      const auto& tqm4321 = term_t(qm4321, Scalar_T(1));
      auto result = multivector_t();
      for (auto& this_term : *this)
      {
        const auto ist = this_term.first;
        if (ist.max() > p-4)
        {
          auto var_term = var_term_t();
          for (auto
              n = index_t(0);
              n != index_t(4);
              ++n)
            if (ist[n+p-3])
              var_term *= term_t(index_set_t(n-q-4), Scalar_T(1)) * tqm4321;
          // Mask out {p-3}..{p}
          result.insert(term_t(ist & ~pm3210, this_term.second) *
                        term_t(var_term.first, var_term.second));
        }
        else
          result.insert(this_term);
      }
      *this = result;
    }
    p -=4; q += 4;
    return *this;
  }

  /// Subalgebra isomorphism: R_{p,q} to R_{p+4,q-4}
  // Reference: [L] 16.4 Periodicity of 8, p216
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  auto
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  centre_pp4_qm4(index_t& p, index_t& q) -> multivector_t&
  {
    // We add 4 to p by subtracting 4 from q
    if (p+4 > HI)
      throw error_t("centre_pp4_qm4(p,q): HI is too low to represent this value");
    if (this->frame().min() < -q+4)
    {
      using index_pair_t = typename index_set_t::index_pair_t;
      const auto qp0123 = index_set_t(index_pair_t(-q,-q+3), true);
      const auto pp1234 = index_set_t(index_pair_t(p+1,p+4), true);
      const auto& tpp1234 = term_t(pp1234, Scalar_T(1));
      auto result = multivector_t();
      for (auto& this_term : *this)
      {
        index_set_t ist = this_term.first;
        if (ist.min() < -q+4)
        {
          auto var_term = var_term_t();
          for (auto
              n = index_t(0);
              n != index_t(4);
              ++n)
            if (ist[n-q])
              var_term *= term_t(index_set_t(n+p+1), Scalar_T(1)) * tpp1234;
          // Mask out {-q}..{-q+3}
          result.insert(term_t(var_term.first, var_term.second) *
                        term_t(ist & ~qp0123, this_term.second));
        }
        else
          result.insert(this_term);
      }
      *this = result;
    }
    p +=4; q -= 4;
    return *this;
  }

  /// Subalgebra isomorphism: R_{p,q} to R_{q+1,p-1}
  // Reference: [P] Proposition 15.20, p 131
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  auto
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  centre_qp1_pm1(index_t& p, index_t& q) -> multivector_t&
  {
    if (q+1 > HI)
      throw error_t("centre_qp1_pm1(p,q): HI is too low to represent this value");
    if (p-1 > -LO)
      throw error_t("centre_qp1_pm1(p,q): LO is too high to represent this value");
    const auto qp1 = index_set_t(q+1);
    const auto& tqp1 = term_t(qp1, Scalar_T(1));
    auto result = multivector_t();
    for (auto& this_term : *this)
    {
      const auto ist = this_term.first;
      auto var_term = var_term_t(index_set_t(), this_term.second);
      for (auto
          n = -q;
          n != p;
          ++n)
        if (n != 0 && ist[n])
          var_term *= term_t(index_set_t(-n) | qp1, Scalar_T(1));
      if (p != 0 && ist[p])
        var_term *= tqp1;
      result.insert(term_t(var_term.first, var_term.second));
    }
    index_t orig_p = p;
    p = q+1;
    q = orig_p-1;
    return *this = result;
  }

  /// Divide multivector into quotient with terms divisible by index set, and remainder
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  auto
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  divide(const index_set_t ist) const -> const framed_pair_t
  {
    auto quo = multivector_t();
    auto rem = multivector_t();
    for (auto& this_term : *this)
      if ((this_term.first | ist) == this_term.first)
        quo.insert(term_t(this_term.first ^ ist, this_term.second));
      else
        rem.insert(this_term);
    return framed_pair_t(quo, rem);
  }

  /// Generalized FFT from multivector_t to matrix_t
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  auto
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  fast(const index_t level, const bool odd) const -> const matrix_t
  {
    // Assume val is already folded and centred
    if (this->empty())
    {
      using matrix_index_t = typename matrix_multi_t::matrix_index_t;
      const auto dim = matrix_index_t(1) << level;
      auto result =matrix_t(dim, dim);
      result.clear();
      return result;
    }
    if (level == 0)
      return matrix::unit<matrix_t>(1) * this->scalar();

    using basis_matrix_t = typename matrix_multi_t::basis_matrix_t;
    using basis_scalar_t = typename basis_matrix_t::value_type;

    const auto&  I = matrix::unit<basis_matrix_t>(2);
    auto J = basis_matrix_t(2,2,2);
    J.clear();
    J(0,1)  = basis_scalar_t(-1);
    J(1,0)  = basis_scalar_t( 1);
    auto K = J;
    K(0,1)  = basis_scalar_t( 1);
    auto JK = I;
    JK(0,0) = basis_scalar_t(-1);

    const auto ist_mn = index_set_t(-level);
    const auto ist_pn = index_set_t(level);
    if (level == 1)
    {
      if (odd)
        return matrix_t(J) * (*this)[ist_mn] + matrix_t(K)  * (*this)[ist_pn];
      else
        return matrix_t(I) * this->scalar()  + matrix_t(JK) * (*this)[ist_mn ^ ist_pn];
    }
    else
    {
      const auto& pair_mn = this->divide(ist_mn);
      const auto& quo_mn = pair_mn.first;
      const auto& rem_mn = pair_mn.second;
      const auto& pair_quo_mnpn = quo_mn.divide(ist_pn);
      const auto& val_mnpn = pair_quo_mnpn.first;
      const auto& val_mn   = pair_quo_mnpn.second;
      const auto& pair_rem_mnpn = rem_mn.divide(ist_pn);
      const auto& val_pn   = pair_rem_mnpn.first;
      const auto& val_1    = pair_rem_mnpn.second;
      using matrix::kron;
      if (odd)
        return - kron(JK, val_1.fast   (level-1, 1))
               + kron(I,  val_mnpn.fast(level-1, 1))
               + kron(J,  val_mn.fast  (level-1, 0))
               + kron(K,  val_pn.fast  (level-1, 0));
      else
        return   kron(I,  val_1.fast   (level-1, 0))
               + kron(JK, val_mnpn.fast(level-1, 0))
               + kron(K,  val_mn.fast  (level-1, 1))
               - kron(J,  val_pn.fast  (level-1, 1));
    }
  }

  /// Use generalized FFT to construct a matrix_multi_t
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  template< typename Other_Scalar_T >
  auto
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  fast_matrix_multi(const index_set_t frm) const -> const matrix_multi<Other_Scalar_T,LO,HI,Tune_P>
  {
    // Fold val
    auto val = this->fold(frm);
    auto p = frm.count_pos();
    auto q = frm.count_neg();
    const auto bott_offset = gen::offset_to_super[pos_mod(p - q, 8)];
    p += std::max(bott_offset,index_t(0));
    q -= std::min(bott_offset,index_t(0));
    if (p > HI)
      throw error_t("fast_matrix_multi(frm): HI is too low to represent this value");
    if (q > -LO)
      throw error_t("fast_matrix_multi(frm): LO is too high to represent this value");
    // Centre val
    while (p - q > 4)
      val.centre_pm4_qp4(p, q);
    while (p - q < -3)
      val.centre_pp4_qm4(p, q);
    if (p - q > 1)
      val.centre_qp1_pm1(p, q);
    const index_t level = (p + q)/2;

    // Do the fast transform
    const auto& ev_val = val.even();
    const auto& od_val = val.odd();
    return matrix_multi<Other_Scalar_T,LO,HI,Tune_P>(ev_val.fast(level, 0) + od_val.fast(level, 1), frm);
  }

  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  inline
  auto
  framed_multi<Scalar_T,LO,HI,Tune_P>::
  fast_framed_multi() const -> const multivector_t
  { return *this; }

  /// Coordinate of product of terms
  template< typename Scalar_T, const index_t LO, const index_t HI >
  inline
  static
  auto
  crd_of_mult(const std::pair<const index_set<LO,HI>, Scalar_T>& lhs,
              const std::pair<const index_set<LO,HI>, Scalar_T>& rhs) -> Scalar_T
  { return lhs.first.sign_of_mult(rhs.first) * lhs.second * rhs.second; }

  /// Product of terms
  template< typename Scalar_T, const index_t LO, const index_t HI >
  inline
  auto
  operator* (const std::pair<const index_set<LO,HI>, Scalar_T>& lhs,
             const std::pair<const index_set<LO,HI>, Scalar_T>& rhs) -> const std::pair<const index_set<LO,HI>, Scalar_T>
  {
    using term_t = std::pair<const index_set<LO,HI>, Scalar_T>;
    return term_t(lhs.first ^ rhs.first, crd_of_mult(lhs, rhs));
  }

  /// Square root of multivector with specified complexifier
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  auto
  sqrt(const framed_multi<Scalar_T,LO,HI,Tune_P>& val, const framed_multi<Scalar_T,LO,HI,Tune_P>& i, bool prechecked) -> const framed_multi<Scalar_T,LO,HI,Tune_P>
  {
    using traits_t = numeric_traits<Scalar_T>;
    if (val.isnan())
      return traits_t::NaN();

    check_complex(val, i, prechecked);

    const auto realval = val.scalar();
    if (val == realval)
    {
      if (realval < Scalar_T(0))
        return i * traits_t::sqrt(-realval);
      else
        return traits_t::sqrt(realval);
    }
    using matrix_multi_t = typename framed_multi<Scalar_T,LO,HI,Tune_P>::matrix_multi_t;
    return sqrt(matrix_multi_t(val), matrix_multi_t(i), prechecked);
  }

  /// Exponential of multivector
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  auto
  exp(const framed_multi<Scalar_T,LO,HI,Tune_P>& val) -> const framed_multi<Scalar_T,LO,HI,Tune_P>
  {
    using traits_t = numeric_traits<Scalar_T>;
    if (val.isnan())
      return traits_t::NaN();

    const auto s = scalar(val);
    if (val == s)
      return traits_t::exp(s);

    const double size = val.size();
    const auto frm_count = val.frame().count();
    const auto algebra_dim = set_value_t(1) << frm_count;

    if( (size * size <= double(algebra_dim)) || (frm_count < Tune_P::mult_matrix_threshold))
    {
      switch (Tune_P::function_precision)
      {
      case precision_demoted:
        {
          using demoted_scalar_t = typename traits_t::demoted::type;
          using demoted_multivector_t = framed_multi<demoted_scalar_t,LO,HI,Tune_P>;

          const auto& demoted_val = demoted_multivector_t(val);
          return clifford_exp(demoted_val);
        }
        break;
      case precision_promoted:
        {
          using promoted_scalar_t = typename traits_t::promoted::type;
          using promoted_multivector_t = framed_multi<promoted_scalar_t,LO,HI,Tune_P>;

          const auto& promoted_val = promoted_multivector_t(val);
          return clifford_exp(promoted_val);
        }
        break;
      default:
        return clifford_exp(val);
      }
    }
    else
    {
      using matrix_multi_t = matrix_multi<Scalar_T,LO,HI,Tune_P>;
      return exp(matrix_multi_t(val));
    }
  }

  /// Natural logarithm of multivector with specified complexifier
  template< typename Scalar_T, const index_t LO, const index_t HI, typename Tune_P >
  auto
  log(const framed_multi<Scalar_T,LO,HI,Tune_P>& val, const framed_multi<Scalar_T,LO,HI,Tune_P>& i, bool prechecked) -> const framed_multi<Scalar_T,LO,HI,Tune_P>
  {
    using traits_t = numeric_traits<Scalar_T>;
    if (val == Scalar_T(0) || val.isnan())
      return traits_t::NaN();

    check_complex(val, i, prechecked);

    const auto realval = val.scalar();
    if (val == realval)
    {
      if (realval < Scalar_T(0))
        return i * traits_t::pi() + traits_t::log(-realval);
      else
        return traits_t::log(realval);
    }
    using matrix_multi_t = typename framed_multi<Scalar_T,LO,HI,Tune_P>::matrix_multi_t;
    return log(matrix_multi_t(val), matrix_multi_t(i), prechecked);
  }
}
#endif  // _GLUCAT_FRAMED_MULTI_IMP_H
