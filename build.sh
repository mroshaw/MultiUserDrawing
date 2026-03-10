#!/bin/bash
set -e  # Exit on error

echo "========================================="
echo "Building server..."
echo "========================================="
cd /cygdrive/e/Dev/MultiUserDrawing/mudserver && make
if [ $? -ne 0 ]; then
    echo "ERROR: Server build failed!"
    exit 1
fi

echo "========================================="
echo "Building client..."
echo "========================================="
cd /cygdrive/e/Dev/MultiUserDrawing/mudclient && make
if [ $? -ne 0 ]; then
    echo "ERROR: Client build failed!"
    exit 1
fi

echo "========================================="
echo "Build complete!"
echo "========================================="