# Tiny Bazooka

Juego de plataformas 2D desarrollado en C++ con SFML 3. Esquiva enemigos, salta entre plataformas y dispara cohetes para conseguir puntos. La partida termina cuando pierdes las tres vidas.

## Características

- Movimiento lateral, salto y doble salto.
- Tres tipos de enemigos: terrestres, aéreos y saltadores.
- Disparo con un máximo de cinco cohetes simultáneos.
- Niveles de dificultad fácil, media y difícil; la dificultad aumenta al conseguir puntos.
- Mapa de plataformas con colisiones, agua y pinchos.
- Cámara que sigue al jugador, fondos con parallax y música y efectos de sonido.
- Medición integrada de rendimiento para comparar el dibujado optimizado del mapa.

## Controles

| Tecla | Acción |
|---|---|
| `↑` | Saltar; vuelve a pulsar en el aire para hacer un doble salto |
| `←` / `→` | Moverse |
| `↓` / `Espacio` | Disparar |
| `P` | Pausar o reanudar la partida |
| `Esc` | Volver al menú durante la partida |
| `F2` | Activar o desactivar el límite de FPS |
| `F4` | Cambiar entre el dibujado optimizado y el modo de comparación del mapa |
| `F5` | Ejecutar la medición de rendimiento |
| `F11` | Alternar entre pantalla completa y modo ventana |

En el menú, usa `↑` y `↓` para elegir una opción; `Enter` o `Espacio` para aceptar; y `←` o `→` para cambiar la dificultad o el sonido. La opción **Controls / Tutorial** muestra las instrucciones dentro del juego.

## Requisitos

- Windows 10 u 11.
- Visual Studio 2022 con la carga de trabajo **Desarrollo para el escritorio con C++**.
- SFML 3 para Visual C++ 17 (MSVC 2022), de 64 bits.

El proyecto está configurado para encontrar SFML en `C:\SFML64`. Si la instalas en otra ubicación, cambia las rutas de inclusión y de bibliotecas en las propiedades del proyecto. Usa una distribución de SFML compatible con MSVC 2022 y con la configuración que quieras ejecutar (`Debug` o `Release`).

## Compilar y ejecutar

1. Clona o descarga este repositorio.
2. Abre `PDGV_motor.sln` con Visual Studio 2022.
3. Selecciona la plataforma `x64` y la configuración `Debug` o `Release`.
4. Compila la solución y ejecuta el proyecto con `F5`.

Los recursos del juego se cargan desde rutas relativas como `Assets/graphics/` y `Assets/audio/`. Ejecuta el juego con la carpeta de este proyecto como directorio de trabajo para que encuentre las imágenes, fuentes, sonidos y el nivel. El proyecto incluye las DLL de SFML junto a los archivos del juego; al sustituirlas, asegúrate de que coincidan con la versión y configuración de la biblioteca usada para compilar.

## Estructura

```text
.
├── Assets/
│   ├── audio/       # Música y efectos
│   ├── fonts/       # Fuentes de la interfaz
│   ├── graphics/    # Personajes, enemigos, tiles y fondos
│   └── levels/      # Definición del nivel
├── PDGV_motor.sln
├── PDGV_motor.vcxproj
└── *.cpp / *.h      # Código fuente
```

### Componentes principales

| Archivo | Responsabilidad |
|---|---|
| `Game.h` / `Game.cpp` | Ventana, bucle principal, pantallas, cámara, entidades, colisiones y puntuación |
| `Heroi.h` / `Heroi.cpp` | Movimiento, gravedad, doble salto, animación y daño del jugador |
| `TileMap.h` / `TileMap.cpp` | Carga del nivel, colisiones con el terreno y dibujado del mapa |
| `Bala.h` / `Bala.cpp` | Cohetes disparados por el jugador |
| `Enemic*.h` / `Enemic*.cpp` | Clase base y variantes de enemigos |
| `DificultatEstat.h` y `Estat*.cpp` | Estados de dificultad y progresión de la partida |
| `Parallax.h` / `Parallax.cpp` | Fondos con movimiento parallax |
| `Menu.*` y `Hud.*` | Menú, tutorial e información de la partida |
| `GestorRecursos.h` | Carga y reutilización de fuentes y sonidos |
| `Config.h` | Constantes, velocidades y rutas de los recursos |

## Nivel

El nivel se define en `Assets/levels/nivell1.txt`. Cada carácter representa una casilla:

| Símbolo | Elemento |
|---|---|
| `.` | Espacio vacío |
| `G`, `T`, `R` | Superficies sólidas |
| `P` | Plataforma que se atraviesa desde abajo |
| `A` | Agua peligrosa |
| `S` | Pinchos peligrosos |
| `H` | Posición inicial del jugador |

## Medición de rendimiento

Pulsa `F5` durante la partida para ejecutar una prueba con una escena y una secuencia reproducibles. El resultado muestra FPS medios, FPS mínimos y tiempo medio por fotograma. `F4` cambia el modo de dibujado del mapa para comparar la versión optimizada con el modo anterior.
