# Manual de Uso de HEX-Viewer
![CleanShot 2024-02-21 at 11 46 13](https://github.com/zsantiago8712/HEX-Viewer/assets/59674428/68b26684-eb8c-47c5-9f16-258b8dc7d308)

## Introducción

Este manual proporciona instrucciones para el uso del repositorio, que permite la visualización del contenido de archivos en formato hexadecimal, ASCII y muestra el offset hexadecimal correspondiente. Este proyecto integra funcionalidades de logging y gestión de memoria avanzadas gracias a las librerías `clog` y `offsetAllocator`.

## Requisitos Previos

Antes de comenzar, asegúrate de tener instalado xmake en tu sistema. xmake es un sistema de construcción de proyectos multiplataforma. Si aún no lo has instalado, puedes encontrar las instrucciones de instalación en https://xmake.io.

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

3. Para la construcción y la compilación del proyecto:

   ```shell
   xmake
   ```


4. Ejecutar el programa
   _Se recomineda tener tu terminal en pantalla completa sobre todo si quieres leer archivos grandes_

### Para ejecutar en modo Debug

```shell
xmake run
```


## Créditos y Agradecimientos

Este proyecto hace uso de las siguientes librerías, las cuales ya están incluidas en el código fuente:

- `clog`: una librería de logging desarrollada por mi, que facilita la generación de registros detallados de la ejecución del programa. El código fuente está disponible en `Utils/clog`.
- `offsetAllocator`: una librería de gestión de memoria, originalmente escrita por [sebbi](https://github.com/sebbbi/OffsetAllocator) y migrada a C por mi. Esta librería se utiliza para asignar memoria de manera eficiente y está integrada en `Utils/offsetAllocator`.

## Contribucio![CleanShot 2024-02-21 at 11 56 21](https://github.com/zsantiago8712/HEX-Viewer/assets/59674428/0f8b18ab-aab4-4f44-be26-6643c8aa4cd2)
nes

Las contribuciones son bienvenidas. Si deseas mejorar el proyecto, por favor revisa las pautas para contribuir en `CONTRIBUTING.md`.
