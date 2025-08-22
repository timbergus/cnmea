BUILD=build
PROJECT=cnmea

init:
	cmake -B $(BUILD) \
  	-DCMAKE_C_COMPILER=/opt/homebrew/opt/llvm/bin/clang \
  	-DCMAKE_CXX_COMPILER=/opt/homebrew/opt/llvm/bin/clang++

project: init
	cmake --build $(BUILD) --config Release

install: project
	cmake --install $(BUILD) --config Release --prefix install

start: project
	./$(BUILD)/$(PROJECT)_usage

documentation: project
	cmake --build $(BUILD) --target docs

test: project
	ctest --test-dir $(BUILD)

package: project documentation
	cpack -G ZIP --config $(BUILD)/CPackConfig.cmake

clean:
	rm -rf $(BUILD) .cache docs _CPack* *.zip
