
#include "root.hpp"
#include <iostream>
#include "errcode.hpp"
#include <sstream>
#include <stdlib.h>
#include <unistd.h>
std::stringstream estdout;
std::stringstream estderr;

root::~root()
{
	
}

void root::init_methods()
{
	func_proto proto;
    std::vector<arg_proto> args;
	//---------exit--------------
    proto.desc="exit program";
	proto.ret_type=real_types::VOID;
    proto.p_func=[this](std::vector<value_type>* args, value_type* retval) -> int {return this->exit(args, retval);};
    args.clear();
    proto.args.clear();
    proto.args.push_back(args);
	methods["exit"]=proto;
	method_aliases["q"]=&methods["exit"];
	//---------help--------------
	proto.desc="Show this message";
	proto.ret_type=real_types::VOID;
    proto.p_func=[this](std::vector<value_type>* args, value_type* retval) -> int {return help(args, retval);};
    args.clear();
    proto.args.clear();
    proto.args.push_back(args);//no args
    args.push_back({real_types::WORD, "name"});
    proto.args.push_back(args);//1 arg
	methods["help"]=proto;
	method_aliases["h"]=&methods["help"];
	//---------pwd--------------
    CUR_DIR=get_current_dir_name();
    PREV_DIR=CUR_DIR;
	proto.desc="Print current directory";
	proto.ret_type=real_types::WORD;
    proto.p_func=[this](std::vector<value_type>* args, value_type* retval) -> int {return pwd(args, retval);};
    args.clear();
    proto.args.clear();
    proto.args.push_back(args);
	methods["pwd"]=proto;
	//---------ls--------------
	proto.desc="ls [options] [file] - list directory contents\n\t\tFor more information use 'man ls'";
	proto.ret_type=real_types::VOID;
    proto.p_func=[this](std::vector<value_type>* args, value_type* retval) -> int {return ls(args, retval);};
	args.clear();
    proto.args.clear();
    proto.args.push_back(args);
    args.push_back({real_types::WORD, "opts_or_dir1"});
    proto.args.push_back(args);
    args.push_back({real_types::WORD, "opts_or_dir2"});
    proto.args.push_back(args);
    methods["ls"]=proto;
	//---------cd--------------
    proto.desc="cd [options] [file] - change directory";
	proto.ret_type=real_types::VOID;
    proto.p_func=[this](std::vector<value_type>* args, value_type* retval) -> int {return cd(args, retval);};
	args.clear();
    proto.args.clear();
    proto.args.push_back(args);
    args.push_back({real_types::WORD, "opts_or_dir1"});
    proto.args.push_back(args);
    args.push_back({real_types::WORD, "opts_or_dir2"});
    proto.args.push_back(args);
    methods["cd"]=proto;
	//---------cat--------------
}

void root::init_childrens()
{
	
}

int root::delete_false_functions(uint32_t arg_num, 
                                 real_types type_val, 
                                 std::vector<std::vector<arg_proto>>* arg_prototypes, 
                                 arg_depends* ad)
{
    //цикл по возможным спискам аргументам
    for(auto it1=arg_prototypes->begin(); it1!=arg_prototypes->end(); it1++)
    {
        if((*it1)[arg_num].type_val!=type_val)
        {
            it1=arg_prototypes->erase(it1);//итератор больше не инвалидируется
        }
    }
    if(arg_prototypes->size()==0)
    {//нет подходящей функции - сразу выходим
        estderr<<arg_num<<" parameter have false type"<<std::endl;
        return FUNCTION_FALSE_ARGUMENTS;
    }
    else
    {//что-то нашлось. можем выставить сразу ссылку на этот объект
        //почти. объект не доделан
        arg_depends tmp_ad;
        tmp_ad.num_of_func=func_num;
        tmp_ad.pointer.ret_type=rt;
        //здесб надо вставить значение
        ad->push_back(tmp_ad);
        return 0;
    }
}

/**
 * Функция сначала разбирает дерево функций на задачи, а затем собирает это
 * в приоритетную очередь. Приоритет в данном случае показывает, сколько задач надо доделать
 * */
void root::create_task_queue(const real_func & function, 
                            std::map<uint32_t, std::vector<real_func>> & prio_queue)
{
    uint32_t args_not_prepared=0;
    for(auto it=function.args.begin(); it!=function.args.end(); it++)
    {
        if(it->return_from_here)
        {
            args_not_prepared++;
        }
    }
    prio_queue[args_not_prepared].push_back(function);
    for(auto it=function.args.begin(); it!=function.args.end(); it++)
    {
        if(it->return_from_here)
        {
            create_task_queue(*(it->return_from_here), prio_queue);
        }
    }
}

#include "colour.hpp"

object_proto * root::check_object(const std::string & name, object_proto *where, const error_desc & ed)
{
    
}

void root::check_function(func* f, object_proto *where, const error_desc & ed)
{
    
}

int root::check_command_and_build_tree(std::vector<object>* command, real_func *rf)
{
    std::vector<error_desc> err_str;
    error_desc err_sink;
    object_proto *where=this;
    if(command)
    {
        for(auto it=command->begin();it!=command->end(); it++)
        {
            if(it->is_object)
            {//ищем по объектам
                if(it->obj.name=="true")
                {
                    
                }
                else if(it->obj.name=="false")
                {
                    
                }
                else
                {
                    //в случае успеха выдаем указатель на дочку
                    //или nullptr
                    where=check_object(it->obj.name, where, err_sink);
                    if(!where)//object not found
                    {//ищем среди функций
                        //возвращаем в случае успеха указатель real_func
                        check_function({it->obj.name, nullptr}, where, err_sink);
                    }
                    err_str.push_back(err_sink);
                }
            }
            else
            {//ищем по методам
                check_function(it->obj.f, where, err_sink);
                err_str.push_back(tmp);
            }
        }
    }
}

int root::process_task(std::map<uint32_t, std::vector<real_func>> & prio_queue)
{
    int ret;
    for(auto it=prio_queue.begin(); it!=prio_queue.end(); it++)
    {//параллелить будем потом в этом месте через io_service
        for(auto it_v=it->begin(); it_v!=it->end(); it_v++)
        {
            ret=it_v->f(it_v->args, it_v->return_to_here);
            if(ret)
            {
                estderr<<"ERROR!\n";
                return ret;
            }
        }
    }
}

static void show_error(int err_code)
{
    static const color::modifier red(color::FG_RED);
    static const color::modifier def(color::FG_DEFAULT);
    
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
    estderr.str(std::string());
}

void root::exec_conveyor(std::vector<conveyor>* c)
{
	if(c)
    {
        int ret;
        std::vector<real_func> func_tree(c->size());
        real_func rf;
        bool has_errors=false;
        for(auto it=c->begin(); it!=c->end(); it++)
        {
            //первым делом преобразуем список функций/объектов из текста в указатели
            //хорошо бы сначала проверить весь конвейер, а потом исполнять
            ret=check_command_and_build_tree(&it->command, &rf);
            if(ret)
            {
                //не вышло...
                //показываем ошибки
                has_errors=true;
                show_error(ret);
            }
            else
            {
                func_tree.push_back(rf);
            }
        }
        if(has_errors)
        {
            return;
        }
        //вышло. исполняем команды
        for(auto it=func_tree->begin(); it!=func_tree->end(); it++)
        {
            std::map<uint32_t, std::vector<real_func>> prio_queue;
            create_task_queue(*it, prio_queue);
            
            ret=process_task(prio_queue);
            
            //Errors first
            show_error(ret);
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

