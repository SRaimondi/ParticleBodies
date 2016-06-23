/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   vmath.hpp
 * Author: Simon
 *
 * Created on June 23, 2016, 5:07 PM
 */

#ifndef VMATH_HPP
#define VMATH_HPP

// Define to include the math constants
#define _USE_MATH_DEFINES
#include <math.h>

// Define PI constant
#define M_PI    3.14159265358979323846

namespace pb {
    namespace vmath {

        // Math class declaration
    //    template <typename T, size_t const w, size_t const h> class Matrix;
        template <typename T, size_t const len> class Vector;
        template <typename T> class Quaternion;

        // Convert radians to degree
        template <typename T>
        inline T degrees(T const & radians) {
            return (radians * static_cast<T>(180.0 / M_PI));
        }

        // Convert degree to radians
        template <typename T>
        inline T radians(T const & degrees) {
            return (degrees * static_cast<T>(M_PI / 180.0));
        }

        // Random number generator
        template <typename T>
        struct RandomNumber {

            T operator()(void) {
                static unsigned int seed = 0x13371337;
                unsigned int res;
                unsigned int tmp;

                seed *= 16807;

                tmp = seed ^ (seed >> 4) ^ (seed << 15);

                res = (tmp >> 9) | 0x3F800000;

                return static_cast<T>(res);
            }
        };

        // Float random number generator
        template<>
        struct RandomNumber<float> {

            float operator()(void) {
                static unsigned int seed = 0x13371337;
                float res;
                unsigned int tmp;

                seed *= 16807;

                tmp = seed ^ (seed >> 4) ^ (seed << 15);

                *((unsigned int *) &res) = (tmp >> 9) | 0x3F800000;

                return (res - 1.0f);
            }
        };

        // Unsigned integer random number generator
        template<>
        struct RandomNumber<unsigned int> {

            unsigned int operator()(void) {
                static unsigned int seed = 0x13371337;
                unsigned int res;
                unsigned int tmp;

                seed *= 16807;

                tmp = seed ^ (seed >> 4) ^ (seed << 15);

                res = (tmp >> 9) | 0x3F800000;

                return res;
            }
        };

        // Vector class
        template <typename T, size_t const len>
        class Vector {
        public:
            typedef class Vector<T, len> my_type;
            typedef T element_type;

            // Default constructor does nothing, just like built-in types
            inline Vector() {
                // Uninitialized variable
            }

            // Copy constructor
            inline Vector(Vector const & that) {
                assign(that);
            }

            // Construction from scalar
            inline Vector(T const & s) {
                size_t n;
                for (n = 0; n < len; n++) {
                    data[n] = s;
                }
            }

            // Assignment operator
            inline Vector & operator=(Vector const & that) {
                assign(that);

                return (*this);
            }

            inline Vector & operator=(T const & that) {
                size_t n;
                for (n = 0; n < len; n++) {
                    data[n] = that;
                }

                return (*this);
            }

            inline Vector operator+(Vector const & that) const {
                my_type result;
                size_t n;
                for (n = 0; n < len; n++) {
                    result.data[n] = data[n] + that.data[n];
                }

                return (result);
            }

            inline Vector & operator+=(Vector const & that) {
                return (*this = *this + that);
            }

            inline Vector operator-() const {
                my_type result;
                size_t n;
                for (n = 0; n < len; n++) {
                    result.data[n] = -data[n];
                }

                return (result);
            }

            inline Vector operator-(Vector const & that) const {
                my_type result;
                size_t n;
                for (n = 0; n < len; n++) {
                    result.data[n] = data[n] - that.data[n];
                }

                return (result);
            }

            inline Vector & operator-=(Vector const & that) {
                return (*this = *this - that);
            }

            inline Vector operator*(Vector const & that) const {
                my_type result;
                size_t n;
                for (n = 0; n < len; n++) {
                    result.data[n] = data[n] * that.data[n];
                }

                return (result);
            }

            inline Vector & operator*=(Vector const & that) {
                return (*this = *this * that);
            }

            inline Vector operator*(T const & that) const {
                my_type result;
                size_t n;
                for (n = 0; n < len; n++) {
                    result.data[n] = data[n] * that;
                }

                return (result);
            }

