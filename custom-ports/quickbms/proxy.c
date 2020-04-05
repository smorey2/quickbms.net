#define EXPORT __declspec(dllexport)

EXPORT int proxy_test(int a) {
    return a;
}

EXPORT u8 *proxy_ver() {
    return "THISISAMARKER";
    // #include "quickbms_ver.h"
    // sprintf(VER2, "%d.%d.%d%c", QUICKBMS_VER);
    // return VER2;
}

EXPORT i32 proxy_script() { //char *text) {
    i32 cmd = 0;
    bms_init(0);
    cmd = parse_bms(NULL, "", cmd, 0);
    return cmd;
}