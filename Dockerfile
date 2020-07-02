FROM jonstonchan/opencv4:latest
RUN apt-get update
RUN apt-get install -y clang-6.0
RUN apt-get -y install curl gnupg
RUN apt -y install python3-pip
RUN pip3 install flask Flask-Limiter
RUN pip3 install Pillow
RUN pip3 install requests
COPY . .
RUN g++ -std=c++11 logic.cpp -o ./a.out `pkg-config opencv4 --cflags --libs`
EXPOSE 80
CMD python3 server.py
