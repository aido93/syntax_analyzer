
#include "root.hpp"
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <functional>

std::stringstream estdout;
std::stringstream estderr;

root::~root()
{
	
}

int root::exit(std::vector<arg> *in)
{
	::exit(EXIT_SUCCESS);
    return 0;
}

int root::help(std::vector<arg> *in)
{
	estdout<<"This is help"<<std::endl;
    for(auto it=methods.begin(); it!=methods.end();it++)
    {
        estdout<<it->first<<"\t - \t"<<it->second.desc<<std::endl;
    }
	return 0;
}

int root::pwd(std::vector<arg> *in)
{
	estdout<<CUR_DIR<<std::endl;
	return 0;
}

int root::ls(std::vector<arg>* in)
{
	std::string args;
	bool has_name=false;
	if(in)
	{
		for(auto it=in->begin(); it!=in->end(); it++)
		{
			switch(it->type_val)
			{
				case WORD_T:
				case FILENAME_T:
				{
					args+=it->val.str;
					//if(is_filename(it->val.num))
					//{
						has_name=true;
					//}
				}
				break;
				case NUM_T:
				{
					args+=it->val.num;
					//if(is_filename(it->val.num))
					//{
						has_name=true;
					//}
				}
				break;
				case VOID:
					estderr<<"Incorrect type (void) for the argument "<<std::distance(in->begin(), it)<<"\n";
					//estdout="";
					return -1;
				case OBJECT:
					ret_object(it->val.obj);
					/*int ret=ret_object(it->val.obj);
					if(ret)
					{
						//estdout="";
						return ret;
					}*/
					//estderr<<"Incorrect type (object) for the argument "<<arg->number<<"\n";
					//return -1;
				break;
				case FUNCTION:
					exec_function(it->val.f);
					/*int ret=exec_function(it->val.f);
					if(ret)
					{
						//estdout="";
						return ret;
					}*/
				break;
				default:
					estderr<<"Undefined type for the argument "<<std::distance(in->begin(), it)<<"\n";
					//estdout="";
					return -2;
				break;
			}
		}
		args+=" ";
	}
	if(!has_name)
		system((std::string("ls ")/*+estdout.c_str()*/+CUR_DIR).c_str());
	else
		system((std::string("ls ")+args).c_str());
	//estdout="";
	return 0;
}

