/************************************************************************
* Fichier          : perspectivecreator.c
* Date de Creation : lun aoû 16 2004
* Auteur           : Ronan Billon
* E-mail           : cirdan@mail.berlios.de

This file was generated on lun aoû 16 2004 at 16:14:00 with umbrello
**************************************************************************/

#include "engine/perspectivecreator.h"
#undef G_LOG_DOMAIN
#define G_LOG_DOMAIN "PerspectiveCreator"

#define max(a,b)	(((a) > (b)) ? (a) : (b))
#define min(a,b)	(((a) < (b)) ? (a) : (b))

static gint doubleCmp(gconstpointer a, gconstpointer b);
static void  lowest (GArray *x, GArray *y, gint n, gdouble xs, 
	      gdouble *ys, gint nleft, gint nright, GArray *w, 
	      gboolean userw, GArray *rw, gboolean *ok);
static void  lowess (GArray *x, GArray *y, gint n, gdouble f, 
	      gint nsteps, gdouble delta, GArray *ys, 
	      GArray *rw, GArray *res);
static void regression(GArray *x, GArray *y, GArray *ys);

/* A function to test the behavior of the class */
void test(PerspectiveCreator *this)
{
  Point2D *pt2D = NULL;
  Point3D *pt3D = NULL;
  Point2D *orig2D = Point2D_new(0,0);
  Point3D *orig3D = Point3D_new(0.0, 2.0, 5.0);

  g_return_if_fail(this != NULL);
  this->focal = 535;
  this->distCamera = 15;
  this->angleCamera = 30;
  this->CCDwidth = 320;
  this->CCDheight = 240;

  pt3D = PerspectiveCreator_pixelToReal(this, orig2D);
  pt2D = PerspectiveCreator_realToPixel(this, pt3D);

  g_message("pixel -> real -> pixel");
  g_message("[%d,%d]", orig2D->x, orig2D->y);
  g_message("[%d,%d]", pt2D->x, pt2D->y);
  Point3D_destroy(pt3D);
  Point2D_destroy(pt2D);

  pt2D = PerspectiveCreator_realToPixel(this, orig3D);
  pt3D = PerspectiveCreator_pixelToReal(this, pt2D);

  g_message("real -> pixel -> real :");
  g_message("[%f,%f,%f]", 
	    orig3D->x, orig3D->y, orig3D->z);
  
  g_message("[%f,%f,%f]",
	    pt3D->x, pt3D->y, pt3D->z);
  Point3D_destroy(pt3D);
  Point2D_destroy(pt2D);
  Point3D_destroy(orig3D);
  Point2D_destroy(orig2D);
}

/**
 * Constructor
 */
PerspectiveCreator*  PerspectiveCreator_new ()
{
  /* Variables and pre-cond */
  PerspectiveCreator* ret = (PerspectiveCreator*) g_malloc(sizeof(PerspectiveCreator));
  /* Code */
  ret->focal = 0;
  ret->distCamera = 0.0;
  ret->angleCamera = 0.0;
  ret->CCDwidth = 0;
  ret->CCDheight = 0;

  return ret;
}

  

/**
 * Destructor
 * @param *this The object to be destroyed
 */
void  PerspectiveCreator_destroy (PerspectiveCreator *this)
{
  /* Variables and pre-cond */
  g_assert(this != NULL);
  /* Code */
  g_free(this);
}

  

/**
 * Computation of a 3D profile starting from a set of 2D points
 * @param *this The object which perform the computation
 * @param *points2D The 2D points to be projected in 3d 
 */
GSList*  PerspectiveCreator_compute (PerspectiveCreator *this, GSList *points2D)
{
  /* Variables and pre-cond */
  GSList *ret = NULL;
  GSList *iter = NULL;
  g_return_val_if_fail(this != NULL, NULL);
  g_return_val_if_fail((g_slist_length(points2D) != 0), NULL);
  /* Code */
  iter = points2D;
  while(iter != NULL) {
    Point2D *pt2D = (Point2D*) iter->data;
    Point3D *pt3D = PerspectiveCreator_pixelToReal(this, pt2D);
    ret = g_slist_prepend(ret, pt3D);
    iter = g_slist_next(iter);
  }

  return ret;
}

  

/**
 * Computation of a 3D profile starting from a set of 2D points (this function tries to smooth the result)
 * @param *this The object which perform the computation
 * @param *points2D The 2D points to be projected in 3D
 */
