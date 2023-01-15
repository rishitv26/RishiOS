cd src
echo IGNORE ANY ERRORS
make clean  # Ignore errors
echo DO NOT ANYMORE
make
cd ..
./update_image.sh
./run_bochs.sh