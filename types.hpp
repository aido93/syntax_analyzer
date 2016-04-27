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
#ifndef TYPES_HPP
#define TYPES_HPP

#include <memory>
#include <string>
#include <vector>
#include <stdint.h>
#include <readline/readline.h>

enum type
{
    VOID,
	NUM_T,
	WORD_T,
    OBJECT,
    FUNCTION,
    FILENAME_T
};

struct func;
struct chain;
class object_proto;

struct arg_list//for func
{
    uint32_t number;
    std::string var_name;
    type type_val;
    union
    {
        double      num;
        char*       str;//have no dots or brackets. though may be func or object
        chain*      obj;//have dots
        func*       f;//have brackets
    } val;
    std::shared_ptr<arg_list> prev_arg;
    std::shared_ptr<arg_list> next_arg;
    ~arg_list()
    {
        if(next_arg.get())
            next_arg.reset();
    }
};

struct func//from command-line
{
    std::string name;
    arg_list *args;
};

typedef int (*func_type)(arg_list*);//always return error code or zero.
									//always have arg list

struct arg_proto//for func_proto
{
    type type_val;
    std::string var_name;
};

struct func_proto//for comparing with proto
{
    std::string name;
    uint32_t arg_count;
    std::string desc;
    type retval;
    arg_proto *prototype;
    bool operator < (const func_proto &a) const
    {
        return (this->name<a.name);
    }
};

class object_proto //for hierarchy
{
    private:
        const std::string name;
        const object_proto* parent;
    public:
        object_proto(const std::string & _name, 
                     const object_proto* _parent): 
                     name(_name), parent(_parent){}
        ~object_proto(){}
        std::vector<func_proto>   methods;
        std::vector<std::shared_ptr<object_proto>> fields;
        const std::string get_name() const
        {
            return name;
        }
        const object_proto* get_parent() const
        {
            return parent;
        }
};

struct chain
{
    uint32_t number;
    union{
            char* obj_name;
            func*  f;
         }object;
    bool is_object;
    std::shared_ptr<chain> prev_link;
    std::shared_ptr<chain> next_link;
    ~chain()
    {
        if(next_link.get())
            next_link.reset();
    }
};

enum conv_type
{
	AND_CONV,//&&
	OR_CONV,//||
	SEMICOLON_CONV,//; - default
	NO_CONV
};

struct conveyor
{
    uint32_t  number;
    chain*    command;
    conv_type conv;
    std::unique_ptr<conveyor> prev_link;
    conveyor* next_link;
    ~conveyor()
    {
        //std::cout<<"destructor "<<number<<" "<<command->object.obj_name<<std::endl;
    }
};

#endif
