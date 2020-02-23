# En este archivo describirá los pasos a seguir para
# que los nombres de carpetas de las dependencias coincidan


# GENERAL (IMPORTANTE!)
- Para todas las librerias en las que sea necesario utilizar CMake para construir las soluciones,
  utilizad carpetas llamadas "Build" (para x64) y "Buildx86" (para x86) dentro de las carpetas de las
  librerías, la ruta quedaria algo como "../Dependencies/ogre-1.12.5/Build" y ""../Dependencies/ogre-1.12.5/Buildx86"


# FMOD-2.00.07
- Utilizar el instalador que hay en un .zip
- Al instalar elegir una ruta de facil acceso.
- Se os generará una carpeta llamada "FMOD Studio API Windows", copia todas las carpetas (bin, api, doc, plugins) que contiene.
- En la carpeta Dependencies de Gaia, crea una carpeta llamada "fmod-2.00.07" y pega dentro de esta las carpetas copiadas.
- La carpeta "api" contiene los .dll y .lib, asi que cambiad el nombre de la carpeta a "build" (minusculas)
- Navega por todas las carpetas de "build" (la antigua "api") y cambia todas las carpetas con nombre "inc" a "include"
- La carpeta generada por el instalador, contiene un uninstall.exe, ejecutadlo para eliminarlo de vuestro sistema
  (solo nos interesa tenerlo en Gaia)


# SDL2-2.0.10
- Utilizar CMake de la manera descrita en el apartado GENERAL
- IMPORTANTE: la compilacion de SDL falla si la ruta donde construis la solucion tiene algun espacio.


# NOTAS ADICIONALES
- Si surge algún problema con las dependencias, se deberá de escribir en este documento el error y la solucion