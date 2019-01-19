.PHONY: all
all: build_main build_test

.PHONY: build_main
build_main:
	docker run --rm -v $(CURDIR):/usr/src/vanilla-core -w /usr/src/vanilla-core/central/build cross-compiler-image cmake ..
	docker run --rm -v $(CURDIR):/usr/src/vanilla-core -w /usr/src/vanilla-core/central/build cross-compiler-image make

.PHONY: build_test
build_test:
	docker run --rm -v $(CURDIR):/usr/src/vanilla-core -w /usr/src/vanilla-core/central/test/build cross-compiler-image cmake ..
	docker run --rm -v $(CURDIR):/usr/src/vanilla-core -w /usr/src/vanilla-core/central/test/build cross-compiler-image make

.PHONY: build_images
build_images:
	docker build -t cross-compiler-image -f docker/arm-rpi/Dockerfile docker/arm-rpi
