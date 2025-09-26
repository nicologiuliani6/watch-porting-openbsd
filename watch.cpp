#include <iostream>
#include <string>
#include <thread>
#include <array>
#include <cstdio>
#include <memory>

//porting of watch command on OpenBSD
int main(int argc, const char* argv[]){
  std::string command;
  std::array<char, 128> buffer; //buffer for result of the command
  std::string result;
  
  //gestione errore dei parametri in input per "watch -n[time]"
  if (argc < 3){
    goto EXIT_SYNTAX;
  }

  //cerchiamo di capire se abbiamo:
  //1. watch -n[time] [command]
  //2. watch -n [time] [command]
 unsigned int seconds;
  if(strlen(argv[1]) > 2){
    //caso 1
    seconds = std::stoi(argv[1]+2); //convert in int the value of the second
  }
  else{
    //gestione errore dei parametri in input per "watch -n [time]"
    if(argc < 4) goto EXIT_SYNTAX;
    //caso 2
    seconds = std::stoi(argv[2]);
  }
  //nel caso 1 argv[2] avra il primo pezzo di comando
  //nel caso 2 partiamo da uno slot dopo, ovvero argv[3]
  for(int i = ((strlen(argv[1])>2) ? 2 : 3); argv[i]; i++){
    command += argv[i];
    command += " ";
  }
  
  while(true){
      // "pulisce" lo schermo (ANSI)
        std::cout << "\033[2J\033[H";  

        std::cout << "Every " << seconds << ": "  << command << "\n\n";

        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);
        if (!pipe) {
            std::cerr << "Errore nell'aprire il comando!" << std::endl;
            return 1;
        }

        // leggo e stampo direttamente l'output, senza accumularlo
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            std::cout << buffer.data();
        }

        std::cout << std::flush;

        std::this_thread::sleep_for(std::chrono::seconds(seconds));
  }
  return 0;
  EXIT_SYNTAX:
    std::cerr << "SYNTAX ERROR: " << std::endl;
    std::cerr << "watch -n [time] [command]" << std::endl;
    return 1;
}
