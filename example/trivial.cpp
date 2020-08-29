// Copyright (C) 2020 T. Zachary Laine
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//[ trivial_example
#include <boost/parser/parser.hpp>

#include <iostream>
#include <string>


namespace bp = boost::parser;

int main()
{
    std::cout << "Enter a list of doubles, separated by commas: ";
    std::string input;
    std::cin >> input;

    auto const result = bp::parse(input, bp::double_ >> *(',' >> bp::double_));

    if (result) {
        std::cout << "You entered:\n";
        for (double x : *result) {
            std::cout << x << "\n";
        }
    } else {
        std::cout << "Please proceed to the recovery annex for cake.\n";
    }
}
//]
