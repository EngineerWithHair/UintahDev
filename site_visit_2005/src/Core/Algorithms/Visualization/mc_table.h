/*
   For more information, please see: http://software.sci.utah.edu

   The MIT License

   Copyright (c) 2004 Scientific Computing and Imaging Institute,
   University of Utah.

   License for the specific language governing rights and limitations under
   Permission is hereby granted, free of charge, to any person obtaining a
   copy of this software and associated documentation files (the "Software"),
   to deal in the Software without restriction, including without limitation
   the rights to use, copy, modify, merge, publish, distribute, sublicense,
   and/or sell copies of the Software, and to permit persons to whom the
   Software is furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included
   in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
   THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
   DEALINGS IN THE SOFTWARE.
*/


/*
 *  mc_table.h : Marching Cubes tesselation table for hexagons.
 *               This table is based on triangles fans.
 *
 *  \author Yarden Livnat
 *   Department of Computer Science
 *   University of Utah
 *   \date Feb 2001
 *
 *  Copyright (C) 2001 SCI Institute
 */

#ifndef MCUBE3_H
#define MCUBE3_H

#if 0
static int edge_table[12][2] = {{0,1}, {1,2}, {3,2}, {0,3},
				{4,5}, {5,6}, {7,6}, {4,7},
				{0,4}, {1,5}, {3,7}, {2,6}};
#endif

struct TriangleCase {
  int n;
  int vertex[16];
};