GSList*  PerspectiveCreator_computeSmooth (PerspectiveCreator *this, GSList *points2D)
{
  /* Variables and pre-cond */
  GSList *ret = NULL;
  GSList *iter = NULL;
  GArray *y = g_array_new(FALSE,TRUE,sizeof(gdouble));
  GArray *z = g_array_new(FALSE,TRUE,sizeof(gdouble));
  GArray *zs = g_array_new(FALSE,TRUE,sizeof(gdouble));
  gdouble tmp = 0.0;
  guint cpt = 0;
  g_return_val_if_fail(this != NULL, NULL);
  g_return_val_if_fail((g_slist_length(points2D) != 0), NULL);
  /* Code */
  /* it must be more than 15 points to compute smooth line */
  if(g_slist_length(points2D) < 15) {
    g_array_free(zs,TRUE);
    g_array_free(z,TRUE);
    g_array_free(y,TRUE);
    return PerspectiveCreator_compute(this, points2D);
  }

  iter = points2D;
  while(iter != NULL) {
    Point2D *pt2D = (Point2D*) iter->data;
    Point3D *pt3D = PerspectiveCreator_pixelToReal(this, pt2D);
    g_array_prepend_val(y,pt3D->y);
    g_array_prepend_val(z,pt3D->z);
    Point3D_destroy(pt3D);
    iter = g_slist_next(iter);
  }
  
  g_array_prepend_val(y,tmp);
  g_array_prepend_val(z,tmp);
  g_array_set_size(zs,y->len);
  regression(y, z, zs);
  cpt = 1;
  while(cpt < y->len) {
    Point3D *pt = Point3D_new(0.0,0.0,0.0);
    pt->y = g_array_index(y,gdouble,cpt);
    pt->z = g_array_index(zs,gdouble,cpt);
    ret = g_slist_append(ret, pt);
    cpt++;
  }
  g_array_free(zs,TRUE);
  g_array_free(z,TRUE);
  g_array_free(y,TRUE);

  return ret;
}

  

/**
 * It creates a 3D point from a 2D point of the picture (don't forget to free the new point)
 * @param *this The object which perform the computation
 * @param *pt The point to be transformed
 */
Point3D*  PerspectiveCreator_pixelToReal (PerspectiveCreator *this, Point2D *pt)
{
  /* Variables and pre-cond */
  gdouble angle = 0.0;
  gdouble alpha = 0.0;
  gdouble longueur = 0.0;
  gdouble Y = 0.0;
  gdouble Z = 0.0;
  Point3D *ret = NULL;
  gdouble w_2 = 0.0;
  gdouble h_2 = 0.0;
  gdouble f = 0.0;
  gdouble x = 0.0;
  gdouble y = 0.0;
  g_return_val_if_fail(pt != NULL, NULL);
  g_return_val_if_fail(this != NULL, NULL);
  g_return_val_if_fail(this->focal > 0, NULL);
  g_return_val_if_fail(this->distCamera > 0, NULL);
  g_return_val_if_fail(this->angleCamera > 0, NULL);
  g_return_val_if_fail(this->CCDwidth > 0, NULL);
  g_return_val_if_fail(this->CCDheight > 0, NULL);
  g_return_val_if_fail(pt->x <= this->CCDwidth, NULL);
  g_return_val_if_fail(pt->y <= this->CCDheight, NULL);
  /* Code */
  w_2 = (gdouble) this->CCDwidth / 2.0;
  h_2 = (gdouble) this->CCDheight / 2.0;
  f = (gdouble) this->focal;
  x = (gdouble) pt->x;
  y = (gdouble) pt->y;
  angle = (this->angleCamera * M_PI) / 180.0;
  /* Compute the depth */
  if(x < w_2) {
    alpha = atan( (w_2 - x) / f );
    Z = tan(angle+alpha) * this->distCamera;
  }
  else {
    alpha = atan( (x - w_2) / f );
    Z = tan(angle - alpha) * this->distCamera;
  }
  /* Compute the height */
  longueur = sqrt( (this->distCamera)*(this->distCamera) + Z*Z );
  Y = ((h_2 - y) * longueur) / f;
  ret = Point3D_new(0.0, Y, Z);

  return ret;
}

  

/**
 * It creates a 2D point from a 3D point (don't forget to free the new point)
 * @param *this The object which perform the computation
 * @param *pt The object to be transformed
 */
