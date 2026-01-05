#!/bin/bash

# ==============================================================================
# Build Script for SecondAID-GUI
# ==============================================================================

set -e

# --- Configuration ---
PROJECT_ROOT="$(pwd)"
PROJECT_NAME="SecondAID-GUI"
BUILD_DIR_LINUX="$PROJECT_ROOT/build-linux"
BUILD_DIR_WINDOWS="$PROJECT_ROOT/build-windows"
RELEASE_DIR="$PROJECT_ROOT/releases"
TOOLS_DIR="$PROJECT_ROOT/tools"
MINGW_TOOLCHAIN="$PROJECT_ROOT/Toolchain-mingw.cmake"

# --- Colors ---
NC='\033[0m'
RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'

log_info()    { echo -e "${BLUE}[INFO]${NC} $1"; }
log_success() { echo -e "${GREEN}[SUCCESS]${NC} $1"; }
log_warn()    { echo -e "${YELLOW}[WARNING]${NC} $1"; }
log_error()   { echo -e "${RED}[ERROR]${NC} $1"; exit 1; }

# --- Dependency Checks ---
check_dependencies() {
    log_info "Checking build dependencies..."
    
    if ! command -v docker &> /dev/null; then
        log_error "Docker is not installed. Required for Linux build."
    fi

    if ! command -v x86_64-w64-mingw32-gcc &> /dev/null; then
        log_warn "MinGW not found. Windows cross-compilation will likely fail."
    fi

    if ! command -v zip &> /dev/null; then
        log_error "The 'zip' utility is not installed."
    fi

    if [ ! -f "$MINGW_TOOLCHAIN" ]; then
        log_error "MinGW toolchain file not found at: $MINGW_TOOLCHAIN"
    fi
}

# --- Initialization ---
prepare_workspace() {
    log_info "Cleaning up old releases..."
    rm -rf "$RELEASE_DIR"
    mkdir -p "$RELEASE_DIR"
}

# --- Linux Build (Docker) ---
build_linux() {
    log_info "Building for Linux (glibc compatibility mode)..."
    
    docker run --rm -v "$PROJECT_ROOT":/src -w /src ubuntu:20.04 bash -c "
        export DEBIAN_FRONTEND=noninteractive
        apt-get update && apt-get install -y \
            build-essential cmake git pkg-config \
            libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libgl1-mesa-dev \
            libwayland-dev wayland-protocols extra-cmake-modules libxkbcommon-dev \
            gcc-10 g++-10
        
        mkdir -p build-linux
        cd build-linux
        cmake .. -DCMAKE_BUILD_TYPE=Release \
                 -DCMAKE_CXX_COMPILER=g++-10 \
                 -DGLFW_BUILD_DOCS=OFF
        make -j$(nproc)
    "

    if [ -f "$BUILD_DIR_LINUX/$PROJECT_NAME" ]; then
        cp "$BUILD_DIR_LINUX/$PROJECT_NAME" "$RELEASE_DIR/${PROJECT_NAME}_linux"
        log_success "Linux binary: $RELEASE_DIR/${PROJECT_NAME}_linux"
    else
        log_error "Linux build failed: Output binary missing."
    fi
}

# --- Windows Build (MinGW) ---
build_windows() {
    log_info "Building for Windows (MinGW Cross-compile)..."
    
    mkdir -p "$BUILD_DIR_WINDOWS"
    # Use subshell to keep directory state clean
    (
        cd "$BUILD_DIR_WINDOWS"
        cmake .. -DCMAKE_TOOLCHAIN_FILE="$MINGW_TOOLCHAIN" \
                 -DCMAKE_BUILD_TYPE=Release
        make -j$(nproc)
    )

    if [ -f "$BUILD_DIR_WINDOWS/$PROJECT_NAME.exe" ]; then
        log_success "Windows binary built successfully."
    else
        log_error "Windows build failed: Output binary missing."
    fi
}

# --- Packaging ---
package_windows() {
    log_info "Creating Windows release bundle..."
    TEMP_WIN_DIR="$PROJECT_ROOT/win_release_temp"
    
    rm -rf "$TEMP_WIN_DIR"
    mkdir -p "$TEMP_WIN_DIR"

    # Copy binary
    cp "$BUILD_DIR_WINDOWS/$PROJECT_NAME.exe" "$TEMP_WIN_DIR/"

    # Copy tools
    if [ -f "$TOOLS_DIR/luacheck.exe" ]; then
        cp "$TOOLS_DIR/luacheck.exe" "$TEMP_WIN_DIR/"
        log_info "Integrated luacheck.exe into bundle."
    else
        log_warn "luacheck.exe not found in $TOOLS_DIR - skipping tool integration."
    fi

    # Create ZIP
    (
        cd "$TEMP_WIN_DIR"
        zip -q -r "$RELEASE_DIR/${PROJECT_NAME}_windows.zip" ./*
    )

    rm -rf "$TEMP_WIN_DIR"
    log_success "Windows bundle: $RELEASE_DIR/${PROJECT_NAME}_windows.zip"
}

# --- Execution ---

check_dependencies
prepare_workspace
build_linux
build_windows
package_windows

log_success "================================================="
log_success "Build Finished Successfully"
log_info "Final artifacts in $RELEASE_DIR:"
ls -lh "$RELEASE_DIR"
log_success "================================================="