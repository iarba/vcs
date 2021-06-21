#ifndef VCS_H
#define VCS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

void vcs_type(const char *buf, size_t sz);

void vcs_restore();

void vcs_mouse_reset();

void vcs_mouse_abs(int x, int y);

void vcs_mouse_rel(int x, int y);

void vcs_init();
void vcs_destroy();

#ifdef __cplusplus
}
#endif

#endif // VCS_H

