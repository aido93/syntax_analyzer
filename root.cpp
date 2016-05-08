
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

int root::process_object(std::vector<object>* obj)
{
    if(obj)
    {
        int ret;
        std::vector<real_func*> *func_tree= nullptr;
        func_number=0;
        //первым делом при выполнении команды проверяем тип объекта и исполняемых функций
        //эта функция проверяет само наследование объектов, возвращаемые типы функций,
        //а также соответствие аргументов каждой функции нужному типу
        //в retval лежит дерево вызовов функций, то есть указатели на конкретные функции 
        //вместе с указателями на их аргументы. Если аргумент уже готов, то заполнение идет сразу.
        //Если нет, то выделяется память создающей функцией и скидывает туда указатель на выделенную память
        //Заполнение аргументов будет идти в обратном порядке по вектору
        real_types rt;
        ret=check_obj_childrens(obj, func_tree, &rt);
        if(ret)
            return ret;
        
        value_type tmp;
        uint32_t func_counter=0;
        //только если проверка успешна - начинаем выполнять
        for(auto it=func_tree->rbegin(); it!=func_tree->rend(); it++)
        {
            func_counter++;
            tmp={real_types::VOID, 0};
            //создаем список аргументов
            std::vector<value_type> a;
            for(auto it1=(*it)->args.begin();it1!=(*it)->args.end(); it1++)
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
            for(auto it1=func_tree->rbegin(); it1!=it; it1--)
            {
                for(auto it2=(*it1)->args.begin(); it2!=(*it1)->args.end(); it2++)
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


int root::delete_false_functions(uint32_t arg_num, real_types type_val, 
                                 std::vector<std::vector<arg_proto>*>* arg_prototypes, 
                                 std::vector<arg_dependencies>* ad, uint32_t func_num)
{
    //цикл по возможным спискам аргументам
    for(auto it1=arg_prototypes->begin(); it1!=arg_prototypes->end(); it1++)
    {
        if((**it1)[arg_num].type_val!=type_val)
        {
            it1=arg_prototypes->erase(it1);//итератор больше не инвалидируется
        }
    }
    if(arg_prototypes->size()==0)
    {//нет подходящей функции - сразу выходим
        estderr<<arg_num<<" parameter cannot be number"<<std::endl;
        return FUNCTION_FALSE_ARGUMENTS;
    }
    else
    {//что-то нашлось. можем выставить сразу ссылку на этот объект
        //почти. объект не доделан
        arg_dependencies tmp_ad;
        tmp_ad.num_of_func=func_num;
        tmp_ad.pointer.ret_type=type_val;
        //здесб надо вставить значение
        ad->push_back(tmp_ad);
        return 0;
    }
}

/**
 * функция проходит по параметрам и если параметр не подходит под шаблон, то удаляет шаблон из списка
 * 
 * func* fp - структура с именем функции и аргументами после парсинга
 * real_func* f - структура с указателем на настоящую функцию и вектором реальных параметров
 * real_types* ret - тип ретурна
 * */

int root::check_func_arg_types(func* fp, real_func* f, real_types* ret)
{
    //ищем метод по имени. имя - уникальный идентификатор каждого метода
    //список параметров должна обрабатывать сама функция
    //пока ищется только в самом верхнем классе, откуда вызвана функция
    auto m=find_method(fp->name);
    if(m)
    {
        func_number++;
        //создаем временный объект, который в конце выполнения функции скопируем в f
        real_func tmp_f({m->p_func.target<int(std::vector<value_type>*, value_type*)>(), std::vector<arg_dependencies>(m->args.size())});
        std::vector<real_func*> pointers_tree;
        
        if(!tmp_f.f)
        {
            return FUNCTION_NOT_DEFINED;
        }
        //сразу выставляем ретурн функции
        *ret=m->ret_type;
        
        //собираем все подходящие по длине шаблоны в одном месте tmp_args
        const size_t length=fp->args->size();
        std::vector<std::vector<arg_proto>*> tmp_args;
        for(auto it_proto=m->args.begin(); it_proto!=m->args.end(); it_proto++)
        {
            if(it_proto->size()==length)
            {//возможно, шаблон функции подойдет
                tmp_args.push_back(&(*it_proto));
            }
        }
        if(tmp_args.size()==0)
        {//нет подходящей функции - сразу выходим
            estderr<<"No one function prototype can be used"<<std::endl;
            return FUNCTION_FALSE_ARGUMENTS;
        }
        //проходим по списку аргументов, которые переданы
        uint32_t counter=0;
        for(auto it=fp->args->begin(); it!=fp->args->end(); it++)
        {
            switch(it->type_val)
            {
                case type::VOID:
                {
                    estderr<<"Void parameter!"<<std::endl;
                    return FUNCTION_FALSE_ARGUMENTS;
                }
                break;
                
                case type::NUM_T:
                {
                    int ret=delete_false_functions(counter, real_types::NUM, 
                                                    &tmp_args, &tmp_f.args,0);
                    if(ret)
                    {
                        return ret;
                    }
                    tmp_f.args.back().pointer.val.num=it->val.num;
                }
                break;
                
                case type::WORD_T:
                {
                    if(it->val.str=="true")
                    {//мб внести в лексику?
                        int ret=delete_false_functions(counter, real_types::BOOL, 
                                                    &tmp_args, &tmp_f.args,0);
                        if(ret)
                        {
                            return ret;
                        }
                        tmp_f.args.back().pointer.val.boolean=true;
                    }
                    else if(it->val.str=="false")
                    {
                        int ret=delete_false_functions(counter, real_types::BOOL, 
                                                    &tmp_args, &tmp_f.args,0);
                        if(ret)
                        {
                            return ret;
                        }
                        tmp_f.args.back().pointer.val.boolean=false;
                    }
                    else
                    {
                        auto c=find_children(it->val.str);
                        real_types rt;
                        if(!c)
                        {//no such object. maybe function without args?
                            real_func f;
                            func ff={it->val.str, nullptr};
                            int ret=check_func_arg_types(&ff, &f, &rt);
                            if(ret)
                            {//no such function
                                return ret;
                            }
                            else
                            {
                                ret=delete_false_functions(counter, rt, 
                                                    &tmp_args, &tmp_f.args,func_number);
                                if(ret)
                                {//no such function
                                    return ret;
                                }
                                tmp_f.args.back().pointer.val.f=&f;
                            }
                        }
                        else
                        {
                            int ret=check_obj_childrens(it->val.obj, &pointers_tree, &rt);
                            if(ret)
                            {
                                return ret;
                            }
                                ret=delete_false_functions(counter, rt, 
                                                    &tmp_args, &tmp_f.args,0);
                                if(ret)
                                {
                                    return ret;
                                }
                                switch(rt)
                                {
                                    case real_types::BROKER:
                                        tmp_f.args.back().pointer.val.b=&pointers_tree;
                                    break;
                                    
                                    case real_types::USER:
                                        tmp_f.args.back().pointer.val.u=&pointers_tree;
                                    break;
                                    
                                    case real_types::ASSET:
                                        tmp_f.args.back().pointer.val.a=&pointers_tree;
                                    break;
                                    /*
                                    case real_types::MATRIX:
                                        tmp_f.args.back()->val.b
                                    break;
                                    
                                    case real_types::SMATRIX:
                                        tmp_f.args.back()->val.b
                                    break;*/
                                }
                        }
                    }
                }
                break;
                case type::OBJECT:
                {
                    real_types rt;
                    int ret=check_obj_childrens(it->val.obj, &pointers_tree, &rt);
                    if(ret)
                    	return ret;
                    ret=delete_false_functions(counter, rt, 
                                                    &tmp_args, &tmp_f.args,0);
                    if(ret)
                    {
                        return ret;
                    }
                    switch(rt)
                    {
                        case real_types::BROKER:
                            tmp_f.args.back().pointer.val.b=&pointers_tree;
                        break;
                        
                        case real_types::USER:
                            tmp_f.args.back().pointer.val.u=&pointers_tree;
                        break;
                        
                        case real_types::ASSET:
                            tmp_f.args.back()->pointer.val.a=&pointers_tree;
                        break;
                        /*
                        case real_types::MATRIX:
                            tmp_f.args.back()->val.b
                        break;
                                    
                        case real_types::SMATRIX:
                            tmp_f.args.back()->val.b
                        break;*/
                    }
                }
                break;
                case type::FUNCTION:
                {
                    real_types rt;
                    real_func f;
                    int ret=check_func_arg_types(it->val.f, &f, &rt);
                    if(ret)
                    {//no such function
                        return ret;
                    }
                    else
                    {
                        ret=delete_false_functions(counter, rt, 
                                            &tmp_args, &tmp_f.args,func_number);
                        if(ret)
                        {//no such function
                            return ret;
                        }
                        tmp_f.args.back().pointer.val.f=&f;
                    }
                }
                break;
            }
            counter++;
        }
        *f=tmp_f;
    }
    else
    {
        return FUNCTION_NOT_FOUND;
    }
}

bool root::check_hierarchy(real_types prev, real_types next)
{
    if( (prev==real_types::ROOT && next==real_types::BROKER) ||
        (prev==real_types::ROOT && next==real_types::WORD) ||
        (prev==real_types::ROOT && next==real_types::NUM) ||
        (prev==real_types::ROOT && next==real_types::BOOL) ||
        (prev==real_types::BROKER && next==real_types::USER) ||
        (prev==real_types::USER && next==real_types::ASSET) ||
        (prev==real_types::ASSET && next==real_types::SMATRIX) ||
        (prev==real_types::ASSET && next==real_types::MATRIX) ||
        (prev==real_types::ASSET && next==real_types::NUM) ||
        (prev==real_types::ASSET && next==real_types::WORD) ||
        (prev==real_types::ASSET && next==real_types::BOOL) ||
        (prev==real_types::USER && next==real_types::WORD) ||
        (prev==real_types::USER && next==real_types::NUM) ||
        (prev==real_types::USER && next==real_types::BOOL) ||
        (prev==real_types::BROKER && next==real_types::WORD) ||
        (prev==real_types::BROKER && next==real_types::NUM))
    return true;
    
    else return false;
}

int root::check_obj_childrens(std::vector<object>* obj, std::vector<real_func*>* pointers_tree, real_types* rt)
{
	if(obj)
    {
		//It is safe, 'cause root must be one and creates when application begin to work
		//and dies when application finish to work
		object_proto* current_obj=this;
		real_types r=real_types::ROOT, old_r;
        bool is_void=true;
        std::vector<real_func*>* pt=new std::vector<real_func*>(obj->size());
        real_func* f;
        int ret=0;
        for(auto it=obj->begin(); it!=obj->end(); it++)
        {
                f=nullptr;
                old_r=r;//иерархия root-брокер-юзер-акция
                if(!(it->is_object))//function(args) apriori
                {
                    //проверяем аргументы функции, заносим в r тип возврата
                    ret=check_func_arg_types(it->obj.f, f, &r);
                    if(ret)
                    {//if there was any error - go out (or try to find all errors?)
                        return ret;
                    }
                    else
                    {
                        //enabled hierarchy
                        if(check_hierarchy(old_r,r))
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
                        if(check_hierarchy(old_r,r))
                            pt->push_back(f);
                        }
                    }
                    else
                    {//argument of the function
                        r=current_obj->get_type();
                        //enabled hierarchy
                        if(!(check_hierarchy(old_r,r)))
                          {
                              estderr<<"Forbidden hierarchy type!"<<std::endl;
                              return FUNCTION_FALSE_ARGUMENTS;
                          }
                    }
                }
        }
        pointers_tree=pt;
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

