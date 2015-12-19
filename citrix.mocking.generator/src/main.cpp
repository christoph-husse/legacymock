
/* Copyright (c) 2015 Christoph Husse


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
//
// Authors: 
//      # Christoph Husse

#include "stdafx.h"
#include "CppParser.h"
#include "Configuration.h"
#include "HeaderEmitter.h"
#include "ImplementationEmitter.h"
#include "../citrix.mocking/include/citrix/mocking/RegexMapper.h"

#include <boost/program_options.hpp>

#ifdef _MSC_VER 
#   pragma comment(linker, "/subsystem:console")
#endif

using namespace citrix::mocking;
namespace po = boost::program_options;

static void parseCommandLine(int argc, char** argv);
static void initializeConfiguration(const po::variables_map& args);
static void parseHeaderFile();

void parseCommandLine(int argc, char** argv) {
    po::options_description desc("Creates a mock framework for a given C++ class");
    desc.add_options()
        ("python", po::value<std::string>(), "Path to your python 2.7 executable.")
        ("parser-script", po::value<std::string>(), "Path to the LegacyMock C++ header parser script.")
        ("input", po::value<std::string>(), "A C++ header file to parse.")
        ("outdir", po::value<std::string>(), "All generated files will be stored in here. If the directory does not exist, it will be created.")
        ("safe-update-check", "Use file content comparison instead of file time comparison. This is recommended if there is a chance that the python generator itself changes. But it is also much slower.")
        ("verbose", "Do emit console output for non-error conditions.")
        ("class-name", po::value<std::string>(), "Process the class with the given name (unqualified, no namespaces). By default, the first class encountered in the header file is processed.")
        ("mock-all-classes", "All classes in the file are processed and wrapped into individual mock stubs.")
        ("class-prefix", po::value<std::string>(), "Allows you to insert custom code into the public preamble of a class. This code is inserted into the public section before any generated code. It is also emitted into the global namespace within the mock implementation.")
        ("include", po::value<std::vector<std::string>>()->multitoken(), "Include this file in the generated public mock header. Use multiple arguments to add multiple files.")
        ("enable-tracing", "Emit mocks that are tracable. This will likely require you to implement translators for the various types that might need to be serialized.")
        ("enable-default-impl", "By default, an abstract mock interface is emitted. But if you know for sure that all return types have default constructors, you can use this option to generate default implementations instead.")
        ("mock-free-api", "Interpret member functions as free functions (like Windows API). This will cause the free functions to be hooked but still grouped into mock classes with support for custom mocks, Google Mock & tracing as usual.")
        ("set-probing-instance", po::value<std::string>(), "Sets the name of the probing class (fully qualified, make sure you also add it\'s header to the include list). You need to provide an explicit probing class if the mocked class is abstract. A probing class should derive from the abstract class you want to mock and provide a default implementation for all abstract functions.")
        ("pure-static-class", "Must be specified if your class is not constructible. This will prevent any non-static members from being hooked.")
        ("mock-protected", "Also mock protected member functions. This requires you to change the source code of the target class to include \"template<typename> friend class ::CitrixMockingAccessor;\" in the class definition.")
        ("mock-private", "Also mock private member functions. This requires you to change the source code of the target class to include \"template<typename> friend class ::CitrixMockingAccessor;\" in the class definition.")
        ("variable-accessors", "Generates setters and getters for member variables, so you can assert on object state.")
        ("disable-gmock", "Do not generate any GMock code. This will significantly decrease compile times for generated headers and should be used when you don\'t intend to use GMock for the mocked class.")
        ("mock-lifetime", "Mock constructors & destructor. This is only supported for classes who have user defined constructors & destructor and will raise an error otherwise.")
        ("enable-ifdef-mapping", "Try to replicate conditional compilation layout in mocked files. This is a very fragile feature.")
        ("debug-omit-impl", "Do not implement the internal mock code. This is useful for easier debugging when mocks can't be compiled and can limit the error amount to something more manageable.");

    po::variables_map vm;
	
	try
	{
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);    
	} catch(const std::exception& e) {
        std::cout << desc << std::endl;
        Configuration::throwError("Failed to parse command line \"" + (std::string)e.what() + "\".");
    }

    if(vm.count("help")) {
		std::cout << desc << "\n";
		throw;
	}

    initializeConfiguration(vm);
}

void initializeConfiguration(const po::variables_map& args) {

    try {
        if(args.count("include")) {
            Configuration::publicIncludes = args["include"].as<std::vector<std::string>>();
        }

        Configuration::pythonExePath = args["python"].as<std::string>();
        Configuration::parserScriptPath = args["parser-script"].as<std::string>();
        Configuration::enableCtorMocking = false;
        Configuration::enableDtorMocking = false;
        Configuration::dstDir = args["outdir"].as<std::string>();
        Configuration::headerPath = args["input"].as<std::string>();
        Configuration::classPrefix = args.count("class-prefix") ? args["class-prefix"].as<std::string>() : "";
        Configuration::className = args.count("class-name") ? args["class-name"].as<std::string>() : "";
        Configuration::isFreeApi = args.count("mock-free-api") > 0;
        Configuration::beSilent = args.count("verbose") == 0;
        Configuration::safeUpdateCheck = args.count("safe-update-check") > 0;
        Configuration::enableTracing = args.count("enable-tracing") > 0;
        Configuration::enableDefaultImpl = args.count("enable-default-impl") > 0;
        Configuration::pureStaticClass = args.count("pure-static-class") > 0;
        Configuration::mockVariables = args.count("variable-accessors") > 0;
        Configuration::disableGMock = args.count("disable-gmock") > 0;
        Configuration::customProbingInstance = args.count("set-probing-instance") ? args["set-probing-instance"].as<std::string>() : "";
        Configuration::mockAllClasses = args.count("mock-all-classes") > 0;
        Configuration::enableIfdefMapping = args.count("enable-ifdef-mapping") > 0;
        Configuration::debugOmitImpl = args.count("debug-omit-impl") > 0;


        // verify that output directory exists & create it if necessary

    } catch(const std::exception& e) {
        Configuration::throwError("Failed to parse command line \"" + (std::string)e.what() + "\".");
    }
}

void writeFileIfChanged(const std::string& targetFile, const std::string& content) {
    FILE* target = fopen(targetFile.c_str(), "r");

    if(target) {
        fclose(target);

        if(StringUtils::fromFile(targetFile) == content) {
            return;
        }
    }

    if(!StringUtils::toFile(targetFile, content)) {
        Configuration::throwError("Could not write to file \"" + targetFile + "\".");
    }
}

void processClasses() {

    if(Configuration::classes.empty()) {
        Configuration::logWarning("No classes found.");
        return;
    }

    if(Configuration::className.empty()) {
        Configuration::className = Configuration::classes.front().name;
    }

    const std::vector<std::string> typeDefUserConfig = Configuration::typeDefs;

    for(size_t i = 0; i < Configuration::classes.size(); i++) {

        // select proper class
        if(!Configuration::mockAllClasses) {
            Configuration::classToMock = NULL;
            BOOST_FOREACH(const CppClass& klass, Configuration::classes) {
                if(klass.name == Configuration::className) {
                    Configuration::classToMock = &klass;
                }
            }

            if(Configuration::classToMock == NULL) {
                Configuration::throwError("Class named \"" + Configuration::className + "\" could not be found.");
            }
        } else {
            Configuration::classToMock = &Configuration::classes[i];
        }

        std::string className = Configuration::className = Configuration::classToMock->name;

        Configuration::mockManagerName = className + "MockManager";
        Configuration::mockBaseName = className + "Mock";
        Configuration::gmockName = className + "GMock";
        Configuration::hooksName = className + "Hooks";
        Configuration::hooksInternalName = className + "HooksInternal";
        Configuration::headerGuard = "_CITRIX_MOCKING_AUTOGEN_" + className + "_H_";

        Configuration::methodsToMock.clear();
        Configuration::variablesToMock.clear();
        Configuration::methodsAndVariablesToMock.clear();

        BOOST_FOREACH(const std::string& typeDef, Configuration::classToMock->enums) {
            Configuration::typeDefs.push_back("typedef " + Configuration::classToMock->nameSpace + "::" + className + "::" + typeDef + " " + typeDef + ";");
        }

        BOOST_FOREACH(const CppMethod& m, Configuration::classToMock->methods) {

            if(m.isConstructor || m.isAbstract || m.isDestructor)
                continue;

            if(m.isGetter && !Configuration::mockVariables)
                continue;

            if(!m.isGetter) {
                Configuration::methodsToMock.push_back(m);
            } else {
                Configuration::variablesToMock.push_back(m);
            }

            Configuration::methodsAndVariablesToMock.push_back(m);
        }

        HeaderEmitter emitter;
        writeFileIfChanged(Configuration::dstDir + "/" + Configuration::mockBaseName + ".h", emitter.process());

        ImplementationEmitter implEmitter;
        writeFileIfChanged(Configuration::dstDir + "/" + Configuration::mockBaseName + ".cpp", implEmitter.process());

        if(!Configuration::mockAllClasses) {
            break;
        }
    }
}

int main(int argc, char** argv) {
    try {
        parseCommandLine(argc, argv);

        CppParser::parse(Configuration::headerPath, Configuration::classes, Configuration::headerContent);

        processClasses();
    } catch(...) {
        return -1;
    }

    return 0;
}