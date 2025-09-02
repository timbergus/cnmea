BUILD=build
BUILD_TYPE ?= debug
PROJECT=cnmea

# To execute it with a different value you can use make BUILD_TYPE=release
init:
	cmake --preset=$(BUILD_TYPE)

project: init
	cmake --build $(BUILD)

# Use "~/.local" for installation prefix to install it for the current user.
install: project
	cmake --install $(BUILD) --prefix install

start: project
	./$(BUILD)/$(PROJECT)_usage

documentation: project
	cmake --build $(BUILD) --target $(PROJECT)_docs

test: project
	ctest --test-dir $(BUILD)

clean:
	rm -rf $(BUILD) install docs .cache
