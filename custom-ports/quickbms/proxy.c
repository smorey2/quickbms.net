
char *proxy_ver() {
    #include "quickbms_ver.h"
    sprintf(VER, "%d.%d.%d%c", QUICKBMS_VER);
    return VER;
}