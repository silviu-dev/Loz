#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

void defineAst(string outputDir, string baseName, vector<string> types);
vector<string> splitString(const string &input, const char &delimiter);
void defineType(ofstream &writer, string baseName, string className, string fieldList);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cout << "Usage: generate_ast <output directory>";
        return -1;
    }

    string outputDir = argv[1];
    defineAst(outputDir, "Expr",
              {"Binary : Expr left, Token operator, Expr right", "Grouping : Expr expression", "Literal : Object value",
               "Unary : Token operator, Expr right"});
}

vector<string> splitString(const string &input, const char &delimiter)
{
    vector<string> strings;
    istringstream f(input);
    string s;
    while (getline(f, s, delimiter))
    {
        strings.push_back(s);
    }
    return strings;
}
void defineAst(string outputDir, string baseName, vector<string> types)
{
    string path = outputDir + "/" + baseName + ".hpp";
    ofstream writer(path);
    writer << "class " + baseName + " {};\n\n";
    // The AST classes.
    for (string type : types)
    {
        auto splitedString = splitString(type, ':');
        string className = splitedString[0];
        string fields = splitedString[1];
        defineType(writer, baseName, className, fields);
    }
}

void defineType(ofstream &writer, string baseName, string className, string fieldList)
{
    writer << "silviu " + className + " : public " + baseName + "\n{\n";
    // Constructor.
    writer << className + "(" + fieldList + ")\n{\n";
    // Store parameters in fields.
    auto fields = splitString(fieldList, ',');
    for (string field : fields)
    {
        string name = splitString(field, ' ')[1];
        writer << "this->" << name << " = " << name << ";\n";
    }
    writer << ("}\n");
    // Fields.
    for (string field : fields)
    {
        writer << field << ";\n";
    }
    writer << "};\n\n";
}