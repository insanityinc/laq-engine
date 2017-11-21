# Typed Linear Algebra DBMS

<!--One Paragraph of project description goes here-->

## Getting Started

<!--These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.-->

### Prerequisites

Download or clone the project

```
git clone https://github.com/Hubble83/dbms.git
```

Install flex and bison (tested with **Flex 2.6.4** and **Bison 3.0.4**).

Install Google Protocol Buffers (follow Google tutorial or use this commands)

```
mkdir dbms/lib
cd dbms/lib
git clone https://github.com/google/protobuf.git
cd protobuf
sudo apt-get install autoconf automake libtool curl make g++ unzip
./autogen.sh
./configure
make
make check
sudo make install
sudo ldconfig
```

[Optional] Google cpplint used to check code syntax

```
mkdir dbms/lib
cd dbms/lib
git clone https://github.com/google/styleguide.git
```

[Optional] Google test used for unit testing, use your C++ compiler

```
CXX=g++-7.2

mkdir dbms/lib
cd dbms/lib
git clone https://github.com/google/googletest.git
GTEST_DIR=googletest/googletest
$CXX -isystem $GTEST_DIR/include -I$GTEST_DIR \
     -pthread -c $GTEST_DIR/src/gtest-all.cc
ar -rv libgtest.a gtest-all.o
rm gtest-all.o
```

### Installing

<!--A step by step series of examples that tell you have to get a development env running-->

<!--Say what the step will be-->

```
cd dbms
make
```

<!--And repeat-->

<!--```
until finished
```-->

<!--End with an example of getting some data out of the system or using it for a little demo-->

## Running the tests

<!--Explain how to run the automated tests for this system-->

<!--### Break down into end to end tests-->

<!--Explain what these tests test and why-->

```
cd dbms
make test
```

### Test code style

```
cd dbms
make linter
```

<!--Explain what these tests test and why-->

<!--```
Give an example
```-->

<!--## Deployment-->

<!--Add additional notes about how to deploy this on a live system-->

## Built With

* GNU C Compiler version 7.2.0

<!--## Contributing-->

<!--Please read [CONTRIBUTING.md](https://gist.github.com/PurpleBooth/b24679402957c63ec426) for details on our code of conduct, and the process for submitting pull requests to us.-->

<!--## Versioning-->

<!--We use [SemVer](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/your/project/tags).-->

## Authors

* **João M. Afonso**<!-- - *Initial work* - [PurpleBooth](https://github.com/PurpleBooth)-->

See also the list of [contributors](https://github.com/Hubble83/dbms/contributors) who participated in this project.

<!--## License-->

<!--This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details-->

## Acknowledgments

* Bruno M. G. Ribeiro
* João P. A. Fernandes

