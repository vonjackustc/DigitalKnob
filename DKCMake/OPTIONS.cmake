## TODO
## NOTE: lets look at this
## STRING(FIND "${CMAKE_BINARY_DIR}" "/win32" index)
## IF(${index} GREATER -1)
##	SET(WIN_32 ON)  ## THIS IS WRONG. This should set the working computer os
##	SET(OS "win32") ## This is the target os
##	STRING(REPLACE "/win32" "" DKPROJECT ${CMAKE_BINARY_DIR})
## ENDIF()


########### Initial setup ####################
IF(COMMAND cmake_policy)
	CMAKE_POLICY(SET CMP0003 NEW) ##what is this for?
ENDIF(COMMAND cmake_policy)
##SET(CMAKE_CONFIGURATION_TYPES Debug Release CACHE TYPE INTERNAL FORCE) #TODO: is this needed?


########### Determine the OS we are building for ####################
STRING(FIND "${CMAKE_BINARY_DIR}" "/win32" index)
IF(${index} GREATER -1)
	SET(WIN_32 ON) ##FIXME - this should be the OS we are building from. 
	SET(OS "win32")
	STRING(REPLACE "/win32" "" DKPROJECT ${CMAKE_BINARY_DIR})
ENDIF()
STRING(FIND "${CMAKE_BINARY_DIR}" "/win64" index)
IF(${index} GREATER -1)
	SET(WIN_64 ON)
	SET(OS "win64")
	STRING(REPLACE "/win64" "" DKPROJECT ${CMAKE_BINARY_DIR})
ENDIF()
STRING(FIND "${CMAKE_BINARY_DIR}" "/mac32" index)
IF(${index} GREATER -1)
	SET(MAC_32 ON)
	SET(OS "mac32")
	STRING(REPLACE "/mac32" "" DKPROJECT ${CMAKE_BINARY_DIR})
ENDIF()
STRING(FIND "${CMAKE_BINARY_DIR}" "/mac64" index)
IF(${index} GREATER -1)
	SET(MAC_64 ON)
	SET(OS "mac64")
	STRING(REPLACE "/mac64" "" DKPROJECT ${CMAKE_BINARY_DIR})
ENDIF()
STRING(FIND "${CMAKE_BINARY_DIR}" "/linux32" index)
IF(${index} GREATER -1)
	SET(LINUX_32 ON)
	SET(OS "linux32")
	STRING(REPLACE "/linux32" "" DKPROJECT ${CMAKE_BINARY_DIR})
ENDIF()
STRING(FIND "${CMAKE_BINARY_DIR}" "/linux64" index)
IF(${index} GREATER -1)
	SET(LINUX_64 ON)
	SET(OS "linux64")
	STRING(REPLACE "/linux64" "" DKPROJECT ${CMAKE_BINARY_DIR})
ENDIF()
STRING(FIND "${CMAKE_BINARY_DIR}" "/ios32" index)
IF(${index} GREATER -1)
	SET(IOS_32 ON)
	SET(OS "ios32")
	STRING(REPLACE "/ios32" "" DKPROJECT ${CMAKE_BINARY_DIR})
ENDIF()
STRING(FIND "${CMAKE_BINARY_DIR}" "/ios64" index)
IF(${index} GREATER -1)
	SET(IOS_64 ON)
	SET(OS "ios64")
	STRING(REPLACE "/ios64" "" DKPROJECT ${CMAKE_BINARY_DIR})
ENDIF()
STRING(FIND "${CMAKE_BINARY_DIR}" "/iossim32" index)
IF(${index} GREATER -1)
	SET(IOSSIM_32 ON)
	SET(OS "iossim32")
	STRING(REPLACE "/iossim32" "" DKPROJECT ${CMAKE_BINARY_DIR})
ENDIF()
STRING(FIND "${CMAKE_BINARY_DIR}" "/iossim64" index)
IF(${index} GREATER -1)
	SET(IOSSIM_64 ON)
	SET(OS "iossim64")
	STRING(REPLACE "/iossim64" "" DKPROJECT ${CMAKE_BINARY_DIR})
ENDIF()
STRING(FIND "${CMAKE_BINARY_DIR}" "/android32" index)
IF(${index} GREATER -1)
	SET(ANDROID_32 ON)
	SET(OS "android32")
	STRING(REPLACE "/android32" "" DKPROJECT ${CMAKE_BINARY_DIR})
