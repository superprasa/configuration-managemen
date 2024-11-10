/*
 * Libconfig.h.cpp
 *
 *  Created on: 11-Apr-2024
 *      Author: developer
 */

/*
 * LibConfig.cpp
 *
 *  Created on: 08-Apr-2024
 *      Author: developer
 */
#include "Libconfig.h"
using namespace std;
#include<vector>
#include<cassert>
#include <cstdio>
#include <cstdarg>


vector<string> error_msg = {
       "LC_ERR_NONE",
       "LC_ERR_EMPTY",
       "LC_ERR_FILE_NO",
       "LC_ERR_MEMORY_NO",
       "LC_ERR_WRITE_NO",
       "LC_ERR_NOT_EXISTS"
   };


#if defined DEBUG

 void lc_Config_Err_war ::warning(std::ostream &out, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stdout, fmt, ap);
    va_end(ap);
}
#else
#define warning(...)
#endif


lc_config_variable::lc_config_variable(){}
lc_config_variable::lc_config_variable( const string & name, const string& value) : name(name), value(value) {}

lc_config_variable::lc_config_variable(const lc_config_variable &other) : name(other.name), value(other.value) {}

string lc_config_variable::getName() const{
	return name;
}
void lc_config_variable::setName(const std::string &Name)
{
	this->name=Name;
}
std::string lc_config_variable:: getValue() const
{
	return value;
}
void lc_config_variable::setValue(const std::string &value)
{
	this->value=value;
}

 int lc_Config::lc_init_config( std::string filepath , const std::string &Delim){
      if(Delim.empty())
      {
    	  warning(cerr, "[WARNING] %s: arguments is null\n", __func__);
    	  return LC_ERROR;
      }
      this->variable.clear();
      this->delim.clear();
      if(!filepath.empty())
     {
      this->filepath = filepath;
      if (this->filepath.empty()){

    	  this->error_type = LC_ERR_MEMORY_NO;
    	  return LC_ERROR;
      }
     }
      else{
    	  this->filepath.clear();
       }

      this->delim=Delim;
       if(this->delim.empty())
       {
           this->filepath.clear();
           this->error_type = LC_ERR_MEMORY_NO;
           return LC_ERROR;
       }


      this->error_type = LC_ERR_NONE;
      return LC_SUCCESS;



  }

 lc_config_variable * lc_Config::lc_create_varaible(const std::string &name,const std::string &value)
 {
	 if(name.empty() || value.empty())
	 {
         warning(cerr, "[WARNING] %s: arguments is null\n", __func__);
		 		return  nullptr;
	}
	 return new lc_config_variable(name, value);

 }
 void lc_Config::add_list_element(lc_config_variable &variable) {
     this->variable.push_back(variable);
 }



 int lc_Config:: addVariable(lc_config_variable *variable){
	 if(variable==nullptr){
		 this->error_type = LC_ERR_MEMORY_NO;
		 		return LC_ERROR;
	 }
	 lc_config_variable *varaibleCopy=new  lc_config_variable(*variable);
	 if(varaibleCopy==nullptr)
	 {
          warning(cerr, "[WARNING] %s: _create_variable_copy failed\n", __func__);
	 		this->error_type = LC_ERR_MEMORY_NO;
	 		return LC_ERROR;
	 }

	     add_list_element(*varaibleCopy);
	     this->error_type = LC_ERR_NONE;
	     return LC_SUCCESS;

 }
void lc_Config::printConfig() const {
    for (auto it = variable.begin(); it != variable.end(); ++it) {
        const lc_config_variable &var = *it;
        // Access each variable through the iterator and print its details
        if (!var.getName().empty() || !var.getValue().empty()) {
            std::cout << var.getName()<<this->delim << var.getValue() << std::endl;
        }
    }
}
int lc_Config ::_dump_config_to_file(std::ofstream &Fp) {
    if (Fp.fail()) {
        this->error_type = LC_ERR_MEMORY_NO;
        return LC_ERROR;
    }

    for (auto it = variable.begin(); it != variable.end(); ++it) {
        Fp << it->getName() << this->delim << it->getValue() << std::endl;
    }

    Fp.close();
    this->error_type = LC_ERR_NONE;
    return LC_SUCCESS;
}

int lc_Config::dumpConfig(const std::string &filepath) {

    std::ofstream outFile;
    if (filepath.empty()) {
        if (this->filepath.empty()) {
            this->error_type = LC_ERR_FILE_NO;
            return LC_ERROR;
        }

         outFile.open(this->filepath, std::ios::app);
        if (!outFile.is_open()) {
            this->error_type = LC_ERR_FILE_NO;
            return LC_ERROR;
        }
    }
        else
        {
        outFile.open(filepath, std::ios::app);

        if (!outFile.is_open()) {
            this->error_type = LC_ERR_FILE_NO;
            return LC_ERROR;

       }

        }

        _dump_config_to_file(outFile);
        return LC_SUCCESS;
}

