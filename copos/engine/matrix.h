/************************************************************************
* Fichier          : matrix.h
* Date de Creation : Thu Sep 29 2005
* Auteur           : Ronan Billon
* E-mail           : cirdan@mail.berlios.de

This file was generated with umbrello
**************************************************************************/

#ifndef MATRIX_H
#define MATRIX_H
#include <math.h>
#include <string.h>
#include "point3d.h"

/**
  * Class Matrix
  * This structure makes it possible to manage the two-dimensional array. 
  * There is some special functions for the 3D transformations.
  * 
  */
typedef struct Matrix
{
  /**
   * Fields
   */
  /**
   * Number of rows
   */
   guint rows;
  /**
   * Number of columns
   */
   guint cols;
  /**
   * Array of data
   */
   gdouble *data;
} Matrix;


  /**
   * Basic constructor
   * @param rows The number of rows
   * @param cols The number of columns
   */
  Matrix*  Matrix_new (guint rows, guint cols);
    
  
  /**
   * Special constructor for a 4x4 matrix
   */
  Matrix*  Matrix_new4x4 ();
    
  
  /**
   * Create a copy of the matrix
   * @param *src The matrix to be cloned
   */
  Matrix*  Matrix_clone (Matrix *src);
    
  
  /**
   * Destructor
   */
  void  Matrix_destroy (Matrix *this);
    
  
  /**
   * Return the element
   * @param row The row where to seek the element
   * @param col The column where to seek the element
   */
  gdouble  Matrix_get (Matrix *this, guint row, guint col);
    
  
  /**
   *  Return a pointer on the modifiable area/box
   * @param row The row where to seek the element 
   * @param col The column where to seek the element
   */
  gdouble*  Matrix_set (Matrix *this, guint row, guint col);
    
  
  /**
   * Copy a matrix in another one
   * @param *src The source to be copied
   * @param *dst The destination Matrix_matrix (Matrix *this, it will be erased)
   */
  void  Matrix_copy (Matrix *src, Matrix *dst);
    
  
  /**
   * Put all elements to zero
   */
  void  Matrix_setZero (Matrix *this);
    
  
  /**
   * Transform into an identity Matrix_matrix (Matrix *this, Warning: cols == rows) 
   */
  void  Matrix_setIdentity (Matrix *this);
    
  
  /**
   * Add a rotation in radian to the matrix 
   * @param rad The angle in radian
   */
  void  Matrix_rotX (Matrix *this, gdouble rad);
    
  
  /**
   * Add a rotation in radian to the matrix 
   * @param rad The angle in radian
   */
  void  Matrix_rotY (Matrix *this, gdouble rad);
    
  
  /**
   * Add a rotation in radian to the matrix 
   * @param rad The angle in radian
   */
  void  Matrix_rotZ (Matrix *this, gdouble rad);
    
  
  /**
   * Add Matrix_translation (Matrix *this, x, y, z) to the matrix 
   * @param x Displacement in x 
   * @param y Displacement in y
   * @param z Displacement in z
   */
  void  Matrix_translation (Matrix *this, gdouble x, gdouble y, gdouble z);
    
  
  /**
   * This function makes the  product of two matrix
   * @param *mat The matrix to be the argument of the product
   */
  void  Matrix_product (Matrix *this, Matrix *mat);
    
  
  /**
   * It makes the transformation of a point
   * @param *src The source point which will be transformed
   * @param *dst the resulting point
   */
  void  Matrix_transform (Matrix *this, Point3D *src, Point3D *dst);
    
  
#endif //MATRIX_H
