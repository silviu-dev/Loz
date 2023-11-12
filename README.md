# Loz

Loz is a C++ 17 implementation of the Bob Nystrom's programming language Lox described in his book, [Crafting Interpreters](https://www.craftinginterpreters.com/)


## Build the soruce code and run the interpreter 

Build dependencies:
- cmake version 3.20.0 or upper
- clang-format
- cppcheck
- lcov

Clone the project

```bash
  git clone https://github.com/silviu-dev/Loz.git
```

Go to the project directory

```bash
  cd Loz
```

Install dependencies

```bash
  ./installDeps.sh
```

Build the project

```bash
  ./build.sh
```

Run the interpreter in promt mode

```bash
  ./build/bin/bootstrap
```

Run the interpreter in file mode

```bash
  ./build/bin/bootstrap 'path to source file'
```
## Running Tests

Build UTs target, run the tests and coverage

```bash
  ./utsBuildAndRun.sh
```

Build UTs target, run the tests and memory checker 

```bash
  ./memchecker.sh
```


## License

This project is licensed under the MIT License, see [LICENSE](LICENSE)


[![MIT License](https://img.shields.io/badge/License-MIT-green.svg)](https://choosealicense.com/licenses/mit/)
