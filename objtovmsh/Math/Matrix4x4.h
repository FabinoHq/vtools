////////////////////////////////////////////////////////////////////////////////
//  _______                       __________________________________________  //
//  \\ .   \            _________/ . . . . . . . . . . . . . . . . . . .   /  //
//   \\ .   \       ___/ . . . . .    ____________________________________/   //
//    \\ .   \   __/. . .   _________/  / \ // .    \ //.  ///. _______/      //
//     \\ .   \_//      ___/.  ________/  ///.      ///.  ///. /____          //
//      \\ .   \/     _/ //.  / //. /\   ///. /\   ///.  / \\ .     \         //
//       \\ .        /  //.  / //. / /  ///. / /  ///.  /   \\____   \        //
//        \\ .      /  //.  / //.  \/  ///.  \/  ///.  /________//   /        //
//         \\ .    /  //.  / // .     /// .     ///.       / .      /         //
//          \\____/  //___/  \\______/ \\______///________/________/          //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//   This is free and unencumbered software released into the public domain.  //
//                                                                            //
//   Anyone is free to copy, modify, publish, use, compile, sell, or          //
//   distribute this software, either in source code form or as a compiled    //
//   binary, for any purpose, commercial or non-commercial, and by any        //
//   means.                                                                   //
//                                                                            //
//   In jurisdictions that recognize copyright laws, the author or authors    //
//   of this software dedicate any and all copyright interest in the          //
//   software to the public domain. We make this dedication for the benefit   //
//   of the public at large and to the detriment of our heirs and             //
//   successors. We intend this dedication to be an overt act of              //
//   relinquishment in perpetuity of all present and future rights to this    //
//   software under copyright law.                                            //
//                                                                            //
//   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,          //
//   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF       //
//   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.   //
//   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR        //
//   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,    //
//   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR    //
//   OTHER DEALINGS IN THE SOFTWARE.                                          //
//                                                                            //
//   For more information, please refer to <https://unlicense.org>            //
////////////////////////////////////////////////////////////////////////////////
//    VTools : Virtual Tools                                                  //
//     Math/Matrix4x4.h : 4x4 matrix management                               //
////////////////////////////////////////////////////////////////////////////////
#ifndef VTOOLS_OBJTOVMSH_MATH_MATRIX4X4_HEADER
#define VTOOLS_OBJTOVMSH_MATH_MATRIX4X4_HEADER

    #include "../System/System.h"
    #include "Math.h"
    #include "Vector2.h"
    #include "Vector3.h"


    ////////////////////////////////////////////////////////////////////////////
    //  Matrix4x4 class definition                                            //
    ////////////////////////////////////////////////////////////////////////////
    class Matrix4x4
    {
        public:
            ////////////////////////////////////////////////////////////////////
            //  Matrix4x4 default constructor                                 //
            ////////////////////////////////////////////////////////////////////
            Matrix4x4()
            {
                mat[0] = 0.0f;
                mat[1] = 0.0f;
                mat[2] = 0.0f;
                mat[3] = 0.0f;
                mat[4] = 0.0f;
                mat[5] = 0.0f;
                mat[6] = 0.0f;
                mat[7] = 0.0f;
                mat[8] = 0.0f;
                mat[9] = 0.0f;
                mat[10] = 0.0f;
                mat[11] = 0.0f;
                mat[12] = 0.0f;
                mat[13] = 0.0f;
                mat[14] = 0.0f;
                mat[15] = 0.0f;
            }

            ////////////////////////////////////////////////////////////////////
            //  Matrix4x4 copy constructor                                    //
            ////////////////////////////////////////////////////////////////////
            Matrix4x4(const Matrix4x4& matrix)
            {
                mat[0] = matrix.mat[0];
                mat[1] = matrix.mat[1];
                mat[2] = matrix.mat[2];
                mat[3] = matrix.mat[3];
                mat[4] = matrix.mat[4];
                mat[5] = matrix.mat[5];
                mat[6] = matrix.mat[6];
                mat[7] = matrix.mat[7];
                mat[8] = matrix.mat[8];
                mat[9] = matrix.mat[9];
                mat[10] = matrix.mat[10];
                mat[11] = matrix.mat[11];
                mat[12] = matrix.mat[12];
                mat[13] = matrix.mat[13];
                mat[14] = matrix.mat[14];
                mat[15] = matrix.mat[15];
            }

            ////////////////////////////////////////////////////////////////////
            //  Matrix4x4 destructor                                          //
            ////////////////////////////////////////////////////////////////////
            ~Matrix4x4()
            {
                mat[0] = 0.0f;
                mat[1] = 0.0f;
                mat[2] = 0.0f;
                mat[3] = 0.0f;
                mat[4] = 0.0f;
                mat[5] = 0.0f;
                mat[6] = 0.0f;
                mat[7] = 0.0f;
                mat[8] = 0.0f;
                mat[9] = 0.0f;
                mat[10] = 0.0f;
                mat[11] = 0.0f;
                mat[12] = 0.0f;
                mat[13] = 0.0f;
                mat[14] = 0.0f;
                mat[15] = 0.0f;
            }


            ////////////////////////////////////////////////////////////////////
            //  Reset Matrix4x4 to zero                                       //
            ////////////////////////////////////////////////////////////////////
            void reset()
            {
                mat[0] = 0.0f;
                mat[1] = 0.0f;
                mat[2] = 0.0f;
                mat[3] = 0.0f;
                mat[4] = 0.0f;
                mat[5] = 0.0f;
                mat[6] = 0.0f;
                mat[7] = 0.0f;
                mat[8] = 0.0f;
                mat[9] = 0.0f;
                mat[10] = 0.0f;
                mat[11] = 0.0f;
                mat[12] = 0.0f;
                mat[13] = 0.0f;
                mat[14] = 0.0f;
                mat[15] = 0.0f;
            }

            ////////////////////////////////////////////////////////////////////
            //  Set Matrix4x4 from a matrix                                   //
            ////////////////////////////////////////////////////////////////////
            void set(const Matrix4x4& matrix)
            {
                mat[0] = matrix.mat[0];
                mat[1] = matrix.mat[1];
                mat[2] = matrix.mat[2];
                mat[3] = matrix.mat[3];
                mat[4] = matrix.mat[4];
                mat[5] = matrix.mat[5];
                mat[6] = matrix.mat[6];
                mat[7] = matrix.mat[7];
                mat[8] = matrix.mat[8];
                mat[9] = matrix.mat[9];
                mat[10] = matrix.mat[10];
                mat[11] = matrix.mat[11];
                mat[12] = matrix.mat[12];
                mat[13] = matrix.mat[13];
                mat[14] = matrix.mat[14];
                mat[15] = matrix.mat[15];
            }

            ////////////////////////////////////////////////////////////////////
            //  Set Matrix4x4 to identity matrix                              //
            ////////////////////////////////////////////////////////////////////
            void setIdentity()
            {
                mat[0] = 1.0f;
                mat[1] = 0.0f;
                mat[2] = 0.0f;
                mat[3] = 0.0f;
                mat[4] = 0.0f;
                mat[5] = 1.0f;
                mat[6] = 0.0f;
                mat[7] = 0.0f;
                mat[8] = 0.0f;
                mat[9] = 0.0f;
                mat[10] = 1.0f;
                mat[11] = 0.0f;
                mat[12] = 0.0f;
                mat[13] = 0.0f;
                mat[14] = 0.0f;
                mat[15] = 1.0f;
            }

            ////////////////////////////////////////////////////////////////////
            //  Set Matrix4x4 to orthographic view matrix                     //
            ////////////////////////////////////////////////////////////////////
            void setOrthographic(float left, float right,
                float top, float bottom, float znear, float zfar)
            {
                float rightLeft = right-left;
                float topBottom = top-bottom;
                float farNear = zfar-znear;
                if (rightLeft != 0.0f && topBottom != 0.0f && farNear != 0.0f)
                {
                    mat[0] = 2.0f/rightLeft;
                    mat[1] = 0.0f;
                    mat[2] = 0.0f;
                    mat[3] = 0.0f;
                    mat[4] = 0.0f;
                    mat[5] = 2.0f/topBottom;
                    mat[6] = 0.0f;
                    mat[7] = 0.0f;
                    mat[8] = 0.0f;
                    mat[9] = 0.0f;
                    mat[10] = -1.0f/farNear;
                    mat[11] = 0.0f;
                    mat[12] = -(right+left)/rightLeft;
                    mat[13] = -(top+bottom)/topBottom;
                    mat[14] = -znear/farNear;
                    mat[15] = 1.0f;
                }
            }

            ////////////////////////////////////////////////////////////////////
            //  Set Matrix4x4 to frustum view matrix                          //
            ////////////////////////////////////////////////////////////////////
            void setFrustum(float left, float right,
                float top, float bottom, float znear, float zfar)
            {
                float rightLeft = right-left;
                float topBottom = top-bottom;
                float farNear = zfar-znear;
                float nearFar = znear-zfar;
                if (rightLeft != 0.0f && topBottom != 0.0f &&
                    farNear != 0.0f && nearFar != 0.0f)
                {
                    mat[0] = (2.0f*znear)/rightLeft;
                    mat[1] = 0.0f;
                    mat[2] = 0.0f;
                    mat[3] = 0.0f;
                    mat[4] = 0.0f;
                    mat[5] = (2.0f*znear)/topBottom;
                    mat[6] = 0.0f;
                    mat[7] = 0.0f;
                    mat[8] = (right+left)/rightLeft;
                    mat[9] = (top+bottom)/topBottom;
                    mat[10] = zfar/nearFar;
                    mat[11] = -1.0f;
                    mat[12] = 0.0f;
                    mat[13] = 0.0f;
                    mat[14] = -(zfar*znear)/farNear;
                    mat[15] = 0.0f;
                }
            }

            ////////////////////////////////////////////////////////////////////
            //  Set Matrix4x4 to perspective view matrix                      //
            ////////////////////////////////////////////////////////////////////
            void setPerspective(
                float fovy, float ratio, float znear, float zfar)
            {
                // Compute view frustum
                float frustHeight = std::tan(fovy*0.5f)*znear;
                float frustWidth = frustHeight*ratio;
                float left = -frustWidth;
                float right = frustWidth;
                float top = frustHeight;
                float bottom = -frustHeight;

                // Set frustum matrix
                float rightLeft = right-left;
                float topBottom = top-bottom;
                float farNear = zfar-znear;
                float nearFar = znear-zfar;
                if (rightLeft != 0.0f && topBottom != 0.0f &&
                    farNear != 0.0f && nearFar != 0.0f)
                {
                    mat[0] = (2.0f*znear)/rightLeft;
                    mat[1] = 0.0f;
                    mat[2] = 0.0f;
                    mat[3] = 0.0f;
                    mat[4] = 0.0f;
                    mat[5] = (2.0f*znear)/topBottom;
                    mat[6] = 0.0f;
                    mat[7] = 0.0f;
                    mat[8] = (right+left)/rightLeft;
                    mat[9] = (top+bottom)/topBottom;
                    mat[10] = zfar/nearFar;
                    mat[11] = -1.0f;
                    mat[12] = 0.0f;
                    mat[13] = 0.0f;
                    mat[14] = -(zfar*znear)/farNear;
                    mat[15] = 0.0f;
                }
            }

            ////////////////////////////////////////////////////////////////////
            //  Translate Matrix4x4                                           //
            ////////////////////////////////////////////////////////////////////
            void translate(float x, float y)
            {
                mat[12] += (mat[0]*x + mat[4]*y);
                mat[13] += (mat[1]*x + mat[5]*y);
                mat[14] += (mat[2]*x + mat[6]*y);
                mat[15] += (mat[3]*x + mat[7]*y);
            }

            void translate(const Vector2& vector)
            {
                mat[12] += (mat[0]*vector.vec[0] + mat[4]*vector.vec[1]);
                mat[13] += (mat[1]*vector.vec[0] + mat[5]*vector.vec[1]);
                mat[14] += (mat[2]*vector.vec[0] + mat[6]*vector.vec[1]);
                mat[15] += (mat[3]*vector.vec[0] + mat[7]*vector.vec[1]);
            }

            void translate(float x, float y, float z)
            {
                mat[12] += (mat[0]*x + mat[4]*y + mat[8]*z);
                mat[13] += (mat[1]*x + mat[5]*y + mat[9]*z);
                mat[14] += (mat[2]*x + mat[6]*y + mat[10]*z);
                mat[15] += (mat[3]*x + mat[7]*y + mat[11]*z);
            }

            void translate(const Vector3& vector)
            {
                mat[12] += (mat[0]*vector.vec[0]
                        + mat[4]*vector.vec[1]
                        + mat[8]*vector.vec[2]);
                mat[13] += (mat[1]*vector.vec[0]
                        + mat[5]*vector.vec[1]
                        + mat[9]*vector.vec[2]);
                mat[14] += (mat[2]*vector.vec[0]
                        + mat[6]*vector.vec[1]
                        + mat[10]*vector.vec[2]);
                mat[15] += (mat[3]*vector.vec[0]
                        + mat[7]*vector.vec[1]
                        + mat[11]*vector.vec[2]);
            }

            ////////////////////////////////////////////////////////////////////
            //  Translate Matrix4x4 on X axis                                 //
            ////////////////////////////////////////////////////////////////////
            void translateX(float x)
            {
                mat[12] += mat[0]*x;
                mat[13] += mat[1]*x;
                mat[14] += mat[2]*x;
                mat[15] += mat[3]*x;
            }

            ////////////////////////////////////////////////////////////////////
            //  Translate Matrix4x4 on Y axis                                 //
            ////////////////////////////////////////////////////////////////////
            void translateY(float y)
            {
                mat[12] += mat[4]*y;
                mat[13] += mat[5]*y;
                mat[14] += mat[6]*y;
                mat[15] += mat[7]*y;
            }

            ////////////////////////////////////////////////////////////////////
            //  Translate Matrix4x4 on Z axis                                 //
            ////////////////////////////////////////////////////////////////////
            void translateZ(float z)
            {
                mat[12] += mat[8]*z;
                mat[13] += mat[9]*z;
                mat[14] += mat[10]*z;
                mat[15] += mat[11]*z;
            }

            ////////////////////////////////////////////////////////////////////
            //  Rotate Matrix4x4 arround an arbritrary axis                   //
            ////////////////////////////////////////////////////////////////////
            void rotate(float angle, float x, float y, float z)
            {
                float magnitude = std::sqrt(x*x + y*y + z*z);
                float sinAngle = std::sin(-angle);
                float cosAngle = std::cos(-angle);
                float oneMinCos = 1.0f-cosAngle;
                if (magnitude > 0.0f)
                {
                    x /= magnitude;
                    y /= magnitude;
                    z /= magnitude;
                }
                float rot0 = ((oneMinCos*x*x)+cosAngle);
                float rot1 = ((oneMinCos*x*y)-(z*sinAngle));
                float rot2 = ((oneMinCos*z*x)+(y*sinAngle));
                float rot4 = ((oneMinCos*x*y)+(z*sinAngle));
                float rot5 = ((oneMinCos*y*y)+cosAngle);
                float rot6 = ((oneMinCos*y*z)-(x*sinAngle));
                float rot8 = ((oneMinCos*z*x)-(y*sinAngle));
                float rot9 = ((oneMinCos*y*z)+(x*sinAngle));
                float rot10 = ((oneMinCos*z*z)+cosAngle);
                float rotMat0 = (mat[0]*rot0 + mat[4]*rot1 + mat[8]*rot2);
                float rotMat1 = (mat[1]*rot0 + mat[5]*rot1 + mat[9]*rot2);
                float rotMat2 = (mat[2]*rot0 + mat[6]*rot1 + mat[10]*rot2);
                float rotMat3 = (mat[3]*rot0 + mat[7]*rot1 + mat[11]*rot2);
                float rotMat4 = (mat[0]*rot4 + mat[4]*rot5 + mat[8]*rot6);
                float rotMat5 = (mat[1]*rot4 + mat[5]*rot5 + mat[9]*rot6);
                float rotMat6 = (mat[2]*rot4 + mat[6]*rot5 + mat[10]*rot6);
                float rotMat7 = (mat[3]*rot4 + mat[7]*rot5 + mat[11]*rot6);
                float rotMat8 = (mat[0]*rot8 + mat[4]*rot9 + mat[8]*rot10);
                float rotMat9 = (mat[1]*rot8 + mat[5]*rot9 + mat[9]*rot10);
                float rotMat10 = (mat[2]*rot8 + mat[6]*rot9 + mat[10]*rot10);
                float rotMat11 = (mat[3]*rot8 + mat[7]*rot9 + mat[11]*rot10);
                mat[0] = rotMat0;
                mat[1] = rotMat1;
                mat[2] = rotMat2;
                mat[3] = rotMat3;
                mat[4] = rotMat4;
                mat[5] = rotMat5;
                mat[6] = rotMat6;
                mat[7] = rotMat7;
                mat[8] = rotMat8;
                mat[9] = rotMat9;
                mat[10] = rotMat10;
                mat[11] = rotMat11;
            }

            ////////////////////////////////////////////////////////////////////
            //  Rotate Matrix4x4                                              //
            ////////////////////////////////////////////////////////////////////
            void rotate(float angleX, float angleY)
            {
                rotateX(angleX);
                rotateY(angleY);
            }

            void rotate(const Vector2& angles)
            {
                rotateX(angles.vec[0]);
                rotateY(angles.vec[1]);
            }

            void rotate(float angleX, float angleY, float angleZ)
            {
                rotateX(angleX);
                rotateY(angleY);
                rotateZ(angleZ);
            }

            void rotate(const Vector3& angles)
            {
                rotateX(angles.vec[0]);
                rotateY(angles.vec[1]);
                rotateZ(angles.vec[2]);
            }

            ////////////////////////////////////////////////////////////////////
            //  Rotate Matrix4x4 arround X axis                               //
            ////////////////////////////////////////////////////////////////////
            void rotateX(float angleX)
            {
                float sinAngle = std::sin(-angleX);
                float cosAngle = std::cos(-angleX);
                float rotMat4 = (mat[4]*cosAngle + mat[8]*-sinAngle);
                float rotMat5 = (mat[5]*cosAngle + mat[9]*-sinAngle);
                float rotMat6 = (mat[6]*cosAngle + mat[10]*-sinAngle);
                float rotMat7 = (mat[7]*cosAngle + mat[11]*-sinAngle);
                float rotMat8 = (mat[4]*sinAngle + mat[8]*cosAngle);
                float rotMat9 = (mat[5]*sinAngle + mat[9]*cosAngle);
                float rotMat10 = (mat[6]*sinAngle + mat[10]*cosAngle);
                float rotMat11 = (mat[7]*sinAngle + mat[11]*cosAngle);
                mat[4] = rotMat4;
                mat[5] = rotMat5;
                mat[6] = rotMat6;
                mat[7] = rotMat7;
                mat[8] = rotMat8;
                mat[9] = rotMat9;
                mat[10] = rotMat10;
                mat[11] = rotMat11;
            }

            ////////////////////////////////////////////////////////////////////
            //  Rotate Matrix4x4 arround Y axis                               //
            ////////////////////////////////////////////////////////////////////
            void rotateY(float angleY)
            {
                float sinAngle = std::sin(-angleY);
                float cosAngle = std::cos(-angleY);
                float rotMat0 = (mat[0]*cosAngle + mat[8]*sinAngle);
                float rotMat1 = (mat[1]*cosAngle + mat[9]*sinAngle);
                float rotMat2 = (mat[2]*cosAngle + mat[10]*sinAngle);
                float rotMat3 = (mat[3]*cosAngle + mat[11]*sinAngle);
                float rotMat8 = (mat[0]*-sinAngle + mat[8]*cosAngle);
                float rotMat9 = (mat[1]*-sinAngle + mat[9]*cosAngle);
                float rotMat10 = (mat[2]*-sinAngle + mat[10]*cosAngle);
                float rotMat11 = (mat[3]*-sinAngle + mat[11]*cosAngle);
                mat[0] = rotMat0;
                mat[1] = rotMat1;
                mat[2] = rotMat2;
                mat[3] = rotMat3;
                mat[8] = rotMat8;
                mat[9] = rotMat9;
                mat[10] = rotMat10;
                mat[11] = rotMat11;
            }

            ////////////////////////////////////////////////////////////////////
            //  Rotate Matrix4x4 arround Z axis                               //
            ////////////////////////////////////////////////////////////////////
            void rotateZ(float angleZ)
            {
                float sinAngle = std::sin(-angleZ);
                float cosAngle = std::cos(-angleZ);
                float rotMat0 = (mat[0]*cosAngle + mat[4]*-sinAngle);
                float rotMat1 = (mat[1]*cosAngle + mat[5]*-sinAngle);
                float rotMat2 = (mat[2]*cosAngle + mat[6]*-sinAngle);
                float rotMat3 = (mat[3]*cosAngle + mat[7]*-sinAngle);
                float rotMat4 = (mat[0]*sinAngle + mat[4]*cosAngle);
                float rotMat5 = (mat[1]*sinAngle + mat[5]*cosAngle);
                float rotMat6 = (mat[2]*sinAngle + mat[6]*cosAngle);
                float rotMat7 = (mat[3]*sinAngle + mat[7]*cosAngle);
                mat[0] = rotMat0;
                mat[1] = rotMat1;
                mat[2] = rotMat2;
                mat[3] = rotMat3;
                mat[4] = rotMat4;
                mat[5] = rotMat5;
                mat[6] = rotMat6;
                mat[7] = rotMat7;
            }

            ////////////////////////////////////////////////////////////////////
            //  Scale Matrix4x4                                               //
            ////////////////////////////////////////////////////////////////////
            void scale(float x, float y, float z)
            {
                mat[0] *= x;
                mat[1] *= x;
                mat[2] *= x;
                mat[3] *= x;
                mat[4] *= y;
                mat[5] *= y;
                mat[6] *= y;
                mat[7] *= y;
                mat[8] *= z;
                mat[9] *= z;
                mat[10] *= z;
                mat[11] *= z;
            }

            void scale(float x, float y)
            {
                mat[0] *= x;
                mat[1] *= x;
                mat[2] *= x;
                mat[3] *= x;
                mat[4] *= y;
                mat[5] *= y;
                mat[6] *= y;
                mat[7] *= y;
            }

            void scale(float scale)
            {
                mat[0] *= scale;
                mat[1] *= scale;
                mat[2] *= scale;
                mat[3] *= scale;
                mat[4] *= scale;
                mat[5] *= scale;
                mat[6] *= scale;
                mat[7] *= scale;
                mat[8] *= scale;
                mat[9] *= scale;
                mat[10] *= scale;
                mat[11] *= scale;
            }

            void scale(const Vector3& vector)
            {
                mat[0] *= vector.vec[0];
                mat[1] *= vector.vec[0];
                mat[2] *= vector.vec[0];
                mat[3] *= vector.vec[0];
                mat[4] *= vector.vec[1];
                mat[5] *= vector.vec[1];
                mat[6] *= vector.vec[1];
                mat[7] *= vector.vec[1];
                mat[8] *= vector.vec[2];
                mat[9] *= vector.vec[2];
                mat[10] *= vector.vec[2];
                mat[11] *= vector.vec[2];
            }

            void scale(const Vector2& vector)
            {
                mat[0] *= vector.vec[0];
                mat[1] *= vector.vec[0];
                mat[2] *= vector.vec[0];
                mat[3] *= vector.vec[0];
                mat[4] *= vector.vec[1];
                mat[5] *= vector.vec[1];
                mat[6] *= vector.vec[1];
                mat[7] *= vector.vec[1];
            }

            ////////////////////////////////////////////////////////////////////
            //  Scale Matrix4x4 along the X axis                              //
            ////////////////////////////////////////////////////////////////////
            void scaleX(float x)
            {
                mat[0] *= x;
                mat[1] *= x;
                mat[2] *= x;
                mat[3] *= x;
            }

            ////////////////////////////////////////////////////////////////////
            //  Scale Matrix4x4 along the Y axis                              //
            ////////////////////////////////////////////////////////////////////
            void scaleY(float y)
            {
                mat[4] *= y;
                mat[5] *= y;
                mat[6] *= y;
                mat[7] *= y;
            }

            ////////////////////////////////////////////////////////////////////
            //  Scale Matrix4x4 along the Z axis                              //
            ////////////////////////////////////////////////////////////////////
            void scaleZ(float z)
            {
                mat[8] *= z;
                mat[9] *= z;
                mat[10] *= z;
                mat[11] *= z;
            }


            ////////////////////////////////////////////////////////////////////
            //  Matrix4x4 affectation operator                                //
            ////////////////////////////////////////////////////////////////////
            Matrix4x4& operator=(const Matrix4x4& matrix)
            {
                mat[0] = matrix.mat[0];
                mat[1] = matrix.mat[1];
                mat[2] = matrix.mat[2];
                mat[3] = matrix.mat[3];
                mat[4] = matrix.mat[4];
                mat[5] = matrix.mat[5];
                mat[6] = matrix.mat[6];
                mat[7] = matrix.mat[7];
                mat[8] = matrix.mat[8];
                mat[9] = matrix.mat[9];
                mat[10] = matrix.mat[10];
                mat[11] = matrix.mat[11];
                mat[12] = matrix.mat[12];
                mat[13] = matrix.mat[13];
                mat[14] = matrix.mat[14];
                mat[15] = matrix.mat[15];
                return *this;
            }

            Matrix4x4& operator=(float val)
            {
                mat[0] = val;
                mat[1] = val;
                mat[2] = val;
                mat[3] = val;
                mat[4] = val;
                mat[5] = val;
                mat[6] = val;
                mat[7] = val;
                mat[8] = val;
                mat[9] = val;
                mat[10] = val;
                mat[11] = val;
                mat[12] = val;
                mat[13] = val;
                mat[14] = val;
                mat[15] = val;
                return *this;
            }

            ////////////////////////////////////////////////////////////////////
            //  Matrix4x4 addition operator                                   //
            ////////////////////////////////////////////////////////////////////
            Matrix4x4 operator+(const Matrix4x4& matrix)
            {
                Matrix4x4 result;
                result.mat[0] = mat[0] + matrix.mat[0];
                result.mat[1] = mat[1] + matrix.mat[1];
                result.mat[2] = mat[2] + matrix.mat[2];
                result.mat[3] = mat[3] + matrix.mat[3];
                result.mat[4] = mat[4] + matrix.mat[4];
                result.mat[5] = mat[5] + matrix.mat[5];
                result.mat[6] = mat[6] + matrix.mat[6];
                result.mat[7] = mat[7] + matrix.mat[7];
                result.mat[8] = mat[8] + matrix.mat[8];
                result.mat[9] = mat[9] + matrix.mat[9];
                result.mat[10] = mat[10] + matrix.mat[10];
                result.mat[11] = mat[11] + matrix.mat[11];
                result.mat[12] = mat[12] + matrix.mat[12];
                result.mat[13] = mat[13] + matrix.mat[13];
                result.mat[14] = mat[14] + matrix.mat[14];
                result.mat[15] = mat[15] + matrix.mat[15];
                return result;
            }

            ////////////////////////////////////////////////////////////////////
            //  Matrix4x4 subtraction operator                                //
            ////////////////////////////////////////////////////////////////////
            Matrix4x4 operator-(const Matrix4x4& matrix)
            {
                Matrix4x4 result;
                result.mat[0] = mat[0] - matrix.mat[0];
                result.mat[1] = mat[1] - matrix.mat[1];
                result.mat[2] = mat[2] - matrix.mat[2];
                result.mat[3] = mat[3] - matrix.mat[3];
                result.mat[4] = mat[4] - matrix.mat[4];
                result.mat[5] = mat[5] - matrix.mat[5];
                result.mat[6] = mat[6] - matrix.mat[6];
                result.mat[7] = mat[7] - matrix.mat[7];
                result.mat[8] = mat[8] - matrix.mat[8];
                result.mat[9] = mat[9] - matrix.mat[9];
                result.mat[10] = mat[10] - matrix.mat[10];
                result.mat[11] = mat[11] - matrix.mat[11];
                result.mat[12] = mat[12] - matrix.mat[12];
                result.mat[13] = mat[13] - matrix.mat[13];
                result.mat[14] = mat[14] - matrix.mat[14];
                result.mat[15] = mat[15] - matrix.mat[15];
                return result;
            }

            ////////////////////////////////////////////////////////////////////
            //  Matrix4x4 multiplication operator                             //
            ////////////////////////////////////////////////////////////////////
            Matrix4x4 operator*(const Matrix4x4& matrix)
            {
                Matrix4x4 result;
                result.mat[0] = (mat[0]*matrix.mat[0] + mat[4]*matrix.mat[1]
                            + mat[8]*matrix.mat[2] + mat[12]*matrix.mat[3]);
                result.mat[1] = (mat[1]*matrix.mat[0] + mat[5]*matrix.mat[1]
                            + mat[9]*matrix.mat[2] + mat[13]*matrix.mat[3]);
                result.mat[2] = (mat[2]*matrix.mat[0] + mat[6]*matrix.mat[1]
                            + mat[10]*matrix.mat[2] + mat[14]*matrix.mat[3]);
                result.mat[3] = (mat[3]*matrix.mat[0] + mat[7]*matrix.mat[1]
                            + mat[11]*matrix.mat[2] + mat[15]*matrix.mat[3]);
                result.mat[4] = (mat[0]*matrix.mat[4] + mat[4]*matrix.mat[5]
                            + mat[8]*matrix.mat[6] + mat[12]*matrix.mat[7]);
                result.mat[5] = (mat[1]*matrix.mat[4] + mat[5]*matrix.mat[5]
                            + mat[9]*matrix.mat[6] + mat[13]*matrix.mat[7]);
                result.mat[6] = (mat[2]*matrix.mat[4] + mat[6]*matrix.mat[5]
                            + mat[10]*matrix.mat[6] + mat[14]*matrix.mat[7]);
                result.mat[7] = (mat[3]*matrix.mat[4] + mat[7]*matrix.mat[5]
                            + mat[11]*matrix.mat[6] + mat[15]*matrix.mat[7]);
                result.mat[8] = (mat[0]*matrix.mat[8] + mat[4]*matrix.mat[9]
                            + mat[8]*matrix.mat[10] + mat[12]*matrix.mat[11]);
                result.mat[9] = (mat[1]*matrix.mat[8] + mat[5]*matrix.mat[9]
                            + mat[9]*matrix.mat[10] + mat[13]*matrix.mat[11]);
                result.mat[10] = (mat[2]*matrix.mat[8] + mat[6]*matrix.mat[9]
                            + mat[10]*matrix.mat[10] + mat[14]*matrix.mat[11]);
                result.mat[11] = (mat[3]*matrix.mat[8] + mat[7]*matrix.mat[9]
                            + mat[11]*matrix.mat[10] + mat[15]*matrix.mat[11]);
                result.mat[12] = (mat[0]*matrix.mat[12] + mat[4]*matrix.mat[13]
                            + mat[8]*matrix.mat[14] + mat[12]*matrix.mat[15]);
                result.mat[13] = (mat[1]*matrix.mat[12] + mat[5]*matrix.mat[13]
                            + mat[9]*matrix.mat[14] + mat[13]*matrix.mat[15]);
                result.mat[14] = (mat[2]*matrix.mat[12] + mat[6]*matrix.mat[13]
                            + mat[10]*matrix.mat[14] + mat[14]*matrix.mat[15]);
                result.mat[15] = (mat[3]*matrix.mat[12] + mat[7]*matrix.mat[13]
                            + mat[11]*matrix.mat[14] + mat[15]*matrix.mat[15]);
                return result;
            }

            ////////////////////////////////////////////////////////////////////
            //  Matrix4x4 addition assignment operator                        //
            ////////////////////////////////////////////////////////////////////
            Matrix4x4& operator+=(const Matrix4x4& matrix)
            {
                mat[0] += matrix.mat[0];
                mat[1] += matrix.mat[1];
                mat[2] += matrix.mat[2];
                mat[3] += matrix.mat[3];
                mat[4] += matrix.mat[4];
                mat[5] += matrix.mat[5];
                mat[6] += matrix.mat[6];
                mat[7] += matrix.mat[7];
                mat[8] += matrix.mat[8];
                mat[9] += matrix.mat[9];
                mat[10] += matrix.mat[10];
                mat[11] += matrix.mat[11];
                mat[12] += matrix.mat[12];
                mat[13] += matrix.mat[13];
                mat[14] += matrix.mat[14];
                mat[15] += matrix.mat[15];
                return *this;
            }

            ////////////////////////////////////////////////////////////////////
            //  Matrix4x4 subtraction assignment operator                     //
            ////////////////////////////////////////////////////////////////////
            Matrix4x4& operator-=(const Matrix4x4& matrix)
            {
                mat[0] -= matrix.mat[0];
                mat[1] -= matrix.mat[1];
                mat[2] -= matrix.mat[2];
                mat[3] -= matrix.mat[3];
                mat[4] -= matrix.mat[4];
                mat[5] -= matrix.mat[5];
                mat[6] -= matrix.mat[6];
                mat[7] -= matrix.mat[7];
                mat[8] -= matrix.mat[8];
                mat[9] -= matrix.mat[9];
                mat[10] -= matrix.mat[10];
                mat[11] -= matrix.mat[11];
                mat[12] -= matrix.mat[12];
                mat[13] -= matrix.mat[13];
                mat[14] -= matrix.mat[14];
                mat[15] -= matrix.mat[15];
                return *this;
            }

            ////////////////////////////////////////////////////////////////////
            //  Matrix4x4 multiplication assignment operator                  //
            ////////////////////////////////////////////////////////////////////
            Matrix4x4& operator*=(const Matrix4x4& matrix)
            {
                float multMat0 = (mat[0]*matrix.mat[0] + mat[4]*matrix.mat[1]
                            + mat[8]*matrix.mat[2] + mat[12]*matrix.mat[3]);
                float multMat1 = (mat[1]*matrix.mat[0] + mat[5]*matrix.mat[1]
                            + mat[9]*matrix.mat[2] + mat[13]*matrix.mat[3]);
                float multMat2 = (mat[2]*matrix.mat[0] + mat[6]*matrix.mat[1]
                            + mat[10]*matrix.mat[2] + mat[14]*matrix.mat[3]);
                float multMat3 = (mat[3]*matrix.mat[0] + mat[7]*matrix.mat[1]
                            + mat[11]*matrix.mat[2] + mat[15]*matrix.mat[3]);
                float multMat4 = (mat[0]*matrix.mat[4] + mat[4]*matrix.mat[5]
                            + mat[8]*matrix.mat[6] + mat[12]*matrix.mat[7]);
                float multMat5 = (mat[1]*matrix.mat[4] + mat[5]*matrix.mat[5]
                            + mat[9]*matrix.mat[6] + mat[13]*matrix.mat[7]);
                float multMat6 = (mat[2]*matrix.mat[4] + mat[6]*matrix.mat[5]
                            + mat[10]*matrix.mat[6] + mat[14]*matrix.mat[7]);
                float multMat7 = (mat[3]*matrix.mat[4] + mat[7]*matrix.mat[5]
                            + mat[11]*matrix.mat[6] + mat[15]*matrix.mat[7]);
                float multMat8 = (mat[0]*matrix.mat[8] + mat[4]*matrix.mat[9]
                            + mat[8]*matrix.mat[10] + mat[12]*matrix.mat[11]);
                float multMat9 = (mat[1]*matrix.mat[8] + mat[5]*matrix.mat[9]
                            + mat[9]*matrix.mat[10] + mat[13]*matrix.mat[11]);
                float multMat10 = (mat[2]*matrix.mat[8] + mat[6]*matrix.mat[9]
                            + mat[10]*matrix.mat[10] + mat[14]*matrix.mat[11]);
                float multMat11 = (mat[3]*matrix.mat[8] + mat[7]*matrix.mat[9]
                            + mat[11]*matrix.mat[10] + mat[15]*matrix.mat[11]);
                float multMat12 = (mat[0]*matrix.mat[12] + mat[4]*matrix.mat[13]
                            + mat[8]*matrix.mat[14] + mat[12]*matrix.mat[15]);
                float multMat13 = (mat[1]*matrix.mat[12] + mat[5]*matrix.mat[13]
                            + mat[9]*matrix.mat[14] + mat[13]*matrix.mat[15]);
                float multMat14 = (mat[2]*matrix.mat[12] + mat[6]*matrix.mat[13]
                            + mat[10]*matrix.mat[14] + mat[14]*matrix.mat[15]);
                float multMat15 = (mat[3]*matrix.mat[12] + mat[7]*matrix.mat[13]
                            + mat[11]*matrix.mat[14] + mat[15]*matrix.mat[15]);
                mat[0] = multMat0;
                mat[1] = multMat1;
                mat[2] = multMat2;
                mat[3] = multMat3;
                mat[4] = multMat4;
                mat[5] = multMat5;
                mat[6] = multMat6;
                mat[7] = multMat7;
                mat[8] = multMat8;
                mat[9] = multMat9;
                mat[10] = multMat10;
                mat[11] = multMat11;
                mat[12] = multMat12;
                mat[13] = multMat13;
                mat[14] = multMat14;
                mat[15] = multMat15;
                return *this;
            }


        public:
            float   mat[16];    // 4x4 matrix representation
    };


#endif // VTOOLS_OBJTOVMSH_MATH_MATRIX4X4_HEADER
