#include <iostream>
#include "chain.h"

int main() {
	auto unknown_handler = std::make_unique<UnknownHandler>(nullptr);
	auto warning_handler = std::make_unique<WarningHandler>(std::move(unknown_handler));
	auto error_handler = std::make_unique<ErrorHandler>(std::move(warning_handler), "error.log");
	auto fatal_error_handler = std::make_unique<FatalErrorHandler>(std::move(error_handler));

	try {
		fatal_error_handler->receiveLog(WarningMessage());
		fatal_error_handler->receiveLog(ErrorMessage());
		fatal_error_handler->receiveLog(FatalErrorMessage());
		fatal_error_handler->receiveLog(UnknownMessage());
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}
