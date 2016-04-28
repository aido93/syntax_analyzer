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
struct object;
class object_proto;

struct arg//for func
{
    std::string var_name;
    union
    {
        double      				num;
        char*       				str;//have no dots or brackets. though may be func or object
        std::vector<object>*      	obj;//have dots
        func*       				f;//have brackets
    } val;
    type type_val;
    /*~arg()
    {
        switch(type_val)
        {
            case WORD_T:
                delete [] val.str;
            break;
            case OBJECT:
                delete val.obj;
            break;
            case FUNCTION:
            case VOID:
            default:
            
            break;
        }
    }*/
};

struct func//from command-line
{
    std::string name;
    std::unique_ptr<std::vector<arg>> args;
};

typedef int (*func_type)(std::vector<arg>);//always return error code or zero.
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
/*
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
};*/

struct object
{
    union{
            char* obj_name;
            func*  f;
         }obj;
    bool is_object;
    /*~object()
    {
        if(is_object)
        {
            delete [] obj.obj_name;
        }
        else
        {
            delete obj.f;
        }
    }*/
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
    std::vector<object>  command;
    conv_type conv;
};


#endif
