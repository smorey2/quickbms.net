@echo off
SETLOCAL

REM All-in-one bat file for compiling QuickBMS on Windows

REM quickbms_4gb_files:
REM compa.bat -DQUICKBMS64

REM quickbms.dll:
REM compa.bat -shared
REM rename quickbms.exe quickbms.dll

REM The libraries located in c:\mingw\lib are available here (you need also the header files):
REM http://aluigi.org/papers/bms/quickbms_dependencies.zip
REM WARNING: Recommended to use GNU GCC 6 or older. The library xcompress.lib is incompatible with
REM          the binutils that are shipped with GCC 7 and newer. Thus quickbms will FAIL to build.
set QBMS_DEPS=C:\ProgramData\MinGW\lib64

REM 32kb collect.exe fails, use a short temporary path or create one in /src
set TEMP=c:\t_
set TMP=c:\t_

windres src/res/quickbms.rc icon.o

gcc -s -O2 -m32 -mtune=generic -static ^
  -Wno-int-to-pointer-cast -D_GLIBCXX_IOSTREAM -Wl,--enable-stdcall-fixup -Wl,--large-address-aware -fno-unit-at-a-time -Wno-sign-compare -Wno-sequence-point -Wno-parentheses -Wl,--stack,4194304 -fstack-protector-all -Wstack-protector -fwrapv --param ssp-buffer-size=1 -fno-strict-overflow -Wno-missing-field-initializers -Wformat-security  -Wl,--dynamicbase -Wl,--nxcompat -Wl,--enable-auto-image-base -Wl,--enable-auto-import -fno-omit-frame-pointer -fstack-protector-all -Wall -Wno-pointer-sign -Wno-reorder -Wno-unused-variable -Wno-unused-function -Wno-cpp -Wno-misleading-indentation -Wno-stack-protector -Wno-unknown-pragmas ^
  -o quickbms.exe src\quickbms.c ^
    -Isrc\libs\libmcrypt ^
    -Isrc\libs\libtomcrypt ^
    -Isrc\libs\zlib src\libs\zlib\*.c ^
    src\compression\*.c* ^
    src\encryption\*.c* ^
    src\libs\lzma\LzmaDec.c src\libs\lzma\Lzma2Dec.c src\libs\lzma\Bra86.c src\libs\lzma\LzFind.c src\libs\lzma\LzmaEnc.c src\libs\lzma\Lzma2Enc.c ^
    src\libs\mspack\*.c ^
    src\libs\ppmd\*.cpp ^
    src\libs\aplib\depacks.c ^
    src\libs\brieflz\depacks.c src\libs\brieflz\brieflz.c ^
    src\compression\jcalg1_static.lib ^
    src\libs\zziplib\*.c ^
    src\libs\bcl\*.c ^
    src\libs\szip\*.c ^
    src\libs\lzhl\*.cpp ^
    src\libs\tdcb\*.c ^
    src\extra\mem2mem.c ^
    src\libs\libkirk\*.c ^
    src\libs\7z_advancecomp\*.cc ^
    src\libs\iris\*.cpp ^
    src\libs\old_cabextract\lzx.c ^
    src\libs\mrci\*.cpp ^
    src\libs\lz4\*.c ^
    src\libs\snappy\snappy.cc src\libs\snappy\snappy-c.cc src\libs\snappy\snappy-stubs-internal.cc src\libs\snappy\snappy-sinksource.cc ^
    src\libs\mmini\mmini_huffman.c src\libs\mmini\mmini_lzl.c ^
    src\libs\clzw\lzw-dec.c src\libs\clzw\lzw-enc.c ^
    src\libs\lzlib\lzlib.c ^
    src\libs\blosc\blosclz.c src\libs\blosc\fastcopy.c ^
    src\libs\gipfeli\*.cc ^
    src\libs\liblzg\src\lib\decode.c src\libs\liblzg\src\lib\encode.c src\libs\liblzg\src\lib\checksum.c ^
    src\libs\doboz\*.cpp ^
    src\libs\sphlib\c\*.c ^
    src\libs\shadowforce\*.cpp ^
    src\libs\zstd_aluigi\common\*.c src\libs\zstd_aluigi\compress\*.c src\libs\zstd_aluigi\decompress\*.c src\libs\zstd_aluigi\dictBuilder\*.c src\libs\zstd_aluigi\legacy\*.c ^
      -Isrc\libs\zstd_aluigi -Isrc\libs\zstd_aluigi\common -Isrc\libs\zstd_aluigi\legacy ^
    src\libs\azo\unAZO.cpp src\libs\azo\Decoder\MainCodeD.cpp src\libs\azo\Common\x86Filter.cpp ^
    src\libs\nintendo_ds\*.c ^
    src\libs\ctw\*.c ^
    src\libs\grzip\libgrzip.c ^
    src\libs\heatshrink\heatshrink_decoder.c src\libs\heatshrink\heatshrink_encoder.c ^
    src\libs\libzling\*.cpp ^
    -Isrc\libs\ecrypt\include -Isrc\libs\libcsc -D_7Z_TYPES_ ^
      src\libs\libcsc\csc_dec.cpp src\libs\libcsc\csc_default_alloc.cpp src\libs\libcsc\csc_filters.cpp src\libs\libcsc\csc_memio.cpp ^
    -DDENSITY_FORCE_INLINE=inline -Drestrict=__restrict__ ^
      src\libs\density\src\*.c src\libs\density\src\algorithms\*.c src\libs\density\src\algorithms\chameleon\core\*.c src\libs\density\src\algorithms\cheetah\core\*.c src\libs\density\src\algorithms\lion\core\*.c src\libs\density\src\algorithms\lion\forms\*.c src\libs\density\src\buffers\*.c src\libs\density\src\structure\*.c ^
    src\libs\spookyhash\*.c ^
    -Isrc\libs\brotli\include src\libs\brotli\dec\*.c src\libs\brotli\enc\*.c src\libs\brotli\common\*.c ^
    src\libs\libbsc\adler32\adler32.cpp src\libs\libbsc\bwt\bwt.cpp src\libs\libbsc\coder\coder.cpp src\libs\libbsc\coder\qlfc\qlfc.cpp src\libs\libbsc\coder\qlfc\qlfc_model.cpp src\libs\libbsc\filters\detectors.cpp src\libs\libbsc\filters\preprocessing.cpp src\libs\libbsc\libbsc\libbsc.cpp src\libs\libbsc\lzp\lzp.cpp src\libs\libbsc\platform\platform.cpp src\libs\libbsc\st\st.cpp ^
    src\libs\shoco\shoco.c ^
    src\libs\ms-compress\src\*.cpp ^
    src\libs\lzjody\lzjody.c src\libs\lzjody\byteplane_xfrm.c ^
    src\disasm\disasm.c src\disasm\cmdlist.c src\disasm\assembl\assembl.c ^
    -DMYDOWN_SSL -DMYDOWN_GLOBAL_COOKIE src\libs\mydownlib\mydownlib.c ^
    src\libs\TurboRLE\trlec.c src\libs\TurboRLE\trled.c src\libs\TurboRLE\ext\mrle.c ^
    src\libs\lhasa\lib\*_decoder.c src\libs\lhasa\lib\crc16.c ^
    src\libs\dipperstein\*.c ^
    src\libs\liblzf\lzf_d.c src\libs\liblzf\lzf_c_best.c ^
    src\libs\zopfli\*.c ^
    src\libs\lzham_codec\lzhamcomp\*.cpp src\libs\lzham_codec\lzhamdecomp\*.cpp src\libs\lzham_codec\lzhamlib\*.cpp -Isrc\libs\lzham_codec\include -Isrc\libs\lzham_codec\lzhamcomp ^
      -Isrc\libs\lzham_codec\lzhamdecomp -DLZHAM_ANSI_CPLUSPLUS ^
    src\libs\dmsdos\*.c ^
    src\libs\tornado\Tornado.cpp src\libs\tornado\Common.cpp ^
    src\libs\PKLib\*.c ^
    src\extra\mybits.c ^
    src\libs\lz5\lz5*.c ^
    src\libs\lizard\*.c ^
    src\libs\ppmz2\*.c* ^
    src\libs\libdivsufsort\*.c ^
    src\libs\xxhash\*.c ^
    src\extra\xalloc.c ^
    src\libs\lzfse\src\*.c ^
    src\libs\hsel\myhsel.cpp src\libs\hsel\HSEL.cpp ^
    src\libs\glza\GLZAmodel.c ^
    -Isrc\libs\lzo\include src\libs\lzo\src\*.c ^
    -Isrc\libs\ucl -Isrc\libs\ucl\include src\libs\ucl\src\n*.c src\libs\ucl\src\alloc.c ^
    src\libs\liblzs\lzs-compression.c src\libs\liblzs\lzs-decompression.c ^
    src\libs\bzip2\*.c ^
    src\libs\zlib\contrib\infback9\*.c ^
    src\libs\lzw-ab\lzw-lib.c ^
    src\libs\cryptohash-sha1\*.c ^
    -Isrc\libs\capstone\include src\libs\capstone\*.c -DCAPSTONE_X86_ATT_DISABLE -DCAPSTONE_USE_SYS_DYN_MEM -DCAPSTONE_HAS_ARM -DCAPSTONE_HAS_ARM64 -DCAPSTONE_HAS_MIPS -DCAPSTONE_HAS_X86 -DCAPSTONE_HAS_POWERPC -DCAPSTONE_HAS_SPARC -DCAPSTONE_HAS_SYSZ -DCAPSTONE_HAS_XCORE ^
      src\libs\capstone\arch\AArch64\*.c src\libs\capstone\arch\ARM\*.c src\libs\capstone\arch\Mips\*.c src\libs\capstone\arch\PowerPC\*.c src\libs\capstone\arch\Sparc\*.c src\libs\capstone\arch\SystemZ\*.c src\libs\capstone\arch\X86\*.c src\libs\capstone\arch\XCore\*.c ^
    -Isrc ^
    src\libs\lzma\MtCoder.c src\libs\lzma\MtDec.c src\libs\lzma\Threads.c src\libs\lzma\LzFindMt.c ^
    src\extra\MemoryModule.c ^
    src\libs\amiga\amiga.s ^
    -Dmalloc_usable_size=_msize ^
    src\libs\zenlib\ZenLib.obj ^
    %QBMS_DEPS%\xcompress.lib ^
    -lcomdlg32 icon.o ^
    %QBMS_DEPS%\libssl.dll.a ^
    %QBMS_DEPS%\libcrypto.dll.a ^
    src\libs\libmcrypt\libmcrypt.a ^
    src\libs\libtomcrypt\libtomcrypt.a ^
    src\libs\libtommath\libtommath.a ^
    -lgdi32 -lstdc++ -lcrypt32 -lm -lws2_32 -lwinmm -lbfd -liberty -liconv -limagehlp ^
    -Wl,--add-stdcall-alias -Wl,--export-all-symbols -Wl,--exclude-libs,xcompress.lib %1 %2

rem useless: strip -s -g --strip-unneeded -x -X quickbms.exe
