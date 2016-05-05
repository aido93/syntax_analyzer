
#include "root.hpp"
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <functional>
#include <boost/filesystem.hpp>
#include "errcode.hpp"

std::stringstream estdout;
std::stringstream estderr;

root::~root()
{
	
}

int root::exit(std::vector<rettype> *in, rettype* retval)
{
	::exit(EXIT_SUCCESS);
    return 0;
}

int root::help(std::vector<rettype> *in, rettype* retval)
{
	estdout<<"This is help"<<std::endl;
    for(auto it=methods.begin(); it!=methods.end();it++)
    {
        estdout<<it->first<<"\t - \t"<<it->second.desc<<std::endl;
    }
    retval=nullptr;
	return 0;
}

int root::pwd(std::vector<rettype> *in, rettype* retval)
{
    if(!in)
    {
        estdout<<CUR_DIR<<std::endl;
        retval->type_val=WORD;
        retval->val.str=CUR_DIR;
        return 0;
    }
    else
    {
        estderr<<"Function takes no arguments!"<<std::endl;
        return FUNCTION_FALSE_ARGUMENTS;
    }	
}

int root::ls(std::vector<rettype>* in, rettype* retval)
{
	std::string args;
	bool has_name=false;
    int ret;
	if(in)
	{
		for(auto it=in->begin(); it!=in->end(); it++)
		{
			switch(it->type_val)
			{
				case WORD_T:
				{
					args+=it->val.str;
                    has_name=true;
				}
				break;
				case NUM_T:
				{
					args+=it->val.num;
					has_name=true;
				}
				break;
				case VOID:
                {
					estderr<<"Incorrect type (void) for the argument "<<std::distance(in->begin(), it)<<"\n";
					return FUNCTION_FALSE_ARGUMENTS;
                }
				case OBJECT:
                {
					ret=ret_object(it->val.obj);
					if(ret)
					{
						return ret;
					}
					//estderr<<"Incorrect type (object) for the argument "<<arg->number<<"\n";
					//return -1;
                }
				break;
				case FUNCTION:
                {
					ret=exec_function(it->val.f);
					if(ret)
					{
						return ret;
					}
                }
				break;
				default:
					estderr<<"Undefined type for the argument "<<std::distance(in->begin(), it)<<"\n";
					return UNDEFINED_TYPE;
				break;
			}
		}
		args+=" ";
	}
	if(!has_name)
		system((std::string("ls ")/*+estdout.c_str()*/+CUR_DIR).c_str());
	else
		system((std::string("ls ")+args).c_str());
	return 0;
}

int root::cd(std::vector<rettype>* in, rettype* retval)
{
    int ret;
	if(in)
	{
		for(auto it=in->begin(); it!=in->end(); it++)
		{
			switch(it->type_val)
			{
				case WORD_T:
				{
                    boost::filesystem::path p(it->val.str);
                    if (boost::filesystem::exists(p))    // does path p actually exist?
                    {
                        if (boost::filesystem::is_regular_file(p))
                        {
                            //error
                        }
                        else if (boost::filesystem::is_directory(p))      // is path p a directory?
                        {
                            PREV_DIR=CUR_DIR;
                            CUR_DIR=boost::filesystem::absolute(p).c_str();
                        }
                        else
                        {
                            std::cout << p << " exists, but is not a regular file or directory\n";
                        }
                    }
                    else
                    {
                        std::cout << p << " does not exist\n";
                    }
				}
				break;
				case NUM_T:
				{
                    boost::filesystem::path p(std::to_string(it->val.num));
                    if (boost::filesystem::exists(p))    // does path p actually exist?
                    {
                        if (boost::filesystem::is_regular_file(p))
                        {
                            //error
                        }
                        else if (boost::filesystem::is_directory(p))      // is path p a directory?
                        {
                            PREV_DIR=CUR_DIR;
                            CUR_DIR=boost::filesystem::absolute(p).c_str();
                        }
                        else
                        {
                            std::cout << p << " exists, but is not a regular file or directory\n";
                        }
                    }
                    else
                    {
                        std::cout << p << " does not exist\n";
                    }
				}
				break;
				case VOID:
					estderr<<"Incorrect type (void) for the argument "<<std::distance(in->begin(), it)<<"\n";
					return FUNCTION_FALSE_ARGUMENTS;
				case OBJECT:
					ret_object(it->val.obj);
					ret=ret_object(it->val.obj);
					if(ret)
					{
						return ret;
					}
					//estderr<<"Incorrect type (object) for the argument "<<arg->number<<"\n";
					//return -1;
				break;
				case FUNCTION:
					ret=exec_function(it->val.f);
					if(ret)
					{
						return ret;
					}
				break;
				default:
					estderr<<"Undefined type for the argument "<<std::distance(in->begin(), it)<<"\n";
					return UNDEFINED_TYPE;
				break;
			}
		}
	}
	return ret;
}

