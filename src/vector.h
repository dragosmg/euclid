#pragma once

#include <cpp11/strings.hpp>
#include <cpp11/doubles.hpp>
#include "cgal_types.h"
#include "geometry_vector.h"
#include "exact_numeric.h"
#include "intersection.h"
#include "distance.h"

class vector2 : public geometry_vector<Vector_2, 2> {
public:
  using geometry_vector::geometry_vector;
  ~vector2() = default;

  cpp11::writable::strings def_names() const {
    return {"x", "y"};
  }

  Primitive geometry_type() const { return VECTOR; }

  Exact_number get_single_definition(size_t i, int which, int element) const {
    switch(which) {
    case 0: return _storage[i].x();
    case 1: return _storage[i].y();
    }
    return _storage[i].x();
  }

  std::vector<double> get_row(size_t i, size_t j) const {
    return {
      CGAL::to_double(_storage[i].x().exact()),
      CGAL::to_double(_storage[i].y().exact())
    };
  }

  cpp11::writable::list intersection(const geometry_vector_base& other) const {
    cpp11::stop("Don't know how to calculate the intersection of these geometries");
  }

  cpp11::writable::logicals do_intersect(const geometry_vector_base& other) const {
    if (other.dimensions() != dimensions()) {
      cpp11::stop("Only geometries of the same dimensionality can intersect");
    }
    return unknown_intersect_impl(std::max(size(), other.size()));
  }

  std::vector<Exact_number> squared_distance(const geometry_vector_base& other) const {
    if (other.dimensions() != dimensions()) {
      cpp11::stop("Only geometries of the same dimensionality can intersect");
    }
    return unknown_squared_distance_impl(std::max(size(), other.size()));
  }

  cpp11::writable::doubles_matrix distance_matrix(const geometry_vector_base& other) const {
    if (other.dimensions() != dimensions()) {
      cpp11::stop("Only geometries of the same dimensionality can intersect");
    }
    return unknown_distance_matrix_impl(size(), other.size());
  }

  std::vector<Vector_2> operator+(const std::vector<Vector_2>& other) const {
    size_t final_size = std::max(size(), other.size());
    std::vector<Vector_2> result;
    if (size() == 0 || other.size() == 0) {
      return result;
    }
    result.reserve(final_size);
    for (size_t i = 0; i < final_size; ++i) {
      if (!_storage[i % size()] || !other[i % other.size()]) {
        result[i] = Vector_2::NA_value();
        continue;
      }
      result.push_back(_storage[i % size()] + other[i % other.size()]);
    }
    return result;
  }
  std::vector<Vector_2> operator-(const std::vector<Vector_2>& other) const {
    size_t final_size = std::max(size(), other.size());
    std::vector<Vector_2> result;
    if (size() == 0 || other.size() == 0) {
      return result;
    }
    result.reserve(final_size);
    for (size_t i = 0; i < final_size; ++i) {
      if (!_storage[i % size()] || !other[i % other.size()]) {
        result[i] = Vector_2::NA_value();
        continue;
      }
      result.push_back(_storage[i % size()] - other[i % other.size()]);
    }
    return result;
  }
  std::vector<Vector_2> operator-() const {
    std::vector<Vector_2> result;
    result.reserve(size());
    for (size_t i = 0; i < size(); ++i) {
      if (!_storage[i]) {
        result[i] = Vector_2::NA_value();
        continue;
      }
      result.push_back(-_storage[i]);
    }
    return result;
  }
  std::vector<Exact_number> operator*(const std::vector<Vector_2>& other) const {
    size_t final_size = std::max(size(), other.size());
    std::vector<Exact_number> result;
    if (size() == 0 || other.size() == 0) {
      return result;
    }
    result.reserve(final_size);
    for (size_t i = 0; i < final_size; ++i) {
      if (!_storage[i % size()] || !other[i % other.size()]) {
        result[i] = Exact_number::NA_value();
        continue;
      }
      result.push_back(_storage[i % size()] * other[i % other.size()]);
    }
    return result;
  }
  std::vector<Vector_2> operator*(const std::vector<Exact_number>& other) const {
    size_t final_size = std::max(size(), other.size());
    std::vector<Vector_2> result;
    if (size() == 0 || other.size() == 0) {
      return result;
    }
    result.reserve(final_size);
    for (size_t i = 0; i < final_size; ++i) {
      if (!_storage[i % size()] || !other[i % other.size()]) {
        result[i] = Vector_2::NA_value();
        continue;
      }
      result.push_back(_storage[i % size()] * other[i % other.size()]);
    }
    return result;
  }
  std::vector<Vector_2> operator/(const std::vector<Exact_number>& other) const {
    size_t final_size = std::max(size(), other.size());
    std::vector<Vector_2> result;
    if (size() == 0 || other.size() == 0) {
      return result;
    }
    result.reserve(final_size);
    for (size_t i = 0; i < final_size; ++i) {
      if (!_storage[i % size()] || !other[i % other.size()] || other[i % other.size()] == 0.0) {
        result[i] = Vector_2::NA_value();
        continue;
      }
      result.push_back(_storage[i % size()] / other[i % other.size()]);
    }
    return result;
  }
  std::vector<Vector_2> sum(bool na_rm) const {
    if (size() == 0) {
      return {};
    }
    Vector_2 total = _storage[0];

    for (size_t i = 1; i < size(); ++i) {
      if (!_storage[i]) {
        if (!na_rm) {
          total = Vector_2::NA_value();
          break;
        }
        continue;
      }
      total += _storage[i];
    }

    return {total};
  }
  std::vector<Vector_2> cumsum() const {
    std::vector<Vector_2> result;
    result.reserve(size());

    if (size() == 0) {
      return {result};
    }

    Vector_2 cum_sum = _storage[0];
    result.push_back(cum_sum);
    bool is_na = false;

    for (size_t i = 1; i < size(); ++i) {
      if (!is_na && !_storage[i]) {
        is_na = true;
        cum_sum = Vector_2::NA_value();
      }
      if (!is_na) {
        cum_sum += _storage[i];
      }
      result.push_back(cum_sum);
    }

    return {result};
  }
};

