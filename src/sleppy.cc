
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <alsa/asoundlib.h>

//#define BIT_RATE 8000
#define BIT_RATE 11025
#define BUFFER_SIZE 88200
#define LPF_ALPHA 0.75

int main(int argc, char *argv[]) {
  int err;
  snd_pcm_t *playback_handle;
  snd_pcm_hw_params_t *hw_params;

  if ((err = snd_pcm_open(&playback_handle, argv[1], SND_PCM_STREAM_PLAYBACK, 0 /* blocking */)) < 0) {
    fprintf(stderr, "cannot open audio device %s (%s)\n", argv[1], snd_strerror(err));
    return -1;
  }
     
  if ((err = snd_pcm_hw_params_malloc(&hw_params)) < 0) {
    fprintf(stderr, "cannot allocate hardware parameter structure (%s)\n", snd_strerror(err));
    return -1;
  }
       
  if ((err = snd_pcm_hw_params_any(playback_handle, hw_params)) < 0) {
    fprintf(stderr, "cannot initialize hardware parameter structure (%s)\n", snd_strerror(err));
    return -1;
  }

  if ((err = snd_pcm_hw_params_set_access(playback_handle, hw_params,
                                          SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
    fprintf(stderr, "cannot set access type (%s)\n", snd_strerror(err));
    return -1;
  }

  if ((err = snd_pcm_hw_params_set_format(playback_handle, hw_params, SND_PCM_FORMAT_S16_LE)) < 0) {
    fprintf(stderr, "cannot set sample format (%s)\n", snd_strerror(err));
    return -1;
  }

  if ((err = snd_pcm_hw_params_set_rate(playback_handle, hw_params, BIT_RATE, 0)) < 0) {
    fprintf(stderr, "cannot set sample rate (%s)\n", snd_strerror(err));
    return -1;
  }

  if ((err = snd_pcm_hw_params_set_channels(playback_handle, hw_params, 2)) < 0) {
    fprintf(stderr, "cannot set channel count (%s)\n", snd_strerror(err));
    return -1;
  }

  if ((err = snd_pcm_hw_params(playback_handle, hw_params)) < 0) {
    fprintf(stderr, "cannot set parameters (%s)\n", snd_strerror(err));
    return -1;
  }

  snd_pcm_hw_params_free(hw_params);

  if ((err = snd_pcm_prepare(playback_handle)) < 0) {
    fprintf(stderr, "cannot prepare audio interface for use (%s)\n", snd_strerror(err));
    return -1;
  }

  short buf[BUFFER_SIZE * 2];
  while (true) {
    short prev = 0;
    for (int j = 0; j < BUFFER_SIZE * 2; ++j) {
      buf[j] = (short)((rand() % (SHRT_MAX - SHRT_MIN)) + SHRT_MIN);
      buf[j] = prev + (short)(LPF_ALPHA * (buf[j] - prev));
    }
    
    if ((err = snd_pcm_writei(playback_handle, buf, BUFFER_SIZE)) != BUFFER_SIZE) {
      fprintf(stderr, "write to audio interface failed (%d: %s)\n",
         err, snd_strerror(err));
      return -1;
    }
  }

  snd_pcm_close(playback_handle);

  return -1;
}
