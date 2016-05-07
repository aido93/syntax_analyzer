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
#ifndef ASSET_HPP
#define ASSET_HPP

#include <deque>
#include <memory>
#include <string>
#include <stdint.h>

#define ASSET_LOT10 		10
#define ASSET_LOT100 		100
#define ASSET_LOT500 		500
#define CURRENCY_LOT	 	10000

enum class currency
{
    USD, //USA
    EUR, //Europe
    JPY, //Japan
    GBP, //Great Britain
    AUD, //Australia
    CAD, //Canada
    CNY, //China
    CHF, //Switzerland
    INR, //India
    RUR  //Russia
};

typedef struct
{
	double open_price;
	double low_price;
	double high_price;
	double close_price;
	uint32_t volume;
} candle;

enum class asset_type
{
	ASSET,
	CURRENCY,
	FUTURES,
	OPTION
};

template <class T, class V> struct deque_candles
{
    std::string ticker; 
    V           days;
    T           open_prices;
    T           low_prices;
    T           high_prices;
    T           close_prices; 
    V           volumes; 
};

int load_prices(const std::string & server, 
				std::string ticker, 
				const uint32_t & from,
				const uint32_t & to);

/*return: length on success, zero on error*/
int parse_file( const std::string&  filename, 
				const std::string 	date_format,
				const std::string 	file_format,
				const char* 		delimiter,
				
                deque_candles<  std::deque<double>, 
                                std::deque<uint32_t>> & candles
              );

/**
 * Container class for all prices.
 * */

class asset
{
	public:
			
		asset(  const deque_candles<double*, 
                                    uint32_t*> & candles, 
				const uint32_t _length,
                const uint32_t _max_length,
                const currency _cur=currency::USD,
                const asset_type _t=asset_type::ASSET);
						
		asset(  const deque_candles<std::deque<double>, 
                                    std::deque<uint32_t>> & candles,
                const uint32_t _max_length,
                const currency _cur=currency::USD, 
                const asset_type _t=asset_type::ASSET);
        
        asset(  const std::string _ticker,
                const uint32_t _max_length,
                const currency _cur=currency::USD, 
                const asset_type _t=asset_type::ASSET);
				
		~asset();
		
		uint32_t 		get_length() const;	
		uint32_t 		get_max_length() const;
        currency			get_currency() const;
        type                get_type() const;
        const std::string   get_ticker() const;
        
		int get_all_days(uint32_t* d) const;
		int set_max_length(const uint32_t l);
		
		int add_day(const uint32_t day, 
					const candle & can);
		
		/*for graphics*/	
		inline candle get_candle ( const uint32_t day) const;
		int get_candles(const uint32_t * days, 
                        uint32_t count, 
                        deque_candles<double*,unsigned int*>* cs) const;
		
		/*for speed-up*/
		inline double  get_open_price (const uint32_t day) const;
		inline double  get_low_price  (const uint32_t day) const;
		inline double  get_high_price (const uint32_t day) const;
		inline double  get_mean_price  (const uint32_t day) const;
		inline double  get_range_price (const uint32_t day) const;
		inline double  get_close_price(const uint32_t day) const;
		inline uint32_t  get_volume	  (const uint32_t day) const;
		
		int get_open_prices (const uint32_t * days, uint32_t count, double* ops) const;
		int get_low_prices  (const uint32_t * days, uint32_t count, double* lps) const;
		int get_high_prices (const uint32_t * days, uint32_t count, double* hps) const;
		int get_close_prices(const uint32_t * days, uint32_t count, double* cps) const;
		int get_volumes	    (const uint32_t * days, uint32_t count, double* vs) const;
		
	private:
		asset() = delete;
		uint32_t find_element(const uint32_t day) const;
		
        mutable uint32_t	length;
        const std::string   ticker;
        mutable uint32_t 	max_length;
		const std::shared_ptr<std::deque<uint32_t>>   days;
        const std::shared_ptr<std::deque<double>>         open_prices;
        const std::shared_ptr<std::deque<double>>         mean_prices;
        const std::shared_ptr<std::deque<double>>         range_prices;
        const std::shared_ptr<std::deque<double>>         close_prices; 
        const std::shared_ptr<std::deque<uint32_t>>   volumes; 
        const currency  cur;
        const asset_type t;
};

#endif
