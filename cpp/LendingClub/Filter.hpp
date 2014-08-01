/*
Created on July 27, 2014

@author:     Gregory Czajkowski

@copyright:  2013 Freedom. All rights reserved.

@license:    Licensed under the Apache License 2.0 http://www.apache.org/licenses/LICENSE-2.0

@contact:    gregczajkowski at yahoo.com
*/

#ifndef __LC_FILTER_HPP__
#define __LC_FILTER_HPP__

#include <map>
#include <string>
#include <vector>
#include <cassert>
#include <cstdlib>
#include <algorithm>
#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>

/*
union FilterUnion {
	unsigned		_int_val;
	float			_float_val;
};

struct FilterValue {
	enum class FilterValueType : unsigned char {INT, FLOAT};		
	FilterUnion		_val;
	FilterValueType _val_type;
};
*/

namespace lc {

typedef unsigned FilterValue;
typedef boost::program_options::variables_map Arguments;

template <int T>
class Filter {
	static const int specialization = T;
public:
	Filter(const std::string& name, const Arguments& args) :
		_args(args),
		_options(NULL),
		_name(name),
		_size(0),
		_size_plus_one(0),
		_current(0)
	{
	}

	~Filter() {
		//delete _options;
	}

	void initialize(const std::vector<FilterValue>* options, unsigned* current) {
		_options = options;
		_size = options->size();
		_size_plus_one = _size + 1;
		if (nullptr == current) {
			_current = _size;
		} else {
			_current = *current;
			assert(_current < _size);
		}
	}

	unsigned convert(const std::string& str_data) {
		return boost::lexical_cast<unsigned>(str_data.c_str());
	}

	inline const FilterValue& get_current() {
		return (*_options)[_current];
	}

	std::string get_string_value() {
		const FilterValue& val = get_current();
		return boost::lexical_cast<std::string>(val);
	}

	size_t get_count() {
		return _size;
	}

	bool next() {
		_current = (_current + 1) % _size_plus_one;
		return (_current != _size);
	}

	std::vector<std::vector<FilterValue>> power_set(const std::vector<FilterValue>& options) {
		std::vector<std::vector<FilterValue>> result;
		std::vector<unsigned> v;
		size_t n = options.size();
		assert(n < 31);
	
		for(unsigned i = 0, pow2n = 1 << n; i < pow2n; ++i) {
			unsigned m = i;
			while (m > 1) {
				v.push_back(m % 2);
				m  >>= 1;
			}
			v.push_back(m);
		
			int k = 0;
			std::vector<FilterValue> set;
			for(int k = 0, end = v.size(); k < end; ++k) {
				if (v[k]) {
					set.push_back(options[k]);
				}
			}
			result.push_back(set);
		}
	}

	std::vector<FilterValue> power_bitset(const std::vector<FilterValue>& options) {
		auto l = power_set(options);

        // Now we need to sum up each individual set_tupple
		std::vector<FilterValue> result(l.size());

		unsigned idx = 0;
		for (auto set_tupple : l) {
			unsigned sum = 0;
			for (auto v : set_tupple) {
				sum += v;
			}
			result[idx] = sum;
			++idx;
		}
		return result;
	}

	Filter* increment(std::vector<Filter>& filters) {
		for (auto& filter : filters) {
			if (!filter.next()) {
				continue;
			}
			return &filter;
		}
		return nullptr;
	}

	std::string get_name() {
		return _name;
	}	
	
	std::string get_details() {
		return get_name() + "=" + get_string_value();
	}

protected:
	const Arguments& _args;
	const std::vector<FilterValue>* _options;
	std::string _name;
	size_t _size;
	size_t _size_plus_one;
	unsigned _current;
};

};

#endif // __LC_FILTER_HPP__