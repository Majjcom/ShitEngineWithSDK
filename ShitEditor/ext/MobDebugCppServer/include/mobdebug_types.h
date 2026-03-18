#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <optional>

namespace MobDebugCpp
{
    struct Breakpoint
    {
        std::string file;
        int line = 0;
    };

    struct StackFrame
    {
        int level = 0;
        std::string source;
        int currentLine = 0;
        std::string funcName;
        std::string funcType;
    };

    struct Variable
    {
        std::string name;
        std::string type;
        std::string value;
        bool expandable = false;
        int index = 0;
    };

    enum class DebuggerState
    {
        Disconnected,
        Connected,
        Paused,
        Running,
    };

    enum class PauseReason
    {
        Breakpoint,
        Step,
        Exception,
        Unknown,
    };

    struct PauseEvent
    {
        PauseReason reason = PauseReason::Unknown;
        std::string file;
        int line = 0;
    };
}
