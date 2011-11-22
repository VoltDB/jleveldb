PLATFORM = $(shell uname)
JARNAME = jleveldb.jar
JNIBASENAME = libjleveldb
ifeq ($(PLATFORM),Darwin)
	JNILIBNAME = $(JNIBASENAME).jnilib
	USER_OBJS := libleveldb-osx.a
endif
ifeq ($(PLATFORM),Linux)
	JNILIBNAME = $(JNIBASENAME).so
	USER_OBJS := libleveldb-linux.a
endif

.PHONEY: all clean test

OBJS := NativeInterface.o

RM := rm -rf

# All Target
all: $(JARNAME) $(JNILIBNAME)

$(JARNAME): org/voltdb/leveldb/*.java
	@echo 'Building jleveldb Jarfile'
	javac org/voltdb/leveldb/*.java
	jar cf $(JARNAME) org/voltdb/leveldb/*.class org/voltdb/leveldb/*.java
	@echo ' '

org_voltdb_leveldb_NativeInterface.h: $(JARNAME)
ifneq ($(shell test -e $@ && echo exists),"exists")
	@echo 'Building jni header files'
	javah -d . org.voltdb.leveldb.NativeInterface
	@echo ' '
endif

NativeInterface.o: NativeInterface.cpp jni.h org_voltdb_leveldb_NativeInterface.h
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
ifeq ($(PLATFORM),Darwin)
	$(CCACHE) g++ -DDEBUG \
	-fmessage-length=0 -O3 -arch x86_64 \
	-isysroot /Developer/SDKs/MacOSX10.6.sdk \
	-mmacosx-version-min=10.6 \
    -I/usr/local/include -I. \
	-DMACOSX \
	-c -o $@ $<
endif
ifeq ($(PLATFORM),Linux)
	$(CCACHE) g++ \
	-g3 -O3 -mmmx -msse -msse2 -msse3 -fPIC \
	-DLINUX -I. \
	-c -o $@ $<
endif
	@echo 'Finished building: $<'
	@echo ' '

$(JNILIBNAME): $(OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
ifeq ($(PLATFORM),Darwin)
	MACOSX_DEPLOYMENT_TARGET="10.6" \
	g++ -arch x86_64 -dynamiclib -isysroot /Developer/SDKs/MacOSX10.6.sdk \
	$(USER_OBJS) $(OBJS) -lpthread \
	-mmacosx-version-min=10.6 -single_module \
	-compatibility_version 1 -current_version 1 \
	-o $@
endif
ifeq ($(PLATFORM),Linux)
	g++ -g3 -rdynamic -ldl -shared \
	-L/usr/lib/jvm/java-1.6.0-openjdk-1.6.0.0.x86_64/jre/lib/amd64/server \
	-L/usr/lib/jvm/java-1.6.0-openjdk-1.6.0.0.x86_64/jre/lib/amd64 \
	-L/usr/lib/jvm/java-1.6.0-openjdk-1.6.0.0.x86_64/jre/../lib/amd64 \
	-L/usr/lib/jvm/java-6-openjdk/jre/lib/amd64/server \
	-L/usr/lib/jvm/java-6-openjdk/jre/lib/amd64 \
	-L/usr/lib/jvm/java-6-openjdk/jre/../lib/amd64 \
	-L/opt/jdk1.6.0_18/jre/lib/amd64/server \
	-L/opt/jdk1.6.0_18/jre/lib/amd64 \
	-L/opt/jdk1.6.0_18/jre/../lib/amd64 \
	-L/usr/java/packages/lib/amd64 \
	-L/usr/lib64 -L/lib64 -L/lib -ljava -ljvm -lverify -lpthread \
	$^ $(USER_OBJS) \
	-o $@
endif
	@echo 'Finished building target: $@'
	@echo ' '

test:
	@echo 'Compiling and running JUnit tests'
	javac -cp jleveldb.jar:junit-4.8.2.jar tests/org/voltdb/leveldb/*.java
	cd tests && java -cp ../jleveldb.jar:../junit-4.8.2.jar:. \
		-Djava.library.path=.. org.junit.runner.JUnitCore org.voltdb.leveldb.TestNative
	@echo ' '

# Other Targets
clean:
	-$(RM) NativeInterface.o org_voltdb_leveldb_NativeInterface.h
	-$(RM) org/voltdb/leveldb/*.class $(JNIBASENAME).* jleveldb.jar
	-$(RM) tests/org/voltdb/leveldb/*.class
	-@echo ' '