void root::init_methods()
{
	func_proto proto;
	//---------exit--------------
    proto.desc="exit program";
	proto.ret_type=real_types::VOID;
    proto.p_func=[this](std::vector<arg>* args) -> int {return this->exit(args);};
	methods["exit"]=proto;
	method_aliases["q"]=proto;
	//---------help--------------
	proto.desc="Show this message";
	proto.ret_type=real_types::VOID;
    proto.p_func=[this](std::vector<arg>* args) -> int {return help(args);};
	methods["help"]=proto;
	method_aliases["h"]=proto;
	//---------pwd--------------
    CUR_DIR=get_current_dir_name();
    PREV_DIR=CUR_DIR;
	proto.desc="Print current directory";
	proto.ret_type=real_types::VOID;
    proto.p_func=[this](std::vector<arg>* args) -> int {return pwd(args);};
	methods["pwd"]=proto;
	//---------ls--------------
	proto.desc="ls [options] [file] - list directory contents\n\t\tFor more information use 'man ls'";
	proto.ret_type=real_types::VOID;
    proto.p_func=[this](std::vector<arg>* args) -> int {return ls(args);};
	methods["ls"]=proto;
	//---------cd--------------
	/*proto.name="cd";
	proto.arg_count=0;
	proto.prototype=nullptr;
	proto.desc="cd [options] [file] - change directory";
	root->register_function(proto, cd);
	//---------cat--------------
	proto.name="cat";
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

//инвалидация итератора. сохранять последний путь
//структура, представляющая из себя полное древо поиска
/*
class type_real
{
    private: 
        std::string type_name;
        std::shared_ptr<type_real> parent;
        std::map<func_proto, func_type> methods;
    public:
        std::vector<std::shared_ptr<type_real>> childrens;
        type_real(const std::string & name,
                  type_real* _parent,
                  const std::map<func_proto, func_type> & _methods,
                  const std::vector<std::shared_ptr<type_real>> & _childrens):
                  type_name(name),
                  parent(_parent),
                  methods(_methods),
                  childrens(_childrens)
                  {
                      
                  }
        type_real() = delete;
        ~type_real(){}
        std::shared_ptr<type_real> get_parent() const
        {
            return parent;
        }
        
        std::string get_name() const
        {
            return type_name;
        }
        
        func_type get_method(const char* name) const
        {
            for(auto it=methods.begin(); it!=methods.end(); ++it)
            {
                if(!strcmp(it->first.name.c_str(), name))
                    return it->second;
            }
            return nullptr;
        }
        
        func_type get_method(const char* name, const type* t, uint32_t arg_count) const
        {
            for(auto it=methods.begin(); it!=methods.end(); ++it)
            {
                if(!strcmp(it->first.name.c_str(), name))
                {
                    for(uint32_t i=0;i<arg_count;++i)
                    {
                        if(t[i]==it->first.prototype[i].type_val)
                            return it->second;
                    }
                }
            }
            return nullptr;
        }
        
        std::shared_ptr<type_real> get_object(const char* name) const
        {
            for(auto it=childrens.begin(); it!=childrens.end();++it)
            {
                if(!strcmp((*it)->get_name().c_str(),name))
                {
                    return *it;
                }
            }
            return nullptr;
        }
        
        func_type find_method_in_tree(const char* name, const type*t, uint32_t arg_count) const
        {//поиск в ширину до 3 уровня (root->broker->asset). Можно сделать и дальше, но смысла нет
            //поэтому костыль=)
            auto ptr=get_method(name, t, arg_count);
            if(ptr)
            {
                return ptr;
            }
            else
            {
                for(auto it=childrens.begin(); it!=childrens.end(); ++it)
                {
                    if(*it)
                    {
                        
                        ptr=(*it)->get_method(name, t, arg_count);
                        if(ptr)
                        {
                            return ptr;
                        }
                    }
                }
                //не вернули ничего
                for(auto it=childrens.begin(); it!=childrens.end(); ++it)
                {
                    if(*it)
                    {
                        for(auto it1=(*it)->childrens.begin(); it1!=(*it)->childrens.end(); ++it1)
                        {
                            if(*it1)
                            {
                                ptr=(*it1)->get_method(name, t, arg_count);
                                if(ptr)
                                {
                                    return ptr;
                                }
                            }
                        }
                    }
                }
            }
            return nullptr;
        }
        
        std::shared_ptr<type_real> find_object_in_tree(const char* name) const
        {//поиск в ширину до 3 уровня (root->broker->asset). Можно сделать и дальше, но смысла нет
            //поэтому костыль=)
            auto ptr=get_object(name);
            if(ptr)
            {
                return ptr;
            }
            else
            {
                for(auto it=childrens.begin(); it!=childrens.end(); ++it)
                {
                    if(*it)
                    {
                        ptr=(*it)->get_object(name);
                        if(ptr)
                        {
                            return ptr;
                        }
                    }
                }
            }
            return nullptr;
        }

        void register_function(const func_proto & proto, func_type body)
        {
            methods.emplace(proto, body);
        }
        
        void add_child(const std::shared_ptr<type_real> & new_child)
        {
            childrens.push_back(new_child);
        }

        void delete_child(const char* name)
        {
            for(auto it=childrens.begin(); it<childrens.end();++it)
            {
                if(!strcmp((*it)->get_name().c_str(),name))
                {
                    childrens.erase(it);
                }
            }
        }
};
*/

void root::ret_object(std::vector<object>* obj)
{
    if(obj)
    {
		//It is safe, 'cause root must be one and creates when application begin to work
		//and dies when application finish to work
		object_proto* current_obj=this;
		estdout.str(std::string());
        for(auto it=obj->begin(); it!=obj->end(); it++)
        {
            if(!(it->is_object))//function
            {
                exec_function(it->obj.f);
            }
            else
            {
				current_obj=current_obj->find_children(it->obj.obj_name).get();
				estdout.str(std::string());
				if(!current_obj)
				{
					std::cout<<"Object is not found!";
					return;
				}
            }
        }
    }
}

void root::exec_function(func* f)
{
    auto it=methods.find(f->name);
    estdout.str(std::string());
    if(it!=methods.end())
    {
        it->second.p_func(f->args.get());
        std::cout<<estdout.str();
    }
    else
    {
		auto it_alias=method_aliases.find(f->name);
		estdout.str(std::string());
		if(it_alias!=method_aliases.end())
		{
			it_alias->second.p_func(f->args.get());
			std::cout<<estdout.str();
		}
		else
		{
			std::cout<<"Function is not found!";
        }
        return;
    }
}

void root::exec_conveyor(std::vector<conveyor>* c)
{
	if(c)
    {
        //size_t i=0;
        for(auto it=c->begin(); it!=c->end(); it++)
        {
            ret_object(&it->command);
            /*if(i<c->size()-1)
            {
                switch (it->conv)
                {
                    case AND_CONV:
                        std::cout<<" && ";
                    break;
                    case OR_CONV:
                        std::cout<<" || ";
                    break;
                    case SEMICOLON_CONV:
                        std::cout<<"; ";
                    break;
                    default:
                    break;
                }
            }*/
            //i++;
        }
    }
}

