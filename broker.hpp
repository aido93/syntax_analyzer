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
#ifndef BROKER_HPP
#define BROKER_HPP

#include <string>
#include <vector>
#include "asset.hpp"
#include "requests.hpp"

enum class commission_type
{
    FIXED,// fixed comission by lot. for example .35 USD per lot
    RELATIVE// relative comission. for example 7%=0,07 per deal
};

class broker
{
	public:
		broker( const std::string &	_name,
                const std::wstring &	_description,
                const std::wstring &	_address,
                const std::string &	_email,
                const std::wstring &	_workdir,
				const requests    & _req,
				const double		_margin,
				const double		_leverage,
                const double _comission=0.07,
                commission_type _ct=commission_type::RELATIVE);
		~broker();
		int add_asset(const asset &  _asset,
					  const uint32_t _lot_size
					  );
		size_t get_assets_count();
		
		std::string get_name() const;
		std::wstring get_description() const;
        std::wstring get_workdir() const;
		double get_margin() const;
		double get_leverage() const;
        
        void set_address(const std::wstring & _desc);
		std::wstring get_address() const;
        
        void set_email(const std::string & _email);
		std::string get_email() const;
		
        requests* send_request();
	private:
		broker() = delete;
		const std::string	name;       //full name
        std::wstring   description;//commercial requisites
        std::wstring        address;
        std::string         email;
        std::wstring        workdir;//for quotes
        requests            req;
        double				margin;
        double				leverage;
        double 		        commission;
        commission_type     ct;
        
        std::vector<unsigned int> lots;
		std::vector<asset>	assets;
};

#endif
