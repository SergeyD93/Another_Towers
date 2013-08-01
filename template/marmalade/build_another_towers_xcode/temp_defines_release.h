#ifndef AIRPLAYUSECHIPMUNK
#   define AIRPLAYUSECHIPMUNK 1
#endif
#ifndef CC_UNDER_MARMALADE
#   define CC_UNDER_MARMALADE 1
#endif
#ifndef DARWIN_NO_CARBON
#   define DARWIN_NO_CARBON 1
#endif
#ifndef FT_CONFIG_OPTION_NO_ASSEMBLER
#   define FT_CONFIG_OPTION_NO_ASSEMBLER 1
#endif
#ifndef I3D_OS_S3E
#   define I3D_OS_S3E 1
#endif
#ifndef I3D_PLATFORM_S3E
#   define I3D_PLATFORM_S3E 1
#endif
#ifndef IW_APP_NAME
#   define IW_APP_NAME "Another_Towers"
#endif
#ifndef IW_APP_VERSION
#   define IW_APP_VERSION "1.0"
#endif
#ifndef IW_ASSERTION_CHANNEL_DIALOG
#   define IW_ASSERTION_CHANNEL_DIALOG 1
#endif
#ifndef IW_ASSERTION_CHANNEL_HTTP
#   define IW_ASSERTION_CHANNEL_HTTP 1
#endif
#ifndef IW_MKF_COCOS2DX
#   define IW_MKF_COCOS2DX 1
#endif
#ifndef IW_MKF_COCOS2DX_EXT
#   define IW_MKF_COCOS2DX_EXT 1
#endif
#ifndef IW_MKF_COCOSDENSHION
#   define IW_MKF_COCOSDENSHION 1
#endif
#ifndef IW_MKF_FREETYPE
#   define IW_MKF_FREETYPE 1
#endif
#ifndef IW_MKF_IWGL
#   define IW_MKF_IWGL 1
#endif
#ifndef IW_MKF_IWHTTP
#   define IW_MKF_IWHTTP 1
#endif
#ifndef IW_MKF_IWUTIL
#   define IW_MKF_IWUTIL 1
#endif
#ifndef IW_MKF_LIBJPEG
#   define IW_MKF_LIBJPEG 1
#endif
#ifndef IW_MKF_LIBPNG
#   define IW_MKF_LIBPNG 1
#endif
#ifndef IW_MKF_LIBTIFF
#   define IW_MKF_LIBTIFF 1
#endif
#ifndef IW_MKF_LIBXML2
#   define IW_MKF_LIBXML2 1
#endif
#ifndef IW_MKF_PUGIXML
#   define IW_MKF_PUGIXML 1
#endif
#ifndef IW_MKF_S3EDIALOG
#   define IW_MKF_S3EDIALOG 1
#endif
#ifndef IW_MKF_ZLIB
#   define IW_MKF_ZLIB 1
#endif
#ifndef IW_PLATFORM_S3E
#   define IW_PLATFORM_S3E 1
#endif
#ifndef IW_SDK
#   define IW_SDK 1
#endif
#ifndef IW_TRACE_CHANNEL_DIALOG
#   define IW_TRACE_CHANNEL_DIALOG 1
#endif
#ifndef IW_TRACE_CHANNEL_DIALOG_VERBOSE
#   define IW_TRACE_CHANNEL_DIALOG_VERBOSE 2
#endif
#ifndef IW_TRACE_CHANNEL_HTTP
#   define IW_TRACE_CHANNEL_HTTP 1
#endif
#ifndef IW_TRACE_CHANNEL_HTTP_VERBOSE
#   define IW_TRACE_CHANNEL_HTTP_VERBOSE 1
#endif
#ifndef LIBXML_ATTR_ALLOC_SIZE
#   define LIBXML_ATTR_ALLOC_SIZE(x)  
#endif
#ifndef LIBXML_ATTR_FORMAT
#   define LIBXML_ATTR_FORMAT(fmt,args)  
#endif
#ifndef LIBXML_CATALOG_ENABLED
#   define LIBXML_CATALOG_ENABLED 1
#endif
#ifndef LIBXML_DEBUG_ENABLED
#   define LIBXML_DEBUG_ENABLED 1
#endif
#ifndef LIBXML_HTML_ENABLED
#   define LIBXML_HTML_ENABLED 1
#endif
#ifndef LIBXML_OUTPUT_ENABLED
#   define LIBXML_OUTPUT_ENABLED 1
#endif
#ifndef LIBXML_PUSH_ENABLED
#   define LIBXML_PUSH_ENABLED 1
#endif
#ifndef LIBXML_SAX1_ENABLED
#   define LIBXML_SAX1_ENABLED 1
#endif
#ifndef LIBXML_TREE_ENABLED
#   define LIBXML_TREE_ENABLED 1
#endif
#ifndef LIBXML_WRITER_ENABLED
#   define LIBXML_WRITER_ENABLED 1
#endif
#ifndef LIBXML_XINCLUDE_ENABLED
#   define LIBXML_XINCLUDE_ENABLED 1
#endif
#ifndef LIBXML_XPATH_ENABLED
#   define LIBXML_XPATH_ENABLED 1
#endif
#ifndef LIBXML_XPTR_ENABLED
#   define LIBXML_XPTR_ENABLED 1
#endif
#ifndef MARMALADE
#   define MARMALADE 1
#endif
#ifndef MARMALADEUSECHIPMUNK
#   define MARMALADEUSECHIPMUNK 1
#endif
#ifndef S3E_INCLUDE_LEGACY
#   define S3E_INCLUDE_LEGACY 1
#endif
#ifndef TRIO_FPCLASSIFY_HACK
#   define TRIO_FPCLASSIFY_HACK 1
#endif
#ifndef WITHOUT_TRIO
#   define WITHOUT_TRIO 1
#endif
#ifndef XMLCALL
#   define XMLCALL  
#endif
#ifndef XMLCDECL
#   define XMLCDECL  
#endif
#ifndef XMLPUBFUN
#   define XMLPUBFUN  
#endif
#ifndef XMLPUBVAR
#   define XMLPUBVAR extern
#endif
#ifndef __S3E__
#   define __S3E__ 1
#endif

