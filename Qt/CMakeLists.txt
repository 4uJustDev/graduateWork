################################################################################
#                                                                              #
#  Copyright (c) 2023, C3D Labs LLC                                            #
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

PROJECT(Examples)
cmake_minimum_required(VERSION 3.4)


################################################################################
# Build the Example/Test executables
################################################################################

IF(C3DVision_GUI_QT5_EXAMPLES)
    ###############################################################################
    # AssemblySolver
    SET(60_AssemblySolver_OUTPUT "60_AssemblySolver")
    ADD_SUBDIRECTORY(60_AssemblySolver)
    ###############################################################################
ENDIF()
