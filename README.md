# Manual de Uso de HEX-Viewer
![CleanShot 2024-02-21 at 11 46 13](https://github.com/zsantiago8712/HEX-Viewer/assets/59674428/68b26684-eb8c-47c5-9f16-258b8dc7d308)

## Introducción

Este manual proporciona instrucciones para el uso del repositorio, que permite la visualización del contenido de archivos en formato hexadecimal, ASCII y muestra el offset hexadecimal correspondiente. Este proyecto integra funcionalidades de logging y gestión de memoria avanzadas gracias a las librerías `clog` y `offsetAllocator`.

## Requisitos Previos

Para compilar y ejecutar este proyecto, necesitarás:

- [Premake](https://premake.github.io/download.html): una herramienta de configuración de proyectos utilizada para generar makefiles y proyectos de solución para diferentes sistemas y compiladores.

## Uso del Programa

![CleanShot 2024-02-21 at 11 56 21](https://github.com/zsantiago8712/HEX-Viewer/assets/59674428/e373e62f-119f-44ff-8812-00e570442c16)


1. Clonar el repositorio:

   ```shell
   git clone [URL de tu repositorio]
   ```

2. Navegar al directorio del repositorio:

   ```shell
   cd [Nombre del Repositorio]
   ```

3. Generar los makefiles o proyectos de solución con Premake:

   ```shell
   premake5 gmake2
   ```

4. Compila el programa

### Para compilar en modo Debug

```shell
make
```

### Para compilar en modo Release

```shell
make config=release
```

5. Ejecutar el programa
   _Se recomineda tener tu terminal en pantalla completa sobre todo si quieres leer archivos grandes_

### Para ejecutar en modo Debug

```shell
./bin/Debug/TestBed
```

### Para ejecutar en modo Release

```shell
./bin/Release/TestBed
```


## Créditos y Agradecimientos

Este proyecto hace uso de las siguientes librerías, las cuales ya están incluidas en el código fuente:

- `clog`: una librería de logging desarrollada por mi, que facilita la generación de registros detallados de la ejecución del programa. El código fuente está disponible en `Utils/clog`.
- `offsetAllocator`: una librería de gestión de memoria, originalmente escrita por [sebbi](https://github.com/sebbbi/OffsetAllocator) y migrada a C por mi. Esta librería se utiliza para asignar memoria de manera eficiente y está integrada en `Utils/offsetAllocator`.

## Contribucio![CleanShot 2024-02-21 at 11 56 21](https://github.com/zsantiago8712/HEX-Viewer/assets/59674428/0f8b18ab-aab4-4f44-be26-6643c8aa4cd2)
nes

Las contribuciones son bienvenidas. Si deseas mejorar el proyecto, por favor revisa las pautas para contribuir en `CONTRIBUTING.md`.
