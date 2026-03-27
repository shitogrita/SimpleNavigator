SHELL := /bin/bash

CMAKE     ?= cmake
CONFIG    ?= Debug
BUILD_DIR ?= build
COV_BUILD ?= build-coverage

# CMake targets
APP_TARGET  ?= Main
TEST_TARGET ?= run_test

# Coverage
LCOV     ?= lcov
GENHTML  ?= genhtml
GCOV_DIR := $(COV_BUILD)/coverage
GCOV_INFO_FILE := $(GCOV_DIR)/coverage.info

.PHONY: all configure test clean  gcov_report  clang-format

all: configure
	@$(CMAKE) --build "$(BUILD_DIR)" --target "$(APP_TARGET)" --config "$(CONFIG)"

configure:
	@$(CMAKE) -S . -B "$(BUILD_DIR)" -DCMAKE_BUILD_TYPE="$(CONFIG)"

test: configure
	@$(CMAKE) --build "$(BUILD_DIR)" --target "$(TEST_TARGET)" --config "$(CONFIG)"
	@set -e; \
	for p in \
	  "$(BUILD_DIR)/$(TEST_TARGET)" \
	  "$(BUILD_DIR)/$(CONFIG)/$(TEST_TARGET)" \
	  "$(BUILD_DIR)/bin/$(TEST_TARGET)" \
	  "$(BUILD_DIR)/$(CONFIG)/bin/$(TEST_TARGET)" \
	  "$(BUILD_DIR)/$(TEST_TARGET).app/Contents/MacOS/$(TEST_TARGET)" \
	  "$(BUILD_DIR)/$(CONFIG)/$(TEST_TARGET).app/Contents/MacOS/$(TEST_TARGET)"; do \
	  if [ -x "$$p" ]; then exec "$$p"; fi; \
	done; \
	echo "Executable $(TEST_TARGET) not found in $(BUILD_DIR)."; \
	exit 1

clean:
	@rm -rf "$(BUILD_DIR)" "$(COV_BUILD)" docs "$(DIST_DIR)"

gcov_report:
	@mkdir -p "$(COV_BUILD)"
	@$(CMAKE) -S . -B "$(COV_BUILD)" \
		-DCMAKE_BUILD_TYPE=Debug \
		-DCMAKE_C_FLAGS="--coverage" \
		-DCMAKE_CXX_FLAGS="--coverage" \
		-DCMAKE_EXE_LINKER_FLAGS="--coverage" \
		-DCMAKE_SHARED_LINKER_FLAGS="--coverage"

	@$(CMAKE) --build "$(COV_BUILD)" --target "$(TEST_TARGET)" --config Debug

	@set -e; \
	for p in \
	  "$(COV_BUILD)/$(TEST_TARGET)" \
	  "$(COV_BUILD)/Debug/$(TEST_TARGET)" \
	  "$(COV_BUILD)/bin/$(TEST_TARGET)" \
	  "$(COV_BUILD)/Debug/bin/$(TEST_TARGET)" \
	  "$(COV_BUILD)/$(TEST_TARGET).app/Contents/MacOS/$(TEST_TARGET)" \
	  "$(COV_BUILD)/Debug/$(TEST_TARGET).app/Contents/MacOS/$(TEST_TARGET)"; do \
	  if [ -x "$$p" ]; then "$$p" || true; break; fi; \
	done

	@if ! command -v "$(LCOV)" >/dev/null 2>&1; then exit 1; fi
	@if ! command -v "$(GENHTML)" >/dev/null 2>&1; then exit 1; fi

	@mkdir -p "$(GCOV_DIR)"
	@$(LCOV) --directory "$(COV_BUILD)" --capture \
		--output-file "$(GCOV_INFO_FILE)" \
		--ignore-errors inconsistent,gcov,unsupported,format >/dev/null

	@$(LCOV) --remove "$(GCOV_INFO_FILE)" \
		"/usr/*" \
		"/System/*" \
		"/Library/*" \
		"/opt/homebrew/*" \
		"*/_deps/*" \
		"*/CMakeFiles/*" \
		"*/moc_*" \
		"*/qrc_*" \
		"*/gtest/*" \
		"*/gmock/*" \
		--output-file "$(GCOV_INFO_FILE)" \
		--ignore-errors inconsistent,gcov,unsupported,format,unused >/dev/null

	@$(GENHTML) "$(GCOV_INFO_FILE)" --output-directory "$(GCOV_DIR)" \
		--ignore-errors inconsistent,gcov,unsupported,format,category,unused >/dev/null

	@echo "$(GCOV_DIR)/index.html"


clang-format:
	 @set -e; \
	 find core web ml view test -type f \( \
	   -name "*.c" -o -name "*.cc" -o -name "*.cpp" -o -name "*.cxx" -o \
	   -name "*.h" -o -name "*.hh" -o -name "*.hpp" -o -name "*.hxx" \
	 \) -print0 | xargs -0 clang-format -i -style=Google