void root::init_methods()
{
	func_proto proto;
    std::vector<arg_proto> args;
	//---------exit--------------
    proto.desc="exit program";
	proto.ret_type=real_types::VOID;
    proto.p_func=[this](std::vector<arg>* args, rettype* retval) -> int {return this->exit(args, retval);};
    args.clear();
    proto.args.clear();
    proto.args.push_back(args);
	methods["exit"]=proto;
	method_aliases["q"]=proto;
	//---------help--------------
	proto.desc="Show this message";
	proto.ret_type=real_types::VOID;
    proto.p_func=[this](std::vector<arg>* args, rettype* retval) -> int {return help(args, retval);};
    args.clear();
    proto.args.clear();
    proto.args.push_back(args);//no args
    args.push_back({WORD, "name"});
    proto.args.push_back(args);//1 arg
	methods["help"]=proto;
	method_aliases["h"]=proto;
	//---------pwd--------------
    CUR_DIR=get_current_dir_name();
    PREV_DIR=CUR_DIR;
	proto.desc="Print current directory";
	proto.ret_type=real_types::WORD;
    proto.p_func=[this](std::vector<arg>* args, rettype* retval) -> int {return pwd(args, retval);};
    args.clear();
    proto.args.clear();
    proto.args.push_back(args);
	methods["pwd"]=proto;
	//---------ls--------------
	proto.desc="ls [options] [file] - list directory contents\n\t\tFor more information use 'man ls'";
	proto.ret_type=real_types::VOID;
    proto.p_func=[this](std::vector<arg>* args, rettype* retval) -> int {return ls(args, retval);};
	args.clear();
    proto.args.clear();
    proto.args.push_back(args);
    args.push_back({WORD, "opts_or_dir1"});
    proto.args.push_back(args);
    args.push_back({WORD, "opts_or_dir2"});
    proto.args.push_back(args);
    methods["ls"]=proto;
	//---------cd--------------
    proto.desc="cd [options] [file] - change directory";
	proto.ret_type=real_types::VOID;
    proto.p_func=[this](std::vector<arg>* args, rettype* retval) -> int {return cd(args, retval);};
	args.clear();
    proto.args.clear();
    proto.args.push_back(args);
    args.push_back({WORD, "opts_or_dir1"});
    proto.args.push_back(args);
    args.push_back({WORD, "opts_or_dir2"});
    proto.args.push_back(args);
    methods["cd"]=proto;
	//---------cat--------------
	/*proto.name="cat";
	proto.arg_count=0;
	proto.prototype=nullptr;
	proto.desc="cat [options] [file] - concatenate files and print on the standard output";
	root->register_function(proto, cat);
	
	//-------User-------------
	//---------s--------------
	proto.name="s";
	proto.arg_count=0;
	proto.prototype=nullptr;
	proto.desc="s - show user status";
	root->register_function(proto, get_status);
	//---------hist--------------
	proto.name="hist";
	proto.arg_count=0;
	proto.prototype=nullptr;
	proto.desc="hist - show history of operations";
	root->register_function(proto, get_history);
	//---------histat--------------
	proto.name="histat";
	proto.arg_count=0;
	proto.prototype=nullptr;
	proto.desc="s - show history statistics";
	root->register_function(proto, get_history_statistics);
	
	//-------Broker-------------
	//---------bl--------------
	proto.name="bl";
	proto.arg_count=0;
	proto.prototype=nullptr;
	proto.desc="bl - show list of the brokers";
	root->register_function(proto, get_broker_info);
	//---------bi--------------
	proto.name="bi";
	proto.arg_count=0;
	proto.prototype=nullptr;
	proto.desc="bi [broker name] - show broker info";
	root->register_function(proto, get_broker_info);
	//---------ab--------------
	proto.name="ab";
	proto.arg_count=11;
	proto.prototype=nullptr;
	proto.desc="ab (parameters) - add broker";
	root->register_function("", proto, add_broker);
	//---------db--------------
	proto.name="db";
	proto.arg_count=1;
	proto.prototype=nullptr;
	proto.desc="db (broker name) - delete broker";
	root->register_function(proto, delete_broker);
	//---------eb--------------
	proto.name="eb";
	proto.arg_count=1;
	proto.prototype=nullptr;
	proto.desc="eb (broker name, option name, value) - edit broker";
	root->register_function(proto, edit_broker);
	//---------ua--------------
	proto.name="ua";
	proto.arg_count=0;
	proto.prototype=nullptr;
	proto.desc="ua - update all brokers";
	root->register_function(proto, update_all);
	//---------rlc--------------
	proto.name="rlc";
	proto.arg_count=0;
	proto.prototype=nullptr;
	proto.desc="rlc [config file name] - reload config";
	root->register_function(proto, reload_config);*/
}

