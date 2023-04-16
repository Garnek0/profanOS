#include <syscall.h>
#include <i_iolib.h>
#include <i_mem.h>

typedef struct {
    int width;
    int height;
    uint32_t *old_framebuffer;
    uint32_t *framebuffer;
    uint32_t *changed_pixels; // coordinates of changed pixels (y * width + x)
    int changed_pixels_count;
} vgui_t;

int main() {
    return 0;
}

void vgui_render(vgui_t *vgui, int render_mode);

vgui_t vgui_setup(int width, int height) {
    vgui_t vgui;
    int buffer_size = width * height * sizeof(uint32_t);
    vgui.width = width;
    vgui.height = height;

    vgui.old_framebuffer = calloc(buffer_size);
    vgui.framebuffer = calloc(buffer_size);
    vgui.changed_pixels = calloc(buffer_size);

    vgui_render(&vgui, 1);
    return vgui;
}

void vgui_exit(vgui_t *vgui) {
    free(vgui->old_framebuffer);
    free(vgui->framebuffer);
    free(vgui->changed_pixels);
}

void vgui_set_pixel(vgui_t *vgui, int x, int y, uint32_t color) {
    if (x < 0 || x >= vgui->width || y < 0 || y >= vgui->height) return;
    uint32_t poss = y * vgui->width + x;
    int already_changed = 0;
    if (vgui->framebuffer[poss] != vgui->old_framebuffer[poss]) {
        already_changed = 1;
    }
    vgui->framebuffer[poss] = color;

    if (vgui->framebuffer[poss] != vgui->old_framebuffer[poss] &&
        !already_changed &&
        vgui->changed_pixels_count < vgui->width * vgui->height
    ) vgui->changed_pixels[vgui->changed_pixels_count++] = poss;
}

uint32_t vgui_get_pixel(vgui_t *vgui, int x, int y) {
    if (x < 0 || x >= vgui->width || y < 0 || y >= vgui->height) return 0;
    return vgui->framebuffer[y * vgui->width + x];
}

void vgui_render(vgui_t *vgui, int render_mode) {
    if (render_mode) {
        for (int i = 0; i < vgui->width * vgui->height; i++) {
            c_vesa_set_pixel(i % vgui->width, i / vgui->width, vgui->framebuffer[i]);
            vgui->old_framebuffer[i] = vgui->framebuffer[i];
        }
    } else {
        int poss;
        for (int i = 0; i < vgui->changed_pixels_count; i++) {
            poss = vgui->changed_pixels[i];
            if (vgui->framebuffer[poss] != vgui->old_framebuffer[poss]) {
                c_vesa_set_pixel(poss % vgui->width, poss / vgui->width, vgui->framebuffer[poss]);
                vgui->old_framebuffer[poss] = vgui->framebuffer[poss];
            }
        }
    }
    vgui->changed_pixels_count = 0;
}

void vgui_draw_rect(vgui_t *vgui, int x, int y, int width, int height, uint32_t color) {
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            vgui_set_pixel(vgui, x + i, y + j, color);
        }
    }
}

void vgui_print(vgui_t *vgui, int x, int y, char msg[], uint32_t color) {
    unsigned char *glyph;
    for (int i = 0; msg[i] != '\0'; i++) {
        glyph = c_font_get(0) + msg[i] * 16;
        for (int j = 0; j < 16; j++) {
            for (int k = 0; k < 8; k++) {
                if (!(glyph[j] & (1 << k))) continue;
                vgui_set_pixel(vgui, i * 8 + x + 8 - k , y + j, color);
            }
        }
    }
}

int abs(int x) {
    return (x < 0) ? -x : x;
}

void vgui_draw_line(vgui_t *vgui, int x1, int y1, int x2, int y2, uint32_t color) {
    // accelerated horizontal and vertical lines
    if (x1 == x2) {
        for (int i = y1; i <= y2; i++) {
            vgui_set_pixel(vgui, x1, i, color);
        }
        return;
    }
    if (y1 == y2) {
        for (int i = x1; i <= x2; i++) {
            vgui_set_pixel(vgui, i, y1, color);
        }
        return;
    }

    // Bresenham's line algorithm
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        vgui_set_pixel(vgui, x1, y1, color);
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

void vgui_clear(vgui_t *vgui, uint32_t color) {
    for (int i = 0; i < vgui->width; i++) {
        for (int j = 0; j < vgui->height; j++) {
            vgui_set_pixel(vgui, i, j, color);
        }
    }
}
