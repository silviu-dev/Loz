# Loz

C++ 17 implementation of the Bob Nystrom's programming language Lox described in his book, [Crafting Interpreters](https://www.craftinginterpreters.com/)


## Build the soruce code and run  

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

Run the interpreter

```bash
  ./build/program
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

This project is licensed under the MIT License, see LICENSE for more information.


[![MIT License](https://img.shields.io/badge/License-MIT-green.svg)](https://choosealicense.com/licenses/mit/)
