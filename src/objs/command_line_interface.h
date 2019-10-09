#pragma once

#include "base_arguments.h"
#include <functional>

#define cli_program(title_arg) cli::this_program.title = title_arg;
#define cli_version(version_arg) cli::this_program.version = version_arg;
#define cli_description(description_arg) cli::this_program.description = description_arg;

#define cli_describe(cli_program_name, cli_program_version, cli_program_description) \
	const decltype(cli::this_program) cli::this_program {\
		cli_program_name,\
		cli_program_version,\
		cli_program_description\
	};

#define cli_entry_point \
	int main (int argc, char **argv)

#define cli_arguments(...) \
	__VA_ARGS__ \
	cli::HelpArgument cli_help_argument; \
	cli::VersionArgument cli_version_argument; \
	cli::capture_all_arguments_from(argc, argv);

namespace cli {

	extern const struct Program {
		string title;
		string version;
		string description;
	} this_program;

	template<typename T>	
	class RequiredArgument : public ValuedArgument<T> {
		public:
			RequiredArgument(const string &argument_label, char argument_abbreviation='\0', const string &description="required") 
				:	ValuedArgument<T>(argument_label, description, argument_abbreviation) {}

			bool is_required (void) const {
				return true;
			}

	};

	template<typename T>
	class OptionalArgument : public ValuedArgument<T> {
		public:
			OptionalArgument(const T& default_value, const string &argument_label, char argument_abbreviation='\0', const string &description="optional")
			   :	ValuedArgument<T>(argument_label, description, argument_abbreviation)
			{
				this->value = default_value;
			}

			bool is_required (void) const {
				return false;
			}

	};

	class FlagArgument : public UnvaluedArgument {
		public:
			FlagArgument(const string &argument_label, char argument_abbreviation='\0', const string &description="flag");

			bool is_required(void) const;

			bool operator*(void) const;
	};

	class FunctionArgument : public UnvaluedArgument {
		private:
			function<void(void)> execute;
		public:
			FunctionArgument(const decltype(execute) &function_to_execute, const string &argument_label, char argument_abbreviation='\0', const string &description="optional execution flow");

			bool is_required(void) const;
			void set_as_present(void);
	};

	class HelpArgument : public FunctionArgument {
		public:
			HelpArgument(void);
	};

	class VersionArgument : public FunctionArgument {
		public:
			VersionArgument(void);
	};

	void capture_all_arguments_from(int argc, char **argv);

}
