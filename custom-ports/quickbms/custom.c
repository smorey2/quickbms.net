/*
    Copyright 2009-2019 Luigi Auriemma

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

    http://www.gnu.org/licenses/gpl-2.0.txt
*/

#define _WIN32_WINNT    0x0601
#define _WIN32_WINDOWS  0x0601
#define WINVER          0x0601

//#define NOLFS
#ifndef NOLFS   // 64 bit file support not really needed since the tool uses signed 32 bits at the moment, anyway I leave it enabled
    #define _LARGE_FILES        // if it's not supported the tool will work
    #define __USE_LARGEFILE64   // without support for large files
    #define __USE_FILE_OFFSET64
    #define _LARGEFILE_SOURCE
    #define _LARGEFILE64_SOURCE
    #define _FILE_OFFSET_BITS   64
#endif

#ifdef _WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
#include <stdarg.h>
#include <math.h>
#include <inttypes.h>
#include <locale.h>
#include <fcntl.h>
#include "stristr.c"

#include "extra/xalloc.h"
#include "extra/uthash_real_alloc.h"
#include "extra/utlist.h"

// this is the old method used by quickbms to handle short strings.
// currently it has been disabled and will rely entirely on allocated memory instead of static buffers.
// I'm still testing the effects on the performances, QUICKBMS_VAR_STATIC is stable and has ever worked,
// while the other method is slower (frostbite.bms is sloooooow) but doesn't use work-arounds.
// so... do NOT touch it!
#define QUICKBMS_VAR_STATIC

// disabled by default because there are some things that don't convince me
// for example during the disassembling of shellcode_Alpha2.txt
#ifdef ENABLE_BEAENGINE
    #define BEA_ENGINE_STATIC
    #define BEA_USE_STDCALL
    #include <BeaEngine.h>
#endif
#include "capstone/capstone.h"

typedef int8_t      i8;
typedef uint8_t     u8;
//typedef int16_t     i16;
typedef uint16_t    u16;
typedef int32_t     i32;
typedef uint32_t    u32;
//typedef int64_t     i64;
typedef uint64_t    u64;

typedef int8_t      int8;
typedef uint8_t     uint8;
typedef int16_t     int16;
typedef uint16_t    uint16;
typedef int32_t     int32;
typedef uint32_t    uint32;
typedef int64_t     int64;
typedef uint64_t    uint64;
typedef unsigned char   byte;   // for sflcomp
typedef unsigned short  word;   // for sflcomp

#define QUICKBMS
// in case you want to make QuickBMS 64bit compatible
// start
#ifdef QUICKBMS64
    #define INTSZ           64
    #define QUICKBMS_int    int64_t     // trick for forcing the usage of signed 32 bit numbers on any system without modifying the code
    #define QUICKBMS_u_int  uint64_t    // used only in some rare occasions
    #define PRId            PRId64
    #define PRIu            PRIu64
    #define PRIx            "016"PRIx64
#else
    #define INTSZ           32
    #define QUICKBMS_int    int32_t     // trick for forcing the usage of signed 32 bit numbers on any system without modifying the code
    #define QUICKBMS_u_int  uint32_t    // used only in some rare occasions
    #define PRId            PRId32
    #define PRIu            PRIu32
    #define PRIx            "08"PRIx32
#endif
// end
#define PATH_DELIMITERS     "\\/"

#ifdef WIN32
#else
    #define stricmp     strcasecmp
    #define strnicmp    strncasecmp
    //#define stristr     strcasestr
    typedef uint16_t    WORD;
    typedef uint32_t    DWORD;
#endif
#ifndef nullptr
#define nullptr NULL
#endif

int (*real_strcmp) ( const char * str1, const char * str2 ) = strcmp;
int (*real_stricmp) ( const char * str1, const char * str2 ) = stricmp;
int (*real_strncmp) ( const char * str1, const char * str2, size_t num ) = strncmp;
int (*real_strnicmp) ( const char * str1, const char * str2, size_t num ) = strnicmp;
i32 mystrcmp(const char *a, const char *b);
i32 mystricmp(const char *a, const char *b);
i32 mystrncmp(const char *a, const char *b, i32 n);
i32 mystrnicmp(const char *a, const char *b, i32 n);
#define strcmp      mystrcmp
#undef stricmp
#define stricmp     mystricmp
#define strncmp     mystrncmp
#undef strnicmp
#define strnicmp    mystrnicmp

