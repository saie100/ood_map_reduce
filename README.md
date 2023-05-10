# ood_map_reduce

## Commands to Build Dynamic Link Library (Linux)
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

## Commands to Build Dynamic Link Library (MAC)

```sh
g++ -std=c++17 -c -fPIC dll/ReduceDll.cpp -o libReduce.o
g++ -std=c++17 -c -fPIC src/FileManager.cpp -o FileManager.o

g++ -std=c++17 -shared -o libReduce.dylib libReduce.o FileManager.o

g++ -std=c++17 src/main.cpp src/Executive.cpp src/FileManager.cpp src/Map.cpp src/Sort.cpp src/Workflow.cpp -o main

# TODO Run the program.
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
