TARGET = linux-vcs

all: $(TARGET)

linux-vcs: linux-vcs-user.cpp
	g++ $^ -o $@

.PHONY: clean
clean:
	rm -rf $(TARGET)
