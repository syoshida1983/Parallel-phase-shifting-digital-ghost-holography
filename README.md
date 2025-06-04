# Parallel-phase-shifting-digital-ghost-holography
[![DOI](https://img.shields.io/badge/DOI-10.48550/arXiv.2505.16454-blue.svg)](https://doi.org/10.48550/arXiv.2505.16454)

## SPI_server

### Build
SPI_server is developed with [Visual Studio 2022](https://visualstudio.microsoft.com/free-developer-offers/) on Windows and depends on [OpenCV](https://opencv.org/). To build SPI_server, install Visual Studio 2022 and OpenCV with [vcpkg](https://github.com/microsoft/vcpkg). After installing vcpkg according to the instructions in the above link, install OpenCV and integrate them with Visual Studio using the following command.

```console
> vcpkg install opencv:x64-windows
> vcpkg integrate install
```

To build and run the program, open `SPI_server.sln` in Visual Studio and type `Ctrl+Shift+B`.

The command line options `-m`, `-p`, `-f`, `-l`, and `-b` must be specified at runtime.
```console
options:
  -h [ --help ]           produce help message
  -m [ --monitor ] arg    set monitor number
  -p [ --power ] arg      set pattern size in powers of 2
  -f [ --factor ] arg     set enlargement factor
  -l [ --level ] arg      set gray level of white pixel
  -b [ --background ] arg set gray level of background
```

By sending an integer value of type `int32_t` to the named pipe `\\.\pipe\SPI`, SPI_server displays the corresponding spatial orthogonal pattern on the specified monitor.
The integer values $`0, 1, 2, 3,\dots`$ correspond to the following patterns: $`(i, j) = (1, 1), (2, 1), (1, 2), (3, 1), (2, 2), (1, 3),\dots`$.
If -1 is received, the program will terminate. For spatial orthogonal patterns, please take a look at the [paper](https://arxiv.org/abs/2505.16454).

<img src="https://github.com/syoshida1983/Parallel-phase-shifting-digital-ghost-holography/blob/images/orthogonal.jpg" width="50%" />

## SPI_client
SPI_client provides functions as a DLL that sends data to SPI_server using a named pipe.
The functions provided by `SPI_client.dll` are as follows.
- `int32_t open()` Creates the named pipe `\\.\pipe\SPI`. Returns -1 if creation fails, otherwise returns 0.
- `void close()` Closes the named pipe.
- `void write(const int32_t)` Sends an integer value of type `int32_t` to the named pipe.

## SPI.vi
SPI.vi is a LabVIEW VI file that displays spatial orthogonal patterns using SPI_server and SPI_client.

## PPSDGH.jl
PPSDGH.jl is Julia code that reconstructs wavefronts from measurement results.
The parameters can be set in lines 62 – 65.
CSV, DataFrames, Statistics, Images, and Hadamard packages are required to execute the code.
To install these packages, open the Julia REPL and run

```julia
julia> ]add CSV DataFrames Statistics Images Hadamard
```

As an example, we have attached the actual measurement results in measurements.csv.

## Citation
```BibTeX
@misc{2505.16454,
	author	= {Shuhei Yoshida},
	title	= {Parallel Phase-shifting Digital Ghost Holography},
	year	= {2025},
	eprint	= {arXiv:2505.16454},
}
```
