# Parallel-phase-shifting-digital-ghost-holography

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

## SPI_client

## SPI.vi

## Citation
```BibTeX
@misc{2505.16454,
	author	= {Shuhei Yoshida},
	title	= {Parallel Phase-shifting Digital Ghost Holography},
	year	= {2025},
	eprint	= {arXiv:2505.16454},
}
```