// yeah it's cdecl by default
int /*__cdecl*/ fake_printf(const char *__format, ...) {
    return 0;
}
int /*__cdecl*/ (*backup_real_printf) (const char *__format, ...) = printf;
int /*__cdecl*/ (*real_printf) (const char *__format, ...) = printf;
int /*__cdecl*/ fake_fprintf(FILE *__stream, const char *__format, ...) {
    if((__stream == stdout) || (__stream == stderr)) return 0;
  register int __retval;
  __builtin_va_list __local_argv; __builtin_va_start( __local_argv, __format );
  __retval = /*__mingw_*/vfprintf( __stream, __format, __local_argv );
  __builtin_va_end( __local_argv );
  return __retval;
}
int /*__cdecl*/ (*backup_real_fprintf) (FILE *__stream, const char *__format, ...) = fprintf;
int /*__cdecl*/ (*real_fprintf) (FILE *__stream, const char *__format, ...) = fprintf;
#define printf      real_printf
#define fprintf     real_fprintf

#ifdef WIN32
    //#define __BCRYPT_H__    // necessary for using my implementation, it's a problem caused by bzlib.h and affecting gcc 7
    //#define __NCRYPT_H__
    // long story short gcc 7 will complain about using bcrypt
#endif

#include "quickiso.c"
#include "quickzip.c"
#include <zlib.h>

// or use -DDISABLE_SSL
#ifndef DISABLE_SSL
    // it's useless to enable the following
    //#define OPENSSL_DOING_MAKEDEPEND
    //#define OPENSSL_NO_KRB5
    #include <openssl/crypto.h>
    #include <openssl/ssl.h>
    #if OPENSSL_VERSION_NUMBER >= 0x10100000L
        #include "extra/evp_locl.h"     // this is a temporary work-around
        #include "extra/hmac_lcl.h"     // this is a temporary work-around
        #define HMAC_CTX_init   HMAC_CTX_reset
        #define EVP_MD_CTX_cleanup(X)
        #define HMAC_CTX_cleanup(X)
    #endif
    #include <openssl/ossl_typ.h>
    #include <openssl/evp.h>
    #include <openssl/aes.h>
    #include <openssl/blowfish.h>
    #include <openssl/hmac.h>
    #include "extra/libcrypto-compat.h"
    #include "extra/libcrypto-compat.c"
#endif
#include "encryption/tea.h"
#include "encryption/xtea.h"
#include "encryption/xxtea.h"
#include "myenc.c"
#include "encryption/twofish.h"
#include "encryption/seed.h"
#include "encryption/serpent.h"
#include "encryption/ice.h"
#include "encryption/rotor.c"
//#include "encryption/libkirk/kirk_engine.h"
#include "encryption/sph.h"
#include "encryption/arc4.h"
int kirk_CMD0(u8* outbuff, u8* inbuff, int size, int generate_trash);
int kirk_CMD1(u8* outbuff, u8* inbuff, int size, int do_check);
int kirk_CMD4(u8* outbuff, u8* inbuff, int size);
int kirk_CMD7(u8* outbuff, u8* inbuff, int size);
int kirk_CMD10(u8* inbuff, int insize);
int kirk_CMD11(u8* outbuff, u8* inbuff, int size);
int kirk_CMD14(u8* outbuff, int size);
int kirk_init(); //CMD 0xF?
void xtea_crypt_ecb( xtea_context *ctx, int mode, u8 input[8], u8 output[8] );
#ifndef DISABLE_MCRYPT
    #include <mcrypt.h>
#endif
//#define DISABLE_TOMCRYPT    // useless at the moment
#ifndef DISABLE_TOMCRYPT
    #define USE_LTM
    #define LTC_MECC
    #define LTM_DESC
    #define LTC_SOURCE
    #define LTC_MRSA
    #define LTC_MKAT
    #define LTC_MDH
    #define LTC_MDSA
    #define LTC_DER
    #include <tomcrypt.h>
