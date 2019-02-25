#ifndef _LINUX_KLAPSE_H
#define _LINUX_KLAPSE_H

// from mdss kcal
extern void kcal_klapse_push(int r, int g, int b);

/* Required variables for external access. Change as per use */
extern void set_rgb_slider(u32 bl_lvl);

int red, green, blue;

#define K_RED    red
#define K_GREEN  green
#define K_BLUE   blue

#endif  /* _LINUX_KLAPSE_H */
