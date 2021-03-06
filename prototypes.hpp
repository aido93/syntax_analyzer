/*
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above
 *   copyright notice, this list of conditions and the following disclaimer
 *   in the documentation and/or other materials provided with the
 *   distribution.
 * * Neither the name of the  nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */
#ifndef PROTOTYPES_HPP
#define PROTOTYPES_HPP

#include "types.hpp"
#include <map>
#include <vector>
#include <memory>
#include <functional>

class object_proto;
struct value_type;
struct real_func;
/**
 * Список реальных типов данных, с которыми умеет работать программа
 * */
enum class real_types
{
	ROOT,
	VOID,
    BROKER,
    USER,
    ASSET,
    NUM,
    WORD,
    SMATRIX,//Symmetric matrix
    MATRIX,
    BOOL
};

/**
 * Тип прототипа аргумента - тип аргумента, имя аргумента
 * */
struct arg_proto//for func_proto
{
    real_types type_val;//name of the arg type
    std::string var_name;
};

/**
 * Прототип функции:
 * 1) desc - описание
 * 2) ret_type - тип возвращаемого значения
 * 3) args - возможные наборы аргументов
 * 4) p_func - указатель на функцию
 * */
struct func_proto//for comparing with proto
{
    std::string desc;
    real_types ret_type;
    std::vector<std::vector<arg_proto>> args;
    //gen_function
    std::function<int(std::vector<value_type>*, value_type*)> p_func;
};

/**
 * Главный класс - прототип всех классов.
 * */
class object_proto //for hierarchy
{
    private:
        const std::string name;
        const std::unique_ptr<object_proto> parent;
        virtual void init_methods() = 0;
        virtual void init_childrens() = 0;
        real_types t;
    protected:
        std::map<std::string, func_proto>   methods;
        std::map<std::string, func_proto*>   method_aliases;
        std::vector<std::shared_ptr<object_proto>> childrens;
        real_types get_type() const
        {
            return t;
        }
    public:
        object_proto(const std::string & _name, 
                     object_proto* _parent, real_types _t): 
                     name(_name), parent(std::move(_parent)), t(_t)
                     {
                         //init_methods();
                     }
        ~object_proto(){}
        std::shared_ptr<object_proto> find_children(const std::string & name);
        std::shared_ptr<func_proto> find_method(const std::string & name);
        const std::string get_name() const
        {
            return name;
        }
        const std::unique_ptr<object_proto> & get_parent() const
        {
            return parent;
        }
};

/*
 * Прототипы окончены.
 * Переходим к реальным данным
 * */

/**
 * Реальный тип любой переменной в программе. 
 * 
 * arg_proto используется для указания в прототипах
 * value_type - для передачи значений в конкретные функции.
 * */
struct value_type
{
    real_types ret_type;
    union
    {
        object_proto*   obj;/*Broker, user, asset, matrix, smatrix*/
        bool            boolean;
        double          num;
        std::string*    str;//have no dots or brackets.
    } val;
};

/**
 * Тип функции. первый аргумент - указатель на вектор реальных параметров
 * Второй аргумент - указатель на возвращаемое значение.
 * Важно!!! каждая функция сама выделяет память на return.
 * Освобождается память после выполнения функции верхнего уровня
 * */
typedef int(*gen_function)(std::vector<value_type>*, value_type*);

/**
 * для составления нового древа инструкций
 * */
struct arg_depends
{
    value_type value;
    real_func *return_from_here;//nullptr if not
};


/**
 * Реальная функция, которая будет выполняться в программе. 
 * f - указатель на нее
 * args - вектор аргументов с порядком вычисления функций (tree)
 * 
 * Программа сама осуществляет связку по указателю
 * */
struct real_func
{
    gen_function f;
    std::vector<arg_depends> args;
    value_type *return_to_here;
};

struct error_desc
{
    int num;
    std::string desc;
};
#endif
