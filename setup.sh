#!/bin/bash

# Exit immediately if a command exits with a non-zero status
set -e

# Function to check if a command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Step 1: Check and install Node.js (required for html-minifier-terser)
install_nodejs() {
    echo "Checking for Node.js..."

    if command_exists node; then
        echo "Node.js is already installed."
    else
        echo "Node.js not found. Installing Node.js..."
        if [[ "$OSTYPE" == "linux-gnu"* ]]; then
            # On Ubuntu/Debian
            sudo apt update
            sudo apt install -y nodejs npm
        elif [[ "$OSTYPE" == "darwin"* ]]; then
            # On macOS
            brew install node
        else
            echo "Unsupported OS. Please install Node.js manually."
            exit 1
        fi
    fi
}

# Step 2: Install html-minifier-terser globally using npm
install_html_minifier() {
    echo "Checking for html-minifier-terser..."

    if npm list -g html-minifier-terser >/dev/null 2>&1; then
        echo "html-minifier-terser is already installed."
    else
        echo "html-minifier-terser not found. Installing..."
        npm install -g html-minifier-terser
    fi
}

# Step 3: Check and install gzip (most Linux/macOS systems should already have it)
install_gzip() {
    echo "Checking for gzip..."

    if command_exists gzip; then
        echo "gzip is already installed."
    else
        echo "gzip not found. Installing gzip..."
        if [[ "$OSTYPE" == "linux-gnu"* ]]; then
            sudo apt update
            sudo apt install -y gzip
        elif [[ "$OSTYPE" == "darwin"* ]]; then
            brew install gzip
        else
            echo "Unsupported OS. Please install gzip manually."
            exit 1
        fi
    fi
}

# Step 4: Check and install xxd (often pre-installed)
install_xxd() {
    echo "Checking for xxd..."

    if command_exists xxd; then
        echo "xxd is already installed."
    else
        echo "xxd not found. Installing xxd..."
        if [[ "$OSTYPE" == "linux-gnu"* ]]; then
            sudo apt update
            sudo apt install -y xxd
        elif [[ "$OSTYPE" == "darwin"* ]]; then
            brew install xxd
        else
            echo "Unsupported OS. Please install xxd manually."
            exit 1
        fi
    fi
}

# Step 5: Check and install bash (likely already installed on most systems)
install_bash() {
    echo "Checking for bash..."

    if command_exists bash; then
        echo "bash is already installed."
    else
        echo "bash not found. Installing bash..."
        if [[ "$OSTYPE" == "linux-gnu"* ]]; then
            sudo apt update
            sudo apt install -y bash
        elif [[ "$OSTYPE" == "darwin"* ]]; then
            brew install bash
        else
            echo "Unsupported OS. Please install bash manually."
            exit 1
        fi
    fi
}

# Step 6: Run the installation functions
setup() {
    install_nodejs
    install_html_minifier
    install_gzip
    install_xxd
    install_bash
}

# Run the setup function
setup

echo "Setup completed successfully!"

# Optional: Instructions for running the build script
echo "Now you can run the 'build.sh' script to minify and compress your HTML."
