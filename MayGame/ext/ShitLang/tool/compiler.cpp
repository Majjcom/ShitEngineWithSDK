
#include <iostream>
#include <filesystem>
#include <string>
#include <fstream>
#include <cstring>

#include "lua.hpp"

using namespace std;

struct LoaderData
{
    bool finished = false;
    char* data = nullptr;
    size_t size = 0;
};

static const char* code_loader(lua_State*, void* ud, size_t* sz)
{
    LoaderData* loader_data = static_cast<LoaderData*>(ud);
    if (loader_data->finished)
    {
        *sz = 0;
        return nullptr;
    }
    loader_data->finished = true;
    *sz = loader_data->size;
    return loader_data->data;
}

static int code_writer(lua_State*, const void *p, const size_t sz, void *ud)
{
    ofstream* pfp = static_cast<ofstream*>(ud);
    ofstream& fp = *pfp;
    fp.write((char*)p, (streamsize)sz);
    return 0;
}

int main(int argc, char* argv[])
{
    if (argc < 4)
    {
        cerr << "Argument Error. Useage: ShitCompiler path out_path dbg_path" << endl;
        return 0;
    }
    const string path = argv[1];
    const string out_path = argv[2];
    const string dbg_path = argv[3];
    if (!filesystem::exists(path))
    {
        cerr << "File Not Exists." << endl;
        return 1;
    }
    if (filesystem::is_directory(out_path))
    {
        cerr << "Out Path Is Directory." << endl;
        return 2;
    }
    ifstream fp;
    fp.open(path, ios::binary | ios::in);
    if (!fp.is_open())
    {
        cerr << "File Open Error." << endl;
        return 3;
    }
    fp.seekg(0, ios::end);
    size_t size = fp.tellg();
    fp.seekg(0, ios::beg);
    if (size == 0)
    {
        cerr << "File Size Error." << endl;
        return 4;
    }
    char* data = new char[size];
    fp.read(data, (streamsize)size);

    fp.close();
    {
        // UTF-8 BOM
        const char* bom_header = "\xEF\xBB\xBF";
        if (memcmp(bom_header, data, 3) == 0)
        {
            char* tmp = new char[size];
            memcpy(tmp, data, size);
            memcpy(data, tmp + 3, size - 3);
            size -= 3;
            delete[] tmp;
        }
    }

    lua_State* L = luaL_newstate();
    LoaderData loader_data{};
    loader_data.data = data;
    loader_data.size = size;

    if (lua_load(L, code_loader, &loader_data, ('@' + dbg_path).c_str(), "bt") != LUA_OK)
    {
        cerr << "Compile Error: " << lua_tostring(L, -1) << endl;
    }

    ofstream ofp;
    ofp.open(out_path, ios::binary | ios::out);

#if 1
    constexpr bool strip = false;
#else
    constexpr bool strip = true;
#endif
    lua_dump(L, code_writer, &ofp, strip);

    ofp.close();

    lua_close(L);

    delete[] data;
    return 0;
}
