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
#include "prototypes.hpp"

std::shared_ptr<object_proto> object_proto::find_children(const std::string & name)
{
    for(auto it=childrens.begin(); it!=childrens.end(); it++)
    {
        if((*it)->get_name()==name)
        {
            return *it;
        }
    }
    return nullptr;
}

std::shared_ptr<func_proto> object_proto::find_method(const std::string & name)
{
    auto it=methods.find(name);
    if(it!=methods.end())
    {
        return std::shared_ptr<func_proto>(&it->second);
    }
    else
    {
        auto it=method_aliases.find(name);
        if(it!=method_aliases.end())
        {
            return std::shared_ptr<func_proto>(it->second);
        }
    }
    return nullptr;
}