Point2D*  PerspectiveCreator_realToPixel (PerspectiveCreator *this, Point3D *pt)
{
  /* Variables and pre-cond */
  gdouble angle = 0.0;
  gdouble longueur = 0.0;
  guint x = 0;
  guint y = 0;
  Point2D *ret = NULL;
  gdouble w_2 = 0.0;
  gdouble h_2 = 0.0;
  gdouble f = 0.0;
  gdouble Y = 0.0;
  gdouble Z = 0.0;
  gdouble alpha = 0.0;
  g_return_val_if_fail(pt != NULL, NULL);
  g_return_val_if_fail(this != NULL, NULL);
  g_return_val_if_fail(this->focal > 0, NULL);
  g_return_val_if_fail(this->distCamera > 0, NULL);
  g_return_val_if_fail(this->angleCamera > 0, NULL);
  g_return_val_if_fail(this->CCDwidth > 0, NULL);
  g_return_val_if_fail(this->CCDheight > 0, NULL);
  /* Code */
  w_2 = (gdouble) this->CCDwidth / 2.0;
  h_2 = (gdouble) this->CCDheight / 2.0;
  f = (gdouble) this->focal;
  Y = (gdouble) pt->y;
  Z = (gdouble) pt->z;
  angle = (this->angleCamera * M_PI) / 180.0;
  
  /* Compute the x coordinate */
  alpha = atan( Z / this->distCamera);
  if(alpha > angle) {
    x = (guint) (w_2 - (tan(alpha - angle) * f));
  }
  else {
    x = (guint) (w_2 + (tan(angle - alpha) * f));
  }
  /* Compute the y coordinate */
  longueur = sqrt( Z*Z + (this->distCamera)*(this->distCamera) );
  y = (guint) (h_2 - (Y * f) / longueur);
  /* Post-conditions */
  g_return_val_if_fail(x <= this->CCDwidth, NULL);
  g_return_val_if_fail(y <= this->CCDheight, NULL);

  ret = Point2D_new(x,y);
  return ret;
}

/* Comparison between two double */
static gint doubleCmp(gconstpointer a, gconstpointer b)
{
  gdouble *tmp_a = (gdouble*) a;
  gdouble *tmp_b = (gdouble*) b;
  
  if(*tmp_a < *tmp_b) {
    return -1;
  }
  else if(*tmp_a == *tmp_b) {
    return 0;
  }
  else {
    return 1;
  }
}

static void regression(GArray *x, GArray *y, GArray *ys)
{
  GArray *rw =  g_array_new(FALSE,TRUE,sizeof(gdouble));
  GArray *res =  g_array_new(FALSE,TRUE,sizeof(gdouble));
  g_array_set_size(rw,x->len);
  g_array_set_size(res,x->len);
  lowess(x,y,x->len,0.03,2,0.0,ys,rw,res);
  g_array_free(res,TRUE);
  g_array_free(rw,TRUE);
}

/*
 * Lowess calcule le lissage du tableau de Y en utilisant la regression local pondérer
 * @param *x Le tableau de x (gdouble)
 * @param *y Le tableau de y (gdouble)
 * @param n Le nombre d'élément dans les tableaux
 * @param f Détermine le niveau de lissage (entre 0.0 et 1.0)
 * @param nsteps Le nombre d'itération pour la regression robuste
 * @param delta Pas compris comment ça marche (defaut=0.0)
 * @param *ys Le tableau comptenant les 'y' lissé
 * @param *rw Retour des poids pour chaque point 
 * @param *res residus : Y(I)-YS(I)
 */
static void  lowess (GArray *x, GArray *y, gint n, gdouble f, gint nsteps, gdouble delta, GArray *ys, GArray *rw, GArray *res)
{
  gint m1,m2,ns;
  gint iter, i, j, nright, nleft, last;
  gdouble cut, cmad, r, d1, d2;
  gdouble c1, c9, alpha, denom;
  gdouble *ys_index;
  gboolean ok;
  
  if(n<2) {
    ys_index = &g_array_index(ys,gdouble,1);
    *ys_index = g_array_index(y,gdouble,1);
    return;
  }
  m1 = (int) floor(f * (float) n);
  m2 = min(m1,n);
  ns = max(m2,2);

  for(iter=1; iter<=nsteps+1; iter=iter+1){
    nleft = 1; 
    nright = ns;
    last = 0;
    i = 1;
    do {
      while(nright<n){
	d1 = g_array_index(x,gdouble,i) - g_array_index(x,gdouble,nleft);
	d2 = g_array_index(x,gdouble,(nright+1)) - g_array_index(x,gdouble,i);
	if (d1<=d2) {
	  break;
	}
	nleft = nleft+1;
	nright = nright+1;
      }
      lowest(x,y,n,g_array_index(x,gdouble,i),&g_array_index(ys,gdouble,i),nleft,nright,res,(iter>1),rw,&ok);
      
      if (!ok) {
	ys_index = &g_array_index(ys,gdouble,i);
	*ys_index = g_array_index(y,gdouble,i);
      }
      if (last<i-1) {
	denom = g_array_index(x,gdouble,i)-g_array_index(x,gdouble,last);
	for(j=last+1; j<i; j=j+1) {
	  ys_index = &g_array_index(ys,gdouble,j);
	  alpha = (g_array_index(x,gdouble,j)-g_array_index(x,gdouble,last))/denom;
	  *ys_index = alpha*g_array_index(ys,gdouble,i)+(1.0-alpha)*g_array_index(ys,gdouble,last);
	}
      }
      last = i;
      cut = g_array_index(x,gdouble,last)+delta;
      for(i=last+1; i<=n; i=i+1){
	if (g_array_index(x,gdouble,i)>cut) {
	  break;
	}
	if(g_array_index(x,gdouble,i)==g_array_index(x,gdouble,last)){
	  ys_index = &g_array_index(ys,gdouble,i);
	  *ys_index = g_array_index(ys,gdouble,last);
	  last = i;
	}
      }
      i=max(last+1,i-1);
    } while(last < n);

    for(i=1;i<=n;i++) {
      gdouble *res_index = &g_array_index(res,gdouble,i);
      *res_index = g_array_index(y,gdouble,i)-g_array_index(ys,gdouble,i);
    }
    if (iter>nsteps) {
      break;
    }
    for(i=1;i<=n;i++) {
      gdouble *rw_index = &g_array_index(rw,gdouble,i);
      *rw_index = fabs(g_array_index(res,gdouble,i));
    }
    g_array_sort(rw,doubleCmp);

    m1 = (int) floor((float) 1 +  ((float) n) / 2.0);
    m2 = n-m1+1;
    cmad = 3.0*(g_array_index(rw,gdouble,m1)+g_array_index(rw,gdouble,m2));
    c9 = .999*cmad; 
    c1 = .001*cmad;
    for(i=1;i<=n;i++) {
      gdouble *rw_index = &g_array_index(rw,gdouble,i);
      r = fabs(g_array_index(res,gdouble,i));
      if(r<=c1) {
	*rw_index = 1.0;
      } else if(r>c9) {
	*rw_index=0.0;
      }
      else {
	*rw_index = pow((1.0-pow((r/cmad),2)),2);
      }
    }
  }
}

  

