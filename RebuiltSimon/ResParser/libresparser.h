//"D:\\SteamLibrary\\steamapps\\common\\Cry of Fear\\cryoffear\\resource\\trackerscheme.res"
#ifndef LIBRESPARSER_H_
#define LIBRESPARSER_H_
#ifdef __cplusplus
extern "C" {
#endif
    typedef struct {
        char* name;        /* font family / face (null-terminated string) */
        int tall;          /* font size */
        int weight;        /* font weight */
        int dropshadow;    /* boolean 0/1 */
        int yres_min;      /* inclusive min height for this entry */
        int yres_max;      /* inclusive max height for this entry */
    } engine_font_t;

    extern int NPRINTF_Y_MARGIN;
    extern int NPRINTF_FONT_HEIGHT;
    extern int INFO_BACKGROUND_Y_MARGIN;
    extern int INFO_BACKGROUND_WIDTH;

    engine_font_t get_engine_font(int window_height);

#ifdef __cplusplus
}
#endif

#endif /* LIBRESPARSER_H_ */