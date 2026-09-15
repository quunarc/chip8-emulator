set -euo pipefail

# haha colors
Color_Off='\033[0m'
Yellow='\033[0;33m'
Green='\033[0;32m'
Cyan='\033[0;36m'
Blue='\033[0;34m'
Red='\033[0;31m'
Magenta='\033[0;35m'

debug_flag="-DDEBUG_TEST=OFF"
build_type="Release"
clean_build=false
only_main=false
jobs=$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

# Help message goooo
show_usage() {
    echo -e "${Cyan}Usage: $0 [OPTIONS]${Color_Off}"
    echo -e "  ${Green}--debug-test${Color_Off}      Enable debug test mode"
    echo -e "  ${Green}--debug${Color_Off}           Build in Debug mode"
    echo -e "  ${Green}--release${Color_Off}         Build in Release mode (default)"
    echo -e "  ${Green}--clean${Color_Off}           Clean build directory before building"
    echo -e "  ${Green}-j, --jobs <N>${Color_Off}    Number of parallel jobs (default: $jobs)"
    echo -e "  ${Green}--help${Color_Off}            Show this help message"
    exit 0
}

run_only_main() {
    if [[ ! -f "./build/main" ]]; then
        echo "No binary exists, please build the project"
    else
        ./build/main
    fi
    exit 0
}

# Argument parser
while [ "$#" -gt 0 ]; do
    case "$1" in
        --debug-test)
            debug_flag="-DDEBUG_TEST=ON"
            ;;
        --debug)
            build_type="Debug"
            ;;
        --release)
            build_type="Release"
            ;;
        --clean)
            clean_build=true
            ;;
        -j|--jobs)
            if [ "$#" -lt 2 ]; then
                echo -e "${Red}Error: --jobs requires an argument${Color_Off}"
                exit 1
            fi
            jobs="$2"
            shift
            ;;
        --help|-h)
            show_usage
            ;;
        --binary|-b)
            run_only_main
            ;;
        *)
            echo -e "${Red}Unknown option: $1${Color_Off}"
            show_usage
            ;;
    esac
    shift
done

if [ "$clean_build" = true ] && [ -d "build" ]; then
    echo -e "${Yellow}Cleaning build directory...${Color_Off}"
    rm -rf build
fi

if [ -d "build" ]; then
    cd build
else
    echo -e "${Blue}Creating build directory...${Color_Off}"
    mkdir build
    cd build
fi

echo -e "${Magenta}═══════════════════════════════════════${Color_Off}"
echo -e "${Cyan}Build Configuration:${Color_Off}"
echo -e "  Build Type: ${Green}$build_type${Color_Off}"
echo -e "  Debug Test: ${Green}${debug_flag#-DDEBUG_TEST=}${Color_Off}"
echo -e "  Jobs: ${Green}$jobs${Color_Off}"
echo -e "${Magenta}═══════════════════════════════════════${Color_Off}\n"

# Start timer
SECONDS=0

echo -e "${Cyan}-- Configuring CMake...${Color_Off}"
if ! cmake .. "$debug_flag" -DCMAKE_BUILD_TYPE="$build_type"; then
    echo -e "${Red}XX CMake configuration failed${Color_Off}"
    exit 1
fi
echo -e "${Green}-- Configuration complete${Color_Off}\n"

# Build
echo -e "${Cyan}-- Building project...${Color_Off}"
if ! cmake --build . --parallel "$jobs"; then
    echo -e "${Red}XX Build failed${Color_Off}"
    exit 1
fi

duration=$SECONDS
echo -e "${Green}-- Build successful${Color_Off}"
echo -e "${Yellow}-- Time Taken: ${Green}$((duration / 60))m $((duration % 60))s${Color_Off}\n"

# Run executable
echo -e "${Cyan}-- Running executable...${Color_Off}"
echo -e "${Magenta}═══════════════════════════════════════${Color_Off}\n"
./main
exit_code=$?

echo -e "\n${Magenta}═══════════════════════════════════════${Color_Off}"
if [ $exit_code -eq 0 ]; then
    echo -e "${Green}-- Program completed successfully${Color_Off}"
else
    echo -e "${Red}XX Program exited with code $exit_code${Color_Off}"
fi

