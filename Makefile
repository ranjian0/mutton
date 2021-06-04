all:
	@cmake -S . -B desktopbuild -DBUILD_DESKTOP=ON
	@cmake --build desktopbuild
	@cmake -S . -B androidbuild -DBUILD_ANDROID=ON
	@cmake --build androidbuild

clean:
	@rm -rf build
	@rm -rf desktopbuild
	@rm -rf androidbuild