/* including "extra_body" specified by the mkb plugin */

#undef win32

#ifndef unix
    #define unix
#endif

#ifndef __unix
    #define __unix
#endif

/*
#ifndef _REENTRANT
    #define _REENTRANT
#endif
*/

#if !defined I3D_ARCH_ARM && !defined I3D_ARCH_X86 && !defined I3D_ARCH_AMD64 && !defined I3D_ARCH_MIPS
    #if defined __ARMCC_VERSION || defined __ARMC__ || defined __arm__
        #define I3D_ARCH_ARM
    #elif defined __mipsel || defined __MIPSEL
        #define I3D_ARCH_MIPS
    #elif defined __amd64
        #define I3D_ARCH_AMD64
    #else
        #define I3D_ARCH_X86
    #endif
#endif

#if defined I3D_ARCH_X86 && !defined __i386__
    #define __i386__
#endif

#if defined I3D_ARCH_ARM && !defined __ARM__
    #define __ARM__
#endif

#if defined __cplusplus && defined _MSC_VER && _MSC_VER < 1400 && !defined IW_WCHAR_T_DEFINED && !defined __clang__
    // wchar_t is a builtin in C++ .. but not for MSVC <= 2003
    typedef unsigned short wchar_t;
    #define IW_WCHAR_T_DEFINED
#endif

#if defined _MSC_VER && !defined __clang__
    #pragma warning (disable: 4786)
#endif

#undef _WIN32
#undef WIN32
#undef __linux__
#undef __linux
#undef __gnu_linux__
#undef linux

#if defined __i386__ && defined __ANDROID__
#ifndef S3E_ANDROID_X86
#   define S3E_ANDROID_X86 1
#endif
#endif
#if !defined I3D_ARCH_ARM && !defined I3D_ARCH_X86
    #if defined __ARMCC_VERSION || defined __arm__
        #define I3D_ARCH_ARM
    #else
        #define I3D_ARCH_X86
        #define I3D_ARCH_I386
    #endif
#endif

#if defined I3D_ARCH_X86 && !defined __i386__
    #define __i386__
#endif

#if defined I3D_ARCH_ARM && !defined __ARM__
    #define __ARM__
#endif
