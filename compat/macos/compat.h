#ifndef COMPAT_H
#define COMPAT_H

#define st_atim st_atimespec
#define st_ctim st_ctimespec
#define st_mtim st_mtimespec

#define AF_PACKET AF_LINK

#define sockaddr_ll sockaddr_dl
#define SDL_OFFSET (saddl)->sdl_nlen
#define sll_addr sdl_data+SDL_OFFSET

#endif