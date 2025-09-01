BUILD=build
PROJECT=cnmea

init:
	cmake --preset=release

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
