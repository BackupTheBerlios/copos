/************************************************************************
* Fichier          : storage.c
* Date de Creation : Thu Sep 29 2005
* Auteur           : Ronan Billon
* E-mail           : cirdan@mail.berlios.de

This file was generated with umbrello
**************************************************************************/

#include "engine/storage.h"
#undef G_LOG_DOMAIN
#define G_LOG_DOMAIN "Storage"

static void convertToRGB24(Storage *this, AVFrame *picture, guchar *image);

/**
 * Constructor
 */
Storage*  Storage_new ()
{
  /* Variables and pre-cond */
  Storage* ret = (Storage*) g_malloc(sizeof(Storage));
  /* Code */
  ret->filename = NULL;
  ret->nbImages = 0;
  ret->formatCtx = NULL;
  ret->codecCtx = NULL;
  ret->streamIndex = -1;
  ret->codecName = NULL;
  ret->hasChangedOrientation = FALSE;
  av_register_all(); /* start ffmpeg */

  return ret;
}

  

/**
 * Destructor
 */
void  Storage_destroy (Storage *this)
{
  /* Variables and pre-cond */
  g_assert(this != NULL);
  /* Code */
  if(this->streamIndex != -1) {
    Storage_close(this);
  }
  if(this->filename != NULL) {
     g_free(this->filename);
  }
  if(this->codecName != NULL) {
     g_free(this->codecName);
  }
  g_free(this);
}

  

/**
 * Set the path to the video file
 * @param *filename the path to the video file
 */
void  Storage_setFileName (Storage *this, const gchar *filename)
{
  /* Variables and pre-cond */
  g_assert(this != NULL);
  g_assert(filename != NULL);
  /* Code */
  this->filename = strdup(filename);
}

  

/**
 * Return all the informations about each image
 * @param *width pointer which receive the width 
 * @param *height pointer which receive the height
 * @param *bytesPerPixel pointer which receive the number of bytes per pixel
 */
void  Storage_getInfos (Storage *this, guint *width, guint *height, guint *bytesPerPixel, gchar *codecName)
{
  /* Variables and pre-cond */
  g_return_if_fail(this != NULL);
  g_return_if_fail(this->streamIndex != -1);
  /* Code */
  if(width != NULL) { 
    if(this->hasChangedOrientation) {
      *width  = this->codecCtx->height;
    }
    else {
      *width  = this->codecCtx->width;
    }
  }
  if(height != NULL) {
    if(this->hasChangedOrientation) {
      *height  = this->codecCtx->width;
    }
    else {
      *height = this->codecCtx->height;
    }
  }
  if(bytesPerPixel != NULL) {*bytesPerPixel = 3;} /*3 by default*/
  if(codecName != NULL) {strcpy(codecName, this->codecName);}
}
 

/**
 * Begin the loading of the video file
 */
gboolean  Storage_load (Storage *this)
{
  /* Variables and pre-cond */
  float framerate = 0;
  guint duration = 0;
  gint i=0;
  AVFormatContext *formatCtx = NULL;
  AVCodecContext *codecCtx;
  AVCodec *codec;
  g_return_val_if_fail(this != NULL, FALSE);
  /* Code */
  if(this->streamIndex != -1) {
    Storage_close(this);
  }
  /* find the correct format context */
  if(av_open_input_file(&formatCtx, this->filename, NULL, 0, NULL)!=0) {
    g_warning("Couldn't open file : %s", this->filename);
    return FALSE;
  }
  if(av_find_stream_info(formatCtx)<0) {
    g_warning("Couldn't get infos of stream");
    return FALSE;
  }
  for(i=0; i<formatCtx->nb_streams; i++) {
    if(formatCtx->streams[i]->codec.codec_type==CODEC_TYPE_VIDEO) {
      this->streamIndex=i;
      break;
    }
  }
  if(this->streamIndex==-1) {
    g_warning("Couldn't find a video stream");
    return FALSE;
  }
  this->formatCtx = formatCtx;
  /* find the correct codec context */
  codecCtx=&formatCtx->streams[this->streamIndex]->codec;
  codec=avcodec_find_decoder(codecCtx->codec_id);
  if(codec==NULL) {
    g_warning("Couldn't find a codec for the stream");
    return FALSE;
  }
  if(codec->capabilities & CODEC_CAP_TRUNCATED)
        codecCtx->flags|=CODEC_FLAG_TRUNCATED;
  if(avcodec_open(codecCtx, codec)<0) {
    g_warning("Couldn't open the codec");
    return FALSE;
  }
  if(codecCtx->frame_rate>1000 && codecCtx->frame_rate_base==1) {
    codecCtx->frame_rate_base=1000;
  }
  this->codecCtx = codecCtx;
  /* Compute the approximately number of frame*/
  framerate = (float)codecCtx->frame_rate / codecCtx->frame_rate_base;
  duration = formatCtx->duration / AV_TIME_BASE;
  this->nbImages = duration * framerate;
  this->codecName = strdup(codec->name);

  return TRUE;
}

  