void root::init_childrens()
{
	
}

bool root::check_hierarchy(const real_types & first, const real_types & second)
{
	if((first==real_types::ROOT   && second==real_types::BROKER ) ||
       (first==real_types::ROOT   && second==real_types::WORD   ) ||
       (first==real_types::ROOT   && second==real_types::NUM    ) ||
       (first==real_types::ROOT   && second==real_types::BOOL   ) ||
       (first==real_types::ROOT   && second==real_types::VOID   ) ||
       
       (first==real_types::BROKER && second==real_types::USER   ) ||
       (first==real_types::BROKER && second==real_types::VOID   ) ||
       (first==real_types::BROKER && second==real_types::WORD   ) ||
       (first==real_types::BROKER && second==real_types::NUM    ) ||
       
       (first==real_types::USER   && second==real_types::ASSET  ) ||
       (first==real_types::USER   && second==real_types::WORD   ) ||
       (first==real_types::USER   && second==real_types::NUM    ) ||
       (first==real_types::USER   && second==real_types::BOOL   ) ||
       (first==real_types::USER   && second==real_types::VOID   ) ||
       
       (first==real_types::ASSET  && second==real_types::SMATRIX) ||
       (first==real_types::ASSET  && second==real_types::MATRIX ) ||
       (first==real_types::ASSET  && second==real_types::NUM    ) ||
       (first==real_types::ASSET  && second==real_types::WORD   ) ||
       (first==real_types::ASSET  && second==real_types::VOID   ) ||
       (first==real_types::ASSET  && second==real_types::BOOL   )  )
		return true;
      else 
		return false;
}

int root::process_object(std::vector<object>* obj)
{
    if(obj)
    {
        int ret;
        std::vector<real_func*> *func_tree;
        
        //первым делом при выполнении команды проверяем тип объекта и исполняемых функций
        //эта функция проверяет само наследование объектов, возвращаемые типы функций,
        //а также соответствие аргументов каждой функции нужному типу
        //в retval лежит дерево вызовов функций, то есть указатели на конкретные функции 
        //вместе с указателями на их аргументы. Если аргумент уже готов, то заполнение идет сразу.
        //Если нет, то выделяется память создающей функцией и скидывает туда указатель на выделенную память
        //Заполнение аргументов будет идти в обратном порядке по вектору
        ret=check_obj_childrens(obj, func_tree);
        if(ret)
            return ret;
        
        rettype tmp;
        uint32_t func_counter;
        //только если проверка успешна - начинаем выполнять
        for(auto it=func_tree.rbegin(); it!=func_tree.rend(); it++)
        {
            func_counter++;
            tmp={0, real_types::VOID};
            //создаем список аргументов
            std::vector<rettype> a;
            for(auto it1=it->args.begin();it1!=it->args.end(); it1++)
            {
                a.push_back(it1->pointer);
            }
            //вызываем функцию
            ret=(*it)->f(&a, &tmp);
            if(ret)
            {
                delete func_tree;
                return ret;
            }
                
            //что-то получили на выходе
            //проставляем зависимости других функций
            for(auto it1=func_tree.rbegin(); it1!=it; it1--)
            {
                for(auto it2=it1->args.begin(); it2!=it1->args.end(); it2++)
                {
                    if(it2->num_of_func==func_counter)
                    {
                        it2->pointer=tmp;
                    }
                }
            }
        }
        delete func_tree;
    }
    return 0;
}

