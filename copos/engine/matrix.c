/************************************************************************
* Fichier          : matrix.c
* Date de Creation : jeu mar 25 2004
* Auteur           : Ronan Billon
* E-mail           : cirdan@mail.berlios.de

This file was generated on jeu mar 25 2004 at 21:26:13 with umbrello
**************************************************************************/

#include "engine/matrix.h"
#undef G_LOG_DOMAIN
#define G_LOG_DOMAIN "Storage"

/**
 * Basic constructor
 * @param rows The Number of rows
 * @param cols The Number of columns
 */
Matrix*  Matrix_new (guint rows, guint cols)
{
  Matrix *ret;
  if(rows == 0 || cols == 0) {
    return NULL;
  }
  ret = (Matrix*) g_malloc(sizeof(Matrix));
  ret->rows = rows;
  ret->cols = cols;
  ret->data = (double*) g_malloc(sizeof(double)*(rows*cols));

  Matrix_setZero(ret);
  return ret;
}
    
  
/**
 * Special constructor for a 4x4 matrix
 */
Matrix*  Matrix_new4x4 ()
{
  Matrix *ret = Matrix_new(4,4);
  Matrix_setIdentity(ret);
  return ret;
}
  
/**
 * Create a copy of the matrix
 * @param *src The matrix to be cloned
 */
Matrix*  Matrix_clone (Matrix *src)
{
  unsigned int i;
  Matrix *ret;
  ret = (Matrix*) g_malloc(sizeof(Matrix));
  ret->rows = src->rows;
  ret->cols = src->cols;
  ret->data = (double*) g_malloc(sizeof(double)*ret->rows*ret->cols);
  for(i=0;i<(ret->rows*ret->cols);i++) {
    ret->data[i] = src->data[i];
  }
  return ret;
}
    
  
/**
 * Destructor
 * @param *this The object to be destroy
 */
void  Matrix_destroy (Matrix *this)
{
  g_free(this->data);
  g_free(this);
}

  
/**
 * Return the element
 * @param *this The matrix
 * @param row The row where to seek the element 
 * @param col The column where to seek the element
 */
gdouble  Matrix_get (Matrix *this, guint row, guint col)
{
  g_assert(row < this->rows && col < this->cols);
  return this->data[this->cols*row + col];
}

/**
 * Return a pointer on the modifiable area/box
 * @param *this The matrix
 * @param row The row where to seek the element 
 * @param col The column where to seek the element
 */
gdouble*  Matrix_set (Matrix *this, guint row, guint col)
{
  g_assert(row < this->rows && col < this->cols);
  return &(this->data[this->cols*row + col]);
}    
  
/**
 * Copy a matrix in another one
 * @param *src The source to be copied
 * @param *dst The destination matrix (it will be erased)
 */
void  Matrix_copy (Matrix *src, Matrix *dst)
{
  unsigned int i;
  g_assert((src->rows == dst->rows) && (src->cols == dst->cols));
  for(i=0;i<(src->rows*src->cols);i++) {
    dst->data[i] = src->data[i];
  }
}
    
  
/**
 * Put to zero the matrix
 * @param *this The matrix
 */
void  Matrix_setZero (Matrix *this)
{
  unsigned int i;
  for(i=0;i<(this->rows*this->cols);i++) {
    this->data[i] = 0;
  }
}
    
  
/**
 * Transform into an identity matrix (Warning: cols == rows) 
 * @param *this  The matrix
 */
void  Matrix_setIdentity (Matrix *this)
{
  unsigned int i=0;
  g_assert(this->rows == this->cols);
  Matrix_setZero(this);
  while(i<this->rows) {
    *Matrix_set(this,i,i) = 1.0;
    i++;
  }
}
    
  
/**
 * Add a rotation in radian to the matrix 
 * @param *this the matrix
 * @param rad The angle in radian
 */
