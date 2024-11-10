#include<iostream>
#include<vector>
#include<list>
#include<vector>
#include<fstream>
using namespace std;

#ifndef LIBCONFIG_H
#define LIBCONFIG_H

#define LINE_SIZE 256
#define LC_SUCCESS 0
#define LC_ERROR -1

#define DEBUG

enum Configerror
{
	LC_ERR_NONE = 0,
	LC_ERR_EMPTY = 1,
	LC_ERR_FILE_NO = 2,
	LC_ERR_MEMORY_NO = 3,
	LC_ERR_WRITE_NO = 4,
	LC_ERR_NOT_EXISTS = 5
};

 enum lc_existence
{
	LC_EF_ERROR = 0,
	LC_EF_EXISTS = 1,
	LC_EF_NOT_EXISTS = 2
};

class ConfigVariable
{
public:

    virtual  std::string getName() const = 0;
    virtual void setName(const std::string &Name) = 0;
    virtual  std::string getValue() const = 0;
    virtual void setValue(const std::string &value) = 0;
    virtual ~ConfigVariable(){};
};
class lc_config_variable : public ConfigVariable
{
private :
     std::string name;
     std::string value;
public:
lc_config_variable();
     lc_config_variable(const lc_config_variable &other);
     lc_config_variable(const std::string& name, const std::string& value);
     inline  std::string getName() const override;
     void setName(const std::string &Name) override;
     inline std::string getValue() const;
     void setValue(const std::string &value);
    static lc_config_variable  * lc_create_varaible(const std::string &name,const std::string &value);
    ~lc_config_variable(){};
};

class lc_Config_Err_war
{
protected:
 Configerror  error_type;
public:
void getError();
static void warning(std::ostream &out, const char *fmt, ...);
};



class lc_Config :public lc_Config_Err_war
{
protected:
	std::list<lc_config_variable> variable;
    std::string filepath;
    std::string delim;
public:

 int lc_init_config( std::string filepath , const std::string &Delim);
 static lc_config_variable * lc_create_varaible(const std::string &name,const std::string &value);
 int addVariable(lc_config_variable *variable);
 void printConfig() const;
int dumpConfig(const std::string &filepath);
int _read_file_to_config(std::ifstream& fp);
std::string _convert_variable_to_line(const std::string& name, const std::string& value);
std::string &getDelim();
int setDelim(const string &delim);
int loadConfig(const std::string& Filepath);
lc_config_variable* _convert_line_to_variable(const std::string& line, const std::string& delim);
void clearPath();
size_t getSize()const;
int _delete_list_element(const std::string& Name) ;
lc_config_variable* lc_get_variable(const std::string& Name) ;
int replaceVariable(const std::string& name, lc_config_variable& newVariable);
lc_config_variable _find_prev_list_element(const std::string& name);
void _delete_list() ;
 void add_list_element(lc_config_variable &variable);
int _dump_config_to_file(std::ofstream &Fp);
void clearConfig();

};
#endif