#if 0
static TriangleCase tri_case[] = {
  /* 00000000 */
  { 0, { -1  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 00000001 */
  { 1, {  0,  3,  8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 00000010 */
  { 1, {  0,  9,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 00000011 */
  { 1, {  1,  3,  8,  9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 00000100 */
  { 1, {  1, 11,  2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 00000101 */
  { 2, {  0,  3,  8, -1,  1, 11,  2, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 00000110 */
  { 1, {  9, 11,  2,  0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 00000111 */
  { 1, {  2,  3,  8,  9, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 00001000 */
  { 1, {  3,  2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 00001001 */
  { 1, {  0,  2, 10,  8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 00001010 */
  { 2, {  1,  0,  9, -1,  2, 10,  3, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 00001011*/
  { 1, {  8,  9,  1,  2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 00001100 */  
  { 1, {  3,  1, 11, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 00001101 */
  { 1, {  0,  1, 11, 10,  8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 00001110 */
  { 1, {  3,  0,  9, 11, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 00001111 */
  { 1, {  9, 11, 10,  8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 00010000 */
  { 1, {  4,  8,  7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 00010001 */
  { 1, {  4,  0,  3,  7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 00010010 */
  { 2, {  0,  9,  1, -1,  8,  7,  4, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 00010011 */
  { 1, {  4,  9,  1,  3,  7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 00010100 */
  { 2, {  1, 11,  2, -1,  8,  7,  4, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 00010101 */
  { 2, {  3,  7,  4,  0, -1,  1, 11,  2, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 00010110 */
  { 2, {  9, 11,  2,  0, -1,  8,  7,  4, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 00010111 */
  { 1, {  2,  3,  7,  4,  9, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 00011000 */
  { 2, {  8,  7,  4, -1,  3,  2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 00011001 */
  { 1, { 10,  7,  4,  0,  2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 00011010 */
  { 3, {  9,  1,  0, -1,  8,  7,  4, -1,  2, 10,  3, -1, -1, -1, -1, -1}},
  /* 00011011 */
  { 1, {  4,  9,  1,  2, 10,  7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 00011100 */
  { 2, {  3,  1, 11, 10, -1,  7,  4,  8, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 00011101 */
  { 1, {  1, 11, 10,  7,  4,  0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 00011110 */
  { 2, {  4,  8,  7, -1,  9, 11, 10,  3,  0, -1, -1, -1, -1, -1, -1, -1}},
  /* 00011111 */
  { 1, {  4,  9, 11, 10,  7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 00100000 */
  {  1, { 9,  4,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 00100001 */
  { 2, {  9,  4,  5, -1,  0,  3,  8, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 00100010 */
  { 1, {  0,  4,  5,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 00100011 */
  { 1, {  8,  4,  5,  1,  3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 00100100 */
  { 2, {  1, 11,  2, -1,  9,  4,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 00100101 */
  { 3, {  3,  8,  0, -1,  1, 11,  2, -1,  4,  5,  9, -1, -1, -1, -1, -1}},
  /* 00100110 */
  { 1, {  5, 11,  2,  0,  4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 00100111 */
  { 1, {  2,  3,  8,  4,  5, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 00101000 */
  { 2, {  9,  4,  5, -1,  2, 10,  3, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 00101001 */
  { 2, {  0,  2, 10,  8, -1,  4,  5,  9, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 00101010 */
  { 2, {  0,  4,  5,  1, -1,  2, 10,  3, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 00101011 */
  { 1, {  2,  10, 8,  4,  5,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 00101100 */
  { 2, { 11, 10,  3,  1, -1,  9,  4,  5, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 00101101 */
  { 2, {  4,  5,  9, -1,  0,  1, 11, 10,  8, -1, -1, -1, -1, -1, -1, -1}},
  /* 00101110 */
  { 1, {  5, 11, 10,  3,  0,  4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 00101111 */
  { 1, {  5, 11, 10,  8,  4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 00110000 */
  { 1, {  9,  8,  7,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 00110001 */
  { 1, {  9,  0,  3,  7,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 00110010 */
  { 1, {  0,  8,  7,  5,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 00110011 */
  { 1, {  1,  3,  7,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 00110100 */
  { 2, {  9,  8,  7,  5, -1, 11,  2,  1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 00110101 */
  { 2, { 11,  2,  1, -1,  9,  0,  3,  7,  5,  3, -1, -1, -1, -1, -1, -1}},
  /* 00110110 */
  { 1, {  8,  7,  5, 11,  2,  0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 00110111 */
  { 1, {  2,  3,  7,  5, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 00111000 */
  { 2, {  7,  5,  9,  8, -1,  3,  2, 10, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 00111001 */
  { 1, {  9,  0,  2, 10,  7,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 00111010 */
  { 2, {  2, 10,  3, -1,  0,  8,  7,  5,  1, -1, -1, -1, -1, -1, -1, -1}},
  /* 00111011 */
  { 1, { 10,  7,  5,  1,  2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 00111100 */
  { 2, {  9,  8,  7,  5, -1, 11, 10,  3,  1, -1, -1, -1, -1, -1, -1, -1}},
  /* 00111101 */
  { 1, {  0,  1, 11, 10,  7,  5,  9, -1, -1, -1, -1, -1, -1, -1, -1, -1}}, 
  /* 00111110 */
  { 1, {  0,  8,  7,  5, 11, 10,  3, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 00111111 */
  { 1, { 10,  7,  5, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 01000000 */
  { 1, { 11,  5,  6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 01000001 */
  { 2, {  0,  3,  8, -1,  5,  6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 01000010 */
  { 2, {  9,  1,  0, -1,  5,  6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 01000011 */
  { 2, {  1,  3,  8,  9, -1,  5,  6, 11, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 01000100 */
  { 1, {  1,  5,  6,  2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 01000101 */
  { 2, {  1,  5,  6,  2, -1,  3,  8,  0, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 01000110 */
  { 1, {  9,  5,  6,  2,  0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 01000111 */
  { 1, {  5,  6,  2,  3,  8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 01001000 */
  { 2, {  2, 10,  3, -1, 11,  5,  6, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 01001001 */
  { 2, { 10,  8,  0,  2, -1, 11,  5,  6, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 01001010 */
  { 3, {  0,  9,  1, -1,  2, 10,  3, -1,  5,  6, 11, -1, -1, -1, -1, -1}},
  /* 01001011 */
  { 2, {  5,  6, 11, -1,  9,  1,  2, 10,  8, -1, -1, -1, -1, -1, -1, -1}},
  /* 01001100 */
  { 1, {  3,  1,  5,  6, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 01001101 */
  { 1, {  0,  1,  5,  6, 10,  8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 01001110 */
  { 1, {  3,  0,  9,  5 , 6, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 01001111 */
  { 1, {  9,  5,  6, 10,  8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 01010000 */
  { 2, {  5,  6, 11, -1,  4,  8,  7, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 01010001 */
  { 2, {  4,  0,  3,  7, -1,  6, 11,  5, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 01010010 */
  { 3, {  1,  0,  9, -1,  5,  6, 11, -1,  8,  7,  4, -1, -1, -1, -1, -1}},
  /* 01010011 */
  { 2, { 11,  5,  6, -1,  9,  1,  3,  7,  4, -1, -1, -1, -1, -1, -1, -1}},
  /* 01010100 */
  { 2, {  6,  2,  1,  5, -1,  4,  8,  7, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 01010101 */
  { 2, {  1,  5,  6,  2, -1,  3,  7,  4,  0, -1, -1, -1, -1, -1, -1, -1}},
  /* 01010110 */
  { 2, {  8,  7,  4, -1,  0,  9,  5,  6,  2, -1, -1, -1, -1, -1, -1, -1}},
  /* 01010111 */
  { 1, {  9,  5,  6,  2,  3,  7,  4, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 01011000 */
  { 3, {  3,  2, 10, -1,  7,  4,  8, -1, 11,  5,  6, -1, -1, -1, -1, -1}},
  /* 01011001 */
  { 2, {  5,  6, 11, -1,  4,  0,  2, 10,  7, -1, -1, -1, -1, -1, -1, -1}},
  /* 01011010 */
  { 4, {  0,  9,  1, -1,  4,  8,  7, -1,  2, 10,  3, -1,  5,  6, 11, -1}},
  /* 01011011 */
  { 2, {  9,  1,  2, 10,  7,  4, -1,  5,  6, 11, -1, -1, -1, -1, -1, -1}},
  /* 01011100 */
  { 2, {  8,  7,  4, -1,  5 , 6, 10,  3,  1, -1, -1, -1, -1, -1, -1, -1}},
  /* 01011101 */ 
  { 1, { 10,  7,  4,  0,  1,  5,  6, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 01011110 */
  { 2, {  0,  9,  5,  6, 10,  3, -1,  8,  7,  4, -1, -1, -1, -1, -1, -1}},
  /* 01011111 */
  { 1, {  9,  5,  6, 10,  7,  4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 01100000 */
  { 1, { 11,  9,  4,  6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 01100001 */
  { 2, {  4,  6, 11,  9, -1,  0,  3,  8, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 01100010 */ 
  { 1, {  0,  4,  6, 11,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 01100011 */
  { 1, {  8,  4,  6, 11,  1,  3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 01100100 */
  { 1, {  4,  6,  2,  1,  9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 01100101 */
  { 2, {  3,  8,  0, -1,  4,  6,  2,  1,  9, -1, -1, -1, -1, -1, -1, -1}},
  /* 01100110 */
  { 1, {  0,  4,  6,  2,  4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 01100111 */
  { 1, {  2,  3,  8,  4,  6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 01101000 */
  { 2, { 11,  9,  4,  6, -1, 10,  3,  2, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 01101001 */
  { 2, {  0,  2, 10,  8, -1,  4,  6, 11,  9, -1, -1, -1, -1, -1, -1, -1}},
  /* 01101010 */
  { 2, {  3,  2, 10, -1,  6, 11,  1,  0,  4, -1, -1, -1, -1, -1, -1, -1}},
  /* 01101011 */
  { 1, {  1, 10,  8,  4,  6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 01101100 */
  { 1, {  6,  9,  1,  3, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 01101101 */
  { 1, {  1,  0,  8, 10,  6,  4,  9, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 01101110 */
  { 1, {  6, 10,  3,  0,  4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 01101111 */
  { 1, {  6,  10, 8,  4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 01110000 */
  { 1, {  7,  6, 11,  9,  8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 01110001 */
  { 1, {  0,  3,  7,  6, 11,  9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 01110010 */
  { 1, {  1,  0,  8,  7,  6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 01110011 */
  { 1, {  1,  3,  7,  6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 01110100 */
  { 1, {  8,  7,  6,  2,  1,  9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 01110101 */
  { 1, {  9,  0,  3,  7,  6,  2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 01110110 */
  { 1, {  0,  8,  7,  6,  2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 01110111 */
  { 1, {  7,  6,  2,  3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 01111000 */
  { 2, {  2, 10,  3, -1,  8,  7,  6, 11,  9, -1, -1, -1, -1, -1, -1, -1}},
  /* 01111001 */
  { 1, {  7,  6, 11,  9,  0,  2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 01111010 */
  { 2, {  7,  1,  0,  8, 11,  6, -1,  2, 10,  3, -1, -1, -1, -1, -1, -1}},
  /* 01111011 */
  { 1, {  1,  2, 10,  7,  6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 01111100 */
  { 1, {  6, 10,  3,  1,  9,  8,  7, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 01111101 */
  { 2, {  0,  1,  9, -1, 10,  7,  6, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 01111110 */
  { 1, {  0,  8,  7,  6, 10,  3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 01111111 */
  { 1, {  7,  6, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 10000000 */
  { 1, {  7, 10,  6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 10000001 */
  { 2, {  3,  8,  0, -1, 10,  6,  7, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 10000010 */
  { 2, {  0,  9,  1, -1, 10,  6,  7, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 10000011 */
  { 2, {  8,  9,  1,  3, 10,  6,  7, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 10000100 */
  { 2, { 11,  2,  1, -1,  6,  7, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 10000101 */
  { 3, {  1, 11,  2, -1,  3,  8,  0, -1,  6,  7, 10, -1, -1, -1, -1, -1}},
  /* 10000110 */
  { 2, {  2,  0,  9, 11, -1,  6,  7, 10, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 10000111 */
  { 2, {  6,  7, 10, -1, 11,  2,  3,  8,  9, -1, -1, -1, -1, -1, -1, -1}},
  /* 10001000 */
  { 1, {  7,  3,  2,  6, -1,  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 10001001 */
  { 1, {  7,  8,  0,  2,  6, -1, -1, -1, -1, -1, -1, -1, -1 ,-1, -1, -1}},
  /* 10001010 */
  { 2, {  2,  6,  7,  3, -1,  0,  9,  1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 10001011 */
  { 1, {  1,  2,  6,  7,  8,  9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 10001100 */
  { 1, { 11,  6,  7,  3,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 10001101 */
  { 1, { 11,  6,  7,  8,  0,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 10001110 */
  { 1, {  0,  9, 11,  6,  7,  3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 10001111 */
  { 1, {  7,  8,  9, 11,  6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 10010000 */
  { 1, {  6,  4,  8, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 10010001 */
  { 1, {  3, 10,  6,  4,  0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 10010010 */
  { 2, {  8, 10,  6,  4, -1,  9,  1,  0, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 10010011 */
  { 1, {  9,  1,  3, 10,  6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 10010100 */
  { 2, {  6,  4,  8, 10, -1,  2,  1, 11, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 10010101 */
  { 2, {  1, 11,  2, -1,  3, 10,  6,  4,  0, -1, -1, -1, -1, -1, -1, -1}},
  /* 10010110 */
  { 2, {  4,  8, 10,  6, -1,  0,  9, 11,  2, -1, -1, -1, -1, -1, -1, -1}},
  /* 10010111 */
  { 1, {  3, 10,  6,  4,  9, 11,  2, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 10011000 */
  { 1, {  8,  3,  2,  6,  4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 10011001 */
  { 1, {  0,  2,  6,  4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 10011010 */
  { 2, {  1,  0,  9, -1,  2,  6,  4,  8,  3, -1, -1, -1, -1, -1, -1, -1}},
  /* 10011011 */
  { 1, {  4,  9,  1,  2,  6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 10011100 */
  { 1, {  4,  3,  8,  6,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 10011101 */
  { 1, {  0,  1, 11,  6,  4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 10011110 */
  { 1, {  3,  0,  9, 11,  6,  4,  8, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 10011111 */
  { 1, { 11,  6,  4,  9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 10100000 */
  { 2, {  4,  5,  9, -1,  7, 10,  6, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 10100001 */
  { 3, {  0,  3,  8, -1,  4,  5,  9, -1, 10,  6,  7, -1, -1, -1, -1, -1}},
  /* 10100010 */
  { 2, {  5,  1,  0,  4, -1,  7, 10,  6, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 10100011 */
  { 2, { 10,  6,  7, -1,  3,  8,  4,  5,  1, -1, -1, -1, -1, -1, -1, -1}},
  /* 10100100 */
  { 3, {  9,  4,  5, -1, 11,  2,  1, -1,  7, 10,  6, -1, -1, -1, -1, -1}},
  /* 10100101 */
  { 4, {  6,  7, 10, -1,  1, 11,  2, -1,  0,  3,  8,  -1, 4,  5,  9, -1}},
  /* 10100110 */
  { 2, {  7, 10,  6, -1,  4,  5, 11,  2,  0, -1, -1, -1, -1, -1, -1, -1}},
  /* 10100111 */
  { 2, {  3,  8,  4,  5, 11,  2, -1, 10,  6,  7, -1, -1, -1, -1, -1, -1}},
  /* 10101000 */
  { 2, {  7,  3,  2,  6, -1,  5,  9,  4, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 10101001 */
  { 2, {  9,  4,  5, -1,  6,  7,  8,  0,  2, -1, -1, -1, -1, -1, -1, -1}},
  /* 10101010 */
  { 2, {  3,  2,  6,  7, -1,  1,  0,  4,  5, -1, -1, -1, -1, -1, -1, -1}},
  /* 10101011 */
  { 1, {  8,  4,  5,  1,  2,  6,  7, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 10101100 */
  { 2, {  9,  4,  5, -1,  11,  6,  7,  3,  1, -1, -1, -1, -1, -1, -1, -1}},
  /* 10101101 */
  { 2, {  1, 11,  6,  7,  8,  0, -1, -1, -1, -1, -1, -1,  9,  4,  5, -1}},
  /* 10101110 */
  { 1, {  4,  5, 11,  6,  7,  3,  0, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 10101111 */
  { 1, { 11,  6,  7,  8,  4,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 10110000 */
  { 1, {  6,  5,  9,  8, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 10110001 */
  { 1, {  0,  3, 10,  6,  5,  9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 10110010 */
  { 1, {  0,  8, 10,  6,  5,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 10110011 */
  { 1, {  3, 10,  6,  5,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 10110100 */
  { 2, {  1, 11,  2, -1, 10,  6,  5,  9,  8, 10, -1, -1, -1, -1, -1, -1}},
  /* 10110101 */
  { 2, {  0,  3, 10,  6,  5,  9, -1,  1, 11,  2, -1, -1, -1, -1, -1, -1}},
  /* 10110110 */
  { 1, {  5, 11,  2,  0,  8, 10,  6, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 10110111 */
  { 1, {  3, 10,  6,  5, 11,  2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 10111000 */
  { 1, {  2,  6,  5,  9,  8,  3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 10111001 */
  { 1, {  6,  5,  9,  0,  2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 10111010 */
  { 1, {  8,  3,  2,  6,  5,  1,  0, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 10111011 */
  { 1,  { 1,  2,  6,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 10111100 */
  { 1, {  6,  5,  9,  8,  3,  1, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 10111101 */
  { 1, {  0,  1, 11,  6,  5,  9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 10111110 */
  { 2, {  0,  8,  3, -1,  5, 11,  6, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 10111111 */
  { 1, { 11,  6,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 11000000 */
  { 1, { 10, 11,  5,  7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 11000001 */
  { 2, { 10, 11,  5,  7, -1,  8,  0,  3, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 11000010 */
  { 2, {  5,  7, 10, 11, -1,  1,  0,  9, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 11000011 */
  { 2, { 11,  5,  7, 10, -1,  9,  1,  3,  8, -1, -1, -1, -1, -1, -1, -1}},
  /* 11000100 */
  { 1, { 10,  2,  1,  5,  7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 11000101 */
  { 2, {  0,  3,  8, -1,  1,  5,  7, 10,  2, -1, -1, -1, -1, -1, -1, -1}},
  /* 11000110 */
  { 1, {  9,  5,  7, 10,  2,  0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 11000111 */
  { 1, {  7, 10,  2,  3,  8,  9,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 11001000 */
  { 1, {  5,  7,  3,  2, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 11001001 */
  { 1, {  8,  0,  2, 11,  5,  7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 11001010 */
  { 2, {  9,  1,  0, -1,  5,  7,  3,  2, 11, -1, -1, -1, -1, -1, -1, -1}},
  /* 11001011 */
  { 1, {  2, 11,  5,  7,  8,  9,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 11001100 */
  { 1, {  3,  1,  5,  7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 11001101 */
  { 1, {  7,  8,  0,  1,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 11001110 */
  { 1, {  3,  0,  9,  5,  7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 11001111 */
  { 1, {  9,  5,  7,  8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 11010000 */
  { 1, {  8, 10, 11,  5,  4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 11010001 */
  { 1, {  5,  4,  0,  3, 10, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 11010010 */
  { 2, {  0,  9,  1, -1,  8, 10, 11,  5,  4, -1, -1, -1, -1, -1, -1, -1}},
  /* 11010011 */
  { 1, {  4,  9,  1,  3, 10, 11,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 11010100 */
  { 1, {  5,  4,  8, 10,  2,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 11010101 */
  { 1, { 10,  2,  1,  5,  4,  0,  3, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 11010110 */
  { 1, {  5,  4,  8, 10,  2,  0,  9, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 11010111 */
  { 2, {  9,  5,  4, -1,  2,  3, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 11011000 */
  { 1, {  5,  4,  8,  3,  2, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 11011001 */
  { 1, {  2, 11,  5,  4,  0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 11011010 */
  { 2, {  3,  2, 11,  5,  4,  8, -1,  0,  9,  1, -1, -1, -1, -1, -1, -1}},
  /* 11011011 */
  { 1, {  2, 11,  5,  4,  9,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 11011100 */
  { 1, {  5,  4,  8,  3,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 11011101 */
  { 1, {  0,  1,  5,  4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 11011110 */
  { 1, {  5,  4,  8,  3,  0,  9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 11011111 */
  { 1, {  9,  5,  4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 11100000 */
  { 1,{  10, 11,  9,  4,  7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 11100001 */
  { 2, {  0,  3,  8, -1,  9,  7,  4, 10, 11, -1, -1, -1, -1, -1, -1, -1}},
  /* 11100010 */
  { 1, { 10, 11,  1,  0,  4,  7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 11100011 */
  { 1, {  4,  7, 10, 11,  1,  3,  8, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 11100100 */
  { 1, {  4,  7, 10,  2,  1,  9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 11100101 */
  { 2, {  9,  4,  7, 10,  2,  1, -1,  0,  3,  8, -1, -1, -1, -1, -1, -1}},
  /* 11100110 */
  { 1, {  4,  7, 10,  2,  0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 11100111 */
  { 1, {  4,  7, 10,  2,  3,  8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 11101000 */
  { 1, {  2, 11,  9,  4,  7,  3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 11101001 */
  { 1, {  7,  8,  0,  2, 11,  9,  4, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 11101010 */
  { 1, { 11,  1,  0,  4,  7,  3,  2, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 11101011 */
  { 2, {  1,  2, 11, -1,  8,  4,  7, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 11101100 */
  { 1, {  1,  9,  4,  7,  3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 11101101 */
  { 1, {  1,  9,  4,  7,  8,  0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 11101110 */
  { 1, {  4,  7,  3,  0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 11101111 */
  { 1, {  4,  7,  8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 11110000 */
  { 1, {  9,  8, 10, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 11110001 */
  { 1, {  9,  0,  3, 10, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 11110010 */
  { 1, { 11,  1,  0,  8, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 11110011 */
  { 1, {  3, 10, 11,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 11110100 */
  { 1, { 10,  2,  1,  9,  8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 11110101 */
  { 1, {  9,  0,  3, 10,  2,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 11110110 */
  { 1, {  0,  8, 10,  2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 11110111 */
  { 1, {  3, 10,  2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 11111000 */
  { 1, {  8,  3,  2, 11,  9,  8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 11111001 */
  { 1, {  9,  0,  2, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 11111010 */
  { 1, {  8,  3,  2, 11,  1,  0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 11111011 */
  { 1, {  1,  2, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 11111100 */
  { 1, {  1,  9,  8,  3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 11111101 */
  { 1, {  0,  1,  9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 11111110 */
  { 1, {  0,  8,  3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
  /* 11111111 */
  { 0, { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}}
};
#endif

#endif // MCUBE3_H
