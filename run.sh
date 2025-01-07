mkdir build
cd build

cmake ..

make

cd ..

if [[ -f build/server.exe && -f build/client.exe ]]; then
    rm ./server.exe
    rm ./client.exe
    mv build/server.exe .
    mv build/client.exe .
    echo "Executables moved to the root directory."
else
    echo "Error: One or both executables are missing."
    exit 1
fi

if [[ -f server.exe ]]; then
    echo "Running server.exe..."
    ./server.exe
else
    echo "server.exe not found."
fi