int root::exec_function(real_func* f, rettype* retval)
{
    auto it=methods.find(f->name);
    int ret;
    if(it!=methods.end())
    {
        if(it->second.p_func)
        {
            ret=it->second.p_func(f->args.get(),retval);
            return ret;
        }
        else
        {
            return FUNCTION_NOT_DEFINED;
        }
    }
    else
    {
		auto it_alias=method_aliases.find(f->name);
		if(it_alias!=method_aliases.end())
		{
            if(it_alias->second.p_func)
            {
                ret=it_alias->second.p_func(f->args.get(), retval);
                return ret;
            }
            else
            {
                return FUNCTION_NOT_DEFINED;
            }
		}
		else
		{
            return FUNCTION_NOT_FOUND;
        }
    }
}


/**
 * Мораль сей басни такова: сначала собираем токены в списки-деревья (сделано),
 * затем еще раз проходимся сверху по всему дереву команды, сравнивая типы.
 * Если такого объекта или функции не найдено и функция принимает на вход WORD, 
 * то берем само слово
 * */
int root::check_func_arg_types(func* fp, real_types* ret, real_func* f)
{
    auto it=methods.find(fp->name);
    gen_function f=nullptr;
    std::vector<std::vector<arg_proto>> arg_prototypes;
    bool is_found=false;
    if(it!=methods.end())
    {
        f=it->second.p_func;
        arg_prototypes=it->second.args;
        *ret=it->second.ret_type;
        is_found=true;
    }
    else
    {
		auto it_alias=method_aliases.find(fp->name);
		if(it_alias!=method_aliases.end())
		{
            f=it_alias->second.p_func;
            arg_prototypes=it->second.args;
            *ret=it->second.ret_type;
            is_found=true;
		}
    }
    
    if(is_found)
    {
        if(f)
        {
            uint32_t num=0;
            for(auto it=fp->args->begin(); it!=fp->args->end(); it++)
            {//перебираем аргументы от пользователя
                for(auto it_proto=arg_prototypes.begin(); 
                         it_proto!=arg_prototypes.end(); it_proto++)
                {//ищем его в дереве возможных прототипов
                    if(it->type_val==NUM_T && it_proto[num]->type_val==NUM)
                    {
                        break;
                    }
                    else if(it->type_val==FUNCTION)
                    {
                        rettype s;
                        int was_error=check_func_arg_types(it->val.f, &s);
                        if(!was_error)
						{
							if(it_proto->size() >= num && s==it_proto[num]->type_val)
							{//функция принимает на вход аргумент нужного типа
								//возврат одной функции равен приему другой функции
								break;
							}
							else
							{
								estderr<<"False parameter type "<<num<<"\n";
							}
						}
						else
						{
							error
						}
                    }
                    else if(it->type_val==OBJECT)
                    {//точки...
                        
                    }
                    else if(it->type_val==VOID)
                    {
                        error
                    }
                    else if(it->type_val==WORD_T && it_proto[num]->type_val==WORD)
                    {//приоритет не тот
                        break;
                    }
                }
                num++;
            }
        }
        else
        {
            return FUNCTION_NOT_DEFINED;
        }
    }
    else
    {
        return FUNCTION_NOT_FOUND;
    }
}