            inline Vector & operator*=(T const & that) {
                assign(*this * that);

                return *this;
            }

            inline Vector operator/(Vector const & that) const {
                my_type result;
                size_t n;
                for (n = 0; n < len; n++) {
                    result.data[n] = data[n] / that.data[n];
                }

                return (result);
            }

            inline Vector & operator/=(Vector const & that) {
                assign(*this / that);

                return (*this);
            }

            inline Vector operator/(T const & that) const {
                my_type result;
                size_t n;
                for (n = 0; n < len; n++) {
                    result.data[n] = data[n] / that;
                }

                return (result);
            }

            inline Vector & operator/=(T const & that) {
                assign(*this / that);

                return (*this);
            }

            inline T & operator[](size_t const n) {
                return (data[n]);
            }

            inline T const & operator[](size_t const n) const {
                return (data[n]);
            }

            inline static int size(void) {
                return (len);
            }

            inline const T * operator()(void) const {
                return (&data[0]);
            }

            // Create random vector
            static inline Vector random() {
                Vector result;
                size_t i;

                for (i = 0; i < len; i++) {
                    result[i] = vmath::RandomNumber<T>();
                }

                return (result);
            }

        protected:
            // Vector data
            T data[len];

            inline void assign(Vector const & that) {
                size_t n;
                for (n = 0; n < len; n++) {
                    data[n] = that.data[n];
                }
            }
        };

        // Vector2 class
        template <typename T>
        class Vector2 : public Vector<T, 2> {
        public:
            typedef Vector<T, 2> base;

            // Uninitialized variable
            inline Vector2() {}
            // Copy constructor
            inline Vector2(base const & v) : base(v) {}

            // Vector2(x, y);
            inline Vector2(T const & x, T const & y) {
                base::data[0] = x;
                base::data[1] = y;
            }
        };

        // Vector 3 class
        template <typename T>
        class Vector3 : public Vector<T, 3> {
        public:
            typedef Vector<T, 3> base;

            // Uninitialized variable
            inline Vector3() {}

            // Copy constructor
            inline Vector3(base const & v) : base(v) {}

            // Vector3(x, y, z);
            inline Vector3(T const & x, T const & y, T const & z) {
                base::data[0] = x;
                base::data[1] = y;
                base::data[2] = z;
            }

            // Vector3(v, z);
            inline Vector3(Vector2<T> const & v, T z) {
                base::data[0] = v[0];
                base::data[1] = v[1];
                base::data[2] = z;
            }

            // Vector(x, v)
            inline Vector3(T const & x, Vector2<T> const & v) {
                base::data[0] = x;
                base::data[1] = v[0];
                base::data[2] = v[1];
            }
        };

        // Vector4 class
        template <typename T>
        class Vector4 : public Vector<T, 4> {
        public:
            typedef Vector<T, 4> base;

            // Uninitialized variable
            inline Vector4() {}

            // Copy constructor
            inline Vector4(base const & v) : base(v) {}

            // Vector4(x, y, z, w);
            inline Vector4(T const & x, T const & y, T const & z, T const & w) {
                base::data[0] = x;
                base::data[1] = y;
                base::data[2] = z;
                base::data[3] = w;
            }

            // Vector4(v, z, w);
            inline Vector4(Vector2<T> const & v, T const & z, T const & w) {
                base::data[0] = v[0];
                base::data[1] = v[1];
                base::data[2] = z;
                base::data[3] = w;
            }

            // Vector4(x, v, w);
            inline Vector4(T const & x, Vector2<T> const & v, T const & w) {
                base::data[0] = x;
                base::data[1] = v[0];
                base::data[2] = v[1];
                base::data[3] = w;
            }

            // Vector4(x, y, v);
            inline Vector4(T const & x, T const & y, Vector2<T> const & v) {
                base::data[0] = x;
                base::data[1] = y;
                base::data[2] = v[0];
                base::data[3] = v[1];
            }

            // Vector4(v1, v2);
            inline Vector4(Vector2<T> const & u, Vector2<T> const & v) {
                base::data[0] = u[0];
                base::data[1] = u[1];
                base::data[2] = v[0];
                base::data[3] = v[1];
            }

            // Vector4(v, w);
            inline Vector4(Vector3<T> const & v, T const & w) {
                base::data[0] = v[0];
                base::data[1] = v[1];
                base::data[2] = v[2];
                base::data[3] = w;
            }