#endif
void zipcrypto_init_keys(const char* passwd,uint32_t* pkeys,const uint32_t* pcrc_32_tab);
void zipcrypto_decrypt(uint32_t* pkeys,const uint32_t* pcrc_32_tab, unsigned char *data, int datalen);
void zipcrypto_encrypt(uint32_t* pkeys,const uint32_t* pcrc_32_tab, unsigned char *data, int datalen);
int threeway_setkey(unsigned *key, unsigned char *data, int datalen);
void threeway_encrypt(unsigned *key, unsigned char *data, int datalen);
void threeway_decrypt(unsigned *key, unsigned char *data, int datalen);
void skipjack_makeKey(byte key[10], byte tab[10][256]);
void skipjack_encrypt(byte tab[10][256], byte in[8], byte out[8]);
void skipjack_decrypt(byte tab[10][256], byte in[8], byte out[8]);
#include "encryption/anubis.h"
typedef struct { Byte rk[16*17]; int Nr; } aria_ctx_t;
int ARIA_DecKeySetup(const Byte *mk, Byte *rk, int keyBits);
int ARIA_EncKeySetup(const Byte *mk, Byte *rk, int keyBits);
void ARIA_Crypt(const Byte *i, int Nr, const Byte *rk, Byte *o);
u_int *crypton_set_key(const u_int in_key[], const u_int key_len, u_int l_key[104]);
u_int crypton_encrypt(const u_int in_blk[4], u_int out_blk[4], u_int l_key[104]);
u_int crypton_decrypt(const u_int in_blk[4], u_int out_blk[4], u_int l_key[104]);
u_int *frog_set_key(const u_int in_key[], const u_int key_len);
void frog_encrypt(const u_int in_blk[4], u_int out_blk[4]);
void frog_decrypt(const u_int in_blk[4], u_int out_blk[4]);
typedef struct { u_int iv[2]; u_int key[8]; int type; } gost_ctx_t;
void gost_kboxinit(void);
void gostcrypt(u_int const in[2], u_int out[2], u_int const key[8]);
void gostdecrypt(u_int const in[2], u_int out[2], u_int const key[8]);
void gostofb(u_int const *in, u_int *out, int len, u_int const iv[2], u_int const key[8]);
void gostcfbencrypt(u_int const *in, u_int *out, int len, u_int iv[2], u_int const key[8]);
void gostcfbdecrypt(u_int const *in, u_int *out, int len, u_int iv[2], u_int const key[8]);
void lucifer(unsigned char *);
void lucifer_loadkey(unsigned char *, int);
u_int *mars_set_key(u_int key_blk[], u_int key_len);
void mars_encrypt(u_int in_blk[], u_int out_blk[]);
void mars_decrypt(u_int in_blk[], u_int out_blk[]);
void misty1_keyinit(u_int  *ek, u_int  *k);
void misty1_decrypt_block(u_int  *ek,u_int  c[2], u_int  p[2]);
void misty1_encrypt_block(u_int  *ek, u_int  p[2], u_int  c[2]);
typedef struct { u_int k[4]; } NOEKEONstruct;
void NOEKEONkeysetup(const unsigned char * const key, 
                    NOEKEONstruct * const structpointer);
void NOEKEONencrypt(const NOEKEONstruct * const structpointer, 
                   const unsigned char * const plaintext,
                   unsigned char * const ciphertext);
void NOEKEONdecrypt(const NOEKEONstruct * const structpointer,
                   const unsigned char * const ciphertext,
                   unsigned char * const plaintext);
#include "encryption/seal.h"
#include "encryption/safer.h"
int pc1_128(unsigned char *cle, unsigned char *data, int size, int decenc);
int pc1_256(unsigned char *cle, unsigned char *data, int size, int decenc);
uint32_t *rc6_set_key(uint32_t *l_key, const uint32_t in_key[], const uint32_t key_len);
void rc6_encrypt(uint32_t *l_key, const uint32_t in_blk[4], uint32_t out_blk[4]);
void rc6_decrypt(uint32_t *l_key, const uint32_t in_blk[4], uint32_t out_blk[4]);
#include "encryption/isaac.h"
void isaacx_crypt(unsigned char *key, int keylen, unsigned char *data, int datasz, int do_encrypt);
void hsel_crypt(unsigned char *key, unsigned char *data, int size, int do_encrypt, char *options);
void molebox_setkey(u8 *data, u8 *key);
void molebox_decrypt(u8 *key, u8 *data, int datasz);
extern void deskey(unsigned char *, short);
extern void des(unsigned char *, unsigned char *);
#include "encryption/chacha20_simple.h"

