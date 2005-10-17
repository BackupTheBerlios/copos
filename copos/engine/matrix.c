/************************************************************************
* Fichier          : matrix.c
* Date de Creation : Thu Sep 29 2005
* Auteur           : Ronan Billon
* E-mail           : cirdan@mail.berlios.de

This file was generated with umbrello
**************************************************************************/

#include "engine/matrix.h"
#undef G_LOG_DOMAIN
#define G_LOG_DOMAIN "Matrix"

/**
 * Basic constructor
 * @param rows The number of rows
 * @param cols The number of columns
 */
Matrix*  Matrix_new (guint rows, guint cols)
{
  /* Variables and pre-cond */
   Matrix *ret;
  if(rows == 0 || cols == 0) {
    return NULL;
  }
  /* Code */
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
  /* Variables and pre-cond */
  Matrix *ret = Matrix_new(4,4);
  /* Code */
  Matrix_setIdentity(ret);
  return ret;
}

  

/**
 * Create a copy of the matrix
 * @param *src The matrix to be cloned
 */
Matrix*  Matrix_clone (Matrix *src)
{
  /* Variables and pre-cond */
  Matrix *ret;
  /* Code */
  ret = (Matrix*) g_malloc(sizeof(Matrix));
  ret->rows = src->rows;
  ret->cols = src->cols;
  ret->data = (double*) g_malloc(sizeof(double)*ret->rows*ret->cols);
  memcpy(ret->data, src->data, ret->rows*ret->cols*sizeof(gdouble));
  return ret;
}

  

/**
 * Destructor
 */
void  Matrix_destroy (Matrix *this)
{
  /* Variables and pre-cond */
  g_assert(this != NULL);
  g_assert(this->data != NULL);
  /* Code */
  g_free(this->data);
  g_free(this);
}

  

/**
 * Return the element
 * @param row The row where to seek the element
 * @param col The column where to seek the element
 */
gdouble  Matrix_get (Matrix *this, guint row, guint col)
{
  /* Variables and pre-cond */
  g_assert(this != NULL);
  g_assert(row < this->rows && col < this->cols);
  /* Code */
  return this->data[this->cols*row + col];
}

  

/**
 *  Return a pointer on the modifiable area/box
 * @param row The row where to seek the element 
 * @param col The column where to seek the element
 */
gdouble*  Matrix_set (Matrix *this, guint row, guint col)
{
  /* Variables and pre-cond */
  g_assert(this != NULL);
  g_assert(row < this->rows && col < this->cols);
  /* Code */
  return &(this->data[this->cols*row + col]);
}

  

/**
 * Copy a matrix in another one
 * @param *src The source to be copied
 * @param *dst The destination Matrix_matrix (Matrix *this, it will be erased)
 */
void  Matrix_copy (Matrix *src, Matrix *dst)
{
  /* Variables and pre-cond */
  g_assert(src != NULL);
  g_assert(dst != NULL);
  g_assert((src->rows == dst->rows) && (src->cols == dst->cols));
  /* Code */
  memcpy(dst->data, src->data, src->rows*src->cols*sizeof(gdouble));
}

  

/**
 * Put all elements to zero
 */
void  Matrix_setZero (Matrix *this)
{
  /* Variables and pre-cond */
  guint i = 0;
  g_assert(this != NULL);
  /* Code */
  while(i<(this->rows*this->cols)) {
    this->data[i] = 0;
    ++i;
  }
}

  

/**
 * Transform into an identity Matrix_matrix (Matrix *this, Warning: cols == rows) 
 */
void  Matrix_setIdentity (Matrix *this)
{
  /* Variables and pre-cond */
  guint i = 0;
  g_assert(this != NULL);
  g_assert(this->rows == this->cols);
  /* Code */
  Matrix_setZero(this);
  while(i<this->rows) {
    *Matrix_set(this,i,i) = 1.0;
    ++i;
  }
}

  

/**
 * Add a rotation in radian to the matrix 
 * @param rad The angle in radian
 */
void  Matrix_rotX (Matrix *this, gdouble rad)
{
  /* Variables and pre-cond */
  g_assert(this != NULL);
  Matrix* temp = Matrix_clone(this);
  /* Code */
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
 * @param rad The angle in radian
 */
void  Matrix_rotY (Matrix *this, gdouble rad)
{
  /* Variables and pre-cond */
  g_assert(this != NULL);
  Matrix* temp = Matrix_clone(this);
  /* Code */
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
 * @param rad The angle in radian
 */
void  Matrix_rotZ (Matrix *this, gdouble rad)
{
  /* Variables and pre-cond */
  g_assert(this != NULL);
  Matrix* temp = Matrix_clone(this);
  /* Code */
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
 * Add Matrix_translation (Matrix *this, x, y, z) to the matrix 
 * @param x Displacement in x 
 * @param y Displacement in y
 * @param z Displacement in z
 */
void  Matrix_translation (Matrix *this, gdouble x, gdouble y, gdouble z)
{
  /* Variables and pre-cond */
  Matrix* temp = Matrix_clone(this);
  g_assert(this != NULL);
  /* Code */
  Matrix_setIdentity(this);
  *Matrix_set(this,3,0) += x;
  *Matrix_set(this,3,1) += y;
  *Matrix_set(this,3,2) += z;
  Matrix_product(this, temp);
  Matrix_destroy(temp);
}

  

/**
 * This function makes the  product of two matrix
 * @param *mat The matrix to be the argument of the product
 */
void  Matrix_product (Matrix *this, Matrix *mat)
{
  /* Variables and pre-cond */
  unsigned int i,j,k;
  Matrix* result = Matrix_new4x4();
  g_assert(this != NULL);
  /* Code */
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
 * It makes the transformation of a point
 * @param *src The source point which will be transformed
 * @param *dst the resulting point
 */
void  Matrix_transform (Matrix *this, Point3D *src, Point3D *dst)
{
  /* Variables and pre-cond */
  gdouble x = src->x;
  gdouble y = src->y;
  gdouble z = src->z;
  g_assert(this != NULL);
  /* Code */
  dst->x = x*Matrix_get(this,0,0) + y*Matrix_get(this,1,0) 
    + z*Matrix_get(this,2,0) + Matrix_get(this,3,0);
  dst->y = x*Matrix_get(this,0,1) + y*Matrix_get(this,1,1) 
    + z*Matrix_get(this,2,1) + Matrix_get(this,3,1);
  dst->z = x*Matrix_get(this,0,2) + y*Matrix_get(this,1,2) 
    + z*Matrix_get(this,2,2) + Matrix_get(this,3,2);
}

  

