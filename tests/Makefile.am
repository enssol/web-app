.POSIX:

include ../common-rules.am

check_PROGRAMS = test_config_loader test_env_loader test_error_handler \
                 test_garbage_collector test_hello test_logger test_main test_validator

all: check

check:
	@echo "Running tests..."
	for test in $(check_PROGRAMS); do \
		./$$test || exit 1; \
	done
