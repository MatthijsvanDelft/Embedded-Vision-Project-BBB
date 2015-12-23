// Main program Embedded Vision Project
// Created by Matthijs van Delft & Mike van Eerd
  
  #include "../Headers/Main.h"
  #include "../Headers/handler.h"
  
 int main() {
 	
	Handler handler;

	try{
 	   handler.start();
	}
	catch(std::exception& e){
    	std::cerr << "Type: " << typeid(e).name() << "\tException: " << e.what() << std::endl;
	}

 	return 0;
}
