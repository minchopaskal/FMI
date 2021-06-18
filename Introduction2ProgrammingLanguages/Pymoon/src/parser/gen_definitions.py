#/usr/bin/python3

defs = []
with open("ast_definitions.fmt", "r") as f:
    l = f.readline()
    while l != '':
        if l.startswith("#"):
            l = f.readline()
            continue
        l = l.rstrip()
        defs.append(l.split(" "))
        l = f.readline()

with open("ast_definitions.hpp", "w") as f:
    f.write("#pragma once\n")
    f.write("#include \"../types.hpp\"\n")
    f.write("#include \"../lexer/token_types.hpp\"\n\n")
    f.write("#define EMPTY_BODY(MACRO)\n\n")
    for df in defs:
        if len(df) == 1:
            f.write("/* {type} */\n".format(type=df[0]))
            continue
        if len(df) == 2:
            continue
        f.write("#define {0}_NODE_MEMBERS(MACRO) \\\n".format(df[0].upper()))
        for i in range(2, len(df), 2):
            f.write("  MACRO({member}, {type}){slash}".format(member=df[i].capitalize(), type=df[i+1], \
                                                              slash=" \\\n" if i < len(df) - 2 else "\n"))
        f.write("\n")

    f.write("/* macro iterator */\n")
    f.write("#define AST_NODE_ITERATOR(MACRO) \\\n")
    with open("ast_fwd.hpp", "w") as fwd:
        fwd.write("#pragma once\n#include \"../types.hpp\"\n\n")
        for df in defs:
            if len(df) == 1: continue
            fwd.write("class {0};\n".format(df[1]))
            f.write("  MACRO({class_name}, {iterator}){slash}" \
                    .format(class_name=df[1], \
                            iterator="EMPTY_BODY" if len(df) == 2 else df[0] + "_NODE_MEMBERS", \
                            slash=" \\\n" if df != defs[-1] else "\n"))
    with open("node_visitor.hpp", "w") as nv:
        nv.write("#pragma once\n")
        nv.write("#include \"ast_fwd.hpp\"\n\n")
        nv.write("class NodeVisitor\n{\npublic:\n")
        nv.write("  virtual ~NodeVisitor() = default;\n\n")
        for df in defs:
            if len(df) == 1: continue
            nv.write("  virtual void Visit({0} *v) {{ }}\n".format(df[1]))
        nv.write("};\n")
