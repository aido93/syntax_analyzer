/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 */
#include "root.hpp"

int root::exit(std::vector<value_type> *in, value_type* retval)
{
	::exit(EXIT_SUCCESS);
    return 0;
}

int root::help(std::vector<value_type> *in, value_type* retval)
{
	estdout<<"This is help"<<std::endl;
    for(auto it=methods.begin(); it!=methods.end();it++)
    {
        estdout<<it->first<<"\t - \t"<<it->second.desc<<std::endl;
    }
    retval=nullptr;
	return 0;
}

int root::pwd(std::vector<value_type> *in, value_type* retval)
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

int root::ls(std::vector<value_type>* in, value_type* retval)
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

int root::cd(std::vector<value_type>* in, value_type* retval)
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