void  Matrix_rotX (Matrix *this, gdouble rad)
{
  Matrix* temp = Matrix_clone(this);
  Matrix_setZero(this);
  *Matrix_set(this,0,0) = 1;
  *Matrix_set(this,1,1) = cos(rad);
  *Matrix_set(this,1,2) = sin(rad);
  *Matrix_set(this,2,1) = -sin(rad);
  *Matrix_set(this,2,2) = cos(rad);
  *Matrix_set(this,3,3) = 1;
  Matrix_product(this, temp);
  Matrix_destroy(temp);
}
    
  
/**
 * Add a rotation in radian to the matrix 
 * @param *this the matrix
 * @param rad The angle in radian
 */
void  Matrix_rotY (Matrix *this, gdouble rad)
{
  Matrix* temp = Matrix_clone(this);
  Matrix_setZero(this);
  *Matrix_set(this,0,0) = cos(rad);
  *Matrix_set(this,0,2) = -sin(rad);
  *Matrix_set(this,1,1) = 1;
  *Matrix_set(this,2,0) = sin(rad);
  *Matrix_set(this,2,2) = cos(rad);
  *Matrix_set(this,3,3) = 1;
  Matrix_product(this, temp);
  Matrix_destroy(temp);
}
    
  
/**
 * Add a rotation in radian to the matrix 
 * @param *this the matrix
 * @param rad The angle in radian
 */
void  Matrix_rotZ (Matrix *this, gdouble rad)
{
  Matrix* temp = Matrix_clone(this);
  Matrix_setZero(this);
  *Matrix_set(this,0,0) = cos(rad);
  *Matrix_set(this,0,1) = sin(rad);
  *Matrix_set(this,1,0) = -sin(rad);
  *Matrix_set(this,1,1) = cos(rad);
  *Matrix_set(this,2,2) = 1;
  *Matrix_set(this,3,3) = 1;
  Matrix_product(this, temp);
  Matrix_destroy(temp);
}
    
  
/**
 * Add translation (x, y, z) to the matrix 
 * @param *this The matrix
 * @param x Displacement in x 
 * @param y Displacement in y
 * @param z Displacement in z 
 */
void  Matrix_translation (Matrix *this, gdouble x, gdouble y, gdouble z)
{
  Matrix* temp = Matrix_clone(this);
  Matrix_setIdentity(this);
  *Matrix_set(this,3,0) += x;
  *Matrix_set(this,3,1) += y;
  *Matrix_set(this,3,2) += z;
  Matrix_product(this, temp);
  Matrix_destroy(temp);
}
  
/**
 * Function makes the  product of two matrix. 
 * The result will be in the first parameter. 
 * @param *this The matrix which will receive the product of both 
 * @param *mat  The other matrix
 */
void  Matrix_product (Matrix *this, Matrix *mat)
{
  unsigned int i,j,k;
  Matrix* result = Matrix_new4x4();
  Matrix_setZero(result);
  for(i=0; i<4; i++) {
    for(j=0; j<4; j++) {
      for(k=0; k<4; k++) {
	*Matrix_set(result,i,j) += Matrix_get(this,i,k)*Matrix_get(mat,k,j);
      }
    }
  }
  Matrix_copy(result,this);
  Matrix_destroy(result);
}
    
  
/**
 * Makes the transformation of the point. 
 * @param *this The transform matrix
 * @param *src The source point which will be transformed
 * @param *dst the resulting point
 */
void  Matrix_transform (Matrix *this, const Point3D *src, Point3D *dst)
{
  gdouble x = src->x;
  gdouble y = src->y;
  gdouble z = src->z;

  dst->x = x*Matrix_get(this,0,0) + y*Matrix_get(this,1,0) 
    + z*Matrix_get(this,2,0) + Matrix_get(this,3,0);
  dst->y = x*Matrix_get(this,0,1) + y*Matrix_get(this,1,1) 
    + z*Matrix_get(this,2,1) + Matrix_get(this,3,1);
  dst->z = x*Matrix_get(this,0,2) + y*Matrix_get(this,1,2) 
    + z*Matrix_get(this,2,2) + Matrix_get(this,3,2);
}
