LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

OF_ROOT := ../..
FI_ROOT := $(OF_ROOT)/FreeImage-3.15.4/Source

LOCAL_MODULE := freeimage
LOCAL_MODULE_FILENAME := libfreeimage
  
LOCAL_CFLAGS := -fexceptions -DANDROID -DFREEIMAGE_FORCE_COLORORDER_RGB

ifeq ($(OS),Windows_NT)
LOCAL_SHORT_COMMANDS := true
endif

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(FI_ROOT) \
                    $(LOCAL_PATH)/$(FI_ROOT)/LibJPEG \
                    $(LOCAL_PATH)/$(FI_ROOT)/LibPNG \
                    $(LOCAL_PATH)/../../../webp/src 

LOCAL_EXPORT_C_INCLUDES :=$(LOCAL_C_INCLUDES)

LOCAL_SRC_FILES := \
	$(FI_ROOT)/FreeImage/BitmapAccess.cpp \
	$(FI_ROOT)/FreeImage/CacheFile.cpp \
	$(FI_ROOT)/FreeImage/ColorLookup.cpp \
	$(FI_ROOT)/FreeImage/Conversion.cpp \
	$(FI_ROOT)/FreeImage/Conversion16_555.cpp \
	$(FI_ROOT)/FreeImage/Conversion16_565.cpp \
	$(FI_ROOT)/FreeImage/Conversion24.cpp \
	$(FI_ROOT)/FreeImage/Conversion32.cpp \
	$(FI_ROOT)/FreeImage/Conversion4.cpp \
	$(FI_ROOT)/FreeImage/Conversion8.cpp \
	$(FI_ROOT)/FreeImage/ConversionFloat.cpp \
	$(FI_ROOT)/FreeImage/ConversionRGB16.cpp \
	$(FI_ROOT)/FreeImage/ConversionRGBF.cpp \
	$(FI_ROOT)/FreeImage/ConversionType.cpp \
	$(FI_ROOT)/FreeImage/ConversionUINT16.cpp \
	$(FI_ROOT)/FreeImage/FreeImage.cpp \
	$(FI_ROOT)/FreeImage/FreeImageC.c \
	$(FI_ROOT)/FreeImage/FreeImageIO.cpp \
	$(FI_ROOT)/FreeImage/GetType.cpp \
	$(FI_ROOT)/FreeImage/Halftoning.cpp \
	$(FI_ROOT)/FreeImage/J2KHelper.cpp \
	$(FI_ROOT)/FreeImage/MNGHelper.cpp \
	$(FI_ROOT)/FreeImage/MemoryIO.cpp \
	$(FI_ROOT)/FreeImage/MultiPage.cpp \
	$(FI_ROOT)/FreeImage/NNQuantizer.cpp \
	$(FI_ROOT)/FreeImage/PSDParser.cpp \
	$(FI_ROOT)/FreeImage/PixelAccess.cpp \
	$(FI_ROOT)/FreeImage/Plugin.cpp \
	$(FI_ROOT)/FreeImage/PluginPNG.cpp \
	$(FI_ROOT)/FreeImage/PluginJPEG.cpp \
	$(FI_ROOT)/FreeImage/PluginJ2K.cpp \
	$(FI_ROOT)/FreeImage/PluginJP2.cpp \
	$(FI_ROOT)/FreeImage/PluginPVR.cpp \
	$(FI_ROOT)/FreeImage/PluginWEBP.cpp
	
#	$(FI_ROOT)/FreeImage/PluginBMP.cpp \
#	$(FI_ROOT)/FreeImage/PluginCUT.cpp \
#	$(FI_ROOT)/FreeImage/PluginDDS.cpp \
#	$(FI_ROOT)/FreeImage/PluginGIF.cpp \
#	$(FI_ROOT)/FreeImage/PluginHDR.cpp \
#	$(FI_ROOT)/FreeImage/PluginICO.cpp \
#	$(FI_ROOT)/FreeImage/PluginIFF.cpp \
#	$(FI_ROOT)/FreeImage/PluginJNG.cpp \
#	$(FI_ROOT)/FreeImage/PluginKOALA.cpp \
#	$(FI_ROOT)/FreeImage/PluginMNG.cpp \
#	$(FI_ROOT)/FreeImage/PluginPCD.cpp \
#	$(FI_ROOT)/FreeImage/PluginPCX.cpp \
#	$(FI_ROOT)/FreeImage/PluginPFM.cpp \
#	$(FI_ROOT)/FreeImage/PluginPICT.cpp \
# 	$(FI_ROOT)/FreeImage/PluginXPM.cpp \
	 
