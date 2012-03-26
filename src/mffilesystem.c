
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <unistd.h>

#include "mffilesystem.h"

const s8 *SLASH = "/";

s8 *mffilesystem_directory = NULL;
size_t mffilesystem_directory_len = 0;

void mffilesystem_setup(void) {
  mffilesystem_directory = getcwd(NULL, 0);
  mffilesystem_directory_len = strlen(mffilesystem_directory);
}

void mffilesystem_shutdown(void) {
  free(mffilesystem_directory);
}

mffile *mffilesystem_open(const s8 *filename, s8 *mode) {
  if (filename == NULL) return NULL;

  size_t file_len = strlen(filename) + 1;
  s8 *fullpath = malloc(sizeof(s8) * file_len);
  if (fullpath == NULL) {
    fprintf(stderr, "Failed allocating space for full path name");
    return NULL;
  } else {
    strncpy(fullpath, mffilesystem_directory, mffilesystem_directory_len);
    strncat(fullpath, SLASH, strlen(SLASH));
    strncat(fullpath, filename, file_len);
  }
  
  errno = 0;
  FILE *f = fopen(fullpath, mode);
  if (f == NULL) {
    fprintf(stderr, "mf filesystem open \"%s\" filed: %s", filename, strerror(errno));
    return NULL;
  }
  
  mffile init = { .file = f, .size = 0 };
  mffile *file = NULL;
  file = (mffile *)malloc(sizeof(mffile));
  if (file == NULL) {
    fprintf(stderr, "mffilesystem_open failed: memory allocation failed\n");
  } else {
    memcpy(file, &init, sizeof(mffile));
  }
  return file;
}

void mffilesystem_close(mffile *file) {
  if (file == NULL) return;

  errno = 0;
  s32 s = fclose(file->file);
  if (s == EOF) {
    perror("File close failed");
  }
  free(file);
}

/*
s32 mffilesystem_write(mffile *file, const s8 *format, ...) {
  
}
*/