#ifdef __DJGPP__
    #define NOLFS
    char **__crt0_glob_function (char *arg) { return 0; }
    void   __crt0_load_environment_file (char *progname) { }
#endif

#define DISABLE_BACKTRACE   // it makes the executable bigger and breaks compatibility with Win98 (_fstat64)

#ifdef WIN32
    #include <windows.h>
    //#include <psapi.h>
    //#include <shlobj.h>
    //#include <tlhelp32.h>
    #include <wincrypt.h>
    #include <direct.h>
    //#include <ddk/ntifs.h>    // I want compatibility even with Win9x
    #include "extra/MemoryModule.h"
    #ifndef DISABLE_BACKTRACE
    #include "extra/backtrace.c"
    #endif

    #define PATHSLASH   '\\'
    #define LOADDLL(X)  LoadLibrary(X)
    #define GETFUNC(X)  (void *)GetProcAddress(hlib, X)
    #define CLOSEDLL    FreeLibrary(hlib)

    char *get_file(char *title, i32 bms, i32 multi);
    char *get_folder(char *title);
#else
    #include <unistd.h>
    #include <dirent.h>
    #include <dlfcn.h>      // -ldl
    #include <sys/mman.h>
    #include <netinet/in.h>

    #ifdef QUICKBMS_STATIC  // not necessary, just in case...
        void *dlopen(const char *filename, int flags) { return NULL; }
        int dlclose(void *handle) { return 0; }
        void *dlsym(void *handle, const char *symbol) { return NULL; }
        char *dlerror(void) { return NULL; }
        int dladdr(const void *addr, void/*Dl_info*/ *info) { return 0; }
    #endif
    #define LOADDLL(X)  dlopen(X, RTLD_LAZY)
    #define GETFUNC(X)  (void *)dlsym(hlib, X)
    #define CLOSEDLL    dlclose(hlib)
    #define HMODULE     void *
    #define GetCurrentProcessId getpid
    #define PATHSLASH   '/'
    #ifdef __APPLE__
        // don't use iconv
    #else
        #define USE_LIBICONV    // -liconv
    #endif
    #define __declspec(X)
#endif

#if defined(_LARGE_FILES)
    #if defined(__APPLE__)
        #define fseek   fseeko
        #define ftell   ftello
    #elif defined(__FreeBSD__)
    #elif !defined(NOLFS)       // use -DNOLFS if this tool can't be compiled on your OS!
        #define off_t   off64_t
        #define fopen   fopen64
        #define fseek   fseeko64
        #define ftell   ftello64
        #define ftruncate   ftruncate64
        #ifndef fstat
            #ifdef WIN32
                #define fstat   _fstati64
                #define stat    _stati64
            #else
                #define fstat   fstat64
                #define stat    stat64
            #endif
        #endif
    #endif
#endif

# ifndef __cdecl 
#  define __cdecl  __attribute__ ((__cdecl__))
# endif
# ifndef __stdcall
#  define __stdcall __attribute__ ((__stdcall__))
# endif
void __cxa_pure_virtual() { while(1); }

#include "threads.h"



static u8   VER[64]     = "";       // kept for compatibility with some functions
#define BUFFSZ          8192
#define MAX_IFS         16          // fixed but exagerated
#define MAX_ARGS        32          // fixed but exagerated
#define MAX_VARS        1024        // fixed but exagerated (name/value_static gives problems with allocated variables)
#define MAX_FILES       1024        // fixed but exagerated, currently the handling of MAX_FILES is uncertain... >= MAX_FILES or > MAX_FILES? both are "correct"
#define MAX_CMDS        4096        // fixed but exagerated
#define MAX_ARRAYS      1024        // fixed but exagerated

#define STRINGSZ        273         // more than MAX_PATH, aligned with +1, 273*15+1 = 4096
#define VAR_VALUE_DELIMITERS    3   // unicode and so on, originally it was just 1
#define NUMBERSZ        24          // ready for 64 bits, includes also space for the NULL delimiter
#define PATHSZ          8192        // 257 was enough, theoretically the system could support 32kb but 1024 is really a lot (more is better due to utf8)
#define ENABLE_DIRECT_COPY

