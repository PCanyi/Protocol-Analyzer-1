mkdir tmp && cd tmp && cmake .. && scan-build make
cd .. && rm -rf tmp
