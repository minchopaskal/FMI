from get_definitions import get_defs

defs = get_defs()

with open("print_visitor.hpp", "w") as pv:
    pv.write("#pragma once\n")
    pv.write("#include \"node_visitor.hpp\"\n\n");
    pv.write("class PrintVisitor : NodeVisitor\n{\npublic:\n")
    pv.write("  virtual ~PrintVisitor() = default;\n\n")
    for df in defs:
        if len(df) == 1: continue
        pv.write("  virtual void Visit({0} *v);\n".format(df[1]))
    pv.write("};\n")

with open("print_visitor.cpp", "w") as pv:
    pv.write("#include <iostream>\n");
    pv.write("#include \"print_visitor.hpp\"\n\n");
    for df in defs:
        if len(df) == 1: continue
        pv.write("void PrintVisitor::Visit({0} *v)\n{{\n".format(df[1]))
        pv.write("  std::cout << \"{0}\" << std::endl;\n".format(df[1]))
        pv.write("}\n\n")