#ifdef QUICKBMS_VAR_STATIC
#define VAR_NAMESZ      STRINGSZ    // 31          // +1 for alignment, 31 for a variable name is perfect
#define VAR_VALUESZ     STRINGSZ    // more than 256 and big enough to contain filenames
#if VAR_NAMESZ < NUMBERSZ
ERROR VAR_NAMESZ < NUMBERSZ
#endif
#endif

#define MYLITTLE_ENDIAN 0
#define MYBIG_ENDIAN    1

#define int             QUICKBMS_int
#define u_int           QUICKBMS_u_int

#define QUICKBMS_DUMMY  "QUICKBMS_DUMMY_TEMP"
#define CMD             g_command[cmd]
#define ARG             argument
#define NUM(X)          CMD.num[X]
#define STR(X)          CMD.str[X]
#define VARVAR(X)       g_variable[CMD.var[X]]
#define VARISNUM(X)     var_is_a_number(CMD.var[X]) //VARVAR(X).isnum
#define VARNAME(X)      get_varname(CMD.var[X])
#define VAR(X)          get_var(CMD.var[X])
#define VAR32(X)        get_var32(CMD.var[X])
#define VARPTR(X)       (CMD.num[X] ? get_var_ptr(CMD.var[X]) : NULL)
#ifdef QUICKBMS_VAR_STATIC
#define VARSZ(X)        VARVAR(X).size   // due to the memory enhancement done on this tool, VARSZ returns ever STRINGSZ for sizes lower than this value... so do NOT trust this value!
#else
#define VARSZ(X)        get_var_fullsz(CMD.var[X])    // causes LOT of problems with static variables, check what happened with quickbms 0.7.2a
#endif
//#define FILEZ(X)        ((NUM(X) < 0) ? NULL : g_filenumber[NUM(X)].fd)  // automatic support for MEMORY_FILE
#define FILEZ(X)        _FILEZ(NUM(X))
#define MEMORY_FNAME    "MEMORY_FILE"
#define MEMORY_FNAMESZ  (sizeof(MEMORY_FNAME) - 1)
#define TEMPORARY_FILE  "TEMPORARY_FILE"
#define ALLOC_ERR       alloc_err(__FILE__, __LINE__, __FUNCTION__)
#define STD_ERR(ERR)    std_err(__FILE__, __LINE__, __FUNCTION__, ERR)
#define MAX_ALLOC_SIZE  (u_int)-1LL // don't use 0xffffffff for compatibility with 64bit integers
#define MAX_ALLOC_CHECK(X)  if((u_int)(X) >= (u_int)0xffffffff) ALLOC_ERR;    // compatibility with 32bit malloc/calloc/realloc

static void FCLOSEX(FILE *X) { if(X && (X != stdout) && (X != stderr) && (X != stdin)) fclose(X); }
#define FCLOSE(X)   { FCLOSEX(X); X = NULL; }   // NULL is very important!
// use FREE instead of free
#define FREE(X)         if(X) { \
                            free(X); \
                            X = NULL; \
                        }
#define FREEX(X,Y)      if(X) { \
                            Y; \
                            FREE(X) \
                        }
// the first 2 checks on fdnum are not necessary
#define fdnum_is_valid  ((fdnum >= 0) && (fdnum < MAX_FILES))
#define CHECK_MEMNUM(X) if(((-fdnum) > MAX_FILES) || ((-fdnum) < 0)) return X;
#define CHECK_FILENUMX  if( \
                            (fdnum < 0) \
                         || (fdnum > MAX_FILES) \
                        ) { \
                            fprintf(stderr, "\nError: the specified file number (%d) is invalid (%s, %d)\n", (i32)fdnum, __FILE__, (i32)__LINE__); \
                            myexit(QUICKBMS_ERROR_BMS); \
                        }
