BIN = ddsbenchmark
SRCS = \
	main.cpp \

PLATFORM=$(shell uname -m)

RTI_BASE = third_party/rti_connext/${PLATFORM}
RTI_INCLUDE = ${RTI_BASE}/include
RTI_LIB = ${RTI_BASE}/lib

RTI_LICENSE_FILE=${RTI_BASE}/../rti_license.dat
export RTI_LICENSE_FILE

RTI_DEFINES += \
	-DRTI_UNIX \

RTI_INCLUDES += \
	-I${RTI_INCLUDE} \
	-I${RTI_INCLUDE}/ndds \
	-I${RTI_INCLUDE}/ndds/hpp \

RTI_LIBS += \
	-L ${RTI_LIB} \
	-Wl,--no-as-needed \
	-Wl,-rpath,${RTI_LIB} \
	-lnddscored \
	-lnddscd \
	-lnddscppd \
	-lnddscpp2d \
	-Wl,--as-needed \

CXXFLAGS += \
	-Wall \
	-Wextra \
	-Werror \
	-O2 \
	-g \
	-std=c++14 \
	-pthread \
	${RTI_INCLUDES} \
	${RTI_DEFINES} \

LDFLAGS += \
	${RTI_LIBS} \
	-ldl \

all: ${BIN}
	./$<

${BIN}: ${SRCS:.cpp=.o} Makefile
	${LINK.cc} ${LDFLAGS} -o $@ $(filter %.o, $^)
