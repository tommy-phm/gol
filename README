# GoL - Conway's Game of Life

**GoL** is a 2D simulation of Conway's Game of Life, implemented in C. It was my first programming project and served as the final assignment for CSE 113. The project demonstrates the evolution of cellular automata. Detailed instructions can be found in `cse113_game_of_life.pdf`. Patterns are included in `conwaylife.tar.gz`.

![Gif](gol.gif)  
*Quasar Pattern*

---

## Features

- **2D Graphics** powered by SDL2.
- **Customizable Settings**: 
  - Screen resolution
  - Background color (RGB values)
  - Sprite size
  - Initial coordinates of patterns
- **Edge Types**:
  - Hedge (bounded grid)
  - Torus (wraparound edges)
  - Klein (non-orientable wraparound)
- **Pattern Import**:
  - Supports Life pattern file format v1.06.
  

---

## Compiling the Program

To compile on Linux:  

1. Install the `libsdl2-dev` package.  
2. Run the `make` command in the project directory.  

---

## Usage

The program is executed with the following command:  
```bash
life [options]
```
If no arguments are provided, the program defaults to running the Blinker pattern with standard settings.

### Command-Line Options

| Option     | Description                                                                                   |
|------------|-----------------------------------------------------------------------------------------------|
| `-w`       | Screen width (e.g., `640`, `800`, `1024`, etc.).                                              |
| `-h`       | Screen height (e.g., `480`, `600`, `768`, etc.).                                              |
| `-e`       | Edge type (`hedge`, `torus`, or `klein`).                                                     |
| `-r`       | Red color value (0–255).                                                                     |
| `-g`       | Green color value (0–255).                                                                   |
| `-b`       | Blue color value (0–255).                                                                    |
| `-s`       | Sprite size (`2`, `4`, `8`, or `16`).                                                        |
| `-f`       | Filename of a Life pattern (in file format v1.06).                                           |
| `-o`       | Initial coordinates for the pattern in file `-f` (format: `x,y` with no spaces).             |
| `-H`       | Display usage information and a brief description of all options. Note: Capitalized `-H`.    |
| `-P`       | Filename of an additional Life pattern (in file format v1.06).                               |
| `-Q`       | Filename of a Life pattern (in file format v1.05).                                           |
| `-p`       | Initial coordinates for pattern `-P` (format: `x,y` with no spaces).                         |
| `-q`       | Initial coordinates for pattern `-Q` (format: `x,y` with no spaces).                         |

### Example Command

```bash
life -w 640 -h 480 -r 100 -g 150 -b 200 -s 2 -f glider_106.lif -o 150,200 -e torus
```

This runs the program with:  
- Screen resolution: `640x480`  
- Pixel color: `RGB(100, 150, 200)`  
- Sprite size: `2x2` pixels  
- Pattern: `glider_106.lif` at coordinates `(150,200)`  
- Edge type: `torus`.