#define CHECK_FILENUM   CHECK_FILENUMX \
                        if( \
                            !g_filenumber[fdnum].fd \
                         && !g_filenumber[fdnum].sd \
                         && !g_filenumber[fdnum].pd \
                         && !g_filenumber[fdnum].ad \
                         && !g_filenumber[fdnum].vd \
                         && !g_filenumber[fdnum].md \
                        ) { \
                            fprintf(stderr, "\nError: the specified file number (%d) has not been opened yet (%s, %d)\n", (i32)fdnum, __FILE__, (i32)__LINE__); \
                            myexit(QUICKBMS_ERROR_BMS); \
                        }
#define myatoi(X)       readbase(X, 10, NULL)
#define CSTRING(X,Y)    { \
                        mystrdup(&CMD.str[X], Y); \
                        CMD.num[X] = cstring(CMD.str[X], CMD.str[X], -1, NULL); \
                        }
#define QUICK_GETi32(X,Y)   (((X)[(Y)])   | ((X)[(Y)+1] << 8) | ((X)[(Y)+2] << 16) | ((X)[(Y)+3] << 24))
#define QUICK_GETb32(X,Y)   (((X)[(Y)+3]) | ((X)[(Y)+2] << 8) | ((X)[(Y)+1] << 16) | ((X)[(Y)]   << 24))
#define QUICK_GETi16(X,Y)   (((X)[(Y)])   | ((X)[(Y)+1] << 8))
#define QUICK_GETb16(X,Y)   (((X)[(Y)+1]) | ((X)[(Y)]   << 8))
#define QUICK_PUTi32(X,Y)   (X)[0]=(Y);      (X)[1]= (Y)>> 8;    (X)[2]= (Y)>>16;     (X)[3]= (Y)>> 24;
#define QUICK_PUTb32(X,Y)   (X)[3]=(Y);      (X)[2]= (Y)>> 8;    (X)[1]= (Y)>>16;     (X)[0]= (Y)>> 24;
#define SCAN_INPUT_FILE_PATH(OUT_BUFF, IN_NAME) \
            switch(path_idx) { \
                case 0:  mypath = g_bms_folder;     break; \
                case 1:  mypath = g_exe_folder;     break; \
                case 2:  mypath = g_file_folder;    break; \
                case 3:  mypath = g_current_folder; break; \
                case 4:  mypath = g_output_folder;  break; \
                case 5:  mypath = ".";              break; \
                default: mypath = NULL;             break; \
            } \
            if(!mypath) break; \
            spr(&OUT_BUFF, "%s%c%s", mypath, PATHSLASH, IN_NAME);

// numbers_to_bytes returns a static buffer so do NOT free it
// NUMS2BYTES(input, input_size, output, output_size)
#define NUMS2BYTES(A,B,C,D,E) { \
                        tmp = numbers_to_bytes(A, &B, 0, E); \
                        myalloc(&C, B, &D); \
                        memcpy(C, tmp, B); \
                        }
#define NUMS2BYTES_HEX(A,B,C,D,E) { \
                        tmp = numbers_to_bytes(A, &B, 1, E); \
                        myalloc(&C, B, &D); \
                        memcpy(C, tmp, B); \
                        }

#define MULTISTATIC     256 // this number is simply the amount of static buffers to use so that
                            // we can use the same function MULTISTATIC times without overlapped results!
#define strdup_dontuse  "Error: do NOT use strdup, use re_strdup or mystrdup!"
char *(*real_strdup)(const char *) = strdup;
#define strdup          strdup_dontuse
#define far
//#define PRINTF64(X)     (i32)(((X) >> 32) & 0xffffffff), (i32)((X) & 0xffffffff)
#define QUICKBMS_CACHED_IO_SIZE (512 * 1024)    // this is probably the best size for optimal speed
#define QUICKBMS_MIN_INT(X)     (((u_int)1 << (u_int)((sizeof(X) * 8) - 1)))
#define QUICKBMS_MAX_INT(X)     (((u_int)1 << (u_int)((sizeof(X) * 8) - 1)) - 1)
#define QUICKBMS_MAX_UINT(X)    (((u_int)1 << (u_int)((sizeof(X) * 8)    )) - 1)

/*
Notes for quickbms 0.9.1:
It may rarely happen that the content of a variable is copied on the same variable for example with "set NAME filename NAME".
For avoiding such issue it's better to use mymemmove instead of memcpy. It's a simple work-around that works on the whole code.
re_strdup uses the same work-around too.
*/
#define STR_MEMCPY  mymemmove



