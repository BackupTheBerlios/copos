/************************************************************************
* Fichier          : matrix.h
* Date de Creation : jeu mar 25 2004
* Auteur           : Ronan Billon
* E-mail           : cirdan@mail.berlios.de

This file was generated on jeu mar 25 2004 at 21:26:13 with umbrello
**************************************************************************/

#ifndef MATRIX_H
#define MATRIX_H

#include <math.h>

#include "point3d.h"

/**
  * Class Matrix
  * This structure makes it possible to manage the two-dimensional array. 
  * There is in more some special functions in tables 4x4 of 
  * 3D transformations.
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
   * @param rows The Number of rows
   * @param cols The Number of columns
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
   * @param *this The object to be destroy
   */
  void  Matrix_destroy (Matrix *this);
    
  
  /**
   * Return the element
   * @param *this The matrix
   * @param row The row where to seek the element 
   * @param col The column where to seek the element
   */
  gdouble  Matrix_get (Matrix *this, guint row, guint col);
    
  
  /**
   * Return a pointer on the modifiable area/box
   * @param *this The matrix
   * @param row The row where to seek the element 
   * @param col The column where to seek the element
   */
  gdouble*  Matrix_set (Matrix *this, guint row, guint col);
    
  
  /**
   * Copy a matrix in another one
   * @param *src The source to be copied
   * @param *dst The destination matrix (it will be erased)
   */
  void  Matrix_copy (Matrix *src, Matrix *dst);
    
  
  /**
   * Put to zero the matrix
   * @param *this The matrix
   */
  void  Matrix_setZero (Matrix *this);
    
  
  /**
   * Transform into an identity matrix (Warning: cols == rows) 
   * @param *this  The matrix
   */
  void  Matrix_setIdentity (Matrix *this);
    
  
  /**
   * Add a rotation in radian to the matrix 
   * @param *this the matrix
   * @param rad The angle in radian
   */
  void  Matrix_rotX (Matrix *this, gdouble rad);
    
  
  /**
   * Add a rotation in radian to the matrix 
   * @param *this the matrix
   * @param rad The angle in radian
   */
  void  Matrix_rotY (Matrix *this, gdouble rad);
    
  
  /**
   * Add a rotation in radian to the matrix 
   * @param *this the matrix
   * @param rad The angle in radian
   */
  void  Matrix_rotZ (Matrix *this, gdouble rad);
    
  
  /**
   * Add translation (x, y, z) to the matrix 
   * @param *this The matrix
   * @param x Displacement in x 
   * @param y Displacement in y
   * @param z Displacement in z 
   */
  void  Matrix_translation (Matrix *this, gdouble x, gdouble y, gdouble z);
    
  
  /**
   * Function makes the  product of two matrix. 
   * The result will be in the first parameter. 
   * @param *this The matrix which will receive the product of both 
   * @param *mat  The other matrix
   */
  void  Matrix_product (Matrix *this, Matrix *mat);
    
  
  /**
   * Makes the transformation of the point. 
   * @param *this The transform matrix
   * @param *src The source point which will be transformed
   * @param *dst the resulting point
   */
  void  Matrix_transform (Matrix *this, const Point3D *src, Point3D *dst);
    
  
#endif //MATRIX_H