LOCAL_SRC_FILES += $(FI_ROOT)/FreeImage/TIFFLogLuv.cpp \
	$(FI_ROOT)/FreeImage/ToneMapping.cpp \
	$(FI_ROOT)/FreeImage/WuQuantizer.cpp \
	$(FI_ROOT)/FreeImage/ZLibInterface.cpp \
	$(FI_ROOT)/FreeImage/tmoColorConvert.cpp \
	$(FI_ROOT)/FreeImage/tmoDrago03.cpp \
	$(FI_ROOT)/FreeImage/tmoFattal02.cpp \
	$(FI_ROOT)/FreeImage/tmoReinhard05.cpp \
	$(FI_ROOT)/FreeImageToolkit/BSplineRotate.cpp \
	$(FI_ROOT)/FreeImageToolkit/Background.cpp \
	$(FI_ROOT)/FreeImageToolkit/Channels.cpp \
	$(FI_ROOT)/FreeImageToolkit/ClassicRotate.cpp \
	$(FI_ROOT)/FreeImageToolkit/Colors.cpp \
	$(FI_ROOT)/FreeImageToolkit/CopyPaste.cpp \
	$(FI_ROOT)/FreeImageToolkit/Display.cpp \
	$(FI_ROOT)/FreeImageToolkit/FlipFI.cpp \
	$(FI_ROOT)/FreeImageToolkit/JPEGTransform.cpp \
	$(FI_ROOT)/FreeImageToolkit/MultigridPoissonSolver.cpp \
	$(FI_ROOT)/FreeImageToolkit/Rescale.cpp \
	$(FI_ROOT)/FreeImageToolkit/Resize.cpp \
	$(FI_ROOT)/LibJPEG/cdjpeg.c \
	$(FI_ROOT)/LibJPEG/jaricom.c \
	$(FI_ROOT)/LibJPEG/jcapimin.c \
	$(FI_ROOT)/LibJPEG/jcapistd.c \
	$(FI_ROOT)/LibJPEG/jcarith.c \
	$(FI_ROOT)/LibJPEG/jccoefct.c \
	$(FI_ROOT)/LibJPEG/jccolor.c \
	$(FI_ROOT)/LibJPEG/jcdctmgr.c \
	$(FI_ROOT)/LibJPEG/jchuff.c \
	$(FI_ROOT)/LibJPEG/jcinit.c \
	$(FI_ROOT)/LibJPEG/jcmainct.c \
	$(FI_ROOT)/LibJPEG/jcmarker.c \
	$(FI_ROOT)/LibJPEG/jcmaster.c \
	$(FI_ROOT)/LibJPEG/jcomapi.c \
	$(FI_ROOT)/LibJPEG/jcparam.c \
	$(FI_ROOT)/LibJPEG/jcprepct.c \
	$(FI_ROOT)/LibJPEG/jcsample.c \
	$(FI_ROOT)/LibJPEG/jctrans.c \
	$(FI_ROOT)/LibJPEG/jdapimin.c \
	$(FI_ROOT)/LibJPEG/jdapistd.c \
	$(FI_ROOT)/LibJPEG/jdarith.c \
	$(FI_ROOT)/LibJPEG/jdatadst.c \
	$(FI_ROOT)/LibJPEG/jdatasrc.c \
	$(FI_ROOT)/LibJPEG/jdcoefct.c \
	$(FI_ROOT)/LibJPEG/jdcolor.c \
	$(FI_ROOT)/LibJPEG/jddctmgr.c \
	$(FI_ROOT)/LibJPEG/jdhuff.c \
	$(FI_ROOT)/LibJPEG/jdinput.c \
	$(FI_ROOT)/LibJPEG/jdmainct.c \
	$(FI_ROOT)/LibJPEG/jdmarker.c \
	$(FI_ROOT)/LibJPEG/jdmaster.c \
	$(FI_ROOT)/LibJPEG/jdmerge.c \
	$(FI_ROOT)/LibJPEG/jdpostct.c \
	$(FI_ROOT)/LibJPEG/jdsample.c \
	$(FI_ROOT)/LibJPEG/jdtrans.c \
	$(FI_ROOT)/LibJPEG/jerror.c \
	$(FI_ROOT)/LibJPEG/jfdctflt.c \
	$(FI_ROOT)/LibJPEG/jfdctfst.c \
	$(FI_ROOT)/LibJPEG/jfdctint.c \
	$(FI_ROOT)/LibJPEG/jidctflt.c \
	$(FI_ROOT)/LibJPEG/jidctfst.c \
	$(FI_ROOT)/LibJPEG/jidctint.c \
	$(FI_ROOT)/LibJPEG/jmemansi.c \
	$(FI_ROOT)/LibJPEG/jmemmgr.c \
	$(FI_ROOT)/LibJPEG/jmemname.c \
	$(FI_ROOT)/LibJPEG/jmemnobs.c \
	$(FI_ROOT)/LibJPEG/jquant1.c \
	$(FI_ROOT)/LibJPEG/jquant2.c \
	$(FI_ROOT)/LibJPEG/jutils.c \
	$(FI_ROOT)/LibJPEG/rdbmp.c \
	$(FI_ROOT)/LibJPEG/rdcolmap.c \
	$(FI_ROOT)/LibJPEG/rdgif.c \
	$(FI_ROOT)/LibJPEG/rdppm.c \
	$(FI_ROOT)/LibJPEG/rdrle.c \
	$(FI_ROOT)/LibJPEG/rdswitch.c \
	$(FI_ROOT)/LibJPEG/rdtarga.c \
	$(FI_ROOT)/LibJPEG/transupp.c \
	$(FI_ROOT)/LibJPEG/wrbmp.c \
	$(FI_ROOT)/LibJPEG/wrgif.c \
	$(FI_ROOT)/LibJPEG/wrppm.c \
	$(FI_ROOT)/LibJPEG/wrrle.c \
	$(FI_ROOT)/LibJPEG/wrtarga.c \
	$(FI_ROOT)/LibOpenJPEG/bio.c \
	$(FI_ROOT)/LibOpenJPEG/cidx_manager.c \
	$(FI_ROOT)/LibOpenJPEG/cio.c \
	$(FI_ROOT)/LibOpenJPEG/dwt.c \
	$(FI_ROOT)/LibOpenJPEG/event.c \
	$(FI_ROOT)/LibOpenJPEG/image.c \
	$(FI_ROOT)/LibOpenJPEG/j2k.c \
	$(FI_ROOT)/LibOpenJPEG/j2k_lib.c \
	$(FI_ROOT)/LibOpenJPEG/jp2.c \
	$(FI_ROOT)/LibOpenJPEG/jpt.c \
	$(FI_ROOT)/LibOpenJPEG/mct.c \
	$(FI_ROOT)/LibOpenJPEG/mqc.c \
	$(FI_ROOT)/LibOpenJPEG/openjpeg.c \
	$(FI_ROOT)/LibOpenJPEG/phix_manager.c \
	$(FI_ROOT)/LibOpenJPEG/pi.c \
	$(FI_ROOT)/LibOpenJPEG/ppix_manager.c \
	$(FI_ROOT)/LibOpenJPEG/raw.c \
	$(FI_ROOT)/LibOpenJPEG/t1.c \
	$(FI_ROOT)/LibOpenJPEG/t2.c \
	$(FI_ROOT)/LibOpenJPEG/tcd.c \
	$(FI_ROOT)/LibOpenJPEG/tgt.c \
	$(FI_ROOT)/LibOpenJPEG/thix_manager.c \
	$(FI_ROOT)/LibOpenJPEG/tpix_manager.c \
	$(FI_ROOT)/LibPNG/png.c \
	$(FI_ROOT)/LibPNG/pngerror.c \
	$(FI_ROOT)/LibPNG/pngget.c \
	$(FI_ROOT)/LibPNG/pngmem.c \
	$(FI_ROOT)/LibPNG/pngpread.c \
	$(FI_ROOT)/LibPNG/pngread.c \
	$(FI_ROOT)/LibPNG/pngrio.c \
	$(FI_ROOT)/LibPNG/pngrtran.c \
	$(FI_ROOT)/LibPNG/pngrutil.c \
	$(FI_ROOT)/LibPNG/pngset.c \
	$(FI_ROOT)/LibPNG/pngtrans.c \
	$(FI_ROOT)/LibPNG/pngwio.c \
	$(FI_ROOT)/LibPNG/pngwrite.c \
	$(FI_ROOT)/LibPNG/pngwtran.c \
	$(FI_ROOT)/LibPNG/pngwutil.c \
	$(FI_ROOT)/Metadata/Exif.cpp \
	$(FI_ROOT)/Metadata/FIRational.cpp \
	$(FI_ROOT)/Metadata/FreeImageTag.cpp \
	$(FI_ROOT)/Metadata/IPTC.cpp \
	$(FI_ROOT)/Metadata/TagConversion.cpp \
	$(FI_ROOT)/Metadata/TagLib.cpp \
	$(FI_ROOT)/ZLib/adler32.c \
	$(FI_ROOT)/ZLib/compress.c \
	$(FI_ROOT)/ZLib/crc32.c \
	$(FI_ROOT)/ZLib/deflate.c \
	$(FI_ROOT)/ZLib/gzclose.c \
	$(FI_ROOT)/ZLib/gzlib.c \
	$(FI_ROOT)/ZLib/gzread.c \
	$(FI_ROOT)/ZLib/gzwrite.c \
	$(FI_ROOT)/ZLib/infback.c \
	$(FI_ROOT)/ZLib/inffast.c \
	$(FI_ROOT)/ZLib/inftrees.c \
	$(FI_ROOT)/ZLib/trees.c \
	$(FI_ROOT)/ZLib/uncompr.c \
	$(FI_ROOT)/ZLib/zutil.c \

LOCAL_STATIC_LIBRARIES := webp

include $(BUILD_STATIC_LIBRARY)
$(call import-module,libs/webp/Android_lib)