            // Vector4(x, v);
            inline Vector4(T const & x, Vector3<T> const & v) {
                base::data[0] = x;
                base::data[1] = v[0];
                base::data[2] = v[1];
                base::data[3] = v[2];
            }
        };

        // Define some use full types
        typedef Vector<float, 1> vec1;
        typedef Vector<int, 1> ivec1;
        typedef Vector<unsigned int, 1> uvec1;
        typedef Vector<double, 1> dvec1;

        typedef Vector2<float> vec2;
        typedef Vector2<int> ivec2;
        typedef Vector2<unsigned int> uvec2;
        typedef Vector2<double> dvec2;

        typedef Vector3<float> vec3;
        typedef Vector3<int> ivec3;
        typedef Vector3<unsigned int> uvec3;
        typedef Vector3<double> dvec3;

        typedef Vector4<float> vec4;
        typedef Vector4<int> ivec4;
        typedef Vector4<unsigned int> uvec4;
        typedef Vector4<double> dvec4;
        
        // Friend operators
        template <typename T, size_t const n>
        static inline const Vector<T, n> operator*(T const & x, Vector<T, n> const & v) {
            return (v * x);
        }

        template <typename T>
        static inline const Vector2<T> operator/(T const & x, Vector2<T> const & v) {
            return (Vector2<T>(x / v[0], x / v[1]));
        }

        template <typename T>
        static inline const Vector3<T> operator/(T const & x, Vector3<T> const & v) {
            return (Vector3<T>(x / v[0], x / v[1], x / v[2]));
        }

        template <typename T>
        static inline const Vector4<T> operator/(T const & x, Vector4<T> const & v) {
            return (Vector4<T>(x / v[0], x / v[1], x / v[2], x / v[3]));
        }

        template <typename T, size_t const len>
        static inline T dot(Vector<T, len> const & a, Vector<T, len> const & b) {
            size_t n;
            T total = T(0);
            for (n = 0; n < len; n++) {
                total += a[n] * b[n];
            }
            
            return (total);
        }

        template <typename T>
        static inline Vector<T, 3> cross(Vector<T, 3> const & a, Vector<T, 3> const & b) {
            return (Vector3<T>( a[1] * b[2] - b[1] * a[2],
                                a[2] * b[0] - b[2] * a[0],
                                a[0] * b[1] - b[0] * a[1]));
        }

        template <typename T, size_t const len>
        static inline T squaredLength(Vector<T, len> const & v) {
            T result(0);

            for (int i = 0; i < v.size(); ++i) {
                result += v[i] * v[i];
            }

            return (result);
        }
        
        template <typename T, size_t const len>
        static inline T length(Vector<T, len> const & v) {
            return (static_cast<T>(sqrt(squaredLength(v))));
        }

        template <typename T, size_t const len>
        static inline Vector<T, len> normalize(Vector<T, len> const & v) {
            return (v / length(v));
        }

        template <typename T, size_t const len>
        static inline T distance(Vector<T, len> const & a, Vector<T, len> const & b) {
            return (length(b - a));
        }

        template <typename T, size_t const len>
        static inline T angle(Vector<T, len> const & a, Vector<T, len> const & b) {
            return (arccos(dot(a, b)));
        }
        
        // Quaternion class
        template <typename T>
        class Quaternion {
        public:
            // Empty constructor
            inline Quaternion() {}

            inline Quaternion(Quaternion const & q)
            : r(q.r), v(q.v) {}

            // Quaternion form real value
            inline Quaternion(T const & real)
            : r(real), v(T(0)) {}

            inline Quaternion(T const & real, Vector3<T> const & axis)
            : r(real), v(axis) {}

            inline Quaternion(Vector4<T> const & v)
            : r(v[0]), v(v[1], v[2], v[3]) {}

            inline Quaternion(T const & real, T const & x, T  const & y, T const & z)
            : r(real), v(x. y. z) {}

            inline T & operator[](size_t const n) {
                return (a[n]);
            }

            inline T const & operator[](size_t const n) const {
                return (a[n]);
            }

            inline Quaternion operator+(Quaternion const & q) const {
                return (Quaternion(r + q.r, v + q.v));
            }

