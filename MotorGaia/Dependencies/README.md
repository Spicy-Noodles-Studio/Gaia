# En este archivo se describen los pasos a seguir para
# la instalación de las librerías del motor


# GENERAL (IMPORTANTE!)
- Descargar el zip Dependencias del drive y extraer todas las librerías en Dependencies.
- Para todas las librerías en las que sea necesario utilizar CMake para construir las soluciones,
  utilizad carpetas llamadas "Build" (para x64) y "Buildx86" (para x86) dentro de las carpetas de las
  librerías. La ruta quedaría así "../Dependencies/ogre-1.12.5/Build" y ""../Dependencies/ogre-1.12.5/Buildx86".


# OGRE3D-1.12.5
- Misma instalación en los tres casos, usando CMake.
- IMPORTANTE: al hacer las soluciones con CMake, desmarcar la opcion de OVERLAY_IMGUI.

# JSON-3.7.3
- NO HACE FALTA COMPILAR NI HACER CMAKE; NI NADA!!

# BULLET3-2.89
- Misma instalación en los tres casos, usando CMake.
- MARCAR la opcion de USE_MSVC_RUNTIME_LIBRARY_DLL

# FMOD-2.00.07
- Extraer todas las carpetas (bin, Build, doc, plugins) que contiene en la carpeta Dependencies de Gaia, dentro de una carpeta llamada "fmod-2.00.07".
- Navegad por todas las carpetas de "Build" y cambiad todas las carpetas con nombre "inc" a "include".

# CEGUI-0.8.7
- Descargar el codigo fuente Y LAS DEPENDENCIAS desde http://cegui.org/download
- Extraer el codigo fuente y extraer las dependencias. Meter la carpeta de las dependencias en la carpeta del codigo fuente.
- Renombrar la carpeta de las dependencias a "dependencies"
- Usar CMake para compilar las dependencias de la misma manera que el resto de librerias. (Con carpetas Build y Buildx86 dentro de la carpeta de dependencias)
- Compilar las dependencias en todas las configuraciones y plataformas.
- Usar CMake para construir las soluciones de CEGUI. (¡¡IMPORTANTE SEGUIR PASOS!!)
- De nuevo, en las carpetas Build/Buildx86, se le dara a Configure.
- Cuando apararezcan las opciones a marcar, buscar "CMAKE_PREFIX_PATH"
- Cambiar CMAKE_PREFIX_PATH de ../Dependencies/cegui-0.8.7/dependencies a /Dependencies/cegui-0.8.7/dependencies/Build/dependencies
- Buscar "ogre" y marcar CEGUI_BUILD_RENDERER_OGRE
- Tambien hay que definir las siguientes rutas (las rutas que pongo son relativas, pero deben ser absolutas):
    - OGRE_H_BUILD_SETTINGS_PATH : ../Dependencies/ogre-1.12.5/Build/include
    - OGRE_H_PATH : ../Dependencies/ogre-1.12.5/OgreMain/include
    - OGRE_LIB : ../Dependencies/ogre-1.12.5/Build/lib/Release/OgreMain.lib
    - OGRE_LIB_DBG : ../Dependencies/ogre-1.12.5/Build/lib/Debug/OgreMain_d.lib
    **Nota: las rutas estan puestas para x64, para contruir la solucion x86 se debe usar Buildx86 en vez de Build

- Buscar "codec" y marcar CEGUI_BUILD_IMAGECODEC_SILLY
- Buscar "parser" y marcar CEGUI_BUILD_XMLPARSER_EXPAT
- Buscar "freetype" y marcar CEGUI_HAS_FREETYPE
- Buscar "pcre" y marcar CEGUI_HAS_PCRE_REGEX
- Volver a pulsar Configure.
- Pulsar Generate.
- Cuando intenteis compilar, os dará errores. Hay que corregir ciertas cosas.
- Buscar en archivo Config.h en ../Dependencies/cegui-0.8.7/Build/cegui/include/CEGUI y buscar (con CTRL+F) "ogre"
- Modificar las siguientes macros a estos valores (cambiar el 0 por el valor y guardar):
    - CEGUI_OGRE_VERSION_MAJOR 1
    - CEGUI_OGRE_VERSION_MINOR 12
    - CEGUI_OGRE_VERSION_PATCH 5

*Nota: si da un error con una declaracion Ogre::Image::Box, cambiarlo a Ogre::Box

