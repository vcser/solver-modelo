
#ifndef COMMANDLINE_HPP
#define COMMANDLINE_HPP

#include <iostream>
#include <string>
/*
-i input
-o output
-v verbose
-s solver

TODO:
- Crear un nombre por default?
*/

class Commandline{
    private:
        bool command_ok ; // Say if the commandline is valid
        std::string input_name ; // input name
        std::string input_path ; // input path
	    std::string output_name ; // Output name
        std::string output_path ; // Output path
        static int verbose;
        static int solver;
        static bool lecturaPorLineaComando;
        void _parsePathToName(std::string to_parse, std::string &tag);

    public:
        std::string get_inputName();
        std::string get_outputName();
        std::string get_inputPath();
        std::string get_outputPath();
        static int get_verbose() ;
        static bool get_lecturaPorLineaComando();
        static int get_solver();

        Commandline(int argc, char* argv[]);// constructor
        ~Commandline();// destructor

        bool is_valid();// say if the commandline is valid

};

#endif