            inline Quaternion & operator+=(Quaternion const & q) {
                r += q.r;
                v += q.v;

                return (*this);
            }

            inline Quaternion operator-(Quaternion const & q) const {
                return Quaternion(r - q.r, v - q.v);
            }

            inline Quaternion & operator-=(Quaternion const & q) {
                r -= q.r;
                v -= q.v;

                return (*this);
            }

            inline Quaternion operator-() const {
                return Quaternion(-r, -v);
            }

            inline Quaternion operator*(T const & s) const {
                return (Quaternion(a[0] * s, a[1] * s, a[2] * s, a[3] * s));
            }

            inline Quaternion & operator*=(T const & s) {
                r *= s;
                v *= s;

                return (*this);
            }

            inline Quaternion operator*(Quaternion const & q) const {
                return (Quaternion(r * q.r - vmath::dot(v, q.v), r * q.v + q.r * v + vmath::cross(v, q.v)));
            }

            inline Quaternion operator/(T const & s) const {
                return (Quaternion(a[0] / s, a[1] / s, a[2] / s, a[3] / s));
            }

            inline Quaternion & operator/=(T const & s) {
                r /= s;
                v /= s;

                return (*this);
            }

            inline Vector4<T> & operator()(void) {
                return (*(Vector4<T>*)&a[0]);
            }

            inline Vector4<T> const & operator()(void) const {
                return (*(Vector4<T> const *)&a[0]);
            }

            inline bool operator==(Quaternion const & q) const {
                return (r == q.r) && (v == q.v);
            }

            inline bool operator!=(Quaternion const & q) const {
                return (r != q.r) || (v != q.v);
            }

//            inline matNM<T,4,4> asMatrix() const
//            {
//                matNM<T,4,4> m;
//
//                const T xx = x * x;
//                const T yy = y * y;
//                const T zz = z * z;
//                const T ww = w * w;
//                const T xy = x * y;
//                const T xz = x * z;
//                const T xw = x * w;
//                const T yz = y * z;
//                const T yw = y * w;
//                const T zw = z * w;
//
//                m[0][0] = T(1) - T(2) * (yy + zz);
//                m[0][1] =        T(2) * (xy - zw);
//                m[0][2] =        T(2) * (xz + yw);
//                m[0][3] =        T(0);
//
//                m[1][0] =        T(2) * (xy + zw);
//                m[1][1] = T(1) - T(2) * (xx + zz);
//                m[1][2] =        T(2) * (yz - xw);
//                m[1][3] =        T(0);
//
//                m[2][0] =        T(2) * (xz - yw);
//                m[2][1] =        T(2) * (yz + xw);
//                m[2][2] = T(1) - T(2) * (xx + yy);
//                m[2][3] =        T(0);
//
//                m[3][0] =        T(0);
//                m[3][1] =        T(0);
//                m[3][2] =        T(0);
//                m[3][3] =        T(1);
//
//                return m;
//            }

        private:
            // Quaternion data
            union {
                struct {
                    T           r;
                    Vector3<T>  v;
                };
                struct {
                    T           x;
                    T           y;
                    T           z;
                    T           w;
                };
                T               a[4];
            };
        };

        // Define quaternion types
        typedef Quaternion<float> quaternion;
        typedef Quaternion<int> iquaternion;
        typedef Quaternion<unsigned int> uquaternion;
        typedef Quaternion<double> dquaternion;

        template <typename T>
        static inline Quaternion<T> operator*(T const & a, Quaternion<T> const & b) {
            return (b * a);
        }

        template <typename T>
        static inline Quaternion<T> operator/(T const & a, Quaternion<T> const & b) {
            return (Quaternion<T>(a / b[0], a / b[1], a / b[2], a / b[3]));
        }

        template <typename T>
        static inline Quaternion<T> normalize(Quaternion<T> const & q) {
            return (q / length(Vector<T, 4>(q)));
        }
        
        template <typename T>
        static inline Quaternion<T> angleAxis(T const & angle, Vector<T, 3> const & axis) {
            return (Quaternion<T>(cos(radians(angle) / 2.0), sin(radians(angle) / 2.0) * normalize(axis)));
        }


    } // vmath namespace
} // pb namespace

#endif /* MATH_HPP */