typedef cpp11::external_pointer<vector2> vector2_p;

class vector3 : public geometry_vector<Vector_3, 3, Vector_2> {
public:
  using geometry_vector::geometry_vector;
  ~vector3() = default;

  Primitive geometry_type() const { return VECTOR; }

  cpp11::writable::strings def_names() const {
    return {"x", "y", "z"};
  }

  Exact_number get_single_definition(size_t i, int which, int element) const {
    switch(which) {
    case 0: return _storage[i].x();
    case 1: return _storage[i].y();
    case 2: return _storage[i].z();
    }
    return _storage[i].x();
  }

  std::vector<double> get_row(size_t i, size_t j) const {
    return {
      CGAL::to_double(_storage[i].x().exact()),
      CGAL::to_double(_storage[i].y().exact()),
      CGAL::to_double(_storage[i].z().exact())
    };
  }

  cpp11::writable::list intersection(const geometry_vector_base& other) const {
    cpp11::stop("Don't know how to calculate the intersection of these geometries");
  }

  cpp11::writable::logicals do_intersect(const geometry_vector_base& other) const {
    if (other.dimensions() != dimensions()) {
      cpp11::stop("Only geometries of the same dimensionality can intersect");
    }
    return unknown_intersect_impl(std::max(size(), other.size()));
  }

  std::vector<Exact_number> squared_distance(const geometry_vector_base& other) const {
    if (other.dimensions() != dimensions()) {
      cpp11::stop("Only geometries of the same dimensionality can intersect");
    }
    return unknown_squared_distance_impl(std::max(size(), other.size()));
  }

  cpp11::writable::doubles_matrix distance_matrix(const geometry_vector_base& other) const {
    if (other.dimensions() != dimensions()) {
      cpp11::stop("Only geometries of the same dimensionality can intersect");
    }
    return unknown_distance_matrix_impl(size(), other.size());
  }

