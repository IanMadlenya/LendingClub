/*
Created on July 28, 2014

@author:     Gregory Czajkowski

@copyright:  2013 Freedom. All rights reserved.

@license:    Licensed under the Apache License 2.0 http://www.apache.org/licenses/LICENSE-2.0

@contact:    gregczajkowski at yahoo.com
*/

#ifndef __LC_ACCOUNTS_OPEN_PAST_24_MONTHS_HPP__
#define __LC_ACCOUNTS_OPEN_PAST_24_MONTHS_HPP__

#include "Filter.hpp"
#include "Loan.hpp"
#include "Utilities.hpp"

namespace lc
{

class AccountsOpenPast24Months : public Filter<AccountsOpenPast24Months>
{
public:
    static const std::string sqlite_type;
    static const std::string csv_name;
    static const std::string name;

    AccountsOpenPast24Months(const Arguments& args, unsigned* current = nullptr) : Filter<AccountsOpenPast24Months>(name, args)
    {
        static const std::vector<FilterValue>* options = create_range(0, 25, 5);
        Filter::initialize(options, current);
    }

    inline FilterValue convert(const std::string& raw_data)
    {
        return (raw_data.empty()) ? 0 : boost::lexical_cast<FilterValue>(raw_data.c_str());
    }

    const std::string get_string_value() const
    {
        return boost::lexical_cast<std::string>(get_value());
    }

    static bool static_apply(const Filter& self, const LCLoan& loan)
    {
        return (loan.acc_open_past_24mths <= self.get_value());
    }

    inline bool apply(const LCLoan& loan) const
    {
        return (loan.acc_open_past_24mths <= get_value());
    }
};

};

#endif // __LC_ACCOUNTS_OPEN_PAST_24_MONTHS_HPP__
