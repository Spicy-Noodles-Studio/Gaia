# En este archivo se describen los pasos a seguir para
# la instalación de las librerías del motor


# GENERAL (IMPORTANTE!)
- Descargar el zip Dependencias del drive y extraer todas las librerías en Dependencies.
- Para todas las librerías en las que sea necesario utilizar CMake para construir las soluciones,
  utilizad carpetas llamadas "Build" (para x64) y "Buildx86" (para x86) dentro de las carpetas de las
  librerías. La ruta quedaría así "../Dependencies/ogre-1.12.5/Build" y ""../Dependencies/ogre-1.12.5/Buildx86".


# OGRE3D-1.12.5/BULLET3-2.89/JSON-3.7.3
- Misma instalación en los tres casos, usando CMake.
- IMPORTANTE: solo en Ogre3D, al hacer las soluciones con CMake, desmarcar la opcion de OVERLAY_IMGUI.


# FMOD-2.00.07
- Extraer todas las carpetas (bin, Build, doc, plugins) que contiene en la carpeta Dependencies de Gaia, dentro de una carpeta llamada "fmod-2.00.07".
- Navegad por todas las carpetas de "Build" y cambiad todas las carpetas con nombre "inc" a "include".