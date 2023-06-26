FROM ubuntu:22.04

COPY out/Ubuntu/build/linux-debug/MayhemExample /

RUN chmod +x /MayhemExample
