#include <syscall.h>
#include <i_vgui.h>

#define MAX_ITER 64
#define LARGEUR 320
#define HAUTEUR 200

#define XMIN -2.4
#define XMAX 1
#define YMIN -1.25
#define YMAX 1.2

int main(int argc, char **argv) {
    vgui_t vgui = vgui_setup(LARGEUR, HAUTEUR);

    while (1) {
        long double cx, cy, xn, yn, tmp_x, tmp_y;
        int n;

        for (int y = 0; y < HAUTEUR; y++) {
            for (int x = 0; x < LARGEUR; x++) {
                cx = (x * (XMAX - XMIN) / LARGEUR + XMIN);
                cy = (y * (YMIN - YMAX) / HAUTEUR + YMAX);
                xn = 0;
                yn = 0;
                n = 0;
                while ((xn * xn + yn * yn) < 4 && n < MAX_ITER) {
                    if (c_kb_get_scancode() == 1) {
                        vgui_exit(&vgui);
                        return 0;
                    }
                    tmp_x = xn;
                    tmp_y = yn;
                    xn = tmp_x * tmp_x - tmp_y * tmp_y + cx;
                    yn = 2 * tmp_x * tmp_y + cy;
                    n++;
                }
                vgui_set_pixel(&vgui, x, y, n*100);
            }
        }
        vgui_print(&vgui, 5, 5, "press escape to exit", 0xFFFFFF);
        vgui_render(&vgui, 1);
    }
}
