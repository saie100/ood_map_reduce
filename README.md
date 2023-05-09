# ood_map_reduce

## Dynamic Link Library Commands 
### For Linux
generates object file using source code
```console
g++ [source code(.cpp file)] -c -fPIC
``` 

creates shared library using object files as input
```console
g++ -shared [object file of source code(.o file)] -o [pick a name for shared library(.so file)]
``` 

compiles main.cpp file using dynamic link library. Outputs "main" executable
```console
g++ main.cpp [shared library name(.so file)] -o main -ldl
``` 

## Unit Testing
This command creates a build folder with generated Makefile. (You should have a CMakeList.txt file in the same directory you run this command)
```console
cmake -B build
``` 
This command generates the unit test executable by executing the Makefile
```console
cd build
make
```
## Sequence Diagram

![seq_diagram](docs/sequence.drawio.svg)

## Development

The repo uses `pre-commit` for formatting (clang-format) and linting (cpplint). To enable pre-commit hooks:

```bash
#install pre-commit
pip install pre-commit

#alternatively you can use homebrew
brew install pre-commit

## install clang-format and cpplint
pip install clang-format

pip install cpplint

# install the git hook scripts
pre-commit install
```
