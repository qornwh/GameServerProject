#pragma once
#include "pch.h"
#include <boost/json.hpp>

#include <WinNls.h>

namespace GameUtils
{
    class JsonParser
    {
    public:
        static boost::json::value GetStrParser(String& jsonStr)
        {
            return boost::json::parse(jsonStr);
        }

        static boost::json::value Parser(String key, const boost::json::value& json)
        {
            // 그냥 바로 크래시 낸다 !!!
            CrashFunc(!json.at(key).is_null());

            return json.at(key);
        }
    };

    class TickCounter
    {
    public:
        TickCounter(int32 value) : _value(value)
        {
        }

        ~TickCounter()
        {
        }

        int32 Add(int32 value = 1)
        {
            _tick = (_tick + value) % _value;
            return _tick;
        }

        int32 GetTick()
        {
            return _tick;
        }

        int32 GetTickValue()
        {
            return _value;
        }

        void ResetTic(int32 value = -1)
        {
            _tick = value;
        }

    private:
        int32 _value;
        int32 _tick = 0;
    };

    class Utils
    {
    public:
        static wchar_t* CharToWchar(const char* cPtr)
        {
            int isize = strlen(cPtr);
            int nLength = MultiByteToWideChar(CP_ACP, 0, cPtr, isize, NULL, NULL);

            int nLen = sizeof(wchar_t) * (nLength + 1);
            wchar_t* wPtr = new wchar_t[nLen];
            memset(wPtr, 0, nLen);

            MultiByteToWideChar(CP_ACP, 0, cPtr, isize, wPtr, nLength);
            return wPtr;
        }

        static char* WcharToChar(const wchar_t* wPtr)
        {
            int nLength = WideCharToMultiByte(CP_ACP, 0, wPtr, -1, NULL, 0, NULL, NULL);

            char* cPtr = new char[nLength + 1];
            memset(cPtr, 0, nLength + 1);

            WideCharToMultiByte(CP_ACP, 0, wPtr, -1, cPtr, nLength, NULL, NULL);
            return cPtr;
        }
    };
}
