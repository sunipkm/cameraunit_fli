/*

  Copyright (c) 2002 Finger Lakes Instrumentation (FLI), L.L.C.
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

        Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.

        Redistributions in binary form must reproduce the above
        copyright notice, this list of conditions and the following
        disclaimer in the documentation and/or other materials
        provided with the distribution.

        Neither the name of Finger Lakes Instrumentation (FLI), LLC
        nor the names of its contributors may be used to endorse or
        promote products derived from this software without specific
        prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
  REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.

  ======================================================================

  Finger Lakes Instrumentation, L.L.C. (FLI)
  web: http://www.fli-cam.com
  email: support@fli-cam.com

*/

#ifndef _LIBFLI_SYS_H
#define _LIBFLI_SYS_H

#include <limits.h>
#include <pthread.h>

#define LIBFLIAPI long

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

typedef struct {
  int fd;
  void *han;
} fli_unixio_t;

typedef struct {
  pthread_mutex_t mutex;
  pthread_mutexattr_t attr;
  long locked;
  long OS;
} fli_unixsysinfo_t;

long unix_fli_connect(flidev_t dev, char *name, long domain);
long unix_fli_disconnect(flidev_t dev);
long unix_fli_lock(flidev_t dev);
long unix_fli_unlock(flidev_t dev);
long unix_fli_trylock(flidev_t dev);
long unix_fli_list(flidomain_t domain, char ***names);

long mac_fli_list(flidomain_t domain, char ***names);
long mac_fli_connect(flidev_t dev, char *name, long domain);
long mac_fli_disconnect(flidev_t dev);
long mac_fli_lock(flidev_t dev);
long mac_fli_unlock(flidev_t dev);
long mac_fli_trylock(flidev_t dev);

#if defined(__linux__)

#define __SYSNAME__ "Linux"
#define __LIBFLI_MINOR__ 999.1
#define USB_READ_SIZ_MAX (1024 * 64)
#define _USE_PTHREAD_LOCK_
//#define _USE_FLOCK_
#define PARPORT_GLOB "/dev/ccd*"
#define USB_GLOB "/dev/fliusb*"
#define SERIAL_GLOB "/dev/ttyS[0-9]*"

#define fli_connect unix_fli_connect
#define fli_disconnect unix_fli_disconnect
#define fli_list unix_fli_list

#elif defined(__FreeBSD__)

#define __SYSNAME__ "FreeBSD"
#define __LIBFLI_MINOR__ 999
#define USB_READ_SIZ_MAX 65536
#define USB_GLOB "/dev/ugen[0-9]"
#define SERIAL_GLOB "/dev/cuaa*"

#define fli_connect unix_fli_connect
#define fli_disconnect unix_fli_disconnect
#define fli_list unix_fli_list

#elif defined (__NetBSD__)

#define __SYSNAME__ "NetBSD"
#define __LIBFLI_MINOR__ 999
#define USB_READ_SIZ_MAX 65536
#define USB_GLOB "/dev/ugen*.0" __STRINGIFY(FLI_USB_CMDENDPOINT)
#define SERIAL_GLOB "/dev/dty0*"

#define fli_connect unix_fli_connect
#define fli_disconnect unix_fli_disconnect
#define fli_list unix_fli_list

#elif defined (__APPLE__)

#define __SYSNAME__ "MacOSX"
#define __LIBFLI_MINOR__ 999
#define USB_READ_SIZ_MAX 65536
#define USB_GLOB "/dev/ugen*.0" __STRINGIFY(FLI_USB_CMDENDPOINT)
#define SERIAL_GLOB "/dev/dty0*"

#if defined(__LIBUSB__)
#define fli_connect unix_fli_connect
#define fli_disconnect unix_fli_disconnect
#define fli_list unix_fli_list
#else
#define fli_connect mac_fli_connect 
#define fli_disconnect mac_fli_disconnect
#define fli_list mac_fli_list
#endif

#define unix_fli_lock mac_fli_lock
#define unix_fli_unlock	mac_fli_unlock
#define unix_fli_trylock mac_fli_trylock

#else
#error "Unknown system"
#endif

#endif /* _LIBFLI_SYS_H */
