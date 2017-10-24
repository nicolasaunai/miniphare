# additional target to perform cppcheck run, requires cppcheck

# get all project files
# HACK this workaround is required to avoid qml files checking ^_^
file(GLOB_RECURSE ALL_SOURCE_FILES *.cpp *.h)


find_program(CPPCHECK cppcheck)

add_custom_target(
        cppcheck ALL
        COMMAND ${CPPCHECK}
        --enable=warning,performance,portability,information,missingInclude
        --std=c++11
        --template="[{severity}][{id}] {message} {callstack} \(On {file}:{line}\)"
        --verbose
        --quiet
        ${ALL_SOURCE_FILES}
)
