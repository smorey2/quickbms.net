cmake_minimum_required(VERSION 3.8)

#set(CMAKE_C_STANDARD 11)
#set(CMAKE_C_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(quickbms VERSION 1.0.0 LANGUAGES C CXX)
set(OPENSSL_ROOT_DIR C:/_GITHUB/smorey2/quickbms.net/_vcpkg/packages/openssl-windows_x64-windows)

# Glob
file(GLOB libs_zlib libs/zlib/*.c libs/zlib/contrib/infback9/*.c) #different
file(GLOB compression compression/*.c*)
file(GLOB encryption encryption/*.c*)
file(GLOB libs_lzma libs/lzma/LzmaDec.c libs/lzma/Lzma2Dec.c libs/lzma/Bra86.c libs/lzma/LzFind.c libs/lzma/LzmaEnc.c libs/lzma/Lzma2Enc.c
   libs/lzma/MtCoder.c libs/lzma/MtDec.c libs/lzma/Threads.c libs/lzma/LzFindMt.c) #added
file(GLOB libs_mspack libs/mspack/*.c)
file(GLOB libs_ppmd libs/ppmd/*.cpp)
file(GLOB libs_aplib libs/aplib/depacks.c)
file(GLOB libs_brieflz libs/brieflz/depacks.c libs/brieflz/brieflz.c)
file(GLOB libs_zziplib libs/zziplib/*.c)
file(GLOB libs_bcl libs/bcl/*.c)
file(GLOB libs_szip libs/szip/*.c)
file(GLOB libs_lzhl libs/lzhl/*.cpp)
file(GLOB libs_tdcb libs/tdcb/*.c)
   #list(REMOVE_ITEM libs_tdcb ${CMAKE_CURRENT_SOURCE_DIR}/libs/tdcb/lzss.c) #: sky
file(GLOB extra_mem2mem extra/mem2mem.c)
file(GLOB libs_libkirk libs/libkirk/*.c)
file(GLOB libs_7z_advancecomp libs/7z_advancecomp/*.cc)
file(GLOB libs_iris libs/iris/*.cpp)
file(GLOB libs_old_cabextract libs/old_cabextract/lzx.c)
file(GLOB libs_mrci libs/mrci/*.cpp)
file(GLOB libs_lz4 libs/lz4/*.c)
file(GLOB libs_snappy libs/snappy/snappy.cc libs/snappy/snappy-c.cc libs/snappy/snappy-stubs-internal.cc libs/snappy/snappy-sinksource.cc)
file(GLOB libs_mmini libs/mmini/mmini_huffman.c libs/mmini/mmini_lzl.c)
file(GLOB libs_clzw libs/clzw/lzw-dec.c libs/clzw/lzw-enc.c)
file(GLOB libs_lzlib libs/lzlib/lzlib.c)
file(GLOB libs_blosc libs/blosc/blosclz.c libs/blosc/fastcopy.c)
file(GLOB libs_gipfeli libs/gipfeli/*.cc)
file(GLOB libs_liblzg libs/liblzg/src/lib/decode.c libs/liblzg/src/lib/encode.c libs/liblzg/src/lib/checksum.c)
file(GLOB libs_doboz libs/doboz/*.cpp)
file(GLOB libs_sphlib libs/sphlib/c/*.c)
file(GLOB libs_shadowforce libs/shadowforce/*.cpp)
file(GLOB libs_zstd_aluigi libs/zstd_aluigi/common/*.c libs/zstd_aluigi/compress/*.c libs/zstd_aluigi/decompress/*.c libs/zstd_aluigi/dictBuilder/*.c libs/zstd_aluigi/legacy/*.c)
file(GLOB libs_azo libs/azo/unAZO.cpp libs/azo/Decoder/MainCodeD.cpp libs/azo/Common/x86Filter.cpp)
file(GLOB libs_nintendo_ds libs/nintendo_ds/*.c)
file(GLOB libs_ctw libs/ctw/*.c)
file(GLOB libs_grzip libs/grzip/libgrzip.c)
file(GLOB libs_heatshrink libs/heatshrink/heatshrink_decoder.c libs/heatshrink/heatshrink_encoder.c)
file(GLOB libs_libzling libs/libzling/*.cpp)
file(GLOB libs_libcsc libs/libcsc/csc_dec.cpp libs/libcsc/csc_default_alloc.cpp libs/libcsc/csc_filters.cpp libs/libcsc/csc_memio.cpp)
file(GLOB libs_density libs/density/src/*.c libs/density/src/algorithms/*.c libs/density/src/algorithms/chameleon/core/*.c libs/density/src/algorithms/cheetah/core/*.c libs/density/src/algorithms/lion/core/*.c libs/density/src/algorithms/lion/forms/*.c libs/density/src/buffers/*.c libs/density/src/structure/*.c)
file(GLOB libs_spookyhash libs/spookyhash/*.c)
file(GLOB libs_brotli libs/brotli/dec/*.c libs/brotli/enc/*.c libs/brotli/common/*.c)
file(GLOB libs_libbsc libs/libbsc/adler32/adler32.cpp libs/libbsc/bwt/bwt.cpp libs/libbsc/coder/coder.cpp libs/libbsc/coder/qlfc/qlfc.cpp libs/libbsc/coder/qlfc/qlfc_model.cpp libs/libbsc/filters/detectors.cpp libs/libbsc/filters/preprocessing.cpp libs/libbsc/libbsc/libbsc.cpp libs/libbsc/lzp/lzp.cpp libs/libbsc/platform/platform.cpp libs/libbsc/st/st.cpp)
file(GLOB libs_shoco libs/shoco/shoco.c)
file(GLOB libs_ms-compress libs/ms-compress/src/*.cpp)
file(GLOB libs_lzjody libs/lzjody/lzjody.c libs/lzjody/byteplane_xfrm.c)
file(GLOB libs_disasm disasm/disasm.c disasm/cmdlist.c disasm/assembl/assembl.c)
file(GLOB libs_mydownlib libs/mydownlib/mydownlib.c)
file(GLOB libs_TurboRLE libs/TurboRLE/trlec.c libs/TurboRLE/trled.c libs/TurboRLE/ext/mrle.c)
file(GLOB libs_lhasa libs/lhasa/lib/*_decoder.c libs/lhasa/lib/crc16.c)
file(GLOB libs_dipperstein libs/dipperstein/*.c)
file(GLOB libs_liblzf libs/liblzf/lzf_d.c libs/liblzf/lzf_c_best.c)
file(GLOB libs_zopfli libs/zopfli/*.c)
file(GLOB libs_lzham_codec libs/lzham_codec/lzhamcomp/*.cpp libs/lzham_codec/lzhamdecomp/*.cpp libs/lzham_codec/lzhamlib/*.cpp)
file(GLOB libs_dmsdos libs/dmsdos/*.c)
file(GLOB libs_tornado libs/tornado/Tornado.cpp libs/tornado/Common.cpp)
file(GLOB libs_PKLib libs/PKLib/*.c)
file(GLOB extra_mybits extra/mybits.c)
file(GLOB libs_lz5 libs/lz5/lz5*.c)
file(GLOB libs_lizard libs/lizard/*.c)
file(GLOB libs_ppmz2 libs/ppmz2/*.c*)
file(GLOB libs_libdivsufsort libs/libdivsufsort/*.c)
file(GLOB libs_xxhash libs/xxhash/*.c)
file(GLOB extra_xalloc extra/xalloc.c)
file(GLOB libs_lzfse libs/lzfse/src/*.c)
file(GLOB libs_hsel libs/hsel/myhsel.cpp libs/hsel/HSEL.cpp)
file(GLOB libs_glza libs/glza/GLZAmodel.c)
file(GLOB libs_lzo libs/lzo/src/*.c)
file(GLOB libs_ucl libs/ucl/src/n*.c libs/ucl/src/alloc.c)
file(GLOB libs_liblzs libs/liblzs/lzs-compression.c libs/liblzs/lzs-decompression.c)
file(GLOB libs_bzip2 libs/bzip2/*.c)
file(GLOB libs_lzw-ab libs/lzw-ab/lzw-lib.c)
file(GLOB libs_cryptohash-sha1 libs/cryptohash-sha1/*.c)
file(GLOB libs_capstone libs/capstone/*.c libs/capstone/arch/AArch64/*.c libs/capstone/arch/ARM/*.c libs/capstone/arch/Mips/*.c libs/capstone/arch/PowerPC/*.c libs/capstone/arch/Sparc/*.c libs/capstone/arch/SystemZ/*.c libs/capstone/arch/X86/*.c libs/capstone/arch/XCore/*.c)
file(GLOB libs_lua libs/lua/src/*.c)
# extra
file(GLOB extra_MemoryModule extra/MemoryModule.c)
file(GLOB libs_amiga libs/amiga/amiga.s)

# Source
set(SOURCE_QUICKBMS
   quickbms.c #error
   #${libs_zlib} #link
   #${compression} #error
   #${encryption} #error
   #${libs_lzma}
   #${libs_mspack}
   #${libs_ppmd}
   #${libs_aplib}
   #${libs_brieflz}
   #${libs_zziplib}
   #${libs_bcl}
   #${libs_szip}
   #${libs_lzhl}
   #${libs_tdcb}
   #${extra_mem2mem}
      #${libs_libkirk} #error
   #${libs_7z_advancecomp} #link
   #${libs_iris} #macro
   #${libs_old_cabextract}
   #${libs_mrci}
   #${libs_lz4} #link
   #${libs_snappy}
   #${libs_mmini}
   #${libs_clzw} #link
   #${libs_lzlib}
   #${libs_blosc} #macro
      #${libs_gipfeli} #error
   #${libs_liblzg}
   #${libs_doboz}
   #${libs_sphlib}
   #${libs_shadowforce}
   #${libs_zstd_aluigi} #link
   #${libs_azo} #link
   #${libs_nintendo_ds}
   #${libs_ctw}
   #${libs_grzip}
   #${libs_heatshrink}
   #${libs_libzling}
   #${libs_libcsc} #macro
   #${libs_density} #macro
   #${libs_spookyhash}
   #${libs_brotli}
   #${libs_libbsc} #macro #link
   #${libs_shoco}
   #${libs_ms-compress}
      #${libs_lzjody} #error
   #${libs_disasm}
      #${libs_mydownlib} #error
   #${libs_TurboRLE} #macro #link
   #${libs_lhasa}
   #${libs_dipperstein} #macro #link
   #${libs_liblzf}
   #${libs_zopfli}
      #${libs_lzham_codec} #error
   #${libs_dmsdos}
      #${libs_tornado} #error #unistd
   #${libs_PKLib}
   #${extra_mybits}
   #${libs_lz5} #link
   #${libs_lizard} #link
      #${libs_ppmz2} #error
   #${libs_libdivsufsort}
   #${libs_xxhash}
      #${extra_xalloc} #error
   #${libs_lzfse}
   #${libs_hsel} #macro
   #${libs_glza}
   #${libs_lzo}
      #${libs_ucl} #error
   #${libs_liblzs}
   #${libs_bzip2} #macro
   #${libs_zlib} #link
   #${libs_lzw-ab}
      #${libs_cryptohash-sha1} #error
   #${libs_capstone}
   #${libs_lua} #macro
   # extra
   #${extra_MemoryModule}
   #${libs_amiga}
)

# Include
set(INCLUDE_QUICKBMS
   libs/libmcrypt
   libs/libtomcrypt
   libs/zlib
   libs/zstd_aluigi
   libs/zstd_aluigi/common
   libs/zstd_aluigi/legacy
   libs/ecrypt/include
   libs/libcsc
   libs/brotli/include
   libs/lzham_codec/include
   libs/lzham_codec/lzhamcomp
   libs/lzham_codec/lzhamdecomp
   libs/lzo/include
   libs/ucl
   libs/ucl/include
   libs/capstone/include
)

# Executable
add_executable(${PROJECT_NAME} ${SOURCE_QUICKBMS})
find_package(OpenSSL REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_QUICKBMS})

# Library
set(QBMS_DEPS C:/ProgramData/chocolatey/lib/mingw/tools/install/mingw64/lib)
set(LIBRARY_QUICKBMS
   OpenSSL::SSL
   compression/jcalg1_static.lib
   #libs/zenlib/ZenLib.obj
   #${QBMS_DEPS}/xcompress.lib
   #${QBMS_DEPS}/libssl.a
   #${QBMS_DEPS}/libcrypto.a
   #libs/libmcrypt/libmcrypt.a
   #libs/libtomcrypt/libtomcrypt.a
   #libs/libtommath/libtommath.a
)
target_link_libraries(${PROJECT_NAME} PUBLIC ${LIBRARY_QUICKBMS})
target_compile_options(${PROJECT_NAME} PUBLIC -s -O2 -m32 -mtune=generic -static
   -Wno-int-to-pointer-cast -D_GLIBCXX_IOSTREAM -Wl,--enable-stdcall-fixup -Wl,--large-address-aware -fno-unit-at-a-time -Wno-sign-compare -Wno-sequence-point -Wno-parentheses -Wl,--stack,4194304 -fstack-protector-all -Wstack-protector -fwrapv --param ssp-buffer-size=1 -fno-strict-overflow -Wno-missing-field-initializers -Wformat-security  -Wl,--dynamicbase -Wl,--nxcompat -Wl,--enable-auto-image-base -Wl,--enable-auto-import -fno-omit-frame-pointer -fstack-protector-all -Wall -Wno-pointer-sign -Wno-unused-variable -Wno-unused-function -Wno-cpp -Wno-misleading-indentation -Wno-stack-protector -Wno-unknown-pragmas #-Wno-reorder
   -Wno-comment -Wno-format-security -Wno-implicit-function-declaration -Wno-deprecated-declarations -Wno-int-in-bool-context -Wno-array-bounds -Wno-stringop-truncation)
#target_compile_options(${PROJECT_NAME} PUBLIC -lcomdlg32 icon.o)
target_compile_options(${PROJECT_NAME} PUBLIC -lgdi32 -lstdc++ -lcrypt32 -lm -lws2_32 -lwinmm -lbfd -liberty -liconv -limagehlp)
target_compile_options(${PROJECT_NAME} PUBLIC -Wl,--add-stdcall-alias -Wl,--export-all-symbols -Wl,--exclude-libs,xcompress.lib)

#target_compile_definitions(${PROJECT_NAME} PUBLIC DISABLE_MCRYPT=1 DISABLE_TOMCRYPT=1)
#target_compile_definitions(${PROJECT_NAME} PUBLIC DISABLE_SSL=1)
#target_compile_definitions(${PROJECT_NAME} PUBLIC _7ZIP_ST=1 E_INVALIDARG=-1)

target_compile_definitions(${PROJECT_NAME} PUBLIC
   stricoll=_stricoll
   # source
   _7Z_TYPES_=1
   DENSITY_FORCE_INLINE=inline
   restrict=__restrict__
   MYDOWN_SSL=1
   MYDOWN_GLOBAL_COOKIE=1
   LZHAM_ANSI_CPLUSPLUS=1
   CAPSTONE_X86_ATT_DISABLE=1
   CAPSTONE_USE_SYS_DYN_MEM=1
   CAPSTONE_HAS_ARM=1
   CAPSTONE_HAS_ARM64=1
   CAPSTONE_HAS_MIPS=1
   CAPSTONE_HAS_X86=1
   CAPSTONE_HAS_POWERPC=1
   CAPSTONE_HAS_SPARC=1
   CAPSTONE_HAS_SYSZ=1
   CAPSTONE_HAS_XCORE=1
   # extra
   malloc_usable_size=_msize
)

#install(TARGETS ${PROJECT_NAME} DESTINATION bin)