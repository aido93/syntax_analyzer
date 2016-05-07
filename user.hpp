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
#ifndef USER_HPP
#define USER_HPP

#include <vector>
#include "broker.hpp"

class user
{
	public:
		user(const uint32_t reserved);
		user(const std::string & config_filename);
		user(const std::vector<broker*> & _brokers, 
			 const std::vector<authorization> & _logins);
		~user();
		void add_broker(broker * _broker, 
						const authorization & _auth);
		void delete_broker( const std::string & _broker_name);
		broker* get_broker(const std::string & name);
		int save_config(const std::string & config_filename);
        
        void exec_script(const std::string & filename, bool silent);
        void update(const std::string & filename, bool silent);
	private:
		//enable different logins for one broker
		std::vector<authorization> logins;
		std::vector<broker*> brokers;
};

#endif
