# ood_map_reduce


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