int root::check_obj_childrens(std::vector<object>* obj, std::vector<real_func*>* pointers_tree, real_types* rt)
{
	if(obj)
    {
		//It is safe, 'cause root must be one and creates when application begin to work
		//and dies when application finish to work
		object_proto* current_obj=this;
		real_types r=real_types::ROOT, old_r;
        std::vector<real_func*>* pt=new std::vector<real_func>(obj->size());
        real_func* f;
        for(auto it=obj->begin(); it!=obj->end(); it++)
        {
            if(r!=real_types::VOID)
            {
                f=nullptr;
                old_r=r;//иерархия root-брокер-юзер-акция
                if(!(it->is_object))//function(args) apriori
                {
                    //проверяем аргументы функции, заносим в r тип возврата
                    ret=check_func_arg_types(it->obj.f, &r, f);
                    if(ret)
                    {//if there was any error - go out (or try to find all errors?)
                        return ret;
                    }
                    else
                    {
                        //enabled hierarchy
                        if(check_hierarchy(old_r, r))
							pt->push_back(f);
                        else
                        {
                            estderr<<"Forbidden hierarchy type!"<<std::endl;
                            return FUNCTION_FALSE_ARGUMENTS;
                        }
                    }
                }
                else
                {//may be object or function
                    current_obj=current_obj->find_children(it->obj.obj_name).get();
                    if(!current_obj)
                    {//one-word function
                        ret=check_func_arg_types({it->obj.obj_name, nullptr}, &r, f);
                        if(ret)
                        {//if there was any error - go out (or try to find all errors?)
                            return OBJECT_NOT_FOUND;
                            //return ret;
                        }
                        else
                        {
                            //enabled hierarchy
                        if(check_hierarchy(old_r, r))
                            pt->push_back(f);
                        }
                    }
                    else
                    {//argument of the function
                        r=current_obj->get_type();
                        //enabled hierarchy
                        if(!check_hierarchy(old_r, r))
                          {
                              estderr<<"Forbidden hierarchy type!"<<std::endl;
                              return FUNCTION_FALSE_ARGUMENTS;
                          }
                    }
                }
            }
            else
            {
                estderr<<"Some function returns void, but void cannot be the argument"<<std::endl;
                return FUNCTION_FALSE_ARGUMENTS;
            }
        }
        pointer_tree=pt;
        *rt=r;//?
        //мы вышли из цикла, а значит, что все слова нашлись где надо
    }
    return 0;
}

#include "colour.hpp"

static void show_error(int err_code)
{
    static color::modifier red(color::FG_RED);
    static color::modifier def(color::FG_DEFAULT);
    
    switch(err_code)
    {
        case FUNCTION_NOT_FOUND:
            std::cerr<<red<<"[Function not found]: "<<def<<estderr.str()<<std::endl;
        break;
        case FUNCTION_NOT_DEFINED:
            std::cerr<<red<<"[Function not defined]: "<<def<<estderr.str()<<std::endl;
        break;
        case FUNCTION_FALSE_ARGUMENTS:
            std::cerr<<red<<"[Function gives false arguments]: "<<def<<estderr.str()<<std::endl;
        break;
        case OBJECT_NOT_FOUND:
            std::cerr<<red<<"[Object not found]: "<<def<<estderr.str()<<std::endl;
        break;
        case DOWNLOAD_FAILED:
            std::cerr<<red<<"[Download failed]: "<<def<<estderr.str()<<std::endl;
        break;
        case CALCULATION_FAILED:
            std::cerr<<red<<"[Calculation failed]: "<<def<<estderr.str()<<std::endl;
        break;
    }
}

void root::exec_conveyor(std::vector<conveyor>* c)
{
	if(c)
    {
        int ret;
        for(auto it=c->begin(); it!=c->end(); it++)
        {
            ret=process_object(&it->command);
            //Errors first
            show_error(ret);
            estderr.str(std::string());
            //Standard messages next
            std::cout<<estdout.str();
            estdout.str(std::string());
            if(ret)
            {
                if(it->conv==AND_CONV)
                    break;//An error was here and AND cannot continue
                else if(it->conv==OR_CONV)
                {
                    //All is good
                }
                else if(it->conv==SEMICOLON_CONV || it->conv==NO_CONV)
                {
                    break;
                }
            }
        }
    }
}

