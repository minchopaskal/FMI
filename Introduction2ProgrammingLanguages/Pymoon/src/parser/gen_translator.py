from get_definitions import get_defs

defs = get_defs()

with open("translator.hpp", "w") as pv:
    pv.write("#pragma once\n")
    pv.write("#include \"node_visitor.hpp\"\n\n");
    pv.write("class Translator : public NodeVisitor\n{\npublic:\n")
    pv.write("  virtual ~Translator() = default;\n\n")
    for df in defs:
        if len(df) == 1: continue
        pv.write("  virtual void Visit({0} *v);\n".format(df[1]))
    pv.write("};\n")

with open("translator.cpp", "w") as pv:
    pv.write("#include \"translator.hpp\"\n\n");
    for df in defs:
        if len(df) == 1: continue
        pv.write("void Translator::Visit({0} *v)\n{{\n".format(df[1]))
        pv.write("  // NOT IMPLEMENTED\n")
        pv.write("}\n\n")
