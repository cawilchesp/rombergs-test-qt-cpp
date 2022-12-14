QT       += core gui help

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG -= app_bundle
CONFIG -= debug_and_release debug_and_release_target
CONFIG += static
TEMPLATE = app
TRANSLATIONS += ingles.ts

SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    loadwindow.cpp \
    senales.cpp \
    parametros.cpp \
    helpbrowser.cpp \
    helpwindow.cpp \
    reporteditwindow.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    loadwindow.h \
    senales.h \
    parametros.h \
    helpbrowser.h \
    helpwindow.h \
    reporteditwindow.h

FORMS    += mainwindow.ui \
    loadwindow.ui \
    helpwindow.ui \
    reporteditwindow.ui

RESOURCES += \
    recursos.qrc

INCLUDEPATH += C:\\opencv\\4.1.0\\include
CONFIG(debug, debug|release) {
    TARGET = TestRombergV2_Debug
    win32 {
        LIBS += -LC:\\opencv\\4.1.0\\x86\\vc15\\lib \
            -lopencv_aruco410d \
            -lopencv_bgsegm410d \
            -lopencv_bioinspired410d \
            -lopencv_calib3d410d \
            -lopencv_ccalib410d \
            -lopencv_core410d \
            -lopencv_datasets410d \
            -lopencv_dnn410d \
            -lopencv_dnn_objdetect410d \
            -lopencv_dpm410d \
            -lopencv_face410d \
            -lopencv_features2d410d \
            -lopencv_flann410d \
            -lopencv_fuzzy410d \
            -lopencv_gapi410d \
            -lopencv_hfs410d \
            -lopencv_highgui410d \
            -lopencv_imgcodecs410d \
            -lopencv_imgproc410d \
            -lopencv_img_hash410d \
            -lopencv_line_descriptor410d \
            -lopencv_ml410d \
            -lopencv_objdetect410d \
            -lopencv_optflow410d \
            -lopencv_phase_unwrapping410d \
            -lopencv_photo410d \
            -lopencv_plot410d \
            -lopencv_quality410d \
            -lopencv_reg410d \
            -lopencv_rgbd410d \
            -lopencv_saliency410d \
            -lopencv_shape410d \
            -lopencv_stereo410d \
            -lopencv_stitching410d \
            -lopencv_structured_light410d \
            -lopencv_superres410d \
            -lopencv_surface_matching410d \
            -lopencv_text410d \
            -lopencv_tracking410d \
            -lopencv_video410d \
            -lopencv_videoio410d \
            -lopencv_videostab410d \
            -lopencv_xfeatures2d410d \
            -lopencv_ximgproc410d \
            -lopencv_xobjdetect410d \
            -lopencv_xphoto410d \
    }
} else {
    TARGET = TestRombergV2
    win32 {
        LIBS += -LC:\\opencv\\4.1.0\\x86\\vc15\\lib \
            -lopencv_aruco410 \
            -lopencv_bgsegm410 \
            -lopencv_bioinspired410 \
            -lopencv_calib3d410 \
            -lopencv_ccalib410 \
            -lopencv_core410 \
            -lopencv_datasets410 \
            -lopencv_dnn410 \
            -lopencv_dnn_objdetect410 \
            -lopencv_dpm410 \
            -lopencv_face410 \
            -lopencv_features2d410 \
            -lopencv_flann410 \
            -lopencv_fuzzy410 \
            -lopencv_gapi410 \
            -lopencv_hfs410 \
            -lopencv_highgui410 \
            -lopencv_imgcodecs410 \
            -lopencv_imgproc410 \
            -lopencv_img_hash410 \
            -lopencv_line_descriptor410 \
            -lopencv_ml410 \
            -lopencv_objdetect410 \
            -lopencv_optflow410 \
            -lopencv_phase_unwrapping410 \
            -lopencv_photo410 \
            -lopencv_plot410 \
            -lopencv_quality410 \
            -lopencv_reg410 \
            -lopencv_rgbd410 \
            -lopencv_saliency410 \
            -lopencv_shape410 \
            -lopencv_stereo410 \
            -lopencv_stitching410 \
            -lopencv_structured_light410 \
            -lopencv_superres410 \
            -lopencv_surface_matching410 \
            -lopencv_text410 \
            -lopencv_tracking410 \
            -lopencv_video410 \
            -lopencv_videoio410 \
            -lopencv_videostab410 \
            -lopencv_xfeatures2d410 \
            -lopencv_ximgproc410 \
            -lopencv_xobjdetect410 \
            -lopencv_xphoto410 \
    }
}

unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += opencv
}
