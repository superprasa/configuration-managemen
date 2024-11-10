/*
 * Libconfig_main.cpp
 *
 *  Created on: 11-Apr-2024
 *      Author: developer
 */
#include "Libconfig.h"

int main(int argc, char** argv)
{

     if(argc != 3)
     {
		return EXIT_FAILURE ;
     }

   ConfigVariable* variable;
   string emptystr;
    lc_Config Configobj1;
    Configobj1.lc_init_config(argv[1], "="); // Use argv[1] for the file path
    variable = Configobj1.lc_create_varaible("name1", "value1");
    if (variable != nullptr) {
        Configobj1.addVariable(static_cast<lc_config_variable*>(variable));
        Configobj1.addVariable(static_cast<lc_config_variable*>(variable));
         Configobj1.dumpConfig(emptystr);
        Configobj1.printConfig();
        Configobj1.clearConfig();
        delete variable;
    }

    else {
        std::cerr << "Failed to create variable" << std::endl;
         }
    lc_Config Configobj2;
    Configobj2.lc_init_config(emptystr,"=");
     variable = Configobj2.lc_create_varaible("variable", "testValue");
    if (variable != nullptr) {
        Configobj2.addVariable(static_cast<lc_config_variable*>(variable));
    }
     else {
        std::cerr << "Failed to create variable" << std::endl;
     }
    Configobj1.dumpConfig(emptystr);
    Configobj2.loadConfig(argv[2]);
    Configobj2.printConfig();
    Configobj2.clearConfig();




/*
    cout<<"No of elements in Configobj1 list:" <<Configobj1.getSize()<<endl;
    cout<<"No of elements in Configobj2 list:"<<Configobj2.getSize()<<endl;
    cout<<"Previours error in configuration is:";
    Configobj2.getError();
    //Configobj1.getError();

    variable = Configobj2.lc_create_varaible("variablenew", "testValuenew");
    Configobj2.replaceVariable("variable",*static_cast<lc_config_variable*>(variable));
    Configobj2.printConfig();

*/


 return 0;

}





