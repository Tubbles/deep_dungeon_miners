#!/bin/bash

set -e

my_dir="$(dirname "$(realpath "${0}")")"
target_dir="${my_dir}/target"
app_name="$(grep -E '^project(.+)' CMakeLists.txt | sed -E 's/project\((.+)\)/\1/g')"

implicit_build=true
verbose=false
extra_verbose=false
clang=false
target="debug"
target_dir_target="${target_dir}/debug"
clean=false
build=false
lint=false
format=false
run=false

usage="Usage: ${0} [options]
where [options] can be zero or more of:
  ?,h,help,--help    display this help text and exit
  v,verbose          run more verbosely
  extra-verbose      run with all verbose text
  clang              build using clang instead of system default
  debug,release      build for debug (default) or release target
  clean              clean everything, does not build implicitly
  build              build, implicitly selected
  lint               run static analysis on the code
  format             format all source code, does not build implicitly
  run                run the application after a successful build, forces build
  --                 pass the following args to the program, if running"

while (($#)); do
case ${1} in
    \?|h|help|--help)
        echo "${usage}"
        exit 0
        ;;
    v|verbose)
        verbose=true
        ;;
    extra-verbose)
        verbose=true
        extra_verbose=true
        set -x
        ;;
    clang)
        clang=true
        ;;
    debug)
        target="debug"
        target_dir_target="${target_dir}/debug"
        ;;
    release)
        target="release"
        target_dir_target="${target_dir}/release"
        ;;
    clean)
        clean=true
        implicit_build=false
        ;;
    build)
        build=true
        ;;
    lint)
        lint=true
        implicit_build=false
        ;;
    format)
        # format=true
        implicit_build=false
        ;;
    run)
        build=true
        run=true
        ;;
    --)
        shift
        break
        ;;
    *)
        echo "Unknown argument: \"${1}\""
        echo "${usage}"
        exit 1
        ;;
esac
shift
done

cmake_debug=""
if [[ ${target} == "debug" ]]; then
    cmake_debug="-DCMAKE_BUILD_TYPE=Debug"
elif [[ ${target} == "release" ]]; then
    cmake_debug="-DCMAKE_BUILD_TYPE=Release"
fi

ninja_verbose=""
if [[ ${verbose} == true ]]; then
    ninja_verbose="-v"
fi

cmake_verbose=""
if [[ ${extra_verbose} == true ]]; then
    cmake_files=$(find "${my_dir}" \( -name CMakeLists.txt -or -name '*.cmake' \) -not -path '*/modules/*/*' -not -path '*/target/*' | paste -sd' ')
    cmake_verbose="--trace-expand "
    for file in ${cmake_files}; do
        cmake_verbose+="--trace-source=${file} "
    done
fi

if [[ ${implicit_build} == true ]]; then
    build=true
fi

clang-format --style=file -i "${my_dir}/src/"*.{cpp,hpp}

if [[ ${clean} == true ]]; then
    rm -fr "${target_dir}"
fi

if [[ ${clang} == true ]]; then
    export CC=/usr/bin/clang
    export CXX=/usr/bin/clang++
fi

if [[ ${build} == true ]]; then
    mkdir -p "${target_dir_target}"
    # shellcheck disable=2086,2248
    cmake ${cmake_verbose} -S "${my_dir}" -B "${target_dir_target}" -G Ninja ${cmake_debug}
    # shellcheck disable=2248
    cmake --build "${target_dir_target}" -- ${ninja_verbose}
fi

if [[ ${lint} == true ]]; then
      clang-tidy -p "${my_dir}/target/debug" "${my_dir}/src"/*.cpp
      clang-tidy -p "${my_dir}/target/debug" --config-file="${my_dir}/.clang-tidy-fixes" --fix --format-style=file "${my_dir}/src"/*.{cpp,hpp}
fi

if [[ ${run} == true ]]; then
    "${target_dir_target}/${app_name}" "$@"
fi
