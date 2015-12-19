
/* Copyright (c) 2015 Christoph Husse


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
//
// Authors: 
//      # Christoph Husse

#include "stdafx.h"

#if BOOST_ALL_NO_LIB
	// this seems to remain stable across a whole bunch of boost versions and spares
	// us from all the boost compilation hassle.
	#include <boost/../libs/program_options/src/positional_options.cpp>
	#include <boost/../libs/program_options/src/utf8_codecvt_facet.cpp>
	#include <boost/../libs/program_options/src/value_semantic.cpp>
	#include <boost/../libs/program_options/src/variables_map.cpp>
	#include <boost/../libs/program_options/src/cmdline.cpp>
	#include <boost/../libs/program_options/src/config_file.cpp>
	#include <boost/../libs/program_options/src/convert.cpp>
	#include <boost/../libs/program_options/src/options_description.cpp>
	#include <boost/../libs/program_options/src/parsers.cpp>
#endif