int lc_Config::_read_file_to_config(std::ifstream& fp) {
    assert(fp.is_open());

    std::string line;
    while (std::getline(fp, line)) {
        lc_config_variable* variable = _convert_line_to_variable(line, this->getDelim());
        if (variable == nullptr) {
            continue;
        }

        add_list_element(*variable);

    }

    return 0;
}


std::string lc_Config::_convert_variable_to_line(const std::string& name, const std::string& value) {
    assert(!name.empty());
    assert(!value.empty());
   // assert(!delim.empty());

    return (name + delim + value + "\n");

}

string &lc_Config ::getDelim()
{
    return this->delim;
}
int lc_Config::setDelim(const string &delim)
{
    if(delim.empty())
    {
        return LC_ERROR;
    }
    this->delim=delim;
    return LC_SUCCESS;

}
int lc_Config::loadConfig(const std::string& Filepath) {
    std::ifstream infile;
    if (Filepath.empty()) {

        if(this->filepath.empty())
        {
            return LC_ERROR;
        }
        infile.open(this->filepath, std::ios::in);
    }
    else {
        infile.open(Filepath, std::ios::in);
    }

    if (infile.fail()) {
        infile.close();
        return -1;
    }

    if (_read_file_to_config(infile) == -1) {
        infile.close();
        return -1;
    }

    infile.close();
    return LC_SUCCESS;
}

lc_config_variable* lc_Config::_convert_line_to_variable(const std::string& line, const std::string& delim) {
    if (line.empty() || delim.empty()) {
        std::cerr << "[ERROR] Invalid line or delimiter." << std::endl;
        return nullptr;
    }

    size_t pos = line.find(delim);
    if (pos == std::string::npos) {
        std::cerr << "[WARNING] Cannot find \"" << delim << "\" delimiter in line \"" << line << "\"" << std::endl;
        return nullptr;
}

    std::string name = line.substr(0, pos);
    std::string value = line.substr(pos + delim.length());

    lc_config_variable* variable = new lc_config_variable(name, value);

    return variable;
}

void lc_Config::clearPath()
{
  this->filepath.clear();
}

size_t lc_Config::getSize()const
{
    return variable.size();
}

int lc_Config::_delete_list_element(const std::string& Name) {
    assert(!Name.empty());

    if (variable.empty()) {
        this->error_type = LC_ERR_EMPTY;
        return LC_ERROR;
    }
  for (auto it = variable.begin(); it != variable.end(); ++it) {
        lc_config_variable &var = *it;

        if (var.getName() == Name) {
            it = variable.erase(it); // Remove the element and update iterator
            this->error_type = LC_ERR_NONE;
            return LC_SUCCESS;
        }
 this->error_type = LC_ERR_NOT_EXISTS;
    return LC_ERROR;

  }
  return LC_SUCCESS;
  }

void lc_Config_Err_war::getError()
{
  cout<<error_msg[error_type]<<endl;

}

lc_config_variable* lc_Config::lc_get_variable(const std::string& Name) {
    if (Name.empty()) {
        std::cerr << "[WARNING] " << __func__ << ": argument is empty" << std::endl;
        return nullptr;
    }

    if (this->variable.empty()) {
        this->error_type = LC_ERR_EMPTY;
        return nullptr;
    }

    for (auto it = variable.begin(); it != variable.end(); ++it) {
        lc_config_variable &var = *it;

        if (var.getName() == Name) {
            return &var;
        }
    }
  // If variable with Name not found, return nullptr
    return nullptr;
}


int lc_Config::replaceVariable(const std::string& name, lc_config_variable& newVariable) {
    assert(!name.empty());

    if (variable.empty()) {
        this->error_type = LC_ERR_EMPTY;
        return LC_ERROR;
    }


    for (auto it = variable.begin(); it != variable.end(); ++it) {
        lc_config_variable& var = *it;


 if (var.getName() == name) {
            var = newVariable;
            this->error_type = LC_ERR_NONE;
            return LC_SUCCESS;
        }
    }

    // If the variable is not found, set error_type and return error
    this->error_type = LC_ERR_NOT_EXISTS;
    return LC_ERROR;
}


void lc_Config::_delete_list() {
    if (variable.empty())
    {
      cerr<<"VaraibleList is empty"<<endl;
        return;
    } else {
        variable.clear();
    }
}

void lc_Config ::clearConfig()
{
    _delete_list();
    clearPath();
}



lc_config_variable lc_Config:: _find_prev_list_element(const std::string& name) {
    assert(!name.empty());

    static lc_config_variable dummy_error; // Static variable to return in case of error

    if (this->variable.empty()) {
        return dummy_error; // Return the dummy error variable
    }

    for (auto it = variable.begin(); it != variable.end(); ++it) {
        lc_config_variable& var = *it;
          if (var.getName() == name) {
                return *(--it);


               this->error_type = LC_ERR_NONE;
            return  var;
        }
    }
    this->error_type = LC_ERR_NOT_EXISTS;
   return dummy_error;
}
