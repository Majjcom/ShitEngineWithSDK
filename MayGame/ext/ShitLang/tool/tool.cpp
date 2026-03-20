#include <iostream>
#include <filesystem>
#include <string>
#include <stack>

using namespace std;
namespace fs = std::filesystem;

int main(int argc, char* argv[])
{
    if (argc < 4)
    {
        cerr << "Argument Error. Useage: ShitCompilerTool compiler path out_path" << endl;
        return 0;
    }
    const string compiler = argv[1];
    const string path = argv[2];
    const string out_path = argv[3];

    stack<fs::directory_iterator> dir_stack;
    fs::directory_entry current_out_dir(out_path);
    fs::directory_entry rev_dir;
    dir_stack.push(fs::directory_iterator(path));
    while (!dir_stack.empty())
    {
        fs::directory_iterator& it = dir_stack.top();
        if (it == fs::directory_iterator())
        {
            dir_stack.pop();
            current_out_dir.assign(current_out_dir.path().parent_path());
            rev_dir.assign(rev_dir.path().parent_path());
            continue;
        }
        if (it->is_directory())
        {
            dir_stack.push(fs::directory_iterator(*it));
            current_out_dir.assign(current_out_dir.path() / it->path().filename());
            if (!current_out_dir.exists())
            {
                fs::create_directory(current_out_dir);
            }
            rev_dir.assign(rev_dir.path() / it->path().filename());
            ++it;
            continue;
        }
        if (it->path().extension() == ".shit")
        {
            fs::path file = it->path();
            fs::path out(file);
            out.replace_extension(".shit");
            fs::directory_entry out_d(current_out_dir.path() / out.filename());
            fs::path rev_path = rev_dir.path() / file.filename();
            cout << "Compiling " << file.filename().string() << " ..." << endl;
            system(
                (compiler + ' ' + it->path().string() + ' ' + out_d.path().string() + ' ' + rev_path.generic_string()).
                c_str());
        }
        ++it;
    }
    return 0;
}