/*
 * Lowest est une fonction utilisé pour calculer le lissage d'un point
 * @param *x Le tableau de x (premier élément=0)
 * @param *y Le tableau de y, premier élément=0
 * @param n Le nombre d'élément du tableau
 * @param xs La valeur en absisse de la zone à lisser
 * @param *ys Valeur de retour lissé
 * @param nleft L'index du premier point à tenir compte pour le lissage
 * @param nright index du dernier point à prendre en compte
 * @param *w retourne le poids pour les y 
 * @param userw détermine si le poids sera utilisé
 * @param *rw Les poids
 * @param *ok retourne si le calcul a été effectué
 */
static void  lowest (GArray *x, GArray *y, gint n, gdouble xs, gdouble *ys,gint nleft, gint nright, GArray *w, gboolean userw, GArray *rw, gboolean *ok)
{
  gint nrt, j;
  gdouble a, b, c, h, r;
  gdouble h1, h9, range;
  gdouble *w_j;
  gdouble m1,m2;

  range = g_array_index(x,gdouble,n) - g_array_index(x,gdouble,1);
  m1 = xs - g_array_index(x,gdouble,nleft);
  m2 = g_array_index(x,gdouble,nright) - xs;
  h = max(m1,m2);
  h9 = .999*h;
  h1 = .001*h;
  a = 0.0;

  for(j=nleft; j<=n; j=j+1) {
    w_j = &g_array_index(w,gdouble,j);
    *w_j=0.0;
    r = fabs(g_array_index(x,gdouble,j) - xs);
    if (r<=h9) {
      if (r>h1) {
	*w_j = pow((1.0-pow(r/h,3)),3);
      }
      else{
	*w_j = 1.0;
      }
      if (userw) {
	*w_j = g_array_index(rw,gdouble,j)*g_array_index(w,gdouble,j);
      }
      a = a+g_array_index(w,gdouble,j);
    }
    else if(g_array_index(x,gdouble,j) > xs) {
      break;
    }
  }
  nrt=j;
  if (a<=0.0) {
    *ok = FALSE;
  }
  else {
    *ok = TRUE;
    for(j=nleft;j<nrt;j++) {
      w_j = &g_array_index(w,gdouble,j);
      *w_j = (*w_j)/a;
    }
    
    if (h>0.0) {
      a = 0.0;
      for(j=nleft;j<nrt;j++) {
	a = a+ g_array_index(w,gdouble,j) * g_array_index(x,gdouble,j);
      }
      b = xs-a;
      c = 0.0;
      for(j=nleft;j<nrt;j++) {
	c = c+ g_array_index(w,gdouble,j) * pow(( g_array_index(x,gdouble,j) - a),2);
      }
      if(sqrt(c) > (0.001*range)) {
	b = b/c;
	for(j=nleft;j<nrt;j++) {
	  w_j = &g_array_index(w,gdouble,j);
	  *w_j = (*w_j)*(1.0+b*(g_array_index(x,gdouble,j)-a));
	}
      }
    }
    *ys = 0.0;
    for(j = nleft;j<nrt;j++) {
      *ys = *ys + g_array_index(w,gdouble,j) * g_array_index(y,gdouble,j);
    }
  }
}