#include "defs.h"



u8 *myitoa(int num);
files_t *add_files(u8 *fname, u64 fsize, QUICKBMS_int *ret_files);
int debug_privileges(void);
int verbose_options(u8 *arg);
u8 *mystrdup_simple(u8 *str);
u8 *mystrdup(u8 **old_buff, u8 *str);
u8 *show_dump(int left, u8 *data, int len, FILE *stream);
int get_parameter_numbers_int(u8 *str, ...);
int get_parameter_numbers_i32(u8 *str, ...);
u64 readbase(u8 *data, QUICKBMS_int size, QUICKBMS_int *readn);
void g_mex_default_init(int file_only);
int start_bms(int startcmd, int nop, int this_is_a_cycle, int *invoked_if, int *invoked_break, int *invoked_continue, u8 **invoked_label);
int check_wildcard(u8 *fname, u8 *wildcard);
int check_wildcards(u8 *fname, u8 **list);
u8 *create_dir(u8 *fname, int mdir, int cdir, int is_path, int filter_bad);
int check_overwrite(u8 *fname, int check_if_present_only);
u8 *myalloc(u8 **data, QUICKBMS_int wantsize, QUICKBMS_int *currsize);
int mymemmove(void *dstx, void *srcx, int size);
void std_err(const char *fname, i32 line, const char *func, signed char error);
void winerr(DWORD error, char *msg);
void myexit(int ret);



// boring 64bit compatibility
#undef int
#undef u_int
#if QUICKBMS_int != 32
    u8 *myalloc32(u8 **data, int wantsize, int *currsize) {
        QUICKBMS_int    lame;
        if(!currsize) {
            myalloc(data, wantsize, NULL);
        } else {
            lame = *currsize;
            myalloc(data, wantsize, &lame);
            *currsize = lame;
        }
        return(*data);
    }
    #define myalloc myalloc32
#endif
#define get_parameter_numbers get_parameter_numbers_i32

// int -> 32
#include "calling_conventions.h"
#include "sign_ext.c"
#include "unz.c"
#include "extra/wcx.c"
#include "extra/window.c"
#include "extra/libtcc.h"
#include "libs/mydownlib/mydownlib.h"
#include "io/sockets.c"
#include "io/process.c"
#include "io/audio.c"
#include "io/video.c"
#include "io/winmsg.c"
#undef myalloc
#define MAINPROG
#include "disasm/disasm.h"
typedef struct t_asmmodel {            // Model to search for assembler command
  uchar          code[MAXCMDSIZE];     // Binary code
  uchar          mask[MAXCMDSIZE];     // Mask for binary code (0: bit ignored)
  int            length;               // Length of code, bytes (0: empty)
  int            jmpsize;              // Offset size if relative jump
  int            jmpoffset;            // Offset relative to IP
  int            jmppos;               // Position of jump offset in command
} t_asmmodel;
int    Assemble(uchar *cmd,ulong ip,t_asmmodel *model,int attempt,
         int constsize,uchar *errtext);
// restore int and u_int after main()



// int -> 32 or 64
#define int             QUICKBMS_int
#define u_int           QUICKBMS_u_int

#undef  get_parameter_numbers
#define get_parameter_numbers get_parameter_numbers_int



extern int g_quickbms_exception_test;
extern int g_insensitive;
extern i32 g_quickbms_argc;
extern char **g_quickbms_argv;
extern char *g_quickbms_arg0;


void quickbms_dll_init2(void) {
    // necessary, in DllMain would be enough, added to other DLL functions just in case
    g_quickbms_dll = 1;
    XDBG_ALLOC_ACTIVE = 0;
}

#include "libs/cryptohash-sha1/sha1.h"
#include "utils_unicode.c"
#include "utils.c"
#include "var.c"
#include "perform.c"
#include "hexhtml.c"
#include "file.c"
#include "cmd.c"
#include "bms.c"
#include "update.c"
#include "help.c"
#include "files_tree.c"
#include "ipc.c"

char *c_ver() {
    #include "quickbms_ver.h"
    sprintf(VER, "%d.%d.%d%c", QUICKBMS_VER);
    return VER;
}