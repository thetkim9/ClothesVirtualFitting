RUN apt-get update
RUN apt-get -y install curl gnupg
RUN mkdir /workspace
WORKDIR /workspace

RUN pip3 install tensorflow ipython tensorflow_hub

RUN cd /usr/local/cuda/lib64 \
 && mv stubs/libcuda.so ./ \
 && ln -s libcuda.so libcuda.so.1 \
 && ldconfig
ENV PATH=/usr/local/cuda/bin${PATH:+:${PATH}}
ENV LD_LIBRARY_PATH=/usr/local/cuda-9.0/lib64${LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}}
ENV CUDA_HOME=/usr/local/cuda

RUN mkdir /workspace/images
RUN pip3 install flask Flask-Limiter
COPY . .
EXPOSE 80
ENTRYPOINT python src/app.py

FROM cpp-build-base:0.1.0 AS build

WORKDIR /src

COPY CMakeLists.txt main.cpp ./

RUN cmake . && make

FROM ubuntu:bionic

WORKDIR /opt/hello-world

COPY --from=build /src/helloworld ./

EXPOSE 80
ENTRYPOINT python src/app.py

CMD ["./helloworld"]
