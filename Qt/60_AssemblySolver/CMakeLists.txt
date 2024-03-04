################################################################################
#                                                                              #
#  Copyright (c) 2016, C3D Labs LLC                                            #
#  All rights reserved.                                                        #
#                                                                              #
#  This file is part of C3DVision Library                                      #
#  http://c3dlabs.com                                                          #
#                                                                              #
#  Exclusive rights to C3D Vision, any copies or components (parts) thereof    #
#  and other intellectual property belong to C3D Labs, LLC (the Rightholder).  # 
#  The license rights and restrictions are regulated by the Agreement between  #
#  the User and the Rightholder.                                               #
#                                                                              #
################################################################################

project(60_AssemblySolver)

################################################################################
# Build the Tutorial/AssemblySolver executables
################################################################################
SET(SRC_AssemblySolver 
    ./assemblysolver.cpp 
    ./assemblysolver.h
    ./assemblywidget.cpp 
    ./assemblywidget.h
    ./prbase.cpp
    ./prbase.h
    ./prinsertItem.cpp
    ./prinsertItem.h
    ./prbinconstraint.cpp
    ./prbinconstraint.h
    ./prrotation.cpp
    ./prrotation.h
    ./reporter.cpp
    ./reporter.h
    ./main.cpp)

SET(MOC_Files ./assemblywidget.h ./reporter.h)

QT5_WRAP_CPP(MOC_Gui ${MOC_Files})

SET(RCC_Files_AssemblySolver ./assemblywidget.qrc)
QT5_ADD_RESOURCES(RCC_Gui ${RCC_Files_AssemblySolver})

ADD_EXECUTABLE(${60_AssemblySolver_OUTPUT} WIN32 ${MOC_Gui} ${RCC_Gui} ${SRC_AssemblySolver} ${SRC_Shared})
TARGET_LINK_LIBRARIES(${60_AssemblySolver_OUTPUT} ${C3D_MATH_LIB} ${C3D_VIS_LIB} ${QtVision_OUTPUT} ${OPENGL_LIBRAREIS} ${STDLIB})

SET_TARGET_PROPERTIES(${60_AssemblySolver_OUTPUT} PROPERTIES FOLDER ${Examples_OUTPUT}/Qt)
qt5_use_modules( ${60_AssemblySolver_OUTPUT} Core Gui OpenGL Widgets )
###############################################################################