/**
 * Return the current image and select the next
 * @param *image the returned Storage_image (Storage *this, must be initialized)
 */
gboolean  Storage_get (Storage *this, guchar *image)
{
  /* Variables and pre-cond */
  gboolean ret = FALSE;
  gint frameFinished = 0;
  gint endOfFile = 0;
  AVPacket packet;
  AVFrame *frame=avcodec_alloc_frame();
  g_return_val_if_fail(this != NULL, FALSE);
  g_return_val_if_fail(this->streamIndex != -1, FALSE);
  /* Code */
  while(!frameFinished && !endOfFile) {
     if(av_read_frame(this->formatCtx, &packet)>=0) {
	// Is this a packet from the video stream?
	if(packet.stream_index==this->streamIndex) {
	  // Decode video frame
	  avcodec_decode_video(this->codecCtx, frame, &frameFinished, 
			       packet.data, packet.size);
	}
	av_free_packet(&packet);
     }
     else {
       endOfFile = 1;
     }
  }
  /* Did we get a video frame? */
  if(frameFinished) {
    convertToRGB24(this, frame, image);
    ret = TRUE;
  }
  av_free(frame);
  return ret;
}

  
/**
 * Seek to a frame
 * @param pos pourcentage position of frame in total timeline
 */
gboolean  Storage_seek (Storage *this, guint pos)
{
  /* Variables and pre-cond */
  int64_t duration = this->formatCtx->duration;
  gint durationFactor = 100;
  g_return_val_if_fail(this != NULL, FALSE);
  g_return_val_if_fail(this->streamIndex != -1, FALSE);
  /* Code */
  if(av_seek_frame(this->formatCtx, this->streamIndex, duration * pos / durationFactor) >= 0) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}

/**
 * Test if the object is loaded
 */
gboolean  Storage_isLoaded (Storage *this)
{
  /* Variables and pre-cond */
  g_return_val_if_fail(this != NULL, FALSE);
  /* Code */
  if(this->streamIndex != -1) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}


/**
 * Stop the reading and close the video file
 */
void  Storage_close (Storage *this)
{
  /* Variables and pre-cond */
  g_return_if_fail(this != NULL);
  g_return_if_fail(this->streamIndex != -1);
  /* Code */
  avcodec_close(this->codecCtx);
  av_close_input_file(this->formatCtx);
  this->streamIndex = -1;
}

static void convertToRGB24(Storage *this, AVFrame *picture, guchar *image) {
  /* Variables and pre-cond */
  AVFrame *output=NULL;
  uint8_t *output_buf;
  int size;
  int pix_fmt = PIX_FMT_RGB24;
  g_return_if_fail(this != NULL);
  /* Code */
  output = avcodec_alloc_frame ();
  if (!output) {
    g_warning("Couldn't allocate frame for conversion");
    return;
  }

  size = avpicture_get_size (pix_fmt, 
			     this->codecCtx->width, 
			     this->codecCtx->height);
  output_buf = av_malloc (size);
  if (!output_buf){
    av_free (output);
    g_warning("Couldn't allocate output for conversion");
    return;
  }

  avpicture_fill ((AVPicture *) output, output_buf, pix_fmt, 
		  this->codecCtx->width, this->codecCtx->height);
  img_convert((AVPicture *)output,pix_fmt,
	      (AVPicture *)picture,
	      this->codecCtx->pix_fmt,
	      this->codecCtx->width,
	      this->codecCtx->height);
  
  if(this->hasChangedOrientation) {
    guchar *p, *q;
    guint x, y;
    guint src_width = this->codecCtx->width;
    guint src_height = this->codecCtx->height;
    guint dst_width = 0;
    guint dst_height = 0;

    Storage_getInfos(this, &dst_width, &dst_height, NULL, NULL);
    for (y = 0; y < src_height; y++) { 
      for (x = 0; x < src_width; x++) {
	p = output->data[0] + (src_width * y + x)*3;
	q = image + (dst_width * x + y)*3;
	memcpy (q, p, 3);
      }
    }
  }
  else {
    memcpy(image, output->data[0], size);
  }
  av_free(output_buf);
  av_free(output);
}