ENDIF()
STRING(FIND "${CMAKE_BINARY_DIR}" "/android64" index)
IF(${index} GREATER -1)
	SET(ANDROID_64 ON)
	SET(OS "android64")
	STRING(REPLACE "/android64" "" DKPROJECT ${CMAKE_BINARY_DIR})
ENDIF()
IF(NOT OS)
	MESSAGE("EXAMPLE: digitalknob/DKApps/MyApp/win32")
	MESSAGE(FATAL_ERROR "The binary directory must contain, win32,win64,mac32,mac64,linux32,linux64,ios32,ios64,iossim32,iossim64,android32 or android64")
	DKREMOVE(${CMAKE_BINARY_DIR})
ENDIF()

## we use /Debug and /Release folders for Linux and Android
## if they are present, remove them and let DEBUG and RELEASE flags deal with that later.
STRING(REPLACE "/Debug" "" DKPROJECT ${DKPROJECT})
STRING(REPLACE "/Release" "" DKPROJECT ${DKPROJECT})

#########################################################################
## Set variables for paths
###############################################################
GET_FILENAME_COMPONENT(DIGITALKNOB ${CMAKE_SOURCE_DIR} ABSOLUTE)
DKSET(DKCMAKE ${DIGITALKNOB}/DKCMake)
DKSET(DKPLUGINS ${DIGITALKNOB}/DKPlugins)
DKSET(3RDPARTY ${DIGITALKNOB}/3rdParty)
DKSET(DKIMPORTS ${3RDPARTY}/_DKIMPORTS)
DKSET(DKWEB http://127.0.0.1)



###########################################################################
## Get variables for Build Type
###########################################################################
OPTION(DEBUG "Build Debug Output" OFF)
OPTION(RELEASE "Build Release Output" OFF)
IF(NOT DEBUG)
IF(NOT RELEASE)
	MESSAGE(FATAL_ERROR "Please select Debug or Release mode")
ENDIF()
ENDIF()

###########################################################################
## Get variables for Build Level
###########################################################################
OPTION(BUILD "Simpily build the app or library" OFF)
OPTION(REBUILD "Rebuild the app" OFF)
OPTION(REBUILDALL "Rebuild the app and all dependencies" OFF)
IF(NOT BUILD)
IF(NOT REBUILD)
IF(NOT REBUILDALL)
	MESSAGE(FATAL_ERROR "Please select Build, Rebuild, or Rebuild All")
ENDIF()
ENDIF()
ENDIF()


###########################################################################
## Get variables for Library Build Type (STATIC or DYNAMIC)
###########################################################################
OPTION(STATIC "Build Static Libraries and Plugins" OFF)
OPTION(DYNAMIC "Build Dynamic Libraries and Plugins" OFF)
IF(NOT STATIC)
IF(NOT DYNAMIC)
	DKSET(STATIC ON)
	##MESSAGE(FATAL_ERROR "Please select STATIC or DYNAMIC build.")
ENDIF()
ENDIF()	


###########################################################################
## Get variables for OpenGL type
###########################################################################
OPTION(OPENGL2 "Set the OpenGL Version" OFF)


###########################################################################
## Get variables for CEF
###########################################################################
OPTION(DKCEF "Use Chromium Embeded Framework" OFF)


###########################################################################
## Set variables for OS selection
###########################################################################
IF(WIN_32)
	MESSAGE("Creating Windows x32 Project Files")
	DKSET(WIN ON)
	DKSET(OS win32)
	IF(DEBUG)
		DKSET(DEBUG Debug)
	ENDIF()
	IF(RELEASE)
		DKSET(RELEASE Release)
	ENDIF()
	DKSET(CMAKE_SKIP_RPATH ON)
ENDIF()

IF(WIN_64)
	MESSAGE("*** Creating Windows x64 Project Files ***")
	DKSET(WIN ON)
	DKSET(OS win64)
	IF(DEBUG)
		DKSET(DEBUG Debug)
	ENDIF()
	IF(RELEASE)
		DKSET(RELEASE Release)
	ENDIF()
	DKSET(CMAKE_SKIP_RPATH ON)
ENDIF()

IF(MAC_32)
	MESSAGE("*** Creating Mac x32 Project Files ***")
	DKSET(MAC ON)
	DKSET(OS mac32)
	IF(DEBUG)
		DKSET(DEBUG Debug)
	ENDIF()
	IF(RELEASE)
		DKSET(RELEASE Release)
	ENDIF()
	DKSET(CMAKE_SKIP_RPATH ON)
ENDIF()

IF(MAC_64)
	MESSAGE("*** Creating Mac x64 Project Files ***")
	DKSET(MAC ON)
	DKSET(OS mac64)
	IF(DEBUG)
		DKSET(DEBUG Debug)
	ENDIF()
	IF(RELEASE)
		DKSET(RELEASE Release)
	ENDIF()
	DKSET(CMAKE_SKIP_RPATH ON)
ENDIF()

IF(IOS_32)
	MESSAGE("*** Creating iOS x32 Project Files ***")
	DKSET(IOS ON)
	DKSET(OS ios32)
	IF(DEBUG)
		DKSET(DEBUG Debug-iphoneos)
	ENDIF()
	IF(RELEASE)
		DKSET(RELEASE Release-iphoneos)
	ENDIF()
	DKSET(CMAKE_SKIP_RPATH ON)
ENDIF()

IF(IOS_64)
	MESSAGE("*** Creating iOS x64 Project Files ***")
	DKSET(IOS ON)
	DKSET(OS ios64)
	IF(DEBUG)
		DKSET(DEBUG Debug-iphoneos)
	ENDIF()
	IF(RELEASE)
		DKSET(RELEASE Release-iphoneos)
	ENDIF()
	DKSET(CMAKE_SKIP_RPATH ON)
ENDIF()

IF(IOSSIM_32)
	MESSAGE("*** Creating iOS-Simulator x32 Project Files ***")
	DKSET(IOSSIM ON)
	DKSET(OS iossim32)
	IF(DEBUG)
		DKSET(DEBUG Debug-iphonesimulator)
	ENDIF()
	IF(RELEASE)
		DKSET(RELEASE Release-iphonesimulator)
	ENDIF()
	DKSET(CMAKE_SKIP_RPATH ON)
ENDIF()

IF(IOSSIM_64)
	MESSAGE("*** Creating iOS-Simulator x64 Project Files ***")
	DKSET(IOSSIM ON)
	DKSET(OS iossim64)
	IF(DEBUG)
		DKSET(DEBUG Debug-iphonesimulator)
	ENDIF()
	IF(RELEASE)
		DKSET(RELEASE Release-iphonesimulator)
	ENDIF()
	DKSET(CMAKE_SKIP_RPATH ON)
ENDIF()

IF(LINUX_32)
	MESSAGE("*** Creating Linux x32 Project Files ***")
	DKSET(LINUX ON)
	DKSET(OS linux32)
	IF(DEBUG)
		DKSET(DEBUG Debug)
	ENDIF()
	IF(RELEASE)
		DKSET(RELEASE Release)
	ENDIF()
	DKSET(CMAKE_C_FLAGS "-m32")
	DKSET(CMAKE_CXX_FLAGS "-m32")
	DKSET(CMAKE_SKIP_RPATH ON)
ENDIF()

IF(LINUX_64)
	MESSAGE("*** Creating Linux x64 Project Files ***")
	DKSET(LINUX ON)
	DKSET(OS linux64)
	IF(DEBUG)
		DKSET(DEBUG Debug)
	ENDIF()
	IF(RELEASE)
		DKSET(RELEASE Release)
	ENDIF()
	DKSET(CMAKE_C_FLAGS "-m64")
	DKSET(CMAKE_CXX_FLAGS "-m64")
	#DKSET(CMAKE_SKIP_RPATH ON)
	
	SET(CMAKE_SKIP_BUILD_RPATH  FALSE)
	SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) 
	SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/")
	SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
	LIST(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/" isSystemDir)
	IF("${isSystemDir}" STREQUAL "-1")
		SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/")
	ENDIF("${isSystemDir}" STREQUAL "-1")
ENDIF()

IF(ANDROID_32)
	MESSAGE("*** Creating Android x32 Project Files ***")
	DKSET(ANDROID ON)
	DKSET(OS android32)
	IF(DEBUG)
		DKSET(DEBUG Debug)
	ENDIF()
	IF(RELEASE)
		DKSET(RELEASE Release)
	ENDIF()
	DKSET(CMAKE_SKIP_RPATH ON)
ENDIF()

IF(ANDROID_64)
	MESSAGE("*** Creating Android x64 Project Files ***")
	DKSET(ANDROID ON)
	DKSET(OS android64)
	IF(DEBUG)
		DKSET(DEBUG Debug)
	ENDIF()
	IF(RELEASE)
		DKSET(RELEASE Release)
	ENDIF()
	DKSET(CMAKE_SKIP_RPATH ON)
ENDIF()


###########################################################################
## Set variables for Build Type
###########################################################################
IF(DEBUG)
	DKSET(CMAKE_BUILD_TYPE DEBUG)
ENDIF()
IF(RELEASE)
	DKSET(CMAKE_BUILD_TYPE RELEASE)
ENDIF(RELEASE)


###########################################################################
## Set variables for Generator
###########################################################################

##### Microsoft Visual Studio 2019 #####
IF(CMAKE_GENERATOR STREQUAL "Visual Studio 16 2019")
	##DKSET(CMAKE_CXX_FLAGS "/DWIN32 /D_WINDOWS /W3 /nologo /GR /EHsc /Yustdafx.h /Zm500 /D_WIN32_WINNT=0x0600")
	DKSET(CMAKE_CXX_FLAGS "/DWIN32 /D_WINDOWS /W3 /nologo /GR /EHsc /Zm500 /D_WIN32_WINNT=0x0600 /D_USING_V110_SDK71_")
	IF(STATIC)
		DKSET(CMAKE_CXX_FLAGS_DEBUG "/MTd /Od /Ob0 /Zi /RTC1 /DDEBUG /D_DEBUG")
		DKSET(CMAKE_CXX_FLAGS_RELEASE "/MT /O2 /Ob2 /DNDEBUG")
	ENDIF()
	IF(DYNAMIC)
		DKSET(CMAKE_CXX_FLAGS_DEBUG "/MDd /Od /Ob0 /Zi /RTC1 /DDEBUG /D_DEBUG")
		DKSET(CMAKE_CXX_FLAGS_RELEASE "/MD /O2 /Ob2 /DNDEBUG")
	ENDIF()
	DKSET(CMAKE_COMMAND C:/Progra~2/CMake/bin/cmake.exe)
	DKSET(FLAGS 
	"/W3 /nologo /Zm500 /EHsc /GR /D_WIN32_WINNT=0x0600 /D_USING_V110_SDK71_ $<$<CONFIG:Debug>:/MTd /Od /Ob0 /Zi /RTC1 /DDEBUG /D_DEBUG> $<$<CONFIG:Release>:/MT /O2 /Ob2 /DNDEBUG>")
ENDIF()

##### Microsoft Visual Studio 2019 Win64 #####
IF(CMAKE_GENERATOR STREQUAL "Visual Studio 16 2019 Win64")
	##DKSET(CMAKE_CXX_FLAGS "/DWIN32 /D_WINDOWS /W3 /nologo /GR /EHsc /Yustdafx.h /Zm500 /D_WIN32_WINNT=0x0600")
	DKSET(CMAKE_CXX_FLAGS "/DWIN32 /DWIN64 /D_WINDOWS /W3 /nologo /GR /EHsc /Zm500 /D_WIN32_WINNT=0x0600")
	DKSET(CMAKE_CXX_FLAGS_DEBUG "/MTd /Od /Ob0 /Zi /RTC1 /DDEBUG /D_DEBUG")
	DKSET(CMAKE_CXX_FLAGS_RELEASE "/MT /O2 /Ob2 /DNDEBUG")
	DKSET(CMAKE_COMMAND C:/Progra~2/CMake/bin/cmake.exe)
	DKSET(FLAGS 
	"/DWIN64 /W3 /nologo /Zm500 /EHsc /GR /D_WIN32_WINNT=0x0600 $<$<CONFIG:Debug>:/MTd /Od /Ob0 /Zi /RTC1 /DDEBUG /D_DEBUG> $<$<CONFIG:Release>:/MT /O2 /Ob2 /DNDEBUG>")
ENDIF()


##### Apple Xcode #####
IF(CMAKE_GENERATOR STREQUAL "Xcode")
	DKSET(CMAKE_CXX_FLAGS "-x objective-c++")
ENDIF()

##### Unix Makefiles #####
IF(CMAKE_GENERATOR STREQUAL "Unix Makefiles")
	DKSET(CMAKE_CXX_FLAGS "-std=c++0x")
	DKSET(CMAKE_C_FLAGS -fPIC)
ENDIF()


















IF(WIN_32)
	ADD_DEFINITIONS(-DWIN32)
	ADD_DEFINITIONS(-D_WINDOWS)
	ADD_DEFINITIONS(-D__WINDOWS_MM__)
	ADD_DEFINITIONS(-D__STDC_CONSTANT_MACROS)
	ADD_DEFINITIONS(-D_SCL_SECURE_NO_WARNINGS)
	ADD_DEFINITIONS(-D_CRT_SECURE_NO_DEPRECATE)
ENDIF()

IF(WIN_64)
	ADD_DEFINITIONS(-DWIN32)
	ADD_DEFINITIONS(-DWIN64)
	ADD_DEFINITIONS(-D_WINDOWS)
	ADD_DEFINITIONS(-D__WINDOWS_MM__)
	ADD_DEFINITIONS(-D__STDC_CONSTANT_MACROS)
	ADD_DEFINITIONS(-D_SCL_SECURE_NO_WARNINGS)
	ADD_DEFINITIONS(-D_CRT_SECURE_NO_DEPRECATE)
ENDIF()

IF(MAC_32)
	ADD_DEFINITIONS(-DMAC32)
	ADD_DEFINITIONS(-DMAC)
	INCLUDE_DIRECTORIES(/usr/X11/include)
ENDIF()

IF(MAC_64)
	ADD_DEFINITIONS(-DMAC64)
	ADD_DEFINITIONS(-DMAC)
	INCLUDE_DIRECTORIES(/usr/X11/include)
ENDIF()

IF(IOS_32)
	ADD_DEFINITIONS(-DIOS32)
	ADD_DEFINITIONS(-DIOS)
	INCLUDE_DIRECTORIES(/usr/X11/include)
	LIST(APPEND App_SRC ${PATH_PLUGINS}/DK/DKiPhone.mm)
	#DKSET(FLAGS -DIOS)
ENDIF()

IF(IOS_64)
	ADD_DEFINITIONS(-DIOS64)
	ADD_DEFINITIONS(-DIOS)
	INCLUDE_DIRECTORIES(/usr/X11/include)
	LIST(APPEND App_SRC ${PATH_PLUGINS}/DK/DKiPhone.mm)
	#DKSET(FLAGS -DIOS)
ENDIF()

IF(IOSSIM_32)
	ADD_DEFINITIONS(-DIOS32)
	ADD_DEFINITIONS(-DIOS)
	ADD_DEFINITIONS(-DIOSSIM32)
	ADD_DEFINITIONS(-DIOSSIM)
	INCLUDE_DIRECTORIES(/usr/X11/include)
	LIST(APPEND App_SRC ${PATH_PLUGINS}/DK/DKiPhone.mm)
	DKSET(CMAKE_OSX_SYSROOT iphoneos)
	#DKSET(FLAGS -DIOSSIM)
ENDIF()

IF(IOSSIM_64)
	ADD_DEFINITIONS(-DIOS64)
	ADD_DEFINITIONS(-DIOS)
	ADD_DEFINITIONS(-DIOSSIM64)
	ADD_DEFINITIONS(-DIOSSIM)
	INCLUDE_DIRECTORIES(/usr/X11/include)
	LIST(APPEND App_SRC ${PATH_PLUGINS}/DK/DKiPhone.mm)
	DKSET(CMAKE_OSX_SYSROOT iphoneos)
	#DKSET(FLAGS -DIOSSIM)
ENDIF()

IF(LINUX_32)
	ADD_DEFINITIONS(-DLINUX32)
	ADD_DEFINITIONS(-DLINUX)
	INCLUDE_DIRECTORIES(/usr/X11/include)
	IF(DEBUG)
		ADD_DEFINITIONS(-DDEBUG)
	ENDIF()
ENDIF()

IF(LINUX_64)
	ADD_DEFINITIONS(-DLINUX64)
	ADD_DEFINITIONS(-DLINUX)
	INCLUDE_DIRECTORIES(/usr/X11/include)
	IF(DEBUG)
		ADD_DEFINITIONS(-DDEBUG)
	ENDIF()
ENDIF()

IF(OPENGL2)
	ADD_DEFINITIONS(-DUSE_SHADERS)
ENDIF()

IF(DKCEF)
	ADD_DEFINITIONS(-DUSE_DKCef)
ENDIF()
