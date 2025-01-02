
#include "CommandLine.hpp"
#include "Managers.hpp"
#include "Parametros.hpp"

int Commandline::verbose;
int Commandline::solver;
bool Commandline::lecturaPorLineaComando;

void Commandline::_parsePathToName(std::string to_parse,std::string &tag){
	char caracther1 = '/' ;
	char caracther2 = '\\' ;

	int position = (int)to_parse.find_last_of(caracther1) ;
	int position2 = (int)to_parse.find_last_of(caracther2) ;
	if (position2 > position) position = position2 ;

	if (position != -1){
		tag =  to_parse.substr(position+1,to_parse.length() - 1)  ;
	}
	else{
		tag = to_parse ;
	}
}

Commandline::Commandline(int argc, char* argv[]){
	if (argc < 3){ //argc%2 != 0 || argc > 12
		std::cout << "Línea de comandos incorrecta:  Número de argumentos introducidos " << argc <<std::endl ;
		command_ok = false;
		return ;
	}

	//Defaults
	verbose = 0;
	output_path = "";
	solver = 0;
	lecturaPorLineaComando=false;

	// Reading the commandline parameters
	// Comente esto, porque ahora los Parametros se pueden modificar por argumento al llamar el solver
	// if(argc < 11){
		// Se cargado por medio de lectura de archivo
		std::cout << "Carga desde archivo... " <<std::endl ;
		for ( int i = 1 ; i < argc ; i += 2 ){
			if ( std::string(argv[i]) == "-i" ){
				input_path = std::string(argv[i+1]);
				_parsePathToName(input_path,this->input_name);
				std:: cerr <<input_path<<std::endl;
			}else if ( std::string(argv[i]) == "-o" ){
				output_path = std::string(argv[i+1]);
				_parsePathToName(input_path,this->output_name);
			}else if ( std::string(argv[i]) == "-v" ){
				verbose = atoi(argv[i+1]);
			}else if ( std::string(argv[i]) == "-s" ){
				solver = atoi(argv[i+1]);
				Parametros::get().setGreedyId(solver);
			}else if (!Parametros::get().loadParameterFromKey(argv[i],argv[i+1]) ){
				std::cout << "Comando no reconocido : " << std::string(argv[i]) << " " << argv[i+1] <<std::endl ;
				command_ok = false ;
				return;
			}
		}

		// Si no recibe archivo de entrada, supone que se lee por consola los datos.
		// Seteamos input_name a un placeHolder para que pueda generar un archivo out. (solo para que no sea "out_")
		if (input_path == "") {
			lecturaPorLineaComando = true;
			input_name = Parametros::get().getDefaultFileName();
		}
		if (output_path  == ""){
			output_name = "out_" + input_name;
			output_path = "../" + output_name;
			// No le veo sentido a este false de aca... Si no se selecciona un archivo de salida
			// se asume que no se lee por comando????
			// lecturaPorLineaComando = false;
		}
	// }else{
	// 	// Esta parte la comente porque si hay muchos argumentos, entonces deberia tirar error
	// 	// // Los datos se cargan por lectura de linea de comando
	// 	// lecturaPorLineaComando = true;
	// 	command_ok = false;
	// 	return;
	// }
	command_ok = true;
}

Commandline::~Commandline(){}

std::string Commandline::get_outputName(){
	return output_name;
}

std::string Commandline::get_inputName(){
	return input_name;
}

std::string Commandline::get_inputPath(){
	return input_path;
}

std::string Commandline::get_outputPath(){
	return output_path;
}

bool Commandline::is_valid(){
	return command_ok;
}

int Commandline::get_verbose(){
	return verbose;
}

bool Commandline::get_lecturaPorLineaComando(){
	return lecturaPorLineaComando;
};

int Commandline::get_solver(){
	return solver;
};
