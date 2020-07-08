.PHONY: build-all
build-all:
	( rm -rf build ; mkdir build ; cd build ; cmake -D CMAKE_C_COMPILER=gcc .. ; cmake -D CMAKE_C_COMPILER=gcc -build  . ; make )


.PHONY: build-all-debug
build-all-debug:
	( rm -rf build ; mkdir build ; cd build ; cmake -D CMAKE_C_COMPILER=gcc -D CMAKE_BUILD_TYPE=Debug .. ; cmake -D CMAKE_C_COMPILER=gcc -D CMAKE_BUILD_TYPE=Debug -build  . ; make )

.PHONY: docs
docs:
	(cd build ; cmake --build . --target docs)

.PHONY: valgrind-all-debug
valgrind-all-debug: build-all-debug
	bash ./scripts/valgrind.sh

.PHONY: valgrind-all
valgrind-all: build-all
	bash ./scripts/valgrind.sh

# Minimal makefile for Sphinx documentation
#

# You can set these variables from the command line, and also
# from the environment for the first two.
SPHINXOPTS    ?=
SPHINXBUILD   ?= sphinx-build
SOURCEDIR     = .
BUILDDIR      = docs-sphinx

# Put it first so that "make" without argument is like "make help".
help:
	@$(SPHINXBUILD) -M help "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) $(O)

.PHONY: help Makefile

# Catch-all target: route all unknown targets to Sphinx using the new
# "make mode" option.  $(O) is meant as a shortcut for $(SPHINXOPTS).
%: Makefile
	@$(SPHINXBUILD) -M $@ "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) $(O)
