cmake_minimum_required(VERSION 3.8)

project(quickbms VERSION 1.0.0 LANGUAGES C CXX)
set(OPENSSL_ROOT_DIR ${CMAKE_SOURCE_DIR}/../../../../packages/openssl-windows_x64-windows)
find_package(OpenSSL REQUIRED)
set(QBMS_DEPS C:/ProgramData/MinGW/mingw64/lib)

if(WIN32)
   if (NOT EXISTS ${CMAKE_SOURCE_DIR}/icon.o)
      execute_process(
         COMMAND windres res/quickbms.rc icon.o
         WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
      )
   endif()
endif()


## Librarys
set(CFLAGS -m32 -s -O2 -fno-stack-protector -fno-unit-at-a-time -fno-omit-frame-pointer -w
   -static -static-libgcc -static-libstdc++ 
   -DDISABLE_MCRYPT -DDISABLE_TOMCRYPT)

# -Ilibs/zlib libs/zlib/*.c#libs/zlib/contrib/infback9/*.c
file(GLOB zlib libs/zlib/*.c libs/zlib/contrib/infback9/*.c)
add_library(zlib OBJECT ${zlib})
target_compile_options(zlib PUBLIC ${CFLAGS})
target_include_directories(zlib PUBLIC libs/zlib)

# compression/*.c*
file(GLOB compression compression/*.c*)
add_library(compression OBJECT ${compression})
target_compile_options(compression PUBLIC ${CFLAGS})
target_include_directories(compression PUBLIC libs/libcsc)

# encryption/*.c*
file(GLOB encryption encryption/*.c*)
add_library(encryption OBJECT ${encryption})
target_compile_options(encryption PUBLIC ${CFLAGS})
target_include_directories(encryption PUBLIC libs/ecrypt/include)

# libs/lzma/LzmaDec.c libs/lzma/Lzma2Dec.c libs/lzma/Bra86.c libs/lzma/LzFind.c libs/lzma/LzmaEnc.c libs/lzma/Lzma2Enc.c
add_library(lzma OBJECT libs/lzma/LzmaDec.c libs/lzma/Lzma2Dec.c libs/lzma/Bra86.c libs/lzma/LzFind.c libs/lzma/LzmaEnc.c libs/lzma/Lzma2Enc.c
   libs/lzma/MtCoder.c libs/lzma/MtDec.c libs/lzma/Threads.c libs/lzma/LzFindMt.c) #: sky
target_compile_options(lzma PUBLIC ${CFLAGS})

# libs/mspack/*.c
file(GLOB mspack libs/mspack/*.c)
add_library(mspack OBJECT ${mspack})
target_compile_options(mspack PUBLIC ${CFLAGS})

# libs/ppmd/*.cpp
file(GLOB ppmd libs/ppmd/*.cpp)
add_library(ppmd OBJECT ${ppmd})
target_compile_options(ppmd PUBLIC ${CFLAGS})

# libs/aplib/depacks.c
add_library(aplib OBJECT libs/aplib/depacks.c)
target_compile_options(aplib PUBLIC ${CFLAGS})

# libs/brieflz/depacks.c libs/brieflz/brieflz.c
add_library(brieflz OBJECT libs/brieflz/depacks.c libs/brieflz/brieflz.c)
target_compile_options(brieflz PUBLIC ${CFLAGS})

# compression/jcalg1_static.lib

# libs/zziplib/*.c
file(GLOB zziplib libs/zziplib/*.c)
add_library(zziplib OBJECT ${zziplib})
target_compile_options(zziplib PUBLIC ${CFLAGS} -Wno-aggressive-loop-optimizations)

# libs/bcl/*.c
file(GLOB bcl libs/bcl/*.c)
add_library(bcl OBJECT ${bcl})
target_compile_options(bcl PUBLIC ${CFLAGS})

# libs/szip/*.c
file(GLOB szip libs/szip/*.c)
add_library(szip OBJECT ${szip})
target_compile_options(szip PUBLIC ${CFLAGS})

# libs/lzhl/*.cpp
file(GLOB lzhl libs/lzhl/*.cpp)
add_library(lzhl OBJECT ${lzhl})
target_compile_options(lzhl PUBLIC ${CFLAGS})

# libs/tdcb/*.c
file(GLOB tdcb libs/tdcb/*.c)
   #list(REMOVE_ITEM tdcb ${CMAKE_CURRENT_SOURCE_DIR}/libs/tdcb/lzss.c) #: sky
add_library(tdcb OBJECT ${tdcb})
target_compile_options(tdcb PUBLIC ${CFLAGS})

# extra/mem2mem.c
file(GLOB mem2mem extra/mem2mem.c)
add_library(mem2mem OBJECT ${mem2mem})
target_compile_options(mem2mem PUBLIC ${CFLAGS})

# libs/libkirk/*.c
file(GLOB libkirk libs/libkirk/*.c)
add_library(libkirk OBJECT ${libkirk})
target_compile_options(libkirk PUBLIC ${CFLAGS})

# libs/7z_advancecomp/*.cc
file(GLOB 7z_advancecomp libs/7z_advancecomp/*.cc)
add_library(7z_advancecomp OBJECT ${7z_advancecomp})
target_compile_options(7z_advancecomp PUBLIC ${CFLAGS})

# libs/iris/*.cpp
file(GLOB iris libs/iris/*.cpp)
add_library(iris OBJECT ${iris})
target_compile_options(iris PUBLIC ${CFLAGS})

# libs/old_cabextract/lzx.c
add_library(old_cabextract OBJECT libs/old_cabextract/lzx.c)
target_compile_options(old_cabextract PUBLIC ${CFLAGS})

# libs/mrci/*.cpp
file(GLOB mrci libs/mrci/*.cpp)
add_library(mrci OBJECT ${mrci})
target_compile_options(mrci PUBLIC ${CFLAGS})

# libs/lz4/*.c
file(GLOB lz4 libs/lz4/*.c)
add_library(lz4 OBJECT ${lz4})
target_compile_options(lz4 PUBLIC ${CFLAGS})

# libs/snappy/snappy.cc libs/snappy/snappy-c.cc libs/snappy/snappy-stubs-internal.cc libs/snappy/snappy-sinksource.cc
add_library(snappy OBJECT libs/snappy/snappy.cc libs/snappy/snappy-c.cc libs/snappy/snappy-stubs-internal.cc libs/snappy/snappy-sinksource.cc)
target_compile_options(snappy PUBLIC ${CFLAGS})

# libs/mmini/mmini_huffman.c libs/mmini/mmini_lzl.c
add_library(mmini OBJECT libs/mmini/mmini_huffman.c libs/mmini/mmini_lzl.c)
target_compile_options(mmini PUBLIC ${CFLAGS})

# libs/clzw/lzw-dec.c libs/clzw/lzw-enc.c
add_library(clzw OBJECT libs/clzw/lzw-dec.c libs/clzw/lzw-enc.c)
target_compile_options(clzw PUBLIC ${CFLAGS})

# libs/lzlib/lzlib.c
add_library(lzlib OBJECT libs/lzlib/lzlib.c)
target_compile_options(lzlib PUBLIC ${CFLAGS})

# libs/blosc/blosclz.c libs/blosc/fastcopy.c
add_library(blosc OBJECT libs/blosc/blosclz.c libs/blosc/fastcopy.c)
target_compile_options(blosc PUBLIC ${CFLAGS})

# libs/gipfeli/*.cc
file(GLOB gipfeli libs/gipfeli/*.cc)
add_library(gipfeli OBJECT ${gipfeli})
target_compile_options(gipfeli PUBLIC ${CFLAGS})

# libs/liblzg/src/lib/decode.c libs/liblzg/src/lib/encode.c libs/liblzg/src/lib/checksum.c
add_library(liblzg OBJECT libs/liblzg/src/lib/decode.c libs/liblzg/src/lib/encode.c libs/liblzg/src/lib/checksum.c)
target_compile_options(liblzg PUBLIC ${CFLAGS})

# libs/doboz/*.cpp
file(GLOB doboz libs/doboz/*.cpp)
add_library(doboz OBJECT ${doboz})
target_compile_options(doboz PUBLIC ${CFLAGS})

# libs/sphlib/c/*.c
file(GLOB sphlib libs/sphlib/c/*.c)
add_library(sphlib OBJECT ${sphlib})
target_compile_options(sphlib PUBLIC ${CFLAGS})

# libs/shadowforce/*.cpp
file(GLOB shadowforce libs/shadowforce/*.cpp)
add_library(shadowforce OBJECT ${shadowforce})
target_compile_options(shadowforce PUBLIC ${CFLAGS} -Wno-delete-incomplete)

# libs/zstd_aluigi/common/*.c libs/zstd_aluigi/compress/*.c libs/zstd_aluigi/decompress/*.c libs/zstd_aluigi/dictBuilder/*.c libs/zstd_aluigi/legacy/*.c -Ilibs/zstd_aluigi -Ilibs/zstd_aluigi/common -Ilibs/zstd_aluigi/legacy
file(GLOB zstd_aluigi libs/zstd_aluigi/common/*.c libs/zstd_aluigi/compress/*.c libs/zstd_aluigi/decompress/*.c libs/zstd_aluigi/dictBuilder/*.c libs/zstd_aluigi/legacy/*.c)
add_library(zstd_aluigi OBJECT ${zstd_aluigi})
target_compile_options(zstd_aluigi PUBLIC ${CFLAGS})
target_include_directories(zstd_aluigi PUBLIC libs/zstd_aluigi libs/zstd_aluigi/common libs/zstd_aluigi/legacy)

# libs/azo/unAZO.cpp libs/azo/Decoder/MainCodeD.cpp libs/azo/Common/x86Filter.cpp
add_library(azo OBJECT libs/azo/unAZO.cpp libs/azo/Decoder/MainCodeD.cpp libs/azo/Common/x86Filter.cpp)
target_compile_options(azo PUBLIC ${CFLAGS})

# libs/nintendo_ds/*.c
file(GLOB nintendo_ds libs/nintendo_ds/*.c)
add_library(nintendo_ds OBJECT ${nintendo_ds})
target_compile_options(nintendo_ds PUBLIC ${CFLAGS})

# libs/ctw/*.c
file(GLOB ctw libs/ctw/*.c)
add_library(ctw OBJECT ${ctw})
target_compile_options(ctw PUBLIC ${CFLAGS})

# libs/grzip/libgrzip.c
add_library(grzip OBJECT libs/grzip/libgrzip.c)
target_compile_options(grzip PUBLIC ${CFLAGS})

# libs/heatshrink/heatshrink_decoder.c libs/heatshrink/heatshrink_encoder.c
add_library(heatshrink OBJECT libs/heatshrink/heatshrink_decoder.c libs/heatshrink/heatshrink_encoder.c)
target_compile_options(heatshrink PUBLIC ${CFLAGS})

# libs/libzling/*.cpp
file(GLOB libzling libs/libzling/*.cpp)
add_library(libzling OBJECT ${libzling})
target_compile_options(libzling PUBLIC ${CFLAGS})

# -Ilibs/ecrypt/include -Ilibs/libcsc -D_7Z_TYPES_ libs/libcsc/csc_dec.cpp libs/libcsc/csc_default_alloc.cpp libs/libcsc/csc_filters.cpp libs/libcsc/csc_memio.cpp
add_library(libcsc OBJECT libs/libcsc/csc_dec.cpp libs/libcsc/csc_default_alloc.cpp libs/libcsc/csc_filters.cpp libs/libcsc/csc_memio.cpp)
target_compile_options(libcsc PUBLIC ${CFLAGS} -D_7Z_TYPES_)
target_include_directories(libcsc PUBLIC libs/ecrypt/include libs/libcsc )

# -DDENSITY_FORCE_INLINE=inline -Drestrict=__restrict__ libs/density/src/*.c libs/density/src/algorithms/*.c libs/density/src/algorithms/chameleon/core/*.c libs/density/src/algorithms/cheetah/core/*.c libs/density/src/algorithms/lion/core/*.c libs/density/src/algorithms/lion/forms/*.c libs/density/src/buffers/*.c libs/density/src/structure/*.c
file(GLOB density libs/density/src/*.c libs/density/src/algorithms/*.c libs/density/src/algorithms/chameleon/core/*.c libs/density/src/algorithms/cheetah/core/*.c libs/density/src/algorithms/lion/core/*.c libs/density/src/algorithms/lion/forms/*.c libs/density/src/buffers/*.c libs/density/src/structure/*.c)
add_library(density OBJECT ${density})
target_compile_options(density PUBLIC ${CFLAGS} -DDENSITY_FORCE_INLINE=inline -Drestrict=__restrict__)

# libs/spookyhash/*.c
file(GLOB spookyhash libs/spookyhash/*.c)
add_library(spookyhash OBJECT ${spookyhash})
target_compile_options(spookyhash PUBLIC ${CFLAGS})

# -Ilibs/brotli/include libs/brotli/dec/*.c libs/brotli/enc/*.c libs/brotli/common/*.c
file(GLOB brotli libs/brotli/dec/*.c libs/brotli/enc/*.c libs/brotli/common/*.c)
add_library(brotli OBJECT ${brotli})
target_compile_options(brotli PUBLIC ${CFLAGS})
target_include_directories(brotli PUBLIC libs/brotli/include)

# libs/libbsc/adler32/adler32.cpp libs/libbsc/bwt/bwt.cpp libs/libbsc/coder/coder.cpp libs/libbsc/coder/qlfc/qlfc.cpp libs/libbsc/coder/qlfc/qlfc_model.cpp libs/libbsc/filters/detectors.cpp libs/libbsc/filters/preprocessing.cpp libs/libbsc/libbsc/libbsc.cpp libs/libbsc/lzp/lzp.cpp libs/libbsc/platform/platform.cpp libs/libbsc/st/st.cpp
add_library(libbsc OBJECT libs/libbsc/adler32/adler32.cpp libs/libbsc/bwt/bwt.cpp libs/libbsc/coder/coder.cpp libs/libbsc/coder/qlfc/qlfc.cpp libs/libbsc/coder/qlfc/qlfc_model.cpp libs/libbsc/filters/detectors.cpp libs/libbsc/filters/preprocessing.cpp libs/libbsc/libbsc/libbsc.cpp libs/libbsc/lzp/lzp.cpp libs/libbsc/platform/platform.cpp libs/libbsc/st/st.cpp)
target_compile_options(libbsc PUBLIC ${CFLAGS})

# libs/shoco/shoco.c
add_library(shoco OBJECT libs/shoco/shoco.c)
target_compile_options(shoco PUBLIC ${CFLAGS})

# libs/ms-compress/src/*.cpp
file(GLOB ms-compress libs/ms-compress/src/*.cpp)
add_library(ms-compress OBJECT ${ms-compress})
target_compile_options(ms-compress PUBLIC ${CFLAGS})

# libs/lzjody/lzjody.c libs/lzjody/byteplane_xfrm.c
add_library(lzjody OBJECT libs/lzjody/lzjody.c libs/lzjody/byteplane_xfrm.c)
target_compile_options(lzjody PUBLIC ${CFLAGS})

# disasm/disasm.c disasm/cmdlist.c disasm/assembl/assembl.c
add_library(disasm OBJECT disasm/disasm.c disasm/cmdlist.c disasm/assembl/assembl.c)
target_compile_options(disasm PUBLIC ${CFLAGS})

# -DMYDOWN_SSL -DMYDOWN_GLOBAL_COOKIE libs/mydownlib/mydownlib.c
add_library(mydownlib OBJECT libs/mydownlib/mydownlib.c)
target_compile_options(mydownlib PUBLIC ${CFLAGS} -DMYDOWN_SSL -DMYDOWN_GLOBAL_COOKIE)
target_link_libraries(mydownlib PUBLIC OpenSSL::SSL)

# libs/TurboRLE/trlec.c libs/TurboRLE/trled.c libs/TurboRLE/ext/mrle.c
add_library(TurboRLE OBJECT libs/TurboRLE/trlec.c libs/TurboRLE/trled.c libs/TurboRLE/ext/mrle.c)
target_compile_options(TurboRLE PUBLIC ${CFLAGS})

# libs/lhasa/lib/*_decoder.c libs/lhasa/lib/crc16.c
file(GLOB lhasa libs/lhasa/lib/*_decoder.c libs/lhasa/lib/crc16.c)
add_library(lhasa OBJECT ${lhasa})
target_compile_options(lhasa PUBLIC ${CFLAGS})

# libs/dipperstein/*.c
file(GLOB dipperstein libs/dipperstein/*.c)
add_library(dipperstein OBJECT ${dipperstein})
target_compile_options(dipperstein PUBLIC ${CFLAGS} -Wno-aggressive-loop-optimizations)

# libs/liblzf/lzf_d.c libs/liblzf/lzf_c_best.c
add_library(liblzf OBJECT libs/liblzf/lzf_d.c libs/liblzf/lzf_c_best.c)
target_compile_options(liblzf PUBLIC ${CFLAGS})

# libs/zopfli/*.c
file(GLOB zopfli libs/zopfli/*.c)
add_library(zopfli OBJECT ${zopfli})
target_compile_options(zopfli PUBLIC ${CFLAGS})

# libs/lzham_codec/lzhamcomp/*.cpp libs/lzham_codec/lzhamdecomp/*.cpp libs/lzham_codec/lzhamlib/*.cpp -Ilibs/lzham_codec/include -Ilibs/lzham_codec/lzhamcomp -Ilibs/lzham_codec/lzhamdecomp -DLZHAM_ANSI_CPLUSPLUS
file(GLOB lzham_codec libs/lzham_codec/lzhamcomp/*.cpp libs/lzham_codec/lzhamdecomp/*.cpp libs/lzham_codec/lzhamlib/*.cpp)
add_library(lzham_codec OBJECT ${lzham_codec})
target_compile_options(lzham_codec PUBLIC ${CFLAGS} -DLZHAM_ANSI_CPLUSPLUS -Wno-cpp -Wno-attributes)
target_include_directories(lzham_codec PUBLIC libs/lzham_codec/include libs/lzham_codec/lzhamcomp libs/lzham_codec/lzhamdecomp)
target_compile_definitions(lzham_codec PUBLIC malloc_usable_size=_msize) #: sky

# libs/dmsdos/*.c
file(GLOB dmsdos libs/dmsdos/*.c)
add_library(dmsdos OBJECT ${dmsdos})
target_compile_options(dmsdos PUBLIC ${CFLAGS})

# libs/tornado/Tornado.cpp libs/tornado/Common.cpp
add_library(tornado OBJECT libs/tornado/Tornado.cpp libs/tornado/Common.cpp)
target_compile_options(tornado PUBLIC ${CFLAGS})

# libs/PKLib/*.c
file(GLOB PKLib libs/PKLib/*.c)
add_library(PKLib OBJECT ${PKLib})
target_compile_options(PKLib PUBLIC ${CFLAGS})

# extra/mybits.c
add_library(mybits OBJECT extra/mybits.c)
target_compile_options(mybits PUBLIC ${CFLAGS})

# libs/lz5/lz5*.c
file(GLOB lz5 libs/lz5/lz5*.c)
add_library(lz5 OBJECT ${lz5})
target_compile_options(lz5 PUBLIC ${CFLAGS})

# libs/lizard/*.c
file(GLOB lizard libs/lizard/*.c)
add_library(lizard OBJECT ${lizard})
target_compile_options(lizard PUBLIC ${CFLAGS})

# libs/ppmz2/*.c*
file(GLOB ppmz2 libs/ppmz2/*.c*)
add_library(ppmz2 OBJECT ${ppmz2})
target_compile_options(ppmz2 PUBLIC ${CFLAGS} -Wno-deprecated-declarations) #-Wno-delete-incomplete

# libs/libdivsufsort/*.c
file(GLOB libdivsufsort libs/libdivsufsort/*.c)
add_library(libdivsufsort OBJECT ${libdivsufsort})
target_compile_options(libdivsufsort PUBLIC ${CFLAGS})

# libs/xxhash/*.c
file(GLOB xxhash libs/xxhash/*.c)
add_library(xxhash OBJECT ${xxhash})
target_compile_options(xxhash PUBLIC ${CFLAGS})

# extra/xalloc.c
add_library(xalloc OBJECT extra/xalloc.c)
target_compile_options(xalloc PUBLIC ${CFLAGS})

# libs/lzfse/src/*.c
file(GLOB lzfse libs/lzfse/src/*.c)
add_library(lzfse OBJECT ${lzfse})
target_compile_options(lzfse PUBLIC ${CFLAGS})

# libs/hsel/myhsel.cpp libs/hsel/HSEL.cpp
add_library(hsel OBJECT libs/hsel/myhsel.cpp libs/hsel/HSEL.cpp)
target_compile_options(hsel PUBLIC ${CFLAGS})

# libs/glza/GLZAmodel.c
add_library(glza OBJECT libs/glza/GLZAmodel.c)
target_compile_options(glza PUBLIC ${CFLAGS})

# -Ilibs/lzo/include libs/lzo/src/*.c
file(GLOB lzo libs/lzo/src/*.c)
add_library(lzo OBJECT ${lzo})
target_compile_options(lzo PUBLIC ${CFLAGS})
target_include_directories(lzo PUBLIC libs/lzo/include)

# -Ilibs/ucl -Ilibs/ucl/include libs/ucl/src/n*.c libs/ucl/src/alloc.c
file(GLOB ucl libs/ucl/src/n*.c libs/ucl/src/alloc.c)
add_library(ucl OBJECT ${ucl})
target_compile_options(ucl PUBLIC ${CFLAGS})
target_include_directories(ucl PUBLIC libs/ucl libs/ucl/include)

# libs/liblzs/lzs-compression.c libs/liblzs/lzs-decompression.c
add_library(liblzs OBJECT libs/liblzs/lzs-compression.c libs/liblzs/lzs-decompression.c)
target_compile_options(liblzs PUBLIC ${CFLAGS})

# libs/bzip2/*.c
file(GLOB bzip2 libs/bzip2/*.c)
add_library(bzip2 OBJECT ${bzip2})
target_compile_options(bzip2 PUBLIC ${CFLAGS})

# libs/lzw-ab/lzw-lib.c
add_library(lzw-ab OBJECT libs/lzw-ab/lzw-lib.c)
target_compile_options(lzw-ab PUBLIC ${CFLAGS})

# libs/cryptohash-sha1/*.c
file(GLOB cryptohash-sha1 libs/cryptohash-sha1/*.c)
add_library(cryptohash-sha1 OBJECT ${cryptohash-sha1})
target_compile_options(cryptohash-sha1 PUBLIC ${CFLAGS})

# -Ilibs/capstone/include libs/capstone/*.c -DCAPSTONE_X86_ATT_DISABLE -DCAPSTONE_USE_SYS_DYN_MEM -DCAPSTONE_HAS_ARM -DCAPSTONE_HAS_ARM64 -DCAPSTONE_HAS_MIPS -DCAPSTONE_HAS_X86 -DCAPSTONE_HAS_POWERPC -DCAPSTONE_HAS_SPARC -DCAPSTONE_HAS_SYSZ -DCAPSTONE_HAS_XCORE libs/capstone/arch/AArch64/*.c libs/capstone/arch/ARM/*.c libs/capstone/arch/Mips/*.c libs/capstone/arch/PowerPC/*.c libs/capstone/arch/Sparc/*.c libs/capstone/arch/SystemZ/*.c libs/capstone/arch/X86/*.c libs/capstone/arch/XCore/*.c
file(GLOB capstone libs/capstone/*.c libs/capstone/arch/AArch64/*.c libs/capstone/arch/ARM/*.c libs/capstone/arch/Mips/*.c libs/capstone/arch/PowerPC/*.c libs/capstone/arch/Sparc/*.c libs/capstone/arch/SystemZ/*.c libs/capstone/arch/X86/*.c libs/capstone/arch/XCore/*.c)
add_library(capstone OBJECT ${capstone})
target_compile_options(capstone PUBLIC ${CFLAGS} -DCAPSTONE_X86_ATT_DISABLE -DCAPSTONE_USE_SYS_DYN_MEM -DCAPSTONE_HAS_ARM -DCAPSTONE_HAS_ARM64 -DCAPSTONE_HAS_MIPS -DCAPSTONE_HAS_X86 -DCAPSTONE_HAS_POWERPC -DCAPSTONE_HAS_SPARC -DCAPSTONE_HAS_SYSZ -DCAPSTONE_HAS_XCORE)
target_include_directories(capstone PUBLIC libs/capstone/include)

# libs/lua/src/*.c
file(GLOB lua libs/lua/src/*.c)
add_library(lua OBJECT ${lua})
target_compile_options(lua PUBLIC ${CFLAGS})

# extra
#add_library(_extra OBJECT extra/MemoryModule.c libs/amiga/amiga.s)
#target_compile_options(_extra PUBLIC ${CFLAGS})


## Executable

add_executable(${PROJECT_NAME} quickbms.c)
target_compile_options(${PROJECT_NAME} PUBLIC ${CFLAGS}) #-ldl
   #-lcomdlg32 -lgdi32 -lstdc++ -lcrypt32 -lm -lws2_32 -lwinmm -lbfd -liberty -liconv -limagehlp
   #-Wl,--add-stdcall-alias -Wl,--export-all-symbols -Wl,--exclude-libs,xcompress.lib)
target_compile_definitions(${PROJECT_NAME} PUBLIC stricoll=_stricoll)
target_include_directories(${PROJECT_NAME} PUBLIC
   libs/capstone/include
   libs/ucl libs/ucl/include
   libs/lzo/include
   libs/brotli/include
   libs/zstd_aluigi libs/zstd_aluigi/common libs/zstd_aluigi/legacy)

target_link_libraries(${PROJECT_NAME} PUBLIC comdlg32 gdi32 crypt32 ws2_32 winmm bfd iberty iconv imagehlp OpenSSL::SSL 
   #ssp
   zlib
   compression
   encryption
   lzma
   mspack
   ppmd
   aplib
   brieflz
   #${CMAKE_SOURCE_DIR}/compression/jcalg1_static.lib
   zziplib
   bcl
   szip
   lzhl
   tdcb
   mem2mem
   libkirk
   7z_advancecomp
   iris
   old_cabextract
   mrci
   lz4
   snappy
   mmini
   clzw
   lzlib
   blosc
   gipfeli
   liblzg
   doboz
   sphlib
   shadowforce
   zstd_aluigi
   azo
   nintendo_ds
   ctw
   grzip
   heatshrink
   libzling
   libcsc
   density
   spookyhash
   brotli
   libbsc
   shoco
   ms-compress
   lzjody
   disasm
   mydownlib
   TurboRLE
   lhasa
   dipperstein
   liblzf
   zopfli
   lzham_codec
   dmsdos
   tornado
   PKLib
   mybits
   lz5
   lizard
   ppmz2
   libdivsufsort
   xxhash
   xalloc
   lzfse
   hsel
   glza
   lzo
   ucl
   liblzs
   bzip2
   lzw-ab
   cryptohash-sha1
   capstone
   lua
   #${QBMS_DEPS}/xcompress.lib
   #-lcomdlg32 icon.o
   #${QBMS_DEPS}/libssl.dll.a
   #${QBMS_DEPS}/libcrypto.dll.a
   #${CMAKE_SOURCE_DIR}/libs/libmcrypt/libmcrypt.a
   #${CMAKE_SOURCE_DIR}/libs/libtomcrypt/libtomcrypt.a
   #${CMAKE_SOURCE_DIR}/libs/libtommath/libtommath.a
   )
   #_extra)

#install(TARGETS ${PROJECT_NAME} DESTINATION bin)