  std::vector<Vector_3> operator+(const std::vector<Vector_3>& other) const {
    size_t final_size = std::max(size(), other.size());
    std::vector<Vector_3> result;
    if (size() == 0 || other.size() == 0) {
      return result;
    }
    result.reserve(final_size);
    for (size_t i = 0; i < final_size; ++i) {
      if (!_storage[i % size()] || !other[i % other.size()]) {
        result[i] = Vector_3::NA_value();
        continue;
      }
      result.push_back(_storage[i % size()] + other[i % other.size()]);
    }
    return {result};
  }
  std::vector<Vector_3> operator-(const std::vector<Vector_3>& other) const {
    size_t final_size = std::max(size(), other.size());
    std::vector<Vector_3> result;
    if (size() == 0 || other.size() == 0) {
      return result;
    }
    result.reserve(final_size);
    for (size_t i = 0; i < final_size; ++i) {
      if (!_storage[i % size()] || !other[i % other.size()]) {
        result[i] = Vector_3::NA_value();
        continue;
      }
      result.push_back(_storage[i % size()] - other[i % other.size()]);
    }
    return {result};
  }
  std::vector<Vector_3> operator-() const {
    std::vector<Vector_3> result;
    result.reserve(size());
    for (size_t i = 0; i < size(); ++i) {
      if (!_storage[i]) {
        result[i] = Vector_3::NA_value();
        continue;
      }
      result.push_back(-_storage[i]);
    }
    return {result};
  }
  std::vector<Exact_number> operator*(const std::vector<Vector_3>& other) const {
    size_t final_size = std::max(size(), other.size());
    std::vector<Exact_number> result;
    if (size() == 0 || other.size() == 0) {
      return result;
    }
    result.reserve(final_size);
    for (size_t i = 0; i < final_size; ++i) {
      if (!_storage[i % size()] || !other[i % other.size()]) {
        result[i] = Exact_number::NA_value();
        continue;
      }
      result.push_back(_storage[i % size()] * other[i % other.size()]);
    }
    return {result};
  }
  std::vector<Vector_3> operator*(const std::vector<Exact_number>& other) const {
    size_t final_size = std::max(size(), (size_t) other.size());
    std::vector<Vector_3> result;
    if (size() == 0 || other.size() == 0) {
      return result;
    }
    result.reserve(final_size);
    for (size_t i = 0; i < final_size; ++i) {
      if (!_storage[i % size()] || !other[i % other.size()]) {
        result[i] = Vector_3::NA_value();
        continue;
      }
      result.push_back(_storage[i % size()] * other[i % other.size()]);
    }
    return {result};
  }
  std::vector<Vector_3> operator/(const std::vector<Exact_number>& other) const {
    size_t final_size = std::max(size(), (size_t) other.size());
    std::vector<Vector_3> result;
    if (size() == 0 || other.size() == 0) {
      return result;
    }
    result.reserve(final_size);
    for (size_t i = 0; i < final_size; ++i) {
      if (!_storage[i % size()] || !other[i % other.size()] || other[i % other.size()] == 0.0) {
        result[i] = Vector_3::NA_value();
        continue;
      }
      result.push_back(_storage[i % size()] / other[i % other.size()]);
    }
    return {result};
  }
  std::vector<Vector_3> sum(bool na_rm) const {
    if (size() == 0) {
      return {};
    }
    Vector_3 total = _storage[0];

    for (size_t i = 1; i < size(); ++i) {
      if (!_storage[i]) {
        if (!na_rm) {
          total = Vector_3::NA_value();
          break;
        }
        continue;
      }
      total += _storage[i];
    }

    return {total};
  }
  std::vector<Vector_3> cumsum() const {
    std::vector<Vector_3> result;
    result.reserve(size());

    if (size() == 0) {
      return {result};
    }

    Vector_3 cum_sum = _storage[0];
    result.push_back(cum_sum);
    bool is_na = false;

    for (size_t i = 1; i < size(); ++i) {
      if (!is_na && !_storage[i]) {
        is_na = true;
        cum_sum = Vector_3::NA_value();
      }
      if (!is_na) {
        cum_sum += _storage[i];
      }
      result.push_back(cum_sum);
    }

    return {result};
  }
};

typedef cpp11::external_pointer<vector3> vector3_p;
