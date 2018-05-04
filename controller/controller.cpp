#include <iostream>
#include <string>
#include <getopt.h>

#define no_argument 0
#define required_argument 1
#define optional_argument 2

#define SQL 100
#define LAQ 101
#define CPP 102
#define EXE 103
#define RES 104

void help() {
  std::cout << "TODO" << std::endl;
}

void version() {
  std::cout << "Alpha" << std::endl;
}

int processInputFile(const std::string& in, int& inputType, bool& inFile) {
  if (in.size() > 4) {
    std::string inExt = in.substr(in.find_last_of(".") + 1);

    if(inExt == "sql" || inExt == "SQL") {
      inputType = SQL;
      inFile = true;
    } else if(inExt == "laq" || inExt == "LAQ") {
      inputType = LAQ;
      inFile = true;
    } else if(inExt == "cpp" || inExt == "CPP" ||
              inExt == "cxx" || inExt == "CXX" ||
              inExt == "cc"  || inExt == "CC") {
      inputType = CPP;
      inFile = true;
    } else {
      std::cerr
        << "Error: unknown input file extension." << std::endl
        << "Use \".sql\", \".laq\", or \".cpp\" (\".cxx\", \".cc\")"
        << std::endl;
      return 1;
    }
  }
  // TODO(João) check file exists
  return 0;
}

int processOutputFile(const std::string& out, int& outputType, bool& outFile) {
  if (out.size() > 4) {
    std::string outExt = out.substr(out.find_last_of(".") + 1);

    if(outExt == "laq" || outExt == "LAQ") {
      outputType = LAQ;
      outFile = true;
    } else if(inExt == "cpp" || inExt == "CPP" ||
              inExt == "cxx" || inExt == "CXX" ||
              inExt == "cc"  || inExt == "CC") {
      outputType = CPP;
      outFile = true;
    } else if(outExt == "exe" || outExt == "EXE" ||
              outExt == "o" || outExt == "O") {
      outputType = EXE;
      outFile = true;
    } else if(outExt == "dat" || outExt == "DAT" ||
              outExt == "txt" || outExt == "TXT" ||
              outExt == "csv" || outExt == "CSV") {
      outputType = RES;
      outFile = true;
    } else {
      std::cerr
        << "Error: unknown output file extension." << std::endl
        << "Use \".laq\", \".exe\" (\".o\"), or \".dat\" (\".txt\", \".csv\")"
        << std::endl;
      return 1;
    }
  }
  // TODO(João) check file exists
  return 0;
}

int sqlToLaq(const std::string& inFile, std::string& laq) {
  std::cout << "SQL support comming soon...";
  return 1;
}

int writeFile(const std::string& fname,
              const std::string& content,
              bool toFile) {
  if (toFile) {

  } else {
    std::cout << content << std::endl;
  }
  return 0;
}

int query(const std::string& in, const std::string& out) {
  int inputType = SQL, outputType = RES;
  int inFile = false, outFile = false;

  int exitcode = processInputFile(in, inputType, inFile);
  exitcode += processOutputFile(out, inputType, inFile);

  if(exitcode || inputType >= outputType) {
    return;
  }

  std::string sql, laq, cpp;

  switch (inputType) {
    case SQL:
      if (sqlToLaq(in, laq)) {
        return 1;
      }
      if (outputType == LAQ) {
        writeFile(out, laq);
        return 0;
      }
      writeFile(out + ".tmp.laq", laq);
      // Don't break;
    case LAQ:
      if (laq == "") {
        if (laqToCpp(in, cpp)) {
          return 2;
        }
      } else if (laqToCpp(out + "tmp.laq", cpp)) {
        return 3;
      }
      if (outputType == CPP) {
        writeFile(out, cpp);
        return 0;
      }
      writeFile(out + ".tmp.cpp", cpp);
      // Don't break;
    case CPP:
      if (cpp == "") {
        if (cppToExe(in)) {
          return 4;
        }
      } else if (cppToExe(out + ".tmp.cpp")) {
        return 5;
      }
    default:
      return;
  }

  switch (outputType) {
    case LAQ:
      break; // DONE
    case CPP:
      break; // DONE
    case EXE:
      break; // DONE
    case RES:
      exeToRes(/***/);
    default:
      break;
  }
  return 0;
}

int main( int argc, char* argv[] ) {
  std::string in, out;

  const struct option longopts[] = {
    {"version",  no_argument,        0, 'v'},
    {"help",     no_argument,        0, 'h'},
    {"help",     no_argument,        0, '?'},
    {"input",    optional_argument,  0, 'i'},
    {"output",   optional_argument,  0, 'o'},
    {0,0,0,0},
  };

  int opt ;
  while((opt = getopt_long(argc, argv, "h?vi:o:")) != -1) {
    switch(opt) {
      case 'h':
      case '?':
        help();
        break;
      case 'v':
        version();
        break;
      case 'i':
        if(optarg) {
          in = optarg;
        }
        break;
      case 'o':
        if(optarg) {
          out = optarg;
        }
        break;
      default:
        break;
    }
  }
  query(in, out);
  return 0;
}
