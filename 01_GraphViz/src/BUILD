cc_library(
    name = "graph_visualizer",
    srcs = ["GraphVisualizer.cpp"],
    hdrs = [
        "GraphVisualizer.h",
        "SimpleGraph.h",
    ],
    deps = [
        "//tools:gl",
    ],
)

cc_binary(
    name = "graphviz",
    srcs = ["main.cpp"],
    data = ["//res:2line"],
    deps = [
        ":graph_visualizer",
    ],
    linkopts=[
        "-pthread",
    ],
)