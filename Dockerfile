FROM alpine:latest
# Setting variables
ENV PROJECT_NAME=My_own_cryptocurrency
ENV PROJECT_DIR=/home/${PROJECT_NAME}

CMD ["sh"]
RUN mkdir ${PROJECT_DIR}
COPY . ${PROJECT_DIR}
# Installing required tools
RUN apk --update add cmake make g++ vim git curl
# Moving working directory to project folder
WORKDIR ${PROJECT_DIR}
# Creatting library static library libcryptopp.a
RUN make -C cryptopp/ libcryptopp.a
# Generating makefiles for sqlite and project + building
#RUN cmake . -B ./linux-build/
#RUN cmake --build ./linux-build/

