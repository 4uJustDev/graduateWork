﻿################################################################################
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

project(ZarubinGraduateWork)

# Добавление всех header файлов изглавной папки
file(GLOB SRC_Configurator_h CONFIGURE_DEPENDS "*.h")
SOURCE_GROUP(Headers FILES ${SRC_Configurator_h})

# Добавление всех файлов деталей из папки details
file(GLOB SRC_Details_cpp CONFIGURE_DEPENDS "./details/*.cpp")
SOURCE_GROUP(Sources FILES ${SRC_Details_cpp})

# Добавление всех source файлов изглавной папки
file(GLOB SRC_Configurator_cpp CONFIGURE_DEPENDS "*.cpp")
SOURCE_GROUP(Sources FILES ${SRC_Configurator_cpp})

# Добавление MOC файлов
SET(MOC_Files ${SRC_Configurator_h})
QT5_WRAP_CPP(MOC_Gui ${MOC_Files})

# Добавление qrc файлов
SET(RCC_Files_AssemblySolver ./assemblywidget.qrc)
QT5_ADD_RESOURCES(RCC_Gui ${RCC_Files_AssemblySolver})

# Создание зависимостей
ADD_EXECUTABLE(${60_AssemblySolver_OUTPUT} WIN32 ${MOC_Gui} ${RCC_Gui} ${SRC_Configurator_h} ${SRC_Configurator_cpp} ${SRC_Details_cpp} ${SRC_Shared})
TARGET_LINK_LIBRARIES(${60_AssemblySolver_OUTPUT} ${C3D_MATH_LIB} ${C3D_VIS_LIB} ${QtVision_OUTPUT} ${OPENGL_LIBRAREIS} ${STDLIB})

SET_TARGET_PROPERTIES(${60_AssemblySolver_OUTPUT} PROPERTIES FOLDER ${Examples_OUTPUT}/Qt)
qt5_use_modules( ${60_AssemblySolver_OUTPUT} Core Gui OpenGL Widgets )
###############